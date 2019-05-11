# Sudoku
Sudoku-solving algorithm implemented in different languages to observe the difference among them and, specially, the performance.

O(_n_ ^_m_) algorithm, where _n_ is the number of empty tiles and _m_ the possible values (In this case, [1, 9]). Sudokus are an NP-Complete problem.

### Use
_Requires go, rustc, gcc, python2/3, and Cython. Other compilers may yield different results_


Place the files in a directory "dir"

$ `go install "dir/GO/Sudoku.go"  #Binary should be placed in GOBIN`

$ `rustc -O "dir/Rust/Sudoku.rs"  #Run the resulting binary file in $pwd`

$ `gcc -O3 -std=c11 -faggressive-loop-optimizations "dir/C/Sudoku.c" #Run the resulting binary in $pwd`

$ `python3 "dir/Python/sudoku.py" `

$ `python2 "dir/Python/compiler.py"  #Requires Cython, check the official website`


### Results
(In my computer, solving the 17-clue sudoku which is hard coded, the proportions should stay the same)

  * **GO**     -> 430ms, 430%. 
  * **Rust**   -> 100ms, 100%
  * **C**      -> 100ms, 100%
  * **Python** -> 9.8s, 9800%
  * **Cython** -> 1.2s, 1200%


### Algorithm
  Note that in order to have a unique solution (proper sudoku), a sudoku has to have at least 17 clues.

  The possible values for each square are the intersection of the possible values in its row / col / sqr

  1. Generate 3 arrays with the possible values for each row / col / square. (An empty row should have all possible values and a full one, zero)

  2. Set all values which are forced (only have 1 possibility)

  3. If there are no remeaning empty tiles: END

  4. Else: Select a tile and, for each value, try to solve the sudoku by making a duplicates and assigning one of the possible values, goto (2.)