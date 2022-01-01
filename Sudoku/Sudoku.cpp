#include "Sudoku.hpp"

//Private functions
void Sudoku::initWindow()
{
	this->videoMode.height = 700;
	this->videoMode.width = 1000;
	this->window = new sf::RenderWindow(this->videoMode, "Sudoku solver", sf::Style::Close | sf::Style::Titlebar);
	this->window->setFramerateLimit(60);
}

void Sudoku::initBoard()
{
	this->board.setPosition(0.f,0.f);
	this->board.setSize(sf::Vector2f(700.f, 700.f));
	this->boardTexture.loadFromFile("Textures/board.png");
	this->board.setTexture(&this->boardTexture);

	this->resetButton.setPosition(750.f, 300.f);
	this->resetButton.setSize(sf::Vector2f(200.f, 100.f));
	this->resetTexture[0].loadFromFile("Textures/reset.png");
	this->resetButton.setTexture(&this->resetTexture[0]);

	this->stepButton.setPosition(750.f, 100.f);
	this->stepButton.setSize(sf::Vector2f(200.f, 100.f));
	this->stepTexture[0].loadFromFile("Textures/onestep.png");
	this->stepButton.setTexture(&this->stepTexture[0]);

	this->solveButton.setPosition(750.f, 200.f);
	this->solveButton.setSize(sf::Vector2f(200.f, 100.f));
	this->solveTexture[0].loadFromFile("Textures/solve.png");
	this->solveButton.setTexture(&this->solveTexture[0]);

	this->saveButton.setPosition(750.f, 400.f);
	this->saveButton.setSize(sf::Vector2f(200.f, 100.f));
	this->saveTexture[0].loadFromFile("Textures/save.png");
	this->saveButton.setTexture(&this->saveTexture[0]);

	this->loadButton.setPosition(750.f, 500.f);
	this->loadButton.setSize(sf::Vector2f(200.f, 100.f));
	this->loadTexture[0].loadFromFile("Textures/load.png");
	this->loadButton.setTexture(&this->loadTexture[0]);

	this->loadTexture[1].loadFromFile("Textures/load-hover.png");
	this->saveTexture[1].loadFromFile("Textures/save-hover.png");
	this->solveTexture[1].loadFromFile("Textures/solve-hover.png");
	this->stepTexture[1].loadFromFile("Textures/onestep-hover.png");
	this->resetTexture[1].loadFromFile("Textures/reset-hover.png");
}

void Sudoku::drawBoard()
{
	this->window->draw(this->board);
	this->window->draw(this->resetButton);
	this->window->draw(this->solveButton);
	this->window->draw(this->stepButton);
	this->window->draw(this->loadButton);
	this->window->draw(this->saveButton);
}

void Sudoku::initBoxes()
{
	this->greyColor = new sf::Color(186, 181, 181);
	for (int i = 0;i < 10;i++)
	{
		std::string t = "Textures/" + std::to_string(i) + ".png";
		boxTextures[i].loadFromFile(t);
	}

	int y = 14;
	for (int i = 0;i < 9;i++)
	{
		int x = 14;
		for (int j = 0;j < 9;j++)
		{
			box[i][j].setFillColor(sf::Color::White);
			box[i][j].setSize(sf::Vector2f(70.f, 70.f));
			box[i][j].setPosition(x, y);
			x += 74;
			if (j == 2 || j == 5)
				x += 5;
		}
		y += 74;
		if (i == 2 || i == 5)
			y += 5;
	}
}

void Sudoku::drawBoxes()
{
	for(int i = 0;i < 9;i++)
		for (int j = 0;j < 9;j++)
			this->window->draw(box[i][j]);
}

//Public functions
Sudoku::Sudoku()
{
	this->initWindow();
	this->initBoard();
	this->initBoxes();
}

Sudoku::~Sudoku()
{
	delete this->window;
}

const bool Sudoku::running() const
{
	return this->window->isOpen();
}

void Sudoku::poolEvent()
{
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;
		}
	}
}

