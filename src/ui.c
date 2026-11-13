/* ui.c - time/div and volts/div tables, button debouncer, table-driven UI state machine. */
#include "ui.h"
#include "drills.h"
#include "stm32f103.h"
#include <stddef.h>

/* @TODO(M4-02/1) tdiv_us: the 8 time/div values in microseconds, from the cheat sheet (100 first, 20000 last). [src: M4-02 step 1] */
const uint32_t tdiv_us[TDIV_COUNT] = { 1 };
/* @TODO(M4-02/2) vdiv_mv: the 3 volts/div values in millivolts (1000, 500, 250). [src: M4-02 step 2] */
const uint32_t vdiv_mv[VDIV_COUNT] = { 1 };
const char *const tdiv_label[TDIV_COUNT] = { "100us", "200us", "500us", "1ms", "2ms", "5ms", "10ms", "20ms" };
const char *const vdiv_label[VDIV_COUNT] = { "1V", "500mV", "250mV" };

/* @TODO(M4-02/3) tdiv_to_fs: sample rate = 16 pixels per division / time per division. fs = 16 * 1,000,000 / tdiv_us[idx], with mul_div (card C5). [src: M4-02 step 3, card C5] */
uint32_t tdiv_to_fs(int tdiv_idx)
{
    (void)tdiv_idx;
    return 0;
}

/* @TODO(M4-02/4) ui_init: mode M_TDIV, tdiv_idx 3, vdiv_idx 0, level_mv 1650, rising 1. [src: M4-02 step 4] */
void ui_init(ui_t *u)
{
    (void)u;
}

/* @TODO(M4-02/5) step_index: idx + delta, forced into 0..max with clamp_i. [src: M4-02 step 5, drill clamp_i] */
int step_index(int idx, int delta, int max)
{
    (void)delta; (void)max;
    return idx;
}

/* @TODO(M4-03/1) debounce_update: raw (0/1). If raw differs from d->last: remember it and the time, return 0. Else if raw differs from d->stable and elapsed(now, t_change) >= DEBOUNCE_MS: stable = raw, return raw (1 = a new press). Otherwise return 0. [src: M4-03 step 1, card C6] */
int debounce_update(debounce_t *d, int raw, uint32_t now_ms)
{
    (void)d; (void)raw; (void)now_ms;
    return 0;
}

/* @TODO(M4-03/2) The seven action functions: tdiv up/down (step_index +1/-1, max 7), vdiv up/down (max 2), level up/down (+100/-100 mV, clamp_i to 0..3300), edge toggle (rising becomes the opposite). [src: M4-03 step 2, drill clamp_i] */
static void act_tdiv_up(ui_t *u)   { (void)u;
}
static void act_tdiv_down(ui_t *u) { (void)u;
}
static void act_vdiv_up(ui_t *u)   { (void)u;
}
static void act_vdiv_down(ui_t *u) { (void)u;
}
static void act_level_up(ui_t *u)  { (void)u;
}
static void act_level_down(ui_t *u){ (void)u;
}
static void act_edge_toggle(ui_t *u){ (void)u;
}

typedef struct { ui_mode_t mode; ui_event_t ev; void (*action)(ui_t *); ui_mode_t next; } ui_row_t;
static const ui_row_t ui_table[] = {
    { M_TDIV,  EV_UP,   act_tdiv_up,     M_TDIV  },
    { M_TDIV,  EV_DOWN, act_tdiv_down,   M_TDIV  },
    { M_TDIV,  EV_MODE, NULL,            M_VDIV  },
    { M_VDIV,  EV_UP,   act_vdiv_up,     M_VDIV  },
    { M_VDIV,  EV_DOWN, act_vdiv_down,   M_VDIV  },
    { M_VDIV,  EV_MODE, NULL,            M_LEVEL },
    { M_LEVEL, EV_UP,   act_level_up,    M_LEVEL },
    { M_LEVEL, EV_DOWN, act_level_down,  M_LEVEL },
    { M_LEVEL, EV_MODE, NULL,            M_EDGE  },
    { M_EDGE,  EV_UP,   act_edge_toggle, M_EDGE  },
    { M_EDGE,  EV_DOWN, act_edge_toggle, M_EDGE  },
    { M_EDGE,  EV_MODE, NULL,            M_TDIV  },
};

/* @TODO(M4-03/3) ui_handle: loop over ui_table; for the row whose mode == u->mode and ev == ev: call action if it is not NULL, then set u->mode = next, and stop. [src: M4-03 step 3] */
void ui_handle(ui_t *u, ui_event_t ev)
{
    (void)u; (void)ev;
}

/* @TODO(M4-03/4) buttons_init, card C2 three times: clock GPIOB; pins 12, 13, 14 nibble 0x8 (input with pull); then set ODR bits 12..14 high through BSRR to select the pull-UP. [src: M4-03 step 4, cards C2+C4] */
void buttons_init(void)
{
}

/* @TODO(M4-03/5) buttons_read: return a 3-bit mask, bit 0 = MODE (PB12), bit 1 = UP (PB13), bit 2 = DOWN (PB14), a 1 meaning PRESSED. Buttons are active-low, so invert IDR first: ((~IDR) >> 12) & 7. [src: M4-03 step 5] */
uint8_t buttons_read(void)
{
    return 0;
}
