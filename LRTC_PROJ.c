/**
 * @file    rtc_proj.c
 * @brief   Real-Time Clock (RTC) configuration and street-light scheduling module.
 *
 * @details This module implements RTC initialization, interrupt configuration,
 *          alarm management, and time-based street-light control.
 *
 *          The module performs the following operations:
 *          1. Initializes the RTC peripheral and configures the initial alarm.
 *          2. Configures the Vector Interrupt Controller (VIC) for RTC interrupts.
 *          3. Handles RTC alarm events through an interrupt service routine.
 *          4. Dynamically updates alarm values based on the configured
 *             street-light operating schedule.
 *          5. Determines the next street-light ON/OFF event based on
 *             the current RTC time.
 *
 *          The RTC alarm mechanism provides automatic switching of street
 *          lights according to the configured operating schedule.
 *
 * @author  Project Development Team
 * @date    13-Nov-2026
 */


/*=============================================================================
 * INCLUDE FILES
 *============================================================================*/

/* LPC214x microcontroller register definitions and peripheral access. */
#include <lpc214x.h>

/* Application-specific macro and constant definitions. */
#include "defines.h"

/* Delay function declarations and definitions. */
#include "delay.h"

/* User-defined data type declarations. */
#include "types.h"

/* LCD driver interface for displaying RTC information. */
#include "LCD.h"

/* LCD-specific command and configuration definitions. */
#include "LCD_defines.h"

/* RTC register and configuration macro definitions. */
#include "RTC_defines.h"

/* Application-specific function declarations and interfaces. */
#include "header.h"


/*=============================================================================
 * GLOBAL VARIABLES
 *============================================================================*/

/**
 * @brief Street-light operating schedule configuration.
 *
 * @details These variables define the scheduled ON and OFF timings
 *          for the automated street-light control system.
 *
 *          start_hour : Hour at which the street lights are scheduled ON.
 *          end_hour   : Hour at which the street lights are scheduled OFF.
 *          start_min  : Minute component of the scheduled ON time.
 *          end_min    : Minute component of the scheduled OFF time.
 *          alarm      : Indicates the next scheduled RTC alarm event.
 *
 * @note Time values are represented in 24-hour format.
 */
u32 start_hour = 19;
u32 end_hour   = 6;
u32 start_min  = 0;
u32 end_min    = 30;
u32 alarm      = 0;


/**
 * @brief External flag indicating a request to update the RTC alarm.
 *
 * @details This flag is declared in another application module and is
 *          monitored by set_alarm() to determine whether the alarm
 *          configuration needs to be updated.
 *
 * @note The volatile qualifier ensures that the compiler does not
 *       optimize accesses to a variable that may be modified externally.
 */
extern volatile u32 flag;


/*=============================================================================
 * FUNCTION DEFINITIONS
 *============================================================================*/


/**
 * @brief RTC alarm interrupt service routine.
 *
 * @details This interrupt handler is executed when an RTC alarm event
 *          occurs.
 *
 *          The routine performs the following operations:
 *          1. Checks whether the RTC alarm interrupt is pending.
 *          2. Clears the alarm interrupt flag.
 *          3. Determines the next alarm action using the alarm state.
 *          4. Updates the RTC alarm hour and minute registers.
 *          5. Updates the alarm state for the subsequent alarm event.
 *          6. Notifies the VIC that interrupt servicing is complete.
 *
 * @param None
 *
 * @return None
 *
 * @note Declared with __irq to identify the function as an IRQ handler.
 */
void RTC_ISR(void) __irq
{
    /* Check whether the RTC alarm interrupt flag is set. */
    if (ILR & 0x02)
    {
        /* Clear the RTC alarm interrupt pending flag. */
        ILR = 0x02;

        /* Check the current alarm state to determine the next alarm action. */
        if (alarm == 0)
        {
            /* Configure the next alarm using the scheduled street-light OFF time. */
            ALHOUR = end_hour;
            ALMIN  = end_min;

            /* Update the alarm state to indicate the next scheduled event. */
            alarm = 1;
        }
        else
        {
            /* Configure the next alarm using the scheduled street-light ON time. */
            ALHOUR = start_hour;
            ALMIN  = start_min;

            /* Reset the alarm state for the subsequent scheduled event. */
            alarm = 0;
        }
    }

    /* Clear the VIC interrupt vector address register to indicate
     * completion of the current interrupt service routine.
     */
    VICVectAddr = 0;
}


/**
 * @brief Configures the RTC interrupt through the Vector Interrupt Controller.
 *
 * @details Initializes the interrupt-related RTC and VIC registers.
 *
 *          Configuration sequence:
 *          1. Clears any previously pending RTC interrupt flags.
 *          2. Disables RTC counter increment interrupts.
 *          3. Enables the RTC interrupt in the VIC interrupt enable register.
 *          4. Selects IRQ mode for the RTC interrupt channel.
 *          5. Assigns RTC interrupt channel 13 to VIC vector slot 0.
 *          6. Registers the RTC interrupt service routine.
 *
 * @param None
 *
 * @return None
 */
