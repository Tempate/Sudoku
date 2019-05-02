package main

import ("fmt";"math";"strconv";"time")

/*------------------------CONST------------------------------*/

const S int = 9
const R int = 3
const ALL Value = (1 << uint(S)) - 1

type Value uint

type Sudoku struct{
	board [S*S]Value
	colsAv [S]Value
	rowsAv [S]Value
	sqrsAv [S]Value
	remeaning []int
}


/*-------------------------GLOBAL----------------------------*/

var forcedChanges int
var updateAllCalls int

//Sudokus, they are initialized but only one is solved
var solvedS [S][S]Value //Testing
var solvedP [S*S]Value //Testing
var sudoku [S][S]Value
var sudokuHard [S][S]Value
var tempate [S][S]Value


/*---------------------INITIALIZATION------------------------*/

func getSudoku(){
	sudoku = [S][S]Value{
		{0, 0, 3,  0, 2, 9,  6, 0, 8},
		{6, 0, 0,  8, 0, 0,  0, 9, 0},
		{0, 9, 5,  4, 0, 6,  0, 0, 7}, //Added 9, 5
		{1, 6, 0,  9, 3, 0,  8, 0, 4}, //Added 1, 6
		{4, 0, 7,  0, 8, 0,  9, 0, 6},
		{9, 0, 8,  0, 4, 2,  0, 0, 0},
		{3, 0, 0,  2, 0, 4,  0, 6, 0}, //Added 6
		{0, 7, 0,  0, 0, 1,  0, 0, 5},
		{5, 0, 9,  7, 6, 0,  3, 1, 2}} //Added 1, 2

	sudokuHard = [S][S]Value{
		{0, 0, 3,  0, 2, 9,  6, 0, 8},
		{6, 0, 0,  8, 0, 0,  0, 9, 0},
		{0, 0, 0,  4, 0, 6,  0, 0, 7},
		{0, 0, 0,  9, 3, 0,  8, 0, 4},
		{4, 0, 7,  0, 8, 0,  9, 0, 6},
		{9, 0, 8,  0, 4, 2,  0, 0, 0},
		{3, 0, 0,  2, 0, 4,  0, 0, 0},
		{0, 7, 0,  0, 0, 1,  0, 0, 5},
		{5, 0, 9,  7, 6, 0,  3, 0, 0}}


	solvedS = [S][S]Value{
		{7, 1, 3,  5, 2, 9,  6, 4, 8},
		{6, 2, 4,  8, 7, 3,  5, 9, 1},
		{8, 9, 5,  4, 1, 6,  2, 3, 7},
		{1, 6, 2,  9, 3, 7,  8, 5, 4},
		{4, 3, 7,  1, 8, 5,  9, 2, 6},
		{9, 5, 8,  6, 4, 2,  1, 7, 3},
		{3, 8, 1,  2, 5, 4,  7, 6, 9},
		{2, 7, 6,  3, 9, 1,  4, 8, 5},
		{5, 4, 9,  7, 6, 8,  3, 1, 2}}

	tempate = [S][S]Value{
		{0, 0, 0,  2, 0, 0,  0, 0, 0},
		{0, 0, 0,  0, 0, 8,  4, 0, 0},
		{0, 7, 0,  4, 0, 0,  8, 9, 0},
		{0, 0, 0,  0, 5, 0,  0, 1, 0},
		{0, 1, 0,  0, 0, 0,  0, 0, 0},
		{0, 0, 3,  0, 0, 6,  0, 0, 2},
		{0, 0, 0,  0, 0, 5,  7, 0, 8},
		{8, 0, 0,  0, 3, 0,  0, 0, 0},
		{0, 6, 1,  0, 0, 0,  0, 4, 0}}
}

func genSud() (s Sudoku){
	getSudoku()
	s.remeaning = make([]int, 0)
	for i, l := range tempate{ //Change this to change the sudoku
		for j, e := range l{
			if e == 0{
				s.board[index(i, j)] = 0
				s.remeaning = append(s.remeaning, index(i, j))
			}else{
				s.board[index(i, j)] = 1 << uint(e - 1)
			}
		}
	}

	return s
}

/*---------------------HELPER------------------------*/

func getVal(v Value) int{
	if v == 0{
		return 0
	}else{
		return 1 + int(math.Log2(float64(v)))
	}
}

func bin(v Value) string{
	return strconv.FormatInt(int64(v), 2)
}

//I think this functions are inlined automagically
func index(i, j int) int{
	return S*i + j
}
func coord(i int) (int, int){
	return i / S, i % S
}
func getSqrIndex(i, j int) int{
	return R*(i/R) + (j/R)
}
func isPow2(v Value) bool{
	return v & (v-1) == 0 && v != 0
}
func invert(v Value) Value{
	return ALL ^ v
}


/*---------------------CHECKING------------------------*/

func (s Sudoku) printSudoku(){
	fmt.Println("--------------------")
	for i := 0; i < S; i++ {
		for j := 0; j < S; j++ {

			if s.board[index(i, j)] == 0{
				fmt.Print("- ")
			}else{
				fmt.Print(getVal(s.board[index(i, j)]), " ")
			}

			if j % R == R - 1{
				fmt.Print(" ")
			}
		}

		fmt.Println("")
		
		if i % R == R - 1 && i != S - 1{
			fmt.Println("")
		}
	}

	fmt.Println("--------------------")
}

