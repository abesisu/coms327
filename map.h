#ifndef MAP_H
#define MAP_H

#include "heap.h"
#include "util.h"
#include "trainer.h"

typedef enum {
    grass,
    tall_grass,
    boulder,
    edge,
    tree,
    willow,
    water,
    road,
    pokemart,
    pokecenter,
    gate,
    bridge,
} terrain_e;



typedef struct map {
    terrain_e terrain[MAP_HEIGHT][MAP_WIDTH];
    int n, s, w, e; // gates

    trainer_t *trainer_map[MAP_HEIGHT][MAP_WIDTH];
    coordinate_t pc_pos;
    heap_t *turn_heap;
    int pc_turn;
} map_t;

void generate_map(map_t *map, int n, int s, int w, int e, int manhattan_distance);

void map_init(map_t *map);

void trainer_map_init(map_t *map, int num_trainers, trainer_t *pc);

void map_delete(map_t *map);

#endif
