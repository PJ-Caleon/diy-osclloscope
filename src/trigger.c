/* trigger.c - find the sample where the signal crosses the trigger level. */
#include "trigger.h"
#include "drills.h"

/* @TODO(M4-01/1) mv_to_counts: millivolts to ADC counts: mv * 4095 / 3300, with mul_div (card C5). [src: M4-01 step 1, card C5] */
uint16_t mv_to_counts(uint32_t mv)
{
  return mul_div(mv, 4095u, 3300u);
}

/* @TODO(M4-01/2) trig_find: for i = 1 .. n_scan-1 return the first i where the signal crosses the level. Rising: s[i-1] < level and s[i] >= level. Falling: s[i-1] >= level and s[i] < level. Return -1 if none. [src: M4-01 step 2] */
int trig_find(const uint16_t *s, unsigned n_scan, uint16_t level, int rising)
{
  for (int i = 0; i < n_scan; i++){
    if (rising){
      if (s[i-1] < level && s[i] >= level){
        if (i != 0){
          return (int)i;
        }
      }
    } else{
      if (s[i-1] >= level && s[i] < level){
        return (int)i;
      }
    }
  }
  return -1;
}

/* @TODO(M4-01/3) trig_start: where does the window start? The trigger index, or 0 when there was no trigger (idx < 0 = auto mode). [src: M4-01 step 3] */
unsigned trig_start(int idx)
{
  if (idx < 0){
    return 0;
  }
  return (unsigned)idx;
}
