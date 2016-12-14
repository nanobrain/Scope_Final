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
 
#include "cmsis_os.h"                   // CMSIS RTOS header file
#include "main.h"
#include "GUI.h"
#include "FRAMEWIN.h"
#include "DIALOG.h"
#include "MULTIPAGE.h"
#include "Scope_GUI.h"
#include "Trigger_Line.h"
#include "Error_Handler.h"
#include "Leds.h"
#include "Threads.h"

#ifdef _RTE_
#include "RTE_Components.h"             // Component selection
#endif

// Threads items
osThreadId tid_GUIThread;
osThreadDef (GUIThread, TH_GUIPRIORITY, 1, TH_GUISTACK);

// GRAPH data
extern GRAPH_DATA_Handle g_hGraphData;
extern uint16_t g_TriggerPoint;

// Main window
static const GUI_WIDGET_CREATE_INFO _aMainWindowCreate[] = {
  {	WINDOW_CreateIndirect, 0, GUI_ID_MAIN_SCOPE, 0, 0, SCREENSIZEX, SCREENSIZEY },
	{	GRAPH_CreateIndirect, 0, GUI_ID_MAIN_SCOPE_GRAPH, GRAPHSTARTX, GRAPHSTARTY, GRAPHSIZEX, GRAPHSIZEY },
};

// Userspace window
static const GUI_WIDGET_CREATE_INFO _aUserSpaceWindowCreate[] = {
	{ WINDOW_CreateIndirect, "User_Space", GUI_ID_USERSPACE, USERSPACESTARTX, USERSPACESTARTY, USERSPACESIZEX, USERSPACESIZEY },
};

int Init_GUIThread (void) {

  tid_GUIThread = osThreadCreate (osThread(GUIThread), NULL);
  if (!tid_GUIThread) return(-1);
  
  return(0);
}

void GUIThread (void const *argument) {
	WM_HWIN 			hTriggerLine;
	uint16_t 			i, triggerVal;
	
	WM_SetCreateFlags(WM_CF_MEMDEV);
	/* Create graph window */
	GUI_CreateDialogBox(_aMainWindowCreate,GUI_COUNTOF(_aMainWindowCreate),GraphCallback,0,0,0);
	/* Create userspace window */
	GUI_CreateDialogBox(_aUserSpaceWindowCreate, GUI_COUNTOF(_aUserSpaceWindowCreate),UserSpaceCallback,0,0,0);
	/* Create Trigger line window */
	hTriggerLine = Trigger_Line_Create(GRAPHSTARTX,(GRAPHSIZEY-GRAPHUNITSIZE)/2,GRAPHSIZEX,GRAPHUNITSIZE, NULL, WM_CF_SHOW | WM_CF_MOTION_Y, NULL, TriggerLineCallback, NULL);
	WM_MOTION_Enable(1);
	WM_SetHasTrans(hTriggerLine);
	
	/* Indicate full initialization finish */
	osSignalSet(tid_Acqusition_Thread,sid_GuiInitialized);
	
	GUI_Clear();
	
  while (1) {
		osMutexWait(mid_Acquisition,osWaitForever);
		/* Critical section */
		{
			GRAPH_DATA_YT_Clear(g_hGraphData);
			
			triggerVal = Trigger(g_TriggerPoint, g_d8_RxBufferMain1, RX_BUFFERCOUNT, RX_BUFFERCOUNT/2-200, GRAPHSIZEX);
			
			for(i=triggerVal;i<GRAPHSIZEX+triggerVal;i++)
			{
				GRAPH_DATA_YT_AddValue(g_hGraphData,(short)(255 - g_d8_RxBufferMain1[i].payload + 127));
			}
		}
		/* END of critical section */
		osMutexRelease(mid_Acquisition);
		GUI_Exec();
		GUI_X_ExecIdle();
  }
}

/**** END OF FILE ****/
