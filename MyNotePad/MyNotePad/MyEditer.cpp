#include "MyEditor.h"
#include "EditTool.h"
#include "resource.h"

HDC              hdc;
PAINTSTRUCT      ps;
TEXTMETRIC       tm;
SCROLLINFO       si;
SIZE             ocaret, ncaret;
HPEN             rpen;
static DWORD     dwCharSet = DEFAULT_CHARSET;
static int       cxChar, cyChar;                             
static int       cxClient, cyClient;                          
static int       cxBuffer, cyBuffer;                          
static int       xCaret, yCaret;                              
static int       iVertPos, iHorzPos;
static int       xAway = 30;                                  
static int       xFocu = xAway;                               
static TCHAR*    pBuffer = NULL;
static RowNode*  PRow[ROWSMAX];
  
int              x = 0, y = 0, i = 0, j = 0, iPaintBeg, iPaintEnd;
int              x1 = 0, x2 = 0, y1 = 0, y2 = 0, strlength = 0;
int              lbdown = 0, drag = 0, sel = 0, move = 0;
int              xlight = 0, ylight = 0;
int              xmove1 = 0, xmove2 = 0, ymove1 = 0, ymove2 = 0;
char             *cBuffer = NULL; 

void OnCreate(HWND hwndEdit) 
{
	  InsertRow(PRow, ROWSMAX);                      
      hdc = GetDC(hwndEdit);                         
      SelectObject(hdc, CreateFont(0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, NULL));
      GetTextMetrics(hdc, &tm);                       
      cxChar = tm.tmAveCharWidth;                    
      cyChar = tm.tmHeight;                          
      DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
      ReleaseDC(hwndEdit, hdc);
}

void OnSize(HWND hwndEdit, LPARAM lParam) 
{
	  cxClient = LOWORD(lParam);                   
      cyClient = HIWORD(lParam);                      
      cxBuffer = max (1, cxClient / cxChar);           
      cyBuffer = max (1, cyClient / cyChar);        
      SetScroll(hwndEdit, cyBuffer, SB_VERT);         
      InvalidateRect(hwndEdit, NULL, TRUE);          
}

void OnFocus(HWND hwndEdit)
{
      CreateCaret(hwndEdit, NULL, 1, cyChar);                  
      SetCaretPos(xFocu, (yCaret - iVertPos) * cyChar);         
      ShowCaret(hwndEdit);                                    
}

void OnKillFocus(HWND hwndEdit)
{
	   HideCaret(hwndEdit);                                  
       DestroyCaret();                                        
}

void OnLeft()
{
     if (xCaret == 0 && yCaret != 0)
     {   
         yCaret --;
         xCaret = PRow[yCaret] -> number;
         xFocu = PRow[yCaret] -> width + xAway;
      }
     else
     {
          if ((xCaret - 1) >= 0)
          {
			 xCaret --;
             if ((PRow[yCaret] -> date[xCaret] >> 8) > 0)
             {
				xFocu -= cxChar * 2;
			 }
             else
             {
			    xFocu -= cxChar;
			 }
          }
     }
}

void OnRight()
{
     if (xCaret == PRow[yCaret] -> number && yCaret != RowSum - 1)
     {
          yCaret ++;
          xFocu = xAway;
		  xCaret = xAway;
     }
     else
     {
          if ((xCaret + 1) <= PRow[yCaret] -> number)
          {
             if ((PRow[yCaret] -> date[xCaret] >> 8) > 0)
             {
			      xFocu += cxChar * 2;
			 }
             else
             {  
			      xFocu += cxChar;
		     }
			 xCaret++;
         }
     }
}

void OnUp()
{
	  yCaret = max(yCaret - 1, 0);
      if (xFocu >= PRow[yCaret] -> width)
      {
		 xCaret = PRow[yCaret] -> number;
		 xFocu = PRow[yCaret] -> width + xAway;
	  }
	  else
	  {
		 while (x < xFocu)
	     {
			x += CharWidth(PRow[yCaret] -> date[i]) * cxChar;
			i ++;
		 }
		 xCaret = i;
		 xFocu = x;
	  }
}

