#include "nand_cmd_layer.h"
#include "device_if.h"
#include "cache_buffer.h"
#include <stdio.h>

#include "compile_flag.h"
#include "nand_cmd_layer.h"
#include "ftl.h"
#include "sim_if.h"

#ifdef _MCSSD_ARM_BARE_METAL_

extern DiskReq g_curReq;

int transIrq[20] = {-1, -1, 0, -1, -1, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1};


uint transIrqToCh(uint irqType)
{
    /*
    uint ch;
    if(irqType == 2){
        ch = 0;
    }else if(irqType == 5){
        ch = 1;
    }else if(irqType == 6){
        ch = 2;
    }else if(irqType == 7){
        ch = 3;
    }else if(irqType == 8){
        ch = 4;
    }else if(irqType == 9){
        ch = 5;
    }else if(irqType == 10){
        ch = 6;
    }else if(irqType == 11){
        ch = 7;
    }else if(irqType == 12){
        ch = 8;
    }else if(irqType == 13){
        ch = 9;
    }

    return ch;
    */
    return transIrq[irqType];
}


void selectTask(uint irqType);

void testLowNandSpec(sNandSpec *spec){

	if (
		spec->iChannelCount == _LLN_NUM_CHANNEL_		&&
		spec->iWayCount == _LLN_NUM_WAY_						&&
		spec->iPageSize == _LLN_PAGE_DATE_SIZE_			&&
		spec->iPagePerBlock == _LLN_PAGE_PER_BLOCK_	&&
		spec->iBlockPerPlane == _LLN_BLOCK_PER_WAY_ &&		// LLN does not have concept of plane
		spec->iChannel_Bitwidth <= _LLN_CHANNEL_PER_SSD_BIT_		&&
		spec->iWay_Bitwidth <= _LLN_WAY_PER_CHANNEL_BIT_				&&
		spec->iPageSize_Bitwidth <= (_LLN_SECTOR_PER_PAGE_BIT_ + _LLN_BYTE_PER_SECTOR_BIT_) &&
		spec->iPagePerBlock_Bitwidth <= _LLN_PAGE_PER_BLOCK_BIT_	&&
		spec->iBlockPerPlane_Bitwidth <= _LLN_BLOCK_PER_WAY_BIT_
		)
	{
		return;
	}

	//barePrintf(" ### Do not match low-level NAND spec and FTL nand spec\n");
	exit(1);

}

sLowNandCmd toLlnCmd(sNandCmd cmd){

	sLowNandCmd retCmd;

	retCmd.opCode = convertOp(cmd.op);
	retCmd.iAddr1 = convertAddr(cmd.dest);
	
	if (cmd.src.isValid == UCHAR_TRUE)
		retCmd.iAddr2 = convertAddr(cmd.src);


    if(cmd.op == NAND_CMD_MAP_READ ||
            cmd.op == NAND_CMD_MAP_PROGRAM)
    {
        ftlAssert(cmd.dataBuffer != NULL, " ### FTL error: data buffer is NULL!!");
        retCmd.iAddr2 = (uint)cmd.dataBuffer;
        retCmd.iAddr3 = logical_addr;
        retCmd.iAddr4 = NAND_PAGE_BYTE;
    }
    else{
        //added bitmap
        retCmd.iAddr3 = *((uint*)cmd.cpValidMap);
    }


	return retCmd;
}

eLowNandOp convertOp(eNandOp op){

	switch (op)
	{
	case NAND_CMD_PROGRAM:
		return LLN_PROGRAM;
		break;
	case NAND_CMD_READ:
		return LLN_READ;
		break;
	case NAND_CMD_COPYBACK:
		return LLN_COPYBACK;
		break;
	case NAND_CMD_COPYBACK_W:
		return LLN_COPYBACK_PGM;
		break;
	case NAND_CMD_COPYBACK_R:
		return LLN_COPYBACK_RD;
		break;
	case NAND_CMD_ERASE:
		return LLN_ERASE;
		break;
	case NAND_CMD_RNM_READ:
		//return LLN_READ;
		//// temporarily RNM -> COPYBACK read... this should be modified.
		// update : hotfixed by making another LLN command
        
        return LLN_RNM_READ;
		break;
//////// map table operations   ///////////////////////////
    case NAND_CMD_MAP_READ:
        return LLN_MAP_READ;
        break;
    case NAND_CMD_MAP_PROGRAM:
        return LLN_MAP_PROGRAM;
        break;
    case NAND_CMD_MAP_COPYBACK:
        return LLN_MAP_COPYBACK;
        break;
    case NAND_CMD_MAP_COPYBACK_W:
        return LLN_MAP_COPYBACK_PGM;
        break;
    case NAND_CMD_MAP_COPYBACK_R:
        return LLN_MAP_COPYBACK_RD;
        break;
    case NAND_CMD_MAP_ERASE:
		return LLN_MAP_ERASE;
        break;
///////////////////////////////////////////////////////////
    default:
		barePrintf(" ### Weird NAND opCode from FTL\n");
		exit(1);
		break;
	}
}

