#pragma once

#include <windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <iostream>

class Load
{
private:
	std::array<std::array<int, 9>, 9> number;


public:
	Load();
	~Load();

	bool openFileDialog(wchar_t szFileName[]);
	void readFile();
	std::array<std::array<int, 9>, 9> returnNumber();
};

