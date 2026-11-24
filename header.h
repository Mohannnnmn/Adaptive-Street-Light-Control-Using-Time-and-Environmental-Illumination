#include  "types.h" 

 // EXTERNAL interrupt delated function names;
void set_external_interrupt(void);
void external_ISR(void)__irq;
void external_int_occur(void);

// display date and time on lcd
void display_time(void);

//rtc related function names in all files
void RTC_INTERUPT(void);
void RTC_ISR(void)__irq;
void set_alarm(void);
void init_alarm(void);
void RTC_INIT(void);
void init_time_set(void);
void start_great_end(void);
void end_great_start(void);

// used to read the adc valur from the rean_adc fucntion 
//update the lights based on the boundaries and light internsity
void Read_adc_time_lights(u16);

// after external interrupt is occured time change related function names;
void set_time(void);
void set_date(void);
void set_lights_time(void);


