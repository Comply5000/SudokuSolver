#pragma once
#include<SFML\Graphics.hpp>
#include<string>
#include<iostream>
#include<vector>
#include<array>
#include<algorithm>

#include "Load.hpp"
#include "Save.hpp"
#include "Solver.hpp"

class Sudoku
{
private:
	//Variables
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	//Objects
	sf::RectangleShape board;
	sf::RectangleShape resetButton;
	sf::RectangleShape stepButton;
	sf::RectangleShape solveButton;
	sf::RectangleShape saveButton;
	sf::RectangleShape loadButton;
	sf::RectangleShape box[9][9];
	sf::RectangleShape checkBox;

	//Textures
	sf::Texture boardTexture;
	sf::Texture boxTextures[10];
	sf::Color* greyColor;
	sf::Texture resetTexture[2];
	sf::Texture stepTexture[2];
	sf::Texture solveTexture[2];
	sf::Texture saveTexture[2];
	sf::Texture loadTexture[2];
	sf::Texture candTexture[10];
	sf::Texture checkedCandTexture[10];
	sf::Texture checkBoxTexture[2];

	//Text
	sf::Font font;
	sf::Text text;

	//Mouse position
	sf::Vector2f mousePosition;

	//Logic
	std::array<std::array<int, 9>, 9> number;
	std::vector<std::vector<std::vector<int>>>cand;
	int checkedX = -1, checkedY = -1;
	bool showCand = false;

	//Display Candidates
	std::vector<std::vector<std::vector<sf::RectangleShape>>>candBox;

	//Checked CandBox
	bool checkedCand[9][9][10];

	//Private functions
	void initWindow();
	void initBoard();
	void drawBoard();
	void initBoxes();
	void drawBoxes();
	void initCand();

public:
	Sudoku();
	~Sudoku();

	const bool running() const;
	void poolEvent();
	void updateMousePosition();

	void updateButtons();
	void updateBoxes();
	void menu();
	void updateNumbers();

	void updateCand();
	void updateCandColor();
	void drawCand();

	void update();
	void render();
};

