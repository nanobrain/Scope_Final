/**
  ******************************************************************************
  * @file    Templates/main.h 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    25-June-2015
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
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
  
#ifndef __MAIN_H
#define __MAIN_H

#include <cstring>
#include "stm32f7xx_hal.h"
#include "Data_Processing.h"
#include "Board_Buttons.h"              // ::Board Support:Buttons

typedef enum LOGIC{
	FALSE=0,
	TRUE=1
}LOGIC;

#define COUNTOF(__BUFFER__)   						(sizeof(__BUFFER__) * sizeof(*(__BUFFER__)))
#define UNUSED(x) 												((void)(x))
#define BUFFERSIZE(x)											(COUNTOF(x) - 1)

#define SPI_DMA														0 // 1 -> Use DMA. 0 -> Use IT
#define FAKE_WAVEFORM 										0	// 1 -> Use fake waveform. 0 -> Use real waveform from ADC
#define RX_BUFFERCOUNT										2048
#define SDRAM_ADDRESS											0xC0200000
#define GUI_BUFFER_ADDRESS 								0xC0200000
#define GUI_NUMBYTES  										0x200000
#define AQQ_MAIN_BUFFER_ADDRESS						SDRAM_ADDRESS+GUI_NUMBYTES
#define AQQ_COPY_BUFFER_ADDRESS						AQQ_MAIN_BUFFER_ADDRESS+RX_BUFFERCOUNT*sizeof(ADCData8)

/* Definitions for SPIx RCC resources */
#define SPIx															SPI2
#define SPIx_CLK_ENABLE()									__HAL_RCC_SPI2_CLK_ENABLE()
#define SPIx_DMAx_CLK_ENABLE()						__HAL_RCC_DMA1_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE()				__HAL_RCC_GPIOI_CLK_ENABLE()
#define SPIx_MISO_GPIO_CLK_ENABLE()				__HAL_RCC_GPIOB_CLK_ENABLE()
#define SPIx_MOSI_GPIO_CLK_ENABLE()				__HAL_RCC_GPIOB_CLK_ENABLE()
#define SPIx_NSS_GPIO_CLK_ENABLE()				__HAL_RCC_GPIOB_CLK_ENABLE()

#define SPIx_FORCE_RESET()								__HAL_RCC_SPI2_FORCE_RESET()
#define SPIx_RELEASE_RESET()							__HAL_RCC_SPI2_RELEASE_RESET()

/* Definitions for SPIx Pins */
#define SPIx_SCK_PIN											GPIO_PIN_1
#define SPIx_SCK_GPIO_PORT								GPIOI
#define SPIx_MISO_PIN											GPIO_PIN_14
#define SPIx_MISO_GPIO_PORT								GPIOB
#define SPIx_MOSI_PIN											GPIO_PIN_15
#define SPIx_MOSI_GPIO_PORT								GPIOB
#define SPIx_NSS_PIN											GPIO_PIN_9
#define SPIx_NSS_PORT											GPIOB

/* Definitions for SPIx's DMA */
#define SPIx_TX_DMA_STREAM								DMA1_Stream4
#define SPIx_RX_DMA_STREAM								DMA1_Stream3
#define SPIx_TX_DMA_CHANNEL								DMA_CHANNEL_0
#define SPIx_RX_DMA_CHANNEL								DMA_CHANNEL_0

/* Definitions for SPIx's NVIC */
#define SPIx_IRQn													SPI2_IRQn
#define SPIx_IRQHandler										SPI2_IRQHandler

#define SPIx_DMA_TX_IRQn									DMA1_Stream4_IRQn
#define SPIx_DMA_RX_IRQn									DMA1_Stream3_IRQn

#define SPIx_DMA_TX_IRQHandler						DMA1_Stream4_IRQHandler
#define SPIx_DMA_RX_IRQHandler						DMA1_Stream3_IRQHandler

/* Definitions for DMA memcp */
#define MEMCP_DMAx_CLK_ENABLE()						__HAL_RCC_DMA1_CLK_ENABLE()
#define MEMCP_DMAx_STREAM									DMA1_Stream1
#define MEMCP_DMAx_CHANNEL								DMA_CHANNEL_1
#define MEMCP_DMAx_IRQn										DMA1_Stream1_IRQn

void SPI_NSS_Init(void);
void SPI_NSS_DeInit(void);

extern ADCData8 g_d8_RxBufferMain1[RX_BUFFERCOUNT];

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
