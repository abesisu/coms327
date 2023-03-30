#include <ncurses.h>
#include <string>
#include <cstring>

#include "action.h"

int check_random_turn(int random_direction, map *map, coordinate_t pos, trainer_type_e type) {
    int check, terrain_cost;
    path p;

    check = 0;

    if (random_direction == 0) { // check up
        if (type == wanderer_e || type == swimmer_e) {
            if (map->terrain_map[pos.y][pos.x] == map->terrain_map[pos.y - 1][pos.x] && map->trainer_map[pos.y - 1][pos.x] == nullptr) {
                check = 1;
            }
        } else if (type == explorer_e) {
            terrain_cost = p.calculate_terrain_cost(map->terrain_map[pos.y - 1][pos.x], type);
            if (terrain_cost != INT_MAX && map->trainer_map[pos.y - 1][pos.x] == nullptr) {
                check = 1;
            }
        }
    } else if (random_direction == 1) { // check right
        if (type == wanderer_e || type == swimmer_e) {
            if (map->terrain_map[pos.y][pos.x] == map->terrain_map[pos.y][pos.x + 1] && map->trainer_map[pos.y][pos.x + 1] == nullptr) {
                check = 1;
            }
        } else if (type == explorer_e) {
            terrain_cost = p.calculate_terrain_cost(map->terrain_map[pos.y][pos.x + 1], type);
            if (terrain_cost != INT_MAX && map->trainer_map[pos.y][pos.x + 1] == nullptr) {
                check = 1;
            }
        }
    } else if (random_direction == 2) { // check down
        if (type == wanderer_e || type == swimmer_e) {
            if (map->terrain_map[pos.y][pos.x] == map->terrain_map[pos.y + 1][pos.x] && map->trainer_map[pos.y + 1][pos.x] == nullptr) {
                check = 1;
            }
        } else if (type == explorer_e) {
            terrain_cost = p.calculate_terrain_cost(map->terrain_map[pos.y + 1][pos.x], type);
            if (terrain_cost != INT_MAX && map->trainer_map[pos.y + 1][pos.x] == nullptr) {
                check = 1;
            }
        }
    }  else if (random_direction == 3) { // check left
        if (type == wanderer_e || type == swimmer_e) {
            if (map->terrain_map[pos.y][pos.x] == map->terrain_map[pos.y][pos.x - 1] && map->trainer_map[pos.y][pos.x - 1] == nullptr) {
                check = 1;
            }
        } else if (type == explorer_e) {
            terrain_cost = path::calculate_terrain_cost(map->terrain_map[pos.y][pos.x - 1], type);
            if (terrain_cost != INT_MAX && map->trainer_map[pos.y][pos.x - 1] == nullptr) {
                check = 1;
            }
        }
    }

    return check;
}

