#include <stdio.h>
#include <limits.h>

#include "path.h"

int get_terrain_cost(terrain_e terrain_type, trainer_type_e trainer_type) {
    int terrain_cost = INT_MAX;

    if (terrain_type == road) {
        if (trainer_type != swimmer) {
            terrain_cost = 10;
        }
    } else if (terrain_type == bridge) {
        if (trainer_type != swimmer) {
            terrain_cost = 10;
        } else {
            terrain_cost = 7;
        }
    } else if (terrain_type == pokemart || terrain_type == pokecenter) {
        if (trainer_type == pc) {
            terrain_cost = 10;
        } else if (trainer_type != swimmer) {
            terrain_cost = 50;
        }
    } else if (terrain_type == tall_grass) {
        if (trainer_type == hiker) {
            terrain_cost = 15;
        } else if (trainer_type != swimmer) {
            terrain_cost = 20;
        }
    } else if (terrain_type == grass) {
        if (trainer_type != swimmer) {
            terrain_cost = 10;
        }
    } else if (terrain_type == boulder || terrain_type == tree) {
        if (trainer_type == hiker) {
            terrain_cost = 15;
        }
    } else if (terrain_type == water) {
        if (trainer_type == swimmer) {
            terrain_cost = 7;
        }
    } else if (terrain_type == gate) {
        if (trainer_type == pc) {
            terrain_cost = 10;
        }
    }

    return terrain_cost;
}

/*
 * Fills in the Dijkstra's cost map for the trainer.
 * In the future, can return just the path for the trainer to the PC.
 */
