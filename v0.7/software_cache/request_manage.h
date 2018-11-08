#ifndef _REQUEST_MANAGE_H_
#define _REQUEST_MANAGE_H_


#include "struct_sim_if.h"
#include "common_header.h"
#include "sfr_addr.h"
//note : uchar is used instead of "bool", partly coz "bool" already has a size of uchar

//host related

typedef struct sHostReq_t{

    HOST_REQ_OP op; 
    uint iAddr;
    uint iLen;

}hostReq_t;

extern hostReq_t hostReq;
static uchar hostReady = 0;


void insert_host(HOST_REQ_OP op, uint iAddr, uint iLen);

uchar req_exists();


   

//flush related


//some flush list


uchar flush_condition();
//int issue_flush();



#endif

