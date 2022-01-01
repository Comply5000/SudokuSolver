#include "Save.hpp"

Save::Save(std::array<std::array<int, 9>, 9> tab)
{
	this->number = tab;
}

Save::~Save()
{
}

bool Save::saveFileDialog(wchar_t szFileName[])
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
    return GetSaveFileName(&ofn);
}

void Save::saveFile()
{
    wchar_t savedFileName[MAX_PATH];
    if (saveFileDialog(savedFileName))
    {
        std::fstream file;
        file.open(savedFileName, std::ios::out);

        for (int i = 0;i < 9;i++)
        {
            for (int j = 0;j < 9;j++)
            {
                file << number[i][j];
            }
            file << std::endl;
        }
        file.close();
    }
}
