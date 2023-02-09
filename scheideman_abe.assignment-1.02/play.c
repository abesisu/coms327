#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"

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

    world->current_map = malloc(sizeof (*world->current_map));
    world->board[world->location.y][world->location.x] = world->current_map;

    generate_map(world->current_map, world->current_map->n, world->current_map->s, world->current_map->w, world->current_map->e, manhattan_distance);

    print_map(world->current_map);
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
            break;
        } else {
            printf("Please enter a valid command: n, s, w, e, f x y, or q\n");
            continue;
        }

        if (world->board[world->location.y][world->location.x] == NULL) {
            world->current_map = malloc(sizeof (*world->current_map));
            world->board[world->location.y][world->location.x] = world->current_map;

            place_gates(world);

            manhattan_distance = abs(world->location.x - START_X) + abs(world->location.y - START_Y);

            generate_map(world->current_map, world->current_map->n, world->current_map->s, world->current_map->w, world->current_map->e, manhattan_distance);
        }

        world->current_map = world->board[world->location.y][world->location.x];

        print_map(world->current_map);
        printf("(%d, %d)\n", world->location.x - START_X, START_Y - world->location.y);
    }

    // loop through world and free everything
    for (i = 0; i < WORLD_HEIGHT; i++) {
        for (j = 0; j < WORLD_WIDTH; j++) {
            if (world->board[i][j] != NULL) {
                free(world->board[i][j]);
            }
        }
    }

    free(world);

    return 0;
}