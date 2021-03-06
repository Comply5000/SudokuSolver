#include "Load.hpp"

Load::Load()
{
}

Load::~Load()
{
}

bool Load::openFileDialog(wchar_t szFileName[])
{
    OPENFILENAME ofn;
    const wchar_t* FilterSpec = L"Text Files(.txt)\0*.txt\0";
    const wchar_t* Title = L"Open....";

    wchar_t szFileTitle[MAX_PATH] = L"";

    *szFileName = 0;

    /* fill in non-variant fields of OPENFILENAME struct. */
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = GetFocus();
    ofn.lpstrFilter = FilterSpec;
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter = 0;
    ofn.nFilterIndex = 0;
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrInitialDir = L"."; // Initial directory.
    ofn.lpstrFileTitle = szFileTitle;
    ofn.nMaxFileTitle = MAX_PATH;
    ofn.lpstrTitle = Title;
    ofn.lpstrDefExt = 0; // I've set to null for demonstration
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

    // false if failed or cancelled
    return GetOpenFileName(&ofn);
}

void Load::readFile()
{
    for (int i = 0;i < 9;i++)
        for (int j = 0;j < 9;j++)
            this->number[i][j] = 0;

    wchar_t openedFileName[MAX_PATH];
    if (openFileDialog(openedFileName))
    {
        std::fstream file;
        file.open(openedFileName, std::ios::in);

        std::string line;
        std::vector<std::string> tab;
        while (!file.eof())
        {
            std::getline(file, line);
            tab.push_back(line);
        }
        file.close();

        try
        {
            for (int i = 0;i < 9;i++)
            {
                for (int j = 0;j < tab[i].length();j++)
                    this->number[i][j] = tab[i][j] - 48;
            }
        } 
        catch(std::exception& e)
        {
        }
    }    
}

std::array<std::array<int, 9>, 9> Load::returnNumber()
{
    return this->number;
}
