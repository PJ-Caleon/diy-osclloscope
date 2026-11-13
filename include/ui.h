#ifndef UI_H
#define UI_H
#include <stdint.h>
typedef enum { EV_NONE = 0, EV_MODE = 1, EV_UP = 2, EV_DOWN = 3 } ui_event_t;
typedef enum { M_TDIV = 0, M_VDIV, M_LEVEL, M_EDGE } ui_mode_t;
typedef struct { ui_mode_t mode; int tdiv_idx; int vdiv_idx; int level_mv; int rising; } ui_t;
typedef struct { uint8_t stable; uint8_t last; uint32_t t_change; } debounce_t;
#define DEBOUNCE_MS 20u
#define TDIV_COUNT 8
#define VDIV_COUNT 3
extern const uint32_t tdiv_us[TDIV_COUNT];
extern const uint32_t vdiv_mv[VDIV_COUNT];
extern const char *const tdiv_label[TDIV_COUNT];
extern const char *const vdiv_label[VDIV_COUNT];
uint32_t tdiv_to_fs(int tdiv_idx);
void     ui_init(ui_t *u);
int      step_index(int idx, int delta, int max);
int      debounce_update(debounce_t *d, int raw, uint32_t now_ms);
void     ui_handle(ui_t *u, ui_event_t ev);
void     buttons_init(void);
uint8_t  buttons_read(void);
#endif
