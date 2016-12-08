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

/* Exported constants --------------------------------------------------------*/
/* Definition for SPIx Pins */
#define DAC_SPIx_CS_PIN                     GPIO_PIN_2
#define DAC_SPIx_CS_GPIO_PORT               GPIOI
#define DAC_GPIO_CLK_ENABLE									__HAL_RCC_GPIOI_CLK_ENABLE

/* Exported macro ------------------------------------------------------------*/
/* Functions ------------------------------------------------------- */
static void DAC_CS_Write(LOGIC a_State);
static HAL_StatusTypeDef DAC_Transmit(void);
HAL_StatusTypeDef DAC_Init(void);
HAL_StatusTypeDef DAC_DeInit(void);
HAL_StatusTypeDef DAC_Set_Power_Mode(DAC_POWER_MODE a_PowerMode);
HAL_StatusTypeDef DAC_Set_Value(uint16_t a_16u_value);
HAL_StatusTypeDef DAC_Set_Voltage(uint8_t a_8u_voltage);
uint8_t Get_DAC_Voltage(void);
uint16_t Get_DAC_Value(void);
DAC_POWER_MODE Get_DAC_Power_Mode(void);


#endif /* __DAC_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
