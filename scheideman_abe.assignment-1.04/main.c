#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "world.h"

static int32_t path_cmp(const void *key, const void *with) { // make heap compare nodes based on cost
    return ((path_t *) key)->cost - ((path_t *) with)->cost;
}

static int32_t turn_cmp(const void *key, const void *with) { // make heap compare turns
    int32_t next_turn = ((trainer_t *) key)->next_turn - ((trainer_t *) with)->next_turn;
    return (next_turn == 0) ? ((trainer_t *) key)->seq_num - ((trainer_t *) with)->seq_num : next_turn;
}

void print_view(map_t *map, char view[MAP_HEIGHT][MAP_WIDTH + 1])
{
    int y, x;

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            switch (map->terrain[y][x])
            {
                case grass:
                    view[y][x] = '.';
                    break;
                case tall_grass:
                    view[y][x] = ':';
                    break;
                case boulder:
                    view[y][x] = '%';
                    break;
                case edge:
                    view[y][x] = '%';
                    break;
                case tree:
                    view[y][x] = '^';
                    break;
                case willow:
                    view[y][x] = '^';
                    break;
                case water:
                    view[y][x] = '~';
                    break;
                case road:
                    view[y][x] = '#';
                    break;
                case gate:
                    view[y][x] = '#';
                    break;
                case bridge:
                    view[y][x] = '#';
                    break;
                case pokemart:
                    view[y][x] = 'M';
                    break;
                case pokecenter:
                    view[y][x] = 'C';
                    break;
                default:
                    view[y][x] = '!'; // Something went wrong
                    break;
            }

            if  (map->trainer_map[y][x] != NULL) {
                switch (map->trainer_map[y][x]->type) {
                    case pc:
                        view[y][x] = '@';
                        break;
                    case hiker:
                        view[y][x] = 'h';
                        break;
                    case rival:
                        view[y][x] = 'r';
                        break;
                    case pacer:
                        view[y][x] = 'p';
                        break;
                    case wanderer:
                        view[y][x] = 'w';
                        break;
                    case sentry:
                        view[y][x] = 's';
                        break;
                    case explorer:
                        view[y][x] = 'e';
                        break;
                    case swimmer:
                        view[y][x] = 'm';
                        break;
                    default:
                        view[y][x] = '?'; // Something went wrong
                        break;
                }
            }
        }



        if (y < MAP_HEIGHT - 1) {
            view[y][MAP_WIDTH] = '\n';
        }
    }

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH + 1; x++) {
            printf("%c", view[y][x]);
        }
    }

    printf("\n");
}

int check_random_turn(int random_direction, map_t *map, coordinate_t pos, trainer_type_e type) {
    int check, terrain_cost;

    check = 0;

    if (random_direction == 0) { // check up
        if (type == wanderer || type == swimmer) {
            if (map->terrain[pos.y][pos.x] == map->terrain[pos.y - 1][pos.x]) {
                check = 1;
            }
        } else if (type == explorer) {
            terrain_cost = get_terrain_cost(map->terrain[pos.y - 1][pos.x], type);
            if (terrain_cost != INT_MAX && map->trainer_map[pos.y - 1][pos.x] == NULL) {
                check = 1;
            }
        }
    } else if (random_direction == 1) { // check right
        if (type == wanderer || type == swimmer) {
            if (map->terrain[pos.y][pos.x] == map->terrain[pos.y][pos.x + 1]) {
                check = 1;
            }
        } else if (type == explorer) {
            terrain_cost = get_terrain_cost(map->terrain[pos.y][pos.x + 1], type);
            if (terrain_cost != INT_MAX && map->trainer_map[pos.y][pos.x + 1] == NULL) {
                check = 1;
            }
        }
    } else if (random_direction == 2) { // check down
        if (type == wanderer || type == swimmer) {
            if (map->terrain[pos.y][pos.x] == map->terrain[pos.y + 1][pos.x]) {
                check = 1;
            }
        } else if (type == explorer) {
            terrain_cost = get_terrain_cost(map->terrain[pos.y + 1][pos.x], type);
            if (terrain_cost != INT_MAX && map->trainer_map[pos.y + 1][pos.x] == NULL) {
                check = 1;
            }
        }
    }  else if (random_direction == 3) { // check left
        if (type == wanderer || type == swimmer) {
            if (map->terrain[pos.y][pos.x] == map->terrain[pos.y][pos.x - 1]) {
                check = 1;
            }
        } else if (type == explorer) {
            terrain_cost = get_terrain_cost(map->terrain[pos.y][pos.x - 1], type);
            if (terrain_cost != INT_MAX && map->trainer_map[pos.y][pos.x - 1] == NULL) {
                check = 1;
            }
        }
    }

    return check;
}

