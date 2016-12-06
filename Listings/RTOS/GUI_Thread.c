/**
  ******************************************************************************
  * @file    RTOS\GUI_Thread.c 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   GUI configuration and execution
  ******************************************************************************
  */

/*----------------------------------------------------------------------------
 *      GUIThread: GUI Thread for Single-Task Execution Model
 *---------------------------------------------------------------------------*/

/************** Screen config ****************/
/*********************************************/
/********** Grid  Graph	 Status bar **********/
/********** 8x10 340x272   120x272  **********/
/*********************************************/

/* Includes ------------------------------------------------------------------*/
#include "cmsis_os.h"                   // CMSIS RTOS header file
#include "GUI.h"
#include "HEADER.h"
#include "FRAMEWIN.h"
#include "DIALOG.h"
#include "MULTIPAGE.h"
#include "main.h"
#include "Error_Handler.h"
#include "Trigger_Line.h"
#include "ADC_Config.h"
#include "VGA_Config.h"
#include "Relays.h"
#include "Leds.h"
#include "Threads.h"

#ifdef _RTE_
#include "RTE_Components.h"             // Component selection
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Windows IDs */
#define GUI_HDLG_ID_FRAMEWIN 1
#define GUI_HDLG_ID_GRAPH_WIN 2

/* Screen & windows sizes */
// Scope graph
#define GRIDLS						GUI_LS_DOT
#define GRAPHCOUNTX 			12
#define GRAPHCOUNTY				8
#define GRAPHUNITSIZE			34
#define GRAPHSIZEX				GRAPHCOUNTX*GRAPHUNITSIZE
#define GRAPHSIZEY				GRAPHCOUNTY*GRAPHUNITSIZE
#define GRAPHSTARTX				0
#define GRAPHSTARTY				0
#define GRAPHENDX					GRAPHSTARTX + GRAPHSIZEX
#define GRAPHENDY					GRAPHSTARTY + GRAPHSIZEY

// User space
#define SCREENSIZEX				480
#define SCREENSIZEY				272
#define USERSPACESTARTX		GRAPHENDX
#define USERSPACESTARTY		GRAPHSTARTY
#define USERSPACEENDX			SCREENSIZEX
#define USERSPACEENDY			SCREENSIZEY
#define USERSPACESIZEX		72//USERSPACEENDX - USERSPACESTARTX
#define USERSPACESIZEY		USERSPACEENDY - USERSPACESTARTY
#define USERSPACEPADDINGX	5
#define USERSPACEPADDINGY	5

// Small button
#define SBUTTON_WIDTH				(USERSPACESIZEX - USERSPACEPADDINGX * 3) / 2
#define SBUTTON_HEIGHT			20

// Big button
#define BBUTTON_WIDTH				USERSPACESIZEX - USERSPACEPADDINGX * 2
#define BBUTTON_HEIGHT			20

// Text
#define TEXT_HEIGHT					15

/* GUI elements IDs */
#define GUI_ID_MAIN_SCOPE								GUI_ID_USER
#define GUI_ID_MAIN_SCOPE_GRAPH					GUI_ID_USER + 1
#define GUI_ID_USERSPACE								GUI_ID_USER + 2
#define GUI_ID_USERSPACE_AC							GUI_ID_USER + 3
#define GUI_ID_USERSPACE_DC							GUI_ID_USER + 4
#define GUI_ID_USERSPACE_GND						GUI_ID_USER + 5
#define GUI_ID_USERSPACE_IN							GUI_ID_USER + 6
#define GUI_ID_USERSPACE_ATT1						GUI_ID_USER + 7
#define GUI_ID_USERSPACE_ATT10					GUI_ID_USER + 8
#define GUI_ID_USERSPACE_GAINUP					GUI_ID_USER + 9
#define GUI_ID_USERSPACE_GAINDOWN				GUI_ID_USER + 10

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint16_t _TriggerPoint=127;
static GRAPH_DATA_Handle _hGraphData;

// Threads items
osThreadId tid_GUIThread;
osThreadDef (GUIThread, TH_GUIPRIORITY, 1, TH_GUISTACK);