void Sudoku::updateMousePosition() //aktualizacja pozycji myszy wzglêcem okna
{
	this->mousePosition = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));
}

void Sudoku::updateButtons() //zmiana koloru przycisków przy najechaniu kursorem na nie
{
	if (this->resetButton.getGlobalBounds().contains(this->mousePosition))
		this->resetButton.setTexture(&this->resetTexture[1]);
	else
		this->resetButton.setTexture(&this->resetTexture[0]);

	if (this->solveButton.getGlobalBounds().contains(this->mousePosition))
		this->solveButton.setTexture(&this->solveTexture[1]);
	else
		this->solveButton.setTexture(&this->solveTexture[0]);

	if (this->stepButton.getGlobalBounds().contains(this->mousePosition))
		this->stepButton.setTexture(&this->stepTexture[1]);
	else
		this->stepButton.setTexture(&this->stepTexture[0]);

	if (this->loadButton.getGlobalBounds().contains(this->mousePosition))
		this->loadButton.setTexture(&this->loadTexture[1]);
	else
		this->loadButton.setTexture(&this->loadTexture[0]);

	if (this->saveButton.getGlobalBounds().contains(this->mousePosition))
		this->saveButton.setTexture(&this->saveTexture[1]);
	else
		this->saveButton.setTexture(&this->saveTexture[0]);
}