void RTC_INTERUPT(void)
{
    /* Clear all pending RTC interrupt flags before enabling interrupts. */
    ILR = CLR_INT;

    /* Disable RTC counter increment interrupts. */
    CIIR = 0x00;

    /* Enable RTC interrupt channel 13 in the VIC interrupt controller. */
    VICIntEnable |= 1 << 13;

    /* Configure RTC interrupt channel 13 as an IRQ interrupt. */
    VICIntSelect &= ~(1 << 13);

    /* Enable VIC vector slot 0 and associate it with RTC interrupt channel 13. */
    VICVectCntl0 = 0x20 | 13;

    /* Register the RTC interrupt service routine in VIC vector slot 0. */
    VICVectAddr0 = (unsigned long)RTC_ISR;
}


/**
 * @brief Initializes the Real-Time Clock peripheral.
 *
 * @details Configures the RTC clock source and enables the RTC counter.
 *          It also invokes the alarm initialization routine to configure
 *          the initial alarm based on the current RTC time.
 *
 *          The alarm mask register is configured to select the RTC
 *          fields used for alarm matching.
 *
 * @param None
 *
 * @return None
 */
void RTC_INIT(void)
{
    /* Enable the RTC counter and select the configured RTC clock source. */
    CCR = CLK_ENABLE | CLK_CLSRC;

    /* Configure the initial alarm based on the current RTC time. */
    init_alarm();

    /* Configure the RTC alarm mask register. */
    AMR = 0XF9;
}


/**
 * @brief Updates the RTC alarm according to the street-light schedule.
 *
 * @details Checks the external update flag to determine whether the
 *          RTC alarm configuration requires modification.
 *
 *          Based on the relationship between the configured start and
 *          end hours, the function invokes the appropriate scheduling
 *          routine:
 *
 *          - start_great_end(): Handles schedules where the ON hour
 *            is greater than the OFF hour, typically an overnight schedule.
 *
 *          - end_great_start(): Handles schedules where the OFF hour
 *            is greater than the ON hour.
 *
 *          After processing the schedule update, the external flag
 *          is cleared.
 *
 * @param None
 *
 * @return None
 *
 * @note The scheduling routines determine the alarm values using
 *       the current RTC hour and minute registers.
 */
void set_alarm(void)
{
    /* Check whether an external request for alarm reconfiguration is pending. */
    if (flag == 1)
    {
        /* Check whether the scheduled ON hour is greater than the OFF hour. */
        if(start_hour > end_hour)
        {
            /* Process the schedule when the operating period crosses midnight. */
            start_great_end();
        }

        /* Check whether the scheduled OFF hour is greater than the ON hour. */
        else if(end_hour > start_hour)
        {
            /* Process the schedule when the OFF time occurs after the ON time. */
            end_great_start();
        }

        /* Clear the external alarm update request after processing. */
        flag = 0;
    }
}


/**
 * @brief Configures the initial RTC alarm based on the current time.
 *
 * @details Evaluates the current RTC hour and minute registers against
 *          the configured street-light operating schedule.
 *
 *          Based on the evaluated time conditions, the function updates
 *          the RTC alarm hour and minute registers and initializes
 *          the alarm state.
 *
 *          The alarm state is used by the RTC interrupt service routine
 *          to determine the subsequent alarm configuration.
 *
 * @param None
 *
 * @return None
 *
 * @note The function directly accesses the RTC HOUR and MIN registers.
 */
void init_alarm(void)
{
    /* Check the configured condition for scheduling the next OFF event. */
    if ((HOUR > 6 && HOUR >= 19 && start_min == 0))
    {
        /* Load the scheduled street-light OFF hour into the RTC alarm register. */
        ALHOUR = end_hour;

        /* Load the scheduled street-light OFF minute into the RTC alarm register. */
        ALMIN = end_min;

        /* Set the alarm state to indicate the next scheduled event. */
        alarm = 1;
    }

    /* Check whether the current hour falls between the scheduled OFF
     * and ON hours.
     */
    else if(HOUR > end_hour && HOUR < start_hour)
    {
        /* Configure the next alarm for the scheduled street-light ON time. */
        ALHOUR = start_hour;

        /* Configure the minute component of the scheduled ON time. */
        ALMIN = start_min;

        /* Set the alarm state for the next scheduled event. */
        alarm = 0;
    }

    /* Check the configured condition for scheduling the next OFF event
     * when the current hour is within the specified end-hour range.
     */
    else if(HOUR <= end_hour && MIN < end_min)
    {
        /* Configure the RTC alarm hour for the scheduled OFF time. */
        ALHOUR = end_hour;

        /* Configure the RTC alarm minute for the scheduled OFF time. */
        ALMIN = end_min;

        /* Update the alarm state for the subsequent interrupt event. */
        alarm = 1;
    }
}


