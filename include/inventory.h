#ifndef INVENTORY_H
#define INVENTORY_H
#include "navigation.h"
#include <stdbool.h>

typedef struct {
    char name[32];
    int weight; // 0 for Scenario A
} Item;

typedef struct {
    Item items[64];
    int count;
} Recipe;

typedef struct {
    Item carried[64];
    int carried_count;
    int carried_weight;
    int capacity; // 0 for Scenario A (no limit)
} Bag;

// Load recipe file. If weights present, fills weight; else weight=0.
// Returns 1 on success.
int load_recipe(const char* path, Recipe* recipe, int* scenarioB, int* total_weight);

// Load ingredient map: returns number of entries read. Stores up to names[], rows[], cols[].
int load_ingredients_map(const char* path, char names[][32], int rows[], int cols[], int maxn);

// Find coordinates by name; returns 1 if found.
int lookup_coord(const char* name, char names[][32], int rows[], int cols[], int n, Position* out);

// Collect current item; returns true if item collected, false if would overflow Bag (Scenario B).
bool collect_item(const Item* item, Bag* bag);

// Drop all carried items at Kitchen (empties bag).
void drop_all(Bag* bag);

#endif
