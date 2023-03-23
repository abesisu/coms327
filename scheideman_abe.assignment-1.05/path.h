#ifndef PATH_H
#define PATH_H

#include "map.h"

typedef struct terrain {
    terrain_e type;
    int cost;
} terrain_t;

typedef struct path {
    heap_node_t *heap_node; // The pointer node in heap/priority queue
    coordinate_t coordinate;
    terrain_t terrain;
    int cost; // Dijkstra's path cost at node
} path_t;

int get_terrain_cost(terrain_e terrain_type, trainer_type_e trainer_type);

void dijkstra_path(heap_t *heap, map_t *map, path_t path[MAP_HEIGHT][MAP_WIDTH], trainer_type_e trainer_type);

#endif
