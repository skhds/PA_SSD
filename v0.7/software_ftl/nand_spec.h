#ifndef __NAND_SPEC_H__
#define __NAND_SPEC_H__


#define NAND_ARRAY_CHANNEL				4
#define NAND_ARRAY_WAY					4
#define NAND_PAGE_BYTE					16384
#define NAND_PAGE_PER_BLOCK				64
#define NAND_BLOCK_PER_PLANE			1024
#define NAND_PLANE_PER_WAY				1

/*
#define NAND_ARRAY_CHANNEL				1
#define NAND_ARRAY_WAY						1
#define NAND_PAGE_BYTE						32768
#define NAND_PAGE_PER_BLOCK				128
#define NAND_BLOCK_PER_PLANE			4096
#define NAND_PLANE_PER_WAY				1
*/

#define NAND_ARRAY_TOTAL_CAP			((uint64)NAND_ARRAY_CHANNEL * NAND_ARRAY_WAY * NAND_PLANE_PER_WAY * NAND_BLOCK_PER_PLANE * NAND_PAGE_PER_BLOCK * NAND_PAGE_BYTE)

#define NAND_TIMING_TPROG					((double)1500000000)
#define NAND_TIMING_TR						((double)75000000)
#define NAND_TIMING_TWC						((double)2500)
#define NAND_TIMING_TRC						((double)2500)
#define NAND_TIMING_TBER					((double)3000000000)

//////////////////

#endif
