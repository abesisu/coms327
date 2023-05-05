#include "card.h"

char Card::get_rank_char()
{
    char rank_char;

    switch (rank) {
        case 1:
            rank_char = 'A';
            break;
        case 10:
            rank_char = 'T';
            break;
        case 11:
            rank_char = 'J';
            break;
        case 12:
            rank_char = 'Q';
            break;
        case 13:
            rank_char = 'K';
            break;
        default:
            rank_char = static_cast<char>(rank + '0');
            break;
    }

    return rank_char;
}

char Card::get_suit_char()
{
    char suit_char;

    switch (suit) {
        case 0:
            suit_char = 'H';
            break;
        case 1:
            suit_char = 'D';
            break;
        case 2:
            suit_char = 'C';
            break;
        case 3:
            suit_char = 'S';
            break;
    }

    return suit_char;
}
