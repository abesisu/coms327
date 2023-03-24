#include <limits.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "action.h"

int check_random_turn(int random_direction, map_t *map, coordinate_t pos, trainer_type_e type) {
    int check, terrain_cost;

    check = 0;

    if (random_direction == 0) { // check up
        if (type == wanderer || type == swimmer) {
            if (map->terrain[pos.y][pos.x] == map->terrain[pos.y - 1][pos.x] && map->trainer_map[pos.y - 1][pos.x] == NULL) {
                check = 1;
            }
        } else if (type == explorer) {
            terrain_cost = get_terrain_cost(map->terrain[pos.y - 1][pos.x], type);
            if (terrain_cost != INT_MAX && map->trainer_map[pos.y - 1][pos.x] == NULL) {
                check = 1;
            }
        }
    } else if (random_direction == 1) { // check right
        if (type == wanderer || type == swimmer) {
            if (map->terrain[pos.y][pos.x] == map->terrain[pos.y][pos.x + 1] && map->trainer_map[pos.y][pos.x + 1] == NULL) {
                check = 1;
            }
        } else if (type == explorer) {
            terrain_cost = get_terrain_cost(map->terrain[pos.y][pos.x + 1], type);
            if (terrain_cost != INT_MAX && map->trainer_map[pos.y][pos.x + 1] == NULL) {
                check = 1;
            }
        }
    } else if (random_direction == 2) { // check down
        if (type == wanderer || type == swimmer) {
            if (map->terrain[pos.y][pos.x] == map->terrain[pos.y + 1][pos.x] && map->trainer_map[pos.y + 1][pos.x] == NULL) {
                check = 1;
            }
        } else if (type == explorer) {
            terrain_cost = get_terrain_cost(map->terrain[pos.y + 1][pos.x], type);
            if (terrain_cost != INT_MAX && map->trainer_map[pos.y + 1][pos.x] == NULL) {
                check = 1;
            }
        }
    }  else if (random_direction == 3) { // check left
        if (type == wanderer || type == swimmer) {
            if (map->terrain[pos.y][pos.x] == map->terrain[pos.y][pos.x - 1] && map->trainer_map[pos.y][pos.x - 1] == NULL) {
                check = 1;
            }
        } else if (type == explorer) {
            terrain_cost = get_terrain_cost(map->terrain[pos.y][pos.x - 1], type);
            if (terrain_cost != INT_MAX && map->trainer_map[pos.y][pos.x - 1] == NULL) {
                check = 1;
            }
        }
    }

    return check;
}

