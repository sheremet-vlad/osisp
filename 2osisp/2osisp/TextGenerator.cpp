#include "stdafx.h"
#include "TextGenerator.h"
#include "ctime"
#include "TableDrawer.h"


TextGenerator::TextGenerator()
{
	TStringList textRow;

	srand(time(NULL));

	for (int i = 0; i < MAX_ROWS; i++)
	{
		for (int j = 0; j < MAX_COLUMNS; j++)
		{
			textRow.push_back(RandomString(10).c_str());
		}
		text.push_back(textRow);
		textRow.clear();
	}
}

std::wstring TextGenerator::RandomString(int len)
{
	std::wstring strRand = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::wstring newstr;
	int pos;
	while (newstr.size() <= len) {
		pos = ((rand() % (strRand.size() - 1)));
		newstr += strRand.substr(pos, 1);
		newstr += L" ";
	}
	return newstr;
}