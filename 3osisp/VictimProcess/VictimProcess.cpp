#include "pch.h"
#include <iostream>
#include <conio.h>

using namespace std;

const int n = 10;
const string strings[n] = {
	"string 1",
	"string 2",
	"string 3",
	"string 4",
	"string 5",
	"string 6",
	"string 7",
	"string 8",
	"string 9",
	"cjjzzv 10",
};

int main()
{
    cout << "press any key to check current variables"; 
	while (true) {
		_getch();
		cout << endl;

		for (auto i = 0; i < n; ++i)
			cout << strings[i].c_str() << endl;
	}
}