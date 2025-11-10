#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "inventory.h"

int load_recipe(const char* path, Recipe* recipe, int* scenarioB, int* total_weight){
    FILE* f=fopen(path,"r"); if(!f) return 0;
    recipe->count=0; *scenarioB=0; *total_weight=0;
    char line[128];
    while(fgets(line,sizeof(line),f)){
        if(line[0]=='#'||strlen(line)<2) continue;
        char name[32]; int w=0;
        int n=sscanf(line,"%31s %d",name,&w);
        if(n==2){ *scenarioB=1; }
        Item it; memset(&it,0,sizeof(it)); strncpy(it.name,name,31); it.name[31]='\0';
        it.weight=(n==2? w:0);
        recipe->items[recipe->count++]=it;
        *total_weight += it.weight;
    }
    fclose(f);
    return 1;
}

int load_ingredients_map(const char* path, char names[][32], int rows[], int cols[], int maxn){
    FILE* f=fopen(path,"r"); if(!f) return 0;
    int n=0; char line[128];
    while(fgets(line,sizeof(line),f)){
        if(line[0]=='#'||strlen(line)<2) continue;
        if(n>=maxn) break;
        char nm[32]; int r,c;
        if(sscanf(line,"%31s %d %d",nm,&r,&c)==3){
            strncpy(names[n],nm,31); names[n][31]='\0';
            rows[n]=r; cols[n]=c; n++;
        }
    }
    fclose(f);
    return n;
}

int lookup_coord(const char* name, char names[][32], int rows[], int cols[], int n, Position* out){
    for(int i=0;i<n;i++){
        if(strcmp(name,names[i])==0){ out->r=rows[i]; out->c=cols[i]; return 1; }
    }
    return 0;
}

/**
     * Task 2: Implement collect_item() function
     *
     * This function should add the given item to the bag if there is enough capacity.
     * For Scenario B, update the bag's carried_weight and carried_count.
     * Print a message if needed to confirm collection.
     *
     * TODO: Add the item to the bag and update the carried weight below.
     */
        
bool collect_item(const Item* item, Bag* bag){
    if(bag->capacity>0 && bag->carried_weight + item->weight > bag->capacity){
        return false;
    }
    // TODO: Add the item to the bag and update carried_weight
    printf("Task 2: collect_item() is not complete.\n");
    return false; // Indicate failure
    
}

void drop_all(Bag* bag){
    bag->carried_count=0;
    bag->carried_weight=0;
}
