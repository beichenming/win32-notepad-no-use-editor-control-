#include <Windows.h>
#include <stdio.h>
#include "MyEditor.h"
#include "EditTool.h"
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void OnCommand(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void OnRegister(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdline, int iCmdShow);

static TCHAR szAppName[] = TEXT("NotePad");
static HWND  hwndEdit;
HWND         hwnd;
MSG          msg;
WNDCLASS     wndclass;



int WINAPI WinMain(HINSTANCE hInstance, 
	               HINSTANCE hPrevInstance,
				   PSTR szCmdline,
				   int iCmdShow)
{
    wndclass.style           = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc     = WndProc;
    wndclass.cbClsExtra      = 0;
    wndclass.cbWndExtra      = 0;
    wndclass.hInstance       = hInstance;
    wndclass.hIcon           = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor         = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground   = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName    = NULL;
    wndclass.lpszClassName   = szAppName;
    
    OnRegister(hInstance, hPrevInstance, szCmdline, iCmdShow);

    return msg.wParam;
}



void OnRegister(HINSTANCE hInstance, 
	            HINSTANCE hPrevInstance,
				PSTR szCmdline,
				int iCmdShow)
{
    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
        return ;
    }

    hwnd = CreateWindow(szAppName, TEXT("NotePad-wuyike(Sorry! Only supports single-line ctrl+c ctrl+v ctrl+x and Only supports mouse choice ,forget add shift choice)"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
         TranslateMessage(&msg);
         DispatchMessage(&msg);
    }
}



void OnCommand(HWND hwnd,
	           UINT message, 
			   WPARAM wParam, 
			   LPARAM lParam)
{
    switch (message)                                                                /*command choose*/
    {
     case WM_INPUTLANGCHANGE:                                                       /*WM_INPUTLANGCHANGE*/
          SendMessage(hwndEdit, WM_INPUTLANGCHANGE, wParam, lParam);
		  break;

     case WM_CREATE:                                                                /*WM_CREATE*/                                            
		  hwndEdit = TextEdit(((LPCREATESTRUCT)lParam) -> hInstance, hwnd);
          break;      
		  
     case WM_SIZE:                                                                  /*WM_SIZE*/
          MoveWindow(hwndEdit, 0, 0, LOWORD (lParam), HIWORD (lParam), TRUE);
          break;
                    
     case WM_SETFOCUS:                                                              /*WM_SETFOCUS*/
		  SetFocus(hwndEdit);
          break;
          
     case WM_KILLFOCUS:                                                             /*WM_KILLFOCUS*/
          SendMessage(hwndEdit, WM_KILLFOCUS, wParam, lParam);
          break;
          
     case WM_DESTROY:                                                               /*WM_DESTROY*/
          PostQuitMessage(0);
          break;
    }
}



LRESULT CALLBACK WndProc (HWND hwnd,
	                      UINT message, 
						  WPARAM wParam, 
						  LPARAM lParam)
{
     OnCommand(hwnd, message, wParam, lParam);
     return DefWindowProc (hwnd, message, wParam, lParam);
}

