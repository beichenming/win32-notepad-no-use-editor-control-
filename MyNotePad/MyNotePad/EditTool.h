#ifndef EDITTOOL_H
#define EDITTOOL_H
#include <Windows.h>

#define ROWSMAX 100                       //������ַ�����
#define COLSMAX 2000                      //������ַ�����
#define MAXES 1000

extern int RowSum;                       //��ǰ����

typedef struct Node
{
	TCHAR date[COLSMAX] ;                                                //���һ�е�����
	int   width ;                                                        //�ַ��ܳ���
	int   number ;                                                       //��ĸ������
}RowNode;

int InsertRow(RowNode* prow[], int row);                                 //����һ��
int DeleteRow(RowNode* prow[], int row);                                 //ɾ��һ��
int InputChar(RowNode* prow[], int row, int cols, TCHAR c);              //����һ���ַ�
int DeleteChar(RowNode* prow[], int row, int cols);                      //ɾ��һ���ַ�
int CopyRow(RowNode* prow[], int row, int cols, int row2, int cols2);
void ClsChar(HDC hdc, int xLeft, int yTop, int xRight, int yBottom);
void SetFont(HDC hdc, DWORD dwCharSet);
void SetScroll(HWND hwnd, int cyBuffer, int Scroll);
int CharWidth(TCHAR c);

#endif