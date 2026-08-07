#include <stdio.h>

#include "ftl.h"
#include "flash.h"

static void randGenData(unsigned char* data, unsigned sz){
    unsigned i;
    for (i = 0; i < sz ; i++){
        data[i] = rand() % 256;
    }
}

static void readWholeDisk(unsigned char* data){
    // read all data in each sector 
    unsigned lba = 0;
    unsigned sec_cnt;

    while(lba < USER_SECTOR_NUM){
        sec_cnt = (rand() % 32) + 1;
        //selecting random number of sectors to copy 
        if (sec_cnt > USER_SECTOR_NUM - lba){
            sec_cnt =USER_SECTOR_NUM - lba;
        }
        readData(lba, sec_cnt , data + lba * SECTOR_SIZE);
        lba += sec_cnt;
    }
}

int main(void){
    srand(2);
    unsigned char* data = (unsigned char*)calloc(USER_SECTOR_NUM * SECTOR_SIZE, sizeof(unsigned char));
    // malloc doesnt clear memory calloc clears memory calloc require 2 arguments (Number of Elements, Size of Element)
    unsigned char* mirror_data = (unsigned char*)calloc(USER_SECTOR_NUM * SECTOR_SIZE, sizeof(unsigned char));
    unsigned int i;
    unsigned test_round;

    unsigned lba, sec_cnt;
    int result;
    




}