void random_turn(map_t *map, trainer_t *t) {
    int random_direction, valid_direction;
    int num_fail;
    int battle_outcome;

    num_fail = 0;
    random_direction = rand() % 4;
    valid_direction = check_random_turn(random_direction, map, t->pos, t->type);
    while (!valid_direction) {
        num_fail++;
        if (num_fail > 20) {
            return;
        }
        random_direction = rand() % 4;
        valid_direction = check_random_turn(random_direction, map, t->pos, t->type);
    }

    if (random_direction == 0) { // GO UP
        t->dir.y = -1;
        t->dir.x = 0;
        if (map->trainer_map[t->pos.y - 1][t->pos.x    ] != NULL &&
            map->trainer_map[t->pos.y - 1][t->pos.x    ]->type == pc) {
            battle_outcome = battle();

            if (battle_outcome == 1) { // npc won
                map->trainer_map[map->pc_pos.y][map->pc_pos.x]->next_turn = -1;
                random_turn(map, t);
            } else if (battle_outcome == -1) { // npc lost
                t->next_turn = -1;
            }
        } else {
            map->trainer_map[t->pos.y][t->pos.x] = NULL;
            t->pos.y--;
            map->trainer_map[t->pos.y][t->pos.x] = t;
        }
    } else if (random_direction == 1) { // go right
        t->dir.y = 0;
        t->dir.x = 1;
        if (map->trainer_map[t->pos.y    ][t->pos.x + 1] != NULL &&
            map->trainer_map[t->pos.y    ][t->pos.x + 1]->type == pc) {
            battle_outcome = battle();

            if (battle_outcome == 1) { // npc won
                map->trainer_map[map->pc_pos.y][map->pc_pos.x]->next_turn = -1;
                random_turn(map, t);
            } else if (battle_outcome == -1) { // npc lost
                t->next_turn = -1;
            }
        } else {
            map->trainer_map[t->pos.y][t->pos.x] = NULL;
            t->pos.x++;
            map->trainer_map[t->pos.y][t->pos.x] = t;
        }
    } else if (random_direction == 2) { // go down
        t->dir.y = 1;
        t->dir.x = 0;
        if (map->trainer_map[t->pos.y + 1][t->pos.x    ] != NULL &&
            map->trainer_map[t->pos.y + 1][t->pos.x    ]->type == pc) {
            battle_outcome = battle();

            if (battle_outcome == 1) { // npc won
                map->trainer_map[map->pc_pos.y][map->pc_pos.x]->next_turn = -1;
                random_turn(map, t);
            } else if (battle_outcome == -1) { // npc lost
                t->next_turn = -1;
            }
        } else {
            map->trainer_map[t->pos.y][t->pos.x] = NULL;
            t->pos.y++;
            map->trainer_map[t->pos.y][t->pos.x] = t;
        }
    } else if (random_direction == 3) { // go left
        t->dir.y = 0;
        t->dir.x = -1;
        if (map->trainer_map[t->pos.y    ][t->pos.x - 1] != NULL &&
            map->trainer_map[t->pos.y    ][t->pos.x - 1]->type == pc) {
            battle_outcome = battle();

            if (battle_outcome == 1) { // npc won
                map->trainer_map[map->pc_pos.y][map->pc_pos.x]->next_turn = -1;
                random_turn(map, t);
            } else if (battle_outcome == -1) { // npc lost
                t->next_turn = -1;
            }
        } else {
            map->trainer_map[t->pos.y][t->pos.x] = NULL;
            t->pos.x--;
            map->trainer_map[t->pos.y][t->pos.x] = t;
        }
    }
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    box(local_win, 0 , 0);		/* 0, 0 gives default characters
					 * for the vertical and horizontal
					 * lines			*/
    wrefresh(local_win);		/* Show that box 		*/

    return local_win;
}

void destroy_win(WINDOW *local_win)
{
    /* box(local_win, ' ', ' '); : This won't produce the desired
     * result of erasing the window. It will leave it's four corners
     * and so an ugly remnant of window.
     */
    wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    /* The parameters taken are
     * 1. win: the window on which to operate
     * 2. ls: character to be used for the left side of the window
     * 3. rs: character to be used for the right side of the window
     * 4. ts: character to be used for the top side of the window
     * 5. bs: character to be used for the bottom side of the window
     * 6. tl: character to be used for the top left corner of the window
     * 7. tr: character to be used for the top right corner of the window
     * 8. bl: character to be used for the bottom left corner of the window
     * 9. br: character to be used for the bottom right corner of the window
     */
    wrefresh(local_win);
    delwin(local_win);
}

void enter_building(map_t *map, trainer_t *pc)
{
    int key;
    WINDOW *building_win;

    if (map->terrain[pc->pos.y][pc->pos.x] == pokecenter || map->terrain[pc->pos.y][pc->pos.x] == pokemart) {
        building_win = create_newwin(24, 80, 0, 0);
        mvwprintw(building_win, 1, 1, "You are in a building. Press '<' to exit.");
        wrefresh(building_win);

        while((key = getch()) != '<')
        {
            wprintw(building_win, "You are in a building. Press '<' to exit.");
            wrefresh(building_win);
        }

        destroy_win(building_win);

        pc->next_turn = pc->next_turn + 10;
    }
}

