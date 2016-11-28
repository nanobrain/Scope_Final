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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
osThreadDef (GUIThread, osPriorityNormal, 1, 2048);

/* Private function prototypes -----------------------------------------------*/
static void Demo_Run(void);
static void Display_HelloMsg(void);

/* Private functions ---------------------------------------------------------*/
int Init_GUIThread (void) {

  tid_GUIThread = osThreadCreate (osThread(GUIThread), NULL);
  if (!tid_GUIThread) return(-1);
  
  return(0);
}

void GUIThread (void const *argument) {

  GUI_Init();                     /* Initialize the Graphics Component */
	/* Test run */
	Demo_Run();
	/* Hello message */
	Display_HelloMsg();

  while (1) {
    
    /* All GUI related activities might only be called from here */
		
    GUI_Exec();                   /* Execute all GUI jobs ... Return 0 if nothing was done. */
    GUI_X_ExecIdle();             /* Nothing left to do for the moment ... Idle processing */
  }
}

static void Display_HelloMsg(void)
{
	GUI_SetColor(GUI_BLUE);
	GUI_SetFont(&GUI_Font16_1);
	GUI_DispStringHCenterAt("Oscyloskop cyfrowy v1.2" , 240, 150);
	GUI_DispStringHCenterAt("Adrian Kurylak" , 240, 175);
	GUI_DispStringHCenterAt("Politechnika Wroclawska" , 240, 200);
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
