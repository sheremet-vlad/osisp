#include "stdafx.h"
#include "MyDll.h"

int Min(const int a, const int b) {
	if (a > b) 
		return b;
	return a;
}

int Max(const int a, const int b) {
	if (a < b) 
		return b;
	return a;
}