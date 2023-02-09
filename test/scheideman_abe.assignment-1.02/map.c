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

    printf("\n");
}

/* 
 * Place a 2x2 grid for the building, roads around the building, 
 * and connect a road to the west to east road.   
 */
void building_to_map(map_t *map, terrain_e building, int y, int x, int gate_row) {
    int building_connected = 0;

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

    if (y > gate_row + 1 && y > 3) { // pave road north to connect to west east
        y--;
        while (y > 1 && map->terrain[y - 1][x] != road) {
            y--;
            map->terrain[y][x] = road;
        }

        if (y != 1) {
            building_connected = 1;
        }
    } else if (y < gate_row - 2 && y < HEIGHT - 4) { // pave road south to connect to west east
        y += 2;
        while (y < HEIGHT - 2 && map->terrain[y + 1][x] != road) {
            y++;
            map->terrain[y][x] = road;
        }

        if (y != HEIGHT - 2) {
            building_connected = 1;
        }
    }

    // Handle corner cases and east and west edge cases
    if (building_connected == 0) { // must be in a corner or edge, so try to connect
        if (map->w > 0) { // If there is a west gate, move towards it
            while (x > 0 && map->terrain[y][x - 1] != road) {
                x--;
                map->terrain[y][x] = road;
            }

            if (x > 0) {
                building_connected = 1;
            }
        } else { // Otherwise go to east gate
            while (x < WIDTH - 1 && map->terrain[y][x + 1] != road) {
                x++;
                map->terrain[y][x] = road;
            }

            if (x < WIDTH - 1) {
                building_connected = 1;
            }
        }

        if (map->n > 0 && building_connected == 0) { // move towards north gate
            while (y > 1 && map->terrain[y - 1][x] != road) {
                y--;
                map->terrain[y][x] = road;
            }
        } else if (building_connected == 0) { // move towards south gate
            while (y < HEIGHT - 1 && map->terrain[y + 1][x] != road) {
                y++;
                map->terrain[y][x] = road;
            }
        }
    }
}

void place_buildings(map_t *map, int manhattan_distance) {
    terrain_e first_building, second_building; 
    int i, y, x;

    i = rand() % 2;

    first_building = (i < 1) ? pokemart : pokecenter;
    second_building = (first_building == pokemart) ? pokecenter : pokemart;

    int building_probability = ((-45 * manhattan_distance) / 200 + 50);

    // place first building in left half of the map
    if (manhattan_distance == 0 || rand() % 100 < building_probability) {
        y = rand() % 16 + 2; // rows 2 - 17
        x = rand() % 36 + 2; // cols 2 - 37

        building_to_map(map, first_building, y, x, map->w);
    }

    if (manhattan_distance == 0 || rand() % 100 < building_probability) {
        // place second building in the right half of the map
        y = rand() % 16 + 2; // rows 2 - 17
        x = rand() % 36 + 41; // cols 41 - 76

        building_to_map(map, second_building, y, x, map->e);
    }
}

/* Connect the gates with one road going north to south and another going west to east. */
void pave_roads(map_t *map)
{
    int x_run = map->s - map->n;
    int row, col;

    row = 1;
    col = map->n;

    // pave north to south road
    if (map->n > 0 && map->s > 0) {
        while (row < (HEIGHT - 2) / 2) { // pave road halfway down
            map->terrain[row][col] = road;
            row++;
        }

        if (x_run < 0) { // south gate is to the west
            while (col > map->s) {
                map->terrain[row][col] = road;
                col--;
            }
        } else if (x_run > 0) { // south gate is to the east
            while (col < map->s) {
                map->terrain[row][col] = road;
                col++;
            }
        }

        while (row < HEIGHT - 1) { // pave road the rest of the way down
            map->terrain[row][col] = road;
            row++;
        }
    }

    int y_rise = map->e - map->w;

    col = 1;
    row = map->w;

    // pave west to east road
    if (map->w > 0 && map->e > 0) {
        while (col < (WIDTH - 2) / 2) { // pave road halfway east
            map->terrain[row][col] = road;
            col++;
        }

        if (y_rise < 0) { // east gate is to the north
            while (row > map->e) {
                map->terrain[row][col] = road;
                row--;
            }
        } else if (y_rise > 0) { // east gate is to the south
            while (row < map->e) {
                map->terrain[row][col] = road;
                row++;
            }
        }

        while (col < WIDTH - 1) { // pave road the rest of the way over
            map->terrain[row][col] = road;
            col++;
        }
    }
}

