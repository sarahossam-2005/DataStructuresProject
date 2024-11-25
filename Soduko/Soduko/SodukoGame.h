#pragma once
#include <iostream>
#include <cmath>
#include <windows.h>

using namespace std;

enum level { Easy, Medium, Hard };

class SudokuGame
{
private:
    level level_m;
    int grid[9][9];
    char grid_code[9][9];
    int current_cell_i;
    int current_cell_j;

    //constants
    const int gridSize = 450;    // Total size of the grid in pixels
    const int cellSize = gridSize / 9;  // Size of one cell
    const int startX = 50;       // Starting X coordinate
    const int startY = 50;       // Starting Y coordinate

public:

    // Constructor
    SudokuGame() : level_m(Easy) {};

    void initializeGame(level);

    void displayGame(HWND hWnd);

    void handleClick(HWND hWnd, int x, int y);

    void handleKeyPress(HWND hWnd, int keycode);

    void updateGridCodes();

    // Solves the Sudoku using backtracking
    bool solve();

    bool isSolved();

    //bool isAvailable(int row_index, int col_index);

    // Backtracking to be called in the solve function
    bool backtrack();

};

