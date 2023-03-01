#ifndef MAP_H
#define MAP_H

#include "heap.h"
#include "util.h"

typedef enum {
    grass,
    tall_grass,
    boulder,
    border,
    tree,
    willow,
    water,
    road,
    pokemart,
    pokecenter,
    gate,
    bridge,
} terrain_e;

typedef enum trainer_type {
    pc,
    hiker,
    rival,
    pacer,
    wanderer,
    sentry,
    explorer,
    swimmer,
} trainer_type_e;

typedef struct trainer {
    trainer_type_e type;
    int next_turn;
    int seq_num;
    coordinate_t pos;
    coordinate_t dir; // Which direction the trainer is headed
} trainer_t;

typedef struct map {
    terrain_e terrain[MAP_HEIGHT][MAP_WIDTH];
    int n, s, w, e; // gates

    trainer_t *trainer_map[MAP_HEIGHT][MAP_WIDTH];
} map_t;

void generate_map(map_t *map, int n, int s, int w, int e, int manhattan_distance);

void map_init(heap_t *turn_heap, map_t *map, int num_trainers);

#endif
