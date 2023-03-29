#include <limits.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "world.h"

static int32_t path_cmp(const void *key, const void *with) { // make heap compare nodes based on cost
    return ((path_t *) key)->cost - ((path_t *) with)->cost;
}

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

void render_view(map_t *map)
{
    int y, x;
    int color;

    clear();

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            switch (map->terrain[y][x])
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

            if  (map->trainer_map[y][x] != NULL) {
                color = COLOR_RED;
                switch (map->trainer_map[y][x]->type) {
                    case pc:
                        view[y][x] = '@';
                        break;
                    case hiker:
                        view[y][x] = 'h';
                        break;
                    case rival:
                        view[y][x] = 'r';
                        break;
                    case pacer:
                        view[y][x] = 'p';
                        break;
                    case wanderer:
                        view[y][x] = 'w';
                        break;
                    case sentry:
                        view[y][x] = 's';
                        break;
                    case explorer:
                        view[y][x] = 'e';
                        break;
                    case swimmer:
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

    refresh();
}

void place_gates(world_t *world)
{
    if (world->location.y == 0) { // top of map
        world->current_map->n = -1;
    } else if (world->location.y == WORLD_HEIGHT - 1) { // bottom of map
        world->current_map->s = -1;
    } 
    
    if (world->location.x == 0) { // west edge of map
        world->current_map->w = -1;
    } else if (world->location.x == WORLD_WIDTH - 1) { // east edge of map
        world->current_map->e = -1;
    }

    // Check for north gate
    if (world->location.y > 0 && world->board[world->location.y - 1][world->location.x] != NULL) {
        world->current_map->n = world->board[world->location.y - 1][world->location.x]->s;
    }

    // Check for south gate
    if (world->location.y < WORLD_HEIGHT - 1 && world->board[world->location.y + 1][world->location.x] != NULL) {
        world->current_map->s = world->board[world->location.y + 1][world->location.x]->n;
    } 
    
    // Check for west gate
    if (world->location.x > 0 && world->board[world->location.y][world->location.x - 1] != NULL) {
        world->current_map->w = world->board[world->location.y][world->location.x - 1]->e;
    }

    // Check for east gate 
    if (world->location.x < WORLD_WIDTH - 1 && world->board[world->location.y][world->location.x + 1] != NULL) {
        world->current_map->e = world->board[world->location.y][world->location.x + 1]->w;
    }
}

void change_map(world_t *world, trainer_t *pc, char new_map)
{
    int manhattan_distance;

    world->current_map->trainer_map[pc->pos.y][pc->pos.x] = NULL;
    world->current_map->pc_turn = pc->next_turn;
    
    if (new_map == 'n' && world->location.y > 0) {
        world->location.y--;
        pc->pos.y = MAP_HEIGHT - 2;
    } else if (new_map == 's' && world->location.y < WORLD_HEIGHT - 1) {
        world->location.y++;
        pc->pos.y = 1;
    } else if (new_map == 'w' && world->location.x > 0) {
        world->location.x--;
        pc->pos.x = MAP_WIDTH - 2;
    } else if (new_map == 'e' && world->location.x < WORLD_WIDTH - 1) {
        world->location.x++;
        pc->pos.x = 1;
    }

    if (world->board[world->location.y][world->location.x] == NULL) {
        world->current_map = malloc(sizeof (*world->current_map));
        map_init(world->current_map);
        world->board[world->location.y][world->location.x] = world->current_map;

        place_gates(world);
        manhattan_distance = abs(world->location.x - START_X) + abs(world->location.y - START_Y);
        generate_map(world->current_map, world->current_map->n, world->current_map->s, world->current_map->w, world->current_map->e, manhattan_distance);
        trainer_map_init(world->current_map, world->num_trainers, pc); // init trainer_map, pc_pos, and turn_heap
    }

    world->current_map = world->board[world->location.y][world->location.x];
    pc->next_turn = world->current_map->pc_turn;
    world->current_map->trainer_map[pc->pos.y][pc->pos.x] = pc;
    world->current_map->pc_pos = pc->pos;
}

int pc_turn(world_t *world, trainer_t *pc)
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
                new_map = move_pc(world->current_map, pc, key);
                valid = 1;
                break;
            case 'f':
                // add flying functionality
            case '5':
            case ' ':
            case '.':
                pc->next_turn = pc->next_turn + 15;
                valid = 1;
                break;
            case '>':
                enter_building(world->current_map, pc);
                valid = 1;
                break;
            case 't':
                trainer_info(world->current_map, world->num_trainers);
                render_view(world->current_map);
                valid = 0;
                continue;
            case 'Q':
                quit_game = 1;
                valid = 1;
                break;
            default:
                mvprintw(22, 1,
                         "Use movement keys, info key, or 'Q' to quit.");
                valid = 0;
        }
    }

    if (pc->next_turn < 0) {
        quit_game = 1;
    }

    if (new_map != 0) {
        change_map(world, pc, new_map);
    }

    return quit_game;
}

