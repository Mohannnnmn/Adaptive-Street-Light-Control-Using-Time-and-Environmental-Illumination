/*******************************************************************************
 * RTC AND SYSTEM CLOCK CONFIGURATION MACROS
 ******************************************************************************/

/* External crystal oscillator frequency (12 MHz) */
#define FOSC             12000000

/* CPU clock frequency (CCLK = FOSC * 5 = 60 MHz) */
#define CCLK             (FOSC*5)

/* Peripheral clock frequency (PCLK = CCLK / 4 = 15 MHz) */
#define PCLK             (CCLK/4)


/* RTC clock frequency (32.768 kHz) */
#define RTC_CLK          32768

/* Integer prescaler value used to generate the required RTC clock frequency */
#define PREINT_VAL       ((int) ((PCLK / RTC_CLK) - 1))

/* Fractional prescaler value used along with PREINT to generate an accurate
   1 Hz clock for the Real-Time Clock */
#define PREFRAC_VAL     (PCLK -((PREINT_VAL + 1) * RTC_CLK))



/*******************************************************************************
 * RTC CONTROL AND INTERRUPT CONFIGURATION MACROS
 ******************************************************************************/

/* Defines the street light operating duration in hours (24 hours) */
#define STREET_LIGHTS    24

/* Enables the Real-Time Clock by setting the CLKEN bit in the CCR register */
#define CLK_ENABLE       0x01

/* Selects the RTC clock source by setting the CLKSRC bit in the CCR register */
#define CLK_CLSRC        0X10

/* Resets the RTC prescaler by setting the CTCRST bit in the CCR register */
#define CLK_RESET        0x02

/* Clears both RTC interrupt flags (RTCCIF and RTCALF) */
#define CLR_INT          0X03

