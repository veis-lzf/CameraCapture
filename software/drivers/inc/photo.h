#ifndef __PHOTO_H__
#define	__PHOTO_H__

#include "type.h"
#include "malloc.h"


#define BMP_MODE	0
#define JPG_MODE	1

// 0,数据没有采集完;
// 1,数据采集完了,但是还没处理;
// 2,数据已经处理完成了,可以开始下一帧接收
#define CAPTURE_BUSY		0
#define CAPTURE_COMPLETE	1
#define PROCESS_COMPLETE	2


#define jpeg_dma_bufsize 5*1024  			// 定义JPEG数据缓存jpeg_buf的大小(*4字节)

#ifdef __MALLOC_H
extern uint32_t *jpeg_buf0;							//JPEG数据缓存buf,通过malloc申请内存
extern uint32_t *jpeg_buf1;							//JPEG数据缓存buf,通过malloc申请内存
extern uint32_t *jpeg_data_buf;						//JPEG数据缓存buf,通过malloc申请内存
extern uint8_t *pname;
#else
extern __align(4) uint32_t jpeg_buf[jpeg_buf_size];	// JPEG数据缓存buf
#endif
extern volatile uint32_t jpeg_data_len; 			// buf中的JPEG有效数据长度 
extern volatile uint8_t jpeg_data_ok;		// JPEG数据采集完成标志 

typedef enum 
{
	eType_QCIF = 0,
	eType_QQVGA,
	eType_CIF,
	eType_QVGA,
	eType_VGA,
	eType_SVGA,
	eType_XGA,
	eType_SXGA,
	eType_UXGA,
} eJPEG_Type;

void Init_JPEG(eJPEG_Type type);
//文件名自增（避免覆盖）
//mode:0,创建.bmp文件;1,创建.jpg文件.
//bmp组合成:形如"0:PHOTO/PIC13141.bmp"的文件名
//jpg组合成:形如"0:PHOTO/PIC13141.jpg"的文件名
void camera_new_pathname(uint8_t *pname, uint8_t mode);
//OV2640拍照jpg图片
//返回值:0,成功
//    其他,错误代码
uint8_t take_jpg_photo(uint8_t *pname);

#endif
