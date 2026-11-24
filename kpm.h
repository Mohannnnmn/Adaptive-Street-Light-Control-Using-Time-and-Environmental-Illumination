/*******************************************************************************
 * File Name    : kpm.h
 * Description  : Contains function prototypes for interfacing and
 *                handling a 4x4 Matrix Keypad with the LPC2129.
 *
 * Details:
 * This header file provides function declarations for keypad
 * initialization, row and column scanning, key detection and
 * reading numeric values entered through the keypad.
 ******************************************************************************/

#include "types.h"

/* Function prototype to initialize the matrix keypad GPIO pins. */
void Initkpm(void);

/* Function prototype to scan the keypad columns and identify the active column. */
u8 colscan (void);

/* Function prototype to check the keypad rows and identify the pressed row. */
u8 rowcheck (void);

/* Function prototype to check the keypad columns and identify the pressed column. */
u8 colcheck (void);

/* Function prototype to scan the keypad and identify the pressed key. */
u8 keyscan (void);

/* Function prototype to read a multi-digit numeric value from the keypad. */
u32 ReadNum (void);

/* Function prototype to read a numeric value and store the result and digit count
   through the provided pointers. */
void ReadNum2 (u32*,u8*);
