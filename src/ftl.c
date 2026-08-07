#include "ftl.h"
#include "flash.h"
#include <limits.h>

#define GC_THRSH (((BLOCK_NUM - 4) * PAGE_PER_BLOCK) - 1) // 32640 - 1 = 32639

unsigned *l2p_table = NULL;
unsigned *p2l_table = NULL;

// Private Functions for this file only

static unsigned page_cnt = 0;
// In flash memory, you cannot write data wherever you want—you must write to pages sequentially (Page 0, then Page 1, then Page 2, etc.). page_cnt remembers the exact physical address of the next blank page on the drive.
static unsigned prog_page_cnt = 0;
// Tracks the total number of written pages on the entire drive, including both valid (good) data and invalid (overwritten/dead) data.
static unsigned erase_cnt[BLOCK_NUM] = {0};
// The Wear Leveling Tracker.
static unsigned block_cnt[BLOCK_NUM] = {0};
// Keeps track of how many pages are currently written inside a specific block.
INVALID_TABLE invalid_tbl[BLOCK_NUM];

static unsigned findYoungBlock(){
    unsigned min_cnt = UINT_MAX; // setting min_cnt to maximum value possible using limits.h
    unsigned i , block = 0;

    for (i = 0; i < BLOCK_NUM; i++) {
		if ((block_cnt[i] == 0) && (erase_cnt[i] < min_cnt)) {
			block = i;
			min_cnt = erase_cnt[i];
		}
	}
    // Finding the block which have minimum erase count and which is empty
	// TODO: check this block is empty or not
	assert(block_cnt[block] == 0);

	return block;
}

static unsigned findGCBlock(){
    unsigned max_invalid = 0;
    unsigned i =0;

    for (i = 1; i < BLOCK_NUM; i++){
        if (invalid_tbl[i].invalid_size > invalid_tbl[max_invalid].invalid_size){
            max_invalid = i;
        }
    }
    // 
    return max_invalid;
}

static void updatePageTable(unsigned lca){
    if ((page_cnt % PAGE_PER_BLOCK) == 0){
        page_cnt = findYoungBlock() * PAGE_PER_BLOCK;
    }
    l2p_table[lca] = page_cnt;
	p2l_table[page_cnt] = lca;
	page_cnt += 1;
}

static void doErase(unsigned block){
    eraseBlock(block); // Erase all content of block and fill it with 0xFF
    erase_cnt[block] += 1;
    prog_page_cnt -= PAGE_PER_BLOCK;
    block_cnt[block] -= PAGE_PER_BLOCK;

    invalid_tbl[block].invalid_size = 0;
	memset(invalid_tbl[block].invalid_page, 0x0, PAGE_PER_BLOCK * sizeof(unsigned char));
}

static void GC(unsigned src_block , unsigned tar_block){
    unsigned src_page = src_block * PAGE_PER_BLOCK;
    unsigned tar_page = tar_block * PAGE_PER_BLOCK;

    unsigned char data[PAGE_SIZE];
    unsigned lca, block;

    while(src_page < (src_block + 1) * PAGE_PER_BLOCK){
        if (invalid_tbl[src_block].invalid_page[src_page % PAGE_PER_BLOCK] == 0){
            readPage(src_page, data); // reading content of page from src_page
            progPage(tar_page, data); // writing content of page to tar_page
            prog_page_cnt += 1;

            block = tar_page / PAGE_PER_BLOCK;
            block_cnt[block] += 1;

            lca = p2l_table[src_page]; // finding local address of src_page
            l2p_table[lca] = tar_page; // mapping location of src_page with target page
            p2l_table[tar_page] = lca; 

            tar_page += 1;
            assert((tar_page % PAGE_PER_BLOCK) != 0);

        }
        src_page += 1;
    }

    doErase(src_block); // Erasing content of sorce block
	page_cnt = tar_page;
}

static void checkGC() {
	unsigned src, tar;
	while (prog_page_cnt > GC_THRSH) { // continue erasing untill value of used pages is greater than threashold of GC
		// src block: most invalid pages
		// tar block: youngest
		src = findGCBlock();
		tar = findYoungBlock();

		GC(src, tar);
	}
}

// Public Functions

void initFTL(){
    l2p_table = (unsigned *)malloc(sizeof(unsigned) * PAGE_PER_BLOCK * USER_BLOCK_NUM);
    p2l_table = (unsigned *)malloc(sizeof(unsigned) * PAGE_PER_BLOCK * BLOCK_NUM);

    memset(l2p_table, 0xFF , sizeof(unsigned)* PAGE_PER_BLOCK * USER_BLOCK_NUM);
    memset(p2l_table, 0xFF , sizeof(unsigned) * PAGE_PER_BLOCK * BLOCK_NUM);
    memset(&invalid_tbl, 0, sizeof(INVALID_TABLE));
}

void freeFTL() { // Free memory space acquired by l2p_table and p2l_table 
    free(l2p_table);
    free(p2l_table);
}



