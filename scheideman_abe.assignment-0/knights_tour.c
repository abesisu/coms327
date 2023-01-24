#include <stdio.h>

// iterate through each array location and call a recursive function that prints all 
// possible knights tour paths
void populate_chess_board(int chess_board[5][5])
{
    int counter = 1;
    int i, j;

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            chess_board[i][j] = counter;
            counter++;
        }
    }
}

int has_knight_visited(int chess_board[5][5], int knights_tour[25], int row, int col) 
{
    int knight_visited = -1;
    int i;

    for (i = 0; i < 25; i++) {
        if (knights_tour[i] == chess_board[row][col]) {
            knight_visited = 0;
            break;
        }
    }

    return knight_visited;
}

int get_move_options(int chess_board[5][5], int knights_tour[25], int move_options[8], int row, int col)
{
    int knight_visited;
    int next_row, next_col;
    int num_options = 0;

        next_row = row - 1;
        next_col = col - 2;
        knight_visited = has_knight_visited(chess_board, knights_tour, next_row, next_col);
        if (next_row >= 0 && next_col >= 0 && knight_visited == -1) {
            move_options[num_options] = chess_board[next_row][next_col];
            num_options++;
        }

        next_row = row - 2;
        next_col = col - 1;
        knight_visited = has_knight_visited(chess_board, knights_tour, next_row, next_col);
        if (next_row >= 0 && next_col >= 0 && knight_visited == -1) {
            move_options[num_options] = chess_board[next_row][next_col];
            num_options++;
        }

        next_row = row - 2;
        next_col = col + 1;
        knight_visited = has_knight_visited(chess_board, knights_tour, next_row, next_col);
        if (next_row >= 0 && next_col <= 4 && knight_visited == -1) {
            move_options[num_options] = chess_board[next_row][next_col];
            num_options++;
        }

        next_row = row - 1;
        next_col = col + 2;
        knight_visited = has_knight_visited(chess_board, knights_tour, next_row, next_col);
        if (next_row >= 0 && next_col <= 4 && knight_visited == -1) {
            move_options[num_options] = chess_board[next_row][next_col];
            num_options++;
        }

        next_row = row + 1;
        next_col = col - 2;
        knight_visited = has_knight_visited(chess_board, knights_tour, next_row, next_col);
        if (next_row <= 4 && next_col >= 0 && knight_visited == -1) {
            move_options[num_options] = chess_board[next_row][next_col];
            num_options++;
        }

        next_row = row + 2;
        next_col = col - 1;
        knight_visited = has_knight_visited(chess_board, knights_tour, next_row, next_col);
        if (next_row <= 4 && next_col >= 0 && knight_visited == -1) {
            move_options[num_options] = chess_board[next_row][next_col];
            num_options++;
        }

        next_row = row + 2;
        next_col = col + 1;
        knight_visited = has_knight_visited(chess_board, knights_tour, next_row, next_col);
        if (next_row <= 4 && next_col <= 4 && knight_visited == -1) {
            move_options[num_options] = chess_board[next_row][next_col];
            num_options++;
        }

        next_row = row + 1;
        next_col = col + 2;
        knight_visited = has_knight_visited(chess_board, knights_tour, next_row, next_col);
        if (next_row <= 4 && next_col <= 4 && knight_visited == -1) {
            move_options[num_options] = chess_board[next_row][next_col];
            num_options++;
        }

    return num_options;
}

int check_knights_tour(int knights_tour[25]) 
{
    int tour_complete = 0;
    int i, j;
    for (i = 0; i < 25; i++) {
        if (knights_tour[i] < 0 || knights_tour[i] > 25) {
            tour_complete = -1;
            return tour_complete;
        }

        for (j = i + 1; j < 24; j++) {
            if (knights_tour[i] == knights_tour[j]) {
                tour_complete = -1;
                return tour_complete;
            }
        }
    }

    return tour_complete;
}

// recursive function that takes a board and an array of what has been visited
void print_knights_tours(int chess_board[5][5], int knights_tour[25], int row, int col, int count)
{
    int tour_complete;
    int next_row, next_col;
    int i;

    // need base case - when knights_tour is filled, print it...
    tour_complete = check_knights_tour(knights_tour);
    if (tour_complete == 0) {
        for (i = 0; i < 24; i++) {
            printf("%d,", knights_tour[i]);
        }
        printf("%d\n", knights_tour[24]);

        return;
    }  
    
    // otherwise, count how many options (checking each one with the knights_tour so far) there are then call knights tour that many times for each move option
    int move_options[8];
    int num_options = get_move_options(chess_board, knights_tour, move_options, row, col);

    if (num_options == 0) {
        return;
    }

    for (i = 0; i < num_options; i++) {
        knights_tour[count] = move_options[i];
        next_row = (move_options[i] - 1) / 5;
        next_col = (move_options[i] - 1) % 5;
        count++;
        
        print_knights_tours(chess_board, knights_tour, next_row, next_col, count);
        knights_tour[count] = -1;
        count--;
    }

    return;
}

int main(int argc, char *argv[])
{
    int chess_board[5][5];
    int knights_tour[25];
    int index;
    int count;
    int row, col;
    int i;

    populate_chess_board(chess_board);

    for (index = 1; index < 26; index++) {
        for (i = 0; i < 25; i++) {
            knights_tour[i] = -1;
        }
        knights_tour[0] = index;
        count = 1;

        row = (index - 1) / 5;
        col = (index - 1) % 5;

        print_knights_tours(chess_board, knights_tour, row, col, count);
    }
    
    return 0;
}
