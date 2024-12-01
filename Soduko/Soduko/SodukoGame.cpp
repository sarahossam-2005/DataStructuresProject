#include "SodukoGame.h"
#include "Program.h"
#include <Windows.h>
#include <gdiplus.h>
#include <gdiplusfont.h>
#include <gdiplusfontfamily.h>
#include <string>



#pragma comment(lib, "gdiplus.lib")

using namespace std;
using namespace Gdiplus;

void SudokuGame::initializeGame(level level_m)
{
    current_cell_i = -1;
    current_cell_j = -1;
    start_solver = false;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            current_guess[i][j] = 0;
    cell_stack = myStack<std::pair<int, int>>();

    // Initialize the Sudoku grid based on the difficulty level
    switch (level_m) {
    case Easy: {
        int easyPuzzle[9][9] = {
            {9, 4, 2, 1, 6, 3, 8, 0, 7},
            {5, 0, 6, 2, 0, 7, 9, 4, 1},
            {8, 7, 1, 9, 5, 4, 0, 3, 0},
            {3, 2, 0, 8, 1, 9, 4, 0, 5},
            {1, 0, 4, 3, 2, 6, 7, 9, 0},
            {6, 9, 8, 7, 0, 5, 1, 2, 3},
            {2, 6, 5, 4, 0, 1, 3, 8, 9},
            {0, 8, 0, 6, 3, 2, 5, 1, 4},
            {4, 1, 0, 5, 9, 8, 6, 7, 0}
        };
        memcpy(grid, easyPuzzle, sizeof(grid));
        break;
        for (int i = 0; i <= 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                if (easyPuzzle[i][j] != 0)
                {
                   
                }
            }
        }
    }
    case Medium: {
        int mediumPuzzle[9][9] = {
            {0, 0, 3, 0, 2, 0, 6, 0, 0},
            {9, 0, 0, 3, 0, 5, 0, 0, 1},
            {0, 0, 1, 8, 0, 6, 4, 0, 0},
            {0, 0, 8, 1, 0, 2, 9, 0, 0},
            {7, 0, 0, 0, 0, 0, 0, 0, 8},
            {0, 0, 6, 7, 0, 8, 2, 0, 0},
            {0, 0, 2, 6, 0, 9, 5, 0, 0},
            {8, 0, 0, 2, 0, 3, 0, 0, 9},
            {0, 0, 5, 0, 1, 0, 3, 0, 0}
        };
        memcpy(grid, mediumPuzzle, sizeof(grid));
        break;
    }
    case Hard: {
        int hardPuzzle[9][9] = {
            {0, 0, 0, 0, 0, 0, 0, 1, 2},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 0, 6, 4, 5, 0, 0},
            {0, 0, 0, 0, 2, 0, 0, 0, 0},
            {0, 0, 0, 5, 0, 9, 0, 0, 0},
            {0, 0, 0, 0, 3, 0, 0, 0, 0},
            {0, 0, 8, 7, 1, 0, 6, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 9, 6, 0, 0, 0, 0, 0, 0}
        };
        memcpy(grid, hardPuzzle, sizeof(grid));
        break;
    }
    default:
        int easyPuzzle[9][9] = {
            {5, 0, 0, 0, 7, 0, 0, 0, 0},
            {6, 0, 0, 1, 9, 5, 0, 0, 0},
            {0, 9, 8, 0, 0, 0, 0, 6, 0},
            {8, 0, 0, 0, 6, 0, 0, 0, 3},
            {4, 0, 0, 8, 0, 3, 0, 0, 1},
            {7, 0, 0, 0, 2, 0, 0, 0, 6},
            {0, 6, 0, 0, 0, 0, 2, 8, 0},
            {0, 0, 0, 4, 1, 9, 0, 0, 5},
            {0, 0, 0, 0, 8, 0, 0, 7, 9}
        };
        memcpy(grid, easyPuzzle, sizeof(grid));
        break;
    }
    //TODO SARA Init grid_codes
    for (int i = 0; i <= 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (grid[i][j] != 0)
            {
                grid_code[i][j] = 'i';
            }
            else
            {
                grid_code[i][j] = 'u';
            }
        }
    }   
}

