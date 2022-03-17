#include "Sudoku.hpp"

int main()
{
    ::ShowWindow(GetConsoleWindow(), SW_HIDE);
    Sudoku sudoku;
    while (sudoku.running())
    {
        sudoku.update();
        sudoku.render();
    }
    return 0;
}
