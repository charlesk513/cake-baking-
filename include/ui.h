#ifndef UI_H
#define UI_H
#include "navigation.h"
#include <stddef.h>

typedef struct {
    int animate;    // 0/1
    int delay_ms;   // per frame
    int verbosity;  // 0=quiet,1=info,2=debug
    const char* log_path; // nullable
} UiOpts;

// Draws grid with optional overlay (overlay[r][c] == 1 highlights planned path).
void draw_grid(int house[NROWS][NCOLS],
               Position pos,
               Position kitchen,
               Position target,
               const int *overlay, // nullable, size NROWS*NCOLS
               int bag_weight, int bag_cap, int trip_no,
               const char *current_item,
               int step_no, int total_steps,
               const UiOpts* opts);

// Simple progress bars
void draw_bar(const char* label, int v, int max, int width);

// Logging helper
void ui_log(const UiOpts* opts, const char* fmt, ...);

#endif
