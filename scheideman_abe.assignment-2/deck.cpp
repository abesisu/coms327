#include <cstdlib>

#include "deck.h"

Deck::Deck() 
{
    int i, j;
    cards = std::vector<Card*>();
    
    for (i = 0; i < 4; i++) {
        for (j = 1; j < 14; j++) {
            cards.push_back(new Card(i, j));
        }
    }
}

Card *Deck::draw_card() 
{
    if (cards.empty()) {
        return 0;
    }
    Card* top_card = cards[0];
    cards.erase(cards.begin());
    return top_card;
}

void Deck::shuffle() {
    int i, j, n;
    srand(time(0));

    n = cards.size();

    for (i = n-1; i > 0; i--) {
        j = rand() % (i+1);
        Card* temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
    }
}
