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

/*
to do:
- stworzy� klas� wczytkuj�c� sudoku z pliku graficznego
- stworzy� klas� generuj�c� sudoku
*/
