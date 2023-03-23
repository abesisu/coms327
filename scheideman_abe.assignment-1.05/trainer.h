#ifndef TRAINER_H
#define TRAINER_H

#include "map.h"

void trainer_init(trainer_t *trainer, trainer_type_e type);

void trainer_map_init(heap_t *turn_heap, map_t *map, int num_trainers);

void trainer_delete(map_t *map);

#endif
