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
osThreadDef (GUIThread, osPriorityHigh, 1, 2048);
//short values[1024] = {49,46,48,48,46,49,47,47,49,46,47,48,45,48,47,46,48,46,47,47,45,48,47,46,48,46,47,48,45,47,47,46,48,46,47,48,45,47,47,45,48,47,46,48,46,47,47,45,48,47,46,51,57,62,70,72,78,85,86,93,97,98,104,104,108,113,113,118,121,121,126,126,128,132,131,135,136,137,140,140,141,144,142,146,146,146,149,148,149,151,149,152,152,151,154,153,153,155,154,155,156,154,156,156,156,158,156,157,158,156,158,158,157,160,158,158,160,157,159,160,158,160,159,160,161,159,160,161,159,161,160,160,162,160,161,162,160,161,161,160,163,156,146,140,130,125,120,111,108,103,95,94,87,83,81,74,73,70,64,65,60,57,56,51,53,51,48,51,48,48,50,47,48,48,46,49,47,47,49,46,47,48,45,48,48,46,49,47,46,48,45,47,47,45,48,47,46,48,46,47,48,45,48,47,46,48,46,47,48,45,47,47,47,48,46,46,48,45,47,47,46,48,46,47,48,51,60,66,69,77,81,84,91,92,96,102,102,108,111,112,118,119,120,124,124,127,131,130,134,135,136,139,138,141,143,142,146,146,146,148,147,149,150,149,152,151,152,154,152,154,154,153,155,155,154,157,155,156,158,155,157,158,156,159,158,158,159,157,158,159,157,160,159,158,161,159,160,160,158,161,160,159,162,160,160,162,159,161,161,160,162,161,161,162,160,151,145,135,129,123,115,112,105,99,96,89,86,83,76,76,71,67,66,61,58,57,52,54,51,49,51,49,49,49,47,49,48,47,49,47,47,49,46,48,47,46,49,47,47,48,45,47,47,46,48,47,46,48,46,47,48,45,48,47,45,48,46,46,48,45,47,47,45,48,47,47,48,45,47,47,46,48,47,46,48,45,47,47,48,57,63,66,74,78,81,88,89,94,99,100,106,109,110,115,116,118,123,123,127,129,129,133,133,136,138,137,141,141,142,145,144,146,148,146,149,149,149,152,151,151,153,151,154,154,153,156,154,155,157,154,156,157,155,158,157,157,159,157,158,159,157,159,159,158,160,159,159,160,158,160,160,158,161,160,160,162,160,160,161,159,161,161,159,162,161,161,162,160,158,150,139,134,126,119,115,107,102,99,91,89,85,79,78,72,69,68,62,61,58,54,55,51,50,51,48,50,49,47,50,48,47,49,46,48,48,46,49,47,46,49,46,47,48,45,48,47,46,49,47,47,48,45,47,48,45,48,47,46,48,45,47,47,45,48,47,46,48,46,47,48,45,48,47,46,48,46,47,48,45,47,47,46,53,58,63,71,73,79,85,87,93,96,99,105,105,109,114,114,118,121,122,127,127,128,132,132,134,137,137,141,141,141,145,143,146,147,146,149,149,149,151,150,152,152,151,154,153,152,155,153,155,156,154,156,155,156,158,157,157,158,156,158,158,157,159,158,158,160,158,158,160,158,159,160,159,161,160,160,161,159,161,161,160,162,160,161,162,160,161,161,160,163,156,146,140,130,124,119,111,107,102,95,93,87,82,81,74,71,70,64,64,154,154,155,153,156,156,155,157,156,156,158,156,157,158,156,159,158,158,160,158,159,160,157,160,159,158,161,160,160,161,159,160,161,159,162,160,160,162,160,161,162,160,161,161,161,156,145,137,131,122,117,112,104,102,96,90,88,81,78,76,70,69,66,61,61,55,54,53,49,52,50,48,50,48,48,49,46,48,48,46,49,47,47,49,46,47,48,46,49,48,46,49,46,47,48,45,48,47,46,47,46,47,48,45,46,47,45,48,47,46,48,45,47,48,45,48,47,46,48,46,46,48,45,48,47,46,49,46,47,56,60,67,73,76,83,87,89,96,96,101,106,107,112,115,115,121,121,123,127,127,130,133,133,137,137,138,141,139,142,144,144,147,147,147,150,148,150,151,150,153,152,152,154,152,154,155,153,156,156,155,157,156,156,158,155,157,158,156,159,158,158,159,157,159,159,158,160,159,159,161,159,160,160,159,161,160,160,162,160,160,161,159,161,161,160,162,161,161,160,149,141,134,125,122,115,107,105,97,93,90,83,81,78,72,71,67,62,62,57,55,54,50,52,50,49,51,48,48,49,47,49,48,47,49,46,47,48,46,48,47,46,48,46,47,48,45,47,47,46,48,47,47,48,45,47,47,45,48,47,46,48,46,46,47,45,47,47,46,48,46,47,47,45,47,47,46,48,46,46,48,45,47,51,56,65,70,73,81,83,87,93,95,99,104,105,110,113,114,119,119,122,126,126,130,131,132,136,135,138,140,139,144,143,144,146,145,147,148,147,151,150,150,153};
extern uint8_t g_8u_SamplesBuffer[2048]; // Main acquisition buffer

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
	
	hMem0 = GUI_MEMDEV_Create(GRIDSTARTX,GRIDSTARTY,GRIDSIZEX,GRIDSIZEY);
	hMem1 = GUI_MEMDEV_Create(USERSPACESTARTX,USERSPACESTARTY,USERSPACESIZEX,USERSPACESIZEY);
	
	GUI_SelectLayer(0);
	GUI_MEMDEV_Select(hMem1);
	GUI_MEMDEV_Clear(hMem1);
	GUI_SetColor(GUI_ORANGE);
	GUI_FillRect(USERSPACESTARTX,USERSPACESTARTY,USERSPACEENDX,USERSPACEENDY);
	GUI_SetColor(GUI_BLACK);
	GUI_MEMDEV_CopyToLCD(hMem1);
	
  while (1) {
		GUI_MEMDEV_Select(hMem0);
		GUI_MEMDEV_Clear(hMem0);
		
		/* Critical section */
		osMutexWait(mid_Acquisition,osWaitForever);
		
		GUI_SetColor(GUI_WHITE);
		GUI_FillRect(GRIDSTARTX,GRIDSTARTY,GRIDENDX,GRIDENDY);
		
		Draw_GraphGrid(GRIDSIZEX,GRIDSIZEY,GRIDSTARTX,GRIDSTARTY,GRIDUNITSIZE,GRIDUNITSIZE);
		
		GUI_SetColor(GUI_BLUE);
		GUI_DrawGraph((short*)g_8u_SamplesBuffer,GRIDSIZEX,GRIDSTARTX,GRIDSTARTY/2); // Address cast, coz I know that data is 8-bit aligned1
	
		osMutexRelease(mid_Acquisition);
		/* END of critical section */
		
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
