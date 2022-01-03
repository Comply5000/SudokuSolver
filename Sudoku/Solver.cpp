#include "Solver.hpp"

Solver::Solver(std::array<std::array<int, 9>, 9> tab)
{
	this->number = tab;
}

Solver::~Solver()
{

}

bool Solver::existInSquare(int y, int x, int k)
{
	int a, b;
	if (x < 3)
		b = 0;
	else if (x >= 3 && x < 6)
		b = 3;
	else if (x >= 6)
		b = 6;

	if (y < 3)
		a = 0;
	else if (y >= 3 && y < 6)
		a = 3;
	else if (y >= 6)
		a = 6;

	for (int i = a;i < a + 3;i++)
		for (int j = b;j < b + 3;j++)
			if (number[i][j] == k)
				return true;
	return false;
}

bool Solver::contains(std::vector<int> vec, const int& elem)
{
	bool result = false;
	for (auto& x : vec)
	{
		if (x == elem)
		{
			result = true;
			break;
		}
	}
	return result;
}

void Solver::findCandidate()
{
	std::vector<std::vector<std::vector<int>>>tab(9, std::vector<std::vector<int>>(9));

	for (int i = 0;i < 9;i++)
	{
		for (int j = 0;j < 9;j++)
		{
			if (this->number[i][j] != 0)
				tab[i][j].push_back(10);
			else
			{
				for (int k = 1;k <= 9;k++)
				{
					bool exist = false;
					for (int x = 0;x < 9;x++)
					{
						if (k == number[i][x])
						{
							exist = true;
							break;
						}
					}
					for (int x = 0;x < 9;x++)
					{
						if (k == number[x][j])
						{
							exist = true;
							break;
						}
					}
					if (this->existInSquare(i, j, k))
						exist = true;

					if (!exist)
						tab[i][j].push_back(k);
				}
			}
		}
	}
	this->cand = tab;
}

void Solver::singleCandidate()
{
	for (int i = 0;i < 9;i++)
	{
		for (int j = 0;j < 9;j++)
		{
			if (this->cand[i][j].size() == 1 && this->cand[i][j][0] != 10)
			{
				this->number[i][j] = this->cand[i][j][0];
				this->solved = true;
				return;
			}
		}
	}
}

void Solver::hiddenLoner() //ukryty kandydat
{
	for (int i = 0;i < 9;i++) //iteracja po wierszach
	{
		for (int j = 0;j < 9;j++)
		{
			if (this->cand[i][j].size() > 0 && this->cand[i][j][0] != 10) // pole nie jest uzupe�nione
			{
				for (int k = 0;k < this->cand[i][j].size();k++) //iteracja po kandydatach w polu
				{
					int n = 0;
					for (int a = 0;a < 9;a++) //ponowna iteracja po wszystkich polach w wierszu aby sparawdzi� ile jest tych kandydat�w
						if (this->cand[i][a][0] != 10 && this->contains(this->cand[i][a], this->cand[i][j][k]))
							n++;
					if (n == 1) //je�eli jest tylko 1 kandydat: uzupe�nienie bloku tym kandydatem
					{
						this->number[i][j] = this->cand[i][j][k];
						this->solved = true;
						return;
					}
				}
			}
		}
	}

	//to samo tylko kolumny
	for (int j = 0;j < 9;j++)
	{
		for (int i = 0;i < 9;i++)
		{
			if (this->cand[i][j].size() > 0 && this->cand[i][j][0] != 10)
			{
				for (int k = 0;k < this->cand[i][j].size();k++)
				{
					int n = 0;
					for (int a = 0;a < 9;a++)
						if (this->cand[a][j][0] != 10 && this->contains(this->cand[a][j], this->cand[i][j][k]))
							n++;
					if (n == 1)
					{
						this->number[i][j] = this->cand[i][j][k];
						this->solved = true;
						return;
					}
				}
			}
		}
	}

	for (int y = 0;y <= 6;y += 3)// x,y iteracja po blokach
	{
		for (int x = 0;x <= 6;x += 3)
		{
			for (int i = y;i < y + 3;i++) //iteracja po polach w ka�dym bloku
			{
				for (int j = x;j < x + 3;j++)
				{
					if (this->cand[i][j].size() > 0 && this->cand[i][j][0] != 10)
					{
						for (int k = 0;k < this->cand[i][j].size();k++)
						{
							int n = 0;
							for (int a = y;a < y + 3;a++)
								for (int b = x;b < x + 3;b++)
									if (this->cand[a][b][0] != 10 && this->contains(this->cand[a][b], this->cand[i][j][k]))
										n++;
							if (n == 1)
							{
								this->number[i][j] = this->cand[i][j][k];
								this->solved = true;
								return;
							}
						}
					}
				}
			}
		}
	}
}

