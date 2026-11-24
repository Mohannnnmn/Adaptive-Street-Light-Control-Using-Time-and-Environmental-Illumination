/*******************************************************************************
 * File Name    : softdelay.c
 * Description  : Software delay functions for generating delays in
 *                microseconds, milliseconds and seconds.
 *
 * Description:
 * 1. Provides delay functions using software-based looping.
 * 2. Generates approximate delays based on the processor clock frequency.
 * 3. Supports delays in microseconds, milliseconds and seconds.
 ******************************************************************************/

#include "types.h"

/*******************************************************************************
 * Function Name : delay_us
 * Description   : Generates a delay in microseconds using a software loop.
 * Parameter     : dlyUS - Delay duration in microseconds.
 * Return Value  : None
 ******************************************************************************/
void delay_us(s32 dlyUS)
{
    /* Executes the loop for the required number of microsecond delay cycles */
    for(dlyUS*=12; dlyUS>0; dlyUS--);
}

/*******************************************************************************
 * Function Name : delay_ms
 * Description   : Generates a delay in milliseconds using a software loop.
 * Parameter     : dlyMS - Delay duration in milliseconds.
 * Return Value  : None
 ******************************************************************************/
void delay_ms(s32 dlyMS)
{
    /* Executes the loop for the required number of millisecond delay cycles */
    for(dlyMS*=12000; dlyMS>0; dlyMS--);
}

/*******************************************************************************
 * Function Name : delay_s
 * Description   : Generates a delay in seconds using a software loop.
 * Parameter     : dlyS - Delay duration in seconds.
 * Return Value  : None
 ******************************************************************************/
void delay_s(s32 dlyS)
{
    /* Executes the loop for the required number of second delay cycles */
    for(dlyS*=10000000; dlyS>0; dlyS--);
}
