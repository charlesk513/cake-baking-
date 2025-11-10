#ifndef NAVIGATION_H
#define NAVIGATION_H
#include <stddef.h>

#define NROWS 8
#define NCOLS 8

typedef struct { int r; int c; } Position;

enum Direction { UP=0, DOWN=1, LEFT=2, RIGHT=3 };

// Move step-by-step. Returns the final position after attempting steps.
// If a blocked/out-of-bounds step is encountered, the caller should treat as dead-end.
Position move(enum Direction dir, int steps, Position start, int house[NROWS][NCOLS]);

// Find a path from start to goal (no diagonals), writing directions into dirs[] (max_dirs).
// Returns number of steps in the path found, or 0 if none.
size_t find_path(Position start, Position goal, int house[NROWS][NCOLS], enum Direction *dirs, size_t max_dirs);

// Helpers to load layout/map
int load_house_layout(const char* path, int house[NROWS][NCOLS]);

#endif
