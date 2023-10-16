#define DEBUG

#include "photo.h"
#include "debug.h"
#include "drivers.h"

#ifdef __MALLOC_H
uint32_t *jpeg_buf0;							//JPEG���ݻ���buf,ͨ��malloc�����ڴ�
uint32_t *jpeg_buf1;							//JPEG���ݻ���buf,ͨ��malloc�����ڴ�
uint32_t *jpeg_data_buf;						//JPEG���ݻ���buf,ͨ��malloc�����ڴ�
uint8_t *pname;
#else
__align(4) uint32_t jpeg_buf[jpeg_buf_size];	// JPEG���ݻ���buf
#endif
volatile uint32_t jpeg_data_len=0; 			// buf�е�JPEG��Ч���ݳ��� 
volatile uint8_t jpeg_data_ok=0;		// JPEG���ݲɼ���ɱ�־ 
										// 0,����û�вɼ���;
										// 1,���ݲɼ�����,���ǻ�û����;
										// 2,�����Ѿ����������,���Կ�ʼ��һ֡����

// JPEG�ߴ�֧���б�
const uint16_t jpeg_img_size_tbl[][2]=
{
	176,144,	// QCIF
	160,120,	// QQVGA
	352,288,	// CIF
	320,240,	// QVGA
	640,480,	// VGA
	800,600,	// SVGA
	1024,768,	// XGA
	1280,1024,	// SXGA
	1600,1200,	// UXGA
}; 

// ��ʼ��JPEGͼƬ����
void Init_JPEG(eJPEG_Type type)
{
    uint8_t effect = 3,saturation = 3,contrast = 3, brightness = 4;
	uint8_t size = type;		                        // Ĭ����QVGA 320*240�ߴ�
    
	OV2640_ImageWin_Set(0,0,jpeg_img_size_tbl[size][0],jpeg_img_size_tbl[size][1]);
	OV2640_OutSize_Set(jpeg_img_size_tbl[size][0], jpeg_img_size_tbl[size][1]);  // ��������ߴ� 
    OV2640_Contrast(contrast);              // ���öԱȶ�
    OV2640_Special_Effects(effect);         // ������Ч
    OV2640_Color_Saturation(saturation);    // ���ñ��Ͷ�
	OV2640_Brightness(brightness);
	OV2640_Light_Mode(0);
}

//�ļ������������⸲�ǣ�
//mode:0,����.bmp�ļ�;1,����.jpg�ļ�.
//bmp��ϳ�:����"0:PHOTO/PIC13141.bmp"���ļ���
//jpg��ϳ�:����"0:PHOTO/PIC13141.jpg"���ļ���
void camera_new_pathname(uint8_t *pname, uint8_t mode)
{	 
	uint8_t res;					 
	uint16_t index = 0;
	
	//while(index < 0XFFFF)
	while(index < 30)
	{
		index = get_random() % 65535;
		if(mode==0) sprintf((char*)pname,"PIC_%05d.bmp", index);
		else sprintf((char*)pname,"PIC_%05d.jpg", index);
		res = f_open(ftemp,(const TCHAR*)pname,FA_READ);//���Դ�����ļ�
		if(res==FR_NO_FILE)break;		//���ļ���������=����������Ҫ��.
		index++;
	}
}

//OV2640����jpgͼƬ
//����ֵ:0,�ɹ�
//    ����,�������
uint8_t take_jpg_photo(uint8_t *pname)
{
	FIL* f_jpg;
	uint8_t res=0;
	uint32_t bwr;
	uint16_t i, jpeg_start, jpeg_len, jpeg_ok;
	uint8_t* pbuf;
	f_jpg=(FIL *)mymalloc(SRAMIN,sizeof(FIL));	//����FIL�ֽڵ��ڴ�����
	if(f_jpg==NULL)return 0XFF;					//�ڴ�����ʧ��.
	
//	while(jpeg_data_ok != CAPTURE_COMPLETE);	// �ȴ���һ֡ͼƬ�ɼ���
//	jpeg_data_ok = PROCESS_COMPLETE;			// ���Ա�֡ͼƬ,������һ֡�ɼ�
//	while(jpeg_data_ok != CAPTURE_COMPLETE);	// �ȴ��ڶ�֡ͼƬ�ɼ���
//	jpeg_data_ok = PROCESS_COMPLETE;			// ���Ա�֡ͼƬ,������һ֡�ɼ�
	while(jpeg_data_ok != CAPTURE_COMPLETE);	// �ȴ�����֡ͼƬ�ɼ���,����֡,�ű��浽SD��ȥ.
	DCMI_Stop();
	
	jpeg_start = 0, jpeg_len = 0, jpeg_ok = 0;
	res = f_open(f_jpg,(const TCHAR*)pname,FA_WRITE|FA_CREATE_NEW);//ģʽ0,���߳��Դ�ʧ��,�򴴽����ļ�	 
	if(res == 0)
	{
		p_info("jpeg data size:%d", jpeg_data_len*4);//���ڴ�ӡJPEG�ļ���С
		pbuf=(uint8_t*)jpeg_data_buf;
		
		for(i=0;i<jpeg_data_len*4;i++)//����0XFF,0XD8
		{
			if((pbuf[i]==0XFF)&&(pbuf[i+1]==0XD8))
			{
				jpeg_start = i;
				jpeg_ok = 1;
			}
			if((pbuf[i]==0XFF)&&(pbuf[i+1]==0XD9))
			{
				jpeg_len = i - jpeg_start + 2;
				break;
			}
		}

		if(jpeg_len && jpeg_ok == 1)//�ҵ���
		{
			p_info("jpeg_len=%d", jpeg_len);
			pbuf += jpeg_start;//ƫ�Ƶ�0XFF,0XD8��
			res=f_write(f_jpg, pbuf, jpeg_len, &bwr);
			if(bwr != jpeg_len)res=0XFE;
		}		
		else
		{
			res = 0XFD;//û�ҵ�0XFF,0XD8
			f_unlink((const TCHAR *)pname);
		}
	}
	memset(jpeg_data_buf, 0, jpeg_data_len*4);
	jpeg_data_len=0;
	f_close(f_jpg);
	myfree(SRAMIN, f_jpg);
	jpeg_data_ok = PROCESS_COMPLETE;
	DCMI_Start();
	p_info("res=%d", res);
	return res;
}  
