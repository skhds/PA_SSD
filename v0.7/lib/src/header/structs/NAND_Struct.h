#ifndef _NAND_STRUCT_H_
#define _NAND_STRUCT_H_

#include "ssd_struct.h"






typedef enum NAND_CMD_SET
{
	Nothing             = 0,
	Program             = 1,
	Read                = 2,
	Erase               = 3,
	Copyback            = 4,
	Copyback_Program    = 5,
	Copyback_Read       = 6,

    MapTable_Read       = 7, // iAddr1: nand // iAddr2: memory // iAddr3: offset (byte) // iAddr4: length (byte)
    MapTable_Program    = 8,
    MapTable_Copyback   = 9,
    MapTable_Copyback_Pgm = 10,
    MapTable_Copyback_Rd = 11,
    MapTable_Erase = 12,

	Waiting             = 100,

	Chip_Select         = 99 // Added by jun
}NAND_CMD_SET;


typedef struct NAND_Cmd{
    NAND_CMD_SET opCode; 
    unsigned int iAddr1; //PBA
    unsigned int iAddr2; //nothing
    unsigned int iAddr3; //LBA
    unsigned int iAddr4; //len
}NAND_Cmd;

typedef struct NAND_Cmd_Buffer{
    NAND_Cmd        cmd;
    unsigned int    DRAM_id;
}NAND_Cmd_Buffer;

typedef enum eDATA_STATE{

    BUFFER_FREE = 0,
    OCCUPIED = 1

}DATA_STATE;

typedef struct sDataBufferEntry{
    
    unsigned char data[16384];
    DATA_STATE state;
    unsigned int    DRAM_id;

}NANDData_t;


typedef enum{

    DONE = 0,
    IN_TRANSACTION = 1

}eIRQState;

typedef struct sIRQReq_t{

    unsigned int id;
    unsigned int addr;
    eIRQState state;

}IRQReq_t;

typedef enum sReqHOSTState{
    REQ_EMPTY = 0,
    NOT_READY = 1,
    REQ_READY = 2
}reqHOSTState;

typedef struct sDirectNANDReq{
    reqHOSTState state;
    unsigned int id;
    unsigned int addr;
    

}directNANDReq_t;


#endif
