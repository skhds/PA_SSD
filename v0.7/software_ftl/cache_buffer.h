#ifndef __CACHE_BUFFER_H__
#define __CACHE_BUFFER_H__
#include "common_header.h"
#include "sfr_addr.h"
////////////////////////////////////////////////////////////
// if CACHE_LRU_POLICY is undefined, cache FIFO policy is used by default
#define CACHE_LRU_POLICY
///////////////////////////////////////////////////////////////

#define NOTHING -1

//#define ENTRY_SIZE (2048)
#define ENTRY_SIZE (1024)
#define HASH_MAX 8179

#define LIST_SIZE  (ENTRY_SIZE + 4) // entry + front + end
#define FREE_FRONT_NODE (LIST_SIZE-4)
#define FREE_BACK_NODE  (LIST_SIZE-3)

#define USED_FRONT_NODE (LIST_SIZE-2)
#define USED_BACK_NODE  (LIST_SIZE-1)


#define SECTOR_SIZE 512
#define SECTOR_PER_LINE 32

#define PAGE_SIZE (SECTOR_SIZE * SECTOR_PER_PAGE)





typedef struct NODE{
	uint id;
	struct NODE* next;
	struct NODE* prev;
	uint addr;
	uint bitmap; // sectir bit map
}NODE;

typedef struct HASH_NODE{
    uint addr;
    struct HASH_NODE* next;
    struct HASH_NODE* prev;
    struct NODE* link;
}HASH_NODE;

typedef enum{
	READ_DRAM = 0,
	WRITE_DRAM = 1,
	READ_SLAVE = 2,
	WRITE_SLAVE = 3,
	READ_BOTH = 4,
	UPDATE_SLAVE = 5 // read slave and modify with dram contents and re-write nand
}OP;

typedef struct
{
	OP op;
	uint slave_addr; // sector 林家? page 林家?
	uint slave_bitmap;
	uint dram_id;
	uint dram_bitmap;
}COMMAND;

void Write_Page(uint addr, uint offset, uint size);
void Read_Page(uint addr, uint offset, uint size);
void Cache_Buffer(uint addr, uint offset, uint size);
void Create_Command(OP op, uint slave_addr, uint slave_bitmap, uint dram_id, uint dram_bitmap);

void List_Push_Back(NODE* back, NODE* n);
void List_Push_Front(NODE* front, NODE* n);
uint List_Is_Empty(NODE* front);
NODE* List_Pop_Middle(NODE* evict_node);
NODE* List_Pop_Back(NODE* back);
NODE* List_Pop_Front(NODE* front);
NODE* List_Find_Node(NODE* front, uint addr);
void Hash_Push_Front(HASH_NODE* front, HASH_NODE* node);
HASH_NODE* Hash_Pop_Front(HASH_NODE* front);
HASH_NODE* Hash_Delete_Node(HASH_NODE* node);
HASH_NODE* Hash_Find_And_Delete_Node(HASH_NODE* front, uint addr);
HASH_NODE* Hash_Find_Node(HASH_NODE* front, uint addr);
void List_Init();



#endif
