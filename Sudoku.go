package main

import ("fmt";"strconv";"time")

/*------------------------CONST------------------------------*/

const S int = 9
const SS int = S*S
const R int = 3
const ALL Value = (1 << uint(S)) - 1

type Value uint

type Sudoku struct{
	board [SS]Value
	colsAv [S]Value
	rowsAv [S]Value
	sqrsAv [S]Value
	remeaning []int
}

/*-------------------------GLOBAL----------------------------*/

var forcedChanges int
var calls int


/*---------------------INITIALIZATION------------------------*/

func initializeSudokus(toUse int) [S][S]Value{
	switch toUse{
	
	case 0:
		return [S][S]Value{
		{0, 0, 3,  0, 2, 9,  6, 0, 8},
		{6, 0, 0,  8, 0, 0,  0, 9, 0},
		{0, 9, 5,  4, 0, 6,  0, 0, 7},
		{1, 6, 0,  9, 3, 0,  8, 0, 4},
		{4, 0, 7,  0, 8, 0,  9, 0, 6},
		{9, 0, 8,  0, 4, 2,  0, 0, 0},
		{3, 0, 0,  2, 0, 4,  0, 6, 0},
		{0, 7, 0,  0, 0, 1,  0, 0, 5},
		{5, 0, 9,  7, 6, 0,  3, 1, 2}} //42 w/o branching 

	case 1:
		return [S][S]Value{
		{0, 0, 3,  0, 2, 9,  6, 0, 8},
		{6, 0, 0,  8, 0, 0,  0, 9, 0},
		{0, 0, 0,  4, 0, 6,  0, 0, 7},
		{0, 0, 0,  9, 3, 0,  8, 0, 4},
		{4, 0, 7,  0, 8, 0,  9, 0, 6},
		{9, 0, 8,  0, 4, 2,  0, 0, 0},
		{3, 0, 0,  2, 0, 4,  0, 0, 0},
		{0, 7, 0,  0, 0, 1,  0, 0, 5},
		{5, 0, 9,  7, 6, 0,  3, 0, 0}} //35

	case 2:
		return [S][S]Value{
		{0, 0, 7,  4, 8, 0,  9, 0, 0},
		{4, 6, 0,  0, 0, 0,  0, 0, 0},
		{0, 0, 0,  0, 0, 1,  0, 5, 8},
		{0, 9, 0,  5, 4, 0,  6, 0, 0},
		{0, 0, 0,  0, 0, 0,  0, 0, 0},
		{0, 0, 4,  0, 6, 9,  0, 1, 0},
		{3, 8, 0,  7, 0, 0,  0, 0, 0},
		{0, 0, 0,  0, 0, 0,  0, 2, 6},
		{0, 0, 5,  0, 1, 6,  8, 0, 0}} //26

	case 3: //Tempate
		return [S][S]Value{
		{0, 0, 0,  2, 0, 0,  0, 0, 0},
		{0, 0, 0,  0, 0, 8,  4, 0, 0},
		{0, 7, 0,  4, 0, 0,  8, 9, 0},
		{0, 0, 0,  0, 5, 0,  0, 1, 0},
		{0, 1, 0,  0, 0, 0,  0, 0, 0},
		{0, 0, 3,  0, 0, 6,  0, 0, 2},
		{0, 0, 0,  0, 0, 5,  7, 0, 8},
		{8, 0, 0,  0, 3, 0,  0, 0, 0},
		{0, 6, 1,  0, 0, 0,  0, 4, 0}} //21

	case 4:
		return [S][S]Value{
		{7, 5, 0,  0, 0, 0,  0, 0, 0},
		{0, 0, 0,  0, 0, 0,  0, 1, 2},
		{0, 0, 3,  0, 0, 4,  0, 0, 0},
		{0, 0, 0,  0, 0, 0,  3, 0, 0},
		{0, 0, 5,  0, 0, 0,  4, 0, 6},
		{0, 7, 0,  1, 8, 0,  5, 0, 0},
		{0, 0, 0,  0, 5, 6,  0, 0, 0},
		{0, 2, 0,  0, 0, 0,  0, 0, 0},
		{1, 8, 0,  0, 0, 0,  0, 0, 0}} //19

	case 5:
		return [S][S]Value{
		{0, 2, 4,  0, 0, 0,  0, 0, 0},
		{0, 0, 0,  0, 0, 7,  1, 0, 0},
		{0, 9, 0,  0, 0, 0,  0, 0, 0},
		{0, 0, 0,  0, 0, 0,  0, 8, 4},
		{0, 0, 0,  0, 7, 5,  0, 0, 0},
		{6, 0, 0,  0, 3, 0,  0, 0, 0},
		{0, 0, 0,  4, 0, 0,  0, 2, 9},
		{0, 0, 0,  2, 0, 0,  3, 0, 0},
		{1, 0, 0,  0, 0, 0,  0, 0, 0}} //17

	}

	panic("Selected sudoku hasn't been implemented")
}

