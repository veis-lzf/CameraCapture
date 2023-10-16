#include "fattester.h"	 
#include "bsp_sdio_sd.h"
#include "debug.h"
#include "exfuns.h"
#include "malloc.h"		  
#include "ff.h"
#include "string.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32F407������
//FATFS ���Դ���	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
    
//Ϊ����ע�Ṥ����	 
//path:����·��������"0:"��"1:"
//mt:0��������ע�ᣨ�Ժ�ע�ᣩ��1������ע��
//����ֵ:ִ�н��
uint8_t mf_mount(uint8_t* path,uint8_t mt)
{		   
	return f_mount(fs[2],(const TCHAR*)path,mt); 
}
//��·���µ��ļ�
//path:·��+�ļ���
//mode:��ģʽ
//����ֵ:ִ�н��
uint8_t mf_open(uint8_t*path,uint8_t mode)
{
	uint8_t res;	 
	res=f_open(file,(const TCHAR*)path,mode);//���ļ���
	return res;
} 
//�ر��ļ�
//����ֵ:ִ�н��
uint8_t mf_close(void)
{
	f_close(file);
	return 0;
}
//��������
//len:�����ĳ���
//����ֵ:ִ�н��
uint8_t mf_read(uint16_t len)
{
	uint16_t i,t;
	uint8_t res=0;
	uint16_t tlen=0;
	p_info("\r\nRead file data is:\r\n");
	for(i=0;i<len/512;i++)
	{
		res=f_read(file,fatbuf,512,&br);
		if(res)
		{
			p_info("Read Error:%d\r\n",res);
			break;
		}else
		{
			tlen+=br;
			for(t=0;t<br;t++)p_info("%c",fatbuf[t]); 
		}
	}
	if(len%512)
	{
		res=f_read(file,fatbuf,len%512,&br);
		if(res)	//�����ݳ�����
		{
			p_info("\r\nRead Error:%d\r\n",res);   
		}else
		{
			tlen+=br;
			for(t=0;t<br;t++)p_info("%c",fatbuf[t]); 
		}	 
	}
	if(tlen)p_info("\r\nReaded data len:%d\r\n",tlen);//���������ݳ���
	p_info("Read data over\r\n");	 
	return res;
}
//д������
//dat:���ݻ�����
//len:д�볤��
//����ֵ:ִ�н��
uint8_t mf_write(uint8_t*dat,uint16_t len)
{			    
	uint8_t res;	   					   

	p_info("\r\nBegin Write file...\r\n");
	p_info("Write data len:%d\r\n",len);	 
	res=f_write(file,dat,len,&bw);
	if(res)
	{
		p_info("Write Error:%d\r\n",res);   
	}else p_info("Writed data len:%d\r\n",bw);
	p_info("Write data over.\r\n");
	return res;
}

//��Ŀ¼
 //path:·��
//����ֵ:ִ�н��
uint8_t mf_opendir(uint8_t* path)
{
	return f_opendir(&dir,(const TCHAR*)path);	
}
//�ر�Ŀ¼ 
//����ֵ:ִ�н��
uint8_t mf_closedir(void)
{
	return f_closedir(&dir);	
}
//���ȡ�ļ���
//����ֵ:ִ�н��
uint8_t mf_readdir(void)
{
	uint8_t res;
	char *fn;			 
#if _USE_LFN
 	fileinfo.lfsize = _MAX_LFN * 2 + 1;
	fileinfo.lfname = mymalloc(SRAMIN,fileinfo.lfsize);
#endif		  
	res=f_readdir(&dir,&fileinfo);//��ȡһ���ļ�����Ϣ
	if(res!=FR_OK||fileinfo.fname[0]==0)
	{
		myfree(SRAMIN,fileinfo.lfname);
		return res;//������.
	}
#if _USE_LFN
	fn=*fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else
	fn=fileinfo.fname;;
#endif	
	p_info("\r\n DIR info:\r\n");

	p_info("dir.id:%d\r\n",dir.id);
	p_info("dir.index:%d\r\n",dir.index);
	p_info("dir.sclust:%d\r\n",dir.sclust);
	p_info("dir.clust:%d\r\n",dir.clust);
	p_info("dir.sect:%d\r\n",dir.sect);	  

	p_info("\r\n");
	p_info("File Name is:%s\r\n",fn);
	p_info("File Size is:%d\r\n",fileinfo.fsize);
	p_info("File data is:%d\r\n",fileinfo.fdate);
	p_info("File time is:%d\r\n",fileinfo.ftime);
	p_info("File Attr is:%d\r\n",fileinfo.fattrib);
	p_info("\r\n");
	myfree(SRAMIN,fileinfo.lfname);
	return 0;
}			 

 //�����ļ�
 //path:·��
 //����ֵ:ִ�н��
