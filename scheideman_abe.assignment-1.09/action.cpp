#include <ncurses.h>
#include <string>
#include <cstring>

#include "action.h"

int check_random_turn(int random_direction, map *map, coordinate_t pos, trainer_type_e type) {
    int check, terrain_cost;

    check = 0;

    if (random_direction == 0) { // check up
        if (type == wanderer_e || type == swimmer_e) {
            if (map->terrain_map[pos.y][pos.x] == map->terrain_map[pos.y - 1][pos.x] && map->trainer_map[pos.y - 1][pos.x] == nullptr) {
                check = 1;
            }
        } else if (type == explorer_e) {
            terrain_cost = path::calculate_terrain_cost(map->terrain_map[pos.y - 1][pos.x], type);
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
            terrain_cost = path::calculate_terrain_cost(map->terrain_map[pos.y][pos.x + 1], type);
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
            terrain_cost = path::calculate_terrain_cost(map->terrain_map[pos.y + 1][pos.x], type);
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

void random_turn(map *map, trainer *t, Data *data) {
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
            battle_outcome = battle((pc *)map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x], (npc *)t, data);

            if (battle_outcome == 1) { // npc won
                random_turn(map, t, data);
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
            battle_outcome = battle((pc *)map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x], (npc *)t, data);

            if (battle_outcome == 1) { // npc won
                random_turn(map, t, data);
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
            battle_outcome = battle((pc *)map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x], (npc *)t, data);

            if (battle_outcome == 1) { // npc won
                
                random_turn(map, t, data);
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
            battle_outcome = battle((pc *)map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x], (npc *)t, data);

            if (battle_outcome == 1) { // npc won
                random_turn(map, t, data);
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

Pokemon *choose_starter(Pokemon *first, Pokemon *second, Pokemon *third)
{
    int key;
    int chosen = 0;
    Pokemon *choice;
    mvprintw(0, 1, "Choose your starter Pokemon. Press 1, 2, or 3 to select.");
    mvprintw(1, 1, "1. %s", first->get_name().c_str());
    mvprintw(2, 1, "2. %s", second->get_name().c_str());
    mvprintw(3, 1, "3. %s", third->get_name().c_str());
    refresh();

    while(!chosen)
    {
        key = getch();

        switch(key)
        {
            case '1':
                choice = first;
                chosen = 1;
                break;
            case '2':
                choice = second;
                chosen = 1;
                break;
            case '3':
                choice = third;
                chosen = 1;
                break;
            default:
                mvprintw(0, 1, "Choose your starter Pokemon. Press 1, 2, or 3 to select.");
                mvprintw(1, 1, "1. %s", first->get_name().c_str());
                mvprintw(2, 1, "2. %s", second->get_name().c_str());
                mvprintw(3, 1, "3. %s", third->get_name().c_str());
                refresh();
                break;
        }
    }

    return choice;
}

void enter_building(map *map, pc *pc)
{
    int key, i;
    if (map->terrain_map[pc->get_pos().y][pc->get_pos().x] == pokecenter) {
        for (i = 0; i < 6; i++) {
            if (pc->get_pokemon(i) != 0) {
                pc->get_pokemon(i)->set_hp(pc->get_pokemon(i)->get_max_hp());
            }
        }

        clear();
        mvprintw(0, 1, "You are in a Pokemon Center!"); //TODO heal pokemon
        mvprintw(1, 1, "Your pokemon have been restored to full health. Your Press '<' to exit."); 
        refresh();

        while((key = getch()) != '<')
        {
            mvprintw(0, 1, "You are in a Pokemon Center!"); //TODO heal pokemon
            mvprintw(1, 1, "Your pokemon have been restored to full health. Your Press '<' to exit.");
            refresh();
        }

        pc->set_next_turn(pc->get_next_turn() + 10);
    } else if (map->terrain_map[pc->get_pos().y][pc->get_pos().x] == pokemart) {
        clear();
        pc->restore_bag();
        mvprintw(0, 1, "You are in a PokeMart!");
        mvprintw(1, 1, "Your supplies are full now. Press '<' to exit.");
        refresh();

        while((key = getch()) != '<')
        {
            mvprintw(0, 1, "You are in a PokeMart!");
            mvprintw(1, 1, "Your supplies are full now. Press '<' to exit.");
            refresh();
        }

        pc->set_next_turn(pc->get_next_turn() + 10);
    }
}

void trainer_info(map *map, int num_trainers)
{
    int x, y, npc_rise, npc_run, key, i, shift;
    char trainer_char;
    std::string output;
    char *trainer_info[num_trainers];
    WINDOW *trainer_win;
    trainer_win = create_newwin(10, 24, 1, 0);
    keypad(trainer_win, TRUE);

    i = 0;
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

int get_damage(Pokemon *attacker, Pokemon *defender, Data *data, int move)
{
    int level, power, attack, defense;
    double critical;
    double random;
    double stab; 
    int type;

    level = attacker->get_level();
    power = data->moves[attacker->get_move(move) - 1].power;
    if (power == INT_MAX) {
        return 0;
    }
    attack = attacker->get_attack();
    defense = defender->get_defense();
    critical = ((attacker->get_base_speed() / 2) > (rand() % 256)) ? 1.5 : 1;
    random = ((rand() % 16) + 85.0) / 100.0;
    stab = (attacker->get_type(0) == data->moves[attacker->get_move(move) - 1].type_id ||
            attacker->get_type(1) == data->moves[attacker->get_move(move) - 1].type_id) ? 1.5 : 1;
    type = 1;

    return (((2 * level / 5) + 2) * power * attack / defense / 50 + 2) * critical * random * stab * type;
}

void attack(Pokemon *first_attacker, Pokemon *second_attacker, Data *data, int first_move, int second_move)
{
    int move_hits;
    
    clear();

    move_hits = (data->moves[first_attacker->get_move(first_move) - 1].accuracy > (rand() % 100)) ? 1 : 0;

    if (move_hits) {
        second_attacker->damage_hp(get_damage(first_attacker, second_attacker, data, first_move));
        mvprintw(0, 0, "%s used %s!", first_attacker->get_name().c_str(), data->moves[first_attacker->get_move(first_move) - 1].identifier.c_str());

        if (second_attacker->get_hp() > 0 && second_move > -1) {
            move_hits = (data->moves[second_attacker->get_move(second_move) - 1].accuracy > (rand() % 100)) ? 1 : 0;

            if(move_hits) {
                first_attacker->damage_hp(get_damage(second_attacker, first_attacker, data, second_move));
                mvprintw(0, 40, "%s used %s!", second_attacker->get_name().c_str(), data->moves[second_attacker->get_move(second_move) - 1].identifier.c_str());
            }
        }
    } else if (second_move > -1) { // opponent attacks if allowed to
        move_hits = (data->moves[second_attacker->get_move(second_move) - 1].accuracy > (rand() % 100)) ? 1 : 0;

        if(move_hits) {
            first_attacker->damage_hp(get_damage(second_attacker, first_attacker, data, second_move));
            mvprintw(0, 40, "%s used %s!", second_attacker->get_name().c_str(), data->moves[second_attacker->get_move(second_move) - 1].identifier.c_str());
        }
    }

    refresh();
}

int fight(Pokemon *pc_pokemon, Pokemon *opponent, Data *data)
{
    int op_knockout = 0;
    int knockout = 0;
    int move_complete = 0;
    int key, move_selected;
    int pc_move, op_move;
    int pc_priority, op_priority;
    int pc_speed, op_speed;
    WINDOW *move_win;
    move_win = create_newwin(5, 40, 1, 0);
    keypad(move_win, TRUE);

    mvwprintw(move_win, 1, 1, "Select a move. Press '<' to exit.");
    mvwprintw(move_win, 2, 1, "1: %s", data->moves[pc_pokemon->get_move(0) - 1].identifier.c_str()); // subtract one because the id is 1 off of data vector index for corresponding move
    if (pc_pokemon->get_move(1) != 0) {
        mvwprintw(move_win, 3, 1, "2: %s", data->moves[pc_pokemon->get_move(1) - 1].identifier.c_str());
    }
    wrefresh(move_win);

    while(!knockout && !op_knockout && !move_complete) {
        move_selected = 0;
        while (!move_selected) {
            key = getch();

            if (key == '1') {
                move_selected = 1;
                pc_priority = data->moves[pc_pokemon->get_move(0) - 1].priority;
                pc_move = 0;
            } else if (key == '2' && pc_pokemon->get_move(1) != 0) {
                move_selected = 1;
                pc_priority = data->moves[pc_pokemon->get_move(1) - 1].priority;
                pc_move = 1;
            } else if (key == '<') {
                break;
            } else {
                mvwprintw(move_win, 1, 1, "Select a move. Press '<' to exit.");
                mvwprintw(move_win, 2, 1, "1: %s", data->moves[pc_pokemon->get_move(0) - 1].identifier.c_str()); // subtract one because the id is 1 off of data vector index for corresponding move
                if (pc_pokemon->get_move(1) != 0) {
                    mvwprintw(move_win, 3, 1, "2: %s", data->moves[pc_pokemon->get_move(1) - 1].identifier.c_str());
                }
                wrefresh(move_win);
            }
        }
        
        if (!move_selected) {
            break;
        }

        if (opponent->get_move(1) == 0) {
            op_move = 0;
        } else {
            op_move = rand() % 2;
        }
        op_priority = data->moves[opponent->get_move(op_move) - 1].priority;

        if (pc_priority > op_priority) {
            attack(pc_pokemon, opponent, data, pc_move, op_move);
        } else if (pc_priority < op_priority) {
            attack(opponent, pc_pokemon, data, op_move, pc_move);
        } else {
            pc_speed = pc_pokemon->get_speed();
            op_speed = opponent->get_speed();
            if (pc_speed > op_speed) {
                attack(pc_pokemon, opponent, data, pc_move, op_move);
            } else if (pc_speed < op_speed) {
                attack(opponent, pc_pokemon, data, op_move, pc_move);
            } else {
                if (rand() % 2) {
                    attack(pc_pokemon, opponent, data, pc_move, op_move);
                } else {
                    attack(opponent, pc_pokemon, data, op_move, pc_move);
                }
            }
        }

        if (pc_pokemon->get_hp() <= 0) {
            knockout = 1;
        } else if (opponent->get_hp() <= 0) {
            op_knockout = 1;
            knockout = -1;
        }

        move_complete = 1;
    }

    wclear(move_win);
    destroy_win(move_win);
    return knockout;
}

int bag(pc *pc, int trainer_battle, int wild_encounter)
{
    int pokeball = 0;
    int key, item_selected, poke_selected;
    int i, row, pokemon;

    clear();
    mvprintw(0, 1, "Select an item using the number keys. Press '<' to exit.");
    mvprintw(1, 1, "1. Potion (%d)", pc->get_bag().get_potions());
    if (wild_encounter) {
        mvprintw(2, 1, "2. Pokeball (%d)", pc->get_bag().get_pokeballs());
    } else if (!trainer_battle) {
        mvprintw(2, 1, "2. Revive (%d)", pc->get_bag().get_revives());
        mvprintw(3, 1, "3. Pokeball (%d)", pc->get_bag().get_pokeballs());
    }

    refresh();

    item_selected = 0;
    while(!item_selected) {
        key = getch();

        if (key == '1' && pc->get_bag().get_potions() > 0) {
            mvprintw(1, 15, "*");
            item_selected = 1;
        } else if ((key == '2' && !trainer_battle) && 
                   ((wild_encounter && pc->get_bag().get_pokeballs() > 0) || 
                    (!wild_encounter && pc->get_bag().get_revives() > 0))) {
            mvprintw(2, 15, "*");
            item_selected = 2;
        } else if (key == '<') { 
            break;
        } else {
            mvprintw(0, 1, "Select an item using the number keys. Press '<' to exit.");
            mvprintw(1, 1, "1. Potion (%d)", pc->get_bag().get_potions());
            if (wild_encounter) {
                mvprintw(2, 1, "2. Pokeball (%d)", pc->get_bag().get_pokeballs());
            } else if (!trainer_battle) {
                mvprintw(2, 1, "2. Revive (%d)", pc->get_bag().get_revives());
                mvprintw(3, 1, "3. Pokeball (%d)", pc->get_bag().get_pokeballs());
            }

            refresh();
            item_selected = 0;
            continue;
        }

        if (item_selected == 2 && wild_encounter) { // if it's a pokeball, done here
            pokeball = pc->use_pokeball();
            break;
        }

        refresh();

        poke_selected = 0;
        while (!poke_selected) {
            mvprintw(4, 1, "Type the number to select which Pokemon to use the item on.");
            mvprintw(5, 1, "Press '<' to reselect item.");
            row = 6;
            pokemon = 0;
            for (i = 0; i < 6; i++) {
                if (!pc->get_pokemon(i)) {
                    break;
                }
                mvprintw(row, 1, "%d. %s: HP: %d", pokemon + 1, pc->get_pokemon(i)->get_name().c_str(), pc->get_pokemon(i)->get_hp());
                row++;
                pokemon++;
            }
            refresh();

            key = getch();

            if (key == '1') { // at this point only option is potion or if not in a battle (ensured because item_selected can't be 2 if in a trainer battle), revive
                if (item_selected == 1) {
                    pc->use_potion(pc->get_pokemon(0));
                    pokeball = -1; // means didn't use a pokeball so nothing else outside of this function needs to be done
                } else {
                    pc->use_revive(pc->get_pokemon(0));
                }
                poke_selected = 1;
            } else if (key == '2' && pokemon >= 2) {
                if (item_selected == 1) {
                    pc->use_potion(pc->get_pokemon(1));
                    pokeball = -1;
                } else {
                    pc->use_revive(pc->get_pokemon(1));
                }
                poke_selected = 1;
            } else if (key == '3' && pokemon >= 3) {
                if (item_selected == 1) {
                    pc->use_potion(pc->get_pokemon(2));
                    pokeball = -1;
                } else {
                    pc->use_revive(pc->get_pokemon(2));
                }
                poke_selected = 1;
            } else if (key == '4' && pokemon >= 4) {
                if (item_selected == 1) {
                    pc->use_potion(pc->get_pokemon(3));
                    pokeball = -1;
                } else {
                    pc->use_revive(pc->get_pokemon(3));
                }
                poke_selected = 1;
            } else if (key == '5' && pokemon >= 5) {
                if (item_selected == 1) {
                    pc->use_potion(pc->get_pokemon(4));
                    pokeball = -1;
                } else {
                    pc->use_revive(pc->get_pokemon(4));
                }
                poke_selected = 1;
            } else if (key == '6' && pokemon >= 6) {
                if (item_selected == 1) {
                    pc->use_potion(pc->get_pokemon(5));
                    pokeball = -1;
                } else {
                    pc->use_revive(pc->get_pokemon(5));
                }
                poke_selected = 1;
            } else if (key == '<') {
                clear();
                mvprintw(0, 1, "Select an item using the number keys. Press '<' to exit.");
                mvprintw(1, 1, "1. Potion (%d)", pc->get_bag().get_potions());
                if (wild_encounter) {
                    mvprintw(2, 1, "2. Pokeball (%d)", pc->get_bag().get_pokeballs());
                } else if (!trainer_battle) {
                    mvprintw(2, 1, "2. Revive (%d)", pc->get_bag().get_revives());
                    mvprintw(3, 1, "3. Pokeball (%d)", pc->get_bag().get_pokeballs());
                }

                refresh();
                item_selected = 0;
                break;
            } else {
                mvprintw(4, 1, "Type the number to select which Pokemon to use the item on.");
                mvprintw(5, 1, "Press '<' to reselect item.");
                row = 6;
                pokemon = 0;
                for (i = 0; i < 6; i++) {
                    if (!pc->get_pokemon(i)) {
                        break;
                    }
                    mvprintw(row, 1, "%d. %s: HP: %d", pokemon + 1, pc->get_pokemon(i)->get_name().c_str(), pc->get_pokemon(i)->get_hp());
                    row++;
                    pokemon++;
                }
                poke_selected = 0;

                refresh();
            }
        }
        
        refresh();
    }

    clear();
    refresh();
    return pokeball;
}

int switch_pokemon(pc *pc)
{
    int key;
    int i, count, row, pokemon_selected;
    
    clear();
    mvprintw(1, 1, "Select which Pokemon to use. Press '<' to exit.");
    row = 3;
    count = 0;
    for (i = 0; i < 6; i++) {
        if (pc->get_pokemon(i) == 0) {
            if (count == 0) {
                mvprintw(2, 1, "All your Pokemon are fainted. Press '<' to exit.");
                refresh();
                while ((key = getch()) != '<') {
                    mvprintw(2, 1, "All your Pokemon are fainted. Press '<' to exit.");
                }
                return -1;
            }
            break;
        } else if (pc->get_pokemon(i)->get_hp() == 0) {
            if (i == 5 && count == 0) { // cycled through all pokemon and none have health.
                mvprintw(2, 1, "All your Pokemon are fainted. Press '<' to exit.");
                refresh();
                while ((key = getch()) != '<') {
                    mvprintw(2, 1, "All your Pokemon are fainted. Press '<' to exit.");
                }
                return -1;
            }
        }
        mvprintw(row, 1, "%d. %s: Level: %d HP: %d", ++count, 
                                                     pc->get_pokemon(i)->get_name().c_str(), 
                                                     pc->get_pokemon(i)->get_level(),
                                                     pc->get_pokemon(i)->get_hp());
        row++;
    }
    refresh();

    pokemon_selected = 0;
    while(!pokemon_selected)
    {
        key = getch();

        if (((key == '1' && count >= 1) && pc->get_pokemon(0)->get_hp() > 0) ||
            ((key == '2' && count >= 2) && pc->get_pokemon(1)->get_hp() > 0) ||
            ((key == '3' && count >= 3) && pc->get_pokemon(2)->get_hp() > 0) ||
            ((key == '4' && count >= 4) && pc->get_pokemon(3)->get_hp() > 0) ||
            ((key == '5' && count >= 5) && pc->get_pokemon(4)->get_hp() > 0) ||
            ((key == '6' && count >= 6) && pc->get_pokemon(5)->get_hp() > 0)) {
            pokemon_selected = key - '0';
        } else if (key == '<') {
            clear();
            refresh();
            pokemon_selected = 0;
            break;
        } else {
            mvprintw(1, 1, "Select which Pokemon to use. Press '<' to exit.");
            row = 3;
            count = 0;
            for (i = 0; i < 6; i++) {
                if (!pc->get_pokemon(i)) {
                    break;
                } else if (pc->get_pokemon(i)->get_hp() == 0) {
                    continue;
                }
                mvprintw(row, 1, "%d. %s: Level: %d HP: %d", ++count, 
                                                             pc->get_pokemon(i)->get_name().c_str(), 
                                                             pc->get_pokemon(i)->get_level(),
                                                             pc->get_pokemon(i)->get_hp());
                row++;
            }

            refresh();
        }
    }

    clear();
    refresh();
    return pokemon_selected;
}

int battle(pc *pc, npc *npc, Data *data)
{
    int key, i, row;
    int chosen_pokemon, npc_pokemon;
    int done = 0;
    int poke_selected;
    int potion = 0;
    int outcome; // 1 = pc lost, -1 = pc won

    clear();
    row = 2;
    for (i = 0; i < 6; i++) {
        if (!pc->get_pokemon(i)) {
            break;
        }
        mvprintw(1, 1, "You are in a battle!");
        mvprintw(row, 1, "%s: Level: %d HP: %d", pc->get_pokemon(i)->get_name().c_str(), 
                                                 pc->get_pokemon(i)->get_level(),
                                                 pc->get_pokemon(i)->get_hp());
        if (pc->get_pokemon(i)->get_move(1) != 0) {
            mvprintw(row + 1, 1, "Moves: %s, %s", data->moves[pc->get_pokemon(i)->get_move(0) - 1].identifier.c_str(), data->moves[pc->get_pokemon(i)->get_move(1) - 1].identifier.c_str()); // subtract one because the id is 1 off of data vector index for corresponding move
        } else {
            mvprintw(row + 1, 1, "Moves: %s", data->moves[pc->get_pokemon(i)->get_move(0) - 1].identifier.c_str());
        }
        row = row + 2;
    }
    row = 2;
    for (i = 0; i < 6; i++) {
        if (!npc->get_pokemon(i)) {
            break;
        }
        mvprintw(row, 40, "%s: Level: %d HP: %d", npc->get_pokemon(i)->get_name().c_str(), 
                                                  npc->get_pokemon(i)->get_level(),
                                                  npc->get_pokemon(i)->get_hp());
        if (npc->get_pokemon(i)->get_move(1) != 0) {
            mvprintw(row + 1, 40, "Moves: %s, %s", data->moves[npc->get_pokemon(i)->get_move(0) - 1].identifier.c_str(), data->moves[npc->get_pokemon(i)->get_move(1) - 1].identifier.c_str()); // subtract one because the id is 1 off of data vector index for corresponding move
        } else {
            mvprintw(row + 1, 40, "Moves: %s", data->moves[npc->get_pokemon(i)->get_move(0) - 1].identifier.c_str());
        }
        row = row + 2;
    }

    mvprintw(20, 1, "1. Fight");
    mvprintw(21, 1, "2. Bag");
    mvprintw(22, 1, "3. Pokemon");
    refresh();

    chosen_pokemon = -1;
    for (i = 0; i < 6; i++) {
        if (pc->get_pokemon(i)) {
            chosen_pokemon = i;
            break;
        }
    }

    if (chosen_pokemon == -1) {
        mvprintw(19, 1, "You have no Pokemon to fight with! Press < to flee.");
        while((key = getch()) != '<') {
            mvprintw(19, 1, "You have no Pokemon to fight with! Press < to flee.");
            refresh();
        }
        outcome = 0;
        done = 1;
    }

    npc_pokemon = 0;

    while(!done)
    {
        key = getch();

        switch (key) {
            case '1':
                    fight(pc->get_pokemon(chosen_pokemon), npc->get_pokemon(npc_pokemon), data);
                    
                    if (npc->get_pokemon(npc_pokemon)->get_hp() <= 0 && npc->get_pokemon(npc_pokemon + 1) != 0) { // pc won fight but opponent has more pokemon
                        npc_pokemon++;
                    } else if (npc->get_pokemon(npc_pokemon)->get_hp() <= 0) { // pc won battle
                        done = 1;
                        outcome = -1;
                        continue;
                    } // if none of those, then exited fight screen without picking or did a move that didn't knock out opponent

                break;
            case '2':
                potion = bag(pc, 1, 0);
                if (potion == -1) { // potion used so opponent attacks
                    if (npc->get_pokemon(npc_pokemon)->get_move(1) == 0) {
                        attack(npc->get_pokemon(npc_pokemon), pc->get_pokemon(chosen_pokemon), data, 0, -1);
                    } else {
                        attack(npc->get_pokemon(npc_pokemon), pc->get_pokemon(chosen_pokemon), data, rand() % 2, -1);
                    }
                }
                
                break;
            case '3':
                poke_selected = switch_pokemon(pc);
                if (poke_selected > 0 && poke_selected < 7) {
                    if (chosen_pokemon != poke_selected - 1) {
                        chosen_pokemon = poke_selected - 1;
                        
                        if (npc->get_pokemon(npc_pokemon)->get_move(1) == 0) {
                            attack(npc->get_pokemon(npc_pokemon), pc->get_pokemon(chosen_pokemon), data, 0, -1);
                        } else {
                            attack(npc->get_pokemon(npc_pokemon), pc->get_pokemon(chosen_pokemon), data, rand() % 2, -1);
                        }
                    }
                } else if (poke_selected == -1){
                    chosen_pokemon = -1;
                    mvprintw(19, 1, "You have no Pokemon to fight with! Press < to flee.");
                    while((key = getch()) != '<') {
                        mvprintw(19, 1, "You have no Pokemon to fight with! Press < to flee.");
                        refresh();
                    }

                    done = 1;
                    continue;
                } 

                break;
            default:
                break;
        }

        if (pc->get_pokemon(chosen_pokemon)->get_hp() == 0) {
            poke_selected = switch_pokemon(pc);
            if (poke_selected > 0 && poke_selected < 7) {
                chosen_pokemon = poke_selected - 1;
            } else {
                chosen_pokemon = -1;
                mvprintw(19, 1, "You have no Pokemon to fight with! Press < to flee.");
                while((key = getch()) != '<') {
                    mvprintw(19, 1, "You have no Pokemon to fight with! Press < to flee.");
                    refresh();
                }

                done = 1;
                outcome = 1; // npc won
                continue;
            }
        } else {
            for (i = 0; i < 6; i++) {
                if (npc->get_pokemon(i) != 0) {
                    if (npc->get_pokemon(i)->get_hp() > 0) {
                        continue;
                    } else if (i == 5) {
                        done = 1;
                        outcome = -1; // pc won
                    }
                    break;
                }
            }
        }

        row = 2;
        for (i = 0; i < 6; i++) {
            if (!pc->get_pokemon(i)) {
                break;
            }
            mvprintw(1, 1, "You are in a battle!");
            mvprintw(row, 1, "%s: Level: %d HP: %d", pc->get_pokemon(i)->get_name().c_str(), 
                                                     pc->get_pokemon(i)->get_level(),
                                                     pc->get_pokemon(i)->get_hp());
            if (pc->get_pokemon(i)->get_move(1) != 0) {
                mvprintw(row + 1, 1, "Moves: %s, %s", data->moves[pc->get_pokemon(i)->get_move(0) - 1].identifier.c_str(), data->moves[pc->get_pokemon(i)->get_move(1) - 1].identifier.c_str()); // subtract one because the id is 1 off of data vector index for corresponding move
            } else {
                mvprintw(row + 1, 1, "Moves: %s", data->moves[pc->get_pokemon(i)->get_move(0) - 1].identifier.c_str());
            }
            row = row + 2;
        }
        row = 2;
        for (i = 0; i < 6; i++) {
            if (!npc->get_pokemon(i)) {
                break;
            }
            mvprintw(row, 40, "%s: Level: %d HP: %d", npc->get_pokemon(i)->get_name().c_str(), 
                                                      npc->get_pokemon(i)->get_level(),
                                                      npc->get_pokemon(i)->get_hp());
            if (npc->get_pokemon(i)->get_move(1) != 0) {
                mvprintw(row + 1, 40, "Moves: %s, %s", data->moves[npc->get_pokemon(i)->get_move(0) - 1].identifier.c_str(), data->moves[npc->get_pokemon(i)->get_move(1) - 1].identifier.c_str()); // subtract one because the id is 1 off of data vector index for corresponding move
            } else {
                mvprintw(row + 1, 40, "Moves: %s", data->moves[npc->get_pokemon(i)->get_move(0) - 1].identifier.c_str());
            }
            row = row + 2;
        }

        mvprintw(20, 1, "1. Fight");
        mvprintw(21, 1, "2. Bag");
        mvprintw(22, 1, "3. Pokemon");
        refresh();
    }

    return outcome;
}

/*
    When encountering pokemon, we’ll add a placeholder function for the battle and capture sequence in
    which we’ll print the pokemon’s level, stats, and moves and pause for input (escape, any key, etc.).
*/
Pokemon *encounter(pc *pc, Data *data, int manhattan_distance) 
{
    int key, i;
    int knockout;
    int done = 0;
    int poke_selected = 0;
    int pokeball;
    int chosen_pokemon;
    int level;
    Pokemon *pokemon;
    level = calculate_level(manhattan_distance);
    pokemon = (Pokemon *)generate_pokemon(data, 
                                          rand() % 1092,
                                          rand() % 2,
                                          level,
                                          rand() % 8192 == 0 ? 1 : 0,
                                          rand() % 16,
                                          rand() % 16,
                                          rand() % 16,
                                          rand() % 16,
                                          rand() % 16,
                                          rand() % 16);

    clear();
    mvprintw(1, 1, "You encountered a Pokemon!");
    mvprintw(2, 1, "%s:", pokemon->get_name().c_str());
    mvprintw(3, 1, "Gender: %d", pokemon->get_gender());
    mvprintw(4, 1, "Is Shiny: %d", pokemon->get_is_shiny());
    mvprintw(5, 1, "Level: %d", pokemon->get_level());
    mvprintw(6, 1, "HP: %d", pokemon->get_hp());
    mvprintw(7, 1, "Attack: %d", pokemon->get_attack());
    mvprintw(8, 1, "Defense: %d", pokemon->get_defense());
    mvprintw(9, 1, "Special Attack: %d", pokemon->get_special_attack());
    mvprintw(10, 1, "Special Defense: %d", pokemon->get_special_defense());
    mvprintw(11, 1, "Speed: %d", pokemon->get_speed());
    mvprintw(20, 1, "1. Fight");
    mvprintw(21, 1, "2. Bag");
    mvprintw(22, 1, "3. Run");
    mvprintw(23, 1, "4. Pokemon");
    if (pokemon->get_move(1) != 0) {
        mvprintw(12, 1, "Moves: %s, %s", data->moves[pokemon->get_move(0) - 1].identifier.c_str(), data->moves[pokemon->get_move(1) - 1].identifier.c_str()); // subtract one because the id is 1 off of data vector index for corresponding move
    } else {
        mvprintw(12, 1, "Moves: %s", data->moves[pokemon->get_move(0) - 1].identifier.c_str());
    }
    refresh();

    chosen_pokemon = -1;
    for (i = 0; i < 6; i++) {
        if (pc->get_pokemon(i)) {
            if (pc->get_pokemon(i)->get_hp() > 0) {
                chosen_pokemon = i;
                break;
            }
        }
    }

    if (chosen_pokemon == -1) {
        mvprintw(19, 1, "You have no Pokemon to fight with! Press < to flee.");
        while((key = getch()) != '<') {
            mvprintw(19, 1, "You have no Pokemon to fight with! Press < to flee.");
            refresh();
        }

        done = 1;
    }

    while(!done)
    {
        key = getch();

        if (rand() % 10 < 1) { // flee
            mvprintw(19, 1, "The Pokemon fled! Press any key to continue.");
            key = getch();

            done = 1;
            continue;
        }

        switch (key) {
            case '1':
                knockout = fight(pc->get_pokemon(chosen_pokemon), pokemon, data);

                break;
            case '2':
                pokeball = bag(pc, 0, 1);
                if (pokeball == 1) {
                    for (i = 0; i < 6; i++) {
                        if (pc->get_pokemon(i) == 0) {
                            pc->set_pokemon(i, pokemon);
                            done = 1;
                            break;
                        } else if (i == 5) {
                            // pokemon escaped
                            done = 1;
                        }
                    }
                } else if (pokeball == -1) { // potion used so opponent attacks
                    if (pokemon->get_move(1) == 0) {
                        attack(pokemon, pc->get_pokemon(chosen_pokemon), data, 0, -1);
                    } else {
                        attack(pokemon, pc->get_pokemon(chosen_pokemon), data, rand() % 2, -1);
                    }
                }

                break;
            case '3':
                done = (rand() % 3 > 1) ? 1 : 0; // fail to flee 1 out of 3 times
                if (!done) { // enemy attacks
                    if (pokemon->get_move(1) == 0) {
                        attack(pokemon, pc->get_pokemon(chosen_pokemon), data, 0, -1);
                    } else {
                        attack(pokemon, pc->get_pokemon(chosen_pokemon), data, rand() % 2, -1);
                    }
                }

                break;
            case '4':
                poke_selected = switch_pokemon(pc);
                if (poke_selected > 0 && poke_selected < 7) {
                    if (chosen_pokemon != poke_selected - 1) {
                        chosen_pokemon = poke_selected - 1;

                        if (pokemon->get_move(1) == 0) {
                            attack(pokemon, pc->get_pokemon(chosen_pokemon), data, 0, -1);
                        } else {
                            attack(pokemon, pc->get_pokemon(chosen_pokemon), data, rand() % 2, -1);
                        }
                    }
                } else if (poke_selected == -1) {
                    chosen_pokemon = -1;
                    mvprintw(19, 1, "You have no Pokemon to fight with! Press < to flee.");
                    while((key = getch()) != '<') {
                        mvprintw(19, 1, "You have no Pokemon to fight with! Press < to flee.");
                        refresh();
                    }
                    
                    done = 1;
                    continue;
                }
                
                break;
            default:
                break;
        }
        
        if (pc->get_pokemon(chosen_pokemon)->get_hp() == 0) {
            poke_selected = switch_pokemon(pc);
            if (poke_selected > 0 && poke_selected < 7) {
                chosen_pokemon = poke_selected - 1;
            } else {
                chosen_pokemon = -1;
                mvprintw(19, 1, "You have no Pokemon to fight with! Press < to flee.");
                while((key = getch()) != '<') {
                    mvprintw(19, 1, "You have no Pokemon to fight with! Press < to flee.");
                    refresh();
                }

                done = 1;
                continue;
            }
        } else if (knockout == -1) { // enemy fainted
            mvprintw(0, 1, "Wild pokemon fainted.");
            done = 1;
            continue;
        }

        mvprintw(1, 1, "You encountered a Pokemon!");
        mvprintw(2, 1, "%s:", pokemon->get_name().c_str());
        mvprintw(3, 1, "Gender: %d", pokemon->get_gender());
        mvprintw(4, 1, "Is Shiny: %d", pokemon->get_is_shiny());
        mvprintw(5, 1, "Level: %d", pokemon->get_level());
        mvprintw(6, 1, "HP: %d", pokemon->get_hp());
        mvprintw(7, 1, "Attack: %d", pokemon->get_attack());
        mvprintw(8, 1, "Defense: %d", pokemon->get_defense());
        mvprintw(9, 1, "Special Attack: %d", pokemon->get_special_attack());
        mvprintw(10, 1, "Special Defense: %d", pokemon->get_special_defense());
        mvprintw(11, 1, "Speed: %d", pokemon->get_speed());
        mvprintw(20, 1, "1. Fight");
        mvprintw(21, 1, "2. Bag");
        mvprintw(22, 1, "3. Run");
        mvprintw(23, 1, "4. Pokemon");
        if (pokemon->get_move(1) != 0) {
            mvprintw(12, 1, "Moves: %s, %s", data->moves[pokemon->get_move(0) - 1].identifier.c_str(), data->moves[pokemon->get_move(1) - 1].identifier.c_str()); // subtract one because the id is 1 off of data vector index for corresponding move
        } else {
            mvprintw(12, 1, "Moves: %s", data->moves[pokemon->get_move(0) - 1].identifier.c_str());
        }

        refresh();
    }

    return 0;
}

char move_pc(map *map, Data *data, pc *pc, int input, int manhattan_distance)
{
    if (map->get_turn_heap()) {}
    char ret_val;
    coordinate_t new_pos;
    int battle_outcome;
    Pokemon *pokemon_captured;

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
                battle_outcome = battle(pc, (npc *)map->trainer_map[new_pos.y][new_pos.x], data);

                if (battle_outcome == -1) { // npc lost
                    map->trainer_map[new_pos.y][new_pos.x]->set_next_turn(-1);
                }
            }
        } else if (rand() % 10 < 1 && map->terrain_map[new_pos.y][new_pos.x] == tall_grass) {
            pokemon_captured = encounter(pc, data, manhattan_distance);
            if (pokemon_captured) {
                pc->add_pokemon(pokemon_captured);
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

void move_dijkstra_trainer(heap_t *path_heap, path path_map[MAP_HEIGHT][MAP_WIDTH], map *map, npc *npc, Data *data)
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

        battle_outcome = battle((pc *)map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x], npc, data);

        if (battle_outcome == -1) { // npc lost
            npc->set_next_turn(-1);
        }
    }

    map->trainer_map[npc->get_pos().y][npc->get_pos().x] = nullptr;
    npc->set_pos(cheapest_path->get_coordinate());
    map->trainer_map[npc->get_pos().y][npc->get_pos().x] = npc;
    if (npc->get_next_turn() > -1) {
        if (cheapest_path->get_terrain_cost() == INT_MAX) {
            npc->set_next_turn(npc->get_next_turn() + 30);
        } else {
            npc->set_next_turn(npc->get_next_turn() + cheapest_path->get_terrain_cost());
        }
    }
}

void move_wanderer_explorer(map *map, npc *npc, Data *data)
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
                battle_outcome = battle((pc *)map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x], npc, data);

                if (battle_outcome == 1) { // npc won
                    random_turn(map, npc, data);
                } else if (battle_outcome == -1) { // npc lost
                    npc->set_next_turn(-1);
                }
            } else {
                random_turn(map, npc, data);
            }
        } else {
            random_turn(map, npc, data);
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
                battle_outcome = battle((pc *)map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x], npc, data);

                if (battle_outcome == 1) { // npc won
                    random_turn(map, npc, data);
                } else if (battle_outcome == -1) { // npc lost
                    npc->set_next_turn(-1);
                }
            } else {
                random_turn(map, npc, data);
            }
        } else {
            random_turn(map, npc, data);
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
                battle_outcome = battle((pc *)map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x], npc, data);

                if (battle_outcome == 1) { // npc won
                    random_turn(map, npc, data);
                } else if (battle_outcome == -1) { // npc lost
                    npc->set_next_turn(-1);
                }
            } else {
                random_turn(map, npc, data);
            }
        } else {
            random_turn(map, npc, data);
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
                battle_outcome = battle((pc *)map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x], npc, data);

                if (battle_outcome == 1) { // npc won
                    
                    random_turn(map, npc, data);
                } else if (battle_outcome == -1) { // npc lost
                    npc->set_next_turn(-1);
                }
            } else {
                random_turn(map, npc, data);
            }
        } else {
            random_turn(map, npc, data);
        }
    }

    if (npc->get_next_turn() > -1) {
        if (path::calculate_terrain_cost(map->terrain_map[npc->get_pos().y][npc->get_pos().x], npc->get_type()) == INT_MAX) {
            npc->set_next_turn(npc->get_next_turn() + 30);
        } else {
            npc->set_next_turn(npc->get_next_turn() + path::calculate_terrain_cost(map->terrain_map[npc->get_pos().y][npc->get_pos().x], npc->get_type()));
        }
    }
}

