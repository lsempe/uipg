// game.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "main.h"

#include <Windowsx.h>

#include "example_game.h"

typedef example_game Game;

std::unique_ptr<Game> g_game;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
 	
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAME));
	
	// Main message loop:
	MSG msg = {0};
	while( WM_QUIT != msg.message )
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_game->Update();
		}
	}

	g_game->Shutdown();

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= nullptr; //LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAME));
	wcex.hCursor		= LoadCursor(hInstance, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= nullptr; //MAKEINTRESOURCE(IDC_GAME);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= nullptr;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   RECT rc = { 0, 0, 1280, 720};

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   g_game = std::unique_ptr<Game>(new Game(hWnd, rc.right - rc.left, rc.bottom - rc.top));
   g_game->Initialize();
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   ShowCursor(true);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//

#define TID_POLLMOUSE 100
#define MOUSE_POLL_DELAY 500
#define WM_MOUSEENTER (WM_USER+1)

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:			
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_MOUSEENTER:
		OutputDebugString(L"Entered!");
		break;
		case WM_SETCURSOR:
		{
			WORD ht = LOWORD(lParam);
			static bool hiddencursor = false;
			if (HTCLIENT==ht && !hiddencursor)
			{
				hiddencursor = true;
				ShowCursor(false);
			}
			else if (HTCLIENT!=ht && hiddencursor) 
			{
				hiddencursor = false;
				ShowCursor(true);
			}
		}
		break;
	case WM_MOUSEMOVE:
		RECT rc;
		POINT pt;
		GetWindowRect(hWnd, &rc);
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);
		if(PtInRect(&rc,pt))
		{
			SetTimer(hWnd,TID_POLLMOUSE,MOUSE_POLL_DELAY,NULL);
			if(hWnd != GetCapture())
			{
				SetCapture(hWnd);
				PostMessage(hWnd,WM_MOUSEENTER,0,0L);
			}
			break;
		}
		else
		{
			ReleaseCapture();
			KillTimer(hWnd,TID_POLLMOUSE);
			PostMessage(hWnd,WM_MOUSELEAVE,0,0L);
		}
		break;
		case WM_TIMER:
			GetWindowRect(hWnd,&rc);
			GetCursorPos(&pt);
			if(PtInRect(&rc,pt))
			{
			  PostMessage(hWnd,WM_MOUSEHOVER,0,0L);
			  break;
			}
			ReleaseCapture();
			KillTimer(hWnd,TID_POLLMOUSE);
			PostMessage(hWnd,WM_MOUSELEAVE,0,0L);
			break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