void OnDown()
{
	   yCaret = min(yCaret + 1, RowSum - 1);
       if (xFocu >= PRow[yCaret] -> width)
	   {
			xCaret = PRow[yCaret] -> number;
			xFocu = PRow[yCaret] -> width + xAway;
	   }
	   else
	   {
		   while (x < xFocu)
		   {
			   x += CharWidth(PRow[yCaret] -> date[i]) * cxChar;
			   i ++;
		  }
		  xCaret = i;
		  xFocu = x;
	  }
}

void OnDelete(HWND hwndEdit)
{
	   if (xCaret < PRow[yCaret] -> number)
       {
          DeleteChar(PRow, yCaret, xCaret);
          HideCaret(hwndEdit);
          hdc = GetDC(hwndEdit);
          SetFont(hdc, dwCharSet);
          ClsChar(hdc, xFocu, (yCaret - iVertPos) * cyChar, min(cxBuffer * cxChar, 
						PRow[yCaret] -> width) + 1, (yCaret - iVertPos + 1) * cyChar);
          TextOut(hdc, xFocu, (yCaret - iVertPos) * cyChar, 
						PRow[yCaret] -> date + xCaret, 
						PRow[yCaret] -> number - xCaret);
          DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
          if ((PRow[yCaret] -> date[xCaret] >> 8) > 0)
          {
			 PRow[yCaret] -> width -= cxChar * 2;
		  }
          else
          {   
			 PRow[yCaret] -> width -= cxChar;
		  }
          ReleaseDC(hwndEdit, hdc);
          ShowCaret(hwndEdit);
       }
	   InvalidateRect(hwndEdit, NULL, true);
       UpdateWindow(hwndEdit);
}

void OnBackspace(HWND hwndEdit)
{
	 if (xCaret > 0)
     {
         xCaret --;
         if ((PRow[yCaret] -> date[xCaret] >> 8) > 0)
         {
			xFocu -= cxChar * 2;
		 }
         else
         { 
		    xFocu -= cxChar;
	     }
         SendMessage(hwndEdit, WM_KEYDOWN, VK_DELETE, 1);                                                              /*send delete msg*/
     }
     else if (xCaret == 0 && yCaret != 0)
     {
         x = PRow[yCaret - 1] -> number;
		 xFocu = PRow[yCaret - 1] -> width + xAway;
         CopyRow(PRow, yCaret, xCaret, yCaret - 1, PRow[yCaret - 1] -> number);                                        /*copy row*/
		 PRow[yCaret-1] -> width = xFocu + PRow[yCaret] -> width;
         DeleteRow(PRow, yCaret);
         xCaret = x;
		 yCaret --;
         HideCaret(hwndEdit);
         hdc = GetDC(hwndEdit);
         SetFont(hdc, dwCharSet);                        
         ClsChar(hdc, 0 + xFocu, (yCaret - iVertPos) * cyChar, cxBuffer * cxChar,  (RowSum - iVertPos + 1) * cyChar);     /*ClsChar*/

         for (y = yCaret; y < RowSum; y++)
		 {
             TextOut(hdc, 0 + xFocu, (y - iVertPos) * cyChar, PRow[y] -> date, PRow[y] -> number);                    /*textout String*/
		 }
		 DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
         ReleaseDC(hwndEdit, hdc);
         ShowCaret(hwndEdit);
      }
	  InvalidateRect(hwndEdit, NULL, true);
      UpdateWindow(hwndEdit);                                                                                         /*update the window*/
}