uint8_t mf_scan_files(uint8_t * path)
{
	FRESULT res;	  
    char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
 	fileinfo.lfsize = _MAX_LFN * 2 + 1;
	fileinfo.lfname = mymalloc(SRAMIN,fileinfo.lfsize);
#endif		  

    res = f_opendir(&dir,(const TCHAR*)path); //��һ��Ŀ¼
    if (res == FR_OK) 
	{	
		p_info("\r\n"); 
		while(1)
		{
	        res = f_readdir(&dir, &fileinfo);                   //��ȡĿ¼�µ�һ���ļ�
	        if (res != FR_OK || fileinfo.fname[0] == 0) break;  //������/��ĩβ��,�˳�
	        //if (fileinfo.fname[0] == '.') continue;             //�����ϼ�Ŀ¼
#if _USE_LFN
        	fn = *fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else							   
        	fn = fileinfo.fname;
#endif	                                              /* It is a file. */
			p_info("%s/", path);//��ӡ·��	
			p_info("%s\r\n",  fn);//��ӡ�ļ���	  
		} 
    }	  
	myfree(SRAMIN,fileinfo.lfname);
    return res;	  
}
//��ʾʣ������
//drv:�̷�
//����ֵ:ʣ������(�ֽ�)
uint32_t mf_showfree(uint8_t *drv)
{
	FATFS *fs1;
	uint8_t res;
    uint32_t fre_clust=0, fre_sect=0, tot_sect=0;
    //�õ�������Ϣ�����д�����
    res = f_getfree((const TCHAR*)drv,(DWORD*)&fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect = (fs1->n_fatent - 2) * fs1->csize;//�õ���������
	    fre_sect = fre_clust * fs1->csize;			//�õ�����������	   
#if _MAX_SS!=512
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif	  
		if(tot_sect<20480)//������С��10M
		{
		    /* Print free space in unit of KB (assuming 512 bytes/sector) */
		    p_info("\r\n����������:%d KB\r\n"
		           "���ÿռ�:%d KB\r\n",
		           tot_sect>>1,fre_sect>>1);
		}else
		{
		    /* Print free space in unit of KB (assuming 512 bytes/sector) */
		    p_info("\r\n����������:%d MB\r\n"
		           "���ÿռ�:%d MB\r\n",
		           tot_sect>>11,fre_sect>>11);
		}
	}
	return fre_sect;
}		    
//�ļ���дָ��ƫ��
//offset:����׵�ַ��ƫ����
//����ֵ:ִ�н��.
uint8_t mf_lseek(uint32_t offset)
{
	return f_lseek(file,offset);
}
//��ȡ�ļ���ǰ��дָ���λ��.
//����ֵ:λ��
uint32_t mf_tell(void)
{
	return f_tell(file);
}
//��ȡ�ļ���С
//����ֵ:�ļ���С
uint32_t mf_size(void)
{
	return f_size(file);
} 
//����Ŀ¼
//pname:Ŀ¼·��+����
//����ֵ:ִ�н��
uint8_t mf_mkdir(uint8_t*pname)
{
	return f_mkdir((const TCHAR *)pname);
}
//��ʽ��
//path:����·��������"0:"��"1:"
//mode:ģʽ
//au:�ش�С
//����ֵ:ִ�н��
uint8_t mf_fmkfs(uint8_t* path,uint8_t mode,uint16_t au)
{
	return f_mkfs((const TCHAR*)path,mode,au);//��ʽ��,drv:�̷�;mode:ģʽ;au:�ش�С
} 
//ɾ���ļ�/Ŀ¼
//pname:�ļ�/Ŀ¼·��+����
//����ֵ:ִ�н��
uint8_t mf_unlink(uint8_t *pname)
{
	return  f_unlink((const TCHAR *)pname);
}

//�޸��ļ�/Ŀ¼����(���Ŀ¼��ͬ,�������ƶ��ļ�Ŷ!)
//oldname:֮ǰ������
//newname:������
//����ֵ:ִ�н��
uint8_t mf_rename(uint8_t *oldname,uint8_t* newname)
{
	return  f_rename((const TCHAR *)oldname,(const TCHAR *)newname);
}
//��ȡ�̷����������֣�
//path:����·��������"0:"��"1:"  
void mf_getlabel(uint8_t *path)
{
	uint8_t buf[20];
	uint32_t sn=0;
	uint8_t res;
	res=f_getlabel ((const TCHAR *)path,(TCHAR *)buf,(DWORD*)&sn);
	if(res==FR_OK)
	{
		p_info("\r\n����%s ���̷�Ϊ:%s\r\n",path,buf);
		p_info("����%s �����к�:%X\r\n\r\n",path,sn); 
	}else p_info("\r\n��ȡʧ�ܣ�������:%X\r\n",res);
}
//�����̷����������֣����11���ַ�������֧�����ֺʹ�д��ĸ����Լ����ֵ�
//path:���̺�+���֣�����"0:ALIENTEK"��"1:OPENEDV"  
void mf_setlabel(uint8_t *path)
{
	uint8_t res;
	res=f_setlabel ((const TCHAR *)path);
	if(res==FR_OK)
	{
		p_info("\r\n�����̷����óɹ�:%s\r\n",path);
	}else p_info("\r\n�����̷�����ʧ�ܣ�������:%X\r\n",res);
} 

//���ļ������ȡһ���ַ���
//size:Ҫ��ȡ�ĳ���
void mf_gets(uint16_t size)
{
 	TCHAR* rbuf;
	rbuf=f_gets((TCHAR*)fatbuf,size,file);
	if(*rbuf==0)return  ;//û�����ݶ���
	else
	{
		p_info("\r\nThe String Readed Is:%s\r\n",rbuf);  	  
	}			    	
}
//��Ҫ_USE_STRFUNC>=1
//дһ���ַ����ļ�
//c:Ҫд����ַ�
//����ֵ:ִ�н��
uint8_t mf_putc(uint8_t c)
{
	return f_putc((TCHAR)c,file);
}
//д�ַ������ļ�
//c:Ҫд����ַ���
//����ֵ:д����ַ�������
uint8_t mf_puts(uint8_t*c)
{
	return f_puts((TCHAR*)c,file);
}












