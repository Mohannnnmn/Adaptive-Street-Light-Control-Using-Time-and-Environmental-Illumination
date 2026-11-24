
#include <lpc21xx.h>
#include "types.h"
#include "defines.h"
#include "kpm_defines.h"
#include "LCD.h"
#include "LCD_DEFINES.h"
#include "delay.h"

/*******************************************************************************
 * File Name    : KPM.c
 * Description  : 4x4 Matrix Keypad Interfacing and Numeric Input Handling.
 *
 * Details:
 * 1. Configures the GPIO pins for keypad row scanning.
 * 2. Detects key presses by scanning rows and columns.
 * 3. Identifies the pressed key using a keypad lookup table.
 * 4. Reads numeric input from the keypad and displays it on the LCD.
 * 5. Supports backspace and clear operations during numeric entry.
 ******************************************************************************/

/* External variable used for external interrupt-related functionality. */
extern u32 external_int;

/* Lookup table mapping keypad row and column positions to their key values. */
u8 kpmlut [4][4]={{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};


/*******************************************************************************
 * Function Name : Initkpm
 * Description   : Configures the keypad row pins as GPIO output pins.
 * Parameters    : None
 * Return Value  : None
 ******************************************************************************/
void Initkpm (void)
{
	/* Configure all four keypad row pins as output pins. */
	IODIR1 |= 1<<ROW0;
  IODIR1 |= 1<<ROW1;
	IODIR1 |= 1<<ROW2;
	IODIR1 |= 1<<ROW3;
}


/*******************************************************************************
 * Function Name : colscan
 * Description   : Checks the logic levels of all four keypad column pins.
 *                 Determines whether any key is currently pressed.
 * Parameters    : None
 * Return Value  : Returns the combined column pin status.
 *                 Returns 0 when a key press is detected.
 ******************************************************************************/
u8 colscan (void)
{
	u8 t;

	/* Read and combine the status of all four column pins. */
	t=READBIT(IOPIN1,COL0)&
	  READBIT(IOPIN1,COL1)&
	  READBIT(IOPIN1,COL2)&
	  READBIT(IOPIN1,COL3);

	return t;
}


/*******************************************************************************
 * Function Name : rowcheck
 * Description   : Identifies the keypad row containing the pressed key.
 *                 Each row is activated individually and the column status
 *                 is checked to determine the corresponding row.
 * Parameters    : None
 * Return Value  : Returns the detected row index (0 to 3).
 ******************************************************************************/
u8 rowcheck (void)
{
	u8 r;

	/* Activate ROW0 and check whether a key is pressed in this row. */
	IOCLR1 = 1<<ROW0;
	IOSET1 = 1<<ROW1;
	IOSET1 = 1<<ROW2;
	IOSET1 = 1<<ROW3;

	if(colscan()==0)
	{
		r=0;
		goto last;
	}

	/* Activate ROW1 and check whether a key is pressed in this row. */
    IOSET1 = 1<<ROW0;
	IOCLR1 = 1<<ROW1;
	IOSET1 = 1<<ROW2;
	IOSET1 = 1<<ROW3;

	if(colscan()==0)
	{
		r=1;
		goto last;
	}

	/* Activate ROW2 and check whether a key is pressed in this row. */
	IOSET1 = 1<<ROW0;
	IOSET1 = 1<<ROW1;
	IOCLR1 = 1<<ROW2;
	IOSET1 = 1<<ROW3;

	if(colscan()==0)
	{
		r=2;
		goto last;
	}

	/* Activate ROW3 and check whether a key is pressed in this row. */
    IOSET1 = 1<<ROW0;
	IOSET1 = 1<<ROW1;
	IOSET1 = 1<<ROW2;
	IOCLR1 = 1<<ROW3;

	if(colscan()==0)
	{
		r=3;
		goto last;
	}

last:
	/* Restore all keypad row pins to their low logic state. */
    IOCLR1 = 1<<ROW0;
	IOCLR1 = 1<<ROW1;
	IOCLR1 = 1<<ROW2;
	IOCLR1 = 1<<ROW3;

	return r;
}


/*******************************************************************************
 * Function Name : colcheck
 * Description   : Identifies the keypad column containing the pressed key.
 *                 Checks the column pins sequentially to locate the active
 *                 column.
 * Parameters    : None
 * Return Value  : Returns the detected column index (0 to 3).
 ******************************************************************************/
u8 colcheck (void)
{
        u8 c;

        /* Check each column pin and identify the active column. */
        if(READBIT(IOPIN1,COL0)==0)
        c=0;
        else if(READBIT(IOPIN1,COL1)==0)
        c=1;
        else if(READBIT(IOPIN1,COL2)==0)
        c=2;
        else if(READBIT(IOPIN1,COL3)==0)
        c=3;

        return c;
}


/*******************************************************************************
 * Function Name : keyscan
 * Description   : Detects a keypad key press and determines its key value.
 *
 *                 The function waits for a key press, identifies its row
 *                 and column positions, and retrieves the corresponding
 *                 character from the keypad lookup table.
 *
 * Parameters    : None
 * Return Value  : Returns the character corresponding to the pressed key.
 ******************************************************************************/
u8 keyscan (void)
{
        u8 r,c,keyV;

        /* Wait until a key press is detected on any column. */
        while(colscan());

        /* Identify the row in which the key is pressed. */
        r= rowcheck();

        /* Identify the column in which the key is pressed. */
        c=colcheck();

        /* Retrieve the corresponding key value from the lookup table. */
        keyV=kpmlut[r][c];

        return keyV;
}


/*******************************************************************************
 * Function Name : ReadNum
 * Description   : Reads a multi-digit decimal number from the keypad and
 *                 displays the entered value on the LCD.
 *
 *                 Supported operations:
 *                 1. Numeric keys (0-9) append digits to the number.
 *                 2. Key 'B' performs a backspace operation.
 *                 3. Key '*' clears the LCD display.
 *                 4. Any other non-numeric key terminates numeric entry.
 *
 * Parameters    : None
 * Return Value  : Returns the numeric value entered by the user.
 ******************************************************************************/
u32 ReadNum (void)
{
	u32 keyV,sum=0;

	while(1)
	{
		/* Read the next key pressed on the keypad. */
		keyV=keyscan();

		/* Apply a delay for keypad input stabilization. */
		delay_ms(300);

		/* Check whether the pressed key is a numeric digit. */
		if((keyV>='0') && (keyV<='9'))
		{
			/* Append the newly entered digit to the accumulated number. */
			sum=(sum*10)+(keyV-'0');

			/* Clear the LCD and display the updated numeric value. */
			Cmdlcd(CLEAR_LCD);
			u32lcd(sum);

			/* Wait until the currently pressed key is released. */
			while((colscan()==0));
		}
		else if(keyV=='B')
		{
			/* Remove the last entered digit using integer division. */
		sum=sum/10;

			/* Refresh the LCD with the updated numeric value. */
		Cmdlcd(CLEAR_LCD);

		if(sum)
		{
			u32lcd(sum);
		}
		}
		else
		{
			/* Clear the LCD when the '*' key is pressed. */
			if(keyV=='*')
			{
				Cmdlcd(CLEAR_LCD);
			}

			/* Wait for key release before terminating numeric entry. */
			while(colscan()==0);

			break;
		}
	}

	return sum;
}


/*******************************************************************************
 * Function Name : ReadNum2
 * Description   : Reads a multi-digit decimal number from the keypad and
 *                 stores the entered value through a pointer.
 *
 *                 The function continuously accepts numeric digits,
 *                 updates the LCD display, and terminates input when
 *                 a non-numeric key is pressed.
 *
 * Parameters    :
 *                 num     - Pointer to the variable storing the entered number.
 *                 lastkey - Pointer to the variable storing the last key pressed.
 *
 * Return Value  : None
 ******************************************************************************/
void ReadNum2(u32 *num,u8 *lastkey)
{
	while(1)
	{
		/* Read the pressed key and store it through the lastkey pointer. */
		*lastkey=keyscan();

		/* Check whether the pressed key is a numeric digit. */
		if((*lastkey>='0') && (*lastkey<='9'))
		{
			/* Append the newly entered digit to the existing number. */
			*num=(*num*10)+(*lastkey-'0');

			/* Position the LCD cursor at the beginning of the second line. */
			Cmdlcd(GOTO_LINE2_POS0);

			/* Display the updated numeric value on the LCD. */
			u32lcd(*num);

			/* Wait until the currently pressed key is released. */
			while(colscan()==0);
		}
		else 
		{
			/* Clear the LCD when the 'c' key is pressed. */
			if(*lastkey=='c')
			{
				Cmdlcd(CLEAR_LCD);
			}

			/* Wait for key release before terminating numeric entry. */
			while(colscan()==0);

			break;
		}
	}
}