void Solver::lockedCandidate()
{
	for (int i = 0;i < 9;i++) //iteracja po wierszach
	{
		for (int j = 0;j < 9;j++)
		{
			if (this->cand[i][j].size() > 0 && this->cand[i][j][0] != 10)
			{
				for (int k = 0;k < this->cand[i][j].size();k++) //itreacja po kandydatach w polach
				{
					int exist = 0, line;
					for (int a = 0;a <= 6;a += 3)
					{
						for (int b = a;b < a + 3;b++) //iteracja po polach w linii w tym samym kwadracie
						{
							if (this->contains(this->cand[i][b], this->cand[i][j][k])) //je�li pole zawiera kandydata
							{
								exist++;
								line = a; // ustawienie linii pocz�tkowej wiersza kwadratu tam gdzie wyst�puje kandydat
								break;
							}
						}
					}
					if (exist == 1) //je�eli kandydaci wyst�puj� tylko w polach kt�re nale�y do tylko jednego kwadratu
					{
						int n; //ustalenie linii kolumny
						if (i < 3) n = 0;
						else if (i >= 3 && i < 6) n = 3;
						else if (i >= 6) n = 6;

						for (int y = n;y < n + 3;y++) // iteracja po polach w kwardacie
							for (int x = line;x < line + 3;x++)
								if (y != i)
									for (int a = 0;a < this->cand[y][x].size();a++) //usni�cie kandydata z p�l kwadratu kt�ry nie le�y na linii y 
										if (this->cand[y][x][a] == this->cand[i][j][k])
											this->cand[y][x].erase(this->cand[y][x].begin() + a);
					}
				}
			}
		}
	}

	for (int j = 0;j < 9;j++) //iteracja po kolumnach
	{
		for (int i = 0;i < 9;i++)
		{
			if (this->cand[i][j].size() > 0 && this->cand[i][j][0] != 10)
			{
				for (int k = 0;k < this->cand[i][j].size();k++) //itreacja po kandydatach w polach
				{
					int exist = 0, line;
					for (int a = 0;a <= 6;a += 3)
					{
						for (int b = a;b < a + 3;b++) //iteracja po polach w linii w tym samym kwadracie
						{
							if (this->contains(this->cand[b][j], this->cand[i][j][k])) //je�li pole zawiera kandydata
							{
								exist++;
								line = a; // ustawienie linii pocz�tkowej kolumny kwadratu tam gdzie wyst�puje kandydat
								break;
							}
						}
					}
					if (exist == 1) //je�eli kandydaci wyst�puj� tylko w polach kt�re nale�y do tylko jednego kwadratu
					{
						int n; //ustalenie linii wiersza
						if (j < 3) n = 0;
						else if (j >= 3 && j < 6) n = 3;
						else if (j >= 6) n = 6;

						for (int x = n;x < n + 3;x++) // iteracja po polach w kwardacie
							for (int y = line;y < line + 3;y++)
								if (x != j)
									for (int a = 0;a < this->cand[y][x].size();a++) //susni�cie kandydata z p�l kwadratu kt�ry nie le�y na linii y 
										if (this->cand[y][x][a] == this->cand[i][j][k])
											this->cand[y][x].erase(this->cand[y][x].begin() + a);
					}
				}

			}
		}
	}

	for (int y = 0;y <= 6;y += 3) // iteracja po kwadratach
	{
		for (int x = 0;x <= 6;x += 3)
		{
			for (int i = y;i < y + 3;i++)
			{
				for (int j = x;j < x + 3;j++)
				{
					if (this->cand[i][j].size() > 0 && this->cand[i][j][0] != 10)
					{
						for (int k = 0;k < this->cand[i][j].size();k++)
						{
							int exist = 0;
							for (int a = y;a < y + 3;a++) //sprawdzenie w ilu wierszach wyst�puje kandydat
							{
								for (int b = x;b < x + 3;b++)
								{
									if (this->contains(this->cand[a][b], this->cand[i][j][k]))
									{
										exist++;
										break;
									}
								}
							}
							if (exist == 1)//wyst�puje tylko w 1 wierszu
							{
								int n;
								if (j < 3) n = 0;
								else if (j >= 3 && j < 6) n = 3;
								else if (j >= 6) n = 6;

								for (int a = 0;a < 9;a++) //usuni�cie kandydata z wiersza (z wy��czeniem p�l kt�re s� w kwadracie)
									if (a != n && a != n + 1 && a != n + 2)
										for (int b = 0;b < this->cand[i][a].size();b++)
											if (this->cand[i][a][b] == this->cand[i][j][k])
												this->cand[i][a].erase(this->cand[i][a].begin() + b);
							}
						}
					}
				}
			}
		}
	}
	for (int y = 0;y <= 6;y += 3)
	{
		for (int x = 0;x <= 6;x += 3)
		{
			for (int j = y;j < y + 3;j++)
			{
				for (int i = x;i < x + 3;i++)
				{
					if (this->cand[i][j].size() > 0 && this->cand[i][j][0] != 10)
					{
						for (int k = 0;k < this->cand[i][j].size();k++)
						{
							int exist = 0;
							for (int b = y;b < y + 3;b++)
							{
								for (int a = x;a < x + 3;a++)
								{
									if (this->contains(this->cand[a][b], this->cand[i][j][k]))
									{
										exist++;
										break;
									}
								}
							}
							if (exist == 1)
							{
								int n;
								if (i < 3) n = 0;
								else if (i >= 3 && i < 6) n = 3;
								else if (i >= 6) n = 6;

								for (int a = 0;a < 9;a++)
									if (a != n && a != n + 1 && a != n + 2)
										for (int b = 0;b < this->cand[a][j].size();b++)
											if (this->cand[a][j][b] == this->cand[i][j][k])
												this->cand[a][j].erase(this->cand[a][j].begin() + b);
							}
						}
					}
				}
			}
		}
	}
}

