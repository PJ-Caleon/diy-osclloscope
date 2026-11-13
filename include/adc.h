#ifndef ADC_H
#define ADC_H
#include <stdint.h>
void     adc_init(void);
int      adc_read(uint16_t *out);   /* 0 = ok, -1 = timeout */
uint32_t adc_to_mv(uint32_t raw);
#endif
