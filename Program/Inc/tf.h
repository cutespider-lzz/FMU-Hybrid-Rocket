#ifndef __TF_H
#define __TF_H

#include "cmsis_os.h"
#include "fatfs.h"

extern uint8_t FileName[];
extern uint8_t StorageBuff[];
extern FRESULT SDRet;

void FileCreate(void);
void FileClose(void);

#endif
