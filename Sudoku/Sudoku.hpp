#pragma once
#include<SFML\Graphics.hpp>
#include<string>
#include<iostream>
#include<vector>
#include<algorithm>

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
	sf::RectangleShape box[9][9];

	//Textures
	sf::Texture boardTexture;
	sf::Texture boxTextures[10];
	sf::Color* greyColor;
	sf::Texture resetTexture;
	sf::Texture stepTexture;
	sf::Texture solveTexture;

	//Mouse position
	sf::Vector2f mousePosition;

	//Logic
	int number[9][9];
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