void OnEnter(HWND hwndEdit)
{
	  xFocu = xAway;
      InsertRow(PRow, yCaret + 1);                                                                                     /*insert row*/
      HideCaret(hwndEdit);
      hdc = GetDC(hwndEdit);
      SetFont(hdc, dwCharSet);
      if (xCaret < PRow[yCaret] -> number)
	  {
		  CopyRow(PRow, yCaret, xCaret, yCaret + 1, 0);                                                                /*copy row*/
	  }

      ClsChar(hdc, 0 + xFocu, (yCaret - iVertPos) * cyChar, cxBuffer * cxChar, (RowSum - iVertPos) * cyChar);      

      for (y = yCaret; y < RowSum; y++)                                                                                /*textout String*/
	  {
           TextOut(hdc, 0 + xFocu, (y - iVertPos) * cyChar, PRow[y] -> date, PRow[y] -> number);
	  }

      DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
      yCaret ++;
      ReleaseDC(hwndEdit, hdc);
      ShowCaret(hwndEdit);
      xCaret = 0;
      InvalidateRect(hwndEdit, NULL, true);
      UpdateWindow(hwndEdit);                                                                                           /*update the window*/
}

void OnKeyDowns(HWND hwndEdit, LPARAM lParam, WPARAM wParam)
{
	  for (i = 0; i < (int)LOWORD(lParam); i++)
      {
         switch(wParam)
         {
             case VK_HOME:
                  xFocu = xCaret = xAway;
                  break;
               
             case VK_END:
                  xCaret = PRow[yCaret] -> number;
				  xFocu = PRow[yCaret] -> width;
                  break;
               
             case VK_PRIOR:
                  yCaret = 0;
                  break;
               
             case VK_NEXT:
                  yCaret = RowSum - 1;
                  break;
               
             case VK_LEFT:
                  OnLeft();
                  break;
               
             case VK_RIGHT:
                  OnRight();
                  break;
               
             case VK_UP:
                  OnUp();
                  break;
               
             case VK_DOWN:
                  OnDown();
                  break;
               
             case VK_DELETE:
                  OnDelete(hwndEdit);
                  break;
		 }
      }
       SetCaretPos(xFocu, (yCaret - iVertPos) * cyChar);
}

void OnAnyKey(HWND hwndEdit, WPARAM wParam)
{
	  InputChar(PRow, yCaret, xCaret, (TCHAR)wParam);
      HideCaret(hwndEdit);
      hdc = GetDC(hwndEdit);
      SetFont(hdc, dwCharSet);
      TextOut(hdc, xFocu, (yCaret - iVertPos) * cyChar, 
		      PRow[yCaret] -> date + xCaret,
			  PRow[yCaret] -> number - xCaret);
      DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));

      ReleaseDC(hwndEdit, hdc);
      ShowCaret(hwndEdit);
      xCaret ++;
      if ((wParam >> 8) > 0)
      {
           PRow[yCaret] -> width += cxChar * 2;
           xFocu += cxChar * 2;
      }
      else
      {
           PRow[yCaret] -> width += cxChar;
           xFocu += cxChar;
      }
}

void OnCharKey(HWND hwndEdit, LPARAM lParam, WPARAM wParam)
{
	    for (i = 0; i < (int)LOWORD(lParam); i++)
        {
            switch (wParam)
            {
               case '\b':                                   /*backspace*/             
                    OnBackspace(hwndEdit);
                    break;
                    
               case '\t':                                   /*tab*/   
				    for (int i = 0; i < 4; i++)
				    {
						OnAnyKey(hwndEdit, '\x20');
					}
                    break ;
                    
               case '\r':                                   /*enter*/   
				    OnEnter(hwndEdit);
                    break ;

			   case '\3':                                   /*ctrl + c*/
				   OnCopy(hwndEdit);
				   break;

			   case '\x16':                                 /*ctrl + v*/
				   OnPaste(hwndEdit);
				   break;

			   case '\x18':                                 /*ctrl + x*/
				   OnCut(hwndEdit);
				   break;

               default:                                     /*anykey*/
                    OnAnyKey(hwndEdit, wParam);
                    break;
           }
       }
       if (yCaret >= cyBuffer+iVertPos)
       {
            x = yCaret - cyBuffer + 1;
            SendMessage(hwndEdit, WM_VSCROLL, SB_THUMBTRACK, x + 1);
            return ;
       }
       else if (yCaret < iVertPos)
       {
            SendMessage(hwndEdit, WM_VSCROLL, SB_THUMBTRACK, yCaret + 1);
            return ;
       }

       SetCaretPos(xFocu, (yCaret-iVertPos) * cyChar);
}

