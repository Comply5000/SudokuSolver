#include "Sudoku.hpp"

int main()
{
    Sudoku sudoku;
    while (sudoku.running())
    {
        sudoku.update();
        sudoku.render();
    }

    return 0;
}
