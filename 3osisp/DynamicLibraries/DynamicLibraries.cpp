#include "pch.h"
#include <iostream>
#include <conio.h>

#pragma comment(lib, "..\\Debug\\MyDll.lib")
#include "..//MyDll/MyDll.h"

#pragma comment(lib, "..\\Debug\\ReplaceDll.lib")
#include "..//ReplaceDll/ReplaceDll.h"

using namespace std;

typedef int Func(int, int);
DWORD GetProcessIdByName(const wstring &processName);
const wstring victimProcessName(L"Osisp_2.exe");

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
	"string 10",
};

void printArray(const string *array, const int size) {
	for (auto i = 0; i < size; i++) {
		cout << array[i].c_str() << "\n";
	}
}

int main()
{
	cout << "Static includes: "  << "min = " << Min(5, 4) << "; max = " << Max(5, 4) << endl;
	
	HMODULE lib;
	if ((lib = LoadLibraryA("MyDll"))) {
		const auto min_d = reinterpret_cast<Func*>(GetProcAddress(lib, MIN));
		const auto max_d = reinterpret_cast<Func*>(GetProcAddress(lib, MAX));
		cout << "Dynamic includes: " << "min = " << min_d(5, 4) << "; max = " << max_d(5, 4) << endl;
		FreeLibrary(lib);
	}

	cout << endl << "Before:" << endl;
	printArray(strings, n);
	Replace(strings[3].c_str(), "Hello!!!");
	cout << endl << "After:" << endl;
	printArray(strings, n);
	cout << endl;

	cout << "Press any key to replace a string in other process" << endl;
	_getch();
	const auto hRemoteProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetProcessIdByName(victimProcessName));
	const auto threadFunction = static_cast<LPVOID>(GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA"));
	string parameter("InjectionDll.dll");
	const auto argAddress = VirtualAllocEx(hRemoteProcess, nullptr, parameter.length() + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	WriteProcessMemory(hRemoteProcess, argAddress, parameter.c_str(), parameter.length() + 1, nullptr);
	CreateRemoteThread(hRemoteProcess, nullptr, 0, static_cast<LPTHREAD_START_ROUTINE>(threadFunction), argAddress, NULL, nullptr);
	CloseHandle(hRemoteProcess);
	return 0;
}

DWORD GetProcessIdByName(const wstring &processName)
{
	DWORD processId = 0;
	auto const hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 processEntry;
	ZeroMemory(&processEntry, sizeof(processEntry));
	processEntry.dwSize = sizeof(processEntry);

	auto isFound = false;
	while (Process32Next(hSnapshot, &processEntry) && !isFound)
	{
		if (!processName.compare(processEntry.szExeFile))
		{
			processId = processEntry.th32ProcessID;
			isFound = true;
		}
	}
	
	return processId;
}