/**
  ******************************************************************************
  * @file    VGA/VGA_Config.h 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Header for variable gain amplifier configuration file
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VGA_CONFIG_H
#define __VGA_CONFIG_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum VGA_POWER_MODE{
	POWER_FULL,
	AUX_HIGHZ,
	HIGHZ
}VGA_POWER_MODE;

typedef enum VGA_BW_FILTER{
	FULL_BW,
	LP20MHZ,
	LP100MHZ,
	LP200MHZ,
	LP350MHZ,
	LP650MHZ,
	LP750MHZ
}VGA_BW_FILTER;

typedef enum VGA_PREAMP_GAIN{
	HG,
	LG
}VGA_PREAMP_GAIN;

typedef enum VGA_LADDER_GAIN{
	DB0=0,
	DB2=2,
	DB4=4,
	DB6=6,
	DB8=8,
	DB10=10,
	DB12=12,
	DB14=14,
	DB16=16,
	DB18=18,
	DB20=20
}VGA_LADDER_GAIN;

/* Exported constants --------------------------------------------------------*/
/* Definition for SPIx Pins */
#define VGA_SPIx_CS_PIN                     GPIO_PIN_4
#define VGA_SPIx_CS_GPIO_PORT               GPIOB
#define VGA_GPIO_CLK_ENABLE										__HAL_RCC_GPIOB_CLK_ENABLE
/* Exported macro ------------------------------------------------------------*/
/* Functions ------------------------------------------------------- */
static void VGA_CS_Write(LOGIC a_SetReset);
static HAL_StatusTypeDef VGA_Transmit(uint8_t*  a_p8u_Data);
HAL_StatusTypeDef Set_VGA_Gain(uint8_t a_8u_gain);
uint8_t Get_VGA_Gain(void);
HAL_StatusTypeDef Set_VGA_Power_Mode(VGA_POWER_MODE a_PowerMode);
HAL_StatusTypeDef VGA_Init(void);
HAL_StatusTypeDef VGA_DeInit(void);

#endif /* __VGA_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
