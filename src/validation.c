#include <string.h>
#include "validation.h"

bool validate_ingredients(const Recipe* recipe, const Item* history, int history_count){
    if(history_count != recipe->count) return false;
    for(int i=0;i<recipe->count;i++){
        if(strcmp(recipe->items[i].name, history[i].name)!=0) return false;
        if(recipe->items[i].weight!=0 && recipe->items[i].weight != history[i].weight) return false;
    }
    return true;
}