void move_swimmer(heap_t *path_heap, path path_map[MAP_HEIGHT][MAP_WIDTH], map *map, npc *m, coordinate_t pc_pos, Data *data)
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
        move_dijkstra_trainer(path_heap, path_map, map, m, data);
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
                    battle_outcome = battle((pc *)map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x], m, data);

                    if (battle_outcome == 1) { // m won
                        
                        random_turn(map, m, data);
                    } else if (battle_outcome == -1) { // m lost
                        m->set_next_turn(-1);
                    }
                } else {
                    random_turn(map, m, data);
                }
            } else {
                random_turn(map, m, data);
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
                    battle_outcome = battle((pc *)map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x], m, data);

                    if (battle_outcome == 1) { // m won
                        
                        random_turn(map, m, data);
                    } else if (battle_outcome == -1) { // m lost
                        m->set_next_turn(-1);
                    }
                } else {
                    random_turn(map, m, data);
                }
            } else {
                random_turn(map, m, data);
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
                    battle_outcome = battle((pc *)map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x], m, data);

                    if (battle_outcome == 1) { // m won
                        
                        random_turn(map, m, data);
                    } else if (battle_outcome == -1) { // m lost
                        m->set_next_turn(-1);
                    }
                } else {
                    random_turn(map, m, data);
                }
            } else {
                random_turn(map, m, data);
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
                    battle_outcome = battle((pc *)map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x], m, data);

                    if (battle_outcome == 1) { // m won
                        
                        random_turn(map, m, data);
                    } else if (battle_outcome == -1) { // m lost
                        m->set_next_turn(-1);
                    }
                } else {
                    random_turn(map, m, data);
                }
            } else {
                random_turn(map, m, data);
            }
        }

        if (m->get_next_turn() > -1) {
            if (path::calculate_terrain_cost(map->terrain_map[m->get_pos().y][m->get_pos().x], m->get_type()) == INT_MAX) {
                m->set_next_turn(m->get_next_turn() + 30);

            } else {
                m->set_next_turn(m->get_next_turn() + path::calculate_terrain_cost(map->terrain_map[m->get_pos().y][m->get_pos().x], m->get_type()));
            }
        }
    }
}

void move_pacer(map *map, npc *p, Data *data)
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
            battle_outcome = battle((pc *)map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x], p, data);

            if (battle_outcome == -1) { // npc lost
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
            battle_outcome = battle((pc *)map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x], p, data);

            if (battle_outcome == -1) { // npc lost
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
            battle_outcome = battle((pc *)map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x], p, data);

            if (battle_outcome == -1) { // npc lost
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
            battle_outcome = battle((pc *)map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x], p, data);

            if (battle_outcome == -1) { // npc lost
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
                battle_outcome = battle((pc *)map->trainer_map[map->get_pc_pos().y][map->get_pc_pos().x], p, data);

                if (battle_outcome == -1) { // npc lost
                    p->set_next_turn(-1);
                }
            }
        }
    }

    if (p->get_next_turn() > -1) {
        if (path::calculate_terrain_cost(map->terrain_map[p->get_pos().y][p->get_pos().x], p->get_type()) == INT_MAX) {
            p->set_next_turn(p->get_next_turn() + 30);
        } else {
            p->set_next_turn(p->get_next_turn() + path::calculate_terrain_cost(map->terrain_map[p->get_pos().y][p->get_pos().x], p->get_type()));
        }
    }
}
