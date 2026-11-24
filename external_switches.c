/*******************************************************************************
 * File Name    : user_settings.c
 *
 * Description  : User Interface for RTC Configuration and Street Light Timing.
 *
 * Details:
 * 1. Provides functions to modify the RTC time and date through the keypad.
 * 2. Allows the user to configure street light ON and OFF timings.
 * 3. Displays the available configuration options on the LCD.
 * 4. Reads user input through the keypad and validates the entered values.
 * 5. Updates the corresponding RTC registers based on user selections.
 * 6. Displays the configured street light timings and alarm information.
 *
 * User Interaction:
 * The user can access these configuration functions through the external
 * switch-controlled menu. Once the corresponding option is selected, the
 * LCD displays the available settings, and the user can enter new values
 * using the keypad.
 *
 * Functions:
 * 1. set_time()       - Modifies RTC hours, minutes and seconds.
 * 2. set_date()       - Modifies RTC date, month, year and day.
 * 3. set_lights_time()- Configures and displays street light ON/OFF timings.
 *
 ******************************************************************************/

#include <lpc21xx.h>
#include "types.h"
#include "delay.h"
#include "LCD.h"
#include "LCD_defines.h"
#include "kpm.h"
#include "kpm_defines.h"

/* External variables used for RTC configuration and street light scheduling. */
extern u32 end_hour,end_min,start_hour,start_min,flag,alarm;


/*******************************************************************************
 * Function Name : set_time()
 *
 * Description   : Allows the user to modify the RTC time through the keypad.
 *
 * Working:
 * 1. Displays the time configuration menu on the LCD.
 * 2. Reads the user's selection through the keypad.
 * 3. Allows modification of hours, minutes and seconds individually.
 * 4. Validates the entered values before updating the RTC registers.
 * 5. Displays an error message if an invalid value is entered.
 * 6. Allows the user to exit the time configuration menu.
 *
 * RTC Registers Updated:
 * HOUR - Stores the hour value (0-23).
 * MIN  - Stores the minute value (0-59).
 * SEC  - Stores the second value (0-59).
 *
 ******************************************************************************/

void set_time(void)
{ 
    s32 timechoice,quitflag=0,sec,min,hour;

    while(1)
    {
        /* Indicate that the user configuration operation is active. */
        flag=1;

        /* Display the available time configuration options. */
        Cmdlcd(CLEAR_LCD);
        Cmdlcd(GOTO_LINE1_POS0);
        Strlcd("1.HOUR");
        Cmdlcd(GOTO_LINE1_POS0+7);
        Strlcd("2.MIN");
        Cmdlcd(GOTO_LINE2_POS0);
        Strlcd("3.SEC");
        Cmdlcd(GOTO_LINE2_POS0+7);
        Strlcd("4.QUIT");

        /* Read the user's menu selection from the keypad. */
        timechoice=ReadNum();

        switch(timechoice)
        {
            case 1:
            {
Hour:
                Cmdlcd(CLEAR_LCD);
                Cmdlcd(GOTO_LINE1_POS0);
                Strlcd("SET HOUR");

                /* Read and validate the hour value (0-23). */
                hour = ReadNum();

                if (hour > 23)
                {
                    Cmdlcd(CLEAR_LCD);
                    Strlcd("INVALID HOUR");
                    delay_ms(500);
                    goto Hour;
                }
                else
                {
                    /* Update the RTC hour register. */
                    HOUR = hour;
                }
            }
            break;

            case 2:
            {
Min:
                Cmdlcd(CLEAR_LCD);
                Strlcd("SET MIN");

                /* Read and validate the minute value (0-59). */
                min = ReadNum();

                if (min > 59)
                {
                    Cmdlcd(CLEAR_LCD);
                    Strlcd("INVALID MIN");
                    delay_ms(500);
                    goto Min;
                } 
                else
                {
                    /* Update the RTC minute register. */
                    MIN = min;
                }
            }
            break;

            case 3:
            {
Sec:
                Cmdlcd(CLEAR_LCD);
                Strlcd("SET SEC");

                /* Read and validate the second value (0-59). */
                sec = ReadNum();

                if (sec > 59)
                {
                    Cmdlcd(CLEAR_LCD);
                    Strlcd("INVALID SEC");
                    goto Sec;
                }
                else
                {
                    /* Update the RTC second register. */
                    SEC = sec;
                }
            }
            break;

            case 4:
            {
                /* Set the exit flag to leave the time configuration menu. */
                quitflag=1;
            }
            break;

            default:
            {
                /* Display an error message for an unsupported menu option. */
                Cmdlcd(CLEAR_LCD);
                Cmdlcd(GOTO_LINE1_POS0);
                Strlcd("INVALID OPTION");
                quitflag=1;
            }
        }

        /* Exit the menu when the user selects QUIT or an invalid option. */
        if(quitflag==1)
        {
            break;
        }
    }
}


