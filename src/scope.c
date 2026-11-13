/* scope.c - glue: capture -> trigger -> measure -> text + waveform. */
#include "scope.h"
#include "sampler.h"
#include "trigger.h"
#include "measure.h"
#include "ssd1306.h"
#include "gfx.h"
#include "fmt.h"
#include "cal.h"
#include "drills.h"

/* scope_init (given): defaults, capture buffer = g_cap. */
void scope_init(scope_t *sc)
{
    ui_init(&sc->ui);
    sc->cap = g_cap;
    sc->trig = -1;
    sc->start = 0;
    sc->st.min = 0; sc->st.max = 0; sc->st.avg = 0;
    sc->freq_hz = 0;
}

/* @TODO(M6-01/1) scope_process: (1) level = mv_to_counts(ui.level_mv); (2) trig = trig_find(cap, 128, level, ui.rising); start = trig_start(trig); (3) measure_stats over all 256; (4) mid = (min + max) / 2; if max - min >= 100 then freq_hz = measure_freq(cap, 256, mid, tdiv_to_fs(ui.tdiv_idx)) else 0. [src: M6-01 step 1] */
/* @TODO(M6-02/3) Calibration hook: at the very top of scope_process, run every sample through cal_apply(&g_cal, ...) in place (a loop over 256). [src: M6-02 step 3] */
void scope_process(scope_t *sc)
{
    (void)sc;
}

/* @TODO(M6-01/2) scope_header: line1 = tdiv_label + ' ' + vdiv_label + ' ' + fmt_hz(freq) + ' ' + 'R' or 'F'. line2 = mode letter (one of "TVLE" picked by ui.mode) + ' ' + 'p' + fmt_mv(vpp) + ' ' + 'a' + fmt_mv(avg). Example: "1ms 1V 1.00kHz R" and "T p3.30V a0.82V". [src: M6-01 step 2] */
void scope_header(const scope_t *sc, char *line1, char *line2)
{
    (void)sc;
    line1[0] = 0; line2[0] = 0;
}

/* @TODO(M6-01/3) scope_draw: fb_clear(); header lines at (0,0) and (0,8) with text_draw; draw_grid(); plot_wave(cap + start, 128, vdiv_mv[ui.vdiv_idx]). Do not flush here. [src: M6-01 step 3] */
void scope_draw(const scope_t *sc)
{
    (void)sc;
}
