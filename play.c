#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "world.h"

static int32_t path_cmp(const void *key, const void *with) { // make heap compare nodes based on cost
    return ((path_t *) key)->cost - ((path_t *) with)->cost;
}

void print_view(world_t *world)
{
    int i, j;

    for (i = 0; i < MAP_HEIGHT; i++) {
        for (j = 0; j < MAP_WIDTH; j++) {
            switch (world->current_map->terrain[i][j])
            {
                case grass:
                    world->view[i][j] = '.';
                    break;
                case tall_grass:
                    world->view[i][j] = ':';
                    break;
                case boulder:
                    world->view[i][j] = '%';
                    break;
                case border:
                    world->view[i][j] = '%';
                    break;
                case tree:
                    world->view[i][j] = '^';
                    break;
                case willow:
                    world->view[i][j] = '^';
                    break;
                case water:
                    world->view[i][j] = '~';
                    break;
                case road:
                    world->view[i][j] = '#';
                    break;
                case gate:
                    world->view[i][j] = '#';
                    break;
                case pokemart:
                    world->view[i][j] = 'M';
                    break;
                case pokecenter:
                    world->view[i][j] = 'C';
                    break;
                case pc:
                    world->view[i][j] = '@';
                    break;
                case hiker:
                    world->view[i][j] = 'h';
                    break;
                case rival:
                    world->view[i][j] = 'r';
                    break;
                default:
                    world->view[i][j] = '!'; // Something went wrong
                    break;
            }
        }

        if (i < MAP_HEIGHT - 1) {
            world->view[i][MAP_WIDTH] = '\n';
        }
    }

    for (i = 0; i < MAP_HEIGHT; i++) {
        for (j = 0; j < MAP_WIDTH + 1; j++) {
            printf("%c", world->view[i][j]);
        }
    }

    printf("\n");
}

/* According to 1.03, print out the Dijkstra's path map. */
void print_path_map(path_t path[MAP_HEIGHT][MAP_WIDTH])
{
    int i, j;
    for (i = 0; i < MAP_HEIGHT; i++) {
        for (j = 0; j < MAP_WIDTH - 1; j++) {
            if (path[i][j].cost == INT_MAX) {
                printf("   ");
            } else {
                printf("%02d ", path[i][j].cost % 100);
            }
        }
        if (path[i][MAP_WIDTH - 1].cost == INT_MAX) {
            printf("  \n");
        } else {
            printf("%02d\n", path[i][MAP_WIDTH - 1].cost % 100);
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
void place_pc(map_t *map, trainer_t pc_trainer)
{
    int i, j, count;
    count = 0;

    // loop through the world once counting all the roads
    for (i = 1; i < MAP_HEIGHT; i++) {
        for (j = 1; j < MAP_WIDTH; j++) {
            if (map->terrain[i][j] == road) {
                count++;
            }
        }
    }

    // create an array of the size of the number of roads
    coordinate_t pc_start_options[count];

    // Add the coordinates of each road to the array
    count = 0;
    for (i = 1; i < MAP_HEIGHT; i++) {
        for (j = 1; j < MAP_WIDTH; j++) {
            if (map->terrain[i][j] == road) {
                pc_start_options[count].x = j;
                pc_start_options[count].y = i;
                count++;
            }
        }
    }

    // randomly pick one of those coordinates for the pc_train
    pc_trainer.position = pc_start_options[rand() % count];
    map->terrain[pc_trainer.position.y][pc_trainer.position.x] = pc;
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

int main(__attribute__((unused)) int argc, __attribute__((unused)) char *argv[])
{
    int i, j;
    world_t *world;
    trainer_t pc_trainer, hiker_trainer, rival_trainer;
    heap_t heap;
    srand(time(NULL));

    world = malloc(sizeof (*world));

    for (i = 0; i < WORLD_HEIGHT; i++) {
        for (j = 0; j < WORLD_WIDTH; j++) {
            world->board[i][j] = NULL;
        }
    }

    int fly_x, fly_y, manhattan_distance;

    world->location.x = START_X;
    world->location.y = START_Y;
    manhattan_distance = 0;

    world->board[world->location.y][world->location.x] = malloc(sizeof (*world->current_map));
    world->current_map = world->board[world->location.y][world->location.x];

    generate_map(world->current_map, world->current_map->n, world->current_map->s, world->current_map->w, world->current_map->e, manhattan_distance);


    pc_trainer.type = pc_train;
    hiker_trainer.type = hiker_train;
    rival_trainer.type = rival_train;

    place_pc(world->current_map, pc_trainer);

    print_view(world);

    heap_init(&heap, path_cmp, NULL);
    dijkstra_path(&heap, world->current_map, world->hiker_path, hiker_trainer.type, pc_trainer.position);
    dijkstra_path(&heap, world->current_map, world->rival_path, rival_trainer.type, pc_trainer.position);

    print_path_map(world->hiker_path);
    print_path_map(world->rival_path);
    printf("(%d, %d)\n", world->location.x - START_X, START_Y - world->location.y);

    char input = '\0';

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
            // loop through world and free everything
            for (i = 0; i < WORLD_HEIGHT; i++) {
                for (j = 0; j < WORLD_WIDTH; j++) {
                    if (world->board[i][j] != NULL) {
                        free(world->board[i][j]);
                    }
                }
            }

            heap_delete(&heap);
            free(world);

            break;
        } else {
            printf("Please enter a valid command: n, s, w, e, f x y, or q\n");
            continue;
        }

        if (world->board[world->location.y][world->location.x] == NULL) {
            world->board[world->location.y][world->location.x] = malloc(sizeof (*world->current_map));
            world->current_map = world->board[world->location.y][world->location.x];

            place_gates(world);

            manhattan_distance = abs(world->location.x - START_X) + abs(world->location.y - START_Y);

            generate_map(world->current_map, world->current_map->n, world->current_map->s, world->current_map->w, world->current_map->e, manhattan_distance);
        }

        world->current_map = world->board[world->location.y][world->location.x];

        place_pc(world->current_map, pc_trainer);

        print_view(world);

        dijkstra_path(&heap, world->current_map, world->hiker_path, hiker_trainer.type, pc_trainer.position);
        dijkstra_path(&heap, world->current_map, world->rival_path, rival_trainer.type, pc_trainer.position);

        print_path_map(world->hiker_path);
        print_path_map(world->rival_path);
        printf("(%d, %d)\n", world->location.x - START_X, START_Y - world->location.y);
    }

    return 0;
}
