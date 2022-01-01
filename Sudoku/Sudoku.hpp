#pragma once
#include<SFML\Graphics.hpp>
#include<string>
#include<iostream>
#include<vector>
#include<array>
#include<algorithm>

#include "Load.hpp"
#include "Save.hpp"

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

	//Textures
	sf::Texture boardTexture;
	sf::Texture boxTextures[10];
	sf::Color* greyColor;
	sf::Texture resetTexture[2];
	sf::Texture stepTexture[2];
	sf::Texture solveTexture[2];
	sf::Texture saveTexture[2];
	sf::Texture loadTexture[2];

	//Mouse position
	sf::Vector2f mousePosition;

	//Logic
	std::array<std::array<int, 9>, 9> number;
	std::vector<std::vector<std::vector<int>>>cand;
	int checkedX = -1, checkedY = -1;
	bool solved = false;
	bool error = false;
	int repeat = 0;
	std::vector<int> usedBoxY, usedBoxX;

	//Private functions
	void initWindow();
	void initBoard();
	void drawBoard();
	void initBoxes();
	void drawBoxes();

public:
	Sudoku();
	~Sudoku();

	const bool running() const;
	void poolEvent();
	void updateMousePosition();

	void updateButtons();
	void updateBoxes();
	void menu();

	//Solving algorithms
	bool existInSquare(int y,int x,int k);
	bool contains(std::vector<int> vec, const int& elem);
	void findCandidate();
	void singleCandidate();
	void hiddenLoner();
	void lockedCandidate();
	void openPairs();
	void openTriples();
	void hiddenPairs();
	void xWing();

	void solve();

	void update();
	void render();
};

