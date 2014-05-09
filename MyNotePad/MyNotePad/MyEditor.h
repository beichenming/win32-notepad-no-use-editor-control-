#ifndef MYEDITOR_H
#define MYEDITOR_H
#include <Windows.h>
LRESULT CALLBACK TextEditProc(HWND, UINT, WPARAM, LPARAM);
HWND TextEdit(HINSTANCE, HWND);
void OnCreate(HWND hwndEdit);                                     
void OnSize(HWND hwndEdit, LPARAM lParam);
void OnFocus(HWND hwndEdit);
void OnKillFocus(HWND hwndEdit);
void OnLeft();
void OnRight();
void OnUp();
void OnDown();
void OnDelete(HWND hwndEdit);
void OnBackspace(HWND hwndEdit);
void OnEnter(HWND hwndEdit);
void OnKeyDowns(HWND hwndEdit, LPARAM lParam, WPARAM wParam);
void OnAnyKey(HWND hwndEdit, WPARAM wParam);
void OnCharKey(HWND hwndEdit, LPARAM lParam, WPARAM wParam);
void OnPaint(HWND hwndEdit);
void OnLbuttonDown(HWND hwndEdit, LPARAM lParam);
void OnLbuttonUp(HWND hwndEdit, LPARAM lParam);
void OnMouseMove(HWND hwndEdit, LPARAM lParam);
void OnCut(HWND hwndEdit);
void OnCopy(HWND hwndEdit);
void OnPaste(HWND hwndEdit);
#endif