#include <stdlib.h>
#include "trainer.h"

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

void trainer_delete(trainer_t *trainer_map[MAP_HEIGHT][MAP_WIDTH])
{
    int x, y;

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            if (trainer_map[y][x] != NULL) {
                free(trainer_map[y][x]);
            }
        }
    }
}