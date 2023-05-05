#include "setup.h"

Setup::Setup(Deck *deck)
{
    int i, j;

    // initialize foundation
    for (i = 0; i < 4; i++) {
        foundation[i] = std::stack<Card*>();
    }

    // initialize tableu
    for (i = 0; i < 7; i++) {
        tableu[i] = std::stack<Card*>();
        for (j = 0; j < i + 1; j++) {
            tableu[i].push(deck->draw_card());
        }
    }

    // initialize tableu columns
    for (i = 0; i < 13; i++) {
        for (j = 0; j < 7; j++) {
            tableu_columns[i][j] = 0;
        }
    }

    // initialize stock
    stock = std::queue<Card*>();
    while (deck->size() > 0) {
        stock.push(deck->draw_card());
    }
}

int Setup::stack_foundation(Card *card, int pile)
{
    if (pile < 0 || pile > 3) {
        return 0;
    }

    if (card->get_suit() != pile) {
        return 0;
    }

    if (foundation[pile].empty() && card->get_rank() != 1) {
        return 0;
    }

    if (foundation[pile].size() >= 13) {
        return 0;
    }

    if (!foundation[pile].empty() && card->get_rank() != foundation[pile].top()->get_rank() + 1) {
        return 0;
    }

    foundation[pile].push(card);

    return 1;
}

int Setup::stack_column(Card *card, int column)
{
    int i;

    if (column < 0 || column > 6) {
        return 0;
    }

    if (tableu[column].empty() && card->get_rank() != 13) {
        return 0;
    }

    i = 0;
    while (tableu_columns[i][column] != 0 && i < 13) {
        i++;
    }

    if (i >= 13) {
        return 0;
    }

    if (i == 0 && tableu[column].empty()) {
        tableu[column].push(card);
        return 1;
    }

    if (i == 0 && card->get_rank() != tableu[column].top()->get_rank() - 1) {
        return 0;
    } else if (i > 0 && card->get_rank() != tableu_columns[i-1][column]->get_rank() - 1) {
        return 0;
    }

    if (i == 0 && card->is_red() == tableu[column].top()->is_red()) {
        return 0;
    } else if (i > 0 && card->is_red() == tableu_columns[i-1][column]->is_red()) {
        return 0;
    }

    tableu_columns[i][column] = card;

    return 1;
}

int Setup::stock_draw(std::vector<Card*> *hand)
{
    int i;

    if (stock.empty()) {
        return -1;
    }

    for (i = 2; i > -1; i--) {
        if (stock.empty()) {
            break;
        }
        hand->push_back(stock.front());
        stock.pop();
    }

    return 0;
}

int Setup::stock_replace(std::vector<Card*> *hand)
{
    int i;

    if (stock.size() > 24) {
        return -1;
    }

    for (i = 0; i < hand->size(); i++) {
        stock.push((*hand)[i]);
    }

    while (i > 0) {
        hand->pop_back();
        i--;
    }

    return 0;
}