void random_turn(map_t *map, trainer_t *t) {
    int random_direction, valid_direction;
    random_direction = rand() % 4;
    valid_direction = check_random_turn(random_direction, map, t->pos, t->type);
    while (!valid_direction) {
        random_direction = rand() % 4;
        valid_direction = check_random_turn(random_direction, map, t->pos, t->type);
    }

    if (random_direction == 0) { // GO UP
        t->dir.y = -1;
        t->dir.x = 0;
        map->trainer_map[t->pos.y][t->pos.x] = NULL;
        t->pos.y--;
        map->trainer_map[t->pos.y][t->pos.x] = t;
    } else if (random_direction == 1) { // go right
        t->dir.y = 0;
        t->dir.x = 1;
        map->trainer_map[t->pos.y][t->pos.x] = NULL;
        t->pos.x++;
        map->trainer_map[t->pos.y][t->pos.x] = t;
    } else if (random_direction == 2) { // go down
        t->dir.y = 1;
        t->dir.x = 0;
        map->trainer_map[t->pos.y][t->pos.x] = NULL;
        t->pos.y++;
        map->trainer_map[t->pos.y][t->pos.x] = t;
    } else if (random_direction == 3) { // go left
        t->dir.y = 0;
        t->dir.x = -1;
        map->trainer_map[t->pos.y][t->pos.x] = NULL;
        t->pos.x--;
        map->trainer_map[t->pos.y][t->pos.x] = t;
    }
}

