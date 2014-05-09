#include "EditTool.h"

int RowSum;

int InsertRow(RowNode* prow[], int row)                                
{
	int i;
	if (row >= RowSum)
	{
		prow[RowSum] = (RowNode *)malloc(sizeof(RowNode));
		prow[RowSum] -> number = 0 ;
		prow[RowSum] -> width = 0 ;
	}
	else if (row < RowSum)
	{
		for (i = RowSum; i > row; i--)
		{
			prow[i] = prow[i-1];
		}
		prow[row] = (RowNode *)malloc(sizeof(RowNode));
		prow[row] -> number = 0;
		prow[RowSum] -> width = 0;
	}
	if (prow[RowSum] == NULL)
		return -1;
	return RowSum++;
}

int DeleteRow(RowNode* prow[], int row)                               
{
	int i;
	if (row >= RowSum)
		free(prow[RowSum]);
	else if (row < RowSum)
	{
		free(prow[row]);
		for (i = row; i < RowSum-1; i++)
		{
			 prow[i] = prow[i+1];
		}
	}
	return RowSum--;
}

    
int InputChar(RowNode* prow[], int row, int cols, TCHAR c)               
{
	int i;
	if (row >= RowSum) 
	{
		row = RowSum - 1;
	}
	if (cols >= prow[row] -> number) 
	{
		prow[row] -> date[prow[row] -> number] = c;
	}
	else if (cols < prow[row] -> number)
	{
		for (i = prow[row]->number; i > cols; i--)
		{
			prow[row] -> date[i] = prow[row] -> date[i - 1];
		}
		prow[row] -> date[cols] = c;
	}
	return (prow[row] -> number)++;
}

int DeleteChar(RowNode* prow[], int row, int cols)                    
{
	int i;
	if (prow[row] -> number == 0) 
    {
		return 0;
	}
	for (i = cols; i < (prow[row] -> number - 1); i++)
	{
		prow[row] -> date[i] = prow[row] -> date[i + 1];
	}
	return --(prow[row] -> number);
}

int CopyRow(RowNode* prow[], int row, int cols, int row2, int cols2)
{
	int i;
	for (i = cols; i < prow[row] -> number; i++, cols2++)
	{
		prow[row2] -> date[cols2] = prow[row] -> date[i];
	}
	prow[row] -> number = cols;
	prow[row2] -> number = cols2;
	return 1;
}

void ClsChar(HDC hdc, int xLeft, int yTop, int xRight, int yBottom)
{
	SelectObject(hdc, CreatePen(PS_SOLID, 1, GetSysColor(COLOR_WINDOW)));
	SelectObject(hdc, CreateSolidBrush(GetSysColor(COLOR_WINDOW)));
	Rectangle(hdc, xLeft, yTop, xRight, yBottom);
	DeleteObject(SelectObject(hdc, GetStockObject(BLACK_PEN)));
	DeleteObject(SelectObject(hdc, GetStockObject(WHITE_BRUSH)));
}

void SetFont(HDC hdc, DWORD dwCharSet)
{
	SelectObject(hdc, CreateFont(0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, NULL)) ;
	SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
	SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
}

void SetScroll(HWND hwnd, int cyBuffer, int Scroll)
{
	SCROLLINFO  si;
	si.cbSize = sizeof(si);
	si.fMask  = SIF_RANGE | SIF_PAGE;
	si.nMin   = 0;
	si.nMax   = RowSum - 1;
	si.nPage  = cyBuffer;
	SetScrollInfo(hwnd, Scroll, &si, TRUE);
}

int CharWidth(TCHAR c)
{
	if((c >> 8) > 0)
	{
		return 2 ;
	}
	return 1 ;
}

