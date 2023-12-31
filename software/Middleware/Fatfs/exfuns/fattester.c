#include "fattester.h"	 
#include "bsp_sdio_sd.h"
#include "debug.h"
#include "exfuns.h"
#include "malloc.h"		  
#include "ff.h"
#include "string.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//FATFS 测试代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/15
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
    
//为磁盘注册工作区	 
//path:磁盘路径，比如"0:"、"1:"
//mt:0，不立即注册（稍后注册）；1，立即注册
//返回值:执行结果
uint8_t mf_mount(uint8_t* path,uint8_t mt)
{		   
	return f_mount(fs[2],(const TCHAR*)path,mt); 
}
//打开路径下的文件
//path:路径+文件名
//mode:打开模式
//返回值:执行结果
uint8_t mf_open(uint8_t*path,uint8_t mode)
{
	uint8_t res;	 
	res=f_open(file,(const TCHAR*)path,mode);//打开文件夹
	return res;
} 
//关闭文件
//返回值:执行结果
uint8_t mf_close(void)
{
	f_close(file);
	return 0;
}
//读出数据
//len:读出的长度
//返回值:执行结果
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
		if(res)	//读数据出错了
		{
			p_info("\r\nRead Error:%d\r\n",res);   
		}else
		{
			tlen+=br;
			for(t=0;t<br;t++)p_info("%c",fatbuf[t]); 
		}	 
	}
	if(tlen)p_info("\r\nReaded data len:%d\r\n",tlen);//读到的数据长度
	p_info("Read data over\r\n");	 
	return res;
}
//写入数据
//dat:数据缓存区
//len:写入长度
//返回值:执行结果
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

//打开目录
 //path:路径
