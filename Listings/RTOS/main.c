 /**
  ******************************************************************************
  * @file    Templates/Src/main.c 
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    22-April-2016 
  * @brief   STM32F7xx HAL API Template project (with modifications from ARM)
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/*
 * This file contains modifications by ARM to provide it as User Code Template
 * within the STM32 Device Family Pack.
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Error_Handler.h"
#include "Leds.h"
#include "Relays.h"
#include "GUI.h"
#include "Board_Touch.h"                // ::Board Support:Touchscreen
#include "stm32746g_discovery_sdram.h"  // Keil.STM32F746G-Discovery::Board Support:Drivers:SDRAM

#ifdef _RTE_
#include "RTE_Components.h"             // Component selection
#endif
#ifdef RTE_CMSIS_RTOS                   // when RTE component CMSIS RTOS is used
#include "cmsis_os.h"                   // CMSIS RTOS header file
#endif

#ifdef RTE_CMSIS_RTOS_RTX
extern uint32_t os_time;

uint32_t HAL_GetTick(void) { 
  return os_time; 
}
#endif

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern SPI_HandleTypeDef g_hspi;
uint8_t g_aRxBuffer[20];
uint8_t g_aTxBuffer[20]="DUMMY";
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);
static void System_Init(void);
static void Demo_Run(void);
extern HAL_StatusTypeDef Init_spi(void);
extern HAL_StatusTypeDef DAC_Init(void);
extern HAL_StatusTypeDef DAC_DeInit(void);
extern HAL_StatusTypeDef VGA_Init(void);
extern HAL_StatusTypeDef VGA_DeInit(void);
extern HAL_StatusTypeDef ADC_Init(void);
extern HAL_StatusTypeDef ADC_DeInit(void);
extern HAL_StatusTypeDef ADC_Receive(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* This project template calls firstly two functions in order to configure MPU feature 
     and to enable the CPU Cache, respectively MPU_Config() and CPU_CACHE_Enable().
     These functions are provided as template implementation that User may integrate 
     in his application, to enhance the performance in case of use of AXI interface 
     with several masters. */ 
  
	/* Locals */
	osThreadId tid_Main;
	osStatus os_status;
	
  /* Configure the MPU attributes as Write Through */
  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

#ifdef RTE_CMSIS_RTOS                   // when using CMSIS RTOS
  osKernelInitialize();                 // initialize CMSIS-RTOS
#endif

  /* STM32F7xx HAL library initialization:
       - Configure the Flash ART accelerator on ITCM interface
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  if( HAL_Init() != HAL_OK )
		Error_Handler(ERROR_HAL_INIT);
		
  /* Configure the System clock to have a frequency of 216 MHz */
  SystemClock_Config();

  /* Initialize */
	System_Init();
	
#ifdef RTE_CMSIS_RTOS                   // when using CMSIS RTOS
  // create 'thread' functions that start executing,
  // example: tid_name = osThreadCreate (osThread(name), NULL);

  osKernelStart();                      // start thread execution 
#endif
	
	/* Test run */
	Demo_Run();
/*
	GUI_Init();
	GUI_SetColor(GUI_BLUE);
	GUI_SetFont(&GUI_Font16_1);
	GUI_DispStringHCenterAt("Oscyloskop cyfrowy v1.2" , 240, 150);
	GUI_DispStringHCenterAt("Adrian Kurylak" , 240, 175);
	GUI_DispStringHCenterAt("Politechnika Wroclawska" , 240, 200);
	*/
	/*tid_Main=osThreadGetId();
	os_status = osThreadTerminate (tid_Main);
	if (os_status != osOK)
		Error_Handler(ERROR_THREAD);*/
	
  /* Infinite loop */
  while (1) {
		if( ADC_Receive() != HAL_OK )
			Error_Handler(ERROR_CONVERSION);
	}
}

static void System_Init(void)
{
	if( Leds_Init() != HAL_OK )
		Error_Handler(ERROR_INIT);
		
	if( BSP_SDRAM_Init() != HAL_OK )
		Error_Handler(ERROR_INIT);
		
	if( Init_spi() != HAL_OK )
		Error_Handler(ERROR_INIT);
	
	if( DAC_Init() != HAL_OK )
		Error_Handler(ERROR_INIT);
	
	if( VGA_Init() != HAL_OK )
		Error_Handler(ERROR_INIT);
	
	if( ADC_Init() != HAL_OK )
		Error_Handler(ERROR_INIT);
	
	if( Relays_Init() != HAL_OK )
		Error_Handler(ERROR_INIT);
	
	if( Buttons_Initialize() != 0)
		Error_Handler(ERROR_INIT);

	Led(LEDGREEN,TRUE);
}

static void Demo_Run(void)
{
	uint8_t cnt=0;
	
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
		
	do
	{		
		for(cnt=0;cnt<31;cnt++)
		{
			HAL_Delay(20);
			Leds_Binary(cnt);
		}
		Leds_All_Off();
	}while(Buttons_GetState()== 0);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;  
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler(ERROR_INIT);
  }

  /* activate the OverDrive to reach the 216 Mhz Frequency */
  if(HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler(ERROR_INIT);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler(ERROR_INIT);
  }
}

/**
  * @brief  Configure the MPU attributes as Write Through for SRAM1/2.
  * @note   The Base Address is 0x20010000 since this memory interface is the AXI.
  *         The Region Size is 256KB, it is related to SRAM1 and SRAM2  memory size.
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;
  
  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU attributes as WT for SRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x20010000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
