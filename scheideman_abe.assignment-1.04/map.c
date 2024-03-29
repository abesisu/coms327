#include <stdlib.h>

#include "map.h"
#include "trainer.h"

void build_bridges(map_t *map)
{
    int x, y;

    for (y = 1; y < MAP_HEIGHT - 1; y++) {
        for (x = 1; x < MAP_WIDTH - 1; x++) {
            if (map->terrain[y][x] == road) {
                if (map->terrain[y - 1][x    ] == water ||
                    map->terrain[y    ][x + 1] == water ||
                    map->terrain[y + 1][x    ] == water ||
                    map->terrain[y    ][x - 1] == water) {
                    map->terrain[y][x] = bridge;
                }
            }
        }
    }
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
    } else if (y < gate_row - 2 && y < MAP_HEIGHT - 4) { // pave road south to connect to west east
        y += 2;
        while (y < MAP_HEIGHT - 2 && map->terrain[y + 1][x] != road) {
            y++;
            map->terrain[y][x] = road;
        }

        if (y != MAP_HEIGHT - 2) {
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
            while (x < MAP_WIDTH - 1 && map->terrain[y][x + 1] != road) {
                x++;
                map->terrain[y][x] = road;
            }

            if (x < MAP_WIDTH - 1) {
                building_connected = 1;
            }
        }

        if (map->n > 0 && building_connected == 0) { // move towards north gate
            while (y > 1 && map->terrain[y - 1][x] != road) {
                y--;
                map->terrain[y][x] = road;
            }
        } else if (building_connected == 0) { // move towards south gate
            while (y < MAP_HEIGHT - 1 && map->terrain[y + 1][x] != road) {
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
    int y, x;

    y = 1;
    x = map->n;

    // pave north to south road
    if (map->n > 0 && map->s > 0) {
        while (y < (MAP_HEIGHT - 2) / 2) { // pave road halfway down
            map->terrain[y][x] = road;
            y++;
        }

        if (x_run < 0) { // south gate is to the west
            while (x > map->s) {
                map->terrain[y][x] = road;
                x--;
            }
        } else if (x_run > 0) { // south gate is to the east
            while (x < map->s) {
                map->terrain[y][x] = road;
                x++;
            }
        }

        while (y < MAP_HEIGHT - 1) { // pave road the rest of the way down
            map->terrain[y][x] = road;
            y++;
        }
    }

    int y_rise = map->e - map->w;

    x = 1;
    y = map->w;

    // pave west to east road
    if (map->w > 0 && map->e > 0) {
        while (x < (MAP_WIDTH - 2) / 2) { // pave road halfway east
            map->terrain[y][x] = road;
            x++;
        }

        if (y_rise < 0) { // east gate is to the north
            while (y > map->e) {
                map->terrain[y][x] = road;
                y--;
            }
        } else if (y_rise > 0) { // east gate is to the south
            while (y < map->e) {
                map->terrain[y][x] = road;
                y++;
            }
        }

        while (x < MAP_WIDTH - 1) { // pave road the rest of the way over
            map->terrain[y][x] = road;
            x++;
        }
    }
}

/* Handle cases for paving the roads where the current map is on the edge or corner of the world */
void pave_roads_edge(map_t *map)
{
    int y, x;

    if (map->n < 0) {
        x = map->s;

        if (map->w < 0) { // northwest corner
            for (y = MAP_HEIGHT - 2; y > map->e; y--) {
                map->terrain[y][x] = road;
            }

            for (x = map->s; x < MAP_WIDTH - 1; x++) {
                map->terrain[y][x] = road;
            }
        } else if (map->e < 0) { // northeast corner
            for (y = MAP_HEIGHT - 2; y > map->w; y--) {
                map->terrain[y][x] = road;
            }

            for (x = map->s; x > 0; x--) {
                map->terrain[y][x] = road;
            }
        } else { // along north edge
            pave_roads(map); // pave west to east road

            // pave south road up to west-east road
            y = MAP_HEIGHT - 2;
            while (map->terrain[y][x] != road && y > 0) {
                map->terrain[y][x] = road;
                y--;
            }
        }
    } else if (map->s < 0) {
        x = map->n;

        if (map->e < 0) { // southeast corner
            for (y = 0; y < map->w; y++) {
                map->terrain[y][x] = road;
            }

            for (x = map->n; x > 0; x--) {
                map->terrain[y][x] = road;
            }
        } else if (map->w < 0) { // southwest corner
            for (y = 0; y < map->e; y++) {
                map->terrain[y][x] = road;
            }

            for (x = map->n; x < MAP_WIDTH - 1; x++) {
                map->terrain[y][x] = road;
            }
        } else { // south edge
            pave_roads(map); // pave west to east road

            // pave north road down to west-east road
            y = 0;
            while (map->terrain[y][x] != road && y < MAP_HEIGHT - 1) {
                map->terrain[y][x] = road;
                y++;
            }
        }
    } else if (map->w < 0) { // west edge of map
        pave_roads(map); //pave north to south road

        y = map->e;
        x = MAP_WIDTH - 2;

        // pave road from east gate to north-south road
        while (map->terrain[y][x] != road && x > 0) {
            map->terrain[y][x] = road;
            x--;
        }
    } else if (map->e < 0) { // east edge of map
        pave_roads(map); //pave north to south road

        y = map->w;
        x = 1;

        // pave road from east gate to north-south road
        while (map->terrain[y][x] != road && x < MAP_WIDTH - 1) {
            map->terrain[y][x] = road;
            x++;
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
        y = rand() % (MAP_HEIGHT - 3) + 2;

        // get a specified size for the region
        region_width = rand() % 30 + 10;
        region_height = rand() % 6 + 5;

        for (row = 0; row < region_height; row++) {
            if (y + row < MAP_HEIGHT - 1) { // row is in bounds
                for (col = 0; col < region_width; col++) {
                    if (x + col < MAP_WIDTH - 1) { // column is in bounds
                        map->terrain[y + row][x + col] = regions[i];
                    } else { // column out of bounds
                        map->terrain[y + row][x - col] = regions[i];
                    }
                }
            } else { // row out of bounds
                for (col = 0; col < region_width; col++) {
                    if (x + col < MAP_WIDTH - 1) { // column is in bounds
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
    generate_regional_terrain(map, 1, MAP_WIDTH / 2 - 1); // covers cols 1-38
    generate_regional_terrain(map, MAP_WIDTH / 2 - 1, MAP_WIDTH - 1); // covers cols 39-78

    // now add some spontaneous terrain that is impassable
    int i, y, x;

    for (i = 0; i < 20; i++) {
        y = rand() % (MAP_HEIGHT - 2) + 1;
        x = rand() % (MAP_WIDTH - 2) + 1;

        map->terrain[y][x] = edge;

        y = rand() % (MAP_HEIGHT - 2) + 1;
        x = rand() % (MAP_WIDTH - 2) + 1;

        map->terrain[y][x] = willow;
    }
}

/* Fill in the edge of the given map with borders and gates. */
void construct_border(map_t *map, int n, int s, int w, int e)
{
    // First make the entire edge, then place the 4 gates
    int i, j;

    for (i = 0; i < MAP_HEIGHT; i++) {
        map->terrain[i][0] = edge;
        map->terrain[i][MAP_WIDTH - 1] = edge;
    }

    for (j = 0; j < MAP_WIDTH; j++) {
        map->terrain[0][j] = edge;
        map->terrain[MAP_HEIGHT - 1][j] = edge;
    }

    // 0 means generate a gate
    map->n = (n == 0) ? rand() % (MAP_WIDTH - 2) + 1 : n; // Minus 2 and plus 1 to ensure corners aren't gates
    map->s = (s == 0) ? rand() % (MAP_WIDTH - 2) + 1 : s;
    map->w = (w == 0) ? rand() % (MAP_HEIGHT - 2) + 1 : w;
    map->e = (e == 0) ? rand() % (MAP_HEIGHT - 2) + 1 : e;

    // -1 means don't generate a gate
    map->terrain[0][map->n] = (n == -1) ? edge : gate;
    map->terrain[MAP_HEIGHT - 1][map->s] = (s == -1) ? edge : gate;
    map->terrain[map->w][0] = (w == -1) ? edge : gate;
    map->terrain[map->e][MAP_WIDTH - 1] = (e == -1) ? edge : gate;
}

/* Take the given map and populate it with the necessary types of terrain, roads, and buildings. */
void generate_map(map_t *map, int n, int s, int w, int e, int manhattan_distance)
{
    construct_border(map, n, s, w, e);

    generate_terrain(map);

    if (n < 0 || s < 0 || w < 0 || e < 0) { // if we are by an edge, handle roads specially
        pave_roads_edge(map);
    } else {
        pave_roads(map);
    }

    place_buildings(map, manhattan_distance);

    build_bridges(map);
}

void map_init(heap_t *turn_heap, map_t *map, int num_trainers)
{
    int x, y;

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            map->terrain[y][x] = grass;
        }
    }

    map->n = 0;
    map->s = 0;
    map->w = 0;
    map->e = 0;

    generate_map(map, map->n, map->s, map->w, map->e, 0);

    trainer_map_init(turn_heap, map, num_trainers);
}
