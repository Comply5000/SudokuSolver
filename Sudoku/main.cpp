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
- doda� do openTriples pola o wielko�ci 2
- napisa� algorytm �a�cuch XY
- wy�wietlanie kandydat�w
- rozbi� klas� sudoku na 2 klasy - menu, solver
- stworzy� klas� wczytkuj�c� sudoku z pliku graficznego
- stworzy� klas� generuj�c� sudoku
*/
