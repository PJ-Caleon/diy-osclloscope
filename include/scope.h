#ifndef SCOPE_H
#define SCOPE_H
#include <stdint.h>
#include "ui.h"
#include "measure.h"
typedef struct {
    ui_t      ui;
    uint16_t *cap;        /* 256 samples (points at g_cap on the board) */
    int       trig;       /* trigger index or -1 */
    unsigned  start;      /* first sample drawn */
    stats_t   st;
    uint32_t  freq_hz;
} scope_t;
void scope_init(scope_t *sc);
void scope_process(scope_t *sc);
void scope_header(const scope_t *sc, char *line1, char *line2);
void scope_draw(const scope_t *sc);
#endif
