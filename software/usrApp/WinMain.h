#ifndef __WINMAIN_H__
#define __WINMAIN_H__

#include "app.h"
#include "api.h"

typedef struct
{
	void (*init)(void *param);
	void (*run)(void *param);
} WinMainStruct;


extern WinMainStruct mainTask;

#endif
