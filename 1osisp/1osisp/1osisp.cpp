#include "stdafx.h"
#include "Windows.h"
#include <objidl.h>
#include "1osisp.h"
#include <gdiplus.h>
#include "windowsx.h"

using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

#define MAX_LOADSTRING 100

/*	GLOBAL VARIABLES  */
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // the title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

PAINTSTRUCT ps;
int Width, Height, idTimer = -1;
int sx = 10;
int sy = 10;
HDC hdc;

// Forward declarations of functions included in this code module:
VOID				SettingPointWindow(HWND hwnd);
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

POINT currentPosition = { 20,20 };
POINT windowStart;
POINT windowSize;
POINT mousePosition{ 0,0 };
POINT previousMousePosition{ 0,0 };
bool leftButtonMousePressed = false;
bool isLimitReached = true;
bool isRebound = false;

void SettingPointWindow(HWND hwnd)
{
	RECT rc;

	GetWindowRect(hwnd, &rc);
	windowStart.x = rc.left;
	windowStart.y = rc.top;
	windowSize.x = rc.right - windowStart.x;
	windowSize.y = rc.bottom - windowStart.y;
}

bool IsMouseInBitmap(POINT ptMouse, int Width, int Height)
{
	return ((ptMouse.x > currentPosition.x) &&
		(ptMouse.x < currentPosition.x + Width) &&
		(ptMouse.y > currentPosition.y) &&
		(ptMouse.y < currentPosition.y + Height));
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	ULONG_PTR token;
	GdiplusStartupInput input = { 0 };
	input.GdiplusVersion = 1;
	GdiplusStartup(&token, &input, NULL);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MY1OSISP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1OSISP));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	GdiplusShutdown(token);
	return (int)msg.wParam;
}

/*	REGISTER THE WINDOW CLASS  */
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1OSISP));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_ACTIVECAPTION	);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY1OSISP);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

/*	PURPOSE: SAVE THE INSTANCE HANDLE
	AND CREATE AND DISPLAY MAIN WINDOW  */
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

/*	DISPLAY IMAGE  */
void DrawSprite(HDC hdc, HWND hWnd)
{
	Gdiplus::Graphics *graphics = new Gdiplus::Graphics(ps.hdc);
	Image *img = new Image(L"java.png", false);
	graphics->DrawImage(img, currentPosition.x, currentPosition.y);
	Height = img->GetHeight() + 50;
	Width = img->GetWidth() + 20;
	delete img;
	delete graphics;
}

/*	GARNITURE ACTION  */
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HBITMAP hBitmap;
	int wheelDelta;

	static HDC memBit;

	switch (message)
	{
	/*	MOVE ON KEYS  */
	case WM_KEYDOWN:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case VK_DOWN:
			if (currentPosition.y <= (windowSize.y - Height) && !isRebound)
			{
				currentPosition.y += 10;
			}
			else
			{
				isRebound = true;
				if (currentPosition.y <= 0) {
					currentPosition.y += 10;
					isRebound = false;
				}
				else
				{
					currentPosition.y -= 10;
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case VK_UP:
			if (currentPosition.y >= 0 && !isRebound)
			{
				currentPosition.y -= 10;
			}
			else
			{
				isRebound = true;
				if (currentPosition.y >= (windowSize.y - Height)) {
					currentPosition.y -= 10;
					isRebound = false;
				}
				else
				{
					currentPosition.y += 10;
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case VK_LEFT:
		{
			if (currentPosition.x >= 0 && !isRebound)
			{
				currentPosition.x -= 10;
			}
			else
			{
				isRebound = true;
				if (currentPosition.x >= (windowSize.x - Width)) {
					currentPosition.x -= 10;
					isRebound = false;
				}
				else
				{
					currentPosition.x += 10;
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;

		case VK_RIGHT:
		{
			if (currentPosition.x <= (windowSize.x - Width) && !isRebound)
			{
				currentPosition.x += 10;
			}
			else
			{
				isRebound = true;
				if (currentPosition.x <= 0) {
					currentPosition.x += 10;
					isRebound = false;
				}
				else
				{
					currentPosition.x -= 10;
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
			break;
		}

	}
	break;
	case WM_KEYUP:
	{
		/*int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case VK_DOWN:
			isRebound = false;
			break;
		case VK_LEFT:
			isRebound = false;
			break;
		case VK_RIGHT:
			isRebound = false;
			break;
		case VK_UP:
			isRebound = false;
			break;
		}*/

		isRebound = false;
		break;
	}
	/*	MOVE ON LBM  */
	case WM_MOUSEMOVE:
	{
		int wmId = LOWORD(wParam);
		if (wmId == MK_LBUTTON)
		{
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			currentPosition.x = xPos - Width / 2;
			currentPosition.y = yPos - Height / 2;
			if (currentPosition.x < 0)
			{
				currentPosition.x = 0;
			}
			if (currentPosition.y < 0)
			{
				currentPosition.y = 0;
			}
			if (currentPosition.x > (windowSize.x - Width))
			{
				currentPosition.x = windowSize.x - Width;
			}
			if (currentPosition.y > (windowSize.y - Height))
			{
				currentPosition.y = windowSize.y - Height;
			}

		}
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
	}
	break;

	case WM_LBUTTONUP:
		leftButtonMousePressed = false;
		break;

		/*	MOVE ON MOUSE WHEEL  */
	case WM_MOUSEWHEEL:
	{
		wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (LOWORD(wParam) == MK_SHIFT)
		{
			if ((currentPosition.x < (windowSize.x - Width) && (wheelDelta < 0)) || (currentPosition.x > (0 + 10) && (wheelDelta > 0)))
			{
				currentPosition.x -= wheelDelta / abs(wheelDelta) * 10;
			}
		}
		else
		{
			if ((currentPosition.y < (windowSize.y - Height) && (wheelDelta < 0)) || (currentPosition.y > (0 + 10) && (wheelDelta > 0)))
			{
				currentPosition.y -= wheelDelta / abs(wheelDelta) * 10;
			}
		}
		InvalidateRgn(hWnd, NULL, TRUE);
		break;

	}

	case WM_SIZE:
	{
		if (currentPosition.x >= (windowSize.x - Width))
		{
			currentPosition.x -= 20;
			InvalidateRect(hWnd, NULL, TRUE);
		}

		if (currentPosition.y >= (windowSize.y - Height))
		{
			currentPosition.y -= 20;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		SettingPointWindow(hWnd);
		break;
	}

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		DrawSprite(hdc, hWnd);
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_GETMINMAXINFO:
	{
		const auto lpMmi = reinterpret_cast<LPMINMAXINFO>(lParam);
		lpMmi->ptMinTrackSize.x = 500;
		lpMmi->ptMinTrackSize.y = 300;
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

		/*	FREE MOVE  */
	

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
