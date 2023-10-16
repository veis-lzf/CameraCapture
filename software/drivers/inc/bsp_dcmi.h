#ifndef _DCMI_H
#define _DCMI_H
#include "bsp.h"

void bsp_DCMI_Init(void);

//memaddr:�洢����ַ    ��Ҫ�洢����ͷ���ݵ��ڴ��ַ(Ҳ�����������ַ)
//DMA_BufferSize:�洢������    0~65535
//DMA_MemoryDataSize:�洢��λ��    @defgroup DMA_memory_data_size :DMA_MemoryDataSize_Byte/DMA_MemoryDataSize_HalfWord/DMA_MemoryDataSize_Word
//DMA_MemoryInc:�洢��������ʽ  @defgroup DMA_memory_incremented_mode  /** @defgroup DMA_memory_incremented_mode : DMA_MemoryInc_Enable/DMA_MemoryInc_Disable
void DCMI_DMA_Init(u32 DMA_Memory0BaseAddr,u32 DMA_Memory1BaseAddr,u16 DMA_BufferSize,u32 DMA_MemoryDataSize,u32 DMA_MemoryInc);

void DCMI_Start(void);
void DCMI_Stop(void);

void DCMI_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);
void DCMI_CR_Set(uint8_t pclk,uint8_t hsync,uint8_t vsync);

#endif /*_DCMI_H*/