/**
 * @brief Loads the predefined initial date and time into the RTC registers.
 *
 * @details Initializes the RTC time and calendar registers with the
 *          application-defined startup values.
 *
 *          Initial time configuration:
 *          - Hour   : 19
 *          - Minute : 00
 *          - Second : 40
 *
 *          Initial date configuration:
 *          - Year   : 2026
 *          - Month  : 11
 *          - Date   : 04
 *
 * @param None
 *
 * @return None
 *
 * @note The RTC counter should be disabled before modifying the time
 *       and calendar registers to ensure consistent initialization.
 */
void init_time_set(void)
{
    /* Configure the initial hour value in the RTC. */
    HOUR = 19;

    /* Configure the initial minute value in the RTC. */
    MIN = 0;

    /* Configure the initial second value in the RTC. */
    SEC = 40;

    /* Configure the initial year value in the RTC calendar. */
    YEAR = 2026;

    /* Configure the initial month value in the RTC calendar. */
    MONTH = 11;

    /* Configure the initial day-of-month value in the RTC calendar. */
    DOM = 4;
}


/**
 * @brief Determines the next RTC alarm for an overnight operating schedule.
 *
 * @details Evaluates the current RTC hour and minute against the configured
 *          street-light ON and OFF timings when the ON hour is greater
 *          than the OFF hour.
 *
 *          Based on the current time, the function configures the next
 *          scheduled alarm by updating the RTC alarm registers.
 *
 *          The alarm state is also updated to indicate the next
 *          scheduled event.
 *
 * @param None
 *
 * @return None
 *
 * @note This function uses the RTC HOUR and MIN registers directly.
 */
void start_great_end(void)
{
    /* Check whether the current hour falls between the OFF and ON hours.
     * If true, schedule the next alarm for the street-light ON time.
     */
    if (HOUR > end_hour && HOUR < start_hour)
    {
        /* Configure the RTC alarm for the scheduled ON hour. */
        ALHOUR = start_hour;

        /* Configure the RTC alarm for the scheduled ON minute. */
        ALMIN = start_min;

        /* Update the alarm state for the next scheduled event. */
        alarm = 0;
    }

    /* Evaluate the additional time conditions to determine whether
     * the next alarm should be configured for the scheduled OFF time.
     */
    else if((HOUR >= start_hour && MIN > start_min) ||
            (HOUR < start_hour && HOUR < end_hour) ||
            (HOUR <= end_hour && MIN < end_min))
    {
        /* Configure the RTC alarm for the scheduled OFF hour. */
        ALHOUR = end_hour;

        /* Configure the RTC alarm for the scheduled OFF minute. */
        ALMIN = end_min;

        /* Update the alarm state for the next scheduled event. */
        alarm = 1;
    }

    /* Check whether the current time satisfies the additional
     * end-hour scheduling condition.
     */
    else if(HOUR >= end_hour && HOUR < start_hour && MIN > end_min)
    {
        /* Configure the next alarm for the scheduled ON hour. */
        ALHOUR = start_hour;

        /* Configure the next alarm for the scheduled ON minute. */
        ALMIN = start_min;

        /* Update the alarm state to indicate the next scheduled event. */
        alarm = 0;
    }
}


/**
 * @brief Determines the next RTC alarm for a daytime operating schedule.
 *
 * @details Evaluates the current RTC hour and minute against the configured
 *          street-light ON and OFF timings when the OFF hour is greater
 *          than the ON hour.
 *
 *          Based on the evaluated time conditions, the function selects
 *          the appropriate next alarm and updates the RTC alarm registers.
 *
 *          The alarm state is updated to identify the next scheduled event.
 *
 * @param None
 *
 * @return None
 *
 * @note This function uses the RTC HOUR and MIN registers directly.
 */
void end_great_start(void)
{
    /* Check whether the current time falls outside the configured
     * street-light operating interval.
     */
    if((HOUR < end_hour && HOUR < start_hour) ||
       HOUR > end_hour && HOUR > start_hour)
    {
        /* Configure the next alarm for the scheduled street-light ON hour. */
        ALHOUR = start_hour;

        /* Configure the minute component of the scheduled ON time. */
        ALMIN = start_min;

        /* Set the alarm state for the next scheduled event. */
        alarm = 0;
    }

    /* Evaluate the additional operating-period conditions to determine
     * whether the next alarm should be configured for the OFF time.
     */
    else if ((HOUR >= start_hour && MIN >= start_min) ||
             (HOUR < end_hour && HOUR > start_hour) ||
             (HOUR <= end_hour && MIN < end_min))
    {
        /* Configure the RTC alarm for the scheduled street-light OFF hour. */
        ALHOUR = end_hour;

        /* Configure the RTC alarm for the scheduled street-light OFF minute. */
        ALMIN = end_min;

        /* Set the alarm state to indicate the next scheduled event. */
        alarm = 1;
    }
}
