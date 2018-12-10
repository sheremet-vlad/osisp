#include "stdafx.h"
#include "TableDrawer.h"
#include <codecvt>
#include <string>
#include <algorithm>
#include <locale>


TableDrawer::TableDrawer(int newRowCount, int newColumnCount, TStringsMatrix newPhrases)
{
	if (newRowCount > maxRows)
	{
		rowCount = maxRows;
	}
	else
	{
		rowCount = newRowCount;
	}

	if (newColumnCount > maxColumns)
	{
		columnCount = maxColumns;
	}
	else
	{
		columnCount = newColumnCount;
	}
	phrases = newPhrases;
}

void TableDrawer::SetClientSize(RECT newClientSize)
{
	clientSize.bottom = newClientSize.bottom;
	clientSize.top = newClientSize.top;
	clientSize.left = newClientSize.left;
	clientSize.right = newClientSize.right;
}

void TableDrawer::RefreshTable(HDC hDc)
{
	fontHeight = 15;
	verticalTab = 0;
	CalculateHorizontalBorders();
	DrawTable(hDc, false);

	while (clientSize.bottom - totalHeight > 5)
	{
		fontHeight++;
		CalculateHorizontalBorders();
		DrawTable(hDc, false);
	}

	while (totalHeight > clientSize.bottom)
	{
		fontHeight--;
		CalculateHorizontalBorders();
		DrawTable(hDc, false);
	}

	while (clientSize.bottom - totalHeight > 5)
	{
		totalHeight = RecalculateVerticalBorders();
		verticalTab++;
	}

	DrawTable(hDc, true);
}

void TableDrawer::DrawBorders(HDC hDc)
{
	for (int i = 0; i < rowCount - 1; i++)
	{
		MoveToEx(hDc, clientSize.left, verticalBorders[i][1], nullptr);
		LineTo(hDc, clientSize.right, verticalBorders[i][1]);
	}
	for (int i = 0; i < columnCount - 1; i++)
	{
		MoveToEx(hDc, horizontalBorders[i][1] - _horizontalTab, 0, nullptr);
		LineTo(hDc, horizontalBorders[i][1] - _horizontalTab, clientSize.bottom);
	}
}

void TableDrawer::PrintPhrases(HDC hDc)
{
	RECT rect;

	for (int i = 0; i < rowCount; i++)
	{
		for (int j = 0; j < columnCount; j++)
		{
			rect.top = verticalBorders[i][0] + verticalTab / 2;
			rect.bottom = verticalBorders[i][1];
			rect.left = horizontalBorders[j][0];
			rect.right = horizontalBorders[j][1];

			DrawText(hDc, phrases[i][j].c_str(), -1, &rect, DT_WORDBREAK | DT_WORD_ELLIPSIS);
		}
	}
}

int TableDrawer::CalculateVerticalBorders(HDC hDc)
{
	RECT rect;
	rect.top = 0;

	int totalHeight = 0;
	verticalBorders[0][0] = 0;
	for (int i = 0; i < rowCount; i++)
	{
		if (i > 0)
		{
			verticalBorders[i][0] = verticalBorders[i - 1][1];
		}
		int rowMaxHeight = 0;
		for (int j = 0; j < columnCount; j++)
		{
			rect.bottom = 0;
			rect.left = horizontalBorders[j][0];
			rect.right = horizontalBorders[j][1];
			if (i == 0 && j == 0) {
				//wstring_convert <codecvt_utf8_utf16<wchar_t» converter;
			
				DrawText(hDc, L"SSSSS", -1, &rect, DT_CALCRECT | DT_WORDBREAK | DT_WORD_ELLIPSIS);
			}
			else {
				DrawText(hDc, phrases[i][j].c_str(), -1, &rect, DT_CALCRECT | DT_WORDBREAK | DT_WORD_ELLIPSIS);
			}
			rowMaxHeight = max(rowMaxHeight, rect.bottom);
		}
		totalHeight += rowMaxHeight;
		if (i > 0)
		{
			verticalBorders[i][1] = verticalBorders[i - 1][1] + rowMaxHeight;
		}
		else
		{
			verticalBorders[0][1] = rowMaxHeight;
		}
	}
	return totalHeight;
}

int TableDrawer::RecalculateVerticalBorders()
{
	auto counter = 1;
	auto totalHeight = 0;
	for (int i = 0; i < rowCount; i++)
	{
		if (i > 0)
		{
			verticalBorders[i][0] += counter - 1;
		}
		verticalBorders[i][1] += counter;

		counter++;
	}
	totalHeight += verticalBorders[rowCount - 1][1];

	return totalHeight;
}

void TableDrawer::CalculateHorizontalBorders()
{
	const auto step = (clientSize.right - clientSize.left) / columnCount;
	for (int i = 0; i < columnCount; i++)
	{
		horizontalBorders[i][0] = i * step;
		horizontalBorders[i][1] = (i + 1) * step;
	}
}

void TableDrawer::DrawTable(HDC hDc, bool isDraw)
{
	auto font = CreateFont(fontHeight, 0, 0, 0, FW_NORMAL, true, false, false, 0, 0, 0, 0, FF_ROMAN, nullptr);
	SelectObject(hDc, font);
	if (isDraw)
	{
		PrintPhrases(hDc);
		DrawBorders(hDc);
	}
	else
	{
		totalHeight = CalculateVerticalBorders(hDc);
	}
	DeleteObject(font);
}