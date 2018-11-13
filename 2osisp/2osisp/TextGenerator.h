#pragma once
#include <vector>
#include <string>

typedef std::vector<std::vector<std::wstring>> TStringsMatrix;
typedef std::vector<std::wstring> TStringList;

class TextGenerator
{
private:
	std::wstring RandomString(int len);

public:
	TStringsMatrix text;
	TextGenerator();
};

