#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define R 3
#define S 9
#define ALL (1<<S) - 1


/*TO RUN:
 * Call with cmd
 * $ gcc -O3 -std=c11 -faggressive-loop-optimizations <file> -o <output file>.out
 * $ <output file>.out
 */

/*--------------------------INLINES--------------------------*/

static inline int POS(int x) { return 1 << (x-1); }
static inline int POW2(char pow) { return 1 << pow; }
static inline int GET_SQR(int i, int j) { return R * (int)(i/R) + (int)(j/R); }
static inline unsigned char GET_X(unsigned short c) { return (c) & 0xf; }
static inline unsigned char GET_Y(unsigned short c) { return ((c) & 0xf0) >> 4; }
static inline unsigned char GET_CRD(int i, int j) { return (i << 4) + j; }
static inline bool VAL_IN_BYTE(unsigned short byte, int k) { return (byte & (1<<k)) != 0; } 

/*------------------------PROTOTYPES------------------------*/

typedef struct {
    unsigned char values[S][S];
    
    unsigned short rowsPos[S];
    unsigned short colsPos[S];
    unsigned short sqrsPos[S];
} Board;

Board generateSudoku();
void printBoard(Board board);

void solve(Board *board);

bool dfs(Board *board, unsigned char index);
char setAllForced(Board *board, unsigned char min);

void calculatePossible(Board *board);
void updateTileAdded(Board *board, const char y, const char x);

unsigned char getNextPossibleValue(Board board, const char val, const char x, const char y);

unsigned char log_2(unsigned short index);

bool checkBoard(Board board);
bool completeBoard(Board board);
bool finishedBoard(Board board);

/*--------------------------GLOBAL--------------------------*/

unsigned char blanks[S*S];
unsigned char blanks_size = 0;

/*--------------------------BOARD---------------------------*/
Board generateSudoku(){
    int values[9][9] = {
        {0,2,4, 0,0,0, 0,0,0},
        {0,0,0, 0,0,7, 1,0,0},
        {0,9,0, 0,0,0, 0,0,0},

        {0,0,0, 0,0,0, 0,8,4},
        {0,0,0, 0,7,5, 0,0,0},
        {6,0,0, 0,3,0, 0,0,0},
        
        {0,0,0, 4,0,0, 0,2,9},
        {0,0,0, 2,0,0, 3,0,0},
        {1,0,0, 0,0,0, 0,0,0}
    };
    
    Board board = (Board) {
        .rowsPos = {0},
        .colsPos = {0},
        .sqrsPos = {0}
    };
    
    for (int i = 0; i < S; i++) {
        for (int j = 0; j < S; j++) {
            board.values[i][j] = values[i][j];
            if (values[i][j] == 0){
                blanks[blanks_size++] = GET_CRD(i, j);
            }
        }
    }
    
    return board;
}

void printBoard(Board board) {
    printf("Board values: \n");
    printf("---------------------------\n");
    
    for (int i = 0; i < S; i++) {
        for (int j = 0; j < S; j++) {
            if (board.values[i][j] == 0) {
                printf(" - ");
            } else {
                printf(" %hhu ", board.values[i][j]);
            }
        }
        printf("\n");
    }
    
    printf("---------------------------\n");
}

/*--------------------------CHECK---------------------------*/

bool checkBoard(Board board) {
    int pos, row, col, sqr;
    
    // Checks to see if all rows match
    for (int i = 0; i < S; i++) {
        row = 0;
        for (int j = 0; j < S; j++) {
            if (board.values[i][j] != 0) {
                pos = POS(board.values[i][j]);
                if ((pos & row) != 0) return false;
                row |= pos;
            }
        }
    }
    
    // Checks to see if all columns match
    for (int i = 0; i < S; i++) {
        col = 0;
        for (int j = 0; j < S; j++) {
            if (board.values[j][i] != 0) {
                pos = POS(board.values[j][i]);
                if ((pos & col) != 0) return false;
                col |= pos;
            }
        }
    }
    
    // Checks to see if all squares match
    for (int i = 0; i < S; i += R) {
        for (int j = 0; j < S / R; j += R) {
            sqr = 0;
            for (int k = i; k < i + R; k++) {
                for (int l = j; l < j + R; l++) {
                    if (board.values[k][l] != 0) {
                        pos = POS(board.values[k][l]);
                        if ((pos & sqr) != 0) return false;
                        sqr |= pos;
                    }
                }
            }
        }
    }
    
    return true;
}

