#ifndef WORLD_H
#define WORLD_H

#include "action.h"

typedef struct world {
    map_t *board[WORLD_HEIGHT][WORLD_WIDTH];
    map_t *current_map;
    coordinate_t location;

    path_t hiker_path[MAP_HEIGHT][MAP_WIDTH];
    path_t rival_path[MAP_HEIGHT][MAP_WIDTH];

    char view[MAP_HEIGHT][MAP_WIDTH + 1];
} world_t;

void world_init(heap_t *turn_heap, world_t *world, int num_trainers);

void world_delete(world_t *world);

#endif
