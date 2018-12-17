#ifndef __CONFIGURABLES_H__
#define __CONFIGIRABLES_H__

/*
 * the following defines are for configurable parameters
 * that is, the values can change, according to the needs
 */





/* Simulation Configurations */



#define HOSTIF_DEBUG    0 
#define DEVIF_DEBUG     0
#define SUB_DEBUG       0
#define DRAM_DEBUG      0
#define NAND_DEBUG      0
#define NANDCtrl_DEBUG  0
#define IRQ_DEBUG       0
//#define CACHE_BUFFER_METHOD 0
//#define G_INITIALIZE 

//#define DATA_COMPARE_ON
#define TRACK_ID_ON




/* NAND parameters*/


#define SECTOR_BYTES    512
#define SECTOR_PER_PAGE 32
#define PAGE_PER_BLOCK  64
#define BLOCK_PER_WAY   1024
#define WAY_PER_CHANNEL 4
#define CHANNEL_PER_SSD 4

#define PAGE_BYTES      SECTOR_BYTES*SECTOR_PER_PAGE
#define SSD_CAPACITY    (unsigned long long)PAGE_BYTES * PAGE_PER_BLOCK * BLOCK_PER_WAY  \
                        * WAY_PER_CHANNEL * CHANNEL_PER_SSD


/* RAM parameters */

#define DRAM_ACCESS_BYTE    32 //in B
#define DRAM_READ_LATENCY   45 // in ns (DRAM : 45 ns, PRAM : 100 ns)
#define DRAM_WRITE_LATENCY  45 // in ns (DRAM : 45 ns, PRAM : 100 ns)
#define PRAM_READ_LATENCY   100 // in ns (DRAM : 45 ns, PRAM : 500 ns)
#define PRAM_WRITE_LATENCY  250 // in ns (DRAM : 45 ns, PRAM : 500 ns)



/* Global defines */



#define DATA_BUS_TOTAL_BW 1.6
#define CMD_BUS_TOTAL_BW  1.6

#define CLOCK_PERIOD 2.5 //400MHz



/* HOST defines */

#define IF_BANDWIDTH    768  //768MB/s (6Gbps) : SATA 3
#define MAX_REQ_LEN     1024 //1MB
#define Q_SIZE          32

#define MAX_ADDR (SSD_CAPACITY) / SECTOR_BYTES 
#define XSTR(x) STR(x)
#define STR(x) #x
#pragma message "the value : " XSTR(MAX_ADDR)

/* SubReq defines */

#define SUBREQ_QUEUE_SIZE   128
#define DATA_QUEUE_SIZE     4


/* DRAM defines */

#define DRAM_ENTRY_SIZE 64

#define SIZE_BUF_CACHE 64
#define SIZE_BUF_EVICT 64
#define SIZE_BUF_ENTRY SIZE_BUF_CACHE + SIZE_BUF_EVICT

/* NAND defines */

#define NAND_CMD_QUEUE_SIZE  16
#define NAND_BUF_ENTRY       8

#endif
