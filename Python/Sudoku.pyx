cdef int R = 3
cdef int S = 9
cdef int SS = S*S
cdef int ALL = (1 << S) - 1

"""
Cython is a super-language of python, it compiles the code to C and then compiles
the C code, getting much better performance than normal Python code. It can also 
be statically typed.
In order to run the program pyximport has to be installed as a lib and the code
has to be imported from a python script (compiler.py)
"""

cdef class Sudoku(object):
    """This is the main class of the program.
    It stores the board, a list with all possible values for each
    row / col / sqr and a list of the remaning indices in the board.
    It stores each value as a power of 2, the posibles values are just
    |s of all the possibilities"""
    cdef public list board, rows, cols, sqrs, remeaning

    def __init__(self, list _board, list  _rows, list _cols, list _sqrs, list remeaning=None):
        self.board = _board
        self.rows = _rows
        self.cols = _cols
        self.sqrs = _sqrs

        if remeaning:
            self.remeaning = remeaning
        else:
            self.remeaning = [i for i in range(SS) if self.board[i] == 0]

    def __repr__(self):
        """
        String representation of the sudoku
        """
        cdef str s = "--------------------\n"
        cdef str s_small
        cdef int i
        cdef int j
        for i in range(S):
            s_small = ""
            for j in range(S):
                if self.board[index(i, j)] == 0:
                    s_small += "- "
                else:
                    s_small += str(log2(self.board[index(i, j)])) + ' '

                if j % R == R - 1:
                    s_small += ' '
            s += s_small + '\n'
            if i % R == R - 1 and i != S - 1:
                s += '\n'

        return s

    def duplicate(self):
        return Sudoku(list(self.board), list(self.rows), list(self.cols), list(self.sqrs), list(self.remeaning))

    def possible(self, int index):
        """
        Returns the possible values for a tile
        """
        cdef int i, j
        i, j = coord(index)
        return self.rows[i] & self.cols[j] & self.sqrs[get_sqr_index(i, j)]

    def update_one(self, int index):
        """
        Removes the value in board[index] from the possible in its row / col / sqr
        """
        cdef int i, j
        i, j = coord(index)
        self.rows[i] &= ALL ^ self.board[index]
        self.cols[j] &= ALL ^ self.board[index]
        self.sqrs[get_sqr_index(i, j)] &= ALL ^ self.board[index]

    cdef int set_forced(self):
        """
        Sets all the tiles which have 1 possible value in one iteration through the board
        Returns:
           0 if there were no changes
           1 if at least one tile was updated
           2 if the board is unsolvable        
        """
        cdef list next_rem = []
        cdef int updated = 0
        cdef int index, i, j, available, mask

        for index in self.remeaning:
            i, j = coord(index)
            available = self.rows[i] & self.cols[j] & self.sqrs[get_sqr_index(i, j)]
            
            if available == 0:
                return 2

            if is_pow_2(available) == 1:
                mask = ALL ^ available
                
                self.board[index] = available
                self.rows[i] &= mask
                self.cols[j] &= mask
                self.sqrs[get_sqr_index(i, j)] &= mask

                updated = 1
            else:
                next_rem.append(index)

        if updated == 1:
            self.remeaning = next_rem

        return updated

    def set_all_forced(self):
        """
        Calls set_forced until it stops making changes or the board is unsolvable
        """
        return self.c_set_all_forced()
        
    cdef int c_set_all_forced(self):
        cdef int last_update = 1
        while last_update == 1:
            last_update = self.set_forced()
            if last_update == 2:
                return 0
        return 1

    def is_finished(self):
        """
        Returns True if all the tiles are set, False otherwise
        """
        cdef int i
        for i in self.board:
            if i == 0:
                return False
        return True

cdef Sudoku generate_sudoku():
    """
    Generates the sudoku, so far it is hard coded.
    It also sets all the starting possible values
    """
    cdef list board = list(map(get_pow, 
        [0, 2, 4,  0, 0, 0,  0, 0, 0, 
        0, 0, 0,  0, 0, 7,  1, 0, 0,
        0, 9, 0,  0, 0, 0,  0, 0, 0,
        0, 0, 0,  0, 0, 0,  0, 8, 4,
        0, 0, 0,  0, 7, 5,  0, 0, 0,
        6, 0, 0,  0, 3, 0,  0, 0, 0,
        0, 0, 0,  4, 0, 0,  0, 2, 9,
        0, 0, 0,  2, 0, 0,  3, 0, 0,
        1, 0, 0,  0, 0, 0,  0, 0, 0])) #17
    """
    cdef list board = list(map(get_pow, 
        [0, 0, 3,  0, 2, 9,  6, 0, 8,
        6, 0, 0,  8, 0, 0,  0, 9, 0,
        0, 9, 5,  4, 0, 6,  0, 0, 7,
        1, 6, 0,  9, 3, 0,  8, 0, 4,
        4, 0, 7,  0, 8, 0,  9, 0, 6,
        9, 0, 8,  0, 4, 2,  0, 0, 0,
        3, 0, 0,  2, 0, 4,  0, 6, 0,
        0, 7, 0,  0, 0, 1,  0, 0, 5,
        5, 0, 9,  7, 6, 0,  3, 1, 2]))
    """


    cdef list rows = [ALL] * S
    cdef list cols = [ALL] * S
    cdef list sqrs = [ALL] * S

    cdef int i, j, val
    for i in range(S):
        for j in range(S):
            val = board[index(i, j)]
            rows[i] ^= val
            cols[j] ^= val
            sqrs[get_sqr_index(i, j)] ^= val

    return Sudoku(board, rows, cols, sqrs)

cdef int log2(int n):
    """
    Returns 1 + log2(n) or 0
    """
    if n == 0:
        return 0
    cdef int log = 0
    while n != 1:
        log += 1
        n >>= 1

    return 1 + log

cdef inline int get_pow(int n):
    return 0 if n == 0 else 1 << (n - 1)

cdef inline int index(int i, int j):
    return S * i + j

cdef inline tuple coord(int i):
    return i // S, i % S

cdef inline int get_sqr_index(int i, int j):
    return R*(i // R) + (j // R)

cdef inline int is_pow_2(int v):
    return (v & (v - 1)) == 0 and (v != 0)

cdef inline list ls_of_possible(int v):
    """
    Parses the possible values into a list of powers of 2
    """
    return [1<<i for i in range(S) if (v & (1 << i)) != 0]

cdef tuple solve(sud):    
    """
    Main algorithm, tries to set all the forced values
    When that is no longer possible, it branches and checks if
    the resulting sudokus are solvable, it tries to reach as deep as
    possible in each branch 
    """
    cdef int is_pos = sud.set_all_forced()

    if is_pos == 0:
        return False, None
    elif sud.is_finished():
        return True, sud

    cdef int index = sud.remeaning.pop()
    cdef int value
    cdef Sudoku new_sud


    for value in ls_of_possible(sud.possible(index)):
        new_sud = sud.duplicate()
        new_sud.board[index] = value
        new_sud.update_one(index)

        pos, new_sud = solve(new_sud)
        if pos:
            return True, new_sud

    return False, None

def main():
    cdef Sudoku sudoku
    sudoku = generate_sudoku()
    print(sudoku)
    _, sudoku = solve(sudoku)
    print(sudoku)


if __name__ == '__main__':
    main()