void OnPaint(HWND hwndEdit)
{
	   hdc = BeginPaint(hwndEdit, &ps);
       SetFont(hdc, dwCharSet);
       SetWindowOrgEx(hdc, cxChar * iHorzPos, cyChar * iVertPos, NULL);
       iPaintBeg = max(0, iVertPos + ps.rcPaint.top / cyChar);
       iPaintEnd = min(RowSum - 1, iVertPos + ps.rcPaint.bottom / cyChar);
	   
	   if (lbdown == 1)                                                                         /*high light*/
	   {
		   HBRUSH hbr = CreateSolidBrush(RGB(220, 220, 220));
		   SetBkMode(hdc, TRANSPARENT);
           HBRUSH  hOldbr = (HBRUSH)SelectObject(hdc, hbr);
           RECT rc = {xAway, ylight - 1, COLSMAX, ylight + cyChar + 1};
           FillRect(hdc, &rc, hbr);
           SelectObject(hdc, hOldbr);
           DeleteObject(hbr);
	   }
	   

	   MoveToEx(hdc, xAway - 5, 0, NULL);
       LineTo(hdc, xAway - 5, MAXES); 
       for (y = iPaintBeg; y <= iPaintEnd; y++)                                                  /*row number*/
       {
			TextOut(hdc, 0 + xAway, y * cyChar, PRow[y] -> date, PRow[y] -> number);
	   }
	   
	   for (i = 0; i < RowSum; i++)                                                             /*textout string*/
	   {
		 char buf[123];
		 int ll = wsprintf((LPWSTR)buf , TEXT("%d"),i + 1);
		 TextOut(hdc, 0, i * cyChar, (LPWSTR)buf, ll);
	   }
	   
	   DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
       EndPaint(hwndEdit, &ps);
}

void OnLButtonDown(HWND hwndEdit, LPARAM lParam)
{
	  x1 = LOWORD(lParam);
      y1 = HIWORD(lParam);
	  ocaret.cx = x1 / cxChar;                                             /*start x*/                        
      ocaret.cy = y1 / cyChar;                                             /*start y*/
 
      if ((ocaret.cy) > RowSum - 1)
	  {	   
	      xCaret = PRow[RowSum - 1] -> number;
		  xFocu  = PRow[RowSum - 1] -> width + xAway;
          yCaret = RowSum - 1;
		  xlight = PRow[RowSum - 1] -> width;
		  ylight = (RowSum - 1) * cyChar;
		  SetCaretPos(xFocu, (RowSum - 1) * cyChar);
	  }
      else  if (ocaret.cx > PRow[ocaret.cy] -> number) 
	  {
		  xCaret = PRow[ocaret.cy] -> number;
		  xFocu  = PRow[ocaret.cy] -> width + xAway;
          yCaret = ocaret.cy;
		  xlight = PRow[yCaret] -> width;
		  ylight = yCaret * cyChar;
		  SetCaretPos(xFocu, yCaret * cyChar);
	  }
      else
	  {
		  xCaret = ocaret.cx;
		  xFocu = xCaret * cxChar + xAway;
          yCaret = ocaret.cy;
		  xlight = xFocu;
		  ylight = yCaret * cyChar;
		  SetCaretPos(xFocu, yCaret * cyChar);
	  }       
	  lbdown = 1;
	  InvalidateRect(hwndEdit, NULL, true);
      UpdateWindow(hwndEdit);
}