void Solver::openPairs()
{
	for (int i = 0;i < 9;i++) //iteracja po wierszu
	{
		for (int j = 0;j < 9;j++) //iteracja po kolumnie
		{
			if (this->cand[i][j].size() == 2) // szukamy takiego pola kt�re posiada 2 kandydat�w
			{
				for (int a = 0;a < 9;a++) //ponowna iteracja po polach w wierszu
				{
					if (this->cand[i][j] == this->cand[i][a] && j != a) //szukane pole ma takich samych kandydat�w i nie jest polem pierwotnym
					{
						int e1 = this->cand[i][j][0]; // zapisanie tych kandydat�w
						int e2 = this->cand[i][j][1];
						for (int b = 0;b < 9;b++)
						{
							for (int c = 0;c < this->cand[i][b].size();c++) //usuwanie zapisanych kandydat�w z p�l (opr�cz tych p�l kt�re zawieraj� dok�adnie tych kandydat�w)
							{
								if (b != j && b != a && (this->cand[i][b][c] == e1 || this->cand[i][b][c] == e2))
								{
									this->cand[i][b].erase(this->cand[i][b].begin() + c);
								}
							}
						}
					}
				}
			}
		}
	}

	//to samo tylko po kolumnach
	for (int j = 0;j < 9;j++)
	{
		for (int i = 0;i < 9;i++)
		{
			if (this->cand[i][j].size() == 2)
			{
				for (int a = 0;a < 9;a++)
				{
					if (this->cand[i][j] == this->cand[a][j] && i != a)
					{
						int e1 = this->cand[i][j][0];
						int e2 = this->cand[i][j][1];
						for (int b = 0;b < 9;b++)
						{
							for (int c = 0;c < this->cand[b][j].size();c++)
							{
								if (b != i && b != a && (this->cand[b][j][c] == e1 || this->cand[b][j][c] == e2))
								{
									this->cand[b][j].erase(this->cand[b][j].begin() + c);
								}
							}
						}
					}
				}
			}
		}
	}

	for (int y = 0;y <= 6;y += 3) // x,y - p�tle odpowiedzialne za przemieszczanie si� pobi�dzy 9 boxami 
	{
		for (int x = 0;x <= 6;x += 3)
		{
			for (int i = y;i < y + 3;i++) // iterazja poszczeg�lnych p�l w boxie
			{
				for (int j = x;j < x + 3;j++)
				{
					if (this->cand[i][j].size() == 2) // szukamy takiego pola kt�re posiada 2 kandydat�w
					{
						for (int a = y;a < y + 3;a++) //ponowna iteracja po polach w boxie by znale�� pole z takimi samymi kandydatami
						{
							for (int b = x;b < x + 3;b++)
							{
								if (this->cand[i][j] == this->cand[a][b] && i != a && a != b) //szukane pole ma takich samych kandydat�w i nie jest polem pierwotnym
								{
									int e1 = this->cand[i][j][0]; //zapisanie tych kandydat�w
									int e2 = this->cand[i][j][1];
									for (int c = y;c < y + 3;c++) //iteracja po polach w boxie
									{
										for (int d = x;d < x + 3;d++)
										{
											for (int e = 0;e < this->cand[c][d].size();e++) //usuwanie zapisanych kandydat�w z p�l (opr�cz tych p�l kt�re zawieraj� dok�adnie tych kandydat�w)
											{
												if (this->cand[c][d] != this->cand[i][j] && this->cand[c][d] != this->cand[a][b] && (this->cand[c][d][e] == e1 || this->cand[c][d][e] == e2))
												{
													this->cand[c][d].erase(this->cand[c][d].begin() + e);
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void Solver::openTriples()
{
	for (int i = 0;i < 9;i++)
	{
		for (int j = 0;j < 9;j++)
		{
			if (this->cand[i][j].size() == 3) //pola o wieko�ci 3
			{
				std::vector<int> line;
				for (int a = 0;a < 9;a++)
				{
					if (this->cand[i][a] == this->cand[i][j]) //je�eli pole jest takie samo
					{
						line.push_back(a);
					}
					if (this->cand[i][a].size() == 2) // je�eli pole jes wielko�ci 2
					{
						int exist = 0;
						for (int b = 0;b < this->cand[i][a].size();b++)
							if (this->contains(this->cand[i][j], this->cand[i][a][b]))
								exist++;
						if (exist == 2) // to pole zawiera 2 takich samych kandydat�w jak pole pierwotne
							line.push_back(a);
					}
				}
				if (line.size() == 3) // je�eli istniej� 3 takie pola
				{
					for (int a = 0;a < 9;a++)
						if (a != line[0] && a != line[1] && a != line[2])
							for (int b = 0;b < this->cand[i][a].size();b++)
							{
								if (this->cand[i][a][b] == this->cand[i][j][0])
									this->cand[i][a].erase(this->cand[i][a].begin() + b);
								if (this->cand[i][a][b] == this->cand[i][j][1])
									this->cand[i][a].erase(this->cand[i][a].begin() + b);
								if (this->cand[i][a][b] == this->cand[i][j][2])
									this->cand[i][a].erase(this->cand[i][a].begin() + b);
							}

				}
			}
		}
	}

	for (int j = 0;j < 9;j++)
	{
		for (int i = 0;i < 9;i++)
		{
			if (this->cand[i][j].size() == 3)
			{
				std::vector<int> line;
				for (int a = 0;a < 9;a++)
				{
					if (this->cand[a][j] == this->cand[i][j])
					{
						line.push_back(a);
					}
					if (this->cand[a][j].size() == 2)
					{
						int exist = 0;
						for (int b = 0;b < this->cand[a][j].size();b++)
							if (this->contains(this->cand[i][j], this->cand[a][j][b]))
								exist++;
						if (exist == 2)
							line.push_back(a);
					}
				}
				if (line.size() == 3)
				{
					for (int a = 0;a < 9;a++)
						if (a != line[0] && a != line[1] && a != line[2])
							for (int b = 0;b < this->cand[a][j].size();b++)
							{
								if (this->cand[a][j][b] == this->cand[i][j][0])
									this->cand[a][j].erase(this->cand[a][j].begin() + b);
								if (this->cand[a][j][b] == this->cand[i][j][1])
									this->cand[a][j].erase(this->cand[a][j].begin() + b);
								if (this->cand[a][j][b] == this->cand[i][j][2])
									this->cand[a][j].erase(this->cand[a][j].begin() + b);
							}
				}
			}
		}
	}

	for (int y = 0;y <= 6;y += 3) // x,y - p�tle odpowiedzialne za przemieszczanie si� pobi�dzy 9 boxami 
	{
		for (int x = 0;x <= 6;x += 3)
		{
			for (int i = y;i < y + 3;i++) // iterazja poszczeg�lnych p�l w boxie
			{
				for (int j = x;j < x + 3;j++)
				{
					if (this->cand[i][j].size() == 3)
					{
						std::vector<std::vector<int>>box;
						for (int a = y;a < y + 3;a++)
						{
							for (int b = x;b < x + 3;b++)
							{
								if (this->cand[a][b] == this->cand[i][j])
								{
									box.push_back(this->cand[a][b]);
								}
								if (this->cand[a][b].size() == 2)
								{
									int exist = 0;
									for (int c = 0;c < this->cand[a][b].size();c++)
										if (this->contains(this->cand[i][j], this->cand[a][b][c]))
											exist++;
									if (exist == 2)
									{
										box.push_back(this->cand[a][b]);
									}
								}
							}
						}
						if (box.size() == 3)
						{
							for (int a = y;a < y + 3;a++)
								for (int b = x;b < x + 3;b++)
									if (this->cand[a][b] != box[0] && this->cand[a][b] != box[1] && this->cand[a][b] != box[2])
										for (int c = 0;c < this->cand[a][b].size();c++)
										{
											if (this->cand[a][b][c] == this->cand[i][j][0])
												this->cand[a][b].erase(this->cand[a][b].begin() + c);
											if (this->cand[a][b][c] == this->cand[i][j][1])
												this->cand[a][b].erase(this->cand[a][b].begin() + c);
											if (this->cand[a][b][c] == this->cand[i][j][2])
												this->cand[a][b].erase(this->cand[a][b].begin() + c);
										}
						}
					}
				}
			}
		}
	}

}

void Solver::hiddenPairs()
{
	for (int i = 0;i < 9;i++)
	{
		for (int a = 1;a <= 9;a++) //iteracja po wszystkich mo�liwych kandydatach
		{
			int exist1 = 0;
			for (int j = 0;j < 9;j++)
				if (this->contains(this->cand[i][j], a))
					exist1++;

			if (exist1 == 2) //je�eli kandydat wyst�puje tylko 2 razy w wierszu
			{
				for (int b = 1;b <= 9;b++) // iteracja po kandydatach r�nych od pierwszego (a)
				{
					int exist2 = 0;
					if (b != a)
						for (int c = 0;c < 9;c++)
							if (this->contains(this->cand[i][c], b))
								exist2++;

					if (exist2 == 2) //je�eli kandydat wyst�puje tylko 2 razy w wierszu
					{
						//sprawdzenie czy ci kandydaci wyst�puj� w 2 tych samych polach
						std::vector<int> sameBoxes; //koordynata bloku w kt�rym wyst�puj� kandydaci
						for (int c = 0;c < 9;c++)
							if (this->contains(this->cand[i][c], a) && this->contains(this->cand[i][c], b))
								sameBoxes.push_back(c);

						if (sameBoxes.size() == 2) //j�zeli wyst�puj� w tych samych polach - usuni�cie pozosta�ych kandydat�w z tych 2 p�l
						{
							if (this->cand[i][sameBoxes[0]].size() > 2)
								for (int c = 0;c < this->cand[i][sameBoxes[0]].size();c++)
									if (a != this->cand[i][sameBoxes[0]][c] && b != this->cand[i][sameBoxes[0]][c])
										this->cand[i][sameBoxes[0]].erase(this->cand[i][sameBoxes[0]].begin() + c);

							if (this->cand[i][sameBoxes[1]].size() > 2)
								for (int c = 0;c < this->cand[i][sameBoxes[1]].size();c++)
									if (a != this->cand[i][sameBoxes[1]][c] && b != this->cand[i][sameBoxes[1]][c])
										this->cand[i][sameBoxes[1]].erase(this->cand[i][sameBoxes[1]].begin() + c);
						}
					}
				}
			}
		}
	}

	for (int j = 0;j < 9;j++)
	{
		for (int a = 1;a <= 9;a++)
		{
			int exist1 = 0;
			for (int i = 0;i < 9;i++)
				if (this->contains(this->cand[i][j], a))
					exist1++;

			if (exist1 == 2)
			{
				for (int b = 1;b <= 9;b++)
				{
					int exist2 = 0;
					if (b != a)
						for (int c = 0;c < 9;c++)
							if (this->contains(this->cand[c][j], b))
								exist2++;

					if (exist2 == 2)
					{
						std::vector<int> sameBoxes;
						for (int c = 0;c < 9;c++)
							if (this->contains(this->cand[c][j], a) && this->contains(this->cand[c][j], b))
								sameBoxes.push_back(c);

						if (sameBoxes.size() == 2)
						{
							if (this->cand[sameBoxes[0]][j].size() > 2)
								for (int c = 0;c < this->cand[sameBoxes[0]][j].size();c++)
									if (a != this->cand[sameBoxes[0]][j][c] && b != this->cand[sameBoxes[0]][j][c])
										this->cand[sameBoxes[0]][j].erase(this->cand[sameBoxes[0]][j].begin() + c);

							if (this->cand[sameBoxes[1]][j].size() > 2)
								for (int c = 0;c < this->cand[sameBoxes[1]][j].size();c++)
									if (a != this->cand[sameBoxes[1]][j][c] && b != this->cand[sameBoxes[1]][j][c])
										this->cand[sameBoxes[1]][j].erase(this->cand[sameBoxes[1]][j].begin() + c);
						}
					}
				}
			}
		}
	}

	for (int y = 0;y <= 6;y += 3)
	{
		for (int x = 0;x <= 6;x += 3)
		{
			for (int a = 1;a <= 9;a++)
			{
				int exist1 = 0;
				for (int i = y;i < y + 3;i++)
					for (int j = x;j < x + 3;j++)
						if (this->contains(this->cand[i][j], a))
							exist1++;

				if (exist1 == 2)
				{
					for (int b = 1;b <= 9;b++)
					{
						int exist2 = 0;
						if (b != a)
							for (int i = y;i < y + 3;i++)
								for (int j = x;j < x + 3;j++)
									if (this->contains(this->cand[i][j], b))
										exist2++;

						if (exist2 == 2)
						{
							std::vector<int> sameBoxesY, sameBoxesX;
							for (int i = y;i < y + 3;i++)
								for (int j = x;j < x + 3;j++)
									if (this->contains(this->cand[i][j], a) && this->contains(this->cand[i][j], b))
									{
										sameBoxesY.push_back(i);
										sameBoxesX.push_back(j);
									}

							if (sameBoxesX.size() == 2)
							{
								if (this->cand[sameBoxesY[0]][sameBoxesX[0]].size() > 2)
									for (int c = 0;c < this->cand[sameBoxesY[0]][sameBoxesX[0]].size();c++)
										if (a != this->cand[sameBoxesY[0]][sameBoxesX[0]][c] && b != this->cand[sameBoxesY[0]][sameBoxesX[0]][c])
											this->cand[sameBoxesY[0]][sameBoxesX[0]].erase(this->cand[sameBoxesY[0]][sameBoxesX[0]].begin() + c);

								if (this->cand[sameBoxesY[1]][sameBoxesX[1]].size() > 2)
									for (int c = 0;c < this->cand[sameBoxesY[1]][sameBoxesX[1]].size();c++)
										if (a != this->cand[sameBoxesY[1]][sameBoxesX[1]][c] && b != this->cand[sameBoxesY[1]][sameBoxesX[1]][c])
											this->cand[sameBoxesY[1]][sameBoxesX[1]].erase(this->cand[sameBoxesY[1]][sameBoxesX[1]].begin() + c);
							}
						}
					}
				}
			}
		}
	}
}

void Solver::xWing()
{
	//wiersze
	for (int i = 0;i < 9;i++)
	{
		for (int j = 0;j < 9;j++)
		{
			if (this->cand[i][j].size() > 0 && this->cand[i][j][0] != 10) // pole nie jest uzupe�nione
			{
				for (int k = 0;k < this->cand[i][j].size();k++) //iteracja po kandydatach w polu
				{
					std::vector<int> columnCoord;
					for (int a = 0;a < 9;a++) //ponowna iteracja po wszystkich polach w wierszu aby sparawdzi� ile jest tych kandydat�w
						if (this->cand[i][a][0] != 10 && this->contains(this->cand[i][a], this->cand[i][j][k]))
							columnCoord.push_back(a);
					if (columnCoord.size() == 2) //je�eli jest tylko 2 kandydat�w
					{
						for (int b = 0;b < 9;b++)
						{
							//sprawdzenie czy w innym wierszu wyst�puje tylko 2 razy ten sam kandydat
							int n = 0;
							for (int c = 0;c < 9;c++)
								if (b != i && this->contains(this->cand[b][c], this->cand[i][j][k]))
									n++;
							//je�li wyst�puje tylko 2 razy oraz na w tych samych kolumnach
							if (n == 2 && this->contains(this->cand[b][columnCoord[0]], this->cand[i][j][k]) && this->contains(this->cand[b][columnCoord[1]], this->cand[i][j][k]))
							{
								//usuni�cie tych kandydat�w z pozosta�ych 2 kolumn
								for (int c = 0;c < 9;c++)
								{
									for (int d = 0;d < this->cand[c][columnCoord[0]].size();d++)
									{
										if (c != b && c != i && this->cand[c][columnCoord[0]][d] == this->cand[i][j][k])
										{
											this->cand[c][columnCoord[0]].erase(this->cand[c][columnCoord[0]].begin() + d);
										}
									}
								}
								for (int c = 0;c < 9;c++)
								{
									for (int d = 0;d < this->cand[c][columnCoord[1]].size();d++)
									{
										if (c != b && c != i && this->cand[c][columnCoord[1]][d] == this->cand[i][j][k])
										{
											this->cand[c][columnCoord[1]].erase(this->cand[c][columnCoord[1]].begin() + d);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	//kolumny
	for (int j = 0;j < 9;j++)
	{
		for (int i = 0;i < 9;i++)
		{
			if (this->cand[i][j].size() > 0 && this->cand[i][j][0] != 10) // pole nie jest uzupe�nione
			{
				for (int k = 0;k < this->cand[i][j].size();k++) //iteracja po kandydatach w polu
				{
					std::vector<int> columnCoord;
					for (int a = 0;a < 9;a++) //ponowna iteracja po wszystkich polach w wierszu aby sparawdzi� ile jest tych kandydat�w
						if (this->cand[a][j][0] != 10 && this->contains(this->cand[a][j], this->cand[i][j][k]))
							columnCoord.push_back(a);
					if (columnCoord.size() == 2) //je�eli jest tylko 2 kandydat�w
					{
						for (int b = 0;b < 9;b++)
						{
							int n = 0;
							for (int c = 0;c < 9;c++)
								if (b != j && this->contains(this->cand[c][b], this->cand[i][j][k]))
									n++;
							if (n == 2 && this->contains(this->cand[columnCoord[0]][b], this->cand[i][j][k]) && this->contains(this->cand[columnCoord[1]][b], this->cand[i][j][k]))
							{
								for (int c = 0;c < 9;c++)
								{
									for (int d = 0;d < this->cand[columnCoord[0]][c].size();d++)
									{
										if (c != b && c != j && this->cand[columnCoord[0]][c][d] == this->cand[i][j][k])
										{
											this->cand[columnCoord[0]][c].erase(this->cand[columnCoord[0]][c].begin() + d);
										}
									}
								}
								for (int c = 0;c < 9;c++)
								{
									for (int d = 0;d < this->cand[columnCoord[1]][c].size();d++)
									{
										if (c != b && c != j && this->cand[columnCoord[1]][c][d] == this->cand[i][j][k])
										{
											this->cand[columnCoord[1]][c].erase(this->cand[columnCoord[1]][c].begin() + d);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void Solver::solve()
{
	this->findCandidate();

	this->singleCandidate();
	if (!this->solved)
		this->hiddenLoner();

	if (!this->solved)
	{
		for (int i = 0;i < 5;i++)
		{
			this->lockedCandidate();
			this->openPairs();
			this->openTriples();
			this->hiddenPairs();
			this->xWing();
			this->singleCandidate();
			if (!this->solved)
				this->hiddenLoner();
			if (this->solved)
				break;
		}
	}

	if (!this->solved)
		this->error = true;
	this->solved = false;
}

std::array<std::array<int, 9>, 9> Solver::returnNumber()
{
	return this->number;
}

std::vector<std::vector<std::vector<int>>> Solver::returnCand()
{
	this->findCandidate();
	return this->cand;
}

bool Solver::returnError()
{
	return this->error;
}