// Main window
static const GUI_WIDGET_CREATE_INFO _aMainWindowCreate[] = {
  {	WINDOW_CreateIndirect, 0, GUI_ID_MAIN_SCOPE, 0, 0, SCREENSIZEX, SCREENSIZEY },
	{	GRAPH_CreateIndirect, 0, GUI_ID_MAIN_SCOPE_GRAPH, GRAPHSTARTX, GRAPHSTARTY, GRAPHSIZEX, GRAPHSIZEY },
};

static const GUI_WIDGET_CREATE_INFO _aUserSpaceWindowCreate[] = {
	{ WINDOW_CreateIndirect, "User_Space", GUI_ID_USERSPACE, USERSPACESTARTX, USERSPACESTARTY, USERSPACESIZEX, USERSPACESIZEY },
	};

// Colors to be used in graph window
typedef struct GUI_Colors{
	GUI_COLOR BackGround;
	GUI_COLOR GraphBackGround;
	GUI_COLOR Grid;
	GUI_COLOR Label;
	GUI_COLOR Border;
	GUI_COLOR Frame;
	GUI_COLOR Waveform;
}GUI_Colors;
static GUI_Colors _GuiColors;

/* Private function prototypes -----------------------------------------------*/
static void _GraphCallback(WM_MESSAGE * pMsg);
static void _UserSpaceCallback(WM_MESSAGE * pMsg);
static void _cbTriggerLine(WM_MESSAGE * pMsg);

int Init_GUIThread (void) {

  tid_GUIThread = osThreadCreate (osThread(GUIThread), NULL);
  if (!tid_GUIThread) return(-1);
  
  return(0);
}

void GUIThread (void const *argument) {
	WM_HWIN hDlg,hTriggerLine,hGraph,hUserSpace;
	uint16_t i,triggerVal;
	char acExtraBytes[20] = "";
	
	/* Define GUI colors */
	_GuiColors.BackGround 			= GUI_WHITE;
	_GuiColors.Border 					= GUI_DARKBLUE;
	_GuiColors.Frame 						= GUI_BLACK;
	_GuiColors.GraphBackGround 	= GUI_BLACK;
	_GuiColors.Grid 						= GUI_GRAY;
	_GuiColors.Label 						= GUI_WHITE;
	_GuiColors.Waveform					= GUI_GREEN;
	
	WM_SetCreateFlags(WM_CF_MEMDEV);
	hDlg = GUI_CreateDialogBox(_aMainWindowCreate,GUI_COUNTOF(_aMainWindowCreate),_GraphCallback,0,0,0);
	//hGraph = WM_GetDialogItem(hDlg, GUI_ID_MAIN_SCOPE_GRAPH);
	hUserSpace = GUI_CreateDialogBox(_aUserSpaceWindowCreate, GUI_COUNTOF(_aUserSpaceWindowCreate),_UserSpaceCallback,0,0,0);
	hTriggerLine = Trigger_Line_Create(GRAPHSTARTX,(GRAPHSIZEY-GRAPHUNITSIZE)/2,GRAPHSIZEX,GRAPHUNITSIZE, NULL, WM_CF_SHOW | WM_CF_MOTION_Y, NULL, _cbTriggerLine, strlen(acExtraBytes));
	Trigger_Line_SetUserData(hTriggerLine, acExtraBytes,strlen(acExtraBytes));
	WM_MOTION_Enable(1);
	WM_SetHasTrans(hTriggerLine);
	
	/* Indicate full initialization finish */
	osSignalSet(tid_Acqusition_Thread,sid_GuiInitialized);
	
	GUI_Clear();
	GUI_CURSOR_Show();
	
  while (1) {
		osMutexWait(mid_Acquisition,osWaitForever);
		/* Critical section */
		{
			GRAPH_DATA_YT_Clear(_hGraphData);
			
			triggerVal = Trigger(_TriggerPoint,g_d8_RxBufferMain1,RX_BUFFERCOUNT);
			for(i=triggerVal;i<GRAPHSIZEX+triggerVal;i++)
			{
				GRAPH_DATA_YT_AddValue(_hGraphData,(short)(255 - g_d8_RxBufferMain1[i].payload + 127));
			}
		}
		/* END of critical section */
		osMutexRelease(mid_Acquisition);
		GUI_Exec();
		GUI_X_ExecIdle();
  }
}