void OnLButtonUp(HWND hwndEdit, LPARAM lParam)
{
	 x2 = LOWORD(lParam);                                                                   /*mouse x*/
     y2 = HIWORD(lParam);                                                                   /*mouse y*/
	 if (drag == 1)
     {
         ncaret.cx = x2 / cxChar;                                                           /*end x*/
         ncaret.cy = y2 / cyChar;                                                           /*end y*/
         ocaret.cx = x1 / cxChar;                                                           /*start x*/
         ocaret.cy = y1 / cyChar;                                                           /*start y*/
         if (ncaret.cx > PRow[ncaret.cy] -> number)
		 {
			  xCaret = PRow[ncaret.cy] -> number;
              xFocu = xCaret * cxChar + xAway;                             
		 }
		 else
		 {
		      xCaret = ncaret.cx;
		      xFocu = ncaret.cx * cxChar + xAway;
		 }
         yCaret = ncaret.cy;
		 SetCaretPos(xFocu, yCaret * cyChar);
		 sel = 1;     
	}
	else
	{
        if ((ocaret.cy) > RowSum - 1)
		{	
			xCaret = PRow[RowSum - 1] -> number;
		    xFocu = PRow[RowSum - 1] -> width + xAway;
            yCaret = RowSum - 1;
			SetCaretPos(xFocu, (RowSum - 1) * cyChar);
		 }
	    else  if (ocaret.cx > PRow[ocaret.cy] -> number) 
		{
			  xCaret = PRow[ocaret.cy] -> number;
		      xFocu = PRow[ocaret.cy] -> width + xAway;
              yCaret = ocaret.cy;
			  SetCaretPos(xFocu, yCaret * cyChar);
		 }
		 else
		 {
			  xCaret = ocaret.cx;
		      xFocu = xCaret * cxChar + xAway;
              yCaret = ocaret.cy;
		      SetCaretPos(xFocu, yCaret * cyChar);                               /*set foucs*/
		 }
		 sel = 0;
	 }		  
	   lbdown = 0;                                                             /*up the mouse*/
	   move = 0;
}

void OnMouseMove(HWND hwndEdit, LPARAM lParam)
{
	   if (lbdown == 1)
	   {
           x2 = LOWORD(lParam);
           y2 = HIWORD(lParam);
		   ocaret.cx = x1 / cxChar;                                                    /*end x*/                            
           ocaret.cy = y1 / cyChar;                                                    /*end y*/
		   ncaret.cx = x2 / cxChar;                                                    /*start x*/
           ncaret.cy = y2 / cyChar;                                                    /*start y*/
		   move = 1;
		   if (ocaret.cy == ncaret.cy)
		   {
			   if (ocaret.cx <= PRow[ocaret.cy] -> number)				   
			   {
				   if (ncaret.cx <= PRow[ncaret.cy] -> number)
				   {
				     hdc = GetDC(hwndEdit);
			         SelectObject(hdc, rpen);
					 xmove1 = ocaret.cx * cxChar + xAway;
					 ymove1 = (ocaret.cy + 1) * cyChar;
					 xmove2 = ncaret.cx * cxChar + xAway;
					 ymove2 = (ncaret.cy + 1) * cyChar;
		             MoveToEx(hdc, xmove1, ymove1, NULL);
			         LineTo(hdc, xmove2, ymove2);     
			         ReleaseDC(hwndEdit, hdc);
			         drag = 1;	
				   }
				   else 
				   {
				      hdc = GetDC(hwndEdit);
			          SelectObject(hdc, rpen);
					  xmove1 = ocaret.cx * cxChar + xAway;
					  ymove1 = (ocaret.cy + 1) * cyChar;
					  xmove2 = PRow[ncaret.cy]->width + xAway;
					  ymove2 = (ncaret.cy + 1) * cyChar;
		              MoveToEx(hdc, xmove1, ymove1, NULL);
			          LineTo(hdc, xmove2, ymove2);
			          ReleaseDC(hwndEdit, hdc);
			          drag = 1;	
				   }
			   }
			 }
		 }
		 else
		 {
			 drag = 0;
	     }
	    
}

