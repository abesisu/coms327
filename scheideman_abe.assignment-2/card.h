class Card {
private:
    int suit;
    int rank;
public:
    Card(int s, int r) 
    {
        suit = s;
        rank = r;
    }
    int get_suit() { return suit; }
    char get_suit_char();
    int get_rank() { return rank; }
    char get_rank_char();
    int is_red() { return suit < 2; }
};
