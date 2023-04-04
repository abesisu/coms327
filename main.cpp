#include <iostream>
#include <ncurses.h>
#include <cstring>

#include "world.h"
#include "data.h"

// static int32_t path_cmp(const void *key, const void *with) { // make heap compare nodes based on cost
//     return ((path *) key)->get_cost() - ((path *) with)->get_cost();
// }

chtype view[MAP_HEIGHT][MAP_WIDTH];

void init_terminal(void)
{
    initscr();
    raw();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    start_color();
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLUE);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
}

void init_view()
{
    int x, y;

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            view[y][x] = ' ';
        }
    }
}

void render_view(map *map, coordinate_t location)
{
    int y, x;
    int color;

    clear();

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            switch (map->terrain_map[y][x])
            {
                case grass:
                    view[y][x] = '.';
                    color = COLOR_GREEN;
                    break;
                case tall_grass:
                    view[y][x] = ':';
                    color = COLOR_GREEN;
                    break;
                case boulder:
                case edge:
                    view[y][x] = '%';
                    color = COLOR_WHITE;
                    break;
                case tree:
                case willow:
                    view[y][x] = '^';
                    color = COLOR_MAGENTA;
                    break;
                case water:
                    view[y][x] = '~';
                    color = COLOR_BLUE;
                    break;
                case road:
                case gate:
                case bridge:
                    view[y][x] = '#';
                    color = COLOR_YELLOW;
                    break;
                case pokemart:
                    view[y][x] = 'M';
                    color = COLOR_CYAN;
                    break;
                case pokecenter:
                    view[y][x] = 'C';
                    color = COLOR_CYAN;
                    break;
                default:
                    view[y][x] = '!'; // Something went wrong
                    color = COLOR_RED;
                    break;
            }

            if  (map->trainer_map[y][x] != nullptr) {
                color = COLOR_RED;
                switch (map->trainer_map[y][x]->get_type()) {
                    case pc_e:
                        view[y][x] = '@';
                        break;
                    case hiker_e:
                        view[y][x] = 'h';
                        break;
                    case rival_e:
                        view[y][x] = 'r';
                        break;
                    case pacer_e:
                        view[y][x] = 'p';
                        break;
                    case wanderer_e:
                        view[y][x] = 'w';
                        break;
                    case sentry_e:
                        view[y][x] = 's';
                        break;
                    case explorer_e:
                        view[y][x] = 'e';
                        break;
                    case swimmer_e:
                        view[y][x] = 'm';
                        break;
                    default:
                        view[y][x] = '?'; // Something went wrong
                        break;
                }
            }

            attron(COLOR_PAIR(color));
            mvaddch(1 + y, x, view[y][x]);
            attroff(COLOR_PAIR(color));
        }
    }

    mvprintw(22, 1, "(%d, %d)", location.x - START_X, START_Y - location.y);

    refresh();
}

void place_gates(world *world)
{
    if (world->get_location().y == 0) { // top of map
        world->get_current_map()->set_n(-1);
    } else if (world->get_location().y == WORLD_HEIGHT - 1) { // bottom of map
        world->get_current_map()->set_s(-1);
    } 
    
    if (world->get_location().x == 0) { // west edge of map
        world->get_current_map()->set_w(-1);
    } else if (world->get_location().x == WORLD_WIDTH - 1) { // east edge of map
        world->get_current_map()->set_e(-1);
    }

    // Check for north gate
    if (world->get_location().y > 0 && world->board[world->get_location().y - 1][world->get_location().x] != nullptr) {
        world->get_current_map()->set_n(world->board[world->get_location().y - 1][world->get_location().x]->get_s());
    }

    // Check for south gate
    if (world->get_location().y < WORLD_HEIGHT - 1 && world->board[world->get_location().y + 1][world->get_location().x] != nullptr) {
        world->get_current_map()->set_s(world->board[world->get_location().y + 1][world->get_location().x]->get_n());
    } 
    
    // Check for west gate
    if (world->get_location().x > 0 && world->board[world->get_location().y][world->get_location().x - 1] != nullptr) {
        world->get_current_map()->set_w(world->board[world->get_location().y][world->get_location().x - 1]->get_e());
    }

    // Check for east gate 
    if (world->get_location().x < WORLD_WIDTH - 1 && world->board[world->get_location().y][world->get_location().x + 1] != nullptr) {
        world->get_current_map()->set_e(world->board[world->get_location().y][world->get_location().x + 1]->get_w());
    }
}

