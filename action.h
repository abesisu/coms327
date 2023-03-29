#ifndef ACTIONS_H
#define ACTIONS_H

#include "path.h"

void enter_building(map_t *map, trainer_t *pc);

void trainer_info(map_t *map, int num_trainers);

int battle();

char move_pc(map_t *map, trainer_t *pc, int input); // Returns which gate the PC entered or 0 if no gate was entered

void move_dijkstra_trainer(heap_t *path_heap, path_t path[MAP_HEIGHT][MAP_WIDTH], map_t *map, trainer_t *t);

void move_wanderer_explorer(map_t *map, trainer_t *t);

void move_swimmer(heap_t *path_heap, path_t path[MAP_HEIGHT][MAP_WIDTH], map_t *map, trainer_t *t, coordinate_t pc_pos);

void move_pacer(map_t *map, trainer_t *t);

#endif
