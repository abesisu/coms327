#ifndef MAP_H
#define MAP_H
#define MAP_WIDTH 80
#define MAP_HEIGHT 21
#define WORLD_WIDTH 401
#define WORLD_HEIGHT 401
#define START_X 200
#define START_Y 200

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
    pc,
    hiker,
    rival,
} terrain_e;

typedef struct map {
    terrain_e terrain[MAP_HEIGHT][MAP_WIDTH];
    int n, s, w, e; // gates
} map_t;

void generate_map(map_t *map, int n, int s, int w, int e, int manhattan_distance);

void map_init(map_t *map);

#endif
