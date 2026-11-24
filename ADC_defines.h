/*******************************************************************************
 * File Name    : adc_defines.h
 * Description  : Defines ADC clock configuration, control register bit
 *                positions, ADC channel configurations and LDR threshold.
 *
 * Details:
 * This header file contains macro definitions required for configuring
 * the ADC peripheral of the LPC2129 microcontroller, including clock
 * settings, ADC control and data register bit positions, analog input
 * channel selection and the predefined LDR threshold value.
 ******************************************************************************/

/* Oscillator frequency of the microcontroller (12 MHz). */
#define  FOSC           12000000

/* Defines the CPU clock frequency (CCLK) as five times the oscillator frequency. */
#define  CCLK           (FOSC*5)

/* Defines the peripheral clock frequency (PCLK) as one-fourth of CCLK. */
#define  PCLK           (CCLK/4) // Peripheral clock

/* Defines the operating clock frequency for the ADC peripheral (3 MHz). */
#define ADC_CLK         3000000

/* Definitions for ADC Control Register (ADCR) bit positions. */

/* Defines the starting bit position of the ADC clock divider field (bits 8-15). */
#define  CLKDIV_BITS            8 // 8-15

/* Defines the Power Down (PDN) bit position used to enable the ADC. */
#define  PDN_BIT                21

/* Defines the starting bit position of the ADC conversion start control field. */
#define  ADC_CONV_START_BIT     24

/* Calculates the ADC clock divider value based on the peripheral and ADC clock frequencies. */
#define  CLKDIV_VAL             ((PCLK/ADC_CLK)-1)

/* Definitions for ADC Data Register (ADDR) bit positions and channel configurations. */

/* Defines the starting bit position of the 10-bit digital conversion result. */
#define  DIGITAL_DATA_BITS   6

/* Defines the bit position of the conversion completion status flag. */
#define  DONE_BIT            31

/* Defines the pin function configuration for ADC channel AIN0. */
#define AIN0_FUNC            0x00400000

/* Defines the pin function configuration for ADC channel AIN1. */
#define AIN1_FUNC            0x01000000

/* Defines the pin function configuration for ADC channel AIN2. */
#define AIN2_FUNC            0x04000000

/* Defines the pin function configuration for ADC channel AIN3. */
#define AIN3_FUNC            0x10000000

/* Predefined threshold values for LDR-based light intensity detection. */

/* Defines the digital ADC threshold value used for LDR light intensity comparison. */
#define  TH_dVAL    650
