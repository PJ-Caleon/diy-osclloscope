#ifndef SAMPLER_H
#define SAMPLER_H
#include <stdint.h>
#define SAMPLE_COUNT 256u
extern uint16_t g_cap[SAMPLE_COUNT];
void sampler_timing(uint32_t fs_hz, uint32_t *psc, uint32_t *arr);
void sampler_init(void);
void sampler_start(uint32_t fs_hz);
int  sampler_wait(uint32_t tries);   /* 0 = capture complete, -1 = not yet / timeout */
void sampler_stop(void);
#endif
