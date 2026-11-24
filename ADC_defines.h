//adc_defines.h
#define  FOSC           12000000
#define  CCLK           (FOSC*5)
#define  PCLK           (CCLK/4) //pheripheral clock
#define ADC_CLK         3000000

//defines for ADCR SFR

#define  CLKDIV_BITS            8 //8-15
#define  PDN_BIT                21
#define  ADC_CONV_START_BIT     24
#define  CLKDIV_VAL             ((PCLK/ADC_CLK)-1)

//difines for ADDR SFR
#define  DIGITAL_DATA_BITS   6
#define  DONE_BIT            31
#define AIN0_FUNC            0x00400000
#define AIN1_FUNC            0x01000000
#define AIN2_FUNC            0x04000000
#define AIN3_FUNC            0x10000000

// fixed values for the LDR
#define TH_dVAL    650 
