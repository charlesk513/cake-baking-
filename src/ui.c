#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h> // usleep
#include "ui.h"
#include "colors.h"

static void clear_screen(){
    printf("\x1b[H\x1b[2J");
}

void draw_bar(const char* label, int v, int max, int width){
    if(width<1) width=10;
    int fill = (max>0? (v*width)/max : width);
    if(fill>width) fill=width;
    printf("%-10s [", label);
    for(int i=0;i<width;i++) putchar(i<fill? '=' : ' ');
    printf("] %d/%d\n", v, max);
}

void ui_log(const UiOpts* opts, const char* fmt, ...){
    if(!opts) return;
    if(opts->verbosity>0){
        va_list ap;
        va_start(ap, fmt);
        vprintf(fmt, ap);
        va_end(ap);
    }
    if(opts->log_path){
        FILE* lf = fopen(opts->log_path, "a");
        if(lf){
            va_list ap;
            va_start(ap, fmt);
            vfprintf(lf, fmt, ap);
            va_end(ap);
            fclose(lf);
        }
    }
}

void draw_grid(int house[NROWS][NCOLS],
               Position pos,
               Position kitchen,
               Position target,
               const int *overlay,
               int bag_weight, int bag_cap, int trip_no,
               const char *current_item,
               int step_no, int total_steps,
               const UiOpts* opts){
    if(!opts || !opts->animate) return;
    clear_screen();
    printf("Step %d/%d | Item: %s | Trip %d | Bag %d/%d\n\n",
           step_no, total_steps, current_item?current_item:"-", trip_no, bag_weight, bag_cap);

    for(int r=0;r<NROWS;r++){
        for(int c=0;c<NCOLS;c++){
            int isWall = house[r][c]==0;
            int onOverlay = (overlay? overlay[r*NCOLS+c] : 0);
            if(r==pos.r && c==pos.c)               printf(C_YELLOW "@ " C_RESET);
            else if(r==kitchen.r && c==kitchen.c)  printf(C_GREEN  "K " C_RESET);
            else if(r==target.r && c==target.c)    printf(C_CYAN   "T " C_RESET);
            else if(isWall)                         printf(C_DIM    "# " C_RESET);
            else if(onOverlay)                      printf(C_BLUE   "• " C_RESET);
            else                                    printf("· ");
        }
        printf("\n");
    }
    printf("\n");
    draw_bar("Bag", bag_weight, bag_cap>0?bag_cap:bag_weight+1, 24);
    usleep(opts->delay_ms * 1000);
}
