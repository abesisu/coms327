#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "world.h"

static int32_t path_cmp(const void *key, const void *with) { // make heap compare nodes based on cost
    return ((path_t *) key)->cost - ((path_t *) with)->cost;
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
                case border:
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
                case pokemart:
                    view[y][x] = 'M';
                    break;
                case pokecenter:
                    view[y][x] = 'C';
                    break;
                case pc:
                    view[y][x] = '@';
                    break;
                case hiker:
                    view[y][x] = 'h';
                    break;
                case rival:
                    view[y][x] = 'r';
                    break;
                default:
                    view[y][x] = '!'; // Something went wrong
                    break;
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

/* According to 1.03, print out the Dijkstra's path map. */
void print_path_map(path_t path[MAP_HEIGHT][MAP_WIDTH])
{
    int y, x;
    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH - 1; x++) {
            if (path[y][x].cost == INT_MAX) {
                printf("   ");
            } else {
                printf("%02d ", path[y][x].cost % 100);
            }
        }
        if (path[y][MAP_WIDTH - 1].cost == INT_MAX) {
            printf("  \n");
        } else {
            printf("%02d\n", path[y][MAP_WIDTH - 1].cost % 100);
        }
    }
    printf("\n");
}

/* Check if trainer can go to location based on terrain and if it is occupied or not. Return 0 if trainer can be at location. */
int check_trainer_position(trainer_type_e type, terrain_e terrain)
{
    int check = 1;

    if (terrain == border ||
        terrain == willow ||
        terrain == pc ||
        terrain == hiker ||
        terrain == rival) {
        check = 0;
    } else if ((type == swimmer_train && terrain != water) || (terrain == water && type != swimmer_train)) {
        check = 0;
    } else if (terrain == boulder || terrain == tree) {
        if (type != hiker_train) {
            check = 0;
        }
    } else if (terrain == gate && type != pc_train) {
        check = 0;
    }

    return check;
}

/* Add the PC to a road in this map. */
void place_pc(map_t *map, trainer_t *pc_trainer)
{
    int y, x, count;
    count = 0;

    // loop through the world once counting all the roads
    for (y = 1; y < MAP_HEIGHT; y++) {
        for (x = 1; x < MAP_WIDTH; x++) {
            if (map->terrain[y][x] == road) {
                count++;
            }
        }
    }

    // create an array of the size of the number of roads
    coordinate_t pc_start_options[count];

    // Add the coordinates of each road to the array
    count = 0;
    for (y = 1; y < MAP_HEIGHT; y++) {
        for (x = 1; x < MAP_WIDTH; x++) {
            if (map->terrain[y][x] == road) {
                pc_start_options[count].x = x;
                pc_start_options[count].y = y;
                count++;
            }
        }
    }

    // randomly pick one of those coordinates for the pc_train
    pc_trainer->position = pc_start_options[rand() % count];
    map->terrain[pc_trainer->position.y][pc_trainer->position.x] = pc;
}

void place_trainer(trainer_t trainer, terrain_e terrain[MAP_HEIGHT][MAP_WIDTH]) {
    int valid_character_position;
    trainer.position.x = (rand() % MAP_WIDTH - 2) + 1;
    trainer.position.y = (rand() % MAP_HEIGHT - 2) + 1;

    valid_character_position = check_trainer_position(trainer.type,
                                                      terrain[trainer.position.y][trainer.position.x]
    );
    while(!valid_character_position) {
        trainer.position.x = (rand() % MAP_WIDTH - 2) + 1;
        trainer.position.y = (rand() % MAP_HEIGHT - 2) + 1;
        valid_character_position = check_trainer_position(trainer.type,
                                                          terrain[trainer.position.y][trainer.position.x]
        );
    }

    // Doesn't cover all cases currently. Only hiker and rival.
    switch (trainer.type) {
        case hiker_train:
            terrain[trainer.position.x][trainer.position.y] = hiker;
            break;
        case rival_train:
            terrain[trainer.position.x][trainer.position.y] = rival;
            break;
        default:
            break;
    }
}

void place_gates(world_t *world)
{
    if (world->location.y == 0) { // top of map
        world->current_map->n = -1;
    } else if (world->location.y == WORLD_HEIGHT - 1) { // bottom of map
        world->current_map->s = -1;
    }

    if (world->location.x == 0) { // west edge of map
        world->current_map->w = -1;
    } else if (world->location.x == WORLD_WIDTH - 1) { // east edge of map
        world->current_map->e = -1;
    }

    // Check for north gate
    if (world->location.y > 0 && world->board[world->location.y - 1][world->location.x] != NULL) {
        world->current_map->n = world->board[world->location.y - 1][world->location.x]->s;
    }

    // Check for south gate
    if (world->location.y < WORLD_HEIGHT - 1 && world->board[world->location.y + 1][world->location.x] != NULL) {
        world->current_map->s = world->board[world->location.y + 1][world->location.x]->n;
    }

    // Check for west gate
    if (world->location.x > 0 && world->board[world->location.y][world->location.x - 1] != NULL) {
        world->current_map->w = world->board[world->location.y][world->location.x - 1]->e;
    }

    // Check for east gate
    if (world->location.x < WORLD_WIDTH - 1 && world->board[world->location.y][world->location.x + 1] != NULL) {
        world->current_map->e = world->board[world->location.y][world->location.x + 1]->w;
    }
}

