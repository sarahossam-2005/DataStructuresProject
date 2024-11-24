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

public:

    // Constructor
    SudokuGame() : level_m(Easy) {};

    void initializeGame(level);

    void displayGame(HWND hWnd);

    // Solves the Sudoku using backtracking
    bool solve();

    bool isAvailable(int row, int col, int num);

    // Backtracking to be called in the solve function
    bool backtrack();
};

