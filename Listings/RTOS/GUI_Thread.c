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
#include "main.h"
#include "Error_Handler.h"
#include "ADC_Config.h"
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
#define SCREENSIZEX				480
#define SCREENSIZEY				272
#define USERSPACESTARTX		GRAPHENDX
#define USERSPACESTARTY		GRAPHSTARTY
#define USERSPACESIZEX		USERSPACEENDX-USERSPACESTARTX
#define USERSPACESIZEY		USERSPACEENDY-USERSPACESTARTY
#define USERSPACEENDX			SCREENSIZEX
#define USERSPACEENDY			SCREENSIZEY
#define GRIDLS						GUI_LS_DOT
#define GRAPHCOUNTX 			14
#define GRAPHCOUNTY				8
#define GRAPHUNITSIZE			34
#define GRAPHSIZEX				GRAPHCOUNTX*GRAPHUNITSIZE
#define GRAPHSIZEY				GRAPHCOUNTY*GRAPHUNITSIZE
#define GRAPHSTARTX				GRAPHUNITSIZE
#define GRAPHSTARTY				0
#define GRAPHENDX					GRAPHSTARTX + GRAPHSIZEX
#define GRAPHENDY					GRAPHSTARTY + GRAPHSIZEY

#define GUI_ID_SCOPE_GRAPH	GUI_ID_USER + 1
#define GUI_ID_AUTOTRIGGER	GUI_ID_USER + 2

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint16_t _TriggerPoint=127;
//
// Graph data handle
//
static GRAPH_DATA_Handle _hGraphData;
//
// Colors to be used in graph window
//
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
//
// Dialog resource
//
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  {	WINDOW_CreateIndirect,		"Background_Window",		0,									0,		0,		SCREENSIZEX,	SCREENSIZEY		},
	{	GRAPH_CreateIndirect,			0,											GUI_ID_SCOPE_GRAPH,	0,		0,		GRAPHSIZEX,		GRAPHSIZEY		},
	{	BUTTON_CreateIndirect,		"Auto-Trigger",					GUI_ID_AUTOTRIGGER,	400,	250,	80,						22						}
};

// Threads items
osThreadId tid_GUIThread;
osThreadDef (GUIThread, TH_GUIPRIORITY, 1, TH_GUISTACK);

/* Private function prototypes -----------------------------------------------*/
static void _cbCallback(WM_MESSAGE * pMsg);
static void Demo_Run(void);
static void Display_HelloMsg(void);

int Init_GUIThread (void) {

  tid_GUIThread = osThreadCreate (osThread(GUIThread), NULL);
  if (!tid_GUIThread) return(-1);
  
  return(0);
}

void GUIThread (void const *argument) {
	WM_HWIN hDlg;
	WM_HWIN hGraph;
	uint16_t i;
	GUI_MEMDEV_Handle hMem0,hMem1;
	GUI_RECT Client_Rect;

  GUI_Init();                     /* Initialize the Graphics Component */
	/* Hello message */
	Display_HelloMsg();
	/* Test run */
	//Demo_Run();
	/* Relays in safe position */
	Relays_Default();
	
	/* Define GUI colors */
	_GuiColors.BackGround 			= GUI_WHITE;
	_GuiColors.Border 					= GUI_DARKBLUE;
	_GuiColors.Frame 						= GUI_BLACK;
	_GuiColors.GraphBackGround 	= GUI_BLACK;
	_GuiColors.Grid 						= GUI_GRAY;
	_GuiColors.Label 						= GUI_WHITE;
	_GuiColors.Waveform					= GUI_GREEN;
	
	WM_SetCreateFlags(WM_CF_MEMDEV);
	hDlg = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbCallback, 0, 0, 0);
	hGraph = WM_GetDialogItem(hDlg, GUI_ID_SCOPE_GRAPH);
	//WM_EnableMemdev(hGraph);
	WM_GetClientRectEx(hDlg,&Client_Rect);
	
	/* Indicate full initialization finish */
	osSignalSet(tid_Acqusition_Thread,sid_GuiInitialized);
	
	GUI_Clear();
	GUI_CURSOR_Show();
	
  while (1) {
		osMutexWait(mid_Acquisition,osWaitForever);
		/* Critical section */
		{
			GRAPH_DATA_YT_Clear(_hGraphData);
			
			i = Trigger(50,g_d8_SamplesBuffer,BUFFERSIZE(g_d8_SamplesBuffer));
			for(;i<(GRAPHSIZEX);i++)
			{
				GRAPH_DATA_YT_AddValue(_hGraphData,(uint8_t)(g_d8_SamplesBuffer[i].payload));
			}
		}
		/* END of critical section */
		osMutexRelease(mid_Acquisition);
		GUI_Exec();
		GUI_X_ExecIdle();
  }
}

