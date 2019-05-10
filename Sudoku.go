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
}

/*-------------------------GLOBAL----------------------------*/

var forcedChanges int
var calls int

var remeaning [SS]int
var counter int

/*---------------------INITIALIZATION------------------------*/


func GenSud() (s Sudoku){
	temp_counter := 0
	
	sud := [S][S]Value{
		{0, 2, 4,  0, 0, 0,  0, 0, 0},
		{0, 0, 0,  0, 0, 7,  1, 0, 0},
		{0, 9, 0,  0, 0, 0,  0, 0, 0},
		{0, 0, 0,  0, 0, 0,  0, 8, 4},
		{0, 0, 0,  0, 7, 5,  0, 0, 0},
		{6, 0, 0,  0, 3, 0,  0, 0, 0},
		{0, 0, 0,  4, 0, 0,  0, 2, 9},
		{0, 0, 0,  2, 0, 0,  3, 0, 0},
		{1, 0, 0,  0, 0, 0,  0, 0, 0}} //17

	for i, l := range sud{
		for j, e := range l{
			if e == 0{
				s.board[index(i, j)] = 0
				remeaning[temp_counter] = index(i, j)
				temp_counter++
			}else{
				s.board[index(i, j)] = 1 << uint(e - 1)
			}
		}
	}
	counter = temp_counter

	return s
}

/*---------------------HELPER------------------------*/

func base10(v Value) int{
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


func (self Sudoku) PrintSudoku() string{
	str := "--------------------\n"
	for i := 0; i < S; i++ {
		sSmall := ""
		for j := 0; j < S; j++ {

			if self.board[index(i, j)] == 0{
				sSmall += "- "
			}else{
				sSmall += strconv.Itoa(base10(self.board[index(i, j)])) + " "
			}

			if j % R == R - 1{
				sSmall += " "
			}
		}

		str += sSmall
		str += "\n"
		
		if i % R == R - 1 && i != S - 1{
			str += "\n"
		}
	}

	return str + "--------------------" + strconv.Itoa(self.filledSquares()) + "/" + strconv.Itoa(SS)
}

func (self Sudoku) filledSquares() int{
	t := 0
	for i := 0; i < SS; i++ {
		if self.board[i] != 0{
			t++
		}
	}
	return t
}

func (self Sudoku) finished() bool{
	for i := 0; i < SS; i++ {
		if self.board[i] == 0{
			return false
		}
	}

	return true
}

func (self Sudoku) verifySudoku() bool{
	var sum Value
	if ! self.finished(){
		return false
	}
	//Check rows
	for i := 0; i < S; i++ {
		sum = 0
		for j := 0; j < S; j++ {
			if sum & self.board[index(i, j)] != 0{
				return false
			}
			sum += self.board[index(i, j)]
		}
		if sum != ALL{
			return false
		}
	}
	//Check cols
	for i := 0; i < S; i++ {
		sum = 0
		for j := 0; j < S; j++ {
			if sum & self.board[index(j, i)] != 0{
				return false
			}
			sum += self.board[index(j, i)]
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
					if sum & self.board[index(R*x + i, R*y + j)] != 0{
						return false
					}
					sum += self.board[index(R*x + i, R*y + j)]		
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
	var count int
	var ls [S]Value
	for i := 0; i < S; i++ {
		if ((1<<uint(i)) & v) != 0{
			ls[count] = (1<<uint(i))
			count++
		}
	}
	return ls[:count]
}

func (self Sudoku) canBeFinished() bool{
	for i := 0; i < counter; i++{
		if self.board[remeaning[i]] == 0 && self.possible(remeaning[i]) == 0{
			return false
		}
	}

	return true
}

func (self Sudoku) possible(index int) Value{
	i := index / S
	j := index % S
	return self.rowsAv[i] & self.colsAv[j] & self.sqrsAv[getSqrIndex(i, j)]
}

func setPossible(s *Sudoku){ //This function is only called 1 time
	var tempRows, tempCols, tempSqrs [S]Value
	var val Value

	for i := 0; i < S; i++ {
		for j := 0; j < S; j++ {
			val = s.board[index(i, j)]
			tempRows[i] |= val
			tempCols[j] |= val
			tempSqrs[getSqrIndex(i, j)] |= val
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

	s.rowsAv[i] &= ALL ^ s.board[index]
	s.colsAv[j] &= ALL ^ s.board[index]
	s.sqrsAv[getSqrIndex(i, j)] &= ALL ^ s.board[index]
}


func setForced(s *Sudoku) int{
	var i, j int
	var val int
	var available, mask Value
	var updated int

	for k := 0; k < counter; k++{
		val = remeaning[k]
		if s.board[val] != 0{
			continue
		}
		i, j = coord(val)
		available = s.rowsAv[i] & s.colsAv[j] & s.sqrsAv[getSqrIndex(i, j)]

		if available == 0{
			return 2
		}

		if isPow2(available){
			s.board[val] = available
			mask = ALL ^ available
			
			s.rowsAv[i] &= mask
			s.colsAv[j] &= mask
			s.sqrsAv[getSqrIndex(i, j)] &= mask

			updated = 1
			
			forcedChanges++ //DEBUG

		}
	}

	return updated
}

func setAllForced(s *Sudoku) bool{
	for lastUpdate := 1; lastUpdate == 1;{
		lastUpdate = setForced(s)
		if lastUpdate == 2{
			return false
		}
	}
	return true
}


func (self Sudoku) solve() (bool, Sudoku){
	//Returns false, _ if it cannot find a solution following this branch
	calls++

	isPos := setAllForced(&self)

	if ! isPos{
		return false, self
	}
	if self.finished(){
		return true, self
	}
	
	//Faster in #3, #5
	var index int
	for i := counter - 1; self.board[index] != 0 && i >= 0; i-- {
		index = remeaning[i]	
	}
	
	//Faster in #4
	/*
	for i := 0; s.board[index] != 0 && i < counter; i++ {
		index = remeaning[i]	
	}
	*/


	for _, val := range lsOfPossible(self.possible(index)) {
		newS := Sudoku(self) //Duplicates s
		newS.board[index] = val
		updateOne(index, &newS)
		
		if newS.canBeFinished(){ //This reduces the number of branches by about 1/4
			pos, newS := newS.solve()
			if pos{
				return true, newS
			}
		}
	}

	return false, Sudoku{}
}

func SolveMaster(s Sudoku) (Sudoku, /*ok*/ bool, /*forcedChanges*/ int, /*calls*/ int){
	rs := Sudoku(s)

	setPossible(&rs)

	if ! rs.canBeFinished(){
		return s, false, 0, 0
	}
	_, rs = rs.solve()

	ok := rs.filledSquares() == SS && rs.verifySudoku()

	return rs, ok, forcedChanges, calls
}

func main(){

	s := GenSud()
	fmt.Println(s.PrintSudoku())
	
	start := time.Now()
	
	s, ok, forcedChanges, calls := SolveMaster(s)

	end := time.Now()

	fmt.Println(s.PrintSudoku())

	fmt.Println("\nTotal changes:", forcedChanges)
	fmt.Println("Total nodes:", calls)
	fmt.Println("Is valid:", ok)
	fmt.Println("Time:", end.Sub(start))
	
}