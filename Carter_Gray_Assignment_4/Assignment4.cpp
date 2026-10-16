/* EECS 348 Assignment 4 - soudko puzzles 

Brief description:
 
The program takes in the inputs from 5 puzzles found in the file system. 
It then sovles every posiible combination and prints all possible solution


 Collaborators: Claude code (Sonnet 5) 
 Other sources: None 
 Author: Carter Gray
 Creation date: 10/15/2026
 Revision date: N/A
 Revisions: N/A

*/


// Assignment4.cpp
// Recursive, object-oriented brute-force Sudoku solver depth-first search
// with backtracking). Finds and prints ALL solutions for each puzzle file.

#include <iostream>// cout endl these for printing
#include <fstream> // ifstream for reading 
#include <string>// string for building file 
#include <cctype> //
using namespace std;// makes it simplier dont need ::cout

class Sudoku { // the sudoku object this is the one loaded and being solved
private:// data and helpers hidden 
    static const int N = 9;  // this is the grid size does not change
    int grid[N][N];  // this is the board a 0 empty 1-9 have been filled 
    int solutionCount;  // num of sol

    // find if a placment is good
    bool isValid(int row, int col, int val) const {  // does not change the object
        for (int i = 0; i < N; i++) { // it covers from 0 to N 
            if (grid[row][i] == val) return false; // illegal if value in row
            if (grid[i][col] == val) return false; // illegal if value in col
        }
        //now check for box
        int br = (row / 3) * 3, bc = (col / 3) * 3;  // the 3x3 block 
        for (int r = br; r < br + 3; r++)  // loop 3 rows ofblock
            for (int c = bc; c < bc + 3; c++)  // loo the col
                if (grid[r][c] == val) return false; //illegal in the block
        return true;  // no erros good spot
    }

    // ensore the givens are good
    bool givensAreConsistent() {
        for (int r = 0; r < N; r++) { // go to all row
            for (int c = 0; c < N; c++) {// go to all col
                int v = grid[r][c];// digit currently in this cell
                if (v == 0) continue;// skip empty cells, nothing to check
                grid[r][c] = 0;// remove it so there is no err with itself
                bool ok = isValid(r, c, v);  // would this digit be illegal
                grid[r][c] = v;   // put it back
                if (!ok) return false;  // illegal there is a duplicate
            }
        }
        return true; // all givens are good and consistent
    }

    // find next cell that is emapty find false if the grid is full.
    bool findEmpty(int &row, int &col) const {// row/col are output parameters (by reference)
        for (row = 0; row < N; row++)// scan rows top to bottom
            for (col = 0; col < N; col++)// scan columns left to right
                if (grid[row][col] == 0) return true; // first empty cell found; row/col hold its position
        return false;// no empty cell: the board is complete
    }

    // this si the depth-first when it backtracks. the it will print every complete solution it reaches and keeps searching.
    void solve() {
        int row, col; //  hold the position  the  empty cell
        if (!findEmpty(row, col)) { //no empty cells left this is when it is returning 
            solutionCount++; // add to count of sol
            cout << "Solution " << solutionCount << ":" << endl;  // label it
            print(); // print the grid
            return; // go back and find others sols
        }
        for (int val = 1; val <= 9; val++) {// try each pos nums 1-9
            if (isValid(row, col, val)) {// oly nums that work
                grid[row][col] = val;// place a try
                solve();  // then recall and try others
                grid[row][col] = 0;// undo and reset before others 
            }
        }
        // if not num works then the loop ends kill,
    }

public:               // interface available to code outside the class
    Sudoku() : solutionCount(0) {                    // constructor: start with zero solutions found
        for (int r = 0; r < N; r++)                  // loop over rows
            for (int c = 0; c < N; c++)              // loop over columns
                grid[r][c] = 0;                      // start with an entirely empty board
    }

    // Read a puzzle from a file. Digits 1-9 are givens; '_' is blank.
    bool load(const string &filename) {
        ifstream in(filename.c_str()); // open 
        if (!in) return false; // did not work the report failure
        int count = 0;  // cell count
        char ch; // the last charater
        while (count < N * N && in.get(ch)) {  // read until 81 cells
            if (ch >= '1' && ch <= '9') { // a digit if assignd
                grid[count / N][count % N] = ch - '0';
                 count++; 
            } // store its numeric val then move
            else if (ch == '_' || ch == '0' || ch == '.') {// mark the a blank cell
                grid[count / N][count % N] = 0;
                count++;// make it a 0 then move
            }
            else if (isspace(static_cast<unsigned char>(ch))) {
    // whitespace between cells is fine, so do nothing
            }
            else {
                return false;  // any other character means the file is not a valid puzzle
            }
        } 
        solutionCount = 0;// new puzzle count
        return count == N * N;// will return if 81 cells are read
    }

    void print() const {// print the grid in a readable layout
        for (int r = 0; r < N; r++) {    // go row by row
            if (r > 0 && r % 3 == 0) cout << "------+-------+------" << endl;  // horizontal divider between blocks
            for (int c = 0; c < N; c++) { // go col by col
                if (c > 0 && c % 3 == 0) cout << "| ";  // vertical divider between blocks
                if (grid[r][c] == 0) cout << "_";    // show empty cells as an underscore
                else cout << grid[r][c];  // show filled cells as their digit
                if (c < N - 1) cout << " ";// space between cells (none after the last one)
            }
            cout << endl;// end of this row
        }
    }

    // Solve the loaded puzzle, printing all solutions found.
    void solveAndReport() {
        solutionCount = 0;// rnsure that count is at 0
        if (givensAreConsistent())// if it initally is wrong thne stop
            solve();// run the  search 
        if (solutionCount == 0)// if search finished and nothing was found
            cout << "No solution found" << endl;  // this will say that there is no sol
        else
            cout << "Total solutions: " << solutionCount << endl;  // summary of how many were found
    }
};

int main() {
    for (int i = 1; i <= 5; i++) {// can handel puzzle1.txt through puzzle5.txt
        string filename = "puzzle" + to_string(i) + ".txt";  // build the file name, e.g. "puzzle3.txt"
        cout << "==============================" << endl;   // it makes a banner line
        cout << filename << endl; // print file name
        cout << "==============================" << endl; // banner line

        Sudoku puzzle;// start with new empty puzzle object
        if (!puzzle.load(filename)) {// try to read the file 
            cout << "Could not read a valid puzzle from " << filename << endl << endl;  // flagg problem
            continue;// go to the next file
        }
        cout << "Puzzle:" << endl;// headingthe first
        puzzle.print();// show how it loaded
        cout << endl;  // blank line 
        puzzle.solveAndReport(); // solve and print every solution
        cout << endl; // blank line between puzzles
    }
    return 0; // kill
}