void SudokuGame::displayGame(HWND hWnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    // Draw the grid
    for (int i = 0; i <= 9; ++i) {
        int x = startX + i * cellSize;
        int y = startY + i * cellSize;

        int penWidth = (i % 3 == 0) ? 3 : 1;  // Thicker lines for 3x3 subgrids

        // Draw vertical line
        HPEN hPen = CreatePen(PS_SOLID, penWidth, RGB(0, 0, 0));
        SelectObject(hdc, hPen);
        MoveToEx(hdc, x, startY, nullptr);
        LineTo(hdc, x, startY + gridSize);
        DeleteObject(hPen);

        // Draw horizontal line
        hPen = CreatePen(PS_SOLID, penWidth, RGB(0, 0, 0));
        SelectObject(hdc, hPen);
        MoveToEx(hdc, startX, y, nullptr);
        LineTo(hdc, startX + gridSize, y);
        DeleteObject(hPen);
    }


    // Initialize GDI+ Graphics
    Graphics graphics(hdc);

    //show current cell
    if (current_cell_i > -1 && current_cell_j > -1){
        SolidBrush highlightBrush(Color(50, 0, 0, 0));
        graphics.FillRectangle(&highlightBrush, startX + current_cell_j * cellSize, startY + current_cell_i * cellSize, cellSize, cellSize);
    }

    FontFamily family(L"Arial");
    Font font(&family, 24);  // Font size of 48
    SolidBrush brush(Color(255, 0, 0, 0));  // Black brush for text

    //TODO SARA make new more brushes for blue and red
    SolidBrush brush1(Color(255, 0, 0, 255)); //blue
    SolidBrush brush2(Color(255, 255, 0, 0));  // red

    

    // Loop through the grid and draw numbers
    
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (grid[i][j] != 0) {  // Only draw if the cell is not empty
                // Calculate the position of the cell
                RECT rect;
                rect.left = startX + j * cellSize;
                rect.top = startY + i * cellSize;
                rect.right = rect.left + cellSize;
                rect.bottom = rect.top + cellSize;

                // Convert the grid value to a character and draw it
                wchar_t str[2] = { (wchar_t)(grid[i][j] + L'0'), L'\0' };  // Convert int to wchar_t

                // Create a point for drawing the text in the center of the cell
                PointF point((REAL)(rect.left + rect.right) / 2 - 12, (REAL)(rect.top + rect.bottom) / 2 - 16);
                
                // Draw the text with the custom font
                //TODO SARA choose bruens based on grid_code
                if(grid_code[i][j]=='i')
                    graphics.DrawString(str, 1, &font, point, &brush);
                else if(grid_code[i][j] == 'u')
                    graphics.DrawString(str, 1, &font, point, &brush1);
                else if (grid_code[i][j] == 'e') {
                    graphics.DrawString(str, 1, &font, point, &brush2);
                }

            }
        }
    }

    // To display the error_count
    std::wstring errorCountStr = L"Errors: " + std::to_wstring(error_count);

    RECT rect;
    GetClientRect(hWnd, &rect);

    
    int windowWidth = rect.right - rect.left;

    
    int textWidth = errorCountStr.length() * 18;  

    
    int xPos = (windowWidth - textWidth) / 2;  

    PointF errorPoint((REAL)xPos, 10);  
    graphics.DrawString(errorCountStr.c_str(), -1, &font, errorPoint, &brush);

    EndPaint(hWnd, &ps);
}

void SudokuGame::handleClick(HWND hWnd, int x, int y) {
    if (solving_complete || start_solver)
        return;
    if (x > startX && x < startX + gridSize && y > startY && y < startY + gridSize) {
        int new_j= (x - startX) / cellSize;
        int new_i = (y - startY) / cellSize;
        if (new_j != current_cell_j || new_i != current_cell_i) {
            current_cell_j = new_j;
            current_cell_i = new_i;
            InvalidateRect(hWnd, NULL, TRUE);
        }
    }
}

void SudokuGame::handleKeyPress(HWND hWnd, int keycode)
{
    if (solving_complete || start_solver)
        return;
    //TODO do not update the cell if its grid_code is "i"

    if (grid_code[current_cell_i][current_cell_j] != 'i') 
    {
        if (keycode >= 49 && (char)keycode <= 57)
        {
            if (grid[current_cell_i][current_cell_j] == keycode - 48)
                return;
            grid[current_cell_i][current_cell_j] = keycode - 48;
            this->updateGridCodes();
            if (grid_code[current_cell_i][current_cell_j] == 'e')
                error_count++;
            GameOver();
            InvalidateRect(hWnd, NULL, TRUE);
        }
        if (keycode >= 97 && (char)keycode <= 105) {
            if (grid[current_cell_i][current_cell_j] == keycode - 9)
                return;
            grid[current_cell_i][current_cell_j] = keycode - 96;
            this->updateGridCodes(); 
            if (grid_code[current_cell_i][current_cell_j] == 'e')
                error_count++;
            GameOver();
            InvalidateRect(hWnd, NULL, TRUE);
        }
    }
    
    
}

void SudokuGame::updateGridCodes()
{
    for (int cell_i = 0; cell_i < 9; cell_i++) {
        for (int cell_j = 0; cell_j < 9; cell_j++) {
            updateCellCode(cell_i, cell_j);
        }
    }
}

void SudokuGame::updateCellCode(int cell_i, int cell_j)
{
    if (grid_code[cell_i][cell_j] == 'i')
        return;

    //Check 3x3 mini grid
    int subgridRow = (cell_i / 3) * 3;
    int subgridCol = (cell_j / 3) * 3;
    grid_code[cell_i][cell_j] = 'u';

    for (int i = subgridRow; i < subgridRow + 3; i++)
    {
        for (int j = subgridCol; j < subgridCol + 3; j++)
        {
            if ((i != cell_i || j != cell_j) && grid[i][j] == grid[cell_i][cell_j])
            {
                grid_code[cell_i][cell_j] = 'e';
                return;
            }
        }
        //check the row
        for (int j = 0; j < 9; j++)
        {
            if (cell_j != j && grid[cell_i][cell_j] == grid[cell_i][j])
            {
                grid_code[cell_i][cell_j] = 'e';
                return;
            }
        }

        //check the column
        for (int i = 0; i < 9; i++)
        {
            if (i != cell_i && grid[cell_i][cell_j] == grid[i][cell_j])
            {
                grid_code[cell_i][cell_j] = 'e';
                return;
            }

        }
    }
}

