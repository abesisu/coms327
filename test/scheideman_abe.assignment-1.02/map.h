#ifndef MAP_H
#define WIDTH 80
#define HEIGHT 21
#define WORLD_WIDTH 401
#define WORLD_HEIGHT 401
#define START_X 200
#define START_Y 200

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

typedef struct coordinate {
    int x;
    int y;
} coordinate_t;

typedef struct map {
    terrain_e terrain[HEIGHT][WIDTH];
    int n, s, w, e; // gates
} map_t;

typedef struct world {
    map_t *board[WORLD_HEIGHT][WORLD_WIDTH];
    map_t *current_map;
    coordinate_t location;
} world_t;

void generate_map(map_t *map, int n, int s, int w, int e, int manhattan_distance);

void print_map(map_t *map);

#endif
