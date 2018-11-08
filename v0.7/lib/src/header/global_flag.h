#ifndef __GLOBAL_FLAG_H__
#define __GLOBAL_FLAG_H__

#include "BTN_waitingTime.h"
#include "trackByID.h"
#include "MemoryModeling.h"

#define SECTOR_SIZE_BYTE 512

#define NUM_OF_CORE 1

#define CMD_BUS_TOTAL_BW 1.6 //in B/ns (400MHz - 32bit)
#define DATA_BUS_TOTAL_BW 1.6 //in B/ns (400MHz - 32bit)

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


#define DEV_AND_TIME "[" << sc_time_stamp() << " @ " << this->basename() << "]\t"

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
