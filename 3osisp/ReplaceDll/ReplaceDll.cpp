#include "stdafx.h"
#include "ReplaceDll.h"

using namespace std;

void Replace(const LPCSTR target, const LPCSTR newStr) {

	string targetstr(target);
	string newstr(newStr);

	const auto hProcess = GetCurrentProcess();
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	const auto minAddress = reinterpret_cast<DWORD_PTR>(systemInfo.lpMinimumApplicationAddress);
	const auto maxAddress = reinterpret_cast<DWORD_PTR>(systemInfo.lpMaximumApplicationAddress);
	CHAR haystack[4096]; 
	MEMORY_BASIC_INFORMATION mbi;

	for (auto addr = minAddress; addr < maxAddress; addr += systemInfo.dwPageSize)
	{
		VirtualQueryEx(hProcess, reinterpret_cast<LPCVOID>(addr), &mbi, sizeof(mbi));

		if (mbi.State == MEM_COMMIT && mbi.Type == MEM_IMAGE && mbi.Protect == PAGE_READWRITE)
		{
			ZeroMemory(haystack, sizeof(haystack));
			ReadProcessMemory(hProcess, reinterpret_cast<LPVOID>(addr), static_cast<LPVOID>(haystack), sizeof(haystack), nullptr);
			
			
			auto match = search(begin(haystack), end(haystack), targetstr.begin(), targetstr.end());
			const auto isMatch = BOOL(match < end(haystack));
			while (match < end(haystack))
			{

				if (newstr.length() > targetstr.length())
					copy_n(newstr.begin(), targetstr.length(), match);
				else { 
					copy(newstr.begin(), newstr.end(), match);
				}
				match = search(match + targetstr.length(), end(haystack), targetstr.begin(), targetstr.end());
			}

			if (isMatch) {
				cout << "	Before: " << haystack << endl;
				WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(addr), static_cast<LPCVOID>(haystack), sizeof(haystack), nullptr);
				cout << "	After: " << haystack << endl;
			}	
			 
		}
	}
}
