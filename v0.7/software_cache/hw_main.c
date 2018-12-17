#include "compile_flag.h"
#include "bare_utils.h"
#include "cache_buffer.h"
#include "struct_sim_if.h"
#include "request_manage.h"

#ifdef _MCSSD_ARM_BARE_METAL_




typedef volatile Req_t vReq;
void sleepcp(int nanosec){
    clock_t time_end;
    time_end = clock()+nanosec*CLOCKS_PER_SEC/100000000;
    while(clock()<time_end){}
}

int main()
{
	sHeap new_heap;

    //Req_t temp_Req;
    uint i;
    uint addr, op, ilen;
    uint count;
    uint size;
    uint dst=10;
    uint bypass;

    //heap initialization
	new_heap.base = _ADDR_HEAP_BASE_;
	new_heap.range = _ADDR_HEAP_LIMIT_;

	_init_alloc(new_heap.base, new_heap.range);
    
    barePrintf("Cache Software intialize start\n");
    List_Init();
    *((vuint *)(_ADDR_SUB_REQ_BASE_ + _OFFSET_SUB_REQ_COMPLETE_)) = 0x2;
    barePrintf("Cache Software intialize completed\n");
    //sleepcp(1000000000);
  
    while(1){
        
        if(req_exists() && !Cache_Buffer_Full()){
           
            bypass = bypass_condition();
            get_host(&op, &addr, &ilen); 
            //barePrintf("[CPU1] executing req : op = %d, addr = %d, length = %d\n", op, addr, ilen);
            //barePrintf("[CPU1] counts : free = %d, used = %d, evict = %d\n", ENTRY_SIZE - evict_count - used_count, used_count, evict_count);
            if(!bypass){ //write to cache
                Cache_Buffer(addr, ilen, op); 
            }
            else{ //flush to NAND
                direct_NAND_write(addr, ilen, op); 
            }

            pop_host(bypass);

        }


        //issue flushes in the flush queue
        if(flush_condition()){
           
            issue_flush();

        }
/*
        if(!EvictDoneEmpty()){
            Free_Evict_Nodes();
            //Print_Node_Counts();
        }
*/
        sleepcp(1000);

    }

    return 0;
}

void irqHandler(){



    uint irqType = (0xff & *((vuint*)(_ADDR_INTCTRL_BASE_ + _OFFSET_INT_SOURCE_)));

    int op;
    int addr;
    int length;
    uint id;
    Req_t tmpReq;

    //barePrintf("[CPU1] IRQ invoked! type : %d\n", irqType);
    switch (irqType)
    {
        case 0:		//Device interface

            //barePrintf("Device interface scheduling start\n");
            //barePrintf("1: Software scheduling start!\n");

            op = *((vuint *)(_ADDR_DEV_IF_BASE_ + 0));
            addr = *((vuint *)(_ADDR_DEV_IF_BASE_ + 4));
            length = *((vuint *)(_ADDR_DEV_IF_BASE_ + 8));
            //barePrintf("[CPU1] req info : op = %d, addr = %d, length = %d\n", op, addr, length);
            tmpReq.Op = op; 
            tmpReq.iAddr = addr; 
            tmpReq.iLen = length; 
            insert_host(op, addr, length); 

            *((vuint *)(_ADDR_INTCTRL_BASE_ + _OFFSET_DEVICEIF_COMPLETE_)) = 0x1;

            //barePrintf("Device interface scheduling end\n");
            break;

        case 1: //Flush Complete
            

            id = *((vuint *)(_ADDR_NAND_MAN_BASE_ + _ADDR_CPU1_BASE_));
            Insert_EvictDone(id); //these two will evict every possible node 
            Free_Evict_Nodes();   //i'll change this to be more convenient
            *((vuint *)(_ADDR_INTCTRL_BASE_ + _OFFSET_NAND_COMPLETE_)) = 0x1;
            break;
        default : 
            break;




    }
    //barePrintf("[CPU1] IRQ done\n");
    return;
}


#endif