void trainer_info(map_t *map)
{
    int x, y, npc_rise, npc_run, key, i, shift;
    char trainer_char;
    char output[25];
    WINDOW *trainer_win;
    char *trainer_info[map->num_trainers];

    trainer_win = create_newwin(10, 24, 1, 0);
    i = 0;
    keypad(trainer_win, TRUE);

    for (y = 1; y < MAP_HEIGHT - 1; y++) {
        for (x = 1; x < MAP_WIDTH - 1; x++) {
            if (map->trainer_map[y][x] != NULL) {
                switch (map->trainer_map[y][x]->type)
                {
                    case pc:
                        trainer_char = '@';
                        break;
                    case hiker:
                        trainer_char = 'h';
                        break;
                    case rival:
                        trainer_char = 'r';
                        break;
                    case pacer:
                        trainer_char = 'p';
                        break;
                    case wanderer:
                        trainer_char = 'w';
                        break;
                    case sentry:
                        trainer_char = 's';
                        break;
                    case explorer:
                        trainer_char = 'e';
                        break;
                    case swimmer:
                        trainer_char = 'm';
                        break;
                }

                npc_rise = map->pc_pos.y - map->trainer_map[y][x]->pos.y;
                npc_run = map->pc_pos.x - map->trainer_map[y][x]->pos.x;

                if (npc_rise < 0) { // npc is south of pc
                    if (npc_run < 0) { // npc is east of pc
                        sprintf(output, "%c, %d %s and %d %s\n", trainer_char, npc_rise * -1, "south", npc_run * -1, "east");
                    } else { // npc is west of pc
                        sprintf(output, "%c, %d %s and %d %s\n", trainer_char, npc_rise * -1, "south", npc_run, "west");
                    }
                } else { // npc is north of pc
                    if (npc_run < 0) { // npc is east of pc
                        sprintf(output, "%c, %d %s and %d %s\n", trainer_char, npc_rise, "north", npc_run * -1, "east");
                    } else { // npc is west of pc
                        sprintf(output, "%c, %d %s and %d %s\n", trainer_char, npc_rise, "north", npc_run, "west");
                    }
                }

                trainer_info[i] = malloc(sizeof (output));
                strcpy(trainer_info[i], output);
                i++;
            }
        }
    }

    for (i = 0; i < map->num_trainers || i < 10; i++) {
        mvwprintw(trainer_win, i, 0, trainer_info[i]);
    }

    wrefresh(trainer_win);

    shift = 0;
    while((key = getch()) != 27)
    {
        switch (key)
        {
            case KEY_DOWN:
                if (map->num_trainers > 10 && map->num_trainers - shift > 10) {
                    shift++;
                    for (i = 0; i < 10 && i + shift < map->num_trainers; i++) {
                        mvwprintw(trainer_win, i, 0, trainer_info[i + shift]);
                    }
                    wrefresh(trainer_win);
                }
                break;
            case KEY_UP:
                if (shift > 0 && map->num_trainers > 10) {
                    shift--;
                    for (i = 0; i < 10 && i + shift < map->num_trainers; i++) {
                        mvwprintw(trainer_win, i, 0, trainer_info[i + shift]);
                    }
                    wrefresh(trainer_win);
                }
                break;
            default:
                continue;
        }
    }

    for (i = 0; i < map->num_trainers; i++) {
        free(trainer_info[i]);
    }

    destroy_win(trainer_win);
}

int battle()
{
    int key;
    WINDOW *battle_win;

    battle_win = create_newwin(24, 80, 0, 0);
    mvwprintw(battle_win, 1, 1, "You are in a battle. Press '<' to exit.");
    wrefresh(battle_win);

    while((key = getch()) != '<')
    {
        wprintw(battle_win, "You are in a battle. Press '<' to exit.");
        wrefresh(battle_win);
    }

    destroy_win(battle_win);

    return 0;
}

