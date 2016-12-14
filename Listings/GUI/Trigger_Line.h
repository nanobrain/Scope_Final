/**
  ******************************************************************************
  * @file    GUI/Trigge_Line.h 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Header for Custom widget for trigger line
  ******************************************************************************
  */
  
#ifndef __TRIGGER_LINE_H
#define __TRIGGER_LINE_H

typedef WM_HMEM Trigger_Line_Handle;

typedef struct {
	U8 Pressed;
	GUI_COLOR DisabledColor;
	GUI_COLOR NoFocusedColor;
	GUI_COLOR FocusedColor;
	U16				YPosition;
	int 			NumExtraBytes;
} Trigger_Line_Obj;

void Trigger_Line_Callback(WM_MESSAGE* pMsg);
int Trigger_Line_GetUserData(Trigger_Line_Handle hWin, void * pDest, int SizeOfBuffer);
int Trigger_Line_SetUserData(Trigger_Line_Handle hWin, void * pSrc,  int SizeOfBuffer);
Trigger_Line_Handle Trigger_Line_Create(int x0, int y0, int xSize, int ySize, WM_HWIN hWinParent, U32 Style, const char * pText, WM_CALLBACK * pfCallback, int NumExtraBytes);

#endif /* __TRIGGER_LINE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
