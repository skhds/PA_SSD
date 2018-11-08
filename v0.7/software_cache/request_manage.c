#include "request_manage.h"

hostReq_t hostReq;


//host related

uchar req_exists(){
    
    return hostReady;
}

void insert_host(HOST_REQ_OP op, uint iAddr, uint iLen){

    assert(!req_exists());
    hostReq.op = op;
    hostReq.iAddr = iAddr;
    hostReq.iLen = iLen;
    hostReady = 1;

}

void get_host(HOST_REQ_OP* op, uint* iAddr, uint* iLen){

    assert(req_exists());
    *op = hostReq.op;
    *iAddr = hostReq.iAddr;
    *iLen = hostReq.iLen;

}

uchar can_issue_write(){
/*
    if(req_exists()){
        int req_count = used_count + evict_count + flush_count;
        assert(req_count > ENTRYSIZE);
        return (req_count <= ENTRY_SIZE);     
    }
  */  return 0;
}

void pop_host(uint bypass){

    assert(req_exists());
    hostReady = 0;
    *((vuint *)((_ADDR_SUB_REQ_BASE_ + _OFFSET_SUB_REQ_CACHE_DATA_) + _OFFSET_DONE_)) = bypass;

}



//flush related
//
//
//


/*int issue_flush(){

    int count;

    do{

   
        count++;
    
    }while(1);
    
    
    return count;


}*/

