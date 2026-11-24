/*******************************************************************************
 * Function Prototypes : ADC Initialization and Data Acquisition
 *
 * Init_ADC() :
 * Initializes and configures the ADC peripheral.
 *
 * Read_ADC() :
 * Reads the analog input from the specified ADC channel and stores
 * the digital conversion result and equivalent analog voltage.
 *
 * Parameters:
 * chNo  - ADC channel number to be selected.
 * dval  - Pointer to store the digital ADC conversion value.
 * eAR   - Pointer to store the equivalent analog reading.
 ******************************************************************************/

#include "types.h"

/* Function prototype to initialize the ADC peripheral. */
void Init_ADC(void);

/* Function prototype to read ADC data from the specified channel. */
void Read_ADC(u8 chNo,u16*dval,f32*eAR);
