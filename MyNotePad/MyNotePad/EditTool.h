#ifndef EDITTOOL_H
#define EDITTOOL_H
#include <Windows.h>

#define ROWSMAX 100                       //列最大字符个数
#define COLSMAX 2000                      //行最大字符个数
#define MAXES 1000

extern int RowSum;                       //当前行数

typedef struct Node
{
	TCHAR date[COLSMAX] ;                                                //存放一行的数据
	int   width ;                                                        //字符总长度
	int   number ;                                                       //字母的数量
}RowNode;

int InsertRow(RowNode* prow[], int row);                                 //插入一行
int DeleteRow(RowNode* prow[], int row);                                 //删除一行
int InputChar(RowNode* prow[], int row, int cols, TCHAR c);              //插入一个字符
int DeleteChar(RowNode* prow[], int row, int cols);                      //删除一个字符
int CopyRow(RowNode* prow[], int row, int cols, int row2, int cols2);
void ClsChar(HDC hdc, int xLeft, int yTop, int xRight, int yBottom);
void SetFont(HDC hdc, DWORD dwCharSet);
void SetScroll(HWND hwnd, int cyBuffer, int Scroll);
int CharWidth(TCHAR c);

#endif