void change_map(world *world, pc *pc, char new_map)
{
    int fly_x, fly_y;
    int manhattan_distance;

    world->get_current_map()->trainer_map[pc->get_pos().y][pc->get_pos().x] = nullptr;
    world->get_current_map()->set_pc_turn(pc->get_next_turn());
    
    if (new_map == 'n' && world->get_location().y > 0) {
        world->set_location_y(world->get_location().y - 1);
        pc->set_pos_y(MAP_HEIGHT - 2);
    } else if (new_map == 's' && world->get_location().y < WORLD_HEIGHT - 1) {
        world->set_location_y(world->get_location().y + 1);
        pc->set_pos_y(1);
    } else if (new_map == 'w' && world->get_location().x > 0) {
        world->set_location_x(world->get_location().x - 1);
        pc->set_pos_x(MAP_WIDTH - 2);
    } else if (new_map == 'e' && world->get_location().x < WORLD_WIDTH - 1) {
        world->set_location_x(world->get_location().x + 1);
        pc->set_pos_x(1);
    } else if (new_map == 'f') {
        mvprintw(22, 1, "Fly to: ");
        refresh();
        echo();
        scanw((char *)"%d %d", &fly_x, &fly_y);
        noecho();
        if (fly_x >= -1 * WORLD_WIDTH / 2 && fly_x <= WORLD_WIDTH / 2) {
            world->set_location_x(START_X + fly_x);
        }

        if (fly_y >= -1 * WORLD_HEIGHT / 2 && fly_y <= WORLD_HEIGHT / 2) {
            world->set_location_y(START_Y - fly_y);
        }
    }

    if (world->board[world->get_location().y][world->get_location().x] == nullptr) {
        world->set_current_map(new map());
        world->board[world->get_location().y][world->get_location().x] = world->get_current_map();

        place_gates(world);
        manhattan_distance = abs(world->get_location().x - START_X) + abs(world->get_location().y - START_Y);
        map::generate_map(world->get_current_map(), world->get_current_map()->get_n(), world->get_current_map()->get_s(), world->get_current_map()->get_w(), world->get_current_map()->get_e(), manhattan_distance);

        if (new_map == 'f') {
            map::place_pc(world->get_current_map(), pc);
        }
        map::trainer_map_init(world->get_current_map(), world->get_num_trainers(), pc); // init trainer_map, pc_pos, and turn_heap
    } else if (new_map == 'f') {
        world->board[world->get_location().y][world->get_location().x]->trainer_map[pc->get_pos().y][pc->get_pos().x] = nullptr;
        map::place_pc(world->board[world->get_location().y][world->get_location().x], pc);
    }

    world->set_current_map(world->board[world->get_location().y][world->get_location().x]);
    pc->set_next_turn(world->get_current_map()->get_pc_turn());
    world->get_current_map()->trainer_map[pc->get_pos().y][pc->get_pos().x] = pc;
    world->get_current_map()->set_pc_pos(pc->get_pos());
}

int pc_turn(world *world, pc *pc)
{
    int quit_game, valid, key;
    char new_map;

    quit_game = valid = 0;

    while (!valid) {
        key = getch();

        switch (key) {
            case '7':
            case 'y':
            case '8':
            case 'k':
            case '9':
            case 'u':
            case '6':
            case 'l':
            case '3':
            case 'n':
            case '2':
            case 'j':
            case '1':
            case 'b':
            case '4':
            case 'h':
                new_map = action::move_pc(world->get_current_map(), pc, key);
                valid = 1;
                break;
            case 'f':
                new_map = 'f';
                valid = 1;
                break;
            case '5':
            case ' ':
            case '.':
                pc->set_next_turn(pc->get_next_turn() + 15);
                valid = 1;
                break;
            case '>':
                action::enter_building(world->get_current_map(), pc);
                valid = 1;
                break;
            case 't':
                action::trainer_info(world->get_current_map(), world->get_num_trainers());
                render_view(world->get_current_map(), world->get_location());
                valid = 0;
                continue;
            case 'Q':
                quit_game = 1;
                valid = 1;
                break;
            default:
                mvprintw(22, 1, "Use movement keys, info key, or 'Q' to quit.");
                valid = 0;
        }
    }

    if (pc->get_next_turn() < 0) {
        quit_game = 1;
    }

    if (new_map != 0) {
        change_map(world, pc, new_map);
    }

    return quit_game;
}