//返回值:执行结果
uint8_t mf_opendir(uint8_t* path)
{
	return f_opendir(&dir,(const TCHAR*)path);	
}
//关闭目录 
//返回值:执行结果
uint8_t mf_closedir(void)
{
	return f_closedir(&dir);	
}
//打读取文件夹
//返回值:执行结果
uint8_t mf_readdir(void)
{
	uint8_t res;
	char *fn;			 
#if _USE_LFN
 	fileinfo.lfsize = _MAX_LFN * 2 + 1;
	fileinfo.lfname = mymalloc(SRAMIN,fileinfo.lfsize);
#endif		  
	res=f_readdir(&dir,&fileinfo);//读取一个文件的信息
	if(res!=FR_OK||fileinfo.fname[0]==0)
	{
		myfree(SRAMIN,fileinfo.lfname);
		return res;//读完了.
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

 //遍历文件
 //path:路径
 //返回值:执行结果
uint8_t mf_scan_files(uint8_t * path)
{
	FRESULT res;	  
    char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
 	fileinfo.lfsize = _MAX_LFN * 2 + 1;
	fileinfo.lfname = mymalloc(SRAMIN,fileinfo.lfsize);
#endif		  

    res = f_opendir(&dir,(const TCHAR*)path); //打开一个目录
    if (res == FR_OK) 
	{	
		p_info("\r\n"); 
		while(1)
		{
	        res = f_readdir(&dir, &fileinfo);                   //读取目录下的一个文件
	        if (res != FR_OK || fileinfo.fname[0] == 0) break;  //错误了/到末尾了,退出
	        //if (fileinfo.fname[0] == '.') continue;             //忽略上级目录
#if _USE_LFN
        	fn = *fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else							   
        	fn = fileinfo.fname;
#endif	                                              /* It is a file. */
			p_info("%s/", path);//打印路径	
			p_info("%s\r\n",  fn);//打印文件名	  
		} 
    }	  
	myfree(SRAMIN,fileinfo.lfname);
    return res;	  
}
//显示剩余容量
//drv:盘符
//返回值:剩余容量(字节)
uint32_t mf_showfree(uint8_t *drv)
{
	FATFS *fs1;
	uint8_t res;
    uint32_t fre_clust=0, fre_sect=0, tot_sect=0;
    //得到磁盘信息及空闲簇数量
    res = f_getfree((const TCHAR*)drv,(DWORD*)&fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect = (fs1->n_fatent - 2) * fs1->csize;//得到总扇区数
	    fre_sect = fre_clust * fs1->csize;			//得到空闲扇区数	   
#if _MAX_SS!=512
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif	  
		if(tot_sect<20480)//总容量小于10M
		{
		    /* Print free space in unit of KB (assuming 512 bytes/sector) */
		    p_info("\r\n磁盘总容量:%d KB\r\n"
		           "可用空间:%d KB\r\n",
		           tot_sect>>1,fre_sect>>1);
		}else
		{
		    /* Print free space in unit of KB (assuming 512 bytes/sector) */
		    p_info("\r\n磁盘总容量:%d MB\r\n"
		           "可用空间:%d MB\r\n",
		           tot_sect>>11,fre_sect>>11);
		}
	}
	return fre_sect;
}		    
//文件读写指针偏移
//offset:相对首地址的偏移量
//返回值:执行结果.
uint8_t mf_lseek(uint32_t offset)
{
	return f_lseek(file,offset);
}
//读取文件当前读写指针的位置.
//返回值:位置
uint32_t mf_tell(void)
{
	return f_tell(file);
}
//读取文件大小
//返回值:文件大小
uint32_t mf_size(void)
{
	return f_size(file);
} 
//创建目录
//pname:目录路径+名字
//返回值:执行结果
uint8_t mf_mkdir(uint8_t*pname)
{
	return f_mkdir((const TCHAR *)pname);
}
//格式化
//path:磁盘路径，比如"0:"、"1:"
//mode:模式
//au:簇大小
//返回值:执行结果
uint8_t mf_fmkfs(uint8_t* path,uint8_t mode,uint16_t au)
{
	return f_mkfs((const TCHAR*)path,mode,au);//格式化,drv:盘符;mode:模式;au:簇大小
} 
//删除文件/目录
//pname:文件/目录路径+名字
//返回值:执行结果
uint8_t mf_unlink(uint8_t *pname)
{
	return  f_unlink((const TCHAR *)pname);
}

//修改文件/目录名字(如果目录不同,还可以移动文件哦!)
//oldname:之前的名字
//newname:新名字
//返回值:执行结果
uint8_t mf_rename(uint8_t *oldname,uint8_t* newname)
{
	return  f_rename((const TCHAR *)oldname,(const TCHAR *)newname);
}
//获取盘符（磁盘名字）
//path:磁盘路径，比如"0:"、"1:"  
void mf_getlabel(uint8_t *path)
{
	uint8_t buf[20];
	uint32_t sn=0;
	uint8_t res;
	res=f_getlabel ((const TCHAR *)path,(TCHAR *)buf,(DWORD*)&sn);
	if(res==FR_OK)
	{
		p_info("\r\n磁盘%s 的盘符为:%s\r\n",path,buf);
		p_info("磁盘%s 的序列号:%X\r\n\r\n",path,sn); 
	}else p_info("\r\n获取失败，错误码:%X\r\n",res);
}
//设置盘符（磁盘名字），最长11个字符！！，支持数字和大写字母组合以及汉字等
//path:磁盘号+名字，比如"0:ALIENTEK"、"1:OPENEDV"  
void mf_setlabel(uint8_t *path)
{
	uint8_t res;
	res=f_setlabel ((const TCHAR *)path);
	if(res==FR_OK)
	{
		p_info("\r\n磁盘盘符设置成功:%s\r\n",path);
	}else p_info("\r\n磁盘盘符设置失败，错误码:%X\r\n",res);
} 

//从文件里面读取一段字符串
//size:要读取的长度
void mf_gets(uint16_t size)
{
 	TCHAR* rbuf;
	rbuf=f_gets((TCHAR*)fatbuf,size,file);
	if(*rbuf==0)return  ;//没有数据读到
	else
	{
		p_info("\r\nThe String Readed Is:%s\r\n",rbuf);  	  
	}			    	
}
//需要_USE_STRFUNC>=1
//写一个字符到文件
//c:要写入的字符
//返回值:执行结果
uint8_t mf_putc(uint8_t c)
{
	return f_putc((TCHAR)c,file);
}
//写字符串到文件
//c:要写入的字符串
//返回值:写入的字符串长度
uint8_t mf_puts(uint8_t*c)
{
	return f_puts((TCHAR*)c,file);
}