void random_turn(map *map, trainer *t) {
    int random_direction, valid_direction;
    int num_fail;
    int battle_outcome;

    num_fail = 0;
    random_direction = rand() % 4;
    valid_direction = check_random_turn(random_direction, map, t->get_pos(), t->get_type());
    while (!valid_direction) {
        num_fail++;
        if (num_fail > 20) {
            return;
        }
        random_direction = rand() % 4;
        valid_direction = check_random_turn(random_direction, map, t->get_pos(), t->get_type());
    }

    if (random_direction == 0) { // GO UP
        t->set_dir_y(-1);
        t->set_dir_x(0);
        if (map->trainer_map[t->get_pos().y - 1][t->get_pos().x    ] != nullptr &&
            map->trainer_map[t->get_pos().y - 1][t->get_pos().x    ]->get_type() == pc_e) {
            battle_outcome = action::battle();

            if (battle_outcome == 1) { // npc won
                map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x]->set_next_turn(-1);
                random_turn(map, t);
            } else if (battle_outcome == -1) { // npc lost
                t->set_next_turn(-1);
            }
        } else {
            map->trainer_map[t->get_pos().y][t->get_pos().x] = nullptr;
            t->set_pos_y(t->get_pos().y - 1);
            map->trainer_map[t->get_pos().y][t->get_pos().x] = t;
        }
    } else if (random_direction == 1) { // go right
        t->set_dir_y(0);
        t->set_dir_x(1);
        if (map->trainer_map[t->get_pos().y    ][t->get_pos().x + 1] != nullptr &&
            map->trainer_map[t->get_pos().y    ][t->get_pos().x + 1]->get_type() == pc_e) {
            battle_outcome = action::battle();

            if (battle_outcome == 1) { // npc won
                map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x]->set_next_turn(-1);
                random_turn(map, t);
            } else if (battle_outcome == -1) { // npc lost
                t->set_next_turn(-1);
            }
        } else {
            map->trainer_map[t->get_pos().y][t->get_pos().x] = nullptr;
            t->set_pos_x(t->get_pos().x + 1);
            map->trainer_map[t->get_pos().y][t->get_pos().x] = t;
        }
    } else if (random_direction == 2) { // go down
        t->set_dir_y(1);
        t->set_dir_x(0);
        if (map->trainer_map[t->get_pos().y + 1][t->get_pos().x    ] != nullptr &&
            map->trainer_map[t->get_pos().y + 1][t->get_pos().x    ]->get_type() == pc_e) {
            battle_outcome = action::battle();

            if (battle_outcome == 1) { // npc won
                map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x]->set_next_turn(-1);
                random_turn(map, t);
            } else if (battle_outcome == -1) { // npc lost
                t->set_next_turn(-1);
            }
        } else {
            map->trainer_map[t->get_pos().y][t->get_pos().x] = nullptr;
            t->set_pos_y(t->get_pos().y + 1);
            map->trainer_map[t->get_pos().y][t->get_pos().x] = t;
        }
    } else if (random_direction == 3) { // go left
        t->set_dir_y(0);
        t->set_dir_x(-1);
        if (map->trainer_map[t->get_pos().y    ][t->get_pos().x - 1] != nullptr &&
            map->trainer_map[t->get_pos().y    ][t->get_pos().x - 1]->get_type() == pc_e) {
            battle_outcome = action::battle();

            if (battle_outcome == 1) { // npc won
                map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x]->set_next_turn(-1);
                random_turn(map, t);
            } else if (battle_outcome == -1) { // npc lost
                t->set_next_turn(-1);
            }
        } else {
            map->trainer_map[t->get_pos().y][t->get_pos().x] = nullptr;
            t->set_pos_x(t->get_pos().x - 1);
            map->trainer_map[t->get_pos().y][t->get_pos().x] = t;
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

void action::enter_building(map *map, pc *pc)
{
    int key;
    WINDOW *building_win;

    if (map->terrain_map[pc->get_pos().y][pc->get_pos().x] == pokecenter || map->terrain_map[pc->get_pos().y][pc->get_pos().x] == pokemart) {
        building_win = create_newwin(24, 80, 0, 0);
        mvwprintw(building_win, 1, 1, "You are in a building. Press '<' to exit.");
        wrefresh(building_win);

        while((key = getch()) != '<')
        {
            wprintw(building_win, "You are in a building. Press '<' to exit.");
            wrefresh(building_win);
        }

        destroy_win(building_win);

        pc->set_next_turn(pc->get_next_turn() + 10);
    }
}

void action::trainer_info(map *map, int num_trainers)
{
    int x, y, npc_rise, npc_run, key, i, shift;
    char trainer_char;
    std::string output;
    WINDOW *trainer_win;
    char *trainer_info[num_trainers];

    trainer_win = create_newwin(10, 24, 1, 0);
    i = 0;
    keypad(trainer_win, TRUE);

    for (y = 1; y < MAP_HEIGHT - 1; y++) {
        for (x = 1; x < MAP_WIDTH - 1; x++) {
            if (map->trainer_map[y][x] != nullptr) {
                output = "";
                switch (map->trainer_map[y][x]->get_type())
                {
                    case pc_e:
                        trainer_char = '@';
                        break;
                    case hiker_e:
                        trainer_char = 'h';
                        break;
                    case rival_e:
                        trainer_char = 'r';
                        break;
                    case pacer_e:
                        trainer_char = 'p';
                        break;
                    case wanderer_e:
                        trainer_char = 'w';
                        break;
                    case sentry_e:
                        trainer_char = 's';
                        break;
                    case explorer_e:
                        trainer_char = 'e';
                        break;
                    case swimmer_e:
                        trainer_char = 'm';
                        break;
                }

                npc_rise = map->get_pc_pos().y - map->trainer_map[y][x]->get_pos().y;
                npc_run = map->get_pc_pos().x - map->trainer_map[y][x]->get_pos().x;

                if (npc_rise < 0) { // npc is south of pc
                    if (npc_run < 0) { // npc is east of pc
                        output.append(1, trainer_char);
                        output.append(", ");
                        output.append(std::to_string(npc_rise * -1));
                        output.append(" south and east ");
                        output.append(std::to_string(npc_run * -1));
                    } else { // npc is west of pc
                        output.append(1, trainer_char);
                        output.append(", ");
                        output.append(std::to_string(npc_rise * -1));
                        output.append(" south and west ");
                        output.append(std::to_string(npc_run));
                    }
                } else { // npc is north of pc
                    if (npc_run < 0) { // npc is east of pc
                        output.append(1, trainer_char);
                        output.append(", ");
                        output.append(std::to_string(npc_rise));
                        output.append(" north and east ");
                        output.append(std::to_string(npc_run * -1));
                    } else { // npc is west of pc
                        output.append(1, trainer_char);
                        output.append(", ");
                        output.append(std::to_string(npc_rise));
                        output.append(" north and west ");
                        output.append(std::to_string(npc_run));
                    }
                }

                trainer_info[i] = (char *) malloc(output.size());
                strcpy(trainer_info[i], output.c_str());
                i++;
            }
        }
    }

    for (i = 0; i < num_trainers || i < 10; i++) {
        mvwprintw(trainer_win, i, 0, trainer_info[i]);
    }

    wrefresh(trainer_win);

    shift = 0;
    while((key = getch()) != 27)
    {
        switch (key)
        {
            case KEY_DOWN:
                if (num_trainers > 10 && num_trainers - shift > 10) {
                    shift++;
                    for (i = 0; i < 10 && i + shift < num_trainers; i++) {
                        mvwprintw(trainer_win, i, 0, trainer_info[i + shift]);
                    }
                    wrefresh(trainer_win);
                }
                break;
            case KEY_UP:
                if (shift > 0 && num_trainers > 10) {
                    shift--;
                    for (i = 0; i < 10 && i + shift < num_trainers; i++) {
                        mvwprintw(trainer_win, i, 0, trainer_info[i + shift]);
                    }
                    wrefresh(trainer_win);
                }
                break;
            default:
                continue;
        }
    }

    for (i = 0; i < num_trainers; i++) {
        free(trainer_info[i]);
    }

    destroy_win(trainer_win);
}

int action::battle()
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

char action::move_pc(map *map, pc *pc, int input)
{
    if (map->get_turn_heap()) {}
    char ret_val;
    coordinate_t new_pos;
    int battle_outcome;

    ret_val = 0;

    if (input == '7' || input == 'y') {
        new_pos.y = pc->get_pos().y - 1;
        new_pos.x = pc->get_pos().x - 1;
    } else if (input == '8' || input == 'k') {
        new_pos.y = pc->get_pos().y - 1;
        new_pos.x = pc->get_pos().x    ;
    } else if (input == '9' || input == 'u') {
        new_pos.y = pc->get_pos().y - 1;
        new_pos.x = pc->get_pos().x + 1;
    } else if (input == '6' || input == 'l') {
        new_pos.y = pc->get_pos().y    ;
        new_pos.x = pc->get_pos().x + 1;
    } else if (input == '3' || input == 'n') {
        new_pos.y = pc->get_pos().y + 1;
        new_pos.x = pc->get_pos().x + 1;
    } else if (input == '2' || input == 'j') {
        new_pos.y = pc->get_pos().y + 1;
        new_pos.x = pc->get_pos().x    ;
    } else if (input == '1' || input == 'b') {
        new_pos.y = pc->get_pos().y + 1;
        new_pos.x = pc->get_pos().x - 1;
    } else if (input == '4' || input == 'h') {
        new_pos.y = pc->get_pos().y    ;
        new_pos.x = pc->get_pos().x - 1;
    }

    if (path::calculate_terrain_cost(map->terrain_map[new_pos.y][new_pos.x], pc->get_type()) != INT_MAX) {
        // Determine if in top row of map or right column of map or bottom row of map or left column of map 
        if (map->terrain_map[new_pos.y][new_pos.x] == gate) {
            if (new_pos.y == 0) {
                ret_val = 'n';
            } else if (new_pos.y == MAP_HEIGHT - 1) {
                ret_val = 's';
            } else if (new_pos.x == 0) {
                ret_val = 'w';
            } else if (new_pos.x == MAP_WIDTH - 1) {
                ret_val = 'e';
            }
        }

        if (map->trainer_map[new_pos.y][new_pos.x] != nullptr) {
            if (map->trainer_map[new_pos.y][new_pos.x]->get_next_turn() > -1) {
                battle_outcome = battle();

                if (battle_outcome == 1) { // npc won
                    pc->set_next_turn(-1);
                } else if (battle_outcome == -1) { // npc lost
                    map->trainer_map[new_pos.y][new_pos.x]->set_next_turn(-1);
                }
            }
        } else {
            map->trainer_map[pc->get_pos().y][pc->get_pos().x] = nullptr;
            pc->set_pos(new_pos);
            map->trainer_map[pc->get_pos().y][pc->get_pos().x] = pc;
            map->set_pc_pos(pc->get_pos());
        }

        if (pc->get_next_turn() > -1) {
            pc->set_next_turn(pc->get_next_turn() + path::calculate_terrain_cost(map->terrain_map[pc->get_pos().y][pc->get_pos().x], pc->get_type()));
        }
    }

    return ret_val;
}

void action::move_dijkstra_trainer(heap_t *path_heap, path path_map[MAP_HEIGHT][MAP_WIDTH], map *map, npc *npc)
{
    path *cheapest_path;
    int battle_outcome;

    cheapest_path = &path_map[npc->get_pos().y - 1][npc->get_pos().x - 1];
    battle_outcome = 0;

    path::dijkstra_path(path_heap, map, path_map, npc->get_type());

    if (path_map[npc->get_pos().y - 1][npc->get_pos().x].get_cost() < cheapest_path->get_cost()) {
        cheapest_path = &path_map[npc->get_pos().y - 1][npc->get_pos().x];
    }
    if (path_map[npc->get_pos().y - 1][npc->get_pos().x + 1].get_cost() < cheapest_path->get_cost()) {
        cheapest_path = &path_map[npc->get_pos().y - 1][npc->get_pos().x + 1];
    }
    if (path_map[npc->get_pos().y][npc->get_pos().x - 1].get_cost() < cheapest_path->get_cost()) {
        cheapest_path = &path_map[npc->get_pos().y][npc->get_pos().x - 1];
    }
    if (path_map[npc->get_pos().y][npc->get_pos().x + 1].get_cost() < cheapest_path->get_cost()) {
        cheapest_path = &path_map[npc->get_pos().y][npc->get_pos().x + 1];
    }
    if (path_map[npc->get_pos().y + 1][npc->get_pos().x - 1].get_cost() < cheapest_path->get_cost()) {
        cheapest_path = &path_map[npc->get_pos().y + 1][npc->get_pos().x - 1];
    }
    if (path_map[npc->get_pos().y + 1][npc->get_pos().x].get_cost() < cheapest_path->get_cost()) {
        cheapest_path = &path_map[npc->get_pos().y + 1][npc->get_pos().x];
    }
    if (path_map[npc->get_pos().y + 1][npc->get_pos().x + 1].get_cost() < cheapest_path->get_cost()) {
        cheapest_path = &path_map[npc->get_pos().y + 1][npc->get_pos().x + 1];
    }
    if (cheapest_path->get_cost() == 0) {
        cheapest_path = &path_map[npc->get_pos().y    ][npc->get_pos().x    ];

        battle_outcome = battle();

        if (battle_outcome == 1) { // npc won
            map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x]->set_next_turn(-1);
        } else if (battle_outcome == -1) { // npc lost
            npc->set_next_turn(-1);
        }
    }

    map->trainer_map[npc->get_pos().y][npc->get_pos().x] = nullptr;
    npc->set_pos(cheapest_path->get_coordinate());
    map->trainer_map[npc->get_pos().y][npc->get_pos().x] = npc;
    if (npc->get_next_turn() > -1) {
        npc->set_next_turn(npc->get_next_turn() + cheapest_path->get_terrain_cost());
    }
}