void OnCut(HWND hwndEdit)
{
	  hdc = GetDC(hwndEdit);
      if (cBuffer != NULL)                                       /*check cBuffer is empty*/
	  {
		  free(cBuffer);
	  }

	  strlength = 0; 
	  if (PRow[yCaret] -> number > max(ocaret.cx, ncaret.cx))                      /*check max strlength*/
	  {
		  strlength = max(ocaret.cx, ncaret.cx) - min(ocaret.cx, ncaret.cx);
	  }
	  else
	  {
		  strlength = PRow[yCaret] -> number - min(ocaret.cx, ncaret.cx);
	  }

	  cBuffer = (char *)calloc(strlength + 1, sizeof(char));
	  if (cBuffer == NULL)
	  {
		  return ;
	  }
	
	  for (i = 0, j = min(ocaret.cx, ncaret.cx); i < strlength && j <= max(ocaret.cx, ncaret.cx); i++, j++)             /*copy string*/
	  {
		    if (j > PRow[yCaret] -> number)
			{
				break;
			}
		    cBuffer[i] = PRow[yCaret] -> date[j];
	  }  
	  cBuffer[i] = '\0';

	  for (j = 0; j < strlength; j++)                                                                          /*delete string*/
	  {
          DeleteChar(PRow, yCaret, min(ocaret.cx, ncaret.cx));
	  }
	 
	  PRow[yCaret] -> width = PRow[yCaret] -> number * cxChar;
	  xCaret = min(ocaret.cx, ncaret.cx);
	  xFocu = xCaret * cxChar + xAway;                                                                       /*set focus*/
	  SetCaretPos(xFocu, yCaret * cyChar);             
	  InvalidateRect(hwndEdit, NULL, true);
	  UpdateWindow(hwndEdit);
	  sel = 0;
}

void OnCopy(HWND hwndEdit)
{
	  hdc = GetDC(hwndEdit);			
	  if (cBuffer != NULL)                                                                                            /*check cBuffer is empty*/
	  {
		  free(cBuffer);
	  }
	  strlength = 0; 
	  if (PRow[yCaret] -> number > max(ocaret.cx, ncaret.cx))                                                          /*check max strlength*/
	  {
		  strlength = max(ocaret.cx, ncaret.cx) - min(ocaret.cx, ncaret.cx);
	  }
	  else
	  {
		  strlength = PRow[yCaret] -> number - min(ocaret.cx, ncaret.cx);
	  }

	  cBuffer = (char *)calloc(strlength + 1, sizeof(char));                                                            /*calloc cBuffer*/

	  if (cBuffer == NULL)
	  {
		  return ;
	  }

	  for (i = 0, j = min(ocaret.cx, ncaret.cx); i < strlength && j <= max(ocaret.cx, ncaret.cx); i++, j++)              /*copy string*/
	  {
		  if (j > PRow[yCaret] -> number)
		  {
				break;
		  }
		  cBuffer[i] = PRow[yCaret] -> date[j];
	  }  

	  cBuffer[i] = '\0';
	  ReleaseDC(hwndEdit, hdc);                                                                                          /*ReleaseDC*/
	  sel = 0;  
}

