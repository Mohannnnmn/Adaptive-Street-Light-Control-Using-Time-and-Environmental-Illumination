/**
 * @file    main.c
 * @brief   Main application module for an automated street lighting system.
 *
 * @details This module implements the main application logic for an
 *          automated street lighting system using the LPC21xx microcontroller.
 *
 *          The system performs the following operations:
 *          1. Acquires ambient light intensity data using the ADC peripheral.
 *          2. Automatically controls street lights based on ambient darkness
 *             and predefined time conditions.
 *          3. Supports user interaction through an external interrupt.
 *          4. Uses the RTC peripheral for timekeeping and scheduled operations.
 *          5. Configures the required GPIO pins for their respective functions.
 *
 * @note    The application is designed to minimize manual intervention
 *          by providing automatic street light control.
 */


/*=============================================================================
 * INCLUDE FILES
 *============================================================================*/

/* LPC21xx device-specific register definitions. */
#include <lpc21xx.h>

/* Application-specific macro and constant definitions. */
#include "defines.h"

/* Delay function declarations and definitions. */
#include "delay.h"

/* User-defined data type declarations. */
#include "types.h"

/* ADC peripheral driver interface. */
#include "ADC.h"

/* LCD driver interface. */
#include "LCD.h"

/* LCD-specific command and configuration definitions. */
#include "LCD_defines.h"

/* Keypad driver interface. */
#include "kpm.h"

/* Keypad-specific configuration definitions. */
#include "kpm_defines.h"

/* RTC-specific configuration and register definitions. */
#include "RTC_defines.h"

/* Application-specific function declarations. */
#include "header.h"


/*=============================================================================
 * MACRO DEFINITIONS
 *============================================================================*/

/* ADC channel selected for ambient light intensity measurement. */
#define ADC_ChanNo         1


/*=============================================================================
 * GLOBAL VARIABLE DECLARATIONS
 *============================================================================*/

/* External declaration of the interrupt status flag.
 * The volatile qualifier ensures that the compiler does not optimize away
 * accesses to this variable, as its value may be modified by an ISR.
 */
extern volatile u32 external_int;


/*=============================================================================
 * MAIN APPLICATION ENTRY POINT
 *============================================================================*/

main()
{
    /* Local variable to store the digital ADC conversion result. */
    u16 dVAL;

    /* Local variable to store the converted ADC value in floating-point format. */
    f32 eAR;


    /*-------------------------------------------------------------------------
     * GPIO CONFIGURATION
     *------------------------------------------------------------------------*/

    /* Configure the four GPIO pins starting from STREET_LIGHTS as outputs.
     * These pins are used to control the street lighting hardware.
     */
    IODIR1 |= 0XF<<STREET_LIGHTS;


    /*-------------------------------------------------------------------------
     * PERIPHERAL INITIALIZATION
     *------------------------------------------------------------------------*/

    /* Initialize the LCD module for displaying system information. */
    Initlcd();

    /* Introduce a startup delay to allow the LCD to stabilize. */
    delay_ms(100);

    /* Initialize the keypad interface for user input. */
    Initkpm();

    /* Initialize the ADC peripheral for analog signal acquisition. */
    Init_ADC();

    /* Configure the external interrupt for detecting user-triggered events. */
    set_external_interrupt();

    /* Initialize the Real-Time Clock peripheral for timekeeping. */
    RTC_INIT();

    /* Enable or configure the RTC interrupt for time-based operations. */
    RTC_INTERUPT();


    /*-------------------------------------------------------------------------
     * MAIN APPLICATION LOOP
     *------------------------------------------------------------------------*/

    /* Continuously execute the street lighting control operations. */
    while(1)
    {
        /* Read the current time from the RTC and display it on the LCD. */
        display_time();

        /* Acquire the analog input from the configured ADC channel.
         * dVAL receives the digital conversion result, while eAR receives
         * the corresponding floating-point representation.
         */
        Read_ADC(ADC_ChanNo,&dVAL,&eAR);

        /* Process the ADC reading and update the street light control
         * according to the configured ambient light and time conditions.
         */
        Read_adc_time_lights(dVAL);


        /* Check whether an external interrupt event has occurred.
         * The external_int flag is updated by the corresponding ISR.
         */
        if(external_int==1)
        {
            /* Execute the application-specific external interrupt handling
             * routine to process the detected user interaction.
             */
            external_int_occur();

            /* Configure or update the RTC alarm according to the
             * application-specific scheduling requirements.
             */
            set_alarm();

            /* Clear the interrupt status flag after processing the event.
             * This allows the application to detect subsequent interrupts.
             */
            external_int=0;
        }
    }
}

