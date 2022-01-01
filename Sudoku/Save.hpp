#pragma once

#include <windows.h>
#include <fstream>
#include <array>
#include <iostream>

class Save
{
private:
	std::array<std::array<int, 9>, 9> number;

public:
	Save(std::array<std::array<int,9>,9> tab);
	~Save();

	bool saveFileDialog(wchar_t szFileName[]);
	void saveFile();
};

