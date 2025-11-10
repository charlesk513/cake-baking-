#include <stdio.h>
#include <string.h>
#include "navigation.h"

/**
 * Task 1: Implement move() function
 *
 * This function should move the robot step-by-step in the given direction for the specified number of steps.
 * For each step:
 *   1. Calculate the next cell based on the direction (UP, DOWN, LEFT, RIGHT).
 *   2. Check if the next cell is within bounds (0 <= row < NROWS, 0 <= col < NCOLS).
 *   3. Check if the next cell is not a wall (house[next.r][next.c] != 0).
 *   4. If either check fails, stop and return the current position.
 *   5. If valid, update the position and continue.
 * The function should return the final position after attempting all steps or stopping early if blocked.
 */
Position move(enum Direction dir, int steps, Position start, int house[NROWS][NCOLS]){
    Position p = start;
    for(int s=0;s<steps;s++){
        Position next = p;
        // TODO: Implement direction and bounds checking logic below
        // 1. Update 'next' based on 'dir' (UP, DOWN, LEFT, RIGHT)
        // 2. Check if 'next' is within bounds
        // 3. Check if 'next' is not a wall (house[next.r][next.c] != 0)
        // If any check fails, return p
        // If valid, update p = next
            printf("Task 1: move() is not complete.\n");
            // Return start position to indicate failure (no move)
        return p;
    }
    return p;
}

// BFS over grid to find a shortest path (no diagonals).
typedef struct { int r,c; } Node;
static int dr[4]={-1,1,0,0};
static int dc[4]={0,0,-1,1};

size_t find_path(Position start, Position goal, int house[NROWS][NCOLS], enum Direction *dirs, size_t max_dirs){
    int visited[NROWS][NCOLS]; memset(visited,0,sizeof(visited));
    Node prev[NROWS][NCOLS]; for(int r=0;r<NROWS;r++)for(int c=0;c<NCOLS;c++){prev[r][c].r=-1;prev[r][c].c=-1;}

    Node q[NROWS*NCOLS]; int h=0,t=0;
    q[t++] = (Node){start.r,start.c};
    visited[start.r][start.c]=1;

    while(h<t){
        Node u=q[h++];
        if(u.r==goal.r && u.c==goal.c) break;
        for(int k=0;k<4;k++){
            int nr=u.r+dr[k], nc=u.c+dc[k];
            if(nr<0||nr>=NROWS||nc<0||nc>=NCOLS) continue;
            if(house[nr][nc]==0) continue;
            if(!visited[nr][nc]){
                visited[nr][nc]=1;
                prev[nr][nc]=u;
                q[t++]=(Node){nr,nc};
            }
        }
    }
    if(!visited[goal.r][goal.c]) return 0;

    enum Direction rev[ NROWS*NCOLS ];
    size_t len=0;
    Node cur = (Node){goal.r,goal.c};
    while(!(cur.r==start.r && cur.c==start.c)){
        Node p = prev[cur.r][cur.c];
        if(cur.r==p.r-1 && cur.c==p.c) rev[len++]=UP;
        else if(cur.r==p.r+1 && cur.c==p.c) rev[len++]=DOWN;
        else if(cur.r==p.r && cur.c==p.c-1) rev[len++]=LEFT;
        else if(cur.r==p.r && cur.c==p.c+1) rev[len++]=RIGHT;
        cur = p;
    }
    if(len>max_dirs) return 0;
    for(size_t i=0;i<len;i++) dirs[i]=rev[len-1-i];
    return len;
}

int load_house_layout(const char* path, int house[NROWS][NCOLS]){
    FILE* f=fopen(path,"r"); if(!f) return 0;
    int R=0,C=0;
    // skip comments to get R C
    char line[256];
    while(fgets(line,sizeof(line),f)){
        if(line[0]=='#' || line[0]=='\n') continue;
        if(sscanf(line,"%d %d",&R,&C)==2) break;
    }
    for(int r=0;r<R && r<NROWS;r++){
        for(int c=0;c<C && c<NCOLS;c++){
            int v=0; if(fscanf(f,"%d",&v)==1) house[r][c]=v;
        }
    }
    fclose(f);
    return 1;
}
