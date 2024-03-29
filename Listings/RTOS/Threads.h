/**
  ******************************************************************************
  * @file    RTOS/Threads.h
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Header for threads configuration
  ******************************************************************************
  */
  
#ifndef __THREADS_H
#define __THREADS_H

/* Heart beat thread */
#define TH_HEARTBEATPRIORITY 			osPriorityNormal
#define TH_HEARTBEATSTACK					256
int Init_Heart_Beat_Thread (void);
void Heart_Beat_Thread (void const *argument);

/* GUI thread */
#define TH_GUIPRIORITY 						osPriorityAboveNormal
#define TH_GUISTACK								2048
int Init_GUIThread (void);
void GUIThread (void const *argument);

/* Acquisiion thread */
#define TH_ACQUISITIONPRIORITY		osPriorityNormal
#define TH_ACQUISITIONSTACK				1024
int Init_Acqusition_Thread (void);
void Acqusition_Thread (void const *argument);

/* Threads IDs */
extern osThreadId tid_GUIThread;
extern osThreadId tid_Heart_Beat_Thread;
extern osThreadId tid_Acqusition_Thread;
/* Mutexes */
extern osMutexId mid_Acquisition;
/* Signals */
#define sid_GuiInitialized	0x01

#endif /* __THREADS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
