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
	std::array<std::array<int, 9>, 9> tab;
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

	bool possible(int y,int x,int n);
	void solve();

	std::array<std::array<int, 9>, 9> returnAllNumbers();
	std::array<std::array<int, 9>, 9> returnSingleNumber();
	std::vector<std::vector<std::vector<int>>> returnCand();
};

