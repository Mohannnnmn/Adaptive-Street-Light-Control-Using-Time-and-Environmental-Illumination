/*******************************************************************************
 * File Name    : LCD_defines.h
 * Description  : Defines HD44780 LCD commands and GPIO pin assignments
 *                used for interfacing the LCD with the LPC2129.
 *
 * Details:
 * This header file contains command macros for LCD initialization,
 * cursor control, display control, display shifting, operating modes,
 * DDRAM and CGRAM addressing, and LCD control/data pin definitions.
 ******************************************************************************/

/* LCD command to clear the display and move the cursor to the home position. */
#define CLEAR_LCD               0x01

/* LCD command to return the cursor to the home position. */
#define RET_CUR_HOME            0x02

/* LCD command to shift the cursor position to the right. */
#define SHIFT_CUR_RIGHT         0x06

/* LCD command to shift the cursor position to the left. */
#define SHIFT_CUR_LEFT		  		0x07

/* LCD command to turn OFF the display. */
#define DSP_OFF			    			  0x08

/* LCD command to turn ON the display and turn OFF the cursor. */
#define DSP_ON_CUR_OFF				  0x0c

/* LCD command to turn ON the display and cursor without blinking. */
#define DSP_ON_CUR_ON		 			 0x0e

/* LCD command to turn ON the display with a blinking cursor. */
#define DSP_ON_CUR_BLK 		 		 0x0f

/* LCD command to shift the entire display to the left. */
#define SHIFT_DSP_LEFT          0x10

/* LCD command to shift the entire display to the right. */
#define SHIFT_DSP_RIGHT				  0x14

/* LCD command to configure the LCD in 8-bit mode with one display line. */
#define MODE_8BIT_1LINE		  0x30

/* LCD command to configure the LCD in 4-bit mode with one display line. */
#define MODE_4BIT_1LINE 		  0x20

/* LCD command to configure the LCD in 8-bit mode with two display lines. */
#define MODE_8BIT_2LINE         0x38

/* LCD command to configure the LCD in 4-bit mode with two display lines. */
#define MODE_4BIT_2LINE 		  0x28

/* LCD command to set the cursor at the beginning of the first display line. */
#define GOTO_LINE1_POS0         0x80

/* LCD command to set the cursor at the beginning of the second display line. */
#define GOTO_LINE2_POS0		  0xc0

/* LCD command to set the cursor at the beginning of the third display line. */
#define GOTO_LINE3_POS0        0x94

/* LCD command to set the cursor at the beginning of the fourth display line. */
#define GOTO_LINE4_POS0		 		 0xd4

/* LCD command to select the starting address of CGRAM for custom characters. */
#define GOTO_CGRAM_START        0x40

/* Defines the starting GPIO pin number for the LCD data connections. */
#define LCD_DATA	          		8

/* Defines the GPIO pin number connected to the LCD Register Select (RS) pin. */
#define LCD_RS                  16

/* Defines the GPIO pin number connected to the LCD Read/Write (RW) pin. */
#define LCD_RW                  18

/* Defines the GPIO pin number connected to the LCD Enable (EN) pin. */
#define LCD_EN                  17
