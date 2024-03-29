#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "map.h"

void print_map(map_t *map)
{
    int i, j;

    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            switch (map->terrain[i][j])
            {
            case grass:
                printf(".");
                break;
            case tall_grass:
                printf(":");
                break;
            case boulder:
                printf("%%");
                break;
            case tree:
                printf("^");
                break;
            case water:
                printf("~");
                break;
            case road:
                printf("#");
                break;
            case gate:
                printf("#");
                break;
            case pokemart:
                printf("M");
                break;
            case pokecenter:
                printf("C");
                break;
            default:
                printf("!"); // Something went wrong
                break;
            }
        }
    }
}

/* 
 * Place a 2x2 grid for the building, roads around the building, 
 * and connect a road to the west to east road.   
 */
void building_to_map(map_t *map, terrain_e building, int y, int x, int gate_row) {
    map->terrain[y][x] = building;
    map->terrain[y + 1][x] = building;
    map->terrain[y][x + 1] = building;
    map->terrain[y + 1][x + 1] = building;

    map->terrain[y - 1][x] = road;
    map->terrain[y - 1][x + 1] = road; 
    map->terrain[y - 1][x + 2] = road;
    map->terrain[y][x + 2] = road;
    map->terrain[y + 1][x + 2] = road;
    map->terrain[y + 2][x + 2] = road;
    map->terrain[y + 2][x + 1] = road;
    map->terrain[y + 2][x] = road;
    map->terrain[y + 2][x - 1] = road;
    map->terrain[y + 1][x - 1] = road;
    map->terrain[y][x - 1] = road;
    map->terrain[y - 1][x - 1] = road;

    if (y > gate_row + 1 && y > 3) {
        y--;
        while (y > 1 && map->terrain[y - 1][x] != road) {
            y--;
            map->terrain[y][x] = road;
        }
    } else if (y < gate_row - 2 && y < HEIGHT - 4) {
        y += 2;
        while (y < HEIGHT - 2 && map->terrain[y + 1][x] != road) {
            y++;
            map->terrain[y][x] = road;
        }
    }
}

void place_buildings(map_t *map) {
    terrain_e first_building, second_building; 
    int i, y, x;

    i = rand() % 2;

    first_building = (i < 1) ? pokemart : pokecenter;
    second_building = (first_building == pokemart) ? pokecenter : pokemart;

    // place first building in left half of the map
    y = rand() % 16 + 2; // rows 2 - 17
    x = rand() % 36 + 2; // cols 2 - 37
    
    building_to_map(map, first_building, y, x, map->gates.west);

    // place second building in the right half of the map 
    x = rand() % 36 + 41; // cols 41 - 76

    building_to_map(map, second_building, y, x, map->gates.east);
}

/* Connect the the gates with one road going north to south and another going west to east. */
void pave_roads(map_t *map)
{
    int x_run = map->gates.south - map->gates.north;
    int row, col;

    row = 1;
    col = map->gates.north;

    // pave north to south road
    while (row < (HEIGHT - 2) / 2) { // pave road halfway down
        map->terrain[row][col] = road;
        row++;
    }

    if (x_run < 0) { // south gate is to the west
        while (col > map->gates.south) {
            map->terrain[row][col] = road;
            col--;
        }
    } else if (x_run > 0) { // south gate is to the east
        while (col < map->gates.south) {
            map->terrain[row][col] = road;
            col++;
        }
    }

    while (row < HEIGHT - 1) { // pave road the rest of the way down
        map->terrain[row][col] = road;
        row++;
    }

    int y_rise = map->gates.east - map->gates.west;

    col = 1;
    row = map->gates.west;

    // pave west to east road
    while (col < (WIDTH - 2) / 2) { // pave road halfway east
        map->terrain[row][col] = road;
        col++;
    }

    if (y_rise < 0) { // east gate is to the north
        while (row > map->gates.east) {
            map->terrain[row][col] = road;
            row--;
        }
    } else if (y_rise > 0) { // east gate is to the south
        while (row < map->gates.east) {
            map->terrain[row][col] = road;
            row++;
        }
    }

    while (col < WIDTH - 1) { // pave road the rest of the way over
        map->terrain[row][col] = road;
        col++;
    }
}