/*******************************************************************************
 * Function Name : set_date()
 *
 * Description   : Allows the user to modify the RTC date configuration.
 *
 * Working:
 * 1. Displays the date configuration menu on the LCD.
 * 2. Reads the user's selection through the keypad.
 * 3. Allows modification of date, month, year and day of the week.
 * 4. Validates the entered date, month and day values.
 * 5. Updates the corresponding RTC registers.
 * 6. Allows the user to exit the date configuration menu.
 *
 * RTC Registers Updated:
 * DOM   - Stores the date of the month.
 * MONTH - Stores the month value (1-12).
 * YEAR  - Stores the year value.
 * DOW   - Stores the day of the week.
 *
 ******************************************************************************/

void set_date(void)
{
    s32 datechoice=0,quitflag=0;

    while(1)
    {
        /* Display the available date configuration options. */
        Cmdlcd(CLEAR_LCD);
        Cmdlcd(GOTO_LINE1_POS0);
        Strlcd("1.DATE 2.M 3.Y");
        Cmdlcd(GOTO_LINE2_POS0);
        Strlcd("4.DAY 5.QUIT");

        /* Read the user's date configuration selection. */
        datechoice = ReadNum();

        switch(datechoice)
        {
            case 3:
            {
                /* Read and update the RTC year register. */
                YEAR = ReadNum();
            }
            break;

            case 2:
            {
Month:
                Cmdlcd(CLEAR_LCD);
                Cmdlcd(GOTO_LINE1_POS0);
                Strlcd("SET MONTH");

                /* Read and validate the month value (1-12). */
                MONTH = ReadNum();

                if (MONTH > 12 || MONTH == 0)
                {
                    Cmdlcd(CLEAR_LCD);
                    Cmdlcd(GOTO_LINE1_POS0);
                    Strlcd("INVALID MONTH");
                    delay_ms(300);
                    goto Month;
                }
            }
            break;

            case 1:
            {
Dom:
                Cmdlcd(CLEAR_LCD);
                Cmdlcd(GOTO_LINE1_POS0);
                Strlcd("SET DATE");

                /* Read and validate the date value for the selected month. */
                DOM = ReadNum();

                if (DOM > 31 || DOM == 0)
                {
                    Cmdlcd(CLEAR_LCD);
                    Cmdlcd(GOTO_LINE1_POS0);
                    Strlcd("INVALID DATE");
                    delay_ms(300);
                    goto Dom;
                }
                else if ((MONTH == 4 || MONTH == 6 || MONTH == 9 || MONTH == 11) && DOM > 30)
                {
                    Cmdlcd(CLEAR_LCD);
                    Cmdlcd(GOTO_LINE1_POS0);
                    Strlcd("INVALID DATE");
                    delay_ms(300);
                    goto Dom;
                }
                else if (MONTH == 2 && DOM > 29)
                {
                    Cmdlcd(CLEAR_LCD);
                    Cmdlcd(GOTO_LINE1_POS0);
                    Strlcd("INVALID DATE");
                    delay_ms(300);
                    goto Dom;
                }
            }
            break;

            case 4:
            {
Dow:
                Cmdlcd(CLEAR_LCD);
                Cmdlcd(GOTO_LINE1_POS0);
                Strlcd("SET DAY");

                /* Read and validate the day of the week (1-7). */
                DOW = ReadNum();

                if (DOW > 7 || DOW == 0)
                {
                    Cmdlcd(CLEAR_LCD);
                    Cmdlcd(GOTO_LINE1_POS0);
                    Strlcd("INVALID DAY");
                    delay_ms(300);
                    goto Dow;
                }

                /* Convert the user-entered day to the RTC register format. */
                DOW = DOW - 1;
            }
            break;

            case 5:
            {
                /* Set the exit flag to leave the date configuration menu. */
                quitflag=1;
            }
            break;

            default:
            {
                /* Display an error message for an unsupported menu option. */
                Cmdlcd(CLEAR_LCD);
                Cmdlcd(GOTO_LINE1_POS0);
                Strlcd("INVALID OPTION");
                quitflag=1;
            }
            break;
        }

        /* Exit the menu when the user selects QUIT or an invalid option. */
        if(quitflag==1)
        {
            break;
        }
    }
}


