#ifndef VALIDATION_H
#define VALIDATION_H
#include "inventory.h"
#include <stdbool.h>

// Validate collected items (in bag_history) contain all items in recipe in order.
bool validate_ingredients(const Recipe* recipe, const Item* history, int history_count);

#endif
