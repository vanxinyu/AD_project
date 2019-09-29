#ifndef __FILE_H
#define	__FILE_H

#include "stm32f37x.h"

#define FAILE   0
#define SUCCESS 0

uint16_t file_init(void);

uint16_t file_creat(char*pathname,char* filename);

uint16_t file_read(char* filename);

uint16_t file_write(char* filename,u16 write_buf[1024]);

uint16_t file_deinit(void);


#endif /* __FILE_H */