/* Handle cases for paving the roads where the current map is on the edge or corner of the world */
void pave_roads_edge(map_t *map)
{
    int row, col;

    if (map->n < 0) {
        col = map->s;

        if (map->w < 0) { // northwest corner
            for (row = HEIGHT - 2; row > map->e; row--) {
                map->terrain[row][col] = road;
            }

            for (col = map->s; col < WIDTH - 1; col++) {
                map->terrain[row][col] = road;
            }
        } else if (map->e < 0) { // northeast corner
            for (row = HEIGHT - 2; row > map->w; row--) {
                map->terrain[row][col] = road;
            }

            for (col = map->s; col > 0; col--) {
                map->terrain[row][col] = road;
            }
        } else { // along north edge
            pave_roads(map); // pave west to east road

            // pave south road up to west-east road
            row = HEIGHT - 2;
            while (map->terrain[row][col] != road && row > 0) {
                map->terrain[row][col] = road;
                row--;
            }
        }
    } else if (map->s < 0) {
        col = map->n;

        if (map->e < 0) { // southeast corner
            for (row = 0; row < map->w; row++) {
                map->terrain[row][col] = road;
            }

            for (col = map->n; col > 0; col--) {
                map->terrain[row][col] = road;
            }
        } else if (map->w < 0) { // southwest corner
            for (row = 0; row < map->e; row++) {
                map->terrain[row][col] = road;
            }

            for (col = map->n; col < WIDTH - 1; col++) {
                map->terrain[row][col] = road;
            }
        } else { // south edge
            pave_roads(map); // pave west to east road

            // pave north road down to west-east road
            row = 0;
            while (map->terrain[row][col] != road && row < HEIGHT - 1) {
                map->terrain[row][col] = road;
                row++;
            }
        }
    } else if (map->w < 0) { // west edge of map
        pave_roads(map); //pave north to south road

        row = map->e;
        col = WIDTH - 2;

        // pave road from east gate to north-south road
        while (map->terrain[row][col] != road && col > 0) {
            map->terrain[row][col] = road;
            col--;
        }
    } else if (map->e < 0) { // east edge of map
        pave_roads(map); //pave north to south road

        row = map->w;
        col = 1;

        // pave road from east gate to north-south road
        while (map->terrain[row][col] != road && col < WIDTH - 1) {
            map->terrain[row][col] = road;
            col++;
        }
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

    // now add some spontaneous terrain
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

/* Fill in the border of the given map with boulders and gates. */
void construct_border(map_t *map, int n, int s, int w, int e)
{
    // First make boulders the entire border, then place the 4 gates
    int i, j;

    for (i = 0; i < HEIGHT; i++) {
        map->terrain[i][0] = boulder;
        map->terrain[i][WIDTH - 1] = boulder;
    }

    for (j = 0; j < WIDTH; j++) {
        map->terrain[0][j] = boulder;
        map->terrain[HEIGHT - 1][j] = boulder;
    }

    // 0 means generate a gate
    map->n = (n == 0) ? rand() % (WIDTH - 2) + 1 : n; // Minus 2 and plus 1 to ensure corners aren't gates
    map->s = (s == 0) ? rand() % (WIDTH - 2) + 1 : s;
    map->w = (w == 0) ? rand() % (HEIGHT - 2) + 1 : w;
    map->e = (e == 0) ? rand() % (HEIGHT - 2) + 1 : e;

    // -1 means don't generate a gate
    map->terrain[0][map->n] = (n == -1) ? boulder : gate;
    map->terrain[HEIGHT - 1][map->s] = (s == -1) ? boulder : gate;
    map->terrain[map->w][0] = (w == -1) ? boulder : gate;
    map->terrain[map->e][WIDTH - 1] = (e == -1) ? boulder : gate;
}

/* Take the given map and populate it with the necessary types of terrain, roads, and buildings. */
void generate_map(map_t *map, int n, int s, int w, int e, int manhattan_distance)
{
    srand(time(NULL));

    construct_border(map, n, s, w, e);
    
    generate_terrain(map);

    if (n < 0 || s < 0 || w < 0 || e < 0) { // if we are by an edge, handle roads specially
        pave_roads_edge(map);
    } else {
        pave_roads(map);
    }

    place_buildings(map, manhattan_distance);
}
