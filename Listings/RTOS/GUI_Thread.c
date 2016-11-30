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
#include "main.h"
#include "Relays.h"
#include "Leds.h"
#include "Threads.h"

#ifdef _RTE_
#include "RTE_Components.h"             // Component selection
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SCREENSIZEX				480
#define SCREENSIZEY				272
#define USERSPACESTARTX		GRIDENDX
#define USERSPACESTARTY		GRIDSTARTY
#define USERSPACESIZEX		USERSPACEENDX-USERSPACESTARTX
#define USERSPACESIZEY		USERSPACEENDY-USERSPACESTARTY
#define USERSPACEENDX			SCREENSIZEX
#define USERSPACEENDY			SCREENSIZEY
#define GRIDCOUNTX 				10
#define GRIDCOUNTY				8
#define GRIDUNITSIZE			34
#define GRIDSIZEX					GRIDCOUNTX*GRIDUNITSIZE
#define GRIDSIZEY					GRIDCOUNTY*GRIDUNITSIZE
#define GRIDSTARTX				0
#define GRIDSTARTY				0
#define GRIDENDX					GRIDSTARTX + GRIDSIZEX
#define GRIDENDY					GRIDSTARTY + GRIDSIZEY

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
osThreadId tid_GUIThread;
osThreadDef (GUIThread, TH_GUIPRIORITY, 1, TH_GUISTACK);

#if FAKE_WAVEFORM
	extern uint8_t g_8u_SamplesBuffer[RX_FAKEBUFFERSIZE];
#else
	extern uint8_t g_8u_SamplesBuffer[RX_BUFFERSIZE];
#endif

/* Private function prototypes -----------------------------------------------*/
static uint16_t Trigger(uint8_t Trig_SP, volatile unsigned short* Signal, uint16_t Sig_Size );
static void Draw_GraphGrid(uint16_t a_XSize, uint16_t a_YSize,uint16_t a_XStart,uint16_t a_YStart, uint16_t a_XDense, uint16_t a_YDense);
static void Demo_Run(void);
static void Display_HelloMsg(void);

/* Private functions ---------------------------------------------------------*/
int Init_GUIThread (void) {

  tid_GUIThread = osThreadCreate (osThread(GUIThread), NULL);
  if (!tid_GUIThread) return(-1);
  
  return(0);
}

void GUIThread (void const *argument) {
	GUI_MEMDEV_Handle hMem0,hMem1;

  GUI_Init();                     /* Initialize the Graphics Component */
	/* Hello message */
	Display_HelloMsg();
	/* Test run */
	Demo_Run();
	
	GUI_Clear();
	
	/* Indicate full initialization finish */
	osSignalSet(tid_Acqusition_Thread,sid_GuiInitialized);

	/* Draw GUI */
	hMem0 = GUI_MEMDEV_Create(GRIDSTARTX,GRIDSTARTY,GRIDSIZEX,GRIDSIZEY);
	hMem1 = GUI_MEMDEV_Create(USERSPACESTARTX,USERSPACESTARTY,USERSPACESIZEX,USERSPACESIZEY);
	
	GUI_SelectLayer(0);
	GUI_MEMDEV_Select(hMem1);
	GUI_MEMDEV_Clear(hMem1);
	GUI_SetColor(GUI_ORANGE);
	GUI_FillRect(USERSPACESTARTX,USERSPACESTARTY,USERSPACEENDX,USERSPACEENDY);
	GUI_SetColor(GUI_BLACK);	
	
  while (1) {
		GUI_MEMDEV_Select(hMem0);
		GUI_MEMDEV_Clear(hMem0);
		
		/* Critical section */
		osMutexWait(mid_Acquisition,osWaitForever);
		
		GUI_SetColor(GUI_WHITE);
		GUI_FillRect(GRIDSTARTX,GRIDSTARTY,GRIDENDX,GRIDENDY);
		
		Draw_GraphGrid(GRIDSIZEX,GRIDSIZEY,GRIDSTARTX,GRIDSTARTY,GRIDUNITSIZE,GRIDUNITSIZE);
		
		GUI_SetColor(GUI_BLUE);
		GUI_DrawGraph((short*)g_8u_SamplesBuffer,GRIDSIZEX,GRIDSTARTX,GRIDSTARTY/2);
	
		osMutexRelease(mid_Acquisition);
		/* END of critical section */
		
		GUI_MEMDEV_CopyToLCD(hMem1);
		GUI_MEMDEV_CopyToLCD(hMem0);
		
    //GUI_Exec();                   /* Execute all GUI jobs ... Return 0 if nothing was done. */
    //GUI_X_ExecIdle();             /* Nothing left to do for the moment ... Idle processing */
		osThreadYield ();
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

static uint16_t Trigger(uint8_t Trig_SP, volatile unsigned short* Signal, uint16_t Sig_Size )
	{

	uint16_t i=0;
	uint16_t ctr=0;
	uint8_t Treshold = 3;
	Sig_Size -=500;
	
	for(i=0;i<=Sig_Size;i++) // Find first raising edge
	{
		if((Signal[i] >= Trig_SP) && i==Treshold)
		{
			ctr=i;
			break;
		}
	}
	
	for(i=ctr;i<=Sig_Size;i++) // Find falling edge
	{
		if(Signal[i] < Trig_SP)
		{
			ctr=i;
			break;
		}
	}
	
	for(i=ctr;i<=Sig_Size;i++) // Find second raising edge
	{
		if(Signal[i] >= Trig_SP)
		{
			return i;
		}
	}
	
	return 0;
}

static void Draw_GraphGrid(uint16_t a_XSize, uint16_t a_YSize,uint16_t a_XStart,uint16_t a_YStart, uint16_t a_XDense, uint16_t a_YDense)
	{
	//
	// THIS CAN BE DONE FASTER.
	//
	uint16_t XStart,YStart;	
	uint16_t Xstep = a_XSize/a_XDense;
	uint16_t Ystep = a_YSize/a_YDense;
	uint16_t i=0;
	unsigned char prevLineStyle, prevPenSize, prevGuiColor;
	
	prevLineStyle = GUI_SetLineStyle(GUI_LS_DOT);
	prevPenSize = GUI_SetPenSize(1);
	prevGuiColor = GUI_GetColor();
	GUI_SetColor(GUI_BLACK);
	
	for(i=1;i<=Xstep;i++)
		GUI_DrawLine(a_XStart+i*a_XDense,a_YStart,a_XStart + i*a_XDense,a_YStart+a_YSize);
	
	for(i=1;i<=Ystep;i++)
		GUI_DrawLine(a_XStart,a_YStart+i*a_YDense,a_XStart+a_XSize,a_YStart+i*a_YDense);
		
	GUI_SetPenSize(4);
	GUI_DrawRect(a_XStart,a_YStart,a_XStart+a_XSize-1,a_YStart+a_YSize-1);
	
	GUI_SetPenSize(prevPenSize);
	GUI_SetColor(prevGuiColor);
	GUI_SetLineStyle(prevLineStyle);

}