void game_loop(heap_t *turn_heap, heap_t *path_heap, world_t *world)
{
    trainer_t *t;
    path_t *cheapest_path;
    terrain_e current_terrain;
    int move_cost;
    int x, y;
    coordinate_t pc_pos;
    path_t swimmer_path[MAP_HEIGHT][MAP_WIDTH];

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            if (world->current_map->trainer_map[y][x] != NULL) {
                if (world->current_map->trainer_map[y][x]->type == pc) {
                    pc_pos.y = y;
                    pc_pos.x = x;
                }
            }

            swimmer_path[y][x].heap_node = NULL;
            swimmer_path[y][x].coordinate.x = x;
            swimmer_path[y][x].coordinate.y = y;
            swimmer_path[y][x].terrain.type = edge;
            swimmer_path[y][x].terrain.cost = INT_MAX;
            swimmer_path[y][x].cost = INT_MAX;
        }
    }

    while ((t = heap_remove_min(turn_heap))) {
        if (t->type == pc) { // for now, just stay in place
            t->next_turn = t->next_turn + 15;
            pc_pos = t->pos;
        } else if (t->type == hiker) {
            cheapest_path = &world->hiker_path[t->pos.y - 1][t->pos.x - 1];

            dijkstra_path(path_heap, world->current_map, world->hiker_path, hiker);

            if (world->hiker_path[t->pos.y - 1][t->pos.x].cost < cheapest_path->cost) {
                cheapest_path = &world->hiker_path[t->pos.y - 1][t->pos.x];
            }
            if (world->hiker_path[t->pos.y - 1][t->pos.x + 1].cost < cheapest_path->cost) {
                cheapest_path = &world->hiker_path[t->pos.y - 1][t->pos.x + 1];
            }
            if (world->hiker_path[t->pos.y][t->pos.x - 1].cost < cheapest_path->cost) {
                cheapest_path = &world->hiker_path[t->pos.y][t->pos.x - 1];
            }
            if (world->hiker_path[t->pos.y][t->pos.x + 1].cost < cheapest_path->cost) {
                cheapest_path = &world->hiker_path[t->pos.y][t->pos.x + 1];
            }
            if (world->hiker_path[t->pos.y + 1][t->pos.x - 1].cost < cheapest_path->cost) {
                cheapest_path = &world->hiker_path[t->pos.y + 1][t->pos.x - 1];
            }
            if (world->hiker_path[t->pos.y + 1][t->pos.x].cost < cheapest_path->cost) {
                cheapest_path = &world->hiker_path[t->pos.y + 1][t->pos.x];
            }
            if (world->hiker_path[t->pos.y + 1][t->pos.x + 1].cost < cheapest_path->cost) {
                cheapest_path = &world->hiker_path[t->pos.y + 1][t->pos.x + 1];
            }
            if (cheapest_path->cost == 0) {
                cheapest_path = &world->rival_path[t->pos.y    ][t->pos.x    ];
            }

            world->current_map->trainer_map[t->pos.y][t->pos.x] = NULL;
            t->pos.y = cheapest_path->coordinate.y;
            t->pos.x = cheapest_path->coordinate.x;
            world->current_map->trainer_map[t->pos.y][t->pos.x] = t;
            t->next_turn = t->next_turn + cheapest_path->terrain.cost;
        } else if (t->type == rival) {
            cheapest_path = &world->rival_path[t->pos.y - 1][t->pos.x - 1];

            dijkstra_path(path_heap, world->current_map, world->rival_path, rival);

            if (world->rival_path[t->pos.y - 1][t->pos.x].cost < cheapest_path->cost) {
                cheapest_path = &world->rival_path[t->pos.y - 1][t->pos.x];
            }
            if (world->rival_path[t->pos.y - 1][t->pos.x + 1].cost < cheapest_path->cost) {
                cheapest_path = &world->rival_path[t->pos.y - 1][t->pos.x + 1];
            }
            if (world->rival_path[t->pos.y][t->pos.x - 1].cost < cheapest_path->cost) {
                cheapest_path = &world->rival_path[t->pos.y][t->pos.x - 1];
            }
            if (world->rival_path[t->pos.y][t->pos.x + 1].cost < cheapest_path->cost) {
                cheapest_path = &world->rival_path[t->pos.y][t->pos.x + 1];
            }
            if (world->rival_path[t->pos.y + 1][t->pos.x - 1].cost < cheapest_path->cost) {
                cheapest_path = &world->rival_path[t->pos.y + 1][t->pos.x - 1];
            }
            if (world->rival_path[t->pos.y + 1][t->pos.x].cost < cheapest_path->cost) {
                cheapest_path = &world->rival_path[t->pos.y + 1][t->pos.x];
            }
            if (world->rival_path[t->pos.y + 1][t->pos.x + 1].cost < cheapest_path->cost) {
                cheapest_path = &world->rival_path[t->pos.y + 1][t->pos.x + 1];
            }
            if (cheapest_path->cost == 0) {
                cheapest_path = &world->rival_path[t->pos.y    ][t->pos.x    ];
            }

            world->current_map->trainer_map[t->pos.y][t->pos.x] = NULL;
            t->pos.y = cheapest_path->coordinate.y;
            t->pos.x = cheapest_path->coordinate.x;
            world->current_map->trainer_map[t->pos.y][t->pos.x] = t;
            t->next_turn = t->next_turn + cheapest_path->terrain.cost;
        } else if (t->type == sentry) {
            t->next_turn = t->next_turn + 15;
        } else if (t->type == pacer) { // add a vector, check terrain map, adjust as needed
            current_terrain = world->current_map->terrain[t->pos.y][t->pos.x];

            // Just starting or moving right and the right is open
            if (t->dir.x >= 0 && world->current_map->terrain[t->pos.y][t->pos.x + 1] == current_terrain) {
                t->dir.x = 1;
                world->current_map->trainer_map[t->pos.y][t->pos.x] = NULL;
                t->pos.x++;
                world->current_map->trainer_map[t->pos.y][t->pos.x] = t;
                // Just starting or moving left and checking if left is open
            } else if (t->dir.x <= 0 && world->current_map->terrain[t->pos.y][t->pos.x - 1] == current_terrain) {
                t->dir.x = -1;
                world->current_map->trainer_map[t->pos.y][t->pos.x] = NULL;
                t->pos.x--;
                world->current_map->trainer_map[t->pos.y][t->pos.x] = t;
                // moving left but need to go back right
            } else if (t->dir.x == -1 && world->current_map->terrain[t->pos.y][t->pos.x + 1] == current_terrain) {
                t->dir.x = 1;
                world->current_map->trainer_map[t->pos.y][t->pos.x] = NULL;
                t->pos.x++;
                world->current_map->trainer_map[t->pos.y][t->pos.x] = t;
                // moving right but need to go back left
            } else if (t->dir.x == 1 && world->current_map->terrain[t->pos.y][t->pos.x - 1] == current_terrain) { // go back left
                t->dir.x = -1;
                world->current_map->trainer_map[t->pos.y][t->pos.x] = NULL;
                t->pos.x--;
                world->current_map->trainer_map[t->pos.y][t->pos.x] = t;
            }

            t->next_turn = t->next_turn + get_terrain_cost(world->current_map->terrain[t->pos.y][t->pos.x], t->type);
        } else if (t->type == wanderer || t->type == explorer) {
            current_terrain = world->current_map->terrain[t->pos.y][t->pos.x];

            // just starting or already going up and need to continue
            if (t->dir.x == 0 && t->dir.y <= 0) {
                move_cost = get_terrain_cost(world->current_map->terrain[t->pos.y - 1][t->pos.x], explorer);

                if ((t->type == wanderer && (current_terrain == world->current_map->terrain[t->pos.y - 1][t->pos.x])) ||
                    (t->type == explorer && (move_cost != INT_MAX))) {
                    t->dir.y = -1;
                    world->current_map->trainer_map[t->pos.y][t->pos.x] = NULL;
                    t->pos.y--;
                    world->current_map->trainer_map[t->pos.y][t->pos.x] = t;
                } else {
                    random_turn(world->current_map, t);
                }
                // just starting or already going right and need to continue
            } else if (t->dir.x >= 0 && t->dir.y == 0) {
                move_cost = get_terrain_cost(world->current_map->terrain[t->pos.y][t->pos.x + 1], explorer);

                if ((t->type == wanderer && (current_terrain == world->current_map->terrain[t->pos.y][t->pos.x + 1])) ||
                    (t->type == explorer && (move_cost != INT_MAX))) {
                    t->dir.x = 1;
                    world->current_map->trainer_map[t->pos.y][t->pos.x] = NULL;
                    t->pos.x--;
                    world->current_map->trainer_map[t->pos.y][t->pos.x] = t;
                } else {
                    random_turn(world->current_map, t);
                }
                // just starting or already going down and need to continue
            } else if (t->dir.x == 0 && t->dir.y >= 0) {
                move_cost = get_terrain_cost(world->current_map->terrain[t->pos.y + 1][t->pos.x], explorer);

                if ((t->type == wanderer && (current_terrain == world->current_map->terrain[t->pos.y + 1][t->pos.x])) ||
                    (t->type == explorer && (move_cost != INT_MAX))) {
                    t->dir.y = 1;
                    world->current_map->trainer_map[t->pos.y][t->pos.x] = NULL;
                    t->pos.y++;
                    world->current_map->trainer_map[t->pos.y][t->pos.x] = t;
                } else {
                    random_turn(world->current_map, t);
                }
                // just starting or already going left and need to continue
            } else if (t->dir.x <= 0 && t->dir.y == 0) {
                move_cost = get_terrain_cost(world->current_map->terrain[t->pos.y    ][t->pos.x - 1], explorer);

                if ((t->type == wanderer && (current_terrain == world->current_map->terrain[t->pos.y    ][t->pos.x - 1])) ||
                    (t->type == explorer && (move_cost != INT_MAX))) {
                    t->dir.x = -1;
                    world->current_map->trainer_map[t->pos.y][t->pos.x] = NULL;
                    t->pos.x--;
                    world->current_map->trainer_map[t->pos.y][t->pos.x] = t;
                } else {
                    random_turn(world->current_map, t);
                }
            }

            t->next_turn = t->next_turn + get_terrain_cost(world->current_map->terrain[t->pos.y][t->pos.x], t->type);
        } else if (t->type == swimmer) {
            // path to pc
            if ((world->current_map->terrain[pc_pos.y - 1][pc_pos.x    ] == water ||
                 world->current_map->terrain[pc_pos.y - 1][pc_pos.x    ] == bridge) ||
                (world->current_map->terrain[pc_pos.y    ][pc_pos.x + 1] == water ||
                 world->current_map->terrain[pc_pos.y    ][pc_pos.x + 1] == bridge) ||
                (world->current_map->terrain[pc_pos.y + 1][pc_pos.x    ] == water ||
                 world->current_map->terrain[pc_pos.y + 1][pc_pos.x    ] == bridge) ||
                (world->current_map->terrain[pc_pos.y    ][pc_pos.x - 1] == water ||
                 world->current_map->terrain[pc_pos.y    ][pc_pos.x - 1] == bridge)) {
                dijkstra_path(path_heap, world->current_map, swimmer_path, swimmer);
                cheapest_path = &world->hiker_path[t->pos.y - 1][t->pos.x - 1];

                if (swimmer_path[t->pos.y - 1][t->pos.x].cost < cheapest_path->cost) {
                    cheapest_path = &swimmer_path[t->pos.y - 1][t->pos.x];
                }
                if (swimmer_path[t->pos.y - 1][t->pos.x + 1].cost < cheapest_path->cost) {
                    cheapest_path = &swimmer_path[t->pos.y - 1][t->pos.x + 1];
                }
                if (swimmer_path[t->pos.y][t->pos.x - 1].cost < cheapest_path->cost) {
                    cheapest_path = &swimmer_path[t->pos.y][t->pos.x - 1];
                }
                if (swimmer_path[t->pos.y][t->pos.x + 1].cost < cheapest_path->cost) {
                    cheapest_path = &swimmer_path[t->pos.y][t->pos.x + 1];
                }
                if (swimmer_path[t->pos.y + 1][t->pos.x - 1].cost < cheapest_path->cost) {
                    cheapest_path = &swimmer_path[t->pos.y + 1][t->pos.x - 1];
                }
                if (swimmer_path[t->pos.y + 1][t->pos.x].cost < cheapest_path->cost) {
                    cheapest_path = &swimmer_path[t->pos.y + 1][t->pos.x];
                }
                if (swimmer_path[t->pos.y + 1][t->pos.x + 1].cost < cheapest_path->cost) {
                    cheapest_path = &swimmer_path[t->pos.y + 1][t->pos.x + 1];
                }
                if (cheapest_path->cost == 0) {
                    cheapest_path = &world->rival_path[t->pos.y    ][t->pos.x    ];
                }

                world->current_map->trainer_map[t->pos.y][t->pos.x] = NULL;
                t->pos.y = cheapest_path->coordinate.y;
                t->pos.x = cheapest_path->coordinate.x;
                world->current_map->trainer_map[t->pos.y][t->pos.x] = t;
                t->next_turn = t->next_turn + cheapest_path->terrain.cost;
            // move like wanderer
            } else {
                // just starting or already going up and need to continue
                if (t->dir.x == 0 && t->dir.y <= 0) {
                    if (world->current_map->terrain[t->pos.y - 1][t->pos.x] == water ||
                        world->current_map->terrain[t->pos.y - 1][t->pos.x] == bridge) {
                        t->dir.y = -1;
                        world->current_map->trainer_map[t->pos.y][t->pos.x] = NULL;
                        t->pos.y--;
                        world->current_map->trainer_map[t->pos.y][t->pos.x] = t;
                    } else {
                        random_turn(world->current_map, t);
                    }
                // just starting or already going right and need to continue
                } else if (t->dir.x >= 0 && t->dir.y == 0) {
                    if (world->current_map->terrain[t->pos.y][t->pos.x + 1] == water ||
                        world->current_map->terrain[t->pos.y][t->pos.x + 1] == bridge) {
                        t->dir.x = 1;
                        world->current_map->trainer_map[t->pos.y][t->pos.x] = NULL;
                        t->pos.x++;
                        world->current_map->trainer_map[t->pos.y][t->pos.x] = t;
                    } else {
                        random_turn(world->current_map, t);
                    }
                    // just starting or already going down and need to continue
                } else if (t->dir.x == 0 && t->dir.y >= 0) {
                    if (world->current_map->terrain[t->pos.y + 1][t->pos.x] == water ||
                        world->current_map->terrain[t->pos.y + 1][t->pos.x] == bridge) {
                        t->dir.y = 1;
                        world->current_map->trainer_map[t->pos.y][t->pos.x] = NULL;
                        t->pos.y++;
                        world->current_map->trainer_map[t->pos.y][t->pos.x] = t;
                        // just starting or already going left and need to continue
                    } else {
                        random_turn(world->current_map, t);
                    }
                } else if (t->dir.x <= 0 && t->dir.y == 0) {
                    if (world->current_map->terrain[t->pos.y][t->pos.x - 1] == water ||
                        world->current_map->terrain[t->pos.y][t->pos.x - 1] == bridge) {
                        t->dir.x = -1;
                        world->current_map->trainer_map[t->pos.y][t->pos.x] = NULL;
                        t->pos.x--;
                        world->current_map->trainer_map[t->pos.y][t->pos.x] = t;
                    } else {
                        random_turn(world->current_map, t);
                    }
                }

                t->next_turn = t->next_turn + get_terrain_cost(world->current_map->terrain[t->pos.y][t->pos.x], t->type);
            }
        }

        heap_insert(turn_heap, t);
        print_view(world->current_map, world->view);
        printf("(%d, %d)\n", world->location.x - START_X, START_Y - world->location.y);
        usleep(250000);
    }
}

