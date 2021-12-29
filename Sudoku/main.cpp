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
- dodaæ do openTriples pola o wielkoœci 2
- napisaæ algorytm ³añcuch XY
- wyœwietlanie kandydatów
- rozbiæ klasê sudoku na 2 klasy - menu, solver
- stworzyæ klasê wczytkuj¹c¹ sudoku z pliku graficznego
- stworzyæ klasê generuj¹c¹ sudoku
*/
