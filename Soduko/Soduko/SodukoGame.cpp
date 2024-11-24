#include "SodukoGame.h"
#include "Program.h"
#include <Windows.h>
#include <gdiplus.h>
#include <gdiplusfont.h>
#include <gdiplusfontfamily.h>

#pragma comment(lib, "gdiplus.lib")

using namespace std;
using namespace Gdiplus;

void SudokuGame::initializeGame(level level_m)
{
    // Initialize the Sudoku grid based on the difficulty level
    switch (level_m) {
    case Easy: {
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
}

void SudokuGame::displayGame(HWND hWnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    // Constants for the grid
    int gridSize = 450;    // Total size of the grid in pixels
    int cellSize = gridSize / 9;  // Size of one cell
    int startX = 50;       // Starting X coordinate
    int startY = 50;       // Starting Y coordinate

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

    // Create a string.
    WCHAR string[] = L"Sample Text";

    // Initialize arguments.

    Font myFont(GenericSansSerifFontFamily, 16);
    PointF origin(0.0f, 0.0f);
    SolidBrush blackBrush(Color(255, 0, 0, 0));

    // Draw string.
    graphics.DrawString(
        string,
        11,
        &myFont,
        origin,
        &blackBrush);
    
    // Create the font and brush for drawing numbers
    FontFamily family(L"Arial");
    Font font(&family, 48);  // Font size of 48
    SolidBrush brush(Color(255, 0, 0, 0));  // Black brush for text

    // Loop through the grid and draw numbers
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (grid[i][j] != 0) {  // Only draw if the cell is not empty
                // Calculate the position of the cell
                RECT rect;
                rect.left = startX + j * cellSize;    // Left position of the cell
                rect.top = startY + i * cellSize;     // Top position of the cell
                rect.right = rect.left + cellSize;    // Right position (left + cell size)
                rect.bottom = rect.top + cellSize;    // Bottom position (top + cell size)

                // Convert the grid value to a character and draw it
                wchar_t str[2] = { (wchar_t)(grid[i][j] + L'0'), L'\0' };  // Convert int to wchar_t

                // Create a point for drawing the text in the center of the cell
                PointF point((REAL)(rect.left + rect.right) / 2 - 12, (REAL)(rect.top + rect.bottom) / 2 - 24);  // Adjust position for font centering

                // Draw the text with the custom font
                graphics.DrawString(str, 1, &font, point, &brush);
            }
        }
    }

    EndPaint(hWnd, &ps);
}
