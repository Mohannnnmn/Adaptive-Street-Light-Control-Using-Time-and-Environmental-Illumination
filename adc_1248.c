#include <LPC21xx.h>
#include "defines.h"
#include "delay.h"
#include "types.h"
#include "adc_defines.h"
#include "adc.h"


void Init_ADC(void)
{
	//cfg pins p0.27 to p0.30
	//for SIN0-AIN1 pin functions 
	PINSEL1&=~(0X3F<<(28-16)*2);
  PINSEL1|=0X15400000;
	//PINSEL1|=(1<<24);
	//cfg ADCR
	ADCR|=((1<<PDN_BIT)|(CLKDIV_VAL<<CLKDIV_BITS));
}
void Read_ADC(u8 chNo,u16*dval,f32*eAR)
{
	//clear any pre channel selection 
	ADCR&=0xffffff00;       //4 channels
	//update on req channel selection
	//start sample for adc conversion
	ADCR|=(1<<ADC_CONV_START_BIT)|(1<<chNo);
	//wait for conv time
	delay_us(3);
	//check status of conv
	while(((ADDR>>DONE_BIT)&1)==0);
	//delay_s(2);
	//stop smapling &conv
	ADCR&=~(1<<ADC_CONV_START_BIT);
	//read extract digital data
	*dval=((ADDR>>DIGITAL_DATA_BITS)&1023);
	//represent in equvivalent voltage level
	*eAR=(((*dval)*3.3)/1023);
}

