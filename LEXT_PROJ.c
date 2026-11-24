
/*******************************************************************************
 * File Name    : ext_proj.c
 * Description  : Configuration and handling of External Interrupt 0 (EINT0)
 *                for updating RTC time, date and street light schedules.
 *
 * Functional Description:
 * 1. Configures the EINT0 pin and its interrupt triggering mode.
 * 2. Enables EINT0 in the Vector Interrupt Controller (VIC).
 * 3. Handles the external interrupt using an interrupt service routine.
 * 4. Provides a menu-driven interface to configure RTC and street light timings.
 *
 * Interrupt Operation:
 * When EINT0 is triggered, the ISR sets a flag to indicate the interrupt
 * occurrence. The main program uses this flag to enter the configuration
 * menu and allows the user to modify the required settings.
 ******************************************************************************/

#include "header_files.h"
#include "header.h"
#include "EXT_defines.h"

/*******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/

/* Flag used to indicate the occurrence of External Interrupt 0. */
volatile u32 external_int = 0, flag = 0;

/* External variables used to store street light ON and OFF timings. */
extern s32 start_hour, end_hour, start_min, end_min;


/*******************************************************************************
 * Function Name : external_ISR
 * Description   : Interrupt Service Routine for External Interrupt 0 (EINT0).
 *
 * Operation:
 * 1. Sets external_int to indicate that an external interrupt has occurred.
 * 2. Clears the pending EINT0 interrupt request using the EXTINT register.
 * 3. Resets VICVectAddr to indicate the completion of interrupt servicing.
 *
 * Return Value  : None
 ******************************************************************************/

void external_ISR(void) __irq
{
    /* Set the flag to indicate the occurrence of EINT0. */
    external_int = 1;

    /* Clear the pending External Interrupt 0 interrupt request. */
    EXTINT = EXT0_INT_CLR;

    /* Signal the VIC that the interrupt service routine has completed. */
    VICVectAddr = 0;
}


/*******************************************************************************
 * Function Name : set_external_interrupt
 * Description   : Initializes and enables External Interrupt 0 (EINT0).
 *
 * Operation:
 * 1. Configures the corresponding microcontroller pin for EINT0 functionality.
 * 2. Clears any previously pending EINT0 interrupt request.
 * 3. Configures EINT0 to operate in edge-sensitive triggering mode.
 * 4. Selects IRQ mode for the corresponding VIC interrupt channel.
 * 5. Enables the EINT0 interrupt channel in the VIC.
 * 6. Configures VIC vector slot 3 and assigns the EINT0 interrupt channel.
 * 7. Registers the external_ISR function as the interrupt service routine.
 *
 * Return Value  : None
 ******************************************************************************/

void set_external_interrupt(void)
{
    /* Configure the corresponding pin for External Interrupt 0 functionality. */
    PINSEL1 &= ~(3 << ((20-16) * 2));
    PINSEL1 |= PIN_EXT_FUNC;

    /* Clear any previously pending External Interrupt 0 request. */
    EXTINT = EXT0_INT_CLR;

    /* Configure External Interrupt 0 for edge-sensitive triggering. */
    EXTMODE = EXT0_MODE_EDG_SEN;

    /* Select IRQ mode for the External Interrupt 0 VIC channel. */
    VICIntSelect &= ~(1 << EXT0_CHAN_NO);

    /* Enable the External Interrupt 0 channel in the VIC. */
    VICIntEnable |= (1 << EXT0_CHAN_NO);

    /* Enable VIC vector slot 3 and assign the EINT0 interrupt channel. */
    VICVectCntl3 = 0x20 | EXT0_CHAN_NO;

    /* Assign the External Interrupt 0 ISR to VIC vector slot 3. */
    VICVectAddr3 = (unsigned int)external_ISR;
}


/*******************************************************************************
 * Function Name : external_int_occur
 * Description   : Displays the configuration menu when an external interrupt
 *                 occurs and allows the user to update RTC and street light
 *                 scheduling parameters.
 *
 * User Options:
 * 1. TIME     - Configure RTC hours, minutes and seconds.
 * 2. DATE     - Configure RTC year, month, date and day of the week.
 * 3. LI_TIME  - Configure street light ON and OFF timings.
 * 4. QUIT     - Exit the configuration menu.
 *
 * Operation:
 * 1. Displays the configuration options on the LCD.
 * 2. Reads the user's selection through the keypad.
 * 3. Calls the corresponding function based on the selected option.
 * 4. Displays an error message for an invalid selection.
 * 5. Terminates the menu when the user selects the QUIT option.
 *
 * Return Value  : None
 ******************************************************************************/

void external_int_occur(void)
{
    /* Variables used to store the menu selection and exit status. */
    s32 choice1=0,quitflag = 0;

    /* Continuously display the menu until the user selects QUIT. */
    while (1)
    {
        /* Clear the LCD and display the available configuration options. */
        Cmdlcd(CLEAR_LCD);

        /* Display the TIME configuration option on the first line. */
        Cmdlcd(GOTO_LINE1_POS0);
        Strlcd("1.TIME");

        /* Display the DATE configuration option on the first line. */
        Cmdlcd(GOTO_LINE1_POS0 + 8);
        Strlcd("2.DATE");

        /* Display the street light timing option on the second line. */
        Cmdlcd(GOTO_LINE2_POS0);
        Strlcd("3.LI_TIME");

        /* Display the QUIT option on the second line. */
        Cmdlcd(GOTO_LINE2_POS0 + 10);
        Strlcd("4.QUIT");

        /* Read the menu option entered by the user. */
        choice1 = ReadNum();

        /* Execute the operation corresponding to the selected option. */
        switch (choice1)
        {
            /*******************************************************************
             * CASE 1: RTC TIME CONFIGURATION
             *
             * Calls set_time() to configure the RTC time.
             * The function handles the input and updating of time parameters.
             *******************************************************************/

            case 1:
            {
                set_time();
            }
            break;


            /*******************************************************************
             * CASE 2: RTC DATE CONFIGURATION
             *
             * Calls set_date() to configure the RTC date.
             * The function handles the input and updating of date parameters.
             *******************************************************************/

            case 2:
            {
                set_date();
            }
            break;


            /*******************************************************************
             * CASE 3: STREET LIGHT TIMING CONFIGURATION
             *
             * Calls set_lights_time() to configure the street light schedule.
             * The function updates the ON and OFF timing parameters used by
             * the RTC-based street light control mechanism.
             *******************************************************************/

            case 3:
            {
                set_lights_time();
            }
            break;


            /*******************************************************************
             * CASE 4: EXIT CONFIGURATION MENU
             *
             * Sets quitflag to 1 to indicate that the user wants to exit
             * the configuration menu.
             *******************************************************************/

            case 4:
            {
                quitflag = 1;
            }
            break;


            /*******************************************************************
             * DEFAULT: INVALID MENU SELECTION
             *
             * Executed when the user enters an option other than 1, 2, 3 or 4.
             * Displays an error message on the LCD and waits for one second.
             *******************************************************************/

            default:
            {
                /* Clear the LCD and display the invalid selection message. */
                Cmdlcd(CLEAR_LCD);
                Cmdlcd(GOTO_LINE1_POS0);
                Strlcd("INVALID OPTION");

                /* Provide a delay before clearing the error message. */
                delay_s(1);

                Cmdlcd(CLEAR_LCD);
            }
        }

        /* Exit the menu loop when the user selects option 4. */
        if (quitflag == 1)
        {
            break;
        }
    }
}
