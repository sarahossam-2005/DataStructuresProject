#ifndef SUDOKU_H
#define SUDOKU_H

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;


class Sudoku 
{
private:
    // Size of the Sudoku grid 
    int gridSize;  

    // Size of each sub-grid 
    int subGridSize;   

    //2D vector 
    vector<vector<int>> sudokuGrid;  

    // Helper function
    // Checks if it's safe to place num into a position
    bool isAvailable(int row, int col, int num);

public:

    // Constructor
    Sudoku(int size);

    // Takes input for the grid from the user
    void inputGrid();

    // Displays the Sudoku grid
    void printGrid();

    // Solves the Sudoku using backtracking
    bool solve();

    // Backtracking to be called in the solve function
    bool backtrack();
};

#endif // SUDOKU_H

