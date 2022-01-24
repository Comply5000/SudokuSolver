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
- stworzyæ klasê wczytkuj¹c¹ sudoku z pliku graficznego
- stworzyæ klasê generuj¹c¹ sudoku
*/
