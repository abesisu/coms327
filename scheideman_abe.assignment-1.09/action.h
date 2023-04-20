#ifndef ACTIONS_H
#define ACTIONS_H

#include "path.h"

Pokemon *choose_starter(Pokemon *first, Pokemon *second, Pokemon *third);
void enter_building(map *map, pc *pc);
void trainer_info(map *map, int num_trainers);
int fight(Pokemon *pc_pokemon, Pokemon *opponent, Data *data);
int bag(pc *pc, int trainer_battle, int wild_encounter);
int switch_pokemon(pc *pc);
int battle(pc *pc, npc *npc, Data *data);
Pokemon *encounter(pc *pc, Data *data, int manhattan_distance);
char move_pc(map *map, Data *data, pc *pc, int input, int manhattan_distance); // Returns which gate the PC entered or 0 if no gate was entered
void move_dijkstra_trainer(heap_t *path_heap, path path_map[MAP_HEIGHT][MAP_WIDTH], map *map, npc *npc, Data *data);
void move_wanderer_explorer(map *map, npc *npc, Data *data);
void move_swimmer(heap_t *path_heap, path path_map[MAP_HEIGHT][MAP_WIDTH], map *map, npc *m, coordinate_t pc_pos, Data *data);
void move_pacer(map *map, npc *p, Data *data);

#endif
