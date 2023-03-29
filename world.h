#ifndef WORLD_H
#define WORLD_H

#include "action.h"

typedef struct world {
    map_t *board[WORLD_HEIGHT][WORLD_WIDTH];
    map_t *current_map;
    coordinate_t location;

    path_t hiker_path[MAP_HEIGHT][MAP_WIDTH];
    path_t rival_path[MAP_HEIGHT][MAP_WIDTH];
    
    int num_trainers;
} world_t;

void world_init(world_t *world, int num_trainers);

void world_delete(world_t *world);

#endif
