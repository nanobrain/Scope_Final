/**
  ******************************************************************************
  * @file    RTOS\Heart_Beat_Thread.c 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Simple status indication thread
  ******************************************************************************
  */

/*----------------------------------------------------------------------------
 *      Heart_Beat_Thread:  Status thread
 *---------------------------------------------------------------------------*/
 
/* Includes ------------------------------------------------------------------*/
#include "cmsis_os.h"                   // CMSIS RTOS header file
#include "main.h"
#include "Leds.h"

#ifdef _RTE_
#include "RTE_Components.h"             // Component selection
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Heart_Beat_Thread (void const *argument);                 	// thread function
osThreadId tid_Heart_Beat_Thread;                              	// thread id
osThreadDef (Heart_Beat_Thread, osPriorityIdle, 1, 64);       	// thread object

/* Private functions ---------------------------------------------------------*/
int Init_Heart_Beat_Thread (void) {

  tid_Heart_Beat_Thread = osThreadCreate (osThread(Heart_Beat_Thread), NULL);
  if (!tid_Heart_Beat_Thread) return(-1);
  
  return(0);
}

void Heart_Beat_Thread (void const *argument) {

  while (1) {
    Led(LEDGREEN,FALSE);
		osDelay(1900);
		Led(LEDGREEN,TRUE);
		osDelay(100);
    osThreadYield ();                                           // suspend thread
  }
}