void action::move_wanderer_explorer(map *map, npc *npc)
{
    int move_cost, battle_outcome;
    terrain_e current_terrain;

    current_terrain = map->terrain_map[npc->get_pos().y][npc->get_pos().x];

    // just starting or already going up and need to continue
    if (npc->get_dir().x == 0 && npc->get_dir().y <= 0) {
        move_cost = path::calculate_terrain_cost(map->terrain_map[npc->get_pos().y - 1][npc->get_pos().x], npc->get_type());

        if ((npc->get_type() == wanderer_e && (current_terrain == map->terrain_map[npc->get_pos().y - 1][npc->get_pos().x])) ||
            (npc->get_type() == explorer_e && (move_cost != INT_MAX))) {
            npc->set_dir_y(-1);
            if (map->trainer_map[npc->get_pos().y - 1][npc->get_pos().x    ] == nullptr) {
                map->trainer_map[npc->get_pos().y][npc->get_pos().x] = nullptr;
                npc->set_pos_y(npc->get_pos().y - 1);
                map->trainer_map[npc->get_pos().y][npc->get_pos().x] = npc;
            } else if (map->trainer_map[npc->get_pos().y - 1][npc->get_pos().x    ]->get_type() == pc_e) {
                battle_outcome = battle();

                if (battle_outcome == 1) { // npc won
                    map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x]->set_next_turn(-1);
                    random_turn(map, npc);
                } else if (battle_outcome == -1) { // npc lost
                    npc->set_next_turn(-1);
                }
            } else {
                random_turn(map, npc);
            }
        } else {
            random_turn(map, npc);
        }
        // just starting or already going right and need to continue
    } else if (npc->get_dir().x >= 0 && npc->get_dir().y == 0) {
        move_cost = path::calculate_terrain_cost(map->terrain_map[npc->get_pos().y][npc->get_pos().x + 1], npc->get_type());

        if ((npc->get_type() == wanderer_e && (current_terrain == map->terrain_map[npc->get_pos().y][npc->get_pos().x + 1])) ||
            (npc->get_type() == explorer_e && (move_cost != INT_MAX))) {
            npc->set_dir_x(1);
            if (map->trainer_map[npc->get_pos().y    ][npc->get_pos().x + 1] == nullptr) {
                map->trainer_map[npc->get_pos().y][npc->get_pos().x] = nullptr;
                npc->set_pos_x(npc->get_pos().x + 1);
                map->trainer_map[npc->get_pos().y][npc->get_pos().x] = npc;
            }  else if (map->trainer_map[npc->get_pos().y    ][npc->get_pos().x + 1]->get_type() == pc_e) {
                battle_outcome = battle();

                if (battle_outcome == 1) { // npc won
                    map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x]->set_next_turn(-1);
                    random_turn(map, npc);
                } else if (battle_outcome == -1) { // npc lost
                    npc->set_next_turn(-1);
                }
            } else {
                random_turn(map, npc);
            }
        } else {
            random_turn(map, npc);
        }
        // just starting or already going down and need to continue
    } else if (npc->get_dir().x == 0 && npc->get_dir().y >= 0) {
        move_cost = path::calculate_terrain_cost(map->terrain_map[npc->get_pos().y + 1][npc->get_pos().x], npc->get_type());

        if ((npc->get_type() == wanderer_e && (current_terrain == map->terrain_map[npc->get_pos().y + 1][npc->get_pos().x])) ||
            (npc->get_type() == explorer_e && (move_cost != INT_MAX))) {
            npc->set_dir_y(1);
            if (map->trainer_map[npc->get_pos().y + 1][npc->get_pos().x    ] == nullptr) {
                map->trainer_map[npc->get_pos().y][npc->get_pos().x] = nullptr;
                npc->set_pos_y(npc->get_pos().y + 1);
                map->trainer_map[npc->get_pos().y][npc->get_pos().x] = npc;
            } else if (map->trainer_map[npc->get_pos().y + 1][npc->get_pos().x    ]->get_type() == pc_e) {
                battle_outcome = battle();

                if (battle_outcome == 1) { // npc won
                    map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x]->set_next_turn(-1);
                    random_turn(map, npc);
                } else if (battle_outcome == -1) { // npc lost
                    npc->set_next_turn(-1);
                }
            } else {
                random_turn(map, npc);
            }
        } else {
            random_turn(map, npc);
        }
        // just starting or already going left and need to continue
    } else if (npc->get_dir().x <= 0 && npc->get_dir().y == 0) {
        move_cost = path::calculate_terrain_cost(map->terrain_map[npc->get_pos().y    ][npc->get_pos().x - 1], npc->get_type());

        if ((npc->get_type() == wanderer_e && (current_terrain == map->terrain_map[npc->get_pos().y    ][npc->get_pos().x - 1])) ||
            (npc->get_type() == explorer_e && (move_cost != INT_MAX))) {
            npc->set_dir_x(-1);
            if (map->trainer_map[npc->get_pos().y    ][npc->get_pos().x - 1] == nullptr) {
                map->trainer_map[npc->get_pos().y][npc->get_pos().x] = nullptr;
                npc->set_pos_x(npc->get_pos().x - 1);
                map->trainer_map[npc->get_pos().y][npc->get_pos().x] = npc;
            } else if (map->trainer_map[npc->get_pos().y    ][npc->get_pos().x - 1]->get_type() == pc_e) {
                battle_outcome = battle();

                if (battle_outcome == 1) { // npc won
                    map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x]->set_next_turn(-1);
                    random_turn(map, npc);
                } else if (battle_outcome == -1) { // npc lost
                    npc->set_next_turn(-1);
                }
            } else {
                random_turn(map, npc);
            }
        } else {
            random_turn(map, npc);
        }
    }

    if (npc->get_next_turn() > -1) {
        npc->set_next_turn(npc->get_next_turn() + path::calculate_terrain_cost(map->terrain_map[npc->get_pos().y][npc->get_pos().x], npc->get_type()));
    }
}