/*******************************************************************************
 * Function Name : set_lights_time()
 *
 * Description   : Allows the user to configure street light ON and OFF timings.
 *
 * Working:
 * 1. Displays the street light scheduling menu on the LCD.
 * 2. Allows the user to configure the street light ON time.
 * 3. Allows the user to configure the street light OFF time.
 * 4. Validates the entered hours and minutes.
 * 5. Displays the configured ON and OFF timings on the LCD.
 * 6. Provides an option to display the current alarm value.
 * 7. Allows the user to exit the street light configuration menu.
 *
 * External Variables Updated:
 * start_hour - Stores the street light ON hour.
 * start_min  - Stores the street light ON minute.
 * end_hour   - Stores the street light OFF hour.
 * end_min    - Stores the street light OFF minute.
 *
 ******************************************************************************/

void set_lights_time(void)
{
    s32 setchoice=0,quitflag=0;

    /* Indicate that the user configuration operation is active. */
    flag=1;

    while(1) 
    {
        /* Display the street light scheduling menu. */
        Cmdlcd(CLEAR_LCD);
        Cmdlcd(GOTO_LINE1_POS0);
        Strlcd("1.S_TIME 3.SHOW");
        Cmdlcd(GOTO_LINE2_POS0);
        Strlcd("2.E_TIME 4.QUIT");

        /* Read the user's street light configuration selection. */
        setchoice = ReadNum();

        switch(setchoice)
        {
            case 1:
            {
Hour_on:
                Cmdlcd(CLEAR_LCD);
                Strlcd("HOURS LIGHT ON");

                /* Read and validate the light ON hour (0-23). */
                start_hour = ReadNum();

                if (start_hour > 23)
                {
                    Cmdlcd(CLEAR_LCD);
                    Strlcd("INVALID TIME");
                    delay_ms(500);
                    goto Hour_on;
                }

Min_on:
                Cmdlcd(CLEAR_LCD);
                Cmdlcd(GOTO_LINE1_POS0);
                Strlcd("MINS LIGHT ON");

                /* Read and validate the light ON minute (0-59). */
                start_min = ReadNum();

                if (start_min > 59)
                {
                    Cmdlcd(CLEAR_LCD);
                    Strlcd("INVALID MIN");
                    delay_ms(500);
                    goto Min_on;
                }
            }
            break;

            case 2:
            {
Hour_end:
                Cmdlcd(CLEAR_LCD);
                Cmdlcd(GOTO_LINE1_POS0);
                Strlcd("HOURS LIGHT OF");

                /* Read and validate the light OFF hour (0-23). */
                end_hour = ReadNum();

                if (end_hour > 23)
                {
                    Cmdlcd(CLEAR_LCD);
                    Strlcd("INVALID TIME");
                    delay_ms(500);
                    goto Hour_end;
                }

Min_off:
                Cmdlcd(CLEAR_LCD);
                Cmdlcd(GOTO_LINE1_POS0);
                Strlcd("MIN LIGHT OFF");

                /* Read and validate the light OFF minute (0-59). */
                end_min = ReadNum();

                if (end_min > 59)
                {
                    Cmdlcd(CLEAR_LCD);
                    Strlcd("INVALID MIN");
                    delay_ms(500);
                    goto Min_off;
                }
            }
            break;

            case 3:
            {
                /* Display the configured street light ON and OFF timings. */
                Cmdlcd(CLEAR_LCD);
                Cmdlcd(GOTO_LINE1_POS0);
                Strlcd("start:");
                u32lcd(start_hour);
                Charlcd(':');
                u32lcd(start_min);

                Cmdlcd(GOTO_LINE2_POS0);
                Strlcd("end:");
                u32lcd(end_hour);
                Charlcd(':');
                u32lcd(end_min);

                delay_s(1);
            }
            break;

            case 4:
            {
                /* Set the exit flag to leave the scheduling menu. */
                quitflag=1;
            }
            break;

            case 5:
            {
                /* Display the currently configured alarm value. */
                Cmdlcd(CLEAR_LCD);
                Strlcd("alarm:");
                u32lcd(alarm);
                delay_s(1);
            }

            default:
            {
                /* Display an error message for an unsupported menu option. */
                Cmdlcd(CLEAR_LCD);
                Cmdlcd(GOTO_LINE1_POS0);
                Strlcd("invalid option");
                delay_s(1);
            }
        }

        /* Exit the menu when the user selects QUIT. */
        if(quitflag==1)
        {
            break;
        }
    }
}