void move_pc(map_t *map, trainer_t *pc, int input)
{
    coordinate_t new_pos;
    int battle_outcome;

    if (input == '7' || input == 'y') {
        new_pos.y = pc->pos.y - 1;
        new_pos.x = pc->pos.x - 1;
    } else if (input == '8' || input == 'k') {
        new_pos.y = pc->pos.y - 1;
        new_pos.x = pc->pos.x    ;
    } else if (input == '9' || input == 'u') {
        new_pos.y = pc->pos.y - 1;
        new_pos.x = pc->pos.x + 1;
    } else if (input == '6' || input == 'l') {
        new_pos.y = pc->pos.y    ;
        new_pos.x = pc->pos.x + 1;
    } else if (input == '3' || input == 'n') {
        new_pos.y = pc->pos.y + 1;
        new_pos.x = pc->pos.x + 1;
    } else if (input == '2' || input == 'j') {
        new_pos.y = pc->pos.y + 1;
        new_pos.x = pc->pos.x    ;
    } else if (input == '1' || input == 'b') {
        new_pos.y = pc->pos.y + 1;
        new_pos.x = pc->pos.x - 1;
    } else if (input == '4' || input == 'h') {
        new_pos.y = pc->pos.y    ;
        new_pos.x = pc->pos.x - 1;
    }

    if (get_terrain_cost(map->terrain[new_pos.y][new_pos.x], pc->type) != INT_MAX) {
        if (map->trainer_map[new_pos.y][new_pos.x] != NULL) {
            if (map->trainer_map[new_pos.y][new_pos.x]->next_turn > -1) {
                battle_outcome = battle();

                if (battle_outcome == 1) { // npc won
                    pc->next_turn = -1;
                } else if (battle_outcome == -1) { // npc lost
                    map->trainer_map[new_pos.y][new_pos.x]->next_turn = -1;
                }
            }
        } else {
            map->trainer_map[pc->pos.y][pc->pos.x] = NULL;
            pc->pos.y = new_pos.y;
            pc->pos.x = new_pos.x;
            map->trainer_map[pc->pos.y][pc->pos.x] = pc;
            map->pc_pos = pc->pos;
        }

        if (pc->next_turn > -1) {
            pc->next_turn = pc->next_turn + get_terrain_cost(map->terrain[pc->pos.y][pc->pos.x], pc->type);
        }
    }
}

void move_dijkstra_trainer(heap_t *path_heap, path_t path[MAP_HEIGHT][MAP_WIDTH], map_t *map, trainer_t *t)
{
    path_t *cheapest_path;
    int battle_outcome;

    cheapest_path = &path[t->pos.y - 1][t->pos.x - 1];
    battle_outcome = 0;

    dijkstra_path(path_heap, map, path, t->type);

    if (path[t->pos.y - 1][t->pos.x].cost < cheapest_path->cost) {
        cheapest_path = &path[t->pos.y - 1][t->pos.x];
    }
    if (path[t->pos.y - 1][t->pos.x + 1].cost < cheapest_path->cost) {
        cheapest_path = &path[t->pos.y - 1][t->pos.x + 1];
    }
    if (path[t->pos.y][t->pos.x - 1].cost < cheapest_path->cost) {
        cheapest_path = &path[t->pos.y][t->pos.x - 1];
    }
    if (path[t->pos.y][t->pos.x + 1].cost < cheapest_path->cost) {
        cheapest_path = &path[t->pos.y][t->pos.x + 1];
    }
    if (path[t->pos.y + 1][t->pos.x - 1].cost < cheapest_path->cost) {
        cheapest_path = &path[t->pos.y + 1][t->pos.x - 1];
    }
    if (path[t->pos.y + 1][t->pos.x].cost < cheapest_path->cost) {
        cheapest_path = &path[t->pos.y + 1][t->pos.x];
    }
    if (path[t->pos.y + 1][t->pos.x + 1].cost < cheapest_path->cost) {
        cheapest_path = &path[t->pos.y + 1][t->pos.x + 1];
    }
    if (cheapest_path->cost == 0) {
        cheapest_path = &path[t->pos.y    ][t->pos.x    ];

        battle_outcome = battle();

        if (battle_outcome == 1) { // npc won
            map->trainer_map[map->pc_pos.y][map->pc_pos.x]->next_turn = -1;
        } else if (battle_outcome == -1) { // npc lost
            t->next_turn = -1;
        }
    }

    map->trainer_map[t->pos.y][t->pos.x] = NULL;
    t->pos.y = cheapest_path->coordinate.y;
    t->pos.x = cheapest_path->coordinate.x;
    map->trainer_map[t->pos.y][t->pos.x] = t;
    if (t->next_turn > -1) {
        t->next_turn = t->next_turn + cheapest_path->terrain.cost;
    }
}

