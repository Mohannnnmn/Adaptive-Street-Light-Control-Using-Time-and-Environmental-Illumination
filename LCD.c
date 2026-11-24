
/*******************************************************************************
 * File Name    : LCD.c
 * Description  : LCD Interfacing Functions for LPC2129/LPC21xx Microcontroller.
 *
 * Description:
 * 1. Initializes the LCD in 8-bit, 2-line mode.
 * 2. Sends commands and data to the LCD.
 * 3. Displays strings, integers, signed integers and floating-point values.
 * 4. Supports creation of user-defined characters using CGRAM.
 ******************************************************************************/

#include <LPC21xx.h>
#include "delay.h"
#include "types.h"
#include "defines.h"
#include "LCD_DEFINES.h"
#include "LCD.h"

/*******************************************************************************
 * Function Name : Writelcd
 * Description   : Sends 8-bit data or command to the LCD.
 * Parameters    : dat - 8-bit data to be sent to the LCD.
 * Return Value  : None.
 * Description   :
 *                 Writes the given byte to the LCD data pins.
 *                 Generates an enable pulse to latch the data into the LCD.
 ******************************************************************************/
void Writelcd(u8 dat)
{
	WRITEBYTE(IOPIN0,LCD_DATA,dat);
	IOSET0=1<<LCD_EN;
	delay_us(1);
	IOCLR0=1<<LCD_EN;
	delay_ms(2);
}

/*******************************************************************************
 * Function Name : Cmdlcd
 * Description   : Sends a command instruction to the LCD.
 * Parameters    : cmd - Command to be executed by the LCD.
 * Return Value  : None.
 * Description   :
 *                 Selects the instruction register by clearing RS.
 *                 Calls Writelcd() to send the command to the LCD.
 ******************************************************************************/
void Cmdlcd(u8 cmd)
{
	IOCLR0=1<<LCD_RS;
	Writelcd(cmd);
}

/*******************************************************************************
 * Function Name : Charlcd
 * Description   : Sends a character or data byte to the LCD.
 * Parameters    : asciival - ASCII value of the character to be displayed.
 * Return Value  : None.
 * Description   :
 *                 Selects the data register by setting RS.
 *                 Calls Writelcd() to send the character to the LCD.
 ******************************************************************************/
void Charlcd(u8 asciival)
{
	IOSET0=1<<LCD_RS;
	Writelcd(asciival);
}

/*******************************************************************************
 * Function Name : Initlcd
 * Description   : Initializes the LCD in 8-bit, 2-line display mode.
 * Parameters    : None.
 * Return Value  : None.
 * Description   :
 *                 Configures LCD data and control pins as output pins.
 *                 Performs the required initialization sequence.
 *                 Configures display ON, cursor OFF and clears the display.
 *                 Sets the cursor movement direction to right.
 ******************************************************************************/
void Initlcd(void)
{
	IODIR0|=(0XFF<<LCD_DATA)|(1<<LCD_RS)|(1<<LCD_RW)|(1<<LCD_EN);
	delay_ms(15);
	Cmdlcd(0x30);
	delay_ms(4);
	delay_us(100);
	Cmdlcd(0x30);
	delay_us(100);
	Cmdlcd(0x30);
	Cmdlcd(MODE_8BIT_2LINE);
	Cmdlcd(DSP_ON_CUR_OFF);
	Cmdlcd(CLEAR_LCD);
	Cmdlcd(SHIFT_CUR_RIGHT);
	
}

/*******************************************************************************
 * Function Name : Strlcd
 * Description   : Displays a null-terminated string on the LCD.
 * Parameters    : s - Pointer to the string to be displayed.
 * Return Value  : None.
 * Description   :
 *                 Reads each character from the string and sends it to the LCD.
 *                 Continues displaying characters until the null character
 *                 is encountered.
 ******************************************************************************/
void Strlcd(s8 *s)
{
	while(*s)
		Charlcd(*s++);
}

/*******************************************************************************
 * Function Name : u32lcd
 * Description   : Displays an unsigned 32-bit integer on the LCD.
 * Parameters    : n - Unsigned integer value to be displayed.
 * Return Value  : None.
 * Description   :
 *                 Extracts individual digits using the modulo operator.
 *                 Stores the digits in reverse order in an array.
 *                 Displays the digits in the correct order.
 ******************************************************************************/
void u32lcd(u32 n)
{
	u8 a[10];
	s32 i=0;
	if(n==0)
	{
		Charlcd('0');
	}
	else
	{
		while(n>0)
		{
			a[i]=(n%10)+48;;
			n/=10;
			i++;
		}
		for(--i;i>=0;i--)
		Charlcd(a[i]);
	}
}

/*******************************************************************************
 * Function Name : s32lcd
 * Description   : Displays a signed 32-bit integer on the LCD.
 * Parameters    : sn - Signed integer value to be displayed.
 * Return Value  : None.
 * Description   :
 *                 Checks whether the given number is negative.
 *                 Displays the minus sign for negative values.
 *                 Converts the number to its positive equivalent and calls
 *                 u32lcd() to display the numerical value.
 ******************************************************************************/
void s32lcd(s32 sn)
{
	if(sn<0)
	{
		Charlcd('-');
		sn=-sn;
	}
	u32lcd(sn);
}

/*******************************************************************************
 * Function Name : f32lcd
 * Description   : Displays a floating-point number on the LCD.
 * Parameters    :
 *                 FN  - Floating-point value to be displayed.
 *                 Ndp - Number of digits to be displayed after the decimal.
 * Return Value  : None.
 * Description   :
 *                 Checks for a negative value and displays the minus sign.
 *                 Extracts and displays the integer part of the number.
 *                 Displays the decimal point.
 *                 Extracts and displays the required number of fractional
 *                 digits based on Ndp.
 ******************************************************************************/
void f32lcd(f32 FN,u8 Ndp)
{
	u32 n,i;
	if(FN<0)
	{
		Charlcd('-');
		FN=-FN;
	}
	n=FN;
	u32lcd(n);
	Charlcd('.');
	for(i=0;i<Ndp;i++)
	{
		FN=(FN-n)*10;
		n=FN;
		Charlcd(n+48);
	}
}

/*******************************************************************************
 * Function Name : BuildCGRAM
 * Description   : Creates user-defined characters in the LCD CGRAM.
 * Parameters    :
 *                 p      - Pointer to the character pattern data.
 *                 nBytes - Number of pattern bytes to be written.
 * Return Value  : None.
 * Description   :
 *                 Sets the LCD address to the beginning of CGRAM.
 *                 Selects the data register and clears the read/write control
 *                 signal to enable writing.
 *                 Writes the given character pattern bytes into CGRAM.
 *                 Returns the LCD cursor to the first position of line 1.
 ******************************************************************************/
void BuildCGRAM(u8 *p,u8 nBytes)
{
	u32 i;
	Cmdlcd(GOTO_CGRAM_START);
	IOSET0=1<<LCD_RS;
	IOCLR0=1<<LCD_RW;
	for(i=0;i<nBytes;i++)
	{
		Writelcd(p[i]);
	}
	Cmdlcd(GOTO_LINE1_POS0);
}