void action::move_swimmer(heap_t *path_heap, path path_map[MAP_HEIGHT][MAP_WIDTH], map *map, npc *m, coordinate_t pc_pos)
{
    int battle_outcome;

    // path_map to pc
    if ((map->terrain_map[pc_pos.y - 1][pc_pos.x    ] == water ||
         map->terrain_map[pc_pos.y - 1][pc_pos.x    ] == bridge) ||
        (map->terrain_map[pc_pos.y    ][pc_pos.x + 1] == water ||
         map->terrain_map[pc_pos.y    ][pc_pos.x + 1] == bridge) ||
        (map->terrain_map[pc_pos.y + 1][pc_pos.x    ] == water ||
         map->terrain_map[pc_pos.y + 1][pc_pos.x    ] == bridge) ||
        (map->terrain_map[pc_pos.y    ][pc_pos.x - 1] == water ||
         map->terrain_map[pc_pos.y    ][pc_pos.x - 1] == bridge)) {
        action::move_dijkstra_trainer(path_heap, path_map, map, m);
        // move similar to wanderer_e
    } else {
        // just starting or already going up and need to continue
        if (m->get_dir().x == 0 && m->get_dir().y <= 0) {
            if (map->terrain_map[m->get_pos().y - 1][m->get_pos().x] == water ||
                map->terrain_map[m->get_pos().y - 1][m->get_pos().x] == bridge) {
                m->set_dir_y(-1);
                if (map->trainer_map[m->get_pos().y - 1][m->get_pos().x] == nullptr) {
                    map->trainer_map[m->get_pos().y][m->get_pos().x] = nullptr;
                    m->set_pos_y(m->get_pos().y - 1);
                    map->trainer_map[m->get_pos().y][m->get_pos().x] = m;
                } else if (map->trainer_map[m->get_pos().y - 1][m->get_pos().x]->get_type() == pc_e) {
                    battle_outcome = battle();

                    if (battle_outcome == 1) { // m won
                        map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x]->set_next_turn(-1);
                        random_turn(map, m);
                    } else if (battle_outcome == -1) { // m lost
                        m->set_next_turn(-1);
                    }
                } else {
                    random_turn(map, m);
                }
            } else {
                random_turn(map, m);
            }
            // just starting or already going right and need to continue
        } else if (m->get_dir().x >= 0 && m->get_dir().y == 0) {
            if (map->terrain_map[m->get_pos().y][m->get_pos().x + 1] == water ||
                map->terrain_map[m->get_pos().y][m->get_pos().x + 1] == bridge) {
                m->set_dir_x(1);
                if (map->trainer_map[m->get_pos().y][m->get_pos().x + 1] == nullptr) {
                    map->trainer_map[m->get_pos().y][m->get_pos().x] = nullptr;
                    m->set_pos_x(m->get_pos().x + 1);
                    map->trainer_map[m->get_pos().y][m->get_pos().x] = m;
                } else if (map->trainer_map[m->get_pos().y][m->get_pos().x + 1]->get_type() == pc_e) {
                    battle_outcome = battle();

                    if (battle_outcome == 1) { // m won
                        map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x]->set_next_turn(-1);
                        random_turn(map, m);
                    } else if (battle_outcome == -1) { // m lost
                        m->set_next_turn(-1);
                    }
                } else {
                    random_turn(map, m);
                }
            } else {
                random_turn(map, m);
            }
            // just starting or already going down and need to continue
        } else if (m->get_dir().x == 0 && m->get_dir().y >= 0) {
            if (map->terrain_map[m->get_pos().y + 1][m->get_pos().x] == water ||
                map->terrain_map[m->get_pos().y + 1][m->get_pos().x] == bridge) {
                m->set_dir_y(1);
                if (map->trainer_map[m->get_pos().y + 1][m->get_pos().x] == nullptr) {
                    map->trainer_map[m->get_pos().y][m->get_pos().x] = nullptr;
                    m->set_pos_y(m->get_pos().y + 1);
                    map->trainer_map[m->get_pos().y][m->get_pos().x] = m;
                } else if (map->trainer_map[m->get_pos().y + 1][m->get_pos().x]->get_type() == pc_e) {
                    battle_outcome = battle();

                    if (battle_outcome == 1) { // m won
                        map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x]->set_next_turn(-1);
                        random_turn(map, m);
                    } else if (battle_outcome == -1) { // m lost
                        m->set_next_turn(-1);
                    }
                } else {
                    random_turn(map, m);
                }
            } else {
                random_turn(map, m);
            }
            // just starting or already going left and need to continue
        } else if (m->get_dir().x <= 0 && m->get_dir().y == 0) {
            if (map->terrain_map[m->get_pos().y][m->get_pos().x - 1] == water ||
                map->terrain_map[m->get_pos().y][m->get_pos().x - 1] == bridge) {
                m->set_dir_x(-1);
                if (map->trainer_map[m->get_pos().y][m->get_pos().x - 1] == nullptr) {
                    map->trainer_map[m->get_pos().y][m->get_pos().x] = nullptr;
                    m->set_pos_x(m->get_pos().x - 1);
                    map->trainer_map[m->get_pos().y][m->get_pos().x] = m;
                } else if (map->trainer_map[m->get_pos().y][m->get_pos().x - 1]->get_type() == pc_e) {
                    battle_outcome = battle();

                    if (battle_outcome == 1) { // m won
                        map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x]->set_next_turn(-1);
                        random_turn(map, m);
                    } else if (battle_outcome == -1) { // m lost
                        m->set_next_turn(-1);
                    }
                } else {
                    random_turn(map, m);
                }
            } else {
                random_turn(map, m);
            }
        }

        if (m->get_next_turn() > -1) {
            m->set_next_turn(m->get_next_turn() + path::calculate_terrain_cost(map->terrain_map[m->get_pos().y][m->get_pos().x], m->get_type()));
        }
    }
}