void trainer_init(map_t *map, trainer_t *pc_trainer, trainer_t *hiker_trainer, trainer_t *rival_trainer)
{
    pc_trainer->type = pc_train;
    pc_trainer->position.x = -1;
    pc_trainer->position.y = -1;
    place_pc(map, pc_trainer);

    hiker_trainer->type = hiker_train;
    hiker_trainer->position.x = -1;
    hiker_trainer->position.y = -1;

    rival_trainer->type = rival_train;
    rival_trainer->position.x = -1;
    rival_trainer->position.y = -1;
}

void world_init(world_t *world)
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
    map_init(world->current_map);

    generate_map(world->current_map, world->current_map->n, world->current_map->s, world->current_map->w, world->current_map->e, 0);

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            world->hiker_path[y][x].heap_node = NULL;
            world->hiker_path[y][x].coordinate.x = x;
            world->hiker_path[y][x].coordinate.y = y;
            world->hiker_path[y][x].terrain.type = border;
            world->hiker_path[y][x].terrain.cost = INT_MAX;
            world->hiker_path[y][x].cost = INT_MAX;

            world->rival_path[y][x].heap_node = NULL;
            world->rival_path[y][x].coordinate.x = x;
            world->rival_path[y][x].coordinate.y = y;
            world->rival_path[y][x].terrain.type = border;
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

void world_delete(world_t *world)
{
    int x, y;
    // loop through world and free everything
    for (y = 0; y < WORLD_HEIGHT; y++) {
        for (x = 0; x < WORLD_WIDTH; x++) {
            if (world->board[y][x] != NULL) {
                free(world->board[y][x]);
            }
        }
    }
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char *argv[])
{
    srand(time(NULL));

    world_t *world;
    world = malloc(sizeof (*world));
    world_init(world);

    trainer_t pc_trainer, hiker_trainer, rival_trainer;
    trainer_init(world->current_map, &pc_trainer, &hiker_trainer, &rival_trainer);

    heap_t heap;
    heap_init(&heap, path_cmp, NULL);

    print_view(world->current_map, world->view);

    dijkstra_path(&heap, world->current_map, world->hiker_path, hiker_trainer.type, pc_trainer.position);
    dijkstra_path(&heap, world->current_map, world->rival_path, rival_trainer.type, pc_trainer.position);

    print_path_map(world->hiker_path);
    print_path_map(world->rival_path);
    printf("(%d, %d)\n", world->location.x - START_X, START_Y - world->location.y);

    char input = '\0';
    int fly_x, fly_y, manhattan_distance;

    while (input != 'q') {
        scanf(" %c", &input);

        if (input == 'n' && world->location.y > 0) {
            world->location.y--;
        } else if (input == 's' && world->location.y < WORLD_HEIGHT - 1) {
            world->location.y++;
        } else if (input == 'w' && world->location.x > 0) {
            world->location.x--;
        } else if (input == 'e' && world->location.x < WORLD_WIDTH - 1) {
            world->location.x++;
        } else if (input == 'f') {
            scanf(" %d %d", &fly_x, &fly_y);

            if (fly_x >= -1 * WORLD_WIDTH / 2 && fly_x <= WORLD_WIDTH / 2) {
                world->location.x = START_X + fly_x;
            } else {
                printf("Please enter a valid x-coordinate: [-200][200]\n");
                continue;
            }

            if (fly_y >= -1 * WORLD_HEIGHT / 2 && fly_y <= WORLD_HEIGHT / 2) {
                world->location.y = START_Y - fly_y;
            } else {
                printf("Please enter a valid y-coordinate: [-200][200]\n");
                continue;
            }
        } else if (input == 'q') {
            heap_delete(&heap);

            world_delete(world);
            free(world);

            break;
        } else {
            printf("Please enter a valid command: n, s, w, e, f x y, or q\n");
            continue;
        }

        if (world->board[world->location.y][world->location.x] == NULL) {
            world->board[world->location.y][world->location.x] = malloc(sizeof (*world->current_map));
            world->current_map = world->board[world->location.y][world->location.x];
            map_init(world->current_map);

            place_gates(world);

            manhattan_distance = abs(world->location.x - START_X) + abs(world->location.y - START_Y);

            generate_map(world->current_map, world->current_map->n, world->current_map->s, world->current_map->w, world->current_map->e, manhattan_distance);
        }

        world->current_map = world->board[world->location.y][world->location.x];

        place_pc(world->current_map, &pc_trainer);

        print_view(world->current_map, world->view);

        dijkstra_path(&heap, world->current_map, world->hiker_path, hiker_trainer.type, pc_trainer.position);
        dijkstra_path(&heap, world->current_map, world->rival_path, rival_trainer.type, pc_trainer.position);

        print_path_map(world->hiker_path);
        print_path_map(world->rival_path);
        printf("(%d, %d)\n", world->location.x - START_X, START_Y - world->location.y);
    }

    return 0;
}