func (s Sudoku) usedSquares() int{
	//Counts the number of squares that have been filled
	t := 0
	for i := 0; i < S*S; i++ {
		if s.board[i] != 0{
			t++
		}
	}
	return t
}

//TODO: Improve this
func (s Sudoku) verifySudoku() bool{
	var sum Value
	if s.usedSquares() != S*S{
		return false
	}
	//Check rows
	for i := 0; i < S; i++ {
		sum = 0
		for j := 0; j < S; j++ {
			sum += s.board[index(i, j)]
		}
		if sum != ALL{
			return false
		}
	}
	//Check cols
	for i := 0; i < S; i++ {
		sum = 0
		for j := 0; j < S; j++ {
			sum += s.board[index(j, i)]
		}
		if sum != ALL{
			return false
		}
	}
	//Check sqrs
	for x := 0; x < R; x++ {
		for y := 0; y < R; y++ {
			sum = 0
			for i := 0; i < R; i++ {
				for j := 0; j < R; j++ {
					sum += s.board[index(R*x + i, R*y + j)]		
				}
			}
			if sum != ALL{
				return false
			}
		}
	}
	return true
}

/*--------------------------SOLVER-----------------------------*/

func (s Sudoku) canBeFinished() bool{
	for _, index := range s.remeaning{
		if s.possible(index) == 0{
			return false
		}
	}

	return true
}

func (s Sudoku) possible(index int) Value{
	i, j := coord(index) 
	return s.rowsAv[i] & s.colsAv[j] & s.sqrsAv[getSqrIndex(i, j)]
}

func allPossible(v Value) []Value{
	ls := make([]Value, 0)
	for i := 0; i < S; i++ {
		if ((1<<uint(i)) & v) != 0{
			ls = append(ls, (1<<uint(i)))
		}
	}
	return ls
}

func (s Sudoku) updateAll() Sudoku{
	var tempRows, tempCols, tempSqrs [S]Value
	var sqrIndex int
	var val Value
	updateAllCalls++

	for i := 0; i < S; i++ {
		for j := 0; j < S; j++ {
			val = s.board[index(i, j)]
			sqrIndex = getSqrIndex(i, j)
			tempRows[i] |= val
			tempCols[j] |= val
			tempSqrs[sqrIndex] |= val
		}
	}

	for i := 0; i < S; i++ {
		s.rowsAv[i] = invert(tempRows[i])
		s.colsAv[i] = invert(tempCols[i])
		s.sqrsAv[i] = invert(tempSqrs[i])
	}

	return s
}

//TODO: Make this shbang work
func (s Sudoku) updateOne(index int){
	i, j := coord(index)
	sqrIndex := getSqrIndex(i, j)
	mask := invert(s.board[index])

	s.rowsAv[i] &= mask
	s.colsAv[j] &= mask
	s.sqrsAv[sqrIndex] &= mask
}

func (s Sudoku) setForced() (bool, Sudoku){
	var i, j, sqrIndex int
	var available, mask Value
	var updated bool
	nextRem := make([]int, 0)


	for _, val := range s.remeaning{
		i, j = coord(val)
		sqrIndex = getSqrIndex(i, j)
		available = s.rowsAv[i] & s.colsAv[j] & s.sqrsAv[sqrIndex]

		if isPow2(available){
			s.board[val] = available
			mask = invert(available)
			
			s.rowsAv[i] &= mask
			s.colsAv[j] &= mask
			s.sqrsAv[sqrIndex] &= mask

			updated = true
			forcedChanges++ //DEBUG

		}else{
			nextRem = append(nextRem, val)
		}
	}

	s.remeaning = nextRem
	return updated, s
}

func (s Sudoku) setAllForced() (Sudoku){
	lastUpdate := true
	
	s = s.updateAll()

	for lastUpdate{
		lastUpdate, s = s.setForced()
	}
	
	return s
}


func (s Sudoku) solve() (bool, Sudoku){
	s = s.setAllForced()
	if len(s.remeaning) == 0{
		return true, s
	}else{
		//removing at len - 1 reduces the calls to 50, making it super fast (3ms)
		index := s.remeaning[len(s.remeaning) - 1]
		s.remeaning = s.remeaning[:len(s.remeaning) - 1]
		//This one should be as fast, but isnt for this sudoku (50ms)
		/*
		index := s.remeaning[0]
		s.remeaning = s.remeaning[1:]
		*/


		for _, val := range allPossible(s.possible(index)) {
			newS := Sudoku(s)
			newS.board[index] = Value(val)
			//newS.updateOne(index) //TODO: Make this work if possible
			
			if newS.canBeFinished(){
				pos, newS := newS.solve()
				if pos{
					return true, newS
				}
			}
		}
	}

	return false, Sudoku{}
}



/*--------------------------MAIN-----------------------------*/

func main(){
	s := genSud()
	s.printSudoku()
	fmt.Println(s.usedSquares())
	
	start := time.Now()
	//s.rowsAv, s.colsAv, s.sqrsAv = s.updateAll() //TODO: if updateOne works, this should be the only updateAll
	_, s = s.solve()
	end := time.Now()

	s.printSudoku()
	fmt.Println(s.usedSquares() == S*S && s.verifySudoku())

	fmt.Println("\nupdateAll calls:", updateAllCalls)
	fmt.Println("Total changes:", forcedChanges)
	fmt.Println("\nTime:", end.Sub(start))
}