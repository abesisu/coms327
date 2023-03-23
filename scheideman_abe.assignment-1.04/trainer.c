#include <stdlib.h>

#include "trainer.h"

/* Check if trainer can go to location based on terrain and if it is occupied or not. Return 0 if trainer can be at location. */
int check_trainer_position(map_t *map, coordinate_t pos, trainer_type_e type)
{
    int check = 1;

    if (map->terrain[pos.y][pos.x] == edge ||
        map->terrain[pos.y][pos.x] == willow ||
        map->trainer_map[pos.y][pos.x] != NULL) {
        check = 0;
    } else if ((type == swimmer && map->terrain[pos.y][pos.x] != water) ||
               (type != swimmer && map->terrain[pos.y][pos.x] == water)) {
        check = 0;
    } else if (map->terrain[pos.y][pos.x] == boulder ||
               map->terrain[pos.y][pos.x] == tree) {
        if (type != hiker) {
            check = 0;
        }
    } else if ((map->terrain[pos.y][pos.x] == gate ||
                map->terrain[pos.y][pos.x] == road ||
                map->terrain[pos.y][pos.x] == bridge) &&
               (type != pc)) {
        check = 0;
    }

    return check;
}

/* Add the PC to a road in this map. */
void place_pc(heap_t *turn_heap, map_t *map)
{
    int y, x, count;
    count = 0;

    // loop through the world once counting all the roads
    for (y = 1; y < MAP_HEIGHT; y++) {
        for (x = 1; x < MAP_WIDTH; x++) {
            if (map->terrain[y][x] == road || map->terrain[y][x] == bridge) {
                count++;
            }
        }
    }

    // create an array of the size of the number of roads
    coordinate_t start;
    coordinate_t start_options[count];

    // Add the coordinates of each road to the array
    count = 0;
    for (y = 1; y < MAP_HEIGHT; y++) {
        for (x = 1; x < MAP_WIDTH; x++) {
            if (map->terrain[y][x] == road || map->terrain[y][x] == bridge) {
                start_options[count].x = x;
                start_options[count].y = y;
                count++;
            }
        }
    }

    start = start_options[rand() % count];

    map->trainer_map[start.y][start.x] = malloc(sizeof (trainer_t));
    trainer_init(map->trainer_map[start.y][start.x], pc);

    map->trainer_map[start.y][start.x]->pos = start_options[rand() % count];
    heap_insert(turn_heap, map->trainer_map[start.y][start.x]);
}

void place_npc(heap_t *turn_heap, map_t *map, trainer_type_e type)
{
    coordinate_t start;
    int valid_character_position;

    start.x = (rand() % MAP_WIDTH - 2) + 1;
    start.y = (rand() % MAP_HEIGHT - 2) + 1;

    valid_character_position = check_trainer_position(map, start, type);

    while(!valid_character_position) {
        start.x = (rand() % MAP_WIDTH - 2) + 1;
        start.y = (rand() % MAP_HEIGHT - 2) + 1;

        valid_character_position = check_trainer_position(map, start, type);
    }

    map->trainer_map[start.y][start.x] = malloc(sizeof (trainer_t));
    trainer_init(map->trainer_map[start.y][start.x], type);

    map->trainer_map[start.y][start.x]->pos = start;
    heap_insert(turn_heap, map->trainer_map[start.y][start.x]);
}

void trainer_init(trainer_t *trainer, trainer_type_e type)
{
    coordinate_t start;
    start.x = 0;
    start.y = 0;

    trainer->type = type;
    trainer->next_turn = 0;
    trainer->pos = start;
    trainer->dir = start;

    switch (type)
    {
        case pc:
            trainer->seq_num = 0;
            break;
        case hiker:
            trainer->seq_num = 1;
            break;
        case rival:
            trainer->seq_num = 2;
            break;
        case pacer:
            trainer->seq_num = 3;
            break;
        case wanderer:
            trainer->seq_num = 4;
            break;
        case sentry:
            trainer->seq_num = 5;
            break;
        case explorer:
            trainer->seq_num = 6;
            break;
        case swimmer:
            trainer->seq_num = 7;
            break;
    }
}

void trainer_map_init(heap_t *turn_heap, map_t *map, int num_trainers)
{
    int x, y, npc_count;

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            map->trainer_map[y][x] = NULL;
        }
    }

    place_pc(turn_heap, map);

    if (num_trainers == 1) {
        place_npc(turn_heap, map, rand() % 7 + 1);
    } else if (num_trainers >= 2) {
        place_npc(turn_heap, map, hiker);
        place_npc(turn_heap, map, rival);

        npc_count = 2;
        if (num_trainers >= 7) {
            place_npc(turn_heap, map, pacer);
            place_npc(turn_heap, map, wanderer);
            place_npc(turn_heap, map, sentry);
            place_npc(turn_heap, map, explorer);
            place_npc(turn_heap, map, swimmer);
            npc_count = 7;
        }

        while(npc_count < num_trainers) {
            place_npc(turn_heap, map, rand() % 7 + 1);
            npc_count++;
        }
    }
}

void trainer_delete(map_t *map)
{
    int x, y;

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            if (map->trainer_map[y][x] != NULL) {
                free(map->trainer_map[y][x]);
            }
        }
    }
}