bool SudokuGame::isSolved()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (grid_code[i][j] == 'e' || grid[i][j]==0)
            {
                return false;
            } 
        }
    }
    return true;
}

//void SudokuGame::update_solver(HWND hWnd) {
//    //if (!start_solver)
//    //    return;
//    for (int i = 0; i < 9; i++) {
//        for (int j = 0; j < 9; j++) {
//            if (grid[i][j] == 0) {
//                grid[i][j] = 1;
//                current_cell_i = i;
//                current_cell_j = j;
//                this->updateGridCodes();
//                return;
//            }
//        }
//    }
//}

// Helper function to check if num can be placed at grid[i][j]
bool SudokuGame::isValid(int row, int col, int num) {
    // Check if num is not in the current row, column, or 3x3 subgrid
    for (int x = 0; x < 9; x++) {
        if (grid[row][x] == num || grid[x][col] == num)
            return false;
    }
    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[i + startRow][j + startCol] == num)
                return false;
        }
    }
    return true;
}

void SudokuGame::update_solver(HWND hWnd) {
    if (!start_solver || solving_complete)
        return;

    //if current_guess is zero then we need to init current_cell_i and current_cell_j
    if (start_solver) {
        bool allZeros = true;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (current_guess[i][j] != 0) {
                    allZeros = false;
                    break;
                }
            }
            if (!allZeros) break;
        }
        if (allZeros) {
            for (int i = 0; i < 9; i++) {
                bool found = false;
                for (int j = 0; j < 9; j++) {
                    if (grid[i][j] == 0 && grid_code[i][j] != 'i') {
                        current_cell_i = i;
                        current_cell_j = j;
                        current_guess[i][j] = 1;
                        found = true;
                        break;
                    }
                }
                if (found) break;
            }
        }
        // No empty cells, puzzle is complete
        if (current_cell_i == -1) {
            solving_complete = true;
            return;
        }


        int i = current_cell_i;
        int j = current_cell_j;
        int num = current_guess[i][j];

        while (num <= 9) {
            if (isValid(i, j, num)) {
                grid[i][j] = num;
                cell_stack.push({ i, j });
                current_guess[i][j] = num;

                // Find next empty cell
                bool found_next = false;
                for (int ii = i; ii < 9; ii++) {
                    for (int jj = (ii == i ? j + 1 : 0); jj < 9; jj++) {
                        if (grid[ii][jj] == 0) {
                            current_cell_i = ii;
                            current_cell_j = jj;
                            current_guess[ii][jj] = 1;
                            found_next = true;
                            break;
                        }
                    }
                    if (found_next)
                        break;
                }
                if (!found_next) {
                    // Puzzle solved
                    solving_complete = true;
                }
                this->updateGridCodes();
                return; // Wait for next update_solver call
            }
            num++;
        }

        // No valid number found, need to backtrack
        grid[i][j] = 0;
        current_guess[i][j] = 1; // Reset for next time
        if (!cell_stack.IsEmpty()) {
            // Backtrack to previous cell
            auto temp = cell_stack.StackTop();
            auto prev_i = temp.first;
            auto prev_j = temp.second;
            cell_stack.pop();
            current_cell_i = prev_i;
            current_cell_j = prev_j;
            current_guess[prev_i][prev_j]++;
            this->updateGridCodes();
        }
        else {
            // No solution
            solving_complete = true;
        }
    }
}

void SudokuGame::Show_Solution(HWND hWnd) {
    solving_complete = false;
    start_solver = true;
    current_cell_i = -1;
    int max_trials = 1000000;

    while (!solving_complete && max_trials-- != 0) {
        this->update_solver(hWnd);
    }
}


void SudokuGame::GameOver()
{
 
        if (error_count >= 6) {
            // Game over due to too many errors
            MessageBox(NULL, L"Game Over! You made too many mistakes.", L"Game Over", MB_ICONERROR | MB_OK);
            solving_complete = true;
            start_solver = false;
            return;
        }

        if (isSolved()) {
            // Game over because the puzzle is successfully solved
            MessageBox(NULL, L"Congratulations! You solved the Sudoku puzzle.", L"Game Complete", MB_ICONINFORMATION | MB_OK);
            solving_complete = true;
            start_solver = false;
            return;
        }
    }



void SudokuGame::clearErrors()
{
    current_cell_i = -1;
    current_cell_j = -1;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++)
        {
            if (grid_code[i][j] != 'i') {
                grid_code[i][j] == 'u';
                grid[i][j] = 0;
            }
        }
    }
}