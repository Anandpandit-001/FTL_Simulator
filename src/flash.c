#include "flash.h"

unsigned char*** flash_arr = NULL;

void initFlash(){ // This function is for memory allocation of flash array 
    flash_arr = (unsigned char***)malloc(BLOCK_NUM * sizeof(unsigned char**));
    for(int i = 0; i < BLOCK_NUM ; i++){
        flash_arr[i] = (unsigned char**)malloc(PAGE_PER_BLOCK * sizeof(unsigned char**));

        for (int j = 0; j <PAGE_PER_BLOCK; j++){
            flash_arr[i][j] = (unsigned char*)malloc(PAGE_SIZE * sizeof(unsigned char));
            assert(flash_arr[i][j] != NULL);
            memset(flash_arr[i][j] , 0xFF, PAGE_SIZE);
        }

    }
};

void freeFlash(){ // this function completely removes alloted memory to above flash array
    unsigned i , j;
    for (i = 0; i<BLOCK_NUM ; i++){
        for (j=0 ; j < PAGE_PER_BLOCK; j++){
            free(flash_arr[i][j]);
        }
    }
    for(i = 0; i<BLOCK_NUM ; i++){
        free(flash_arr[i]);
    }
    free(flash_arr);
};

void eraseBlock(unsigned block){ // this function rewrites every value in page with 0xFF 
    for (unsigned page = 0; page < PAGE_PER_BLOCK;page++){
        memset(flash_arr[block][page], 0xFF, PAGE_SIZE);
    }
};

void progPage(unsigned p_adr, unsigned char* data){ // Write provided data in data parameter into designated page 
    unsigned block = p_adr / PAGE_PER_BLOCK;
    unsigned page = p_adr % PAGE_PER_BLOCK;

    memcpy(flash_arr[block][page], data,PAGE_SIZE);
};

void readPage(unsigned p_adr , unsigned char* data){ // read the data from designated page and copy it in data variable provide as parameter
    unsigned block = p_adr / PAGE_PER_BLOCK;
    unsigned page = p_adr % PAGE_PER_BLOCK;

    memcpy(data , flash_arr[block][page], PAGE_SIZE);

}