void game_loop(heap_t *path_heap, world *world)
{
    trainer *t;
    pc *p;
    npc *n;

    int x, y, quit_game;
    path swimmer_path[MAP_HEIGHT][MAP_WIDTH];

    quit_game = 0;

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            swimmer_path[y][x] = *new path();
        }
    }

    render_view(world->get_current_map(), world->get_location());

    while (!quit_game) {
        t = (trainer *) heap_remove_min(world->get_current_map()->get_turn_heap());

        if (t->get_type() != pc_e) {
            n = new npc(t);
        }

        switch (t->get_type()) {
            case pc_e:
                p = new pc(t);
                quit_game = pc_turn(world, p);
                t = p;
                break;
            case hiker_e:
                action::move_dijkstra_trainer(path_heap, world->hiker_path, world->get_current_map(), n);
                break;
            case rival_e:
                action::move_dijkstra_trainer(path_heap, world->rival_path, world->get_current_map(), n);
                break;
            case pacer_e:
                action::move_pacer(world->get_current_map(), n);
                break;
            case wanderer_e:
            case explorer_e:
                action::move_wanderer_explorer(world->get_current_map(), n);
                break;
            case sentry_e:
                n->set_next_turn(n->get_next_turn() + 15);
                break;
            case swimmer_e:
                action::move_swimmer(path_heap, swimmer_path, world->get_current_map(), n, world->get_current_map()->get_pc_pos());
                break;
        }
        if (t->get_type() != pc_e) {
            t = n;
        }

        render_view(world->get_current_map(), world->get_location());

        if (t->get_next_turn() > -1) {
            heap_insert(world->get_current_map()->get_turn_heap(), t);
        }

        if (world->get_current_map()->trainer_map[world->get_current_map()->get_pc_pos().y][world->get_current_map()->get_pc_pos().x] != nullptr &&
            world->get_current_map()->trainer_map[world->get_current_map()->get_pc_pos().y][world->get_current_map()->get_pc_pos().x]->get_next_turn() == -1) {
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    // int manhattan_distance;
    // heap_t path_heap;
    // world *w;
    int got_data;
    Data data;

    // srand(time(nullptr));

    // init_terminal();

    // init_view();

    // heap_init(&path_heap, path_cmp, nullptr);

    if (argc == 2) {
        // if (strcmp(argv[1], "--numtrainers" == 0)) {
        //      w = new world(atoi(argv[2]));
        // } else 
        if (strcmp(argv[1], "pokemon") == 0) { 
            got_data = data.get_pokemon(data);
            Pokemon *p;

            if (!got_data) {
                std::cerr << "Error: Could not get pokemon data." << std::endl;
            } else {
                std::vector<Pokemon>::size_type i;
                for (i = 0; i < data.pokemon.size(); i++) {
                    p = &data.pokemon[i];
                    std::cout << (*p).id << ", " << (*p).identifier << ", " << (*p).species_id << ", " 
                              <<(*p).height << ", " << (*p).weight << ", " << (*p).base_experience << ", " 
                              << (*p).order << ", " << (*p).is_default << std::endl;
                }
            }
        } else if (strcmp(argv[1], "pokemon_types") == 0) {
            got_data = data.get_pokemon_types(data);
            PokemonType *pt;

            if (!got_data) {
                std::cerr << "Error: Could not get pokemon types data." << std::endl;
            } else {
                std::vector<PokemonType>::size_type i;
                for (i = 0; i < data.pokemon_types.size(); i++) {
                    pt = &data.pokemon_types[i];
                    std::cout << (*pt).pokemon_id << ", " << (*pt).type_id << ", " << (*pt).slot << std::endl;
                }
            }
        } else if (strcmp(argv[1], "type_names") == 0) {
            got_data = data.get_type_names(data);
            TypeName *tn;

            if (!got_data) {
                std::cerr << "Error: Could not get type names data." << std::endl;
            } else {
                std::vector<TypeName>::size_type i;
                for (i = 0; i < data.type_names.size(); i++) {
                    tn = &data.type_names[i];
                    std::cout << (*tn).type_id << ", " << (*tn).local_language_id << ", " << (*tn).name << std::endl;
                }
            }
        } else if (strcmp(argv[1], "pokemon_species") == 0) {
            got_data = data.get_pokemon_species(data);
            PokemonSpecies *ps;

            if (!got_data) {
                std::cerr << "Error: Could not get pokemon species data." << std::endl;
            } else {
                std::vector<PokemonSpecies>::size_type i;
                for (i = 0; i < data.pokemon_species.size(); i++) {
                    ps = &data.pokemon_species[i];
                    std::cout << (*ps).id << ", " << (*ps).identifier << ", " << (*ps).generation_id << ", " 
                              << (*ps).evolves_from_species_id << ", " << (*ps).evolution_chain_id << ", " 
                              << (*ps).color_id << ", " << (*ps).shape_id << ", " << (*ps).habitat_id << ", " 
                              << (*ps).gender_rate << ", " << (*ps).capture_rate << ", " << (*ps).base_happiness 
                              << ", " << (*ps).is_baby << ", " << (*ps).hatch_counter << ", " 
                              << (*ps).has_gender_differences << ", " << (*ps).growth_rate_id << ", " 
                              << (*ps).forms_switchable << ", " << (*ps).is_legendary << ", " 
                              << (*ps).is_mythical << ", " << (*ps).order << ", " << (*ps).conquest_order << std::endl;
                }
            }
        } else if (strcmp(argv[1], "pokemon_stats") == 0) {
            got_data = data.get_pokemon_stats(data);
            PokemonStat *ps;

            if (!got_data) {
                std::cerr << "Error: Could not get pokemon stats data." << std::endl;
            } else {
                std::vector<PokemonStat>::size_type i;
                for (i = 0; i < data.pokemon_stats.size(); i++) {
                    ps = &data.pokemon_stats[i];
                    std::cout << (*ps).pokemon_id << ", " << (*ps).stat_id << ", " 
                              << (*ps).base_stat << ", " << (*ps).effort << std::endl;
                }
            }
        } else if (strcmp(argv[1], "stats") == 0) {
            got_data = data.get_stats(data);
            Stat *s;

            if (!got_data) {
                std::cerr << "Error: Could not get stats data." << std::endl;
            } else {
                std::vector<Stat>::size_type i;
                for (i = 0; i < data.stats.size(); i++) {
                    s = &data.stats[i];
                    std::cout << (*s).id << ", " << (*s).damage_class_id << ", " << (*s).identifier << ", " 
                              << (*s).is_battle_only << ", " << (*s).game_index << std::endl;
                }
            }
        } else if (strcmp(argv[1], "pokemon_moves") == 0) {
            got_data = data.get_pokemon_moves(data);
            PokemonMove *pm;

            if (!got_data) {
                std::cerr << "Error: Could not get pokemon moves data." << std::endl;
            } else {
                std::vector<PokemonMove>::size_type i;
                for (i = 0; i < data.pokemon_moves.size(); i++) {
                    pm = &data.pokemon_moves[i];
                    std::cout << (*pm).pokemon_id << ", " << (*pm).version_group_id << ", " << (*pm).move_id << ", " 
                              << (*pm).pokemon_move_method_id << ", " << (*pm).level << ", " << (*pm).order << std::endl;
                }
            }
        } else if (strcmp(argv[1], "moves") == 0) {
            got_data = data.get_moves(data);
            Move *m;

            if (!got_data) {
                std::cerr << "Error: Could not get moves data." << std::endl;
            } else {
                std::vector<Move>::size_type i;
                for (i = 0; i < data.moves.size(); i++) {
                    m = &data.moves[i];
                    std::cout << (*m).id << ", " << (*m).identifier << ", " << (*m).generation_id << ", " << (*m).type_id << ", " 
                              << (*m).power << ", " << (*m).pp << ", " << (*m).accuracy << ", " << (*m).priority << ", " 
                              << (*m).target_id << ", " << (*m).damage_class_id << ", " << (*m).effect_id << ", " 
                              << (*m).effect_chance << ", " << (*m).contest_type_id << ", " << (*m).contest_effect_id << ", " 
                              << (*m).super_contest_effect_id << std::endl;
                }
            }
        } else if (strcmp(argv[1], "experience") == 0) { 
            got_data = data.get_experience(data);
            Experience *e;

            if (!got_data) {
                std::cerr << "Error: Could not get experience data." << std::endl;
            } else {
                std::vector<Experience>::size_type i;
                for (i = 0; i < data.experience.size(); i++) {
                    e = &data.experience[i];
                    std::cout << (*e).growth_rate_id << ", " << (*e).level << ", " << (*e).experience << std::endl;
                }
            }
        } else {
            std::cerr << "Usage: \"./play <object>\" or \"./play\" where <object> is one of \n \\
                pokemon, moves, pokemon_moves, pokemon_species, experience, type_names, pokemon stats, stats or pokemon_types" << std::endl;

            got_data = -1;
        }
    } else if (argc == 1) {
        // w = new world(10);
       got_data = data.get_data(data);

       if (!got_data) {
           std::cerr << "Error: Could not get data." << std::endl;
       }
    } else {
        std::cerr << "Usage: \"./play <object>\" or \"./play\" where <object> is one of \n \\
                pokemon, moves, pokemon_moves, pokemon_species, experience, type_names, pokemon stats, stats or pokemon_types" << std::endl;

        got_data = -1;
    }

    return got_data;

    // place_gates(w);
    // manhattan_distance = abs(w->get_location().x - START_X) + abs(w->get_location().y - START_Y);
    // map::generate_map(w->get_current_map(), w->get_current_map()->get_n(), w->get_current_map()->get_s(), w->get_current_map()->get_w(), w->get_current_map()->get_e(), manhattan_distance);
    // map::trainer_map_init(w->get_current_map(), w->get_num_trainers(), nullptr); // init trainer_map, pc_pos, and turn_heap

    // game_loop(&path_heap, w);

    // endwin();

    // heap_delete(&path_heap);

    // printf("Thanks for playing!\n");

    // return 0;
}
