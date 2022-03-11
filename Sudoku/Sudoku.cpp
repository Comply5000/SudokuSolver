#include "Sudoku.hpp"

//Private functions
void Sudoku::initWindow() // inicjowanie okna
{
	this->videoMode.height = 850;
	this->videoMode.width = 1000;
	this->window = new sf::RenderWindow(this->videoMode, "Sudoku solver", sf::Style::Close | sf::Style::Titlebar);
	this->window->setFramerateLimit(60);
}

void Sudoku::initBoard() //inicjowanie planszy
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

	this->checkBox.setPosition(750.f, 625.f);
	this->checkBox.setSize(sf::Vector2f(50.f, 50.f));
	this->checkBoxTexture[0].loadFromFile("Textures/check2.png");
	this->checkBoxTexture[1].loadFromFile("Textures/check.png");
	this->checkBox.setTexture(&this->checkBoxTexture[0]);

	this->font.loadFromFile("Fonts/arial.ttf");
	this->text.setFont(this->font);
	this->text.setString("Candidates");
	this->text.setPosition(sf::Vector2f(810.f, 626.f));
	this->text.setCharacterSize(35);
	this->text.setFillColor(sf::Color::White);
}

void Sudoku::drawBoard() //narysowanie planszy
{
	this->window->draw(this->board);
	this->window->draw(this->resetButton);
	this->window->draw(this->solveButton);
	this->window->draw(this->stepButton);
	this->window->draw(this->loadButton);
	this->window->draw(this->saveButton);
	this->window->draw(this->checkBox);
	this->window->draw(this->text);
}

void Sudoku::initBoxes() //inicjowanie pól
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

void Sudoku::initCand()
{
	for (int i = 1;i < 10;i++)
	{
		std::string t = "Textures/CandText/" + std::to_string(i) + ".png";
		this->candTexture[i].loadFromFile(t);
		t = "Textures/CheckedCand/" + std::to_string(i) + ".png";
		this->checkedCandTexture[i].loadFromFile(t);
	}
}

void Sudoku::initMethodButtons()
{
	this->listButton.setPosition(50.f, 725.f);
	this->listButton.setSize(sf::Vector2f(200.f, 100.f));
	this->listTexture.loadFromFile("Textures/MethodList/none.png");
	this->listButton.setTexture(&this->listTexture);

	this->methodTexture[0].loadFromFile("Textures/MethodList/nakedSingle.png");
	this->methodTexture[1].loadFromFile("Textures/MethodList/hiddenSingle.png");
	this->methodTexture[2].loadFromFile("Textures/MethodList/lockedCandidate.png");
	this->methodTexture[3].loadFromFile("Textures/MethodList/nakedPair.png");
	this->methodTexture[4].loadFromFile("Textures/MethodList/nakedTriple.png");
	this->methodTexture[5].loadFromFile("Textures/MethodList/hiddenPair.png");
	this->methodTexture[6].loadFromFile("Textures/MethodList/xWing.png");



	for (int i = 0;i < this->methodButton.size();i++)
	{
		this->methodButton[i].setPosition(50.f, 625.f-(i*100));
		this->methodButton[i].setSize(sf::Vector2f(200.f, 100.f));
		this->methodButton[i].setTexture(&this->methodTexture[i]);
	}

	for (int i = 0;i < 3;i++)
	{
		this->methodList[i].setFont(this->font);
		this->methodList[i].setPosition(sf::Vector2f(270.f, 720.f+i*35));
		this->text.setCharacterSize(32);
		this->text.setFillColor(sf::Color::White);
	}

	this->upButton.setPosition(920.f, 715.f);
	this->upButton.setSize(sf::Vector2f(60.f, 60.f));
	this->upTexture.loadFromFile("Textures/up.png");
	this->upButton.setTexture(&this->upTexture);

	this->downButton.setPosition(920.f, 775.f);
	this->downButton.setSize(sf::Vector2f(60.f, 60.f));
	this->downTexture.loadFromFile("Textures/down.png");
	this->downButton.setTexture(&this->downTexture);
}

void Sudoku::drawMethodButtons()
{
	this->window->draw(this->listButton);
	this->window->draw(this->upButton);
	this->window->draw(this->downButton);

	for (auto i : this->methodList)
		this->window->draw(i);

	if(this->openList)
		for (auto i : methodButton)
			this->window->draw(i);
}




