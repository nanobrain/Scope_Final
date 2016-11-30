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
osThreadId tid_Acqusition_Thread;
osMutexId mid_Acquisition;
osThreadDef (Acqusition_Thread, TH_ACQUISITIONPRIORITY, 1, TH_ACQUISITIONSTACK);
osMutexDef (m_Acquisition);

#if FAKE_WAVEFORM
	extern uint8_t g_8u_SamplesBuffer[RX_FAKEBUFFERSIZE];
#else
	extern uint8_t g_8u_SamplesBuffer[RX_BUFFERSIZE];
#endif

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
		/* Critical section */
		osMutexWait(mid_Acquisition,osWaitForever);

		if (Buttons_GetState() == 0)
			Relay(REL_GND,FALSE);
		else
			Relay(REL_GND,TRUE);
			
		#if FAKE_WAVEFORM == 0
		
		if( ADC_Receive() != HAL_OK )
			Error_Handler(ERROR_CONVERSION);
		
		while(ADC_Is_Received())
			osDelay(1);
		
		#endif
		
		osMutexRelease(mid_Acquisition); 	// ATT: Mutex can be released here ONLY if ADC_Receive is blocking!
																			// Mutex cannot be released until acquisition finish!
		/* END of critical section */
    osThreadYield ();                                         // suspend thread
		
  }
}