void move_wanderer_explorer(map_t *map, trainer_t *t)
{
    int move_cost, battle_outcome;
    terrain_e current_terrain;

    current_terrain = map->terrain[t->pos.y][t->pos.x];

    // just starting or already going up and need to continue
    if (t->dir.x == 0 && t->dir.y <= 0) {
        move_cost = get_terrain_cost(map->terrain[t->pos.y - 1][t->pos.x], t->type);

        if ((t->type == wanderer && (current_terrain == map->terrain[t->pos.y - 1][t->pos.x])) ||
            (t->type == explorer && (move_cost != INT_MAX))) {
            t->dir.y = -1;
            if (map->trainer_map[t->pos.y - 1][t->pos.x    ] == NULL) {
                map->trainer_map[t->pos.y][t->pos.x] = NULL;
                t->pos.y--;
                map->trainer_map[t->pos.y][t->pos.x] = t;
            } else if (map->trainer_map[t->pos.y - 1][t->pos.x    ]->type == pc) {
                battle_outcome = battle();

                if (battle_outcome == 1) { // npc won
                    map->trainer_map[map->pc_pos.y][map->pc_pos.x]->next_turn = -1;
                    random_turn(map, t);
                } else if (battle_outcome == -1) { // npc lost
                    t->next_turn = -1;
                }
            } else {
                random_turn(map, t);
            }
        } else {
            random_turn(map, t);
        }
        // just starting or already going right and need to continue
    } else if (t->dir.x >= 0 && t->dir.y == 0) {
        move_cost = get_terrain_cost(map->terrain[t->pos.y][t->pos.x + 1], t->type);

        if ((t->type == wanderer && (current_terrain == map->terrain[t->pos.y][t->pos.x + 1])) ||
            (t->type == explorer && (move_cost != INT_MAX))) {
            t->dir.x = 1;
            if (map->trainer_map[t->pos.y    ][t->pos.x + 1] == NULL) {
                map->trainer_map[t->pos.y][t->pos.x] = NULL;
                t->pos.x++;
                map->trainer_map[t->pos.y][t->pos.x] = t;
            }  else if (map->trainer_map[t->pos.y    ][t->pos.x + 1]->type == pc) {
                battle_outcome = battle();

                if (battle_outcome == 1) { // npc won
                    map->trainer_map[map->pc_pos.y][map->pc_pos.x]->next_turn = -1;
                    random_turn(map, t);
                } else if (battle_outcome == -1) { // npc lost
                    t->next_turn = -1;
                }
            } else {
                random_turn(map, t);
            }
        } else {
            random_turn(map, t);
        }
        // just starting or already going down and need to continue
    } else if (t->dir.x == 0 && t->dir.y >= 0) {
        move_cost = get_terrain_cost(map->terrain[t->pos.y + 1][t->pos.x], t->type);

        if ((t->type == wanderer && (current_terrain == map->terrain[t->pos.y + 1][t->pos.x])) ||
            (t->type == explorer && (move_cost != INT_MAX))) {
            t->dir.y = 1;
            if (map->trainer_map[t->pos.y + 1][t->pos.x    ] == NULL) {
                map->trainer_map[t->pos.y][t->pos.x] = NULL;
                t->pos.y++;
                map->trainer_map[t->pos.y][t->pos.x] = t;
            } else if (map->trainer_map[t->pos.y + 1][t->pos.x    ]->type == pc) {
                battle_outcome = battle();

                if (battle_outcome == 1) { // npc won
                    map->trainer_map[map->pc_pos.y][map->pc_pos.x]->next_turn = -1;
                    random_turn(map, t);
                } else if (battle_outcome == -1) { // npc lost
                    t->next_turn = -1;
                }
            } else {
                random_turn(map, t);
            }
        } else {
            random_turn(map, t);
        }
        // just starting or already going left and need to continue
    } else if (t->dir.x <= 0 && t->dir.y == 0) {
        move_cost = get_terrain_cost(map->terrain[t->pos.y    ][t->pos.x - 1], t->type);

        if ((t->type == wanderer && (current_terrain == map->terrain[t->pos.y    ][t->pos.x - 1])) ||
            (t->type == explorer && (move_cost != INT_MAX))) {
            t->dir.x = -1;
            if (map->trainer_map[t->pos.y    ][t->pos.x - 1] == NULL) {
                map->trainer_map[t->pos.y][t->pos.x] = NULL;
                t->pos.x--;
                map->trainer_map[t->pos.y][t->pos.x] = t;
            } else if (map->trainer_map[t->pos.y    ][t->pos.x - 1]->type == pc) {
                battle_outcome = battle();

                if (battle_outcome == 1) { // npc won
                    map->trainer_map[map->pc_pos.y][map->pc_pos.x]->next_turn = -1;
                    random_turn(map, t);
                } else if (battle_outcome == -1) { // npc lost
                    t->next_turn = -1;
                }
            } else {
                random_turn(map, t);
            }
        } else {
            random_turn(map, t);
        }
    }

    if (t->next_turn > -1) {
        t->next_turn = t->next_turn + get_terrain_cost(map->terrain[t->pos.y][t->pos.x], t->type);
    }
}