void action::move_pacer(map *map, npc *p)
{
    terrain_e current_terrain;
    int battle_outcome;
    int moved;

    current_terrain = map->terrain_map[p->get_pos().y][p->get_pos().x];
    moved = 0;
    // Just starting or moving right and the right is open
    if (p->get_dir().x >= 0 && map->terrain_map[p->get_pos().y][p->get_pos().x + 1] == current_terrain) {
        if (map->trainer_map[p->get_pos().y    ][p->get_pos().x + 1] == nullptr) {
            moved = 1;
            p->set_dir_x(1);
            map->trainer_map[p->get_pos().y][p->get_pos().x] = nullptr;
            p->set_pos_x(p->get_pos().x + 1);
            map->trainer_map[p->get_pos().y][p->get_pos().x] = p;
        } else if (map->trainer_map[p->get_pos().y    ][p->get_pos().x + 1]->get_type() == pc_e) {
            battle_outcome = battle();

            if (battle_outcome == 1) { // npc won
                map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x]->set_next_turn(-1);
            } else if (battle_outcome == -1) { // npc lost
                p->set_next_turn(-1);
            }
        }
        // Just starting or moving left and checking if left is open
    } else if (p->get_dir().x <= 0 && map->terrain_map[p->get_pos().y][p->get_pos().x - 1] == current_terrain) {
        if (map->trainer_map[p->get_pos().y    ][p->get_pos().x - 1] == nullptr) {
            moved = 1;
            p->set_dir_x(-1);
            map->trainer_map[p->get_pos().y][p->get_pos().x] = nullptr;
            p->set_pos_x(p->get_pos().x - 1);
            map->trainer_map[p->get_pos().y][p->get_pos().x] = p;
        } else if (map->trainer_map[p->get_pos().y    ][p->get_pos().x - 1]->get_type() == pc_e) {
            battle_outcome = battle();

            if (battle_outcome == 1) { // npc won
                map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x]->set_next_turn(-1);
            } else if (battle_outcome == -1) { // npc lost
                p->set_next_turn(-1);
            }
        }
    }

    // moving left but need to go back right
    if (!moved && p->get_dir().x == -1 && map->terrain_map[p->get_pos().y][p->get_pos().x + 1] == current_terrain) {
        p->set_dir_x(1);
        if (map->trainer_map[p->get_pos().y    ][p->get_pos().x + 1] == nullptr) {
            moved = 1;
            map->trainer_map[p->get_pos().y][p->get_pos().x] = nullptr;
            p->set_pos_x(p->get_pos().x + 1);
            map->trainer_map[p->get_pos().y][p->get_pos().x] = p;
        } else if (map->trainer_map[p->get_pos().y    ][p->get_pos().x + 1]->get_type() == pc_e) {
            battle_outcome = battle();

            if (battle_outcome == 1) { // npc won
                map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x]->set_next_turn(-1);
            } else if (battle_outcome == -1) { // npc lost
                p->set_next_turn(-1);
            }
        }
    }

    // moving right but need to go back left
    if (!moved && p->get_dir().x == 1 && map->terrain_map[p->get_pos().y][p->get_pos().x - 1] == current_terrain) { // go back left
        p->set_dir_x(-1);
        if (map->trainer_map[p->get_pos().y    ][p->get_pos().x - 1] == nullptr) {
            moved = 1;
            map->trainer_map[p->get_pos().y][p->get_pos().x] = nullptr;
            p->set_pos_x(p->get_pos().x - 1);
            map->trainer_map[p->get_pos().y][p->get_pos().x] = p;
        } else if (map->trainer_map[p->get_pos().y    ][p->get_pos().x - 1]->get_type() == pc_e) {
            battle_outcome = battle();

            if (battle_outcome == 1) { // npc won
                map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x]->set_next_turn(-1);
            } else if (battle_outcome == -1) { // npc lost
                p->set_next_turn(-1);
            }
        } else {
            if (map->trainer_map[p->get_pos().y    ][p->get_pos().x + 1] == nullptr) {
                p->set_dir_x(1);
                moved = 1;
                map->trainer_map[p->get_pos().y][p->get_pos().x] = nullptr;
                p->set_pos_x(p->get_pos().x + 1);
                map->trainer_map[p->get_pos().y][p->get_pos().x] = p;
            } else if (map->trainer_map[p->get_pos().y    ][p->get_pos().x + 1]->get_type() == pc_e) {
                battle_outcome = battle();

                if (battle_outcome == 1) { // npc won
                    map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x]->set_next_turn(-1);
                } else if (battle_outcome == -1) { // npc lost
                    p->set_next_turn(-1);
                }
            }
        }
    }

    if (p->get_next_turn() > -1) {
        p->set_next_turn(p->get_next_turn() + path::calculate_terrain_cost(map->terrain_map[p->get_pos().y][p->get_pos().x], p->get_type()));
    }
}
