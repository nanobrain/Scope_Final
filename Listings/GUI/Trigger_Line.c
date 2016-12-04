/**
  ******************************************************************************
  * @file    GUI/Trigge_Line.c 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Custom widget for trigger line
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "GUI.h"
#include "HEADER.h"
#include "FRAMEWIN.h"
#include "DIALOG.h"
#include "Trigger_Line.h"
#include <stdlib.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TRANSPARENCY_LOW						(0x40uL << 24)
#define TRANSPARENCY_MEDIUM					(0x80uL << 24)
#define TRANSPARENCY_HIGH						(0xC0uL << 24)
#define TRANSPARENCY_VERY_HIGH			(0xF0uL << 24)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

const Trigger_Line_Obj Trigger_Line_Default = {
	0, 							// Pressed
	GUI_LIGHTGRAY, 	// DisabledColor
	GUI_ORANGE, 		// NoFocusedColor
	GUI_BLUE, 			// FocusedColor
	NULL, 					// YPosition
	0 							// NumExtraBytes
};

/* Private function prototypes -----------------------------------------------*/
void Trigger_Line_Callback(WM_MESSAGE * pMsg) {
  Trigger_Line_Handle	hWin;
  GUI_PID_STATE*			pState;
  Trigger_Line_Obj		TriggerLine;
  GUI_RECT						WinRect;
  GUI_COLOR						Color;
  U8									Pressed;
	U32									Transparency;
	U16									Height;
	U16									Width;

  hWin = pMsg->hWin;
  WM_GetWindowRectEx(hWin, &WinRect);
	Height = WinRect.y1 - WinRect.y0;
	Width = WinRect.x1 - WinRect.x0;
  WM_GetUserData(hWin, &TriggerLine, sizeof(Trigger_Line_Obj));
  switch (pMsg->MsgId) {
  case WM_PAINT:
    if (WM_IsEnabled(hWin))
		{
      if (TriggerLine.Pressed)
			{
        Color = TriggerLine.FocusedColor;
			}
			else
			{
        Color = TRANSPARENCY_VERY_HIGH | TriggerLine.NoFocusedColor;
			}
		}
		else
		{
			Color = TriggerLine.DisabledColor;
		}
		
		GUI_EnableAlpha(1);
		GUI_SetPenSize(5);
		GUI_SetColor(GUI_BLUE);
		GUI_DrawCircle(10,Height/2,10);
		GUI_SetPenSize(1);
		GUI_SetColor(Color);
		GUI_FillCircle(10,Height/2,10);
		GUI_DrawLine(WinRect.x0,Height/2,Width,Height/2);
		
    break;
  case WM_TOUCH:
    pState = (GUI_PID_STATE *)pMsg->Data.p;
    if (pState)
      Pressed = pState->Pressed;
    else
      Pressed = 0;

    if (TriggerLine.Pressed != Pressed) {
      TriggerLine.Pressed = Pressed;
      WM_SetUserData(hWin, &TriggerLine, sizeof(Trigger_Line_Obj));
      if (Pressed)
        WM_SetFocus(hWin);
      WM_InvalidateWindow(hWin);
    }
    break;
  case WM_SET_FOCUS:
    if (pMsg->Data.v) {
      pMsg->Data.v = 0;
    } 
    WM_InvalidateWindow(hWin);
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

int Trigger_Line_GetUserData(Trigger_Line_Handle hWin, void * pDest, int SizeOfBuffer) {
  Trigger_Line_Obj		TriggerLine;
  int            			NumBytes;
  U8* 								pExtraBytes;

  if (SizeOfBuffer <= 0) {
    return 0;
  }
  WM_GetUserData(hWin, &TriggerLine, sizeof(Trigger_Line_Obj));
  pExtraBytes = (U8 *)malloc(sizeof(Trigger_Line_Obj) + TriggerLine.NumExtraBytes);
  if (pExtraBytes) {
    WM_GetUserData(hWin, pExtraBytes, sizeof(Trigger_Line_Obj) + TriggerLine.NumExtraBytes);
    if (SizeOfBuffer >= TriggerLine.NumExtraBytes) {
      NumBytes = TriggerLine.NumExtraBytes;
    } else {
      NumBytes = SizeOfBuffer;
    }
    GUI_MEMCPY(pDest, pExtraBytes + sizeof(Trigger_Line_Obj), NumBytes);
    free(pExtraBytes);
    return NumBytes;
  }
  return 0;
}

int Trigger_Line_SetUserData(Trigger_Line_Handle hWin, void * pSrc, int SizeOfBuffer) {
  Trigger_Line_Obj		TriggerLine;
  int            			NumBytes;
  U8*									pExtraBytes;

  if (SizeOfBuffer <= 0) {
    return 1;
  }
  WM_GetUserData(hWin, &TriggerLine, sizeof(Trigger_Line_Obj));
  pExtraBytes = (U8 *)malloc(sizeof(Trigger_Line_Obj) + TriggerLine.NumExtraBytes);
  if (pExtraBytes) {
    WM_GetUserData(hWin, pExtraBytes, sizeof(Trigger_Line_Obj) + TriggerLine.NumExtraBytes);
    if (SizeOfBuffer >= TriggerLine.NumExtraBytes) {
      NumBytes = TriggerLine.NumExtraBytes;
    } else {
      NumBytes = SizeOfBuffer;
    }
    GUI_MEMCPY(pExtraBytes + sizeof(Trigger_Line_Obj), pSrc, NumBytes);
    WM_SetUserData(hWin, pExtraBytes, sizeof(Trigger_Line_Obj) + TriggerLine.NumExtraBytes);
    free(pExtraBytes);
    return 0;
  }
  return 1;
}

Trigger_Line_Handle Trigger_Line_Create(int x0, int y0, int xSize, int ySize, WM_HWIN hWinParent, U32 Style, const char * pText, WM_CALLBACK * pfCallback, int NumExtraBytes) {
  Trigger_Line_Handle   hWin;
  Trigger_Line_Obj      TriggerLine;
  WM_CALLBACK*					pfUsed;
	
  if (pfCallback) {
    pfUsed = pfCallback;
  } else {
    pfUsed = Trigger_Line_Callback;
  }
  TriggerLine               = Trigger_Line_Default;
  TriggerLine.NumExtraBytes = NumExtraBytes;
	
  hWin = WM_CreateWindowAsChild(x0, y0, xSize, ySize, hWinParent, Style, pfUsed, sizeof(Trigger_Line_Obj) + NumExtraBytes);
  WM_SetUserData(hWin, &TriggerLine, sizeof(Trigger_Line_Obj));
  return hWin;
}

/**** END OF FILE ****/