bool completeBoard(Board board) {
    for (int i = 0; i < S; i++) {
        for (int j = 0; j < S; j++) {
            if (board.values[i][j] <= 0 || board.values[i][j] > S) return false;
        }
    }
    
    return true;
}

bool finishedBoard(Board board) {
    return completeBoard(board) && checkBoard(board);
}

/*--------------------------SOLVER--------------------------*/

bool dfs(Board *board, unsigned char index) {
    Board new_board;
    unsigned char x, y, v;
    unsigned char state, new_index;
    
    // If all values are set, the recursion is finished
    if (index >= blanks_size) return true;
    
    x = GET_X(blanks[index]);
    y = GET_Y(blanks[index]);
    
    // Goes to the next tile if the current one is already set
    if (board->values[y][x] != 0)
        return dfs(board, index+1);
    
    v = getNextPossibleValue(*board, board->values[y][x], x, y);
    
    while (v != 0) {
        // Generates a new copy of the board to improve performance
        new_board = *board;
        new_board.values[y][x] = v;
        new_index = index + 1;
        
        // Removes the picked value from the possible ones
        updateTileAdded(&new_board, y, x);
        
        // Sets all the possible forced tiles
        do {
            state = setAllForced(&new_board, new_index);
        } while (state == 1);
        
        
        if (state != 2 && dfs(&new_board, new_index)) {
            *board = new_board;
            return true;
        }

        v = getNextPossibleValue(*board, v, x, y);
    }

    // There was no possible value to be chosen
    return false;
}

/* Calculates all the possible values for each row, column, and square
 */
void calculatePossible(Board *board) {
    unsigned short pos;
    unsigned char m;
    
    // Goes tile by tile and saves all the found values
    for (char i = 0; i < S; i++) {
        for (char j = 0; j < S; j++) {
            if (board->values[i][j] != 0) {
                m = GET_SQR(i,j);
                pos = POS(board->values[i][j]);
                
                board->rowsPos[i] |= pos;
                board->colsPos[j] |= pos;
                board->sqrsPos[m] |= pos; 
            }
        }
    }
    
    for (char k = 0; k < S; k++) {
        board->rowsPos[k] ^= ALL;
        board->colsPos[k] ^= ALL;
        board->sqrsPos[k] ^= ALL;
    }
}


/* Updates the possible values for the row, column, and square of the 
 * tile that was changed.
 */
void updateTileAdded(Board *board, const char y, const char x) {
    unsigned short val = ALL ^ POS(board->values[y][x]);
    unsigned char m = GET_SQR(y,x);
    
    board->rowsPos[y] &= val;
    board->colsPos[x] &= val;
    board->sqrsPos[m] &= val;
}


/* Takes a tile with a value and returns its next possible value.
 * If none, it returns 0.
 */
unsigned char getNextPossibleValue(Board board, const char val, const char x, const char y) {
    unsigned char m = GET_SQR(y, x);
    unsigned short possible = board.rowsPos[y] & board.colsPos[x] & board.sqrsPos[m];
    
    for (char k = val; k < S; k++) {
        if (VAL_IN_BYTE(possible, k))
            return k + 1;
    }
    
    return 0;
}

/* Sets all the tiles that only have one possible value.
 * 
 * Returns:
 *  1 if a modification was made
 *  2 if there is a tile with no possible value
 *  any other value if no modifications were made
 */
char setAllForced(Board *board, unsigned char min) {
    unsigned char x, y, m;
    unsigned short possible;
    char state = 0;

    for (char i = min; i < blanks_size && state != 2; i++) {
        x = GET_X(blanks[i]);
        y = GET_Y(blanks[i]);
        m = GET_SQR(y, x);
        possible = board->rowsPos[y] & board->colsPos[x] & board->sqrsPos[m];
        
        if (board->values[y][x] == 0) {
            state = 2 - __builtin_popcount(possible);
            
            if (state == 1) {
                board->values[y][x] = log_2(possible)+1;
                updateTileAdded(board, y, x);
            }
        }
    }
    
    return state;
}

unsigned char log_2(unsigned short index) {
    unsigned char val = 0;
    while (index >>= 1) ++val;
    return val;
}


void solve(Board *board) {
    calculatePossible(board);
    
    dfs(board, 0);
}


int main(int argc, char const *argv[])
{
    Board b = generateSudoku();
    printBoard(b);


    time_t start = clock();
    
    solve(&b);

    time_t end = clock();


    printBoard(b);
    printf("Is valid: "); printf((finishedBoard(b))? "true\n" : "false\n");
    printf("Time: %ldms\n", (end - start)/1000);

    return (EXIT_SUCCESS);
}