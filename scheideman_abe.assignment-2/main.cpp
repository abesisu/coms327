#include <cstdlib>
#include <ncurses.h>
#include <string>

#include "setup.h"

#define VIEW_HEIGHT 13

void init_terminal(void)
{
    initscr();
    raw();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    start_color();
    init_pair(COLOR_RED, COLOR_RED, COLOR_WHITE);
    init_pair(COLOR_WHITE, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_WHITE, COLOR_RED);
}

void display(Setup *setup, std::vector<Card*> *hand)
{
    int x, y;
    int color;
    char suits[4] = {'H', 'D', 'C', 'S'};
    int colors[4] = {COLOR_RED, COLOR_RED, COLOR_WHITE, COLOR_WHITE};

    clear();

    // print foundation
    for (x = 0; x < 4; x++) {
        if (!setup->foundation[x].empty()) {
            mvaddch(0, x * 3, setup->foundation[x].top()->get_rank_char());
        } else {
            mvaddch(0, x * 3, ' ');
        }

        color = colors[x];
        attron(COLOR_PAIR(color));
        mvaddch(0, x * 3 + 1, suits[x]);
        attroff(COLOR_PAIR(color));
        mvaddch(0, x * 3 + 2, ' ');
    }

    // print tableu 
    for (x = 0; x < 7; x++) {
        if (!setup->tableu[x].empty()) {
            mvaddch(1, x * 3, setup->tableu[x].top()->get_rank_char());
            if (setup->tableu[x].top()->get_suit() < 2) {
                color = COLOR_RED;
            } else {
                color = COLOR_WHITE;
            }
            attron(COLOR_PAIR(color));
            mvaddch(1, x * 3 + 1, setup->tableu[x].top()->get_suit_char());
            attroff(COLOR_PAIR(color));
            mvaddch(1, x * 3 + 2, ' ');
        } else {
            mvaddch(1, x * 3, ' ');
            mvaddch(1, x * 3 + 1, ' ');
            mvaddch(1, x * 3 + 2, ' ');
        }
    }

    // print tableu columns
    for (y = 1; y < VIEW_HEIGHT; y++) {
        for (x = 0; x < 7; x++) {
            if (setup->tableu_columns[y - 1][x]) {
                mvaddch(y + 1, x * 3, setup->tableu_columns[y - 1][x]->get_rank_char());
                if (setup->tableu_columns[y - 1][x]->get_suit() < 2) {
                    color = COLOR_RED;
                } else {
                    color = COLOR_WHITE;
                }
                attron(COLOR_PAIR(color));
                mvaddch(y + 1, x * 3 + 1, setup->tableu_columns[y - 1][x]->get_suit_char());
                attroff(COLOR_PAIR(color));
                mvaddch(y + 1, x * 3 + 2, ' ');
            } else {
                mvaddch(y + 1, x * 3, ' ');
                mvaddch(y + 1, x * 3 + 1, ' ');
                mvaddch(y + 1, x * 3 + 2, ' ');
            }
        }
    }

    // print stock
    move(14, 0);
    clrtoeol();
    if ((*hand).size() > 3) {
        mvprintw(14, 0, "Error with stock draw.");
    } else if ((*hand).size() == 3) {
        mvprintw(14, 0, "Stock Size: %d Stock Draw: %c  %c  %c ", setup->stock.size(), (*hand)[0]->get_rank_char(), (*hand)[1]->get_rank_char(), (*hand)[2]->get_rank_char());
        if ((*hand)[0]->is_red()) {
            attron(COLOR_PAIR(COLOR_RED));
            mvaddch(14, 28, (*hand)[0]->get_suit_char());
            attroff(COLOR_PAIR(COLOR_RED));
        } else {
            attron(COLOR_PAIR(COLOR_WHITE));
            mvaddch(14, 28, (*hand)[0]->get_suit_char());
            attroff(COLOR_PAIR(COLOR_WHITE));
        }

        if ((*hand)[1]->is_red()) {
            attron(COLOR_PAIR(COLOR_RED));
            mvaddch(14, 31, (*hand)[1]->get_suit_char());
            attroff(COLOR_PAIR(COLOR_RED));
        } else {
            attron(COLOR_PAIR(COLOR_WHITE));
            mvaddch(14, 31, (*hand)[1]->get_suit_char());
            attroff(COLOR_PAIR(COLOR_WHITE));
        }

        if ((*hand)[2]->is_red()) {
            attron(COLOR_PAIR(COLOR_RED));
            mvaddch(14, 34, (*hand)[2]->get_suit_char());
            attroff(COLOR_PAIR(COLOR_RED));
        } else {
            attron(COLOR_PAIR(COLOR_WHITE));
            mvaddch(14, 34, (*hand)[2]->get_suit_char());
            attroff(COLOR_PAIR(COLOR_WHITE));
        }
    } else if ((*hand).size() == 2) {
        mvprintw(14, 0, "Stock Size: %d Stock Draw: %c  %c ", setup->stock.size(), (*hand)[0]->get_rank_char(), (*hand)[1]->get_rank_char());
        if ((*hand)[0]->is_red()) {
            attron(COLOR_PAIR(COLOR_RED));
            mvaddch(14, 28, (*hand)[0]->get_suit_char());
            attroff(COLOR_PAIR(COLOR_RED));
        } else {
            attron(COLOR_PAIR(COLOR_WHITE));
            mvaddch(14, 28, (*hand)[0]->get_suit_char());
            attroff(COLOR_PAIR(COLOR_WHITE));
        }

        if ((*hand)[1]->is_red()) {
            attron(COLOR_PAIR(COLOR_RED));
            mvaddch(14, 31, (*hand)[1]->get_suit_char());
            attroff(COLOR_PAIR(COLOR_RED));
        } else {
            attron(COLOR_PAIR(COLOR_WHITE));
            mvaddch(14, 31, (*hand)[1]->get_suit_char());
            attroff(COLOR_PAIR(COLOR_WHITE));
        }
    } else if ((*hand).size() == 1) {
        mvprintw(14, 0, "Stock Size: %d Stock Draw: %c ", setup->stock.size(), (*hand)[0]->get_rank_char());
        if ((*hand)[0]->is_red()) {
            attron(COLOR_PAIR(COLOR_RED));
            mvaddch(14, 28, (*hand)[0]->get_suit_char());
            attroff(COLOR_PAIR(COLOR_RED));
        } else {
            attron(COLOR_PAIR(COLOR_WHITE));
            mvaddch(14, 28, (*hand)[0]->get_suit_char());
            attroff(COLOR_PAIR(COLOR_WHITE));
        }
    } else if ((*hand).size() == 0) {
        mvprintw(14, 0, "Stock Size: %d", setup->stock.size());
    }

    move(15, 0);
    clrtoeol();
    move(16, 0);
    clrtoeol();
    mvprintw(15, 0, "Press 'd' to cycle through the next cards in stock, press 'D' to place the top (rightmost)");
    mvprintw(16, 0, "card from the stock, enter a number 1-7 to move a card, or press 'Q' to quit.");
    refresh();
}

