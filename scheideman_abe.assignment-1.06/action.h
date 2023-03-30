#ifndef ACTIONS_H
#define ACTIONS_H

#include "path.h"

class action {
    public:
        static void enter_building(map *map, pc *pc);
        static void trainer_info(map *map, int num_trainers);
        static int battle();
        static char move_pc(map *map, pc *pc, int input); // Returns which gate the PC entered or 0 if no gate was entered
        static void move_dijkstra_trainer(heap *path_heap, path path_map[MAP_HEIGHT][MAP_WIDTH], map *map, npc *npc);
        static void move_wanderer_explorer(map *map, npc *npc);
        static void move_swimmer(heap_t *path_heap, path path_map[MAP_HEIGHT][MAP_WIDTH], map *map, npc *m, coordinate_t pc_pos);
        static void move_pacer(map *map, npc *p);
};



#endif
