#ifndef __PHOTO_H__
#define	__PHOTO_H__

#include "type.h"
#include "malloc.h"


#define BMP_MODE	0
#define JPG_MODE	1

// 0,����û�вɼ���;
// 1,���ݲɼ�����,���ǻ�û����;
// 2,�����Ѿ����������,���Կ�ʼ��һ֡����
#define CAPTURE_BUSY		0
#define CAPTURE_COMPLETE	1
#define PROCESS_COMPLETE	2


#define jpeg_dma_bufsize 5*1024  			// ����JPEG���ݻ���jpeg_buf�Ĵ�С(*4�ֽ�)

#ifdef __MALLOC_H
extern uint32_t *jpeg_buf0;							//JPEG���ݻ���buf,ͨ��malloc�����ڴ�
extern uint32_t *jpeg_buf1;							//JPEG���ݻ���buf,ͨ��malloc�����ڴ�
extern uint32_t *jpeg_data_buf;						//JPEG���ݻ���buf,ͨ��malloc�����ڴ�
extern uint8_t *pname;
#else
extern __align(4) uint32_t jpeg_buf[jpeg_buf_size];	// JPEG���ݻ���buf
#endif
extern volatile uint32_t jpeg_data_len; 			// buf�е�JPEG��Ч���ݳ��� 
extern volatile uint8_t jpeg_data_ok;		// JPEG���ݲɼ���ɱ�־ 

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
//�ļ������������⸲�ǣ�
//mode:0,����.bmp�ļ�;1,����.jpg�ļ�.
//bmp��ϳ�:����"0:PHOTO/PIC13141.bmp"���ļ���
//jpg��ϳ�:����"0:PHOTO/PIC13141.jpg"���ļ���
void camera_new_pathname(uint8_t *pname, uint8_t mode);
//OV2640����jpgͼƬ
//����ֵ:0,�ɹ�
//    ����,�������
uint8_t take_jpg_photo(uint8_t *pname);

#endif
