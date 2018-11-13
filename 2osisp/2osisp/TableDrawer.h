#pragma once
#include "TextGenerator.h"

#define MAX_ROWS 7
#define MAX_COLUMNS 7

class TableDrawer
{
	const int _horizontalTab = 0;
	int maxRows = MAX_ROWS;
	int maxColumns = MAX_COLUMNS;
	RECT clientSize;
	TStringsMatrix phrases;
	int fontHeight = 0, verticalTab = 0, totalHeight = 0;
	int rowCount = 0, columnCount = 0;
	int horizontalBorders[MAX_ROWS][2];
	int verticalBorders[MAX_COLUMNS][2];


	void DrawBorders(HDC hDc);
	void DrawTable(HDC hDc, bool isDraw);
	void PrintPhrases(HDC hDc);
	int RecalculateVerticalBorders();
	void CalculateHorizontalBorders();
	int CalculateVerticalBorders(HDC hDc);

public:
	TableDrawer(int rowCount, int columnCount, TStringsMatrix phrases);

	void RefreshTable(HDC hDc);
	void SetClientSize(RECT clientSize);
};