void OnPaste(HWND hwndEdit)
{
	  hdc = GetDC(hwndEdit) ;
	  HideCaret(hwndEdit);
	  if (cBuffer == NULL)
	  {
		  return ;
	  }

	  for (i = 0; i < strlength; i++)                                                                                   /*paste string*/
	  {
		  InputChar(PRow, yCaret, xCaret, cBuffer[i]);
		  TextOut(hdc, xFocu, yCaret * cyChar, PRow[yCaret] -> date + xCaret, PRow[yCaret] -> number - xCaret);
		  xCaret++;
		  if ((cBuffer[i] >> 8) > 0)
          {
               PRow[yCaret] -> width += cxChar * 2;
               xFocu += cxChar * 2;
          }
          else
          {
               PRow[yCaret]->width += cxChar ;
               xFocu += cxChar ;
		  }
	   }
		 SetCaretPos(xFocu, yCaret * cyChar);                                                                          /*set focus*/
		 InvalidateRect(hwndEdit, NULL, true);
	     UpdateWindow(hwndEdit);
         ReleaseDC(hwndEdit, hdc) ;
	     ShowCaret(hwndEdit);
}
  
HWND TextEdit(HINSTANCE hInstance,
	          HWND hwnd)
{
    static TCHAR TextEditClass[] = TEXT("TextEdit");
    HWND hwndEdit;
    WNDCLASS wndclass;
    wndclass.style              = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc        = TextEditProc;
    wndclass.cbClsExtra         = 0;
    wndclass.cbWndExtra         = 0;
    wndclass.hInstance          = hInstance;
    wndclass.hIcon              = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor            = LoadCursor(NULL, IDC_IBEAM);                  
    wndclass.hbrBackground      = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName       = NULL;
    wndclass.lpszClassName      = TextEditClass;
    RegisterClass(&wndclass);
    hwndEdit = CreateWindow(TextEditClass, 
		                     NULL,
                             WS_CHILDWINDOW | WS_VISIBLE | WS_BORDER,
                             0, 0, 0, 0,
                             hwnd, NULL,
                             NULL, NULL) ;
    return hwndEdit ;
}


LRESULT CALLBACK TextEditProc (HWND hwndEdit, 
	                           UINT message, 
							   WPARAM wParam, 
							   LPARAM lParam)
{
    switch(message)
    {
       case WM_INPUTLANGCHANGE:                       /*WM_INPUTLANGCHANGE*/
            dwCharSet = wParam ;
            return 0 ;

       case WM_CREATE:                                /*WM_CREATE*/
            OnCreate(hwndEdit);
            return 0 ;      
        
       case WM_SIZE:                                  /*WM_SIZE*/
            OnSize(hwndEdit, lParam);
            return 0 ;
                   
       case WM_SETFOCUS:                              /*WM_SETFOCUS*/
            OnFocus(hwndEdit);
            return 0 ;
       
       case WM_KILLFOCUS:                             /*WM_KILLFOCUS*/
            OnKillFocus(hwndEdit);
            return 0 ;
			
       case WM_KEYDOWN:                               /*WM_KEYDOWN*/
            OnKeyDowns(hwndEdit, lParam, wParam);
            return 0 ;

       case WM_CHAR:                                   /*WM_CHAR*/
            OnCharKey(hwndEdit, lParam, wParam);
            return 0 ;
		
       case WM_PAINT:                                  /*WM_PAINT*/
            OnPaint(hwndEdit);
            return 0;
      
       case WM_LBUTTONDOWN:                            /*WM_LBUTTONDOWN*/
		    OnLButtonDown(hwndEdit, lParam);
     		 return 0;  

	   case WM_LBUTTONUP:                              /*WM_LBUTTONUP*/
		    OnLButtonUp(hwndEdit, lParam);
		    return 0;
      
       case WM_MOUSEMOVE:                              /*WM_MOUSEMOVE*/
		    OnMouseMove(hwndEdit, lParam);
	        return 0;
	  
       case WM_DESTROY:                                /*WM_DESTROY*/
            PostQuitMessage(0);
            return 0;
     }
     return DefWindowProc(hwndEdit, message, wParam, lParam);
}