static void _GraphCallback(WM_MESSAGE * pMsg) {
  WM_HWIN		hDlg;
  WM_HWIN		hItem;
	WM_HWIN		hScaleV;
	WM_HWIN		hScaleH;

  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
		case WM_INIT_DIALOG:
		{
			hItem = WM_GetDialogItem(hDlg, GUI_ID_MAIN_SCOPE_GRAPH);
			_hGraphData = GRAPH_DATA_YT_Create(_GuiColors.Waveform, SCREENSIZEX, 0, 0);
			GRAPH_AttachData(hItem,_hGraphData);
			GRAPH_DATA_YT_SetAlign(_hGraphData,GRAPH_ALIGN_LEFT);
			//GRAPH_SetVSizeX(hItem,RX_BUFFERCOUNT-50);
			GRAPH_SetBorder(hItem,1,1,1,1);
			GRAPH_SetGridDistX(hItem, GRAPHUNITSIZE);
			GRAPH_SetGridDistY(hItem, GRAPHUNITSIZE);
			GRAPH_SetLineStyleH(hItem,GRIDLS);
			GRAPH_SetLineStyleV(hItem,GRIDLS);
			GRAPH_SetGridVis(hItem, 1);
			GRAPH_SetGridFixedX(hItem, 1);
			GRAPH_SetColor(hItem,_GuiColors.BackGround,GRAPH_CI_BK);
			GRAPH_SetColor(hItem,_GuiColors.Border,GRAPH_CI_BORDER);
			GRAPH_SetColor(hItem,_GuiColors.Frame,GRAPH_CI_FRAME);
			GRAPH_SetColor(hItem,_GuiColors.Grid,GRAPH_CI_GRID);
		} break;
		/*case WM_NOTIFY_PARENT:
		{
			if (pMsg->Data.v == WM_NOTIFICATION_RELEASED)
			{
				int Id = WM_GetId(pMsg->hWinSrc);
				switch(Id)
				{
					//case GUI_ID_MULTIPAGE_BT_ACD:
					//	BUTTON_GetText( )
					//break;
				}
			}
		}break;*/
		default:
    WM_DefaultProc(pMsg);
  }
}

static void _cbTriggerLine(WM_MESSAGE * pMsg) {
  char acText[20] = { 0 };

  switch (pMsg->MsgId) {
  case WM_PAINT:
	{
    Trigger_Line_Callback(pMsg);
    Trigger_Line_GetUserData(pMsg->hWin, acText, sizeof(acText));
		_TriggerPoint = WM_GetWindowOrgY(pMsg->hWin)+127;
	}break;
  default:
    Trigger_Line_Callback(pMsg);
  }
}

