//What do I need structs for...
// struct of seeds then a method that uses those seeds to grow the map
// Important info: terrain seeds, exits, pokecenter, pokemart, 
// Terrain has Regions, Exits, Roads, and Marts. 
// Regions can be of type short grass, tall grass, mountain, forest, or water. Each region a list of each coordinates where it exists.
// Each exit needs coordinates
// Roads will need ex

// Too complicated:
/*
Structs needed: coordinate, terrain which has the 2d array, exits struct 
Need enums for each terrain type.
couple methods to generate terrain, print terrain  
*/

#ifndef TERRAIN_GENERATION_H
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

int generate_map(map_t *map);

void print_map(map_t *map);

#endif