void game_loop(heap_t *path_heap, world_t *world)
{
    trainer_t *t;
    int x, y, quit_game;
    path_t swimmer_path[MAP_HEIGHT][MAP_WIDTH];

    quit_game = 0;

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            swimmer_path[y][x].heap_node = NULL;
            swimmer_path[y][x].coordinate.x = x;
            swimmer_path[y][x].coordinate.y = y;
            swimmer_path[y][x].terrain.type = edge;
            swimmer_path[y][x].terrain.cost = INT_MAX;
            swimmer_path[y][x].cost = INT_MAX;
        }
    }

    render_view(world->current_map);

    while (!quit_game) {
        t = heap_remove_min(world->current_map->turn_heap);

        switch (t->type) {
            case pc:
                quit_game = pc_turn(world, t);
                break;
            case hiker:
                move_dijkstra_trainer(path_heap, world->hiker_path, world->current_map, t);
                break;
            case rival:
                move_dijkstra_trainer(path_heap, world->rival_path, world->current_map, t);
                break;
            case pacer:
                move_pacer(world->current_map, t);
                break;
            case wanderer:
            case explorer:
                move_wanderer_explorer(world->current_map, t);
                break;
            case sentry:
                t->next_turn = t->next_turn + 15;
                break;
            case swimmer:
                move_swimmer(path_heap, swimmer_path, world->current_map, t, world->current_map->pc_pos);
                break;
        }

        render_view(world->current_map);

        if (t->next_turn > -1) {
            heap_insert(world->current_map->turn_heap, t);
        }

        if (world->current_map->trainer_map[world->current_map->pc_pos.y][world->current_map->pc_pos.x] != NULL &&
            world->current_map->trainer_map[world->current_map->pc_pos.y][world->current_map->pc_pos.x]->next_turn == -1) {
            break;
        }
    }
}

void world_init(world_t *world, int num_trainers)
{
    int x, y, manhattan_distance;
    for (y = 0; y < WORLD_HEIGHT; y++) {
        for (x = 0; x < WORLD_WIDTH; x++) {
            world->board[y][x] = NULL;
        }
    }
    world->current_map = NULL;
    world->current_map = malloc(sizeof (*world->current_map));

    world->location.x = START_X;
    world->location.y = START_Y;
    world->num_trainers = num_trainers;

    world->board[START_Y][START_X] = malloc(sizeof (*world->current_map));
    world->current_map = world->board[START_Y][START_X];
    map_init(world->current_map);
    place_gates(world);
    manhattan_distance = abs(world->location.x - START_X) + abs(world->location.y - START_Y);
    generate_map(world->current_map, world->current_map->n, world->current_map->s, world->current_map->w, world->current_map->e, manhattan_distance);
    trainer_map_init(world->current_map, world->num_trainers, NULL); // init trainer_map, pc_pos, and turn_heap

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            world->hiker_path[y][x].heap_node = NULL;
            world->hiker_path[y][x].coordinate.x = x;
            world->hiker_path[y][x].coordinate.y = y;
            world->hiker_path[y][x].terrain.type = edge;
            world->hiker_path[y][x].terrain.cost = INT_MAX;
            world->hiker_path[y][x].cost = INT_MAX;

            world->rival_path[y][x].heap_node = NULL;
            world->rival_path[y][x].coordinate.x = x;
            world->rival_path[y][x].coordinate.y = y;
            world->rival_path[y][x].terrain.type = edge;
            world->rival_path[y][x].terrain.cost = INT_MAX;
            world->rival_path[y][x].cost = INT_MAX;
        }
    }
}

void world_delete(world_t *world)
{
    int x, y;

    for (y = 0; y < WORLD_HEIGHT; y++) {
        for (x = 0; x < WORLD_WIDTH; x++) {
            if (world->board[y][x] != NULL) {
                trainer_delete(world->board[y][x]->trainer_map);
                map_delete(world->board[y][x]);
            }
        }
    }

    free(world);
}

int main(int argc, char *argv[])
{
    heap_t path_heap;
    world_t *world;

    srand(time(NULL));

    init_terminal();

    init_view();

    heap_init(&path_heap, path_cmp, NULL);

    world = malloc(sizeof (*world));
    if (argc == 3) {
        if (!strcmp(argv[1], "--numtrainers")) {
            world_init(world, atoi(argv[2]));
        } else {
            fprintf(stderr, "Usage: \"./play --numtrainers <integer>\" or \"./play\" \n");

            return -1;
        }
    } else if (argc == 1) {
        world_init(world, 10);
    } else {
        fprintf(stderr, "Usage: \"./play --numtrainers <integer>\" or \"./play\" \n");

        return -1;
    }

    game_loop(&path_heap, world);

    endwin();

    heap_delete(&path_heap);
    world_delete(world);

    printf("Thanks for playing!\n");

    return 0;
}