static void _UserSpaceCallback(WM_MESSAGE * pMsg)
{
  WM_HWIN							hDlg;
	int 	          		NCode;
  int									Id;
	WM_HWIN							hButton;
	uint8_t 						x0,y0,offset;

  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
		case WM_INIT_DIALOG:
		{
			// ACDC text and buttons
			offset=0;
			x0 = USERSPACEPADDINGX;
			y0 = USERSPACEPADDINGY;
			TEXT_CreateEx(x0,y0,USERSPACESIZEX,TEXT_HEIGHT,hDlg,WM_CF_SHOW | TEXT_CF_VCENTER,0,0,"Coupling");
			x0 = x0;
			y0 = USERSPACEPADDINGY*2+TEXT_HEIGHT;
			hButton = BUTTON_CreateEx(x0,y0,SBUTTON_WIDTH,SBUTTON_HEIGHT,hDlg, WM_CF_SHOW, 0, GUI_ID_USERSPACE_AC);
			BUTTON_SetText(hButton,"AC");
			WM_EnableWindow(hButton);
			x0 = USERSPACEPADDINGX*2+SBUTTON_WIDTH;
			y0 = y0;
			hButton = BUTTON_CreateEx(x0,y0,SBUTTON_WIDTH,SBUTTON_HEIGHT,hDlg, WM_CF_SHOW, 0, GUI_ID_USERSPACE_DC);
			BUTTON_SetText(hButton,"DC");
			WM_DisableWindow(hButton);
			
			// GND/IN text and buttons
			offset+= TEXT_HEIGHT + SBUTTON_HEIGHT + 3*USERSPACEPADDINGY;
			x0 = USERSPACEPADDINGX;
			y0 = USERSPACEPADDINGY + offset;
			TEXT_CreateEx(x0,y0,USERSPACESIZEX,TEXT_HEIGHT,hDlg,WM_CF_SHOW | TEXT_CF_VCENTER,0,0,"Input");
			x0 = x0;
			y0 = USERSPACEPADDINGY*2+TEXT_HEIGHT + offset;
			hButton = BUTTON_CreateEx(x0,y0,SBUTTON_WIDTH,SBUTTON_HEIGHT,hDlg, WM_CF_SHOW, 0, GUI_ID_USERSPACE_GND);
			BUTTON_SetText(hButton,"GND");
			WM_DisableWindow(hButton);
			x0 = USERSPACEPADDINGX*2+SBUTTON_WIDTH;
			y0 = y0;
			hButton = BUTTON_CreateEx(x0,y0,SBUTTON_WIDTH,SBUTTON_HEIGHT,hDlg, WM_CF_SHOW, 0, GUI_ID_USERSPACE_IN);
			BUTTON_SetText(hButton,"BNC");
			WM_EnableWindow(hButton);
			
			// GND/IN text and buttons
			offset+= TEXT_HEIGHT + SBUTTON_HEIGHT + 3*USERSPACEPADDINGY;
			x0 = USERSPACEPADDINGX;
			y0 = USERSPACEPADDINGY + offset;
			TEXT_CreateEx(x0,y0,USERSPACESIZEX,TEXT_HEIGHT,hDlg,WM_CF_SHOW | TEXT_CF_VCENTER,0,0,"Attenuation");
			x0 = x0;
			y0 = USERSPACEPADDINGY*2+TEXT_HEIGHT + offset;
			hButton = BUTTON_CreateEx(x0,y0,SBUTTON_WIDTH,SBUTTON_HEIGHT,hDlg, WM_CF_SHOW, 0, GUI_ID_USERSPACE_ATT1);
			BUTTON_SetText(hButton,"1:1");
			WM_EnableWindow(hButton);
			x0 = USERSPACEPADDINGX*2+SBUTTON_WIDTH;
			y0 = y0;
			hButton = BUTTON_CreateEx(x0,y0,SBUTTON_WIDTH,SBUTTON_HEIGHT,hDlg, WM_CF_SHOW, 0, GUI_ID_USERSPACE_ATT10);
			BUTTON_SetText(hButton,"1:10");
			WM_DisableWindow(hButton);
			
			// Gain Up/Down buttons
			offset+= TEXT_HEIGHT + SBUTTON_HEIGHT + 3*USERSPACEPADDINGY;
			x0 = USERSPACEPADDINGX;
			y0 = USERSPACEPADDINGY + offset;
			TEXT_CreateEx(x0,y0,USERSPACESIZEX,TEXT_HEIGHT,hDlg,WM_CF_SHOW | TEXT_CF_VCENTER,0,0,"Gain");
			x0 = x0;
			y0 += USERSPACEPADDINGY+TEXT_HEIGHT;
			hButton = BUTTON_CreateEx(x0,y0,BBUTTON_WIDTH,2*BBUTTON_HEIGHT,hDlg, WM_CF_SHOW, 0, GUI_ID_USERSPACE_GAINUP);
			BUTTON_SetText(hButton,"Up");
			x0 = x0;
			y0 += 2*BBUTTON_HEIGHT + USERSPACEPADDINGY;
			hButton = BUTTON_CreateEx(x0,y0,BBUTTON_WIDTH,2*BBUTTON_HEIGHT,hDlg, WM_CF_SHOW, 0, GUI_ID_USERSPACE_GAINDOWN);
			BUTTON_SetText(hButton,"Down");
			
		}break;
		case WM_NOTIFY_PARENT:
		{
			Id = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(NCode)
			{
				case WM_NOTIFICATION_CLICKED: // Intentionaly it is not WM_NOTIFICATION_RELEASED
				{
					GUI_Delay(10);
					if( Id == GUI_ID_USERSPACE_AC )
					{
						hButton = WM_GetDialogItem(hDlg,GUI_ID_USERSPACE_AC);
						WM_DisableWindow(hButton);
						hButton = WM_GetDialogItem(hDlg,GUI_ID_USERSPACE_DC);
						WM_EnableWindow(hButton);
						Relay_ACDC(AC);
					}
					if( Id == GUI_ID_USERSPACE_DC )
					{
						hButton = WM_GetDialogItem(hDlg,GUI_ID_USERSPACE_AC);
						WM_EnableWindow(hButton);
						hButton = WM_GetDialogItem(hDlg,GUI_ID_USERSPACE_DC);
						WM_DisableWindow(hButton);
						Relay_ACDC(DC);
					}
					if( Id == GUI_ID_USERSPACE_GND )
					{
						hButton = WM_GetDialogItem(hDlg,GUI_ID_USERSPACE_GND);
						WM_DisableWindow(hButton);
						hButton = WM_GetDialogItem(hDlg,GUI_ID_USERSPACE_IN);
						WM_EnableWindow(hButton);
						Relay_Input(GND);
					}
					if( Id == GUI_ID_USERSPACE_IN )
					{
						hButton = WM_GetDialogItem(hDlg,GUI_ID_USERSPACE_GND);
						WM_EnableWindow(hButton);
						hButton = WM_GetDialogItem(hDlg,GUI_ID_USERSPACE_IN);
						WM_DisableWindow(hButton);
						Relay_Input(INPUT);
					}
					if( Id == GUI_ID_USERSPACE_ATT1 )
					{
						hButton = WM_GetDialogItem(hDlg,GUI_ID_USERSPACE_ATT1);
						WM_DisableWindow(hButton);
						hButton = WM_GetDialogItem(hDlg,GUI_ID_USERSPACE_ATT10);
						WM_EnableWindow(hButton);
						Relay_Attenuator(DB0);
					}
					if( Id == GUI_ID_USERSPACE_ATT10 )
					{
						hButton = WM_GetDialogItem(hDlg,GUI_ID_USERSPACE_ATT1);
						WM_EnableWindow(hButton);
						hButton = WM_GetDialogItem(hDlg,GUI_ID_USERSPACE_ATT10);
						WM_DisableWindow(hButton);
						Relay_Attenuator(DB20);
					}
					if( Id == GUI_ID_USERSPACE_GAINUP )
					{
						if( VGA_Step_Gain(STEP_UP) != 0 )
							WM_DisableWindow(WM_GetDialogItem(hDlg,GUI_ID_USERSPACE_GAINUP));
						else
						{
							WM_EnableWindow(WM_GetDialogItem(hDlg,GUI_ID_USERSPACE_GAINDOWN));
							WM_EnableWindow(WM_GetDialogItem(hDlg,GUI_ID_USERSPACE_GAINUP));
						}
					}
					if( Id == GUI_ID_USERSPACE_GAINDOWN )
					{
						if( VGA_Step_Gain(STEP_DOWN) )
							WM_DisableWindow(WM_GetDialogItem(hDlg,GUI_ID_USERSPACE_GAINDOWN));
						else
						{
							WM_EnableWindow(WM_GetDialogItem(hDlg,GUI_ID_USERSPACE_GAINDOWN));
							WM_EnableWindow(WM_GetDialogItem(hDlg,GUI_ID_USERSPACE_GAINUP));
						}
					}
				}break;
		}break;
		default:
    WM_DefaultProc(pMsg);
  }
}
}
