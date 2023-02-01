#ifndef MAP_H
#define WIDTH 80
#define HEIGHT 21

typedef enum terrain {
    grass,
    tall_grass,
    boulder,
    tree,
    water,
    road,
    pokemart,
    pokecenter,
    gate,
} terrain_e;

typedef struct gates {
    int north;
    int east;
    int south;
    int west;
} gates_t;

typedef struct map {
    terrain_e terrain[HEIGHT][WIDTH];
    gates_t gates;
} map_t;

void generate_map(map_t *map);

void print_map(map_t *map);

#endif
