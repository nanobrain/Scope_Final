/**
  ******************************************************************************
  * @file    RTOS\Acqusition_Thread.c 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Main acquisition thread
  ******************************************************************************
  */

/*----------------------------------------------------------------------------
 *      Acquisition_Thread:  ADC communication thread
 *---------------------------------------------------------------------------*/
 
 /* Includes ------------------------------------------------------------------*/
#include "cmsis_os.h"                   // CMSIS RTOS header file
#include "main.h"
#include "Error_Handler.h"
#include "ADC_Config.h"
#include "Threads.h"
#include "Relays.h"

#ifdef _RTE_
#include "RTE_Components.h"             // Component selection
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
// Threads items
osThreadId tid_Acqusition_Thread;
osMutexId mid_Acquisition;
osThreadDef (Acqusition_Thread, TH_ACQUISITIONPRIORITY, 1, TH_ACQUISITIONSTACK);
osMutexDef (m_Acquisition);

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
int Init_Acqusition_Thread (void) {

	mid_Acquisition = osMutexCreate (osMutex (m_Acquisition));
  if (!mid_Acquisition)
    return(-2);
	
  tid_Acqusition_Thread = osThreadCreate (osThread(Acqusition_Thread), NULL);
  if (!tid_Acqusition_Thread) return(-1);
  
  return(0);
}

void Acqusition_Thread (void const *argument) {

	osSignalWait(sid_GuiInitialized,osWaitForever);
  while (1) {

		if (Buttons_GetState() == 0) // TEMPORARY
			Relay(REL_GND,TRUE);
		else
			Relay(REL_GND,FALSE);
		
		osMutexWait(mid_Acquisition,osWaitForever);
		/* Critical section */
		{
		if( ADC_Receive() != HAL_OK )
			Error_Handler(ERROR_CONVERSION);
		
		while(ADC_Is_Busy())
				osDelay(1);
		}
		/* END of critical section */
		osMutexRelease(mid_Acquisition);
    osThreadYield ();                                         // suspend thread
		
  }
}
