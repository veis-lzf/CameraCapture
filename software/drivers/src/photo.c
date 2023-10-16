#define DEBUG

#include "photo.h"
#include "debug.h"
#include "drivers.h"

#ifdef __MALLOC_H
uint32_t *jpeg_buf0;							//JPEG数据缓存buf,通过malloc申请内存
uint32_t *jpeg_buf1;							//JPEG数据缓存buf,通过malloc申请内存
uint32_t *jpeg_data_buf;						//JPEG数据缓存buf,通过malloc申请内存
uint8_t *pname;
#else
__align(4) uint32_t jpeg_buf[jpeg_buf_size];	// JPEG数据缓存buf
#endif
volatile uint32_t jpeg_data_len=0; 			// buf中的JPEG有效数据长度 
volatile uint8_t jpeg_data_ok=0;		// JPEG数据采集完成标志 
										// 0,数据没有采集完;
										// 1,数据采集完了,但是还没处理;
										// 2,数据已经处理完成了,可以开始下一帧接收

// JPEG尺寸支持列表
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

// 初始化JPEG图片参数
void Init_JPEG(eJPEG_Type type)
{
    uint8_t effect = 3,saturation = 3,contrast = 3, brightness = 4;
	uint8_t size = type;		                        // 默认是QVGA 320*240尺寸
    
	OV2640_ImageWin_Set(0,0,jpeg_img_size_tbl[size][0],jpeg_img_size_tbl[size][1]);
	OV2640_OutSize_Set(jpeg_img_size_tbl[size][0], jpeg_img_size_tbl[size][1]);  // 设置输出尺寸 
    OV2640_Contrast(contrast);              // 设置对比度
    OV2640_Special_Effects(effect);         // 设置特效
    OV2640_Color_Saturation(saturation);    // 设置饱和度
	OV2640_Brightness(brightness);
	OV2640_Light_Mode(0);
}

//文件名自增（避免覆盖）
//mode:0,创建.bmp文件;1,创建.jpg文件.
//bmp组合成:形如"0:PHOTO/PIC13141.bmp"的文件名
//jpg组合成:形如"0:PHOTO/PIC13141.jpg"的文件名
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
		res = f_open(ftemp,(const TCHAR*)pname,FA_READ);//尝试打开这个文件
		if(res==FR_NO_FILE)break;		//该文件名不存在=正是我们需要的.
		index++;
	}
}

//OV2640拍照jpg图片
//返回值:0,成功
//    其他,错误代码
uint8_t take_jpg_photo(uint8_t *pname)
{
	FIL* f_jpg;
	uint8_t res=0;
	uint32_t bwr;
	uint16_t i, jpeg_start, jpeg_len, jpeg_ok;
	uint8_t* pbuf;
	f_jpg=(FIL *)mymalloc(SRAMIN,sizeof(FIL));	//开辟FIL字节的内存区域
	if(f_jpg==NULL)return 0XFF;					//内存申请失败.
	
//	while(jpeg_data_ok != CAPTURE_COMPLETE);	// 等待第一帧图片采集完
//	jpeg_data_ok = PROCESS_COMPLETE;			// 忽略本帧图片,启动下一帧采集
//	while(jpeg_data_ok != CAPTURE_COMPLETE);	// 等待第二帧图片采集完
//	jpeg_data_ok = PROCESS_COMPLETE;			// 忽略本帧图片,启动下一帧采集
	while(jpeg_data_ok != CAPTURE_COMPLETE);	// 等待第三帧图片采集完,第三帧,才保存到SD卡去.
	DCMI_Stop();
	
	jpeg_start = 0, jpeg_len = 0, jpeg_ok = 0;
	res = f_open(f_jpg,(const TCHAR*)pname,FA_WRITE|FA_CREATE_NEW);//模式0,或者尝试打开失败,则创建新文件	 
	if(res == 0)
	{
		p_info("jpeg data size:%d", jpeg_data_len*4);//串口打印JPEG文件大小
		pbuf=(uint8_t*)jpeg_data_buf;
		
		for(i=0;i<jpeg_data_len*4;i++)//查找0XFF,0XD8
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

		if(jpeg_len && jpeg_ok == 1)//找到了
		{
			p_info("jpeg_len=%d", jpeg_len);
			pbuf += jpeg_start;//偏移到0XFF,0XD8处
			res=f_write(f_jpg, pbuf, jpeg_len, &bwr);
			if(bwr != jpeg_len)res=0XFE;
		}		
		else
		{
			res = 0XFD;//没找到0XFF,0XD8
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