void move_swimmer(heap_t *path_heap, path_t path[MAP_HEIGHT][MAP_WIDTH], map_t *map, trainer_t *t, coordinate_t pc_pos)
{
    int battle_outcome;

    // path to pc
    if ((map->terrain[pc_pos.y - 1][pc_pos.x    ] == water ||
         map->terrain[pc_pos.y - 1][pc_pos.x    ] == bridge) ||
        (map->terrain[pc_pos.y    ][pc_pos.x + 1] == water ||
         map->terrain[pc_pos.y    ][pc_pos.x + 1] == bridge) ||
        (map->terrain[pc_pos.y + 1][pc_pos.x    ] == water ||
         map->terrain[pc_pos.y + 1][pc_pos.x    ] == bridge) ||
        (map->terrain[pc_pos.y    ][pc_pos.x - 1] == water ||
         map->terrain[pc_pos.y    ][pc_pos.x - 1] == bridge)) {
        move_dijkstra_trainer(path_heap, path, map, t);
        // move similar to wanderer
    } else {
        // just starting or already going up and need to continue
        if (t->dir.x == 0 && t->dir.y <= 0) {
            if (map->terrain[t->pos.y - 1][t->pos.x] == water ||
                map->terrain[t->pos.y - 1][t->pos.x] == bridge) {
                t->dir.y = -1;
                if (map->trainer_map[t->pos.y - 1][t->pos.x] == NULL) {
                    map->trainer_map[t->pos.y][t->pos.x] = NULL;
                    t->pos.y--;
                    map->trainer_map[t->pos.y][t->pos.x] = t;
                } else if (map->trainer_map[t->pos.y - 1][t->pos.x]->type == pc) {
                    battle_outcome = battle();

                    if (battle_outcome == 1) { // npc won
                        map->trainer_map[map->pc_pos.y][map->pc_pos.x]->next_turn = -1;
                        random_turn(map, t);
                    } else if (battle_outcome == -1) { // npc lost
                        t->next_turn = -1;
                    }
                } else {
                    random_turn(map, t);
                }
            } else {
                random_turn(map, t);
            }
            // just starting or already going right and need to continue
        } else if (t->dir.x >= 0 && t->dir.y == 0) {
            if (map->terrain[t->pos.y][t->pos.x + 1] == water ||
                map->terrain[t->pos.y][t->pos.x + 1] == bridge) {
                t->dir.x = 1;
                if (map->trainer_map[t->pos.y][t->pos.x + 1] == NULL) {
                    map->trainer_map[t->pos.y][t->pos.x] = NULL;
                    t->pos.x++;
                    map->trainer_map[t->pos.y][t->pos.x] = t;
                } else if (map->trainer_map[t->pos.y][t->pos.x + 1]->type == pc) {
                    battle_outcome = battle();

                    if (battle_outcome == 1) { // npc won
                        map->trainer_map[map->pc_pos.y][map->pc_pos.x]->next_turn = -1;
                        random_turn(map, t);
                    } else if (battle_outcome == -1) { // npc lost
                        t->next_turn = -1;
                    }
                } else {
                    random_turn(map, t);
                }
            } else {
                random_turn(map, t);
            }
            // just starting or already going down and need to continue
        } else if (t->dir.x == 0 && t->dir.y >= 0) {
            if (map->terrain[t->pos.y + 1][t->pos.x] == water ||
                map->terrain[t->pos.y + 1][t->pos.x] == bridge) {
                t->dir.y = 1;
                if (map->trainer_map[t->pos.y + 1][t->pos.x] == NULL) {
                    map->trainer_map[t->pos.y][t->pos.x] = NULL;
                    t->pos.y++;
                    map->trainer_map[t->pos.y][t->pos.x] = t;
                } else if (map->trainer_map[t->pos.y + 1][t->pos.x]->type == pc) {
                    battle_outcome = battle();

                    if (battle_outcome == 1) { // npc won
                        map->trainer_map[map->pc_pos.y][map->pc_pos.x]->next_turn = -1;
                        random_turn(map, t);
                    } else if (battle_outcome == -1) { // npc lost
                        t->next_turn = -1;
                    }
                } else {
                    random_turn(map, t);
                }
            } else {
                random_turn(map, t);
            }
            // just starting or already going left and need to continue
        } else if (t->dir.x <= 0 && t->dir.y == 0) {
            if (map->terrain[t->pos.y][t->pos.x - 1] == water ||
                map->terrain[t->pos.y][t->pos.x - 1] == bridge) {
                t->dir.x = -1;
                if (map->trainer_map[t->pos.y][t->pos.x - 1] == NULL) {
                    map->trainer_map[t->pos.y][t->pos.x] = NULL;
                    t->pos.x--;
                    map->trainer_map[t->pos.y][t->pos.x] = t;
                } else if (map->trainer_map[t->pos.y][t->pos.x - 1]->type == pc) {
                    battle_outcome = battle();

                    if (battle_outcome == 1) { // npc won
                        map->trainer_map[map->pc_pos.y][map->pc_pos.x]->next_turn = -1;
                        random_turn(map, t);
                    } else if (battle_outcome == -1) { // npc lost
                        t->next_turn = -1;
                    }
                } else {
                    random_turn(map, t);
                }
            } else {
                random_turn(map, t);
            }
        }

        if (t->next_turn > -1) {
            t->next_turn = t->next_turn + get_terrain_cost(map->terrain[t->pos.y][t->pos.x], t->type);
        }
    }
}