//Public functions
Sudoku::Sudoku()
{
	this->initWindow();
	this->initBoard();
	this->initBoxes();
	this->initCand();
	this->initMethodButtons();
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
				//sprawdza czy zosta³ kliknêty kandydat a nie pole
				bool candClick = false;
				for (int k = 0;k < this->candBox[i][j].size();k++)
					if (this->candBox[i][j][k].getGlobalBounds().contains(this->mousePosition))
					{
						candClick = true;
						break;
					}

				//jesli nie zosta³ klikniêty kandydat lub pokazywanie kandydatów jest wy³¹czone 
				if (!candClick || !showCand)
				{
					pressed = true;
					if (this->checkedX == j && this->checkedY == i)
					{
						this->box[i][j].setFillColor(sf::Color::White);
						checkedX = -1;
						checkedY = -1;
					}
					else if (this->checkedX != -1 && this->checkedY != -1)
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

void Sudoku::menu() //wywo³ywanie klas i funkcji po naciœniêciu przycisków
{
	static bool pressed;
	if (this->stepButton.getGlobalBounds().contains(this->mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !pressed)
	{
		pressed = true;

		Solver solver(this->number);
		this->number = solver.returnSingleNumber();
		this->updateNumbers();
	}

	else if (this->solveButton.getGlobalBounds().contains(this->mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !pressed)
	{
		pressed = true;

		Solver solver(this->number);
		this->number = solver.returnAllNumbers();
		this->updateNumbers();
	}

	else if (this->resetButton.getGlobalBounds().contains(this->mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		for (int i = 0;i < 9;i++)
		{
			for (int j = 0;j < 9;j++)
			{
				this->box[i][j].setTexture(&this->boxTextures[0]);
				this->number[i][j] = 0;
				for (int k = 1;k < 10;k++)
					this->checkedCand[i][j][k] = false;
			}
		}

		for (int i = 0;i < 9;i++)
			for (int j = 0;j < 9;j++)
				for (int k = 0;k < 10;k++)
				{
					this->candMethod[i][j][k] = false;
					this->candMethodDel[i][j][k] = false;
				}
	}

	else if (this->loadButton.getGlobalBounds().contains(this->mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		for (int i = 0;i < 9;i++)
			for (int j = 0;j < 9;j++)
				for (int k = 1;k < 10;k++)
					this->checkedCand[i][j][k] = false;

		Load load;
		load.readFile();
		this->number = load.returnNumber();
		this->updateNumbers();

		//Wyczyszczenie wszystkich wartoœci w wyœwietlaniu metod
		for (int i = 0;i < 3;i++)
			this->methodNumber[i] = i;

		this->selectedMethod = -1;
		this->startPoint = 0;

		for (int i = 0;i < 9;i++)
			for (int j = 0;j < 9;j++)
				for (int k = 0;k < 10;k++)
				{
					this->candMethod[i][j][k] = false;
					this->candMethodDel[i][j][k] = false;
				}
	}

	else if (this->saveButton.getGlobalBounds().contains(this->mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		Save save(this->number);
		save.saveFile();
	}

	else if (this->checkBox.getGlobalBounds().contains(this->mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !pressed)
	{
		pressed = true;
		if (!showCand)
		{
			showCand = true;
			checkBox.setTexture(&this->checkBoxTexture[1]);
		}
		else
		{
			showCand = false;
			checkBox.setTexture(&this->checkBoxTexture[0]);
		}
	}

	if (this->ev.type == sf::Event::MouseButtonReleased)
		if (this->ev.mouseButton.button == sf::Mouse::Left)
			pressed = false;

}

void Sudoku::updateNumbers()
{
	for (int i = 0;i < 9;i++)
	{
		for (int j = 0;j < 9;j++)
		{
			this->box[i][j].setTexture(&boxTextures[this->number[i][j]]);
		}
	}
}

void Sudoku::updateCand()
{
	//aktualizowanie pozycji kandydatów
	Solver solver(this->number);
	this->cand = solver.returnCand();

	std::vector<std::vector<std::vector<sf::RectangleShape>>>candBox(9, std::vector<std::vector<sf::RectangleShape>>(9));

	for (int i = 0;i < 9;i++)
	{
		for (int j = 0;j < 9;j++)
		{
			if (this->number[i][j] == 0)
			{
				float x = 0;
				float y = 0;
				for (int k = 0;k < this->cand[i][j].size();k++)
				{
					sf::RectangleShape a;
					a.setPosition(this->box[i][j].getPosition().x + x, this->box[i][j].getPosition().y + y);
					a.setSize(sf::Vector2f(17.5f, 17.5f));
					a.setTexture(&this->candTexture[this->cand[i][j][k]]);
					candBox[i][j].push_back(a);

					if (x == 52.5)
					{
						x = 0;
						y += 17.5;
					}
					else
						x += 17.5;

				}
			}
		}
	}
	this->candBox = candBox;

	//aktualizowanie zaznaczenia kandydatów
	static bool pressed = false;
	for (int i = 0;i < 9;i++)
	{
		for (int j = 0;j < 9;j++)
		{
			for (int k = 0;k < this->candBox[i][j].size();k++)
			{
				if (this->candBox[i][j][k].getGlobalBounds().contains(this->mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !pressed)
				{
					pressed = true;
					if(this->checkedCand[i][j][this->cand[i][j][k]] == true)
						this->checkedCand[i][j][this->cand[i][j][k]] = false;
					else
						this->checkedCand[i][j][this->cand[i][j][k]] = true;
				}			
			}
		}
	}
	if (this->ev.type == sf::Event::MouseButtonReleased)
		if (this->ev.mouseButton.button == sf::Mouse::Left)
			pressed = false;
}

void Sudoku::updateCandColor()
{
	for (int i = 0;i < 9;i++)
	{
		for (int j = 0;j < 9;j++)
		{
			for (int k = 0;k < this->candBox[i][j].size();k++)
			{
				if (this->checkedCand[i][j][this->cand[i][j][k]] == true)
					this->candBox[i][j][k].setTexture(&this->checkedCandTexture[this->cand[i][j][k]]);
				else
					this->candBox[i][j][k].setTexture(&this->candTexture[this->cand[i][j][k]]);

				if (i == this->checkedY && j == this->checkedX)
					this->candBox[i][j][k].setFillColor(*this->greyColor);
				else
					this->candBox[i][j][k].setFillColor(sf::Color::White);
			}
		}
	}
}

void Sudoku::drawCand()
{
	for (int i = 0;i < 9;i++)
	{
		for (int j = 0;j < 9;j++)
		{
			for (int k = 0;k < this->candBox[i][j].size();k++)
			{
				this->window->draw(this->candBox[i][j][k]);
			}
		}
	}
}

void Sudoku::updateList() //rozwijanie i klikanie listy
{
	static bool pressed = false;
	if (!this->openList)
	{
		if (this->listButton.getGlobalBounds().contains(this->mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !pressed)
		{
			pressed = true;
			this->openList = true;
			this->listButton.setTexture(&this->listTexture);
		}
	}
	else
	{
		if (this->listButton.getGlobalBounds().contains(this->mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !pressed)
		{
			pressed = true;
			this->openList = false;
			this->methodType = -1;
			this->listButton.setTexture(&this->listTexture);


			for (int i = 0;i < 3;i++)
				this->methodNumber[i] = i;

			this->selectedMethod = -1;
			this->startPoint = 0;

			for (int i = 0;i < 9;i++)
				for (int j = 0;j < 9;j++)
					for (int k = 0;k < 10;k++)
					{
						this->candMethod[i][j][k] = false;
						this->candMethodDel[i][j][k] = false;
					}
		}

		for (int i = 0;i < this->methodButton.size();i++)
		{
			if (this->methodButton[i].getGlobalBounds().contains(this->mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !pressed)
			{
				pressed = true;
				this->openList = false;
				this->methodType = i;
				this->listButton.setTexture(&this->methodTexture[i]);


				for (int i = 0;i < 3;i++)
					this->methodNumber[i] = i;

				this->selectedMethod = -1;
				this->startPoint = 0;

				for (int i = 0;i < 9;i++)
					for (int j = 0;j < 9;j++)
						for (int k = 0;k < 10;k++)
						{
							this->candMethod[i][j][k] = false;
							this->candMethodDel[i][j][k] = false;
						}
			}
		}
	}

	if (this->ev.type == sf::Event::MouseButtonReleased)
		if (this->ev.mouseButton.button == sf::Mouse::Left)
			pressed = false;

}

void Sudoku::updateMethod() //pobieranie wartoœci metod z klasy Solver i zapis do listy tekstowej
{
	listText.clear();
	listText.shrink_to_fit();
	Solver solver(this->number);
	if (this->methodType == -1)
	{
		this->updateMethodList();
	}
	if (this->methodType == 0)
	{
		this->methodDel.clear();
		solver.nakedSingle();
		this->method = solver.returnMethod();
		for (int m = 0;m < this->method.size();m++)
		{
			for (int i = 0;i < 9;i++)
			{
				for (int j = 0;j < 9;j++)
				{
					if (this->method[m][i][j].size() > 0)
					{
						std::string text = std::to_string(m + 1) + ". The cell R" + std::to_string(i+1) + "C" + std::to_string(j+1) + " can contain only the value " + std::to_string(this->method[m][i][j][0]);
						this->listText.push_back(text);
					}
				}
			}
		}
		this->updateMethodList();
	}
	else if (this->methodType == 1)
	{
		this->methodDel.clear();
		solver.hiddenSingle();
		this->method = solver.returnMethod();
		for (int m = 0;m < this->method.size();m++)
		{
			for (int i = 0;i < 9;i++)
			{
				for (int j = 0;j < 9;j++)
				{
					if (this->method[m][i][j].size() > 0)
					{
						std::string text = std::to_string(m + 1) + ". The cell R" + std::to_string(i + 1) + "C" + std::to_string(j + 1) + " can contain only the value " + std::to_string(this->method[m][i][j][0]);
						this->listText.push_back(text);
					}
				}
			}
		}
		this->updateMethodList();
	}
	else if (this->methodType == 2)
	{
		solver.lockedCandidate();
		this->method = solver.returnMethod();
		this->methodDel = solver.returnMethodDel();
		this->structureType = solver.returnStructureType();
		/*for (int m = 0;m < this->method.size();m++)
		{
			std::string text = std::to_string(m + 1);
			this->listText.push_back(text);
		}*/
		for (int m = 0;m < this->method.size();m++)
		{
			std::string type;
			if (this->structureType[m] == 0)
				type = "row";
			else if (this->structureType[m] == 1)
				type = "column";
			else if (this->structureType[m] == 2)
				type = "square";			
			std::string text = std::to_string(m + 1) + ". Locket candidate in "+ type;
			this->listText.push_back(text);
		}

		this->updateMethodList();
	}
	else if (this->methodType == 3)
	{
		solver.nakedPair();
		this->method = solver.returnMethod();
		this->methodDel = solver.returnMethodDel();
		this->structureType = solver.returnStructureType();

		for (int m = 0;m < this->method.size();m++)
		{
			std::string type;
			if (this->structureType[m] == 0)
				type = "row";
			else if (this->structureType[m] == 1)
				type = "column";
			else if (this->structureType[m] == 2)
				type = "square";
			std::vector<std::string> box;
			for (int i = 0;i < 9;i++)
			{
				for (int j = 0;j < 9;j++)
				{
					if (this->method[m][i][j].size() > 0)
					{
						std::string t = "R" + std::to_string(i + 1) + "C" + std::to_string(j + 1);
						box.push_back(t);
					}					
				}
			}
			std::string text = std::to_string(m + 1) + ". "+box[0]+" and "+box[1]+ " create pair in "+type;
			this->listText.push_back(text);
		}
		this->updateMethodList();
	}
	else if (this->methodType == 4)
	{
		solver.nakedTriple();
		this->method = solver.returnMethod();
		this->methodDel = solver.returnMethodDel();
		this->structureType = solver.returnStructureType();
		//std::cout << method.size();

		for (int m = 0;m < this->method.size();m++)
		{
			std::string type;
			if (this->structureType[m] == 0)
				type = "row";
			else if (this->structureType[m] == 1)
				type = "column";
			else if (this->structureType[m] == 2)
				type = "square";
			std::vector<std::string> box;
			for (int i = 0;i < 9;i++)
			{
				for (int j = 0;j < 9;j++)
				{
					if (this->method[m][i][j].size() > 0)
					{
						std::string t = "R" + std::to_string(i + 1) + "C" + std::to_string(j + 1);
						box.push_back(t);
					}
				}
			}
			std::string text = std::to_string(m + 1) + ". " + box[0] + ", " + box[1] + " and " + box[2] + " create triple in " + type;
			this->listText.push_back(text);
		}
		this->updateMethodList();
	}
	else if (this->methodType == 6)
	{
		solver.xWing();
		this->method = solver.returnMethod();
		this->methodDel = solver.returnMethodDel();
		this->structureType = solver.returnStructureType();

		for (int m = 0; m < this->method.size(); m++)
		{
			int number;
			std::string type;
			if (this->structureType[m] == 0)
				type = "Row";
			else if (this->structureType[m] == 1)
				type = "Column";
			std::vector<std::string> box;
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					if (this->method[m][i][j].size() > 0)
					{
						number = method[m][i][j][0];
						std::string t;
						if (structureType[m] == 0)
							t = std::to_string(i + 1);
						else if(structureType[m] == 1)
							t = std::to_string(j + 1);
						box.push_back(t);
						break;
					}
				}
			}
			std::string text = std::to_string(m + 1) + ". "+type+ " " + box[0] + " and " + box[1] + " create X-Wing with number " + std::to_string(number);
			this->listText.push_back(text);
		}
		this->updateMethodList();
	}
}

void Sudoku::updateMethodList() //wyœwietlenie listy tekstowej na ekranie
{
	int a = 0;
	for (int i = this->startPoint;i < startPoint + 3;i++)
	{
		if (this->listText.size() > i)
			this->methodList[a].setString(listText[i]);
		else
			this->methodList[a].setString("");
		a++;
	}
}

void Sudoku::updateStartPosition() //mechanizm przewijania listy z wynikami 
{
	static bool pressed = false;
	if (this->listText.size() > 3)
	{
		if (this->downButton.getGlobalBounds().contains(this->mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !pressed)
		{
			pressed = true;
			if (this->startPoint < listText.size() - 3)
			{
				this->startPoint++;
				for (int i = 0;i < 3;i++)
					this->methodNumber[i]++;
			}
		}
		else if (this->upButton.getGlobalBounds().contains(this->mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !pressed)
		{
			pressed = true;
			if (this->startPoint > 0)
			{
				this->startPoint--;
				for (int i = 0;i < 3;i++)
					this->methodNumber[i]--;
			}
		}
	}
	if (this->ev.type == sf::Event::MouseButtonReleased)
		if (this->ev.mouseButton.button == sf::Mouse::Left)
			pressed = false;
}

void Sudoku::methodResult() //koloruje kandydatów wed³ug metody
{
	for (int l = 0;l < 3;l++)
	{
		if (this->methodList[l].getGlobalBounds().contains(this->mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			for (int i = 0;i < 9;i++)
				for (int j = 0;j < 9;j++)
					for (int k = 0;k < 10;k++)
					{
						this->candMethod[i][j][k] = false;
						this->candMethodDel[i][j][k] = false;
					}

			this->selectedMethod = this->methodNumber[l];

			for (int i = 0;i < 9;i++)
				for (int j = 0;j < 9;j++)
					for (int k = 0;k < this->cand[i][j].size();k++)
					{
						if (this->contains(this->method[this->selectedMethod][i][j], this->cand[i][j][k]))
							this->candMethod[i][j][k] = true;
						if (this->methodType != 0 && this->methodType != 1 && this->contains(this->methodDel[this->selectedMethod][i][j], this->cand[i][j][k]))
							this->candMethodDel[i][j][k] = true;
					}
		}
	}

	for (int i = 0;i < 3;i++)
	{
		if (this->methodNumber[i] == this->selectedMethod)
			this->methodList[i].setFillColor(sf::Color::Green);
		else
			this->methodList[i].setFillColor(sf::Color::White);
	}

	for (int i = 0;i < 9;i++)
		for (int j = 0;j < 9;j++)
			for (int k = 0;k < 10;k++)
			{
				if (candMethod[i][j][k])
					this->candBox[i][j][k].setFillColor(sf::Color::Green);
				if (candMethodDel[i][j][k])
					this->candBox[i][j][k].setFillColor(sf::Color::Red);
			}
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

void Sudoku::update()
{
	this->running();
	this->poolEvent();
	this->updateMousePosition();
	this->updateButtons();
	this->updateBoxes();
	this->menu();
	this->updateCand();
	this->updateCandColor();
	this->updateList();
	this->updateMethod();
	this->updateStartPosition();
	this->methodResult();
}

void Sudoku::render()
{
	this->window->clear();
	this->drawBoard();
	this->drawBoxes();
	if(showCand)
		this->drawCand();
	this->drawMethodButtons();
	this->window->display();
}