#include <iostream>
#include <string>

using namespace std;

//main gameboard 
struct GameBoard {
    string cells; //all grid cells in 1D
    int cols; //no. of columns 
};


//in simple words, finding a cell at a given row and column (index)
char fetchCell(const GameBoard& board, int row, int col) {
    if (row < 0 || col < 0 || col >= board.cols) return '.'; //boundary check
    
    int cellIndex = row * board.cols + col; //2D to 1D mapping
    if (cellIndex >= (int)board.cells.length()) {
        return '.';
    }
    return board.cells[cellIndex];
}

//counting alive neighbours
int calculateAlive(const GameBoard& board, int row, int col) {
    int livingCount = 0;
    
    for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
        for (int colOffset = -1; colOffset <= 1; colOffset++) {
            if (rowOffset == 0 && colOffset == 0) continue; //skipping the main cell itself 
            
            if (fetchCell(board, row + rowOffset, col + colOffset) == 'O') {
                livingCount++;
            }
        }
    }
    
    return livingCount;
}

//1 time step the board 
void stepBoard(GameBoard& board) {
    GameBoard nextBoard = board;
    int cellCount = (int)board.cells.length();
    
    for (int cellIdx = 0; cellIdx < cellCount; cellIdx++) {
        int gridRow = cellIdx / board.cols;
        int gridCol = cellIdx % board.cols;
        int neighborCount = calculateAlive(board, gridRow, gridCol);
        
        bool cellIsAlive = (board.cells[cellIdx] == 'O');
        //alive cell rules 
        if (cellIsAlive) {
            //underpopulation or overpopulation
            if (neighborCount < 2 || neighborCount > 3) {
                nextBoard.cells[cellIdx] = '.';
            }
        } else { //dead cell rules 
            //revival on exactly 3 neighbours 
            if (neighborCount == 3) {
                nextBoard.cells[cellIdx] = 'O';
            }
        }
    }
    
    board = nextBoard;
}

//print the board 
void displayBoard(GameBoard& board) {
    for (int colIdx = 0; colIdx < board.cols; colIdx++) {
        cout << "|";
    }
    cout << "\n";
        
    int cellCount = (int)board.cells.length();
    for (int cellIdx = 0; cellIdx < cellCount; cellIdx++) {
        cout << board.cells[cellIdx];
        if ((cellIdx + 1) % board.cols == 0) {
            cout << "\n";
        }
    }
    
    for (int colIdx = 0; colIdx < board.cols; colIdx++) {
        cout << "|";
    }
    cout << "\n";
}
    
int main() {
    GameBoard gameBoard;
    string firstLine;
    
    if (!(cin >> firstLine)) return 0;
    
    gameBoard.cells = firstLine;
    gameBoard.cols = firstLine.length();
    
    string rowLine;
    while (cin >> rowLine && rowLine != "x") {
        gameBoard.cells += rowLine;
    }
    
    char cmd;

    //command input 
    while (cin >> cmd) {
        //print the board 
        if (cmd == 'p') {
            displayBoard(gameBoard);
        } else if (cmd == 's') { //step the board by 1
            stepBoard(gameBoard);
        } else { //invalid command error
            cerr << "Invalid command: " << cmd << "\n";
            cerr << "Test is invalid input.\n";
        }
    }
    
    return 0;
}