#ifndef __GLOBAL_FLAG_H__
#define __GLOBAL_FLAG_H__

#include "etc/etc_main.h"
#include "utils/utils_main.h"
#include "structs/structs_main.h"
#include "specs/specs_main.h"
#include "addr/addr_main.h"




#define HOSTIF_DEBUG 0 
#define DEVIF_DEBUG 0
#define SUB_DEBUG 0
#define DRAM_DEBUG 0
#define NAND_DEBUG 0
#define NANDCtrl_DEBUG 0
#define IRQ_DEBUG 0
//#define CACHE_BUFFER_METHOD 0
//#define G_INITIALIZE 0

//in entries
#define SIZE_BUF_CACHE 64
#define SIZE_BUF_EVICT 64



long long int NAND_STATS[13][4][4][1024] = {0}; //12 : # of nand opcodes, 4 : # channel, 4 : # way, 1024 : # of blocks
long long int DRAM_STATS[CacheStatMAX][CacheTypeMAX][cacheOpMAX] = {0}; //2 : Cache Buffer/Metadata, 2: Write/Read

SSDmetric::trackByID testTrack("../results/test_result.txt");//skhds

unsigned int buffer_write_count = 0;
unsigned int meta_write_count;
unsigned int NAND_write_count;
unsigned int cache_bypass_count = 0;
unsigned int total_req_count = 0;

double CMD_BUS_TIME_CONSUMED[3] = {0};
double DATA_BUS_TIME_CONSUMED[3] = {0};

double SOFTWARE_TIME[2] = {0};
static double SWtime_prev = 0;
long long int SOFTWARE_count = 0;
bool SOFTWARE_FLAG = false;

bool        g_isFtlReady = false;
bool        g_isCacheReady = false;

bool        g_initialize_end = false;



#endif