void Sudoku::updateBoxes()
{
	//zaznaczenie pola w które bêdziemy wpisywaæ cyfrê
	static bool pressed = false;
	for (int i = 0;i < 9;i++)
	{
		for (int j = 0;j < 9;j++)
		{
			if (this->box[i][j].getGlobalBounds().contains(this->mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !pressed)
			{
				pressed = true;
				if (this->checkedX == j && this->checkedY == i)
				{
					this->box[i][j].setFillColor(sf::Color::White);
					checkedX = -1;
					checkedY = -1;
				}
				else if(this->checkedX != -1 && this->checkedY != -1)
				{
					this->box[this->checkedY][this->checkedX].setFillColor(sf::Color::White);
					this->box[i][j].setFillColor(*this->greyColor);
					this->checkedX = j;
					this->checkedY = i;
				}
				else
				{
					this->box[i][j].setFillColor(*this->greyColor);
					this->checkedX = j;
					this->checkedY = i;
				}
			}
		}
	}
	if (this->ev.type == sf::Event::MouseButtonReleased)
		if (this->ev.mouseButton.button == sf::Mouse::Left)
			pressed = false;

	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) && !pressed && this->window->hasFocus())
	{
		pressed = true;
		if (this->checkedX == -1 && this->checkedY == -1)
		{
			this->box[0][0].setFillColor(*this->greyColor);
			this->checkedX = 0;
			this->checkedY = 0;
		}
		else if (this->checkedX == 8 && this->checkedY != 8)
		{
			this->box[this->checkedY][this->checkedX].setFillColor(sf::Color::White);
			this->checkedX = 0;
			this->checkedY++;
			this->box[this->checkedY][this->checkedX].setFillColor(*this->greyColor);
		}
		else if (this->checkedY == 8 && this->checkedX == 8)
		{
			this->box[this->checkedY][this->checkedX].setFillColor(sf::Color::White);
			this->checkedX = 0;
			this->checkedY = 0;
			this->box[this->checkedY][this->checkedX].setFillColor(*this->greyColor);
		}
		else
		{
			this->box[this->checkedY][this->checkedX].setFillColor(sf::Color::White);
			this->checkedX++;
			this->box[this->checkedY][this->checkedX].setFillColor(*this->greyColor);
		}
	}
	else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && !pressed && this->window->hasFocus())
	{
		pressed = true;
		if (this->checkedX == -1 && this->checkedY == -1)
		{
			this->box[0][0].setFillColor(*this->greyColor);
			this->checkedX = 0;
			this->checkedY = 0;
		}
		else
		{
			this->box[this->checkedY][this->checkedX].setFillColor(sf::Color::White);
			this->checkedY--;
			if (checkedY < 0)
				this->checkedY = 8;
			this->box[this->checkedY][this->checkedX].setFillColor(*this->greyColor);
		}
	}
	else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) && !pressed && this->window->hasFocus())
	{
		if (this->checkedX == -1 && this->checkedY == -1)
		{
			this->box[0][0].setFillColor(*this->greyColor);
			this->checkedX = 0;
			this->checkedY = 0;
		}
		else
		{
			pressed = true;
			this->box[this->checkedY][this->checkedX].setFillColor(sf::Color::White);
			this->checkedY++;
			if (checkedY > 8)
				this->checkedY = 0;
			this->box[this->checkedY][this->checkedX].setFillColor(*this->greyColor);
		}
	}
	else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) && !pressed && this->window->hasFocus())
	{
		if (this->checkedX == -1 && this->checkedY == -1)
		{
			this->box[0][0].setFillColor(*this->greyColor);
			this->checkedX = 0;
			this->checkedY = 0;
		}
		else
		{
			pressed = true;
			this->box[this->checkedY][this->checkedX].setFillColor(sf::Color::White);
			this->checkedX--;
			if (checkedX < 0)
				this->checkedX = 8;
			this->box[this->checkedY][this->checkedX].setFillColor(*this->greyColor);
		}
	}
	else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) && !pressed && this->window->hasFocus())
	{
		if (this->checkedX == -1 && this->checkedY == -1)
		{
			this->box[0][0].setFillColor(*this->greyColor);
			this->checkedX = 0;
			this->checkedY = 0;
		}
		else
		{
			pressed = true;
			this->box[this->checkedY][this->checkedX].setFillColor(sf::Color::White);
			this->checkedX++;
			if (checkedX > 8)
				this->checkedX = 0;
			this->box[this->checkedY][this->checkedX].setFillColor(*this->greyColor);
		}
	}


	if (this->ev.type == sf::Event::KeyReleased)
		if (this->ev.mouseButton.button == sf::Keyboard::Tab || this->ev.mouseButton.button == sf::Keyboard::Up ||
			this->ev.mouseButton.button == sf::Keyboard::Down || this->ev.mouseButton.button == sf::Keyboard::Left ||
			this->ev.mouseButton.button == sf::Keyboard::Right)
			pressed = false;


	//wypisanie cyfry z klawiatury
	if (this->checkedX != -1 && this->checkedY != -1 && this->window->hasFocus())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
		{
			this->box[this->checkedY][this->checkedX].setTexture(&boxTextures[0]);
			this->number[this->checkedY][this->checkedX] = 0;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
		{	
			this->box[this->checkedY][this->checkedX].setTexture(&boxTextures[1]);
			this->number[this->checkedY][this->checkedX] = 1;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
		{
			this->box[this->checkedY][this->checkedX].setTexture(&boxTextures[2]);
			this->number[this->checkedY][this->checkedX] = 2;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
		{
			this->box[this->checkedY][this->checkedX].setTexture(&boxTextures[3]);
			this->number[this->checkedY][this->checkedX] = 3;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
		{
			this->box[this->checkedY][this->checkedX].setTexture(&boxTextures[4]);
			this->number[this->checkedY][this->checkedX] = 4;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
		{
			this->box[this->checkedY][this->checkedX].setTexture(&boxTextures[5]);
			this->number[this->checkedY][this->checkedX] = 5;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
		{
			this->box[this->checkedY][this->checkedX].setTexture(&boxTextures[6]);
			this->number[this->checkedY][this->checkedX] = 6;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
		{
			this->box[this->checkedY][this->checkedX].setTexture(&boxTextures[7]);
			this->number[this->checkedY][this->checkedX] = 7;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
		{
			this->box[this->checkedY][this->checkedX].setTexture(&boxTextures[8]);
			this->number[this->checkedY][this->checkedX] = 8;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
		{
			this->box[this->checkedY][this->checkedX].setTexture(&boxTextures[9]);
			this->number[this->checkedY][this->checkedX] = 9;
		}
	}
}

void Sudoku::menu()
{
	static bool pressed;
	if (this->stepButton.getGlobalBounds().contains(this->mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !pressed)
	{
		pressed = true;
		this->solve();
		std::cout << "Kandydaci:"<<std::endl;
		for (int i = 0;i < 9;i++)
		{
			for (int j = 0;j < 9;j++)
			{
				for (int k = 0;k < cand[i][j].size();k++)
				{
					std::cout << cand[i][j][k] << " ";
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;	
	}
	else if (this->solveButton.getGlobalBounds().contains(this->mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !pressed)
	{
		pressed = true;
		while (true)
		{
			this->solve();
			int checked = 0;
			for (int i = 0;i < 9;i++)
				for (int j = 0;j < 9;j++)
					if (number[i][j] != 0)
							checked++;
			if (checked == 81 || this->error)
			{
				this->error = false;
				break;
			}
		}
	}
	else if (this->resetButton.getGlobalBounds().contains(this->mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		for (int i = 0;i < 9;i++)
		{
			for (int j = 0;j < 9;j++)
			{
				this->box[i][j].setTexture(&this->boxTextures[0]);
				this->number[i][j] = 0;
			}
		}
	}
	else if (this->loadButton.getGlobalBounds().contains(this->mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		Load load;
		load.readFile();
		this->number = load.returnNumber();
	}
	else if (this->saveButton.getGlobalBounds().contains(this->mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		Save save(this->number);
		save.saveFile();
	}

	if (this->ev.type == sf::Event::MouseButtonReleased)
		if (this->ev.mouseButton.button == sf::Mouse::Left)
			pressed = false;


	for (int i = 0;i < 9;i++)
	{
		for (int j = 0;j < 9;j++)
		{
			this->box[i][j].setTexture(&boxTextures[this->number[i][j]]);
		}
	}
}

bool Sudoku::existInSquare(int y,int x,int k)
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

bool Sudoku::contains(std::vector<int> vec, const int& elem)
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

void Sudoku::findCandidate()
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

void Sudoku::singleCandidate()
{
	for (int i = 0;i < 9;i++)
	{
		for (int j = 0;j < 9;j++)
		{
			if (this->cand[i][j].size() == 1 && this->cand[i][j][0] != 10)
			{
				this->box[i][j].setTexture(&boxTextures[this->cand[i][j][0]]);
				this->number[i][j] = this->cand[i][j][0];
				this->solved = true;
				return;
			}
		}
	}
}

void Sudoku::hiddenLoner() //ukryty kandydat
{
	for (int i = 0;i < 9;i++) //iteracja po wierszach
	{
		for (int j = 0;j < 9;j++)
		{
			if (this->cand[i][j][0] != 10) // pole nie jest uzupe³nione
			{
				for (int k = 0;k < this->cand[i][j].size();k++) //iteracja po kandydatach w polu
				{
					int n = 0;
					for (int a = 0;a < 9;a++) //ponowna iteracja po wszystkich polach w wierszu aby sparawdziæ ile jest tych kandydatów
						if (this->cand[i][a][0] != 10 && this->contains(this->cand[i][a], this->cand[i][j][k]))
							n++;
					if (n == 1) //je¿eli jest tylko 1 kandydat: uzupe³nienie bloku tym kandydatem
					{
						this->box[i][j].setTexture(&boxTextures[this->cand[i][j][k]]);
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
			if (this->cand[i][j][0] != 10)
			{
				for (int k = 0;k < this->cand[i][j].size();k++)
				{
					int n = 0;
					for (int a = 0;a < 9;a++)
						if (this->cand[a][j][0] != 10 && this->contains(this->cand[a][j], this->cand[i][j][k]))
							n++;
					if (n == 1)
					{
						this->box[i][j].setTexture(&boxTextures[this->cand[i][j][k]]);
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
			for (int i = y;i < y+3;i++) //iteracja po polach w ka¿dym bloku
			{
				for (int j = x;j < x+3;j++)
				{
					if (this->cand[i][j][0] != 10)
					{
						for (int k = 0;k < this->cand[i][j].size();k++)
						{
							int n = 0;
							for (int a = y;a < y+3;a++)
								for (int b = x;b <x+3;b++)
									if (this->cand[a][b][0] != 10 && this->contains(this->cand[a][b], this->cand[i][j][k]))
										n++;
							if (n == 1)
							{
								this->box[i][j].setTexture(&boxTextures[this->cand[i][j][k]]);
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

void Sudoku::lockedCandidate()
{
	for (int i = 0;i < 9;i++) //iteracja po wierszach
	{
		for (int j = 0;j < 9;j++)
		{
			if (this->cand[i][j][0] != 10)
			{
				for (int k = 0;k < this->cand[i][j].size();k++) //itreacja po kandydatach w polach
				{
					int exist = 0, line;
					for (int a = 0;a <= 6;a += 3)
					{
						for (int b = a;b < a + 3;b++) //iteracja po polach w linii w tym samym kwadracie
						{
							if (this->contains(this->cand[i][b], this->cand[i][j][k])) //jeœli pole zawiera kandydata
							{
								exist++; 
								line = a; // ustawienie linii pocz¹tkowej wiersza kwadratu tam gdzie wystêpuje kandydat
								break;
							}
						}
					}
					if (exist == 1) //je¿eli kandydaci wystêpuj¹ tylko w polach które nale¿y do tylko jednego kwadratu
					{
						int n; //ustalenie linii kolumny
						if (i < 3) n = 0;
						else if (i >= 3 && i < 6) n = 3;
						else if (i >= 6) n = 6;

						for (int y = n;y < n + 3;y++) // iteracja po polach w kwardacie
							for (int x = line;x < line + 3;x++)
								if (y != i)
									for (int a = 0;a < this->cand[y][x].size();a++) //usniêcie kandydata z pól kwadratu który nie le¿y na linii y 
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
			if (this->cand[i][j][0] != 10)
			{
				for (int k = 0;k < this->cand[i][j].size();k++) //itreacja po kandydatach w polach
				{
					int exist = 0, line;
					for (int a = 0;a <= 6;a += 3)
					{
						for (int b = a;b < a + 3;b++) //iteracja po polach w linii w tym samym kwadracie
						{
							if (this->contains(this->cand[b][j], this->cand[i][j][k])) //jeœli pole zawiera kandydata
							{
								exist++;
								line = a; // ustawienie linii pocz¹tkowej kolumny kwadratu tam gdzie wystêpuje kandydat
								break;
							}
						}
					}
					if (exist == 1) //je¿eli kandydaci wystêpuj¹ tylko w polach które nale¿y do tylko jednego kwadratu
					{
						int n; //ustalenie linii wiersza
						if (j < 3) n = 0;
						else if (j >= 3 && j < 6) n = 3;
						else if (j >= 6) n = 6;

						for (int x = n;x < n + 3;x++) // iteracja po polach w kwardacie
							for (int y = line;y < line + 3;y++)
								if (x != j)
									for (int a = 0;a < this->cand[y][x].size();a++) //susniêcie kandydata z pól kwadratu który nie le¿y na linii y 
										if (this->cand[y][x][a] == this->cand[i][j][k])
											this->cand[y][x].erase(this->cand[y][x].begin() + a);
					}
				}
			
			}
		}
	}

	for (int y = 0;y <= 6;y+=3) // iteracja po kwadratach
	{
		for (int x = 0;x <= 6;x+=3)
		{
			for (int i = y;i < y + 3;i++)
			{
				for (int j = x;j < x + 3;j++)
				{
					if (this->cand[i][j][0] != 10)
					{
						for (int k = 0;k < this->cand[i][j].size();k++)
						{
							int exist = 0;
							for (int a = y;a < y + 3;a++) //sprawdzenie w ilu wierszach wystêpuje kandydat
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
							if (exist == 1)//wystêpuje tylko w 1 wierszu
							{								
								int n;
								if (j < 3) n = 0;
								else if (j >= 3 && j < 6) n = 3;
								else if (j >= 6) n = 6;

								for (int a = 0;a < 9;a++) //usuniêcie kandydata z wiersza (z wy³¹czeniem pól które s¹ w kwadracie)
									if(a != n && a != n+1 && a != n+2)
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
					if (this->cand[i][j][0] != 10)
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

void Sudoku::openPairs()
{
	for (int i = 0;i < 9;i++) //iteracja po wierszu
	{
		for (int j = 0;j < 9;j++) //iteracja po kolumnie
		{
			if (this->cand[i][j].size() == 2) // szukamy takiego pola które posiada 2 kandydatów
			{
				for (int a = 0;a < 9;a++) //ponowna iteracja po polach w wierszu
				{
					if (this->cand[i][j] == this->cand[i][a] && j != a) //szukane pole ma takich samych kandydatów i nie jest polem pierwotnym
					{
						int e1 = this->cand[i][j][0]; // zapisanie tych kandydatów
						int e2 = this->cand[i][j][1];
						for (int b = 0;b < 9;b++)
						{
							for (int c = 0;c < this->cand[i][b].size();c++) //usuwanie zapisanych kandydatów z pól (oprócz tych pól które zawieraj¹ dok³adnie tych kandydatów)
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

	for (int y = 0;y <= 6;y += 3) // x,y - pêtle odpowiedzialne za przemieszczanie siê pobiêdzy 9 boxami 
	{
		for (int x = 0;x <= 6;x += 3)
		{
			for (int i = y;i < y + 3;i++) // iterazja poszczególnych pól w boxie
			{
				for (int j = x;j < x + 3;j++)
				{
					if (this->cand[i][j].size() == 2) // szukamy takiego pola które posiada 2 kandydatów
					{
						for (int a = y;a < y + 3;a++) //ponowna iteracja po polach w boxie by znaleœæ pole z takimi samymi kandydatami
						{
							for (int b = x;b < x + 3;b++)
							{							
								if (this->cand[i][j] == this->cand[a][b] && i != a && a != b) //szukane pole ma takich samych kandydatów i nie jest polem pierwotnym
								{
									int e1 = this->cand[i][j][0]; //zapisanie tych kandydatów
									int e2 = this->cand[i][j][1];
									for (int c = y;c < y + 3;c++) //iteracja po polach w boxie
									{
										for (int d = x;d < x+3;d++)
										{
											for (int e = 0;e < this->cand[c][d].size();e++) //usuwanie zapisanych kandydatów z pól (oprócz tych pól które zawieraj¹ dok³adnie tych kandydatów)
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

void Sudoku::openTriples()
{
	for (int i = 0;i < 9;i++)
	{
		for (int j = 0;j < 9;j++)
		{
			if (this->cand[i][j].size() == 3) //pola o wiekoœci 3
			{
				std::vector<int> line;
				for (int a = 0;a < 9;a++)
				{
					if (this->cand[i][a] == this->cand[i][j]) //je¿eli pole jest takie samo
					{
						line.push_back(a);
					}
					if (this->cand[i][a].size() == 2) // je¿eli pole jes wielkoœci 2
					{
						int exist = 0;
						for (int b = 0;b < this->cand[i][a].size();b++)
							if (this->contains(this->cand[i][j], this->cand[i][a][b]))
								exist++;
						if (exist == 2) // to pole zawiera 2 takich samych kandydatów jak pole pierwotne
							line.push_back(a);
					}
				}
				if (line.size() == 3) // je¿eli istniej¹ 3 takie pola
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

	for (int y = 0;y <= 6;y += 3) // x,y - pêtle odpowiedzialne za przemieszczanie siê pobiêdzy 9 boxami 
	{
		for (int x = 0;x <= 6;x += 3)
		{
			for (int i = y;i < y + 3;i++) // iterazja poszczególnych pól w boxie
			{
				for (int j = x;j < x + 3;j++)
				{
					if (this->cand[i][j].size() == 3)
					{
						std::vector<std::vector<int>>box;
						for (int a = y;a < y + 3;a++)
						{
							for(int b = x;b < x + 3;b++)
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


void Sudoku::hiddenPairs()
{
	for (int i = 0;i < 9;i++)
	{
		for (int a = 1;a <= 9;a++) //iteracja po wszystkich mo¿liwych kandydatach
		{
			int exist1 = 0;
			for (int j = 0;j < 9;j++)
				if (this->contains(this->cand[i][j], a))
					exist1++;

			if (exist1 == 2) //je¿eli kandydat wystêpuje tylko 2 razy w wierszu
			{
				for (int b = 1;b <= 9;b++) // iteracja po kandydatach ró¿nych od pierwszego (a)
				{
					int exist2 = 0;
					if(b != a)
						for (int c = 0;c < 9;c++)
							if (this->contains(this->cand[i][c], b))
								exist2++;

					if (exist2 == 2) //je¿eli kandydat wystêpuje tylko 2 razy w wierszu
					{
						//sprawdzenie czy ci kandydaci wystêpuj¹ w 2 tych samych polach
						std::vector<int> sameBoxes; //koordynata bloku w którym wystêpuj¹ kandydaci
						for (int c = 0;c < 9;c++)
							if (this->contains(this->cand[i][c], a) && this->contains(this->cand[i][c], b))
								sameBoxes.push_back(c);

						if (sameBoxes.size() == 2) //jêzeli wystêpuj¹ w tych samych polach - usuniêcie pozosta³ych kandydatów z tych 2 pól
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
							std::vector<int> sameBoxesY,sameBoxesX;
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

void Sudoku::xWing()
{
	//wiersze
	for (int i = 0;i < 9;i++)
	{
		for (int j = 0;j < 9;j++)
		{
			if (this->cand[i][j][0] != 10) // pole nie jest uzupe³nione
			{
				for (int k = 0;k < this->cand[i][j].size();k++) //iteracja po kandydatach w polu
				{
					std::vector<int> columnCoord;
					for (int a = 0;a < 9;a++) //ponowna iteracja po wszystkich polach w wierszu aby sparawdziæ ile jest tych kandydatów
						if (this->cand[i][a][0] != 10 && this->contains(this->cand[i][a], this->cand[i][j][k]))
							columnCoord.push_back(a);
					if (columnCoord.size() == 2) //je¿eli jest tylko 2 kandydatów
					{
						for (int b = 0;b < 9;b++)
						{
							//sprawdzenie czy w innym wierszu wystêpuje tylko 2 razy ten sam kandydat
							int n = 0;
							for (int c = 0;c < 9;c++)
								if (b != i && this->contains(this->cand[b][c], this->cand[i][j][k]))
									n++;
							//jeœli wystêpuje tylko 2 razy oraz na w tych samych kolumnach
							if (n == 2 && this->contains(this->cand[b][columnCoord[0]], this->cand[i][j][k]) && this->contains(this->cand[b][columnCoord[1]], this->cand[i][j][k]))
							{
								//usuniêcie tych kandydatów z pozosta³ych 2 kolumn
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
			if (this->cand[i][j][0] != 10) // pole nie jest uzupe³nione
			{
				for (int k = 0;k < this->cand[i][j].size();k++) //iteracja po kandydatach w polu
				{
					std::vector<int> columnCoord;
					for (int a = 0;a < 9;a++) //ponowna iteracja po wszystkich polach w wierszu aby sparawdziæ ile jest tych kandydatów
						if (this->cand[a][j][0] != 10 && this->contains(this->cand[a][j], this->cand[i][j][k]))
							columnCoord.push_back(a);
					if (columnCoord.size() == 2) //je¿eli jest tylko 2 kandydatów
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

void Sudoku::solve()
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

void Sudoku::update()
{
	this->running();
	this->poolEvent();
	this->updateMousePosition();
	this->updateButtons();
	this->updateBoxes();
	this->menu();
}

void Sudoku::render()
{
	this->window->clear();
	this->drawBoard();
	this->drawBoxes();
	this->window->display();
}