void world_init(heap_t *turn_heap, world_t *world, int num_trainers)
{
    int x, y;
    for (y = 0; y < WORLD_HEIGHT; y++) {
        for (x = 0; x < WORLD_WIDTH; x++) {
            world->board[y][x] = NULL;
        }
    }

    world->current_map = malloc(sizeof (*world->current_map));

    world->location.x = START_X;
    world->location.y = START_Y;

    world->board[START_Y][START_X] = malloc(sizeof (*world->current_map));
    world->current_map = world->board[START_Y][START_X];
    map_init(turn_heap, world->current_map, num_trainers);

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            world->hiker_path[y][x].heap_node = NULL;
            world->hiker_path[y][x].coordinate.x = x;
            world->hiker_path[y][x].coordinate.y = y;
            world->hiker_path[y][x].terrain.type = edge;
            world->hiker_path[y][x].terrain.cost = INT_MAX;
            world->hiker_path[y][x].cost = INT_MAX;

            world->rival_path[y][x].heap_node = NULL;
            world->rival_path[y][x].coordinate.x = x;
            world->rival_path[y][x].coordinate.y = y;
            world->rival_path[y][x].terrain.type = edge;
            world->rival_path[y][x].terrain.cost = INT_MAX;
            world->rival_path[y][x].cost = INT_MAX;
        }
    }

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            world->view[y][x] = '!';
        }

        world->view[y][MAP_WIDTH] = '\n';
    }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    heap_t path_heap, turn_heap;
    heap_init(&path_heap, path_cmp, NULL);
    heap_init(&turn_heap, turn_cmp, NULL);

    world_t *world;
    world = malloc(sizeof (*world));
    if (argc == 3) {
        if (!strcmp(argv[1], "--numtrainers")) {
            world_init(&turn_heap, world, atoi(argv[2]));
        } else {
            fprintf(stderr, "Usage: \"./play --numtrainers <integer>\" or \"./play\" \n");

            return -1;
        }
    } else if (argc == 1) {
        world_init(&turn_heap, world, 10);
    } else {
        fprintf(stderr, "Usage: \"./play --numtrainers <integer>\" or \"./play\" \n");

        return -1;
    }

    print_view(world->current_map, world->view);
    printf("(%d, %d)\n", world->location.x - START_X, START_Y - world->location.y);

    dijkstra_path(&path_heap, world->current_map, world->hiker_path, hiker);
    dijkstra_path(&path_heap, world->current_map, world->rival_path, rival);

    game_loop(&turn_heap, &path_heap, world);

    return 0;
}
