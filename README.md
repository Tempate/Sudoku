# Sudoku
Sudoku-solving algorithm implemented in _GO_ / _Rust_ / _C_ to observe the difference between the languages and, specially, the performance.

O(_n_ ^_m_) algorithm, where _n_ is the number of empty tiles and _m_ the possible values (In this case, [1, 9]). Sudokus are an NP-Complete problem.

### Use
_Requires go, rustc, and gcc. Other compilers may yield different results_


Place the files in a directory "dir"

$ `go install "dir/Sudoku.go"  #Binary should be placed in GOBIN`

$ `rustc -O "dir/Sudoku.rs"    #Run the resulting binary file in $pwd`

$ `gcc -O3 -std=c11 -faggressive-loop-optimizations "dir/Sudoku.c" #Run the resulting binary in $pwd`


### Results
(In my computer, solving the 17-clue sudoku which is hard coded, the proportions should stay the same)

  * **GO**    -> 430ms, 430%. 
  * **Rust**  -> 100ms, 100%
  * **C**     -> 100ms, 100%


### Algorithm
  Note that in order to have a unique solution (proper sudoku), a sudoku has to have at least 17 clues.
  
  The possible values for each square are the intersection of the possible values in its row / col / sqr
  
  1. Generate 3 arrays with the possible values for each row / col / square. (An empty row should have all possible values and a full one, zero)
  
  2. Set all values which are forced (only have 1 possibility)
  
  3. If there are no remeaning empty tiles: END
  
  4. Else: Select a tile and, for each value, try to solve the sudoku by making a duplicates and assigning one of the possible values, goto (2.)
  
