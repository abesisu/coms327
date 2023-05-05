#include <queue>
#include <stack>
#include "deck.h"

class Setup {
public:
    std::stack<Card*> foundation[4];
    std::stack<Card*> tableu[7];
    Card *tableu_columns[13][7];
    std::queue<Card*> stock;

    Setup(Deck *deck);
    int stack_foundation(Card *card, int foundation);
    int stack_column(Card *card, int column);
    int stock_draw(std::vector<Card*> *hand);
    int stock_replace(std::vector<Card*> *hand);
};
