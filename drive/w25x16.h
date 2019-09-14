#ifndef __W25X16_H
#define __W25X16_H


#include "stm32f37x.h"

/* Private typedef -----------------------------------------------------------*/
//#define SPI_FLASH_PageSize      4096
#define SPI_FLASH_PageSize      256
#define SPI_FLASH_PerWritePageSize      256

/* Private define ------------------------------------------------------------*/
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		    0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F 

#define WIP_Flag                  0x01  /* Write In Progress (WIP) flag */

#define Dummy_Byte                0xFF

#define FLASH_CS_PIN         GPIO_Pin_2
#define FLASH_CS_PORT        GPIOA
#define FLASH_CS_PIN_SCK     RCC_AHBPeriph_GPIOA

#define FLASH_SCK_PIN        GPIO_Pin_8
#define FLASH_SCK_PORT       GPIOD
#define FLASH_SCK_PIN_SCK    RCC_AHBPeriph_GPIOD
#define FLASH_SCK_SOURCE     GPIO_PinSource8
#define FLASH_SCK_AF         GPIO_AF_5

#define FLASH_MISO_PIN       GPIO_Pin_14
#define FLASH_MISO_PORT      GPIOB
#define FLASH_MISO_PIN_SCK   RCC_AHBPeriph_GPIOB
#define FLASH_MISO_SOURCE    GPIO_PinSource14
#define FLASH_MISO_AF        GPIO_AF_5

#define FLASH_MOSI_PIN       GPIO_Pin_15
#define FLASH_MOSI_PORT      GPIOB
#define FLASH_MOSI_PIN_SCK   RCC_AHBPeriph_GPIOB
#define FLASH_MOSI_SOURCE    GPIO_PinSource15
#define FLASH_MOSI_AF        GPIO_AF_5


#define FLASH_SPI2               RCC_APB1Periph_SPI2
#define SPI_FLASH_CS_LOW()       GPIO_ResetBits(GPIOA, GPIO_Pin_2)
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(GPIOA, GPIO_Pin_2)


void SPI_FLASH_Init(void);
void SPI_FLASH_SectorErase(uint32_t SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
uint32_t SPI_FLASH_ReadID(void);
uint32_t SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);


uint8_t SPI_FLASH_ReadByte(void);
uint8_t SPI_FLASH_SendByte(uint8_t byte);
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);

#endif /* __W25X16_H */