/* Generate the regions in each half of the map to get a better terrain distribution. */
void generate_regional_terrain(map_t *map, int west_bound, int east_bound)
{
    int x, y, region_width, region_height; 
    int i, row, col;
    terrain_e regions[5] = {boulder, tree, water, tall_grass, tall_grass};

    for (i = 0; i < 5; i++) {
        // get a random coordinate in the range
        x = rand() % (east_bound - west_bound);
        x += west_bound;
        y = rand() % (HEIGHT - 3) + 2;

        // get a specified size for the region
        region_width = rand() % 30 + 10;
        region_height = rand() % 6 + 5;

        for (row = 0; row < region_height; row++) {
            if (y + row < HEIGHT - 1) { // row is in bounds
                for (col = 0; col < region_width; col++) {
                    if (x + col < WIDTH - 1) { // column is in bounds
                        map->terrain[y + row][x + col] = regions[i];
                    } else { // column out of bounds
                        map->terrain[y + row][x - col] = regions[i];
                    }
                }
            } else { // row out of bounds
                for (col = 0; col < region_width; col++) {
                    if (x + col < WIDTH - 1) { // column is in bounds
                        map->terrain[y - row][x + col] = regions[i];
                    } else { // column out of bounds
                        map->terrain[y - row][x - col] = regions[i];
                    }
                }
            }
        }
    }
}

/* Fill in the terrain for the map. */
void generate_terrain(map_t *map)
{
    generate_regional_terrain(map, 1, WIDTH / 2 - 1); // covers cols 1-38
    generate_regional_terrain(map, WIDTH / 2 - 1, WIDTH - 1); // covers cols 39-78

    // now add some sporatic terrain
    int i, row, col;

    for (i = 0; i < 20; i++) {
        row = rand() % (HEIGHT - 2) + 1;
        col = rand() % (WIDTH - 2) + 1;

        map->terrain[row][col] = boulder;

        row = rand() % (HEIGHT - 2) + 1;
        col = rand() % (WIDTH - 2) + 1;

        map->terrain[row][col] = tree;
    }
}

/* Fill in the edge of the given map with boulders and gates. */
void construct_border(map_t *map)
{
    // First make boulders the entire edge, then randomly pick the 4 gates.
    int i, j;

    for (i = 0; i < HEIGHT; i++) {
        map->terrain[i][0] = boulder;
        map->terrain[i][WIDTH - 1] = boulder;
    }

    for (j = 0; j < WIDTH; j++) {
        map->terrain[0][j] = boulder;
        map->terrain[HEIGHT - 1][j] = boulder;
    }

    map->gates.north = rand() % (WIDTH - 2) + 1; // Minus 2 and plus 1 to ensure corners aren't gates.
    map->gates.south = rand() % (WIDTH - 2) + 1;
    map->gates.east = rand() % (HEIGHT - 2) + 1;
    map->gates.west = rand() % (HEIGHT - 2) + 1;

    map->terrain[0][map->gates.north] = gate;
    map->terrain[HEIGHT - 1][map->gates.south] = gate;
    map->terrain[map->gates.east][WIDTH - 1] = gate;
    map->terrain[map->gates.west][0] = gate;
}

/* Take the given struct and populate it with the necessary types of terrain, roads, and buildings. */
void generate_map(map_t *map)
{
    srand(time(NULL));

    construct_border(map);
    
    generate_terrain(map);
    
    pave_roads(map);
    
    place_buildings(map);
}

/* Allocate memory for structs then call appropriate methods to generate the map. */
int main(int argc, char *argv[])
{
    map_t *map;

    map = malloc(sizeof (*map));

    generate_map(map);

    print_map(map);

    free(map);

    return 0;
}
