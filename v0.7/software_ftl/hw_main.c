#include "compile_flag.h"
#include "bare_utils.h"
#include "cache_buffer.h"
#include "struct_sim_if.h"
#include "time.h"

#ifdef _MCSSD_ARM_BARE_METAL_


void sleepcp(int nanosec){
    clock_t time_end;
    time_end = clock()+nanosec*CLOCKS_PER_SEC/100000000;
    while(clock()<time_end){}
}

int main()
{
	sHeap new_heap;
    
    uint op, addr, length, dram_addr, dram_length, id;
    uint i;
    uint isNANDfull;


    DiskReq request;
    //heap initialization
	new_heap.base = _ADDR_HEAP_BASE_;
	new_heap.range = _ADDR_HEAP_LIMIT_;
	
    _init_alloc(new_heap.base, new_heap.range);
    
    barePrintf("1Ftl software initialize start\n");

    initFtl();
    dram_count = 0;

    barePrintf("Ftl software initialize completed\n");
    *((vuint *)(_ADDR_SUB_REQ_BASE_ + _OFFSET_SUB_REQ_COMPLETE_)) = 0x3;
    //sleepcp(1000000000);
    while(1){
 /*       //barePrintf("CPU2 recycle");
        sleepcp(1000);
        //barePrintf("CPU2 call : %d\n", temp_count++);
        op          =*((vuint *)((_ADDR_SUB_REQ_BASE_ + _OFFSET_CPU2_ ) + _OFFSET_CPU2_OP_ ));
        isNANDfull  =*((vuint *)((_ADDR_NAND_MAN_BASE_ + _OFFSET_CPU2_) + _OFFSET_R_BUS_NAND_FULL));

        if (op == 2 && isNANDfull ) continue;
        addr        =*((vuint *)((_ADDR_SUB_REQ_BASE_ + _OFFSET_CPU2_ ) + _OFFSET_CPU2_ADDR_ ));
        length      =*((vuint *)((_ADDR_SUB_REQ_BASE_ + _OFFSET_CPU2_ ) + _OFFSET_CPU2_LENGTH_ ));
        DramInfo[dram_count].dram_addr   =*((vuint *)((_ADDR_SUB_REQ_BASE_ + _OFFSET_CPU2_ ) + _OFFSET_CPU2_DRAM_ADDR_ ));
        DramInfo[dram_count].dram_length =*((vuint *)((_ADDR_SUB_REQ_BASE_ + _OFFSET_CPU2_ ) + _OFFSET_CPU2_DRAM_LENGTH_ ));
        DramInfo[dram_count].id          =*((vuint *)((_ADDR_SUB_REQ_BASE_ + _OFFSET_CPU2_ ) + _OFFSET_CPU2_ID_ ));
        request.sectorAddr = addr;
        logical_addr = addr; // maintain the logical addr information to send NAND_Manager
        request.sectorCount = length;
        logical_length = length; // maintain the logical length information to send NAND_Manager
        if(op==0){
            request.cmd = DISK_CMD_WRITE;
        }
        else if(op==1){
            request.cmd = DISK_CMD_READ;
        }
        request.li_issuedTick= 0;
        request.dIssueTime = 0;

        if(op!=2){
        
        //barePrintf("[CPU 2] Start %d\t%d\t%d\t%d\t%d\t%d\n", op, addr, length, DramInfo[dram_count].dram_addr, DramInfo[dram_count].dram_length, DramInfo[dram_count].id);
            callFtl(request);
            //
            //barePrintf("[CPU 2] %d\t%d\t%d\t%d\t%d\t%d\n", op, addr, length, dram_addr, dram_length);
            //barePrintf("[CPU 2] Ftl is operated\n");
            // notify Ftl is completed
            //*((vuint *)(_ADDR_NAND_MAN_BASE_ + _OFFSET_FTL_COMPLETE_))=0x1;
        }
        else{
            //barePrintf("this case is not allowed : No reqeuest to deal with ftl\n ");
        }
        dram_count++;
        dram_count=0;
        */
    }

	return 0;
}

void irqHandler(){

    //temp
    DiskReq request;
    uint dram_id; //dram node id //declared as global
    uint addr; //lpa
    uint bitmap; //bitmap

 //   barePrintf("[CPU2] Received IRQ!\n");
    dram_id = *((vuint*)(_ADDR_NAND_MAN_BASE_ + _ADDR_CPU2_BASE_ + _OFFSET_IRQ_BASE_ + _OFFSET_IRQ_ID_));
    addr = *((vuint*)(_ADDR_NAND_MAN_BASE_ + _ADDR_CPU2_BASE_ + _OFFSET_IRQ_BASE_ + _OFFSET_IRQ_ADDR_));
    bitmap = *((vuint*)(_ADDR_NAND_MAN_BASE_ + _ADDR_CPU2_BASE_ + _OFFSET_IRQ_BASE_ + _OFFSET_IRQ_BITMAP_));
    
    
    
    request.sectorAddr = addr*32;
    request.bitmap = bitmap;
    //request.sectorCount = 32;
    request.cmd = DISK_CMD_WRITE;
    //barePrintf("[CPU2] Eviction Request : id - %d, addr - %d, bitmap - %d\n", dram_id, addr, bitmap);
    
    callFtl(request);
    
    (*((vuint*)(_ADDR_NAND_MAN_BASE_ + _OFFSET_R_BUS_REQ_ID_))) = dram_id;

}   
#endif
