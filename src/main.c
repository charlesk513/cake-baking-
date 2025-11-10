#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "navigation.h"
#include "inventory.h"
#include "validation.h"
#include "ui.h"
#include "colors.h"

typedef struct { Item arr[256]; int n; } History;

int main(int argc, char** argv){
    UiOpts opts = { .animate=0, .delay_ms=120, .verbosity=1, .log_path=NULL };
    if(argc < 3){
        fprintf(stderr, "Usage: %s <ingredients_map.txt> <recipe_file.txt> [--animate] [--delay=ms] [--quiet|--debug] [--log=path]\n", argv[0]);
        return 1;
    }
    const char* map_path = argv[1];
    const char* recipe_path = argv[2];

    for(int i=3;i<argc;i++){
        if(strcmp(argv[i],"--animate")==0) opts.animate=1;
        else if(strncmp(argv[i],"--delay=",8)==0) opts.delay_ms=atoi(argv[i]+8);
        else if(strcmp(argv[i],"--quiet")==0) opts.verbosity=0;
        else if(strcmp(argv[i],"--debug")==0) opts.verbosity=2;
        else if(strncmp(argv[i],"--log=",6)==0) opts.log_path=argv[i]+6;
    }

    int house[NROWS][NCOLS];
    if(!load_house_layout("house_layout.txt", house)){
        fprintf(stderr, "Failed to load house_layout.txt\n");
        return 1;
    }

    char names[128][32]; int rows[128]; int cols[128];
    int nmap = load_ingredients_map(map_path, names, rows, cols, 128);
    if(nmap <= 0){ fprintf(stderr, "Failed to load ingredients map\n"); return 1; }

    Recipe recipe={0}; int scenarioB=0; int totalW=0;
    if(!load_recipe(recipe_path, &recipe, &scenarioB, &totalW)){
        fprintf(stderr, "Failed to load recipe %s\n", recipe_path); return 1;
    }

    Bag bag = { .carried_count=0, .carried_weight=0, .capacity = 0 };
    if(scenarioB){
        bag.capacity = (totalW*6 + 9)/10; // ceil(0.6*total)
        if(bag.capacity <= 0) bag.capacity = totalW; // fallback
        ui_log(&opts, C_CYAN "Scenario B: total recipe weight=%d, bag capacity=%d\n" C_RESET, totalW, bag.capacity);
    } else {
        ui_log(&opts, C_CYAN "Scenario A: no bag capacity limit.\n" C_RESET);
    }

    Position kitchen = (Position){0,0};
    Position pos = kitchen;
    History history = { .n=0 };
    int trip_no = 1;

    for(int i=0;i<recipe.count;i++){
        Item it = recipe.items[i];
        Position target;
        if(!lookup_coord(it.name, names, rows, cols, nmap, &target)){
            fprintf(stderr, "No coordinates for ingredient '%s'\n", it.name);
            return 1;
        }

        if(bag.capacity>0 && bag.carried_weight + it.weight > bag.capacity){
            ui_log(&opts, C_YELLOW "📦 Bag would overflow before %s — returning to Kitchen.\n" C_RESET, it.name);
              // TODO: Implement logic to return to Kitchen when bag would overflow.
              // 1. Plan a path back to the kitchen (0,0)
              // 2. Move the robot along that path
              // 3. Drop all items from the bag
              // 4. Increment the trip counter
              printf("Task 4: Return-to-Kitchen logic is not complete.\n");
              return 1; // Indicate failure
        }

        enum Direction path[512]; size_t steps = find_path(pos, target, house, path, 512);
        if(steps==0){
            fprintf(stderr,"No path to %s at (%d,%d)\n", it.name, target.r, target.c);
            return 1;
        }
        int overlay[NROWS*NCOLS]; for(int iov=0;iov<NROWS*NCOLS;iov++) overlay[iov]=0;
        Position sim = pos;
        for(size_t s=0;s<steps;s++){
            if(path[s]==UP) sim.r--; else if(path[s]==DOWN) sim.r++; else if(path[s]==LEFT) sim.c--; else if(path[s]==RIGHT) sim.c++;
            if(sim.r>=0 && sim.r<NROWS && sim.c>=0 && sim.c<NCOLS) overlay[sim.r*NCOLS+sim.c]=1;
        }
        draw_grid(house, pos, kitchen, target, overlay, bag.carried_weight, bag.capacity, trip_no, it.name, 0, (int)steps, &opts);

        for(size_t s=0;s<steps;s++){
            pos = move(path[s], 1, pos, house);
            draw_grid(house, pos, kitchen, target, NULL, bag.carried_weight, bag.capacity, trip_no, it.name, (int)(s+1), (int)steps, &opts);
        }

        if(!collect_item(&it, &bag)){
            ui_log(&opts, C_YELLOW "📦 At target but bag overflow. Returning to Kitchen.\n" C_RESET);
            enum Direction back[512]; size_t backsteps = find_path(pos, kitchen, house, back, 512);
            for(size_t s=0;s<backsteps;s++){ pos = move(back[s], 1, pos, house); draw_grid(house, pos, kitchen, kitchen, NULL, bag.carried_weight, bag.capacity, trip_no, "-", (int)(s+1), (int)backsteps, &opts); }
            drop_all(&bag); trip_no++;
            steps = find_path(pos, target, house, path, 512);
            for(size_t s=0;s<steps;s++){ pos = move(path[s], 1, pos, house); draw_grid(house, pos, kitchen, target, NULL, bag.carried_weight, bag.capacity, trip_no, it.name, (int)(s+1), (int)steps, &opts); }
            if(!collect_item(&it, &bag)){
                fprintf(stderr,"Unexpected overflow collecting %s\n", it.name);
                return 1;
            }
        }
        ui_log(&opts, C_GREEN "🧺 Collected %s%s\n" C_RESET, it.name, (it.weight? " (weighted)":""));
        history.arr[history.n++] = it;
    }

    enum Direction pathend[512]; size_t stpend = find_path(pos, kitchen, house, pathend, 512);
    for(size_t s=0;s<stpend;s++){ pos = move(pathend[s], 1, pos, house); draw_grid(house, pos, kitchen, kitchen, NULL, bag.carried_weight, bag.capacity, trip_no, "-", (int)(s+1), (int)stpend, &opts); }
    if(bag.capacity>0) drop_all(&bag);

    if(validate_ingredients(&recipe, history.arr, history.n)){
        ui_log(&opts, C_CYAN "✅ Recipe complete and validated.\n" C_RESET);
        return 0;
    } else {
        ui_log(&opts, C_RED "❌ Validation failed.\n" C_RESET);
        return 2;
    }
}
