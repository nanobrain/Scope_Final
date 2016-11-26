/**
  ******************************************************************************
  * @file    DAC/DAC_Config.h 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Header for dac configuration file
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DAC_CONFIG_H
#define __DAC_CONFIG_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum DAC_POWER_MODE{
	POWER_DOWN,
	POWER_FULL
}DAC_POWER_MODE;

typedef enum LOGIC{
	FALSE=0,
	TRUE
}LOGIC;

/* Exported constants --------------------------------------------------------*/
/* Definition for SPIx Pins */
#define DAC_SPIx_SYNC_PIN                     GPIO_PIN_2
#define DAC_SPIx_SYNC_GPIO_PORT               GPIOI
#define DAC_GPIO_CLK_ENABLE										__HAL_RCC_GPIOI_CLK_ENABLE
#define DAC_GPIO_CLK_DISABLE									__HAL_RCC_GPIOI_CLK_DISABLE

/* Size of buffer */
#define BUFFERSIZE(x)                       	(COUNTOF(x) - 1)

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
	
/* Functions ------------------------------------------------------- */
static void DAC_Sync_Write(LOGIC a_SetReset);
static HAL_StatusTypeDef DAC_Transmit(uint8_t*  a_p8u_Data);
HAL_StatusTypeDef Set_DAC_Output(uint8_t a_8u_voltage);
uint8_t Get_DAC_Output(void);
HAL_StatusTypeDef Set_DAC_Power_Mode(DAC_POWER_MODE a_PowerMode);
HAL_StatusTypeDef DAC_Init(void);
HAL_StatusTypeDef DAC_DeInit(void);

#endif /* __DAC_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
