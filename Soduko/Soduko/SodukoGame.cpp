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
    error_count = 0;
    start_solver = false;
    solving_complete = false;
    score = 100;
    hint_count = 0;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            current_guess[i][j] = 0;
    cell_stack.clear();

    // Initialize the Sudoku grid based on the difficulty level
    switch (level_m) {
    case Easy: {
        max_errors = 3;
        max_hints = 3;
        InitGrid(65);
        break;
    }
    case Medium: {
        max_errors = 5;
        max_hints = 5;
        InitGrid(55);
        break;
    }
    case Hard: {
        max_errors = 8;
        max_hints = 8;
        InitGrid(45);
        break;
    }
    default:
        InitGrid(65);
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
    SolidBrush brush3(Color(255, 255, 165, 0)); // orange
    

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
                else if (grid_code[i][j] == 'h')
                {
                    graphics.DrawString(str, 1, &font, point, &brush3);
                }

            }
        }
    }

    std::wstring errorCountStr = L"Errors: " + std::to_wstring(error_count);
    std::wstring hintCountStr = L"Hints: " + std::to_wstring(hint_count);
    std::wstring scoreStr = L"Score: " + std::to_wstring(score);

    RECT rect;
    GetClientRect(hWnd, &rect);
    int windowWidth = rect.right - rect.left;

    PointF errorPoint(10.0f, 10.0f);
    int hintTextWidth = hintCountStr.length() * 18;
    PointF hintPoint((REAL)(windowWidth - hintTextWidth - 10), 10.0f);

    int scoreTextWidth = scoreStr.length() * 18;
    PointF scorePoint((REAL)((windowWidth - scoreTextWidth) / 2), 10.0f);

    graphics.DrawString(scoreStr.c_str(), -1, &font, scorePoint, &brush);
    graphics.DrawString(errorCountStr.c_str(), -1, &font, errorPoint, &brush);
    graphics.DrawString(hintCountStr.c_str(), -1, &font, hintPoint, &brush);


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

    if (grid_code[current_cell_i][current_cell_j] != 'i' && grid_code[current_cell_i][current_cell_j] != 'h')
    {
        if (keycode >= 49 && (char)keycode <= 57)
        {
            if (grid[current_cell_i][current_cell_j] == keycode - 48)
                return;
            grid[current_cell_i][current_cell_j] = keycode - 48;
            this->updateGridCodes();
            if (grid_code[current_cell_i][current_cell_j] == 'e') {
                score -= 10;
                error_count++;
            }
            else if (grid_code[current_cell_i][current_cell_j] != 'e') {
                score+=5;
            }
            GameOver();
            InvalidateRect(hWnd, NULL, TRUE);
        }
        if (keycode >= 97 && (char)keycode <= 105) {
            if (grid[current_cell_i][current_cell_j] == keycode - 9)
                return;
            grid[current_cell_i][current_cell_j] = keycode - 96;
            this->updateGridCodes(); 
            if (grid_code[current_cell_i][current_cell_j] == 'e') {
                error_count++;
                score -= 10;
            }
            else if (grid_code[current_cell_i][current_cell_j] != 'e')
            {
                score += 5;
            }
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
    /*if (!start_solver || solving_complete)
        return;*/

    //if current_guess is zero then we need to init current_cell_i and current_cell_j
    /*if (start_solver) */{
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
 
        if (error_count >= max_errors){
            // Game over due to too many errors or used up all the allowed hints per level 
            MessageBox(NULL, L"Game Over! You made too many mistakes.", L"Game Over", MB_ICONERROR | MB_OK);
            solving_complete = true;
            start_solver = false;
            return;
        }
        if (isSolved()) {
            std::wstring scoreMessage = L"Congratulations! You solved the Sudoku puzzle.\nYour score: " + std::to_wstring(score);
            MessageBox(NULL, scoreMessage.c_str(), L"Game Complete", MB_ICONINFORMATION | MB_OK);
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

void SudokuGame::InitGrid(int iCellCount)
{
    int base[9][9] = {
            {1,2,3,4,5,6,7,8,9},
            {4,5,6,7,8,9,1,2,3},
            {7,8,9,1,2,3,4,5,6},
            {2,3,4,5,6,7,8,9,1},
            {5,6,7,8,9,1,2,3,4},
            {8,9,1,2,3,4,5,6,7},
            {3,4,5,6,7,8,9,1,2},
            {6,7,8,9,1,2,3,4,5},
            {9,1,2,3,4,5,6,7,8}
    };
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            grid[i][j] = base[i][j];
    
    int shuffleCount = rand() % 10 + 5;
    for (int k = 0; k < shuffleCount; k++)
    {
        int row_shuffle = rand() % 2;
        int group = rand() % 3;
        int t1 = rand() % 3;
        int t2 = rand() % 3;
        if (t1 == t2)
            t2 = (t2 + 1) % 3;
        if (row_shuffle) {
            for (int i = 0; i < 9; i++)
            {
                int v1 = grid[group * 3 + t1][i];
                grid[group * 3 + t1][i] = grid[group * 3 + t2][i];
                grid[group * 3 + t2][i] = v1;
            }
        }
        else {
            for (int i = 0; i < 9; i++)
            {
                int v1 = grid[i][group * 3 + t1];
                grid[i][group * 3 + t1] = grid[i][group * 3 + t2];
                grid[i][group * 3 + t2] = v1;
            }
        }
    }
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            solution_grid[i][j] = grid[i][j];
        }
    }
    int maskCount = 81 - iCellCount;
    for (int k = 0; k < maskCount; k++) {
        int i = rand() % 9;
        int j = rand() % 9;
        while (grid[i][j] == 0)
        {
            i = rand() % 9;
            j = rand() % 9;
        }
        grid[i][j] = 0;
    }
}

void SudokuGame::handleDeleteOrSpaceKey(HWND hWnd, int keycode, int cell_i, int cell_j)
{
    if (grid_code[cell_i][cell_j] != 'i' && grid_code[cell_i][cell_j] != 'h') {
        if (keycode == VK_DELETE || keycode == VK_SPACE)
        {
            grid[cell_i][cell_j] = 0;
            grid_code[cell_i][cell_j] = 'u';
            InvalidateRect(hWnd, NULL, TRUE);
        }
    } 
}

void SudokuGame::handleHint(HWND hWnd)
{
    if (current_cell_i == -1 || current_cell_j == -1) {
        MessageBox(hWnd, L"Please select a cell before requesting a hint.", L"Hint Error", MB_OK | MB_ICONERROR);
        return;
    }
    if (hint_count >= max_hints)
    {
        MessageBox(NULL, L"You've asked for too many hints.", L"Good Luck", MB_ICONERROR | MB_OK);
        return;
    }
    else {
        hint_count++;
        
        grid[current_cell_i][current_cell_j] = solution_grid[current_cell_i][current_cell_j];
        grid_code[current_cell_i][current_cell_j] = 'i';
        score -= 5;
        InvalidateRect(hWnd, NULL, TRUE);
        UpdateWindow(hWnd);
        GameOver();
    }
}