Setup *init_setup()
{
    Deck *deck;
    Setup *setup;

    deck = new Deck();
    deck->shuffle();

    setup = new Setup(deck);

    return setup;
}

int place_from_hand(Setup *setup, Card *card)
{
    int cancel = 0;
    int card_placed;
    char key;

    move(15, 0);
    clrtoeol();
    move(16, 0);
    clrtoeol();
    mvprintw(15, 0, "To place in the foundation, press 'H', 'D', 'C', or 'S'.");
    mvprintw(16, 0, "Press 1-7 to place in a column. Press 'x' to cancel.");
    refresh();
    while (!cancel) {
        key = getch();
        switch (key) {
            case 'x':
                cancel = 1;
                card_placed = 0;
                break;
            case 'H':
                cancel = setup->stack_foundation(card, 0);
                if (cancel) {
                    card_placed = 1;
                }
                break;
            case 'D':
                cancel = setup->stack_foundation(card, 1);
                if (cancel) {
                    card_placed = 1;
                }
                break;
            case 'C':
                cancel = setup->stack_foundation(card, 2);
                if (cancel) {
                    card_placed = 1;
                }
                break;
            case 'S':
                cancel = setup->stack_foundation(card, 3);
                if (cancel) {
                    card_placed = 1;
                }
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
                cancel = setup->stack_column(card, key - '0' - 1);
                if (cancel) {
                    card_placed = 1;
                }
                break;
            default:
                cancel = 0;
                break;
        }
    }

    return card_placed;
}

int move_pile(Setup *setup, int column, int num)
{
    int pile_size;
    int cancel = 0;
    int cards_placed;
    char key;

    if (setup->tableu[column].empty()) {
        return 0;
    }
    pile_size = 1;
    while (setup->tableu_columns[pile_size - 1][column] != 0 && pile_size - 1 < 13) {
        pile_size++;
    }

    if (num < 0 || num > pile_size) {
        return 0;
    }

    move(15, 0);
    clrtoeol();
    move(16, 0);
    clrtoeol();
    mvprintw(15, 0, "Press 1-7 to place in a column. Press 'x' to cancel.");
    refresh();

    while (!cancel) {
        key = getch();
        switch (key) {
            case 'x':
                cancel = 1;
                cards_placed = 0;
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
                if (pile_size >= num && column != key - '0' - 1) {
                    while (num > 0) {
                        if (pile_size == 1 || pile_size == num) {
                            cancel = setup->stack_column(setup->tableu[column].top(), key - '0' - 1);
                            if (cancel) {
                                setup->tableu[column].pop();
                                cards_placed = 1;
                            } else {
                                cards_placed = 0;
                            }
                        } else {
                            cancel = setup->stack_column(setup->tableu_columns[pile_size - num - 1][column], key - '0' - 1);
                            if (cancel) {
                                setup->tableu_columns[pile_size - num - 1][column] = 0;
                                cards_placed = 1;
                            } else {
                                cards_placed = 0;
                            }
                        }
                        if (cards_placed) {
                            num--;
                        } else {
                            cancel = 0;
                            break;
                        }
                    }
                } else {
                    cancel = 0;
                }
                break;
            default:
                cancel = 0;
                break;
        }
    }

    return cards_placed;
}

