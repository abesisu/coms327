#ifndef TRAINER_H
#define TRAINER_H

#include "util.h"

typedef enum trainer_type {
    pc,
    hiker,
    rival,
    pacer,
    wanderer,
    sentry,
    explorer,
    swimmer,
} trainer_type_e;

typedef struct trainer {
    trainer_type_e type;
    int next_turn; // Will be set to -1 when the trainer is defeated
    int seq_num;
    coordinate_t pos;
    coordinate_t dir; // Which direction the trainer is headed
} trainer_t;

void trainer_init(trainer_t *trainer, trainer_type_e type);

void trainer_delete(trainer_t *trainer_map[MAP_HEIGHT][MAP_WIDTH]);

#endif
