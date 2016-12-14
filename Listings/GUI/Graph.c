/**
  ******************************************************************************
  * @file    GUI/Graph.c 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Custom widget for trigger line
  ******************************************************************************
  */
#include "cmsis_os.h"                   // CMSIS RTOS header file
#include "main.h"
#include "GUI.h"
#include "FRAMEWIN.h"
#include "DIALOG.h"
#include "Scope_GUI.h"
#include "Trigger_Line.h"

//extern GUI_Colors g_GuiColors;
//extern uint16_t g_TriggerPoint;
//extern GRAPH_DATA_Handle g_hGraphData;

// Colors to be used in graph window
GUI_Colors g_GuiColors;
uint16_t g_TriggerPoint=128;
GRAPH_DATA_Handle g_hGraphData;

void createGraphWindow(WM_HWIN a_hDlg);

void GraphCallback(WM_MESSAGE * pMsg) {
  WM_HWIN		hDlg;

  hDlg = pMsg->hWin;
	
  switch (pMsg->MsgId) {
		
		case WM_INIT_DIALOG:
		{
			/* Define GUI colors */
			g_GuiColors.BackGround 				= GUI_WHITE;
			g_GuiColors.Border 						= GUI_DARKBLUE;
			g_GuiColors.Frame 						= GUI_BLACK;
			g_GuiColors.GraphBackGround 	= GUI_BLACK;
			g_GuiColors.Grid 							= GUI_GRAY;
			g_GuiColors.Label 						= GUI_WHITE;
			g_GuiColors.Waveform					= GUI_GREEN;
			createGraphWindow(hDlg);
		} break;
		
		default:
    WM_DefaultProc(pMsg);
		
  }
}

void TriggerLineCallback(WM_MESSAGE * pMsg) {

  switch (pMsg->MsgId) {
  case WM_PAINT:
	{
    Trigger_Line_Callback(pMsg);
		g_TriggerPoint = WM_GetWindowOrgY(pMsg->hWin)+128;
	}break;
  default:
    Trigger_Line_Callback(pMsg);
  }
}

void createGraphWindow(WM_HWIN a_hDlg)
{
	WM_HWIN hItem;
	
	/* Configure window */
	hItem = WM_GetDialogItem(a_hDlg, GUI_ID_MAIN_SCOPE_GRAPH);
	g_hGraphData = GRAPH_DATA_YT_Create(g_GuiColors.Waveform, SCREENSIZEX, 0, 0);
	GRAPH_AttachData(hItem,g_hGraphData);
	GRAPH_DATA_YT_SetAlign(g_hGraphData,GRAPH_ALIGN_LEFT);
	GRAPH_SetBorder(hItem,1,1,1,1);
	GRAPH_SetGridDistX(hItem, GRAPHUNITSIZE);
	GRAPH_SetGridDistY(hItem, GRAPHUNITSIZE);
	GRAPH_SetLineStyleH(hItem,GRIDLS);
	GRAPH_SetLineStyleV(hItem,GRIDLS);
	GRAPH_SetGridVis(hItem, 1);
	GRAPH_SetGridFixedX(hItem, 1);
	GRAPH_SetColor(hItem,g_GuiColors.BackGround,GRAPH_CI_BK);
	GRAPH_SetColor(hItem,g_GuiColors.Border,GRAPH_CI_BORDER);
	GRAPH_SetColor(hItem,g_GuiColors.Frame,GRAPH_CI_FRAME);
	GRAPH_SetColor(hItem,g_GuiColors.Grid,GRAPH_CI_GRID);
	GUI_SetColor(g_GuiColors.Border);
	GUI_DrawRect(GRAPHSTARTX+1,GRAPHSTARTY+1,GRAPHENDX-1,GRAPHENDY-1);
}

/**** END OF FILE ****/
