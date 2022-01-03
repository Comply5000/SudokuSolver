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

	this->qwer.setPosition(750.f, 600.f);
	this->qwer.setSize(sf::Vector2f(200.f, 100.f));
	qwer.setFillColor(sf::Color::Red);
}

void Sudoku::drawBoard()
{
	this->window->draw(this->board);
	this->window->draw(this->resetButton);
	this->window->draw(this->solveButton);
	this->window->draw(this->stepButton);
	this->window->draw(this->loadButton);
	this->window->draw(this->saveButton);
	this->window->draw(this->qwer);
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

void Sudoku::initCand()
{
	for (int i = 1;i < 10;i++)
	{
		std::string t = "Textures/CandText/" + std::to_string(i) + ".png";
		this->candTexture[i].loadFromFile(t);
	}
}



//Public functions
Sudoku::Sudoku()
{
	this->initWindow();
	this->initBoard();
	this->initBoxes();
	this->initCand();
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

		Solver solver(this->number);
		solver.solve();
		this->number = solver.returnNumber();
		this->cand = solver.returnCand();
		this->updateNumbers();
	}

	else if (this->solveButton.getGlobalBounds().contains(this->mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !pressed)
	{
		pressed = true;

		Solver solver(this->number);
		solver.solve();
		this->number = solver.returnNumber();
		this->updateNumbers();

		while (true)
		{
			solver.solve();
			int checked = 0;
			for (int i = 0;i < 9;i++)
				for (int j = 0;j < 9;j++)
					if (number[i][j] != 0)
							checked++;
			if (checked == 81 || solver.returnError())
				break;
		}
		this->number = solver.returnNumber();
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
			}
		}
	}

	else if (this->loadButton.getGlobalBounds().contains(this->mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		Load load;
		load.readFile();
		this->number = load.returnNumber();
		this->updateNumbers();
	}

	else if (this->saveButton.getGlobalBounds().contains(this->mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		Save save(this->number);
		save.saveFile();
	}

	else if (this->qwer.getGlobalBounds().contains(this->mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !pressed)
	{
		pressed = true;
		if (!showCand)
			showCand = true;
		else
			showCand = false;
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
}

void Sudoku::updateCandColor()
{
	for (int i = 0;i < 9;i++)
	{
		for (int j = 0;j < 9;j++)
		{
			if (i == this->checkedY && j == this->checkedX)
			{
				for (int k = 0;k < this->candBox[i][j].size();k++)
					this->candBox[i][j][k].setFillColor(*this->greyColor);
			}
			else
				for (int k = 0;k < this->candBox[i][j].size();k++)
					this->candBox[i][j][k].setFillColor(sf::Color::White);
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
}

void Sudoku::render()
{
	this->window->clear();
	this->drawBoard();
	this->drawBoxes();
	if(showCand)
		this->drawCand();
	this->window->display();
}