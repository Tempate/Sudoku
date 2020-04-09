# Sudoku

This project is meant to be a common repository for the same application written in different languages. Its goal is mainly educative. The basic idea is to try to solve the same problem with the same algorithms, or as close as possible, in each language.

This would not only give us an idea on how all these different languages work, but also what are their strong suits and how they end up compared to one another.

### The problem to solve

Our choice of focus has been the game Sudoku. Checking whether a Sudoku has any errors, solving a Sudoku and generating a Sudoku. These features would be implemented in all the chosen languages.

### The algorithms used

The best way to solve a Sudoku is using Depth First Search, as the number of branches gets really small after setting a few tiles. This version has also been improved for the task at hand by setting all the forced values after a branch is chosen. It's understood by possible values those that only have one possibility. We have also seen performance increases when removing a branch if a tile has no possible value.

### The languages it's implemented in

The first implementation was done in C. But we are aiming to do so in many more. Here are some of them.

**Implemented**
- C
- Go
- Rust
- Python

**To be implemented**
- Java
- JavaScript
- Haskell
- Clisp
- Swift
- Ruby
- Perl
- Octave
