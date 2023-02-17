#ifndef PATH_H
#define PATH_H

#include "heap.h"
#include "map.h"

typedef struct coordinate {
    int x;
    int y;
} coordinate_t;

typedef enum trainer_type {
    pc_train,
    hiker_train,
    rival_train,
    swimmer_train,
    other,
} trainer_type_e;

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

typedef struct trainer {
    trainer_type_e type;
    coordinate_t position;
} trainer_t;

void dijkstra_path(map_t *map, path_t path[MAP_HEIGHT][MAP_WIDTH], trainer_type_e trainer_type, coordinate_t to);

#endif //COMS327_PATH_H