uint convertAddr(sNandAddr addr){

	uint iRetAddr = 0;

//	if (addr.isValid != UCHAR_TRUE){
		//barePrintf(" ### Income NAND address is not valid.\n");
		//exit(1);
//	}

	/// currently nand does not support plane, thus omit plane address.

	iRetAddr += (addr.iChannel << _LLN_BIT_FOR_CHANNEL_);
	iRetAddr += (addr.iWay << _LLN_BIT_FOR_WAY_);
	iRetAddr += (addr.iBlk << _LLN_BIT_FOR_BLOCK_);
	iRetAddr += (addr.iPage << _LLN_BIT_FOR_PAGE_);

	return iRetAddr;
}

void regLlnCmd(sLowNandCmd cmd, uint logicalAddr){
	/// register cmd to nand manager

    uint numCh = logicalAddr & (NAND_ARRAY_CHANNEL - 1);
	

    
    *((vuint *)(_ADDR_NAND_MAN_BASE_ + _OFFSET_R_BUS_NAND_CMD_)) = cmd.opCode;
	*((vuint *)(_ADDR_NAND_MAN_BASE_ + _OFFSET_R_BUS_NAND_ADDR1_)) = cmd.iAddr1;
	*((vuint *)(_ADDR_NAND_MAN_BASE_ + _OFFSET_R_BUS_NAND_ADDR2_)) = cmd.iAddr2;
	*((vuint *)(_ADDR_NAND_MAN_BASE_ + _OFFSET_R_BUS_NAND_ADDR3_)) = cmd.iAddr3; 
    *((vuint *)(_ADDR_NAND_MAN_BASE_ + _OFFSET_R_BUS_NAND_ADDR4_)) = cmd.iAddr4;

}

void activateNand(uchar cIsLast, uint logicalAddr){

	unsigned int iVal = 1;

    uint numCh = logicalAddr & (NAND_ARRAY_CHANNEL - 1);

	if (cIsLast == UCHAR_TRUE)
		iVal += 1 << 1;

	//*((vuint *)(ADDR_NAND_MAN_BASE[numCh] + _OFFSEt_R_BUS_INT_)) = iVal;
}


#define setFtlComplete(x) (*((vuint *)(ADDR_NAND_MAN_BASE[x] + _OFFSEt_R_BUS_INT_)) = 0x2)
/*
void setFtlComplete(uint ch){
    //barePrintf("Complete ch = %d\n", ch);
	*((vuint *)(ADDR_NAND_MAN_BASE[ch] + _OFFSEt_R_BUS_INT_)) = 0x2;
}
*/

#define setFtlReady() (*((vuint *)(_ADDR_NAND_MAN_BASE_ + _OFFSEt_R_BUS_INT_)) = (0x1 << 2))

/*
void setFtlReady(){
	*((vuint *)(_ADDR_NAND_MAN_BASE_ + _OFFSEt_R_BUS_INT_)) = (0x1 << 2);
	
}
*/

void waitLln(uint logicalAddr){
	/// wait low level nand complete
	unsigned int iReadVal;
    uint numCh = logicalAddr & (NAND_ARRAY_CHANNEL - 1);
	do{
		iReadVal = *((vuint *)(ADDR_NAND_MAN_BASE[numCh] + _OFFSET_R_BUS_NAND_CMD_STATUS_));
	} while (iReadVal != 0x0);

}

enum irqFunctionType
{
    IRQ_UNKNOWN = 0,
    IRQ_DEVICE_INTERFACE = 1,
    IRQ_SUB_REQUEST_MANAGER = 2,
    IRQ_FTL = 3,
    IRQ_CACHE_BUFFER_CONTROLLER = 4,
    IRQ_INITIALIZATION = 5
};

int irqFunction[20] = {
    IRQ_DEVICE_INTERFACE, // 0
    IRQ_SUB_REQUEST_MANAGER, // 1
    IRQ_FTL, // 2
    IRQ_FTL, // 3
    IRQ_CACHE_BUFFER_CONTROLLER, // 4
    IRQ_FTL, // 5
    IRQ_FTL, // 6
    IRQ_FTL, // 7
    IRQ_FTL, // 8
    IRQ_FTL, // 9
    IRQ_FTL, // 10
    IRQ_FTL, // 11
    IRQ_FTL, // 12
    IRQ_FTL, // 13
    IRQ_INITIALIZATION}; // 14


    
    
    

void selectTask(uint irqType)
{

    SWPointer = SWPointer << 1;
    while (!(irqType & SWPointer & SWFlag)){
        SWPointer = SWPointer << 1;

        if (SWPointer == 32){
            SWPointer = 0x1;
        }
    }
}

#endif
