#ifndef FLASH_H  /*Uses because if this file is already included in code it should not include again*/
#define FLASH_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h> /*for assert The program instantly aborts and prints a detailed error message to the console showing exactly which file and line number caused the crash.*/

#define BLOCK_NUM  (1024)
#define PAGE_PER_BLOCK (32)
#define INVALID_PAGE (0xFFFFFFFF)

// Total pages = 1024 * 32 = 32768 
#define PAGE_SIZE (4096)
#define SECTOR_SIZE (512)
#define SECTOR_PER_PAGE (PAGE_SIZE / SECTOR_SIZE)

#define USER_BLOCK_NUM (1000)
#define USER_SECTOR_NUM (USER_BLOCK_NUM * PAGE_PER_BLOCK * SECTOR_PER_PAGE) // 256000
//If USER_BLOCK_NUM were equal to BLOCK_NUM (1024), and the user filled 100% of the drive, there would be zero clean blocks left to copy valid data into during GC. The drive would instantly lock up.

extern unsigned char*** flash_arr;

void initFlash();
void freeFlash();
void eraseBlock(unsigned int block);
void progPage(unsigned p_adr , unsigned char* data);
void readPage(unsigned p_adr , unsigned char* data); //The caller (the function asking for the data) creates an empty buffer (a container) of the correct size where it can pass output.


#endif