static void _cbCallback(WM_MESSAGE * pMsg) {
  unsigned i;
  WM_HWIN  hDlg;
  WM_HWIN  hItem;

  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
			  case WM_INIT_DIALOG:
						hItem = WM_GetDialogItem(hDlg, GUI_ID_SCOPE_GRAPH);
						_hGraphData = GRAPH_DATA_YT_Create(_GuiColors.Waveform, SCREENSIZEX, 0, 0);
						GRAPH_AttachData(hItem,_hGraphData);
						GRAPH_SetVSizeX(hItem,RX_BUFFERCOUNT-50);
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
						//GRAPH_SetUserDraw(hItem, _UserDraw);
						//
						// Create and add vertical scale
						//
				/*
						_hScaleV = GRAPH_SCALE_Create( 35, GUI_TA_RIGHT, GRAPH_SCALE_CF_VERTICAL, 25);
						GRAPH_SCALE_SetTextColor(_hScaleV, GUI_YELLOW);
						GRAPH_AttachScale(hItem, _hScaleV);
				*/
						//
						// Create and add horizontal scale
						//
				/*
						_hScaleH = GRAPH_SCALE_Create(155, GUI_TA_HCENTER, GRAPH_SCALE_CF_HORIZONTAL, 50);
						GRAPH_SCALE_SetTextColor(_hScaleH, GUI_DARKGREEN);
						GRAPH_AttachScale(hItem, _hScaleH);
				*/
				break;
				case WM_TOUCH:
						
				break;
				case WM_TIMER:
						
				break;
				case WM_NOTIFY_PARENT:
					if (pMsg->Data.v == WM_NOTIFICATION_RELEASED)
					{
						int Id = WM_GetId(pMsg->hWinSrc);
						switch(Id)
						{
							case GUI_ID_AUTOTRIGGER:
								Auto_Trigger();
							break;
						}
					}
  default:
    WM_DefaultProc(pMsg);
  }
}
	
static void Display_HelloMsg(void)
{
	GUI_SetColor(GUI_BLUE);
	GUI_SetFont(&GUI_Font24_1);
	GUI_DispStringHCenterAt("Oscyloskop cyfrowy v1.2" , 240, 50);
	GUI_DispStringHCenterAt("Adrian Kurylak" , 240, 75);
	GUI_DispStringHCenterAt("Politechnika Wroclawska" , 240, 100);
	GUI_SetColor(GUI_GREEN);
	GUI_DispStringHCenterAt("Wcisnij przycisk" , 240, 175);
	GUI_SetBkColor(GUI_WHITE);
}

static void Demo_Run(void)
{
	while(Buttons_GetState() == 0){} // Start on button press
	HAL_Delay(50);
	
	Leds_All_Off();
	HAL_Delay(100);
	Relay(REL_GND,TRUE);
	Led(LEDRED1,1);
	HAL_Delay(100);
	Relay(REL_ATT,TRUE);
	Led(LEDRED2,1);
	HAL_Delay(100);
	Relay(REL_ACDC,TRUE);
	Led(LEDRED3,1);
	HAL_Delay(100);
	Relay(REL_GND,FALSE);
	Led(LEDBLUE,1);
	HAL_Delay(100);
	Relay(REL_ATT,FALSE);
	Led(LEDGREEN,1);
	HAL_Delay(100);
	Relay(REL_ACDC,FALSE);
	Leds_All_Off();
}