void dijkstra_path(heap_t *heap, map_t *map, path_t path[MAP_HEIGHT][MAP_WIDTH], trainer_type_e trainer_type)
{
    if (trainer_type != hiker && trainer_type != rival && trainer_type != swimmer) {
        return;
    }

    path_t *p;

    int x, y;

    // get all coordinates
    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            path[y][x].coordinate.y = y;
            path[y][x].coordinate.x = x;
        }
    }

    for (y = 0; y < MAP_HEIGHT; y++) { // assign every path to infinity
        for (x = 0; x < MAP_WIDTH; x++) {
            path[y][x].cost = INT_MAX;
        }
    }

    int move_cost;
    for (y = 0; y < MAP_HEIGHT; y++) { // add every traversable path to the heap as a node
        for (x = 0; x < MAP_WIDTH; x++) {
            path[y][x].terrain.type = map->terrain[y][x];

            move_cost = get_terrain_cost(path[y][x].terrain.type, trainer_type);

            if (map->trainer_map[y][x] != NULL) {
                if (map->trainer_map[y][x]->type != pc) {
                    move_cost = INT_MAX;
                } else {
                    move_cost = 0;
                }
            }

            if (move_cost != INT_MAX) {
                if (move_cost == 0) {
                    path[y][x].cost = move_cost; // So the pc node is pulled out first to get cost map.
                }
                path[y][x].heap_node = heap_insert(heap, &path[y][x]);
            } else {
                path[y][x].heap_node = NULL;
            }

            path[y][x].terrain.cost = move_cost;
        }
    }

    while ((p = heap_remove_min(heap))) { // visit each node and recalculate the distances of nodes around it
        p->heap_node = NULL;

        if ((path[p->coordinate.y - 1][p->coordinate.x - 1].heap_node) &&
            (path[p->coordinate.y - 1][p->coordinate.x - 1].cost >
             (p->cost + path[p->coordinate.y - 1][p->coordinate.x - 1].terrain.cost) &&
              p->cost + path[p->coordinate.y - 1][p->coordinate.x - 1].terrain.cost > 0)) {
            path[p->coordinate.y - 1][p->coordinate.x - 1].cost =
                    (p->cost + path[p->coordinate.y - 1][p->coordinate.x - 1].terrain.cost);
            heap_decrease_key_no_replace(heap, path[p->coordinate.y - 1][p->coordinate.x - 1].heap_node);
        }
        if ((path[p->coordinate.y - 1][p->coordinate.x    ].heap_node) &&
            (path[p->coordinate.y - 1][p->coordinate.x    ].cost >
             (p->cost + path[p->coordinate.y - 1][p->coordinate.x].terrain.cost) &&
              p->cost + path[p->coordinate.y - 1][p->coordinate.x].terrain.cost > 0)) {
            path[p->coordinate.y - 1][p->coordinate.x    ].cost =
                    (p->cost + path[p->coordinate.y - 1][p->coordinate.x    ].terrain.cost);
            heap_decrease_key_no_replace(heap, path[p->coordinate.y - 1][p->coordinate.x    ].heap_node);
        }
        if ((path[p->coordinate.y - 1][p->coordinate.x + 1].heap_node) &&
            (path[p->coordinate.y - 1][p->coordinate.x + 1].cost >
             (p->cost + path[p->coordinate.y - 1][p->coordinate.x + 1].terrain.cost) &&
              p->cost + path[p->coordinate.y - 1][p->coordinate.x + 1].terrain.cost > 0)) {
            path[p->coordinate.y - 1][p->coordinate.x + 1].cost =
                    (p->cost + path[p->coordinate.y - 1][p->coordinate.x + 1].terrain.cost);
            heap_decrease_key_no_replace(heap, path[p->coordinate.y - 1][p->coordinate.x + 1].heap_node);
        }
        if ((path[p->coordinate.y    ][p->coordinate.x - 1].heap_node) &&
            (path[p->coordinate.y    ][p->coordinate.x - 1].cost >
             (p->cost + path[p->coordinate.y    ][p->coordinate.x - 1].terrain.cost) &&
              p->cost + path[p->coordinate.y    ][p->coordinate.x - 1].terrain.cost > 0)) {
            path[p->coordinate.y    ][p->coordinate.x - 1].cost =
                    (p->cost + path[p->coordinate.y    ][p->coordinate.x - 1].terrain.cost);
            heap_decrease_key_no_replace(heap, path[p->coordinate.y    ][p->coordinate.x - 1].heap_node);
        }
        if ((path[p->coordinate.y    ][p->coordinate.x + 1].heap_node) &&
            (path[p->coordinate.y    ][p->coordinate.x + 1].cost >
             (p->cost + path[p->coordinate.y    ][p->coordinate.x + 1].terrain.cost) &&
              p->cost + path[p->coordinate.y    ][p->coordinate.x + 1].terrain.cost > 0)) {
            path[p->coordinate.y    ][p->coordinate.x + 1].cost =
                    (p->cost + path[p->coordinate.y    ][p->coordinate.x + 1].terrain.cost);
            heap_decrease_key_no_replace(heap, path[p->coordinate.y    ][p->coordinate.x + 1].heap_node);
        }
        if ((path[p->coordinate.y + 1][p->coordinate.x - 1].heap_node) &&
            (path[p->coordinate.y + 1][p->coordinate.x - 1].cost >
             (p->cost + path[p->coordinate.y + 1][p->coordinate.x - 1].terrain.cost) &&
              p->cost + path[p->coordinate.y + 1][p->coordinate.x - 1].terrain.cost > 0)) {
            path[p->coordinate.y + 1][p->coordinate.x - 1].cost =
                    (p->cost + path[p->coordinate.y + 1][p->coordinate.x - 1].terrain.cost);
            heap_decrease_key_no_replace(heap, path[p->coordinate.y + 1][p->coordinate.x - 1].heap_node);
        }
        if ((path[p->coordinate.y + 1][p->coordinate.x    ].heap_node) &&
            (path[p->coordinate.y + 1][p->coordinate.x    ].cost >
             (p->cost + path[p->coordinate.y + 1][p->coordinate.x    ].terrain.cost) &&
              p->cost + path[p->coordinate.y + 1][p->coordinate.x    ].terrain.cost > 0)) {
            path[p->coordinate.y + 1][p->coordinate.x    ].cost =
                    (p->cost + path[p->coordinate.y + 1][p->coordinate.x    ].terrain.cost);
            heap_decrease_key_no_replace(heap, path[p->coordinate.y + 1][p->coordinate.x    ].heap_node);
        }
        if ((path[p->coordinate.y + 1][p->coordinate.x + 1].heap_node) &&
            (path[p->coordinate.y + 1][p->coordinate.x + 1].cost >
             (p->cost + path[p->coordinate.y + 1][p->coordinate.x + 1].terrain.cost) &&
              p->cost + path[p->coordinate.y + 1][p->coordinate.x + 1].terrain.cost > 0)) {
            path[p->coordinate.y + 1][p->coordinate.x + 1].cost =
                    (p->cost + path[p->coordinate.y + 1][p->coordinate.x + 1].terrain.cost);
            heap_decrease_key_no_replace(heap, path[p->coordinate.y + 1][p->coordinate.x + 1].heap_node);
        }
    }
}