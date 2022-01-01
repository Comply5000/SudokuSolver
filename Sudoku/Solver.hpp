#pragma once

#include<string>
#include<iostream>
#include<vector>
#include<array>
#include<algorithm>

class Solver
{
private:
	std::array<std::array<int, 9>, 9> number;
	std::vector<std::vector<std::vector<int>>>cand;
	bool solved = false;
	bool error = false;

public:
	Solver(std::array<std::array<int, 9>, 9> tab);
	~Solver();

	//Solving algorithms
	bool existInSquare(int y, int x, int k);
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
	std::array<std::array<int, 9>, 9> returnNumber();
	std::vector<std::vector<std::vector<int>>> returnCand();
	bool returnError();
};

