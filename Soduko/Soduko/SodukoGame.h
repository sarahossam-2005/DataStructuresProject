#pragma once
#include <iostream>
#include <cmath>
#include <windows.h>
#include "myStack.h"
#include <utility>

using namespace std;

enum level { Easy, Medium, Hard };

class SudokuGame
{
private:
   
    

    //constants
    const int gridSize = 450;    // Total size of the grid in pixels
    const int cellSize = gridSize / 9;  // Size of one cell
    const int startX = 50;       // Starting X coordinate
    const int startY = 50;       // Starting Y coordinate

    void InitGrid(int iCellsCount);
public:
    level level_m;
    int grid[9][9];
    char grid_code[9][9];
    int current_cell_i;
    int current_cell_j;
    int current_guess[9][9] = { 0 };
    myStack cell_stack;
    bool start_solver = false;
    bool solving_complete = false;
    int max_errors;
    int error_count = 0;
    int max_hints;
    int hint_count;
    int solution_grid[9][9];
    
    int score=100;

    // Constructor
    SudokuGame() : level_m(Easy) {};

    void initializeGame(level);

    void displayGame(HWND hWnd);

    void handleClick(HWND hWnd, int x, int y);
    
    void handleHint(HWND hWnd);

    void handleKeyPress(HWND hWnd, int keycode);

    void updateGridCodes();

    void clearErrors();

    void updateCellCode(int cell_i, int cell_j);

    void update_solver(HWND hWnd);

    bool isValid(int row, int col, int num);

    // Solves the Sudoku using backtracking
    bool solve();

    void GameOver();
    bool isSolved();

    void Show_Solution(HWND);
    //bool isAvailable(int row_index, int col_index);

    // Backtracking to be called in the solve function
    bool backtrack();

    void handleDeleteOrSpaceKey(HWND hWnd, int keycode, int cell_i, int cell_j);

};