void move_pacer(map_t *map, trainer_t *t)
{
    terrain_e current_terrain;
    int battle_outcome;
    int moved;

    current_terrain = map->terrain[t->pos.y][t->pos.x];
    moved = 0;
    // Just starting or moving right and the right is open
    if (t->dir.x >= 0 && map->terrain[t->pos.y][t->pos.x + 1] == current_terrain) {
        if (map->trainer_map[t->pos.y    ][t->pos.x + 1] == NULL) {
            moved = 1;
            t->dir.x = 1;
            map->trainer_map[t->pos.y][t->pos.x] = NULL;
            t->pos.x++;
            map->trainer_map[t->pos.y][t->pos.x] = t;
        } else if (map->trainer_map[t->pos.y    ][t->pos.x + 1]->type == pc) {
            battle_outcome = battle();

            if (battle_outcome == 1) { // npc won
                map->trainer_map[map->pc_pos.y][map->pc_pos.x]->next_turn = -1;
            } else if (battle_outcome == -1) { // npc lost
                t->next_turn = -1;
            }
        }
        // Just starting or moving left and checking if left is open
    } else if (t->dir.x <= 0 && map->terrain[t->pos.y][t->pos.x - 1] == current_terrain) {
        if (map->trainer_map[t->pos.y    ][t->pos.x - 1] == NULL) {
            moved = 1;
            t->dir.x = -1;
            map->trainer_map[t->pos.y][t->pos.x] = NULL;
            t->pos.x--;
            map->trainer_map[t->pos.y][t->pos.x] = t;
        } else if (map->trainer_map[t->pos.y    ][t->pos.x - 1]->type == pc) {
            battle_outcome = battle();

            if (battle_outcome == 1) { // npc won
                map->trainer_map[map->pc_pos.y][map->pc_pos.x]->next_turn = -1;
            } else if (battle_outcome == -1) { // npc lost
                t->next_turn = -1;
            }
        }
    }

    // moving left but need to go back right
    if (!moved && t->dir.x == -1 && map->terrain[t->pos.y][t->pos.x + 1] == current_terrain) {
        t->dir.x = 1;
        if (map->trainer_map[t->pos.y    ][t->pos.x + 1] == NULL) {
            moved = 1;
            map->trainer_map[t->pos.y][t->pos.x] = NULL;
            t->pos.x++;
            map->trainer_map[t->pos.y][t->pos.x] = t;
        } else if (map->trainer_map[t->pos.y    ][t->pos.x + 1]->type == pc) {
            battle_outcome = battle();

            if (battle_outcome == 1) { // npc won
                map->trainer_map[map->pc_pos.y][map->pc_pos.x]->next_turn = -1;
            } else if (battle_outcome == -1) { // npc lost
                t->next_turn = -1;
            }
        }
    }

    // moving right but need to go back left
    if (!moved && t->dir.x == 1 && map->terrain[t->pos.y][t->pos.x - 1] == current_terrain) { // go back left
        t->dir.x = -1;
        if (map->trainer_map[t->pos.y    ][t->pos.x - 1] == NULL) {
            moved = 1;
            map->trainer_map[t->pos.y][t->pos.x] = NULL;
            t->pos.x--;
            map->trainer_map[t->pos.y][t->pos.x] = t;
        } else if (map->trainer_map[t->pos.y    ][t->pos.x - 1]->type == pc) {
            battle_outcome = battle();

            if (battle_outcome == 1) { // npc won
                map->trainer_map[map->pc_pos.y][map->pc_pos.x]->next_turn = -1;
            } else if (battle_outcome == -1) { // npc lost
                t->next_turn = -1;
            }
        } else {
            if (map->trainer_map[t->pos.y    ][t->pos.x + 1] == NULL) {
                t->dir.x = 1;
                moved = 1;
                map->trainer_map[t->pos.y][t->pos.x] = NULL;
                t->pos.x++;
                map->trainer_map[t->pos.y][t->pos.x] = t;
            } else if (map->trainer_map[t->pos.y    ][t->pos.x + 1]->type == pc) {
                battle_outcome = battle();

                if (battle_outcome == 1) { // npc won
                    map->trainer_map[map->pc_pos.y][map->pc_pos.x]->next_turn = -1;
                } else if (battle_outcome == -1) { // npc lost
                    t->next_turn = -1;
                }
            }
        }
    }

    if (t->next_turn > -1) {
        t->next_turn = t->next_turn + get_terrain_cost(map->terrain[t->pos.y][t->pos.x], t->type);
    }
}