func genSud(toUse int) (s Sudoku){
	s.remeaning = make([]int, 0)
	for i, l := range initializeSudokus(toUse){ //Change this to change the sudoku
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
		var log int
		for v != 1{
			log++
			v >>=1
		}
		return 1 + log
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

	fmt.Println("--------------------", s.filledSquares(), "/", SS)
}

func (s Sudoku) filledSquares() int{
	t := 0
	for i := 0; i < SS; i++ {
		if s.board[i] != 0{
			t++
		}
	}
	return t
}

func (s Sudoku) verifySudoku() bool{
	var sum Value
	if s.filledSquares() != SS{
		return false
	}
	//Check rows
	for i := 0; i < S; i++ {
		sum = 0
		for j := 0; j < S; j++ {
			if sum & s.board[index(i, j)] != 0{
				return false
			}
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
			if sum & s.board[index(j, i)] != 0{
				return false
			}
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
					if sum & s.board[index(R*x + i, R*y + j)] != 0{
						return false
					}
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

func lsOfPossible(v Value) []Value{
	var ls [S]Value
	var count int
	for i := 0; i < S; i++ {
		if ((1<<uint(i)) & v) != 0{
			ls[count] = (1<<uint(i))
			count++
		}
	}
	return ls[:count]
}

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

func setPossible(s *Sudoku){ //This function is only called 1 time
	var tempRows, tempCols, tempSqrs [S]Value
	var sqrIndex int
	var val Value

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
		s.rowsAv[i] = ALL ^ tempRows[i]
		s.colsAv[i] = ALL ^ tempCols[i]
		s.sqrsAv[i] = ALL ^ tempSqrs[i]
	}
}

func updateOne(index int, s *Sudoku){
	i, j := coord(index)
	sqrIndex := getSqrIndex(i, j)
	mask := ALL ^ s.board[index]

	s.rowsAv[i] &= mask
	s.colsAv[j] &= mask
	s.sqrsAv[sqrIndex] &= mask
}


func setForced(s *Sudoku) bool{
	var i, j, sqrIndex int
	var count int
	var available, mask Value
	var updated bool
	var nextRem [SS]int //This isnt very space efficient, maybe nextRem := make([]int, len(s.remeaning))


	for _, val := range s.remeaning{
		i, j = coord(val)
		sqrIndex = getSqrIndex(i, j)
		available = s.rowsAv[i] & s.colsAv[j] & s.sqrsAv[sqrIndex]

		if isPow2(available){
			s.board[val] = available
			mask = ALL ^ available
			
			s.rowsAv[i] &= mask
			s.colsAv[j] &= mask
			s.sqrsAv[sqrIndex] &= mask

			updated = true
			
			forcedChanges++ //DEBUG

		}else{
			nextRem[count] = val
			count++
		}
	}
	if updated{
		s.remeaning = nextRem[:count]
	}
	return updated
}

func setAllForced(s *Sudoku){

	for lastUpdate := true; lastUpdate;{
		lastUpdate = setForced(s)
	}
}


func (s Sudoku) solve() (bool, Sudoku){
	//Returns false, _ if it cannot find a solution following this branch
	calls++

	setAllForced(&s)

	if len(s.remeaning) == 0{
		return true, s
	}else{
		//Faster in #3, #5
		
		index := s.remeaning[len(s.remeaning) - 1]
		s.remeaning = s.remeaning[:len(s.remeaning) - 1]
		
		//Faster in #4
		/*
		index := s.remeaning[0]
		s.remeaning = s.remeaning[1:]
		*/


		for _, val := range lsOfPossible(s.possible(index)) {
			newS := Sudoku(s) //Duplicates s
			newS.board[index] = val
			updateOne(index, &newS)
			
			if newS.canBeFinished(){ //This reduces the number of branches by about 1/4
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
	s := genSud(4)
	s.printSudoku()
	
	start := time.Now()
	
	setPossible(&s)
	_, s = s.solve()

	end := time.Now()

	s.printSudoku()

	fmt.Println("\nTotal changes:", forcedChanges)
	fmt.Println("Total nodes:", calls)
	fmt.Println("Is valid:", s.filledSquares() == SS && s.verifySudoku())
	fmt.Println("Time:", end.Sub(start))
}