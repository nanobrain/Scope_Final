/**
  ******************************************************************************
  * @file    VGA/ADC_Config.h 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Header for variable gain amplifier configuration file
  ******************************************************************************
  */
  
#ifndef __ADC_CONFIG_H
#define __ADC_CONFIG_H

/* Definition for SPIx Pins */
#define ADC_SPIx_CS_PIN                     GPIO_PIN_6
#define ADC_OPAMP_PD_PIN										GPIO_PIN_8
#define ADC_SPIx_CS_GPIO_PORT               GPIOG
#define ADC_OPAMP_PD_PORT										GPIOB
#define ADC_GPIO_CLK_ENABLE									__HAL_RCC_GPIOG_CLK_ENABLE
#define ADC_OPAMP_GPIO_CLK_ENABLE						__HAL_RCC_GPIOB_CLK_ENABLE
	
HAL_StatusTypeDef ADC_Receive(void);
HAL_StatusTypeDef ADC_Init(void);
HAL_StatusTypeDef ADC_DeInit(void);
uint8_t ADC_Is_Busy(void);

uint8_t ADC_Get_Payload(uint16_t a_Data);

#endif /* __ADC_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