int place_from_column(Setup *setup, int column)
{
    int i;
    int cancel = 0;
    int card_placed;
    char key;

    i = 0;
    while (setup->tableu_columns[i][column] != 0 && i < 13) {
        i++;
    }

    if (i >= 13) {
        return 0;
    }

    if (i == 0 && setup->tableu[column].empty()) {
        return 0;
    }

    move(15, 0);
    clrtoeol();
    move(16, 0);
    clrtoeol();
    mvprintw(15, 0, "To place in the foundation, press 'H', 'D', 'C', or 'S'.");
    mvprintw(16, 0, "Press 1-7 to place in a column. Press 'x' to cancel.");
    refresh();

    while (!cancel) {
        key = getch();
        switch (key) {
            case 'x':
                cancel = 1;
                card_placed = 0;
                break;
            case 'H':
                if (i == 0) {
                    cancel = setup->stack_foundation(setup->tableu[column].top(), 0);
                    if (cancel) {
                        setup->tableu[column].pop();
                        card_placed = 1;
                    }
                } else {
                    cancel = setup->stack_foundation(setup->tableu_columns[i][column], 0);
                    if (cancel) {
                        setup->tableu_columns[i][column] = 0;
                        card_placed = 1;
                    }
                }
                break;
            case 'D':
                if (i == 0) {
                    cancel = setup->stack_foundation(setup->tableu[column].top(), 1);
                    if (cancel) {
                        setup->tableu[column].pop();
                        card_placed = 1;
                    }
                } else {
                    cancel = setup->stack_foundation(setup->tableu_columns[i][column], 1);
                    if (cancel) {
                        setup->tableu_columns[i][column] = 0;
                        card_placed = 1;
                    }
                }
                break;
            case 'C':
                if (i == 0) {
                    cancel = setup->stack_foundation(setup->tableu[column].top(), 2);
                    if (cancel) {
                        setup->tableu[column].pop();
                        card_placed = 1;
                    }
                } else {
                    cancel = setup->stack_foundation(setup->tableu_columns[i][column], 2);
                    if (cancel) {
                        setup->tableu_columns[i][column] = 0;
                        card_placed = 1;
                    }
                }
                break;
            case 'S':
                if (i == 0) {
                    cancel = setup->stack_foundation(setup->tableu[column].top(), 3);
                    if (cancel) {
                        setup->tableu[column].pop();
                        card_placed = 1;
                    }
                } else {
                    cancel = setup->stack_foundation(setup->tableu_columns[i][column], 3);
                    if (cancel) {
                        setup->tableu_columns[i][column] = 0;
                        card_placed = 1;
                    }
                }
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
                if (i == 0 && column != key - '0' - 1) {
                    cancel = setup->stack_column(setup->tableu[column].top(), key - '0' - 1);
                    if (cancel) {
                        setup->tableu[column].pop();
                        card_placed = 1;
                    }
                } else if (column != key - '0' - 1) {
                    cancel = setup->stack_column(setup->tableu_columns[i][column], key - '0' - 1);
                    if (cancel) {
                        setup->tableu_columns[i][column] = 0;
                        card_placed = 1;
                    }
                } else {
                    cancel = 0;
                }
                break;
            default:
                cancel = 0;
                break;
        }
    }

    return card_placed;
}

int main(int argc, char *argv[])
{
    Setup *setup;
    std::vector<Card*> hand;
    int quit_game = 0;
    int card_placed;
    int cards_placed;
    char key;
    // int num = -1;
    int input;
    // char num_str[3];

    setup = init_setup();
    hand = std::vector<Card*>();

    init_terminal();

    while (!quit_game) {
        display(setup, &hand);
        key = getch();
        switch (key) {
            case 'Q':
                quit_game = 1;
                break;
            case 'd':
                setup->stock_replace(&hand);
                setup->stock_draw(&hand);
                break;
            case 'D':
                card_placed = 0;
                if (hand.size() > 0) {
                    card_placed = place_from_hand(setup, hand[hand.size() - 1]);
                }

                if (card_placed) {
                    hand.pop_back();
                }
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
                if (setup->tableu_columns[0][key - '0' - 1] != 0) {
                    input = -1;
                    move(15, 0);
                    clrtoeol();
                    move(16, 0);
                    clrtoeol();
                    mvprintw(15, 0, "Enter the number of cards to move or 0 to cancel: ");
                    refresh();
                    echo();
                    while (input < 0 || input > 13) {
                        scanw((char *)"%d", &input);
                        noecho();
                        if (input == 0) {
                            break;
                        } else if (input > 0 && input < 14) {
                            cards_placed = move_pile(setup, key - '0' - 1, input);
                            if (cards_placed) {
                                break;
                            } else {
                                input = -1;
                            }
                        }
                    }
                } else {
                    place_from_column(setup, key - '0' - 1);
                }
                break;
            default:
                quit_game = 0;
                break;
        }
    }
 
    return 0;
}
