// GT_HelloWorldWin32.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <iostream>
#include "ComUtility.h"
#include <vector>

using namespace std;
using namespace MyCOM;
// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("WinBtnClkrn");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("WinBtnClkrn");

HINSTANCE hInst;


// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
BOOL CALLBACK EnumChildWindowsProc(_In_  HWND hwnd, _In_  LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Win32 Guided Tour"),
            NULL);

        return 1;
    }

    hInst = hInstance; // Store instance handle in our global variable

    // The parameters to CreateWindow explained:
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    HWND hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 100,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("WinBtnClkr"),
            NULL);

        return 1;
    }

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);

	EnumWindows(EnumWindowsProc, NULL);


    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("WinBtnClkr");

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        // Here your application is laid out.
        // For this introduction, we just print out "Hello, World!"
        // in the top left corner.
        TextOut(hdc,
            5, 5,
            greeting, _tcslen(greeting));
        // End application-specific layout section.

        EndPaint(hWnd, &ps);       
	
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}

// Get Each of the windows...
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	/* Get the process ID of the Window */
	LPDWORD process_id = (LPDWORD) CoTaskMemAlloc( sizeof( LPDWORD) );
	GetWindowThreadProcessId( hwnd, process_id );
	
	/* Check to see if the window belongs to the PID listed below*/
	if( process_id ){
		if( *process_id == 2584 ){		
			const int MAX_STR_LEN = 80;
			SIZE_T string_buffer_size = sizeof( LPWSTR ) * MAX_STR_LEN;
			LPWSTR class_name = ( LPWSTR ) CoTaskMemAlloc( string_buffer_size );
			LPWSTR title = ( LPWSTR ) CoTaskMemAlloc( string_buffer_size );
			
			/* Get some information about this window real quick.*/

			GetClassName( hwnd, class_name, string_buffer_size );
			GetWindowText( hwnd, title, string_buffer_size);			
			
			/* We only want to show visible windows */
			BOOL visible = IsWindowVisible(hwnd);
			if(visible){
				MessageBox( NULL, class_name, NULL, NULL );
				/* Now for this window, enumerate through the child windows(controls)*/
				EnumChildWindows( hwnd, EnumChildWindowsProc, NULL );
			}
			
			
			CoTaskMemFree( class_name );
			CoTaskMemFree( title );
			
		}
		CoTaskMemFree( process_id );
	}
	return TRUE;
}

// Get the child windows callback
BOOL CALLBACK EnumChildWindowsProc(HWND hwnd, LPARAM lParam){
		
		TCHAR buff[256];		
		int nCtrlID = ::GetDlgCtrlID(hwnd); // for safe keeps
		GetClassName(hwnd,buff,256);
		MessageBox(NULL,buff,L"CONTROL",NULL);

  		return TRUE;
}

