//--------------------------------------------------------------                //%
// Important:  The template for this file was generated                         //%
//  automatically and can only be re-generated if rules                         //%
//  are followed when editing:                                                  //%
// 1. Do not modify any line containing //%                                     //%
// 2. Only insert your lines of code between 2 lines                            //%
//    containing "//% user begin" and "//% user end"                            //%
//                                                                              //%
// :generate:date: Thu Jan 09 2014 18:03:58                                     //%
// :generate:version: 2010.1.0-dev                                              //%
//--------------------------------------------------------------                //%
                                                                                //%
#ifndef __SUBREQMAN_H__                                                         //%
#define __SUBREQMAN_H__                                                         //%
                                                                                //%USERBEGIN MODEL_DEBUG_ENABLE
#if defined(DEBUG) || defined(DBG_SCP)
# define DBG_SCP_SUBREQMAN
#endif
                                                                                //%USEREND MODEL_DEBUG_ENABLE
#define SC_INCLUDE_DYNAMIC_PROCESSES                                            //%
                                                                                //%
#include <scml2.h>                                                              //%
#include <systemc>                                                              //%
#include "scmlinc/scml_property.h"                                              //%
#include "scmlinc/scml_command_processor.h"                                     //%
#include "tlm_utils/simple_target_socket.h"                                     //%
#include "tlm.h"                                                                //%
                                                                                //%USERBEGIN HEADER_H
// TODO: Insert your includes, declarations, etc. here.
#include "./header/SubReq_def.h"
#include "./header/global_flag.h"
#include "./header/buffer_ctrl.h"
#include <list>
#include <cmath>
#include <iostream>

#define CPU2_BASE 0x100

ofstream Subfile;
ofstream Listfile;

using namespace std;

                                                                                //%USEREND HEADER_H
                                                                                //%
//------------------------                                                      //%
// Module Interface                                                             //%
//------------------------                                                      //%
class SubReqMan :                                                               //%
	public sc_core::sc_module                                               //%
                                                                                //%USERBEGIN INHERITANCE
    // TODO: Specify class inheritance here. Start each line with a , character

                                                                                //%USEREND INHERITANCE
                                                                                //%
{                                                                               //%
public:                                                                         //%
	// constructor                                                          //%
	SubReqMan( const sc_core::sc_module_name & n ,                          //%
		unsigned int MAX_OUTSTANDING_REQ = 4,                          //%
		unsigned int MAX_DATA_BUF_KB = 16) ;                           //%
	SC_HAS_PROCESS( SubReqMan );                                            //%
	// destructor                                                           //%
	virtual ~SubReqMan();                                                   //%
private:                                                                        //%
	// disable default and copy constructors by making them private         //%
	SubReqMan() ;                                                           //%
	SubReqMan( const SubReqMan& ) ;                                         //%
	SubReqMan & operator= ( const SubReqMan& ) ;                            //%
public:                                                                         //%
	// method called at end of elaboration of design -- for initialization  //%
	virtual void end_of_elaboration();                                      //%
	// initialization methods for ports & registers                         //%
	virtual void initPorts();                                               //%
	virtual void initRegisters();                                           //%
	// method to handle interactive commands -- for dynamic instrumentation //%
	virtual std::string commandProcessor(const std::vector<std::string>& cmd);//%
public:                                                                         //%
	// Port declarations                                                    //%

    tlm::tlm_target_socket< 32 > WriteSlave;                                //%
	tlm::tlm_target_socket< 32 > CmdSlave;                                //%
	tlm::tlm_target_socket< 32 > DataSlave;                                //%
	scml2::initiator_socket< 32 > ReadMaster;                              //%
	scml2::initiator_socket< 32 > CmdMaster;                              //%
	scml2::initiator_socket< 32 > DataMaster;                              //%
    

	sc_core::sc_out< bool > RnBMaster;                                      //%
	sc_core::sc_in< bool > rst_n;                                           //%
protected:                                                                      //%
                                                                                //%
	// Parameter/Property declarations                                      //%
	scml_property< unsigned int > MAX_OUTSTANDING_REQ_;                     //%
	scml_property< unsigned int > MAX_DATA_BUF_KB_;                         //%
                                                                                //%
protected:                                                                      //%
                                                                                //%
	// Memory declarations                                                  //%
	scml2::memory< unsigned int > WriteSlaveBuf;                           //%
	  scml2::memory_alias< unsigned int > WriteSlaveBuf_writeBuf;//%
                                                                                //%
	scml2::tlm2_gp_target_adapter< 32 > WriteSlave_WriteSlaveBuf_adapter;   //%
    scml2::memory< unsigned int > CmdSlaveBuf;                           //%
	  scml2::memory_alias< unsigned int > CmdSlaveBuf_writeBuf;//%
                                                                                //%
	scml2::tlm2_gp_target_adapter< 32 > CmdSlave_CmdSlaveBuf_adapter;   //%
    scml2::memory< unsigned int > DataSlaveBuf;                           //%
	  scml2::memory_alias< unsigned int > DataSlaveBuf_writeBuf;//%
                                                                                //%
	scml2::tlm2_gp_target_adapter< 32 > DataSlave_DataSlaveBuf_adapter;   //%
                                                                                 //%
public:                                                                         //%
	// Process declarations                                                 //%
	void                                                                    //%
	rst_nHandler();                                                         //%
    void
    DivideSubReq();
    void
    WriteCmd_Write();
    void
    ReadDataFromCache();
	void                                                                    //%
	ReadManager();
    void
    Copy_SFR_To_CMDList(uint size);    //%
                                                                                //%
protected:                                                                      //%
	// Memory callback declarations                                         //%
	void                                                                    //%
	WriteSlaveCB(                                                           //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t);          //%
    void                                                                    //%
	CmdSlaveCB(                                                           //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t);          //%
    void                                                                    //%
	DataSlaveCB(                                                           //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t);          //%

	// support for message logging                                          //%
	void initMessageLogging();                                              //%
	void REPORT_MSG(std::string sev, std::string msg);                      //%
	void DBG_MSG(std::string msg)   { REPORT_MSG("debug",msg); }            //%
	void WARN_MSG(std::string msg)  { REPORT_MSG("warn",msg); }             //%
	void ERROR_MSG(std::string msg) { REPORT_MSG("error",msg); }            //%
	void FATAL_MSG(std::string msg) { REPORT_MSG("fatal",msg); }            //%
                                                                                //%USERBEGIN MEMBER_DECLS
    enum DbgCategories { GENERAL = 0, PORTS, REGS } ;
    bool dbgFlag[sizeof(DbgCategories)];

    unsigned int cmd_size;
    COMMAND cmd_list[10];
    // TODO: Insert declarations of member data and functions here.

protected:
    

    // member methods
    //
    //
    //
    inline void PrintBitMap(uint id, std::string msg, uchar* bitmap, uint len)
    {
        if (SUB_DEBUG) cout << DEV_AND_TIME << "[PrintBitMap] "<< id << ":" << msg << ":bitmap[";
        for(int i=0; i < len ; i++)
        {
            if (SUB_DEBUG) cout << (uint)((bitmap[i/8] >> (7-(i%8)) & 0x1));
        }
        if (SUB_DEBUG) cout << "]" << endl;
    }
    inline void PrintBufStatus(std::string msg)
    {
//        cout << msg << ":bufStat" << ": " << this->m_iUsedDataBuf << "B " << " [" << this->basename() << "@" << sc_core::sc_time_stamp() << "]" << std::endl;
    }

    inline bool IsAllRead(sIntReq_t *intReq)
    {

        PrintBitMap(intReq->req.iId, "", intReq->cpBitMap, intReq->req.iLen);

        for(int i=0; i < intReq->req.iLen ; i++)
        {
            if(!((intReq->cpBitMap[i/8] >> (7-(i%8))) & (0x1)))
                return false;
        }

        return true;
    }


    inline bool IsReqBufEmpty()
    {
        return (m_IntReqList.size() <= 0);
    }

    inline bool IsReqBufFull()
    {
        if(m_IntReqList.size() >= MAX_OUTSTANDING_REQ_)
        {
            if(dbgFlag[GENERAL])
                if (SUB_DEBUG) cout << DEV_AND_TIME << "[IsDataBufFull]Request Buffer Full" << endl;
        }
        return (m_IntReqList.size() >= MAX_OUTSTANDING_REQ_);
    }

    inline bool IsDataBufFull(unsigned int iLen)
    {
        if(((m_iUsedDataBuf) + (iLen * SECTOR_SIZE_BYTE))
                > MAX_DATA_BUF_KB_ * KILO)
        {
            //if(dbgFlag[GENERAL])
                if (SUB_DEBUG) cout << DEV_AND_TIME << "[IsDataBufFull] Data Buffer Full" << endl;
        }
        return (((m_iUsedDataBuf) + (iLen * SECTOR_SIZE_BYTE))
                > MAX_DATA_BUF_KB_ * KILO);

    }

    inline unsigned int GetStartSectorAddr(sIntReq_t *intReq)
    {
        
        unsigned int iNextAlignedSector = (((intReq->req.iAddr + intReq->iNextSector) / SECTOR_PER_PAGE) * SECTOR_PER_PAGE);
        unsigned int iReqFirstSector = intReq->req.iAddr;
        //cout<<iNextAlignedSector << "\t"<<iReqFirstSector<<"\t"<<endl;
        return iNextAlignedSector >= iReqFirstSector ? iNextAlignedSector : iReqFirstSector;
    }

    inline unsigned int GetLastSectorAddr(sIntReq_t *intReq)
    {
        unsigned int iNextAlignedSector = (((intReq->req.iAddr + intReq->iNextSector) / SECTOR_PER_PAGE) * SECTOR_PER_PAGE) + SECTOR_PER_PAGE - 1;
        unsigned int iReqLastSector = intReq->req.iAddr + intReq->req.iLen - 1;
        return iNextAlignedSector <= iReqLastSector ? iNextAlignedSector : iReqLastSector;
    }

    inline void SetNextSector(sIntReq_t *intReq, uint iStartAddr, uint iLen)
    {
        intReq->iNextSector += iLen;
    }

    inline bool IsAllWritten(sSubReqWithData_t *subReq)
    {
        return ((subReq->subReq->iStartAddr + subReq->subReq->iLen - 1) >=
            (subReq->subReq->oriReq.iAddr + subReq->subReq->oriReq.iLen - 1));
    }


    inline sIntReq_t* ComposeIntReq(Req_t newReq)
    {
        sIntReq_t* newIntReq = new sIntReq_t;
        unsigned int iBitMapSize = ceil((float)newReq.iLen / 8);
        if(SUB_DEBUG){
            cout<< DEV_AND_TIME << "[ComposeIntReq] iBitMapSize : "<< iBitMapSize <<endl;
        }
        newIntReq->req = newReq;
        newIntReq->cpBitMap = new uchar[iBitMapSize];
        newIntReq->state = false;
        memset(newIntReq->cpBitMap, 0x00, iBitMapSize);

        PrintBitMap(newReq.iId, "initial", newIntReq->cpBitMap, newReq.iLen);

        newIntReq->cpDataBuf = new uchar[newReq.iLen * SECTOR_SIZE_BYTE];
        memset(newIntReq->cpDataBuf, 0x00, newReq.iLen * SECTOR_SIZE_BYTE); 
        newIntReq->iNextSector = 0;

        return newIntReq;        
    }
    
    inline void InsertBuf(sIntReq_t* newIntReq)
    {
        m_IntReqList.push_back(newIntReq);
        m_iUsedDataBuf += (newIntReq->req.iLen * SECTOR_SIZE_BYTE); 

        m_iaNumLiveOps[newIntReq->req.Op]++;

        PrintBufStatus("inserted");

    }

    void PrintBufId(){//debugging purposes


        std::list<sIntReq_t *>::iterator iter;
        for(iter = m_IntReqList.begin() ; iter != m_IntReqList.end() ; iter++)  
           cout<< "ID list " << (((sIntReq_t *)(*iter))->req.iId) <<endl;

    }

    std::list<sIntReq_t *>::iterator FindCompleteIntReq()
    {
        std::list<sIntReq_t *>::iterator iter;
        for(iter = m_IntReqList.begin() ; iter != m_IntReqList.end() ; iter++)
        {
            
            
       //temporary 
            if(IsAllRead((sIntReq_t *)(*iter)))
            {
                return iter;
            }
        }
        return iter;

    }


    std::list<sIntReq_t *>::iterator FindIntReqById(unsigned int iId)
    {
        std::list<sIntReq_t *>::iterator iter;
        for(iter = m_IntReqList.begin() ; iter != m_IntReqList.end() ; iter++)
        {
            
            
       //temporary 
            if((((sIntReq_t *)(*iter))->req.iId) == iId)
            {
                return iter;
            }
        }

        ERROR_MSG("No proper request has been found\n");
        sc_stop();
    }


    inline void DeleteBuf(std::list<sIntReq_t *>::iterator iter)
    {
        //by skhds:
        //The usage of this function does not always result in buffer being completely deallocated from this module;
        //the function may be used after a request transfer from IntReqList to CacheQueue to deallocate buffer in the
        //source queue. Current implementation of calculating m_iUsedDataBuf is based on the total size of buffer being 
        //used in the entire module (not just a single queue).

        
        if(SUB_DEBUG)     cout<< DEV_AND_TIME << "[DeleteBuf] Deleting request id : " << (((sIntReq_t *)(*iter))->req.iId) <<endl;
        sIntReq_t* curIntReq = (sIntReq_t *)(*iter);
        if(curIntReq->req.Op==HOST_READ) //modified by skhds
            m_iUsedDataBuf -= ((sIntReq_t *)(*iter))->req.iLen * SECTOR_SIZE_BYTE;//modified by skhds
        m_iaNumLiveOps[((sIntReq_t *)(*iter))->req.Op]--;
        //delete[] curIntReq->cpBitMap;
        //delete[] curIntReq->cpDataBuf;
        //delete (curIntReq); 

        m_IntReqList.erase(iter); //assuming the embedded function deallocates the element (including internal buffers)

        PrintBufStatus("deleted");

        
        //if(curIntReq->req.Op==HOST_READ)
        //    RnBMaster.write(1); //modified by skhds

    }

    sSubReqWithData_t SetSubReq(sIntReq_t* curIntReq)
    {
        sSubReqWithData_t subReqAndData;

        subReqAndData.subReq = new sSubReq_t;

        subReqAndData.subReq->oriReq = curIntReq->req;
        subReqAndData.subReq->iStartAddr = GetStartSectorAddr(curIntReq);
        subReqAndData.subReq->iLen = GetLastSectorAddr(curIntReq) - subReqAndData.subReq->iStartAddr + 1;
        subReqAndData.subReq->Id = curIntReq->req.iId;
        subReqAndData.subReq->dIssueTime = sc_time_stamp().to_double();
        
        SetNextSector(curIntReq, subReqAndData.subReq->iStartAddr, subReqAndData.subReq->iLen);
        
        if(curIntReq->req.Op==HOST_WRITE){
            subReqAndData.cpData = new uchar[subReqAndData.subReq->iLen* SECTOR_SIZE_BYTE];
            //subReqAndData.cpData = new uchar[SECTOR_PER_PAGE * SECTOR_SIZE_BYTE];
            memcpy(subReqAndData.cpData, curIntReq->cpDataBuf + (subReqAndData.subReq->iStartAddr - curIntReq->req.iAddr), subReqAndData.subReq->iLen*SECTOR_SIZE_BYTE);

            // For simulation check, 
            // set infinite data buffer capacity 
            //m_iUsedDataBuf += (subReqAndData.subReq->iLen * SECTOR_SIZE_BYTE); 
        }
        return subReqAndData;
    }

    void SetCacheCmd(sSubReqWithData_t sub_queue, COMMAND cmd_list, std::list <sSubReqWithData_t>::iterator req_itr){
        uint length=0;
        uint startPointer=0, endPointer=0;
        uint isFirst = true;
        memcpy(sub_queue.subReq, req_itr->subReq, sizeof(sSubReq_t));
        if((cmd_list.op == READ_DRAM)||(cmd_list.op == READ_BOTH) ){ // read DRAM
            sub_queue.subReq->cmd.cmd.Op = DRAM_READ;
        }
        else if(cmd_list.op == WRITE_DRAM ){ // write DRAM
            sub_queue.subReq->cmd.cmd.Op = DRAM_WRITE;
            memcpy(sub_queue.cpData, req_itr->cpData, req_itr->subReq->iLen*SECTOR_SIZE_BYTE);
        }
        else{
            if (SUB_DEBUG) cout<<"This SetCahceCmd case is not allowed"<<endl;
        }
        //find the start point, end point of page which will be written.
        for(uint j=0; j<SECTOR_PER_PAGE ; j++){
            if(cmd_list.dram_bitmap & (1<<j)){
                if(isFirst){
                    isFirst = false;
                    startPointer = j;
                }
                length++;
                endPointer = j;
            }
        }
        
        
        if(SUB_DEBUG){
            cout <<DEV_AND_TIME <<"[SetCacheCmd] iStarAddr : " <<sub_queue.subReq->iStartAddr;
            cout <<", Id : "<< sub_queue.subReq->Id  <<endl;
        }

        if((cmd_list.op == READ_DRAM)||(cmd_list.op == WRITE_DRAM) ){
            sub_queue.subReq->cmd.cmd.Len = length * SECTOR_SIZE_BYTE;
        }
        else if(cmd_list.op == READ_BOTH){
            sub_queue.subReq->cmd.cmd.Len = (endPointer - startPointer + 1)*SECTOR_SIZE_BYTE ;
        }
        sub_queue.subReq->cmd.cmd.Addr = cmd_list.dram_id * SECTOR_SIZE_BYTE * SECTOR_PER_PAGE + startPointer * SECTOR_SIZE_BYTE;
        if((cmd_list.op == READ_DRAM)||(cmd_list.op == WRITE_DRAM) ){
            sub_queue.subReq->cmd.cmd.lba = req_itr->subReq->iStartAddr;
        }
        else if(cmd_list.op == READ_BOTH){
            sub_queue.subReq->cmd.cmd.lba = req_itr->subReq->iStartAddr + startPointer;
        }
        sub_queue.subReq->cmd.cmd.Id = req_itr->subReq->Id;
        sub_queue.subReq->Id = req_itr->subReq->Id;
        sub_queue.reqType = hostReq;
    }

    void SetNandCmd(sSubReqWithData_t nand_queue, COMMAND cmd_list, std::list <sSubReqWithData_t>::iterator req_itr){
        uint length=0;
        uint for_start, for_end;
        uint startPointer, endPointer;
        uint isFirst = true;

        memcpy(nand_queue.subReq, req_itr->subReq, sizeof(sSubReq_t));
        if(cmd_list.op == READ_SLAVE){ // read NAND only
            for_start = req_itr->subReq->iStartAddr % SECTOR_PER_PAGE;;
            for_end = (req_itr->subReq->iStartAddr % SECTOR_PER_PAGE) + req_itr->subReq->iLen ;   
        }
        else if(cmd_list.op == READ_BOTH){//read NAND and DRAM both
            for_start = 0;
            for_end = SECTOR_PER_PAGE ;   
        }
        else{
           if (SUB_DEBUG)  cout<<"This SetCahceCmd case is not allowed"<<endl;
        }   

        for(uint j=for_start; j< for_end; j++){
            if(cmd_list.slave_bitmap & (1<<j)){
                if(isFirst){
                    isFirst = false;
                    startPointer = j;
                }
                length++;
                endPointer = j;
            }
        }

        if(cmd_list.op == READ_SLAVE){ // read NAND only
            nand_queue.subReq->iLen = length;
        }
        else if(cmd_list.op == READ_BOTH){ // read NAND and DRAM both
            nand_queue.subReq->iLen = endPointer - startPointer + 1;
        }
        if(SUB_DEBUG){
            cout <<DEV_AND_TIME <<"[SetNandCmd] iStarAddr : " <<nand_queue.subReq->iStartAddr;
            cout <<", Id : "<< nand_queue.subReq->Id  <<endl;
        }
        nand_queue.subReq->iStartAddr = cmd_list.slave_addr * SECTOR_PER_PAGE + startPointer;
        nand_queue.subReq->Id = req_itr->subReq->Id;
        nand_queue.reqType = hostReq;
    }


    void move_queue(){
        std::list<sSubReqWithData_t>::iterator iter;
        sSubReqWithData_t request;

        iter=Cache_Queue.begin();
        request.subReq = new sSubReq_t;        
        memcpy(request.subReq, iter->subReq, sizeof(sSubReq_t));
        request.reqType = iter->reqType;

        Nand_Queue.push_back(request);
    }

    void SubReqResponse(sReadSubResp_t response){
        if(SUB_DEBUG) cout<<DEV_AND_TIME << "[SubReqResponse] response id : "  << response.Id << endl;

        //PrintBufId();
        std::list<sIntReq_t *>::iterator iter=FindIntReqById(response.Id);
        sIntReq_t* curIntReq =(sIntReq_t *)(*iter);

        unsigned int offset = response.logical_addr - curIntReq->req.iAddr ;
        memcpy(curIntReq->cpDataBuf + offset * 512, response.cpData, response.len * 512);
        //if(g_initialize_end) testTrack.respStart(response.Id, response.logical_addr, response.len, SSDmetric::SubReqMan, sc_time_stamp().value()/1000.0); 


        if (SUB_DEBUG) cout<<DEV_AND_TIME<<"[SubReqResponse] logical addr of subreq : "<< response.logical_addr <<endl;
        if (SUB_DEBUG) cout<<DEV_AND_TIME<<"[SubReqResponse] offset of subreq : "<< offset  <<endl;
        if (SUB_DEBUG) cout<<DEV_AND_TIME<<"[SubReqResponse] sizeof subreq length : "<< response.len <<endl;
        for(unsigned int i=0; i < response.len; i++ ){
            curIntReq->cpBitMap[(offset+i)/8] |= ( 0x1 << ( 7 - ((offset+i) % 8)));
        }


        if(IsAllRead(curIntReq))
        {   // if all the data is read, transfer it to my host
//            if (SUB_DEBUG) cout<< DEV_AND_TIME << "[SubReqResponse] All subReq of HOST read request is completed"<<endl;
//            m_seReadPath.wait();    // read path semaphore
//
//            //if (SUB_DEBUG) cout<< DEV_AND_TIME << "[SubReqResponse] m_seReadPath passed, ID : " << response.Id << endl;
//            wait(SC_ZERO_TIME);     // wait zero time to avoid deadlock
//            m_CompleteReaddIter = iter;  // completed internal request
            m_eNewReadResp.notify();        // activate the thread
        }

    }

        // debug methods

        // member variable
    //

    /* Hostside Queue */
	std::list <sSubReqWithData_t>	Write_Queue;
	std::list <sSubReqWithData_t>	Read_Queue;
	std::list <sSubReq_t>		    Seq_Op_Queue;
    std::list <sReadSubResp_t>      Notified_Queue;
    /* Cache and Nand side Queue */
    std::list <sSubReqWithData_t>   Cache_Queue;        // For Cache read / write
    std::list <sSubReqWithData_t>   Nand_Queue; 

    sReadSubResp_t  ReadSubRespFromNand;
    sReadSubResp_t  ReadSubRespFromCache;
    sReadSubResp_t  ReadSubResptemp;

    eWSState m_WSState;
    std::list<sIntReq_t *> m_IntReqList;
    std::list<sIntReq_t *>::iterator m_CompleteReaddIter;

    sc_core::sc_event m_eNewHostReq;
    sc_core::sc_event m_eNewReadResp;
    sc_core::sc_event m_DRAMWrite;
    sc_core::sc_event m_Read_NandData;
    sc_core::sc_event m_Read_CacheData;

    sc_core::sc_semaphore m_WaitCachePossesion;
    sc_core::sc_semaphore m_seReadPath;
    sc_core::sc_semaphore m_seWaitCacheRead;

    unsigned int m_iUsedDataBuf;
    unsigned int m_iaNumLiveOps[128];
    unsigned int count=0;
    unsigned int reqId;
 
 public:
    // member methods
    //
    

                                                                                //%USEREND MEMBER_DECLS
};                                                                              //%
                                                                                //%
//------------------------                                                      //%
// Module Implementation                                                        //%
//------------------------                                                      //%
                                                                                //%USERBEGIN HEADER_CPP
// TODO: Insert your includes, declarations, etc. here.
                                                                                //%USEREND HEADER_CPP
                                                                                //%
SubReqMan::SubReqMan( const sc_core::sc_module_name & n,                        //%
		unsigned int MAX_OUTSTANDING_REQ,                               //%
		unsigned int MAX_DATA_BUF_KB)                                   //%
		: sc_core::sc_module(n)                                         //%
,                                                                               //%
 WriteSlave("WriteSlave"),                                                      //%
 ReadMaster("ReadMaster"),                                                      //%
 CmdSlave("CmdSlave"),
 CmdMaster("CmdMaster"),
 DataSlave("DataSlave"),
 DataMaster("DataMaster"),
 RnBMaster("RnBMaster"),                                                        //%
 rst_n("rst_n"),                                                                //%
 WriteSlave_WriteSlaveBuf_adapter("WriteSlave_WriteSlaveBuf_adapter", WriteSlave),//%
 CmdSlave_CmdSlaveBuf_adapter("CmdSlave_CmdSlaveBuf_adapter", CmdSlave),//%
 DataSlave_DataSlaveBuf_adapter("DataSlave_DataSlaveBuf_adapter", DataSlave),//%
 MAX_OUTSTANDING_REQ_("MAX_OUTSTANDING_REQ", MAX_OUTSTANDING_REQ),              //%
 MAX_DATA_BUF_KB_("MAX_DATA_BUF_KB", MAX_DATA_BUF_KB),                          //%
 WriteSlaveBuf("WriteSlaveBuf", 524288ULL),                                          //%
  WriteSlaveBuf_writeBuf("WriteSlaveBuf_writeBuf", WriteSlaveBuf, 0LL, 524288LL )                                                                              //%
,                                                                               //%
 CmdSlaveBuf("CmdSlaveBuf", 524288ULL),                                          //%
  CmdSlaveBuf_writeBuf("CmdSlaveBuf_writeBuf", CmdSlaveBuf, 0LL, 524288LL )//%
,                                                                               //%
 DataSlaveBuf("DataSlaveBuf", 524288ULL),                                          //%
  DataSlaveBuf_writeBuf("DataSlaveBuf_writeBuf", DataSlaveBuf, 0LL, 524288LL )//%
,                                                                               //%
                                                                               //%USERBEGIN INITIALIZER
                                                                                //
    // TODO: Add your class initialization here.
 m_WaitCachePossesion(1),
 m_seReadPath(1),
 m_seWaitCacheRead(1)
                                                                                //%USEREND INITIALIZER
                                                                                //%
{                                                                               //%
 // declare processes and their sensitivities                                   //%
	SC_METHOD(rst_nHandler);                                                //%
	 sensitive << rst_n.neg() ;                                             //%
	 dont_initialize();                                                     //%
    SC_THREAD(DivideSubReq);
     sensitive << m_eNewHostReq;
     dont_initialize();
    SC_THREAD(WriteCmd_Write);
     sensitive << m_DRAMWrite;
     dont_initialize();

    SC_THREAD(ReadDataFromCache);
     sensitive << m_Read_CacheData;
     dont_initialize();
	SC_THREAD(ReadManager);                                                 //%
     sensitive << m_eNewReadResp;
	 dont_initialize();                                                     //%
                                                                                //%
	// bind target ports to memories                                        //%
	WriteSlave_WriteSlaveBuf_adapter(WriteSlaveBuf);                        //%
	CmdSlave_CmdSlaveBuf_adapter(CmdSlaveBuf);                        //%
	DataSlave_DataSlaveBuf_adapter(DataSlaveBuf);                        //%
                                                                                //%
 // setup memory callbacks                                                      //%
	// - Memory: WriteSlaveBuf                                              //%
	scml2::set_callback(                                                    //%
		WriteSlaveBuf_writeBuf,                           //%
		SCML2_CALLBACK(WriteSlaveCB),                                   //%
		scml2::AUTO_SYNCING);                                           //%
    // - Memory: CmdSlaveBuf                                              //%
	scml2::set_callback(                                                    //%
		CmdSlaveBuf_writeBuf,                           //%
		SCML2_CALLBACK(CmdSlaveCB),                                   //%
		scml2::AUTO_SYNCING);                                           //%
    // - Memory: DataSlaveBuf                                              //%
	scml2::set_callback(                                                    //%
		DataSlaveBuf_writeBuf,                           //%
		SCML2_CALLBACK(DataSlaveCB),                                   //%
		scml2::AUTO_SYNCING);                                           //%
                                                                       //%
	                                                                           //%
                                                                                //%
#ifdef SCML_COMMAND_PROCESSOR_H                                                 //%
	SCML_COMMAND_PROCESSOR(commandProcessor);                               //%
	SCML_ADD_COMMAND("debug",0,6,"debug [-+][general|regs|ports|all]", "Turn on/off debug flags");//%
#endif // SCML_COMMAND_PROCESSOR_H                                              //%
                                                                                //%
    initMessageLogging();                                                       //%
                                                                                //%USERBEGIN CONSTRUCTOR

    // TODO: Add constructor code here.
    if ( dbgFlag[GENERAL] ) DBG_MSG("Created " + (std::string)this->basename());
    
    if (SUB_DEBUG) cout << "subreqman constructor" << endl;

                                                                                //%USEREND CONSTRUCTOR
                                                                                //%
}                                                                               //%
// destructor                                                                   //%
SubReqMan::~SubReqMan()                                                         //%
{                                                                               //%
                                                                                //%USERBEGIN DESTRUCTOR

    // TODO: Add destructor code here.
    Subfile.close();
    Listfile.close();

                                                                                //%USEREND DESTRUCTOR
}                                                                               //%
// initialization method for ports                                              //%
void                                                                            //%
SubReqMan::initPorts()                                                          //%
{                                                                               //%
	RnBMaster.initialize(0x1);                                              //%
}                                                                               //%
// initialization method for registers                                          //%
void                                                                            //%
SubReqMan::initRegisters()                                                      //%
{                                                                               //%
    WriteSlaveBuf.initialize(0x0);                                              //%
    WriteSlaveBuf_writeBuf.initialize(0x0);                       //%
    CmdSlaveBuf.initialize(0x0);                                              //%
    CmdSlaveBuf_writeBuf.initialize(0x0);                       //%
    DataSlaveBuf.initialize(0x0);                                              //%
    DataSlaveBuf_writeBuf.initialize(0x0);                       //%


}
//%
//This method is invoked after the design is fully elaborated, but              //%
// before simulation begins.                                                    //%
void                                                                            //%
SubReqMan::end_of_elaboration()                                                 //%
{                                                                               //%
                                                                                //%USERBEGIN END_OF_ELAB
    // TODO: Add end_of_elaboration code here.
    this->initRegisters();
    this->initPorts();
    Subfile.open("subfile.txt");
    Listfile.open("Listfile.txt");
                                                                                //%USEREND END_OF_ELAB
}                                                                               //%
// command processing method                                                    //%
//  During simulation, this command may be called to handle                     //%
//  interactive commands -- for dynamic instrumentation of the instance.        //%
std::string                                                                     //%
SubReqMan::commandProcessor(const std::vector< std::string >& cmd)              //%
{                                                                               //%
                                                                                //%USERBEGIN CMD_PROCESSOR
    // TODO: Add commandProcessor code here.
    //  The default code generated here handles toggling off/on the
    //  default debug instrumentation flags: GENERAL, PORTS, and REGS.

    std::string command = cmd[0];
    std::ostringstream outs;

    if (command == "debug") {
        bool print_usage_flag = false;
        std::string debug_cmd;

        if ((cmd.size() == 1) || (cmd[1] == "?")) {
            print_usage_flag = true;
        } else {
            int i = 1;
            while (i < cmd.size()) {
                bool flag = true;
                if ((cmd[i] == "-") || (cmd[i] == "+")) {
                    if (cmd[i] == "-") {
                        flag = false;
                    }
                    i++;
                }
                debug_cmd = cmd[i];
                if ((debug_cmd[0] == '+') || (debug_cmd[0] == '-')) {
                    if (debug_cmd[0] == '-') {
                        flag = false;
                    }
                    debug_cmd = debug_cmd.substr(1,debug_cmd.size()-1);
                }
                if (debug_cmd == "all") {
		    for ( int jj = 0; jj<sizeof(DbgCategories); jj++ ) {
		    	dbgFlag[jj] = flag;
		    }
                } else if (debug_cmd == "general") {
                    dbgFlag[GENERAL] = flag;
                } else if (debug_cmd == "ports" || debug_cmd == "pins") {
                    dbgFlag[PORTS] = flag;
                } else if (debug_cmd == "regs") {
                    dbgFlag[REGS] = flag;
                } else {
		    DBG_MSG(std::string("ERROR: Unknown Option:") + debug_cmd);
                    print_usage_flag = true;
                    break;
                }
                i++;
            }
        }
        outs << "Current debug options:"
            << (dbgFlag[GENERAL] ? " general" : "")
            << (dbgFlag[PORTS] ? " ports" : "")
            << (dbgFlag[REGS] ? " regs" : "")
            << ((dbgFlag[GENERAL] || dbgFlag[PORTS] || dbgFlag[REGS] )? "" : " Off")
            << std::endl;
        if (print_usage_flag) {
            outs << "Usage: debug [-+][general|ports|regs|all]" << std::endl;
            outs << " Example1: 'debug -ports' toggles OFF the debug flag for ports." << std::endl;
            outs << " Example2: 'debug ports regs' toggles ON the flags for ports & registers." << std::endl;
        }
    }
    return outs.str();
                                                                                //%USEREND CMD_PROCESSOR
}                                                                               //%
                                                                                //%
// initialization method for logging messages                                   //%
                                                                                //%
void                                                                            //%
SubReqMan::initMessageLogging()                                                 //%
{                                                                               //%
                                                                                //%USERBEGIN MODEL_DEBUG_INIT
    // toggle on/off debugging flags
    for (int i=0;i<sizeof(DbgCategories);i++) {
//#ifdef DBG_SCP_SUBREQMAN
//        dbgFlag[i] = true;
//#else
//        dbgFlag[i] = false;
//#endif
    }
                                                                                //%USEREND MODEL_DEBUG_INIT
                                                                                //%
}                                                                               //%
// method for logging messages                                                  //%
void                                              //%
SubReqMan::REPORT_MSG(std::string sev, std::string msg)//%
{                                                 //%
                                                                                //%USERBEGIN REPORT_MSG
    std::cout << sev << ": " << msg << " [" << this->basename() << "@" << sc_core::sc_time_stamp() << "]" << std::endl;
                                                                                //%USEREND REPORT_MSG
                                                                                //%
}                                                                               //%
void                                                                            //%
SubReqMan::rst_nHandler()                                                       //%
                                                                                //%USERBEGIN rst_nHandler
{
	if ( dbgFlag[PORTS] ) DBG_MSG( "rst_nHandler invoked because of event on port rst_n" );

	// TODO: INSERT code for the rst_nHandler method here.

    // make RnB to high for the first request
    m_WSState = IDLE;

    m_iUsedDataBuf = 0;
    for(unsigned int i=0; i<128; i++)
    {
        m_iaNumLiveOps[i] = 0;
    }
    reqId=0;
    for(unsigned int i=0; i<10; i++)
    {
        cmd_list[i].op  = UPDATE_SLAVE;
        cmd_list[i].slave_addr  = 0;
        cmd_list[i].slave_bitmap= 0;
        cmd_list[i].dram_id     = 0;
        cmd_list[i].dram_bitmap = 0;
       
    }        
 
    RnBMaster.write(1);

} 
                                                                                //%USEREND rst_nHandler
void
SubReqMan::DivideSubReq()
{
    while(1){
        sSubReqWithData_t request;
        std::list<sIntReq_t *>::iterator iter;
        sIntReq_t *curIntReq;

        if(m_IntReqList.size() == 0)
        {
            ERROR_MSG("[DivideSubReq] No IntReq to divide 1");
            sc_stop();
        }

        iter = (m_IntReqList.begin());
        curIntReq = (sIntReq_t *)(*(iter));
        while((curIntReq->req.Op == HOST_READ) && (curIntReq->state == true))
        {
            // current iterator should not point HOST_read request which wait for read response.
            std::advance(iter, 1);  
            if(iter == m_IntReqList.end())
            {
                ERROR_MSG("[DivideSubReq] No IntReq to divide 2");
                sc_stop();
            }
            curIntReq = (sIntReq_t *)(*(iter));
        }
            

        if(g_initialize_end) testTrack.track( curIntReq->req.iId, curIntReq->req.iAddr, curIntReq->req.iLen, SSDmetric::SubReqMan, sc_time_stamp().value()/1000.0); //track 1 (write)
        
        int NUMBER_OF_PAGE = ((curIntReq->req.iAddr%SECTOR_PER_PAGE)+curIntReq->req.iLen-1)/SECTOR_PER_PAGE +1;


        for(int count=0; count< NUMBER_OF_PAGE ; count++){
            iter = (m_IntReqList.begin());
            curIntReq = (sIntReq_t *)(*(iter));
            while((curIntReq->req.Op == HOST_READ) && (curIntReq->state == true)
                   && curIntReq->iNextSector == (curIntReq->req.iLen))
            {
                // current iterator should not point HOST_read request which wait for read response.
                std::advance(iter, 1);  
                if(iter == m_IntReqList.end())
                {
                    ERROR_MSG("[DivideSubReq] No IntReq to divide 2");
                    sc_stop();
                }

                curIntReq = (sIntReq_t *)(*(iter));
            }
            
            if(curIntReq->req.Op == HOST_READ){
                request=SetSubReq(curIntReq);
                Read_Queue.push_back(request);
                Seq_Op_Queue.push_back(*request.subReq);
                if(SUB_DEBUG)
                    cout<<DEV_AND_TIME<<"[DivideSubReq] Host read request is inserted Seq_Op_Queue & size = "<< Seq_Op_Queue.size()<<endl;

            }
            else if(curIntReq->req.Op == HOST_WRITE){
                request=SetSubReq(curIntReq);
                Write_Queue.push_back(request);
                Seq_Op_Queue.push_back(*request.subReq);
                
                
                if(SUB_DEBUG)
                    cout<<DEV_AND_TIME<<"[DivideSubReq] Host write request is inserted Seq_Op_Queue & size = "<< Seq_Op_Queue.size()<<endl;

            }       
        }

        if(curIntReq->req.Op == HOST_READ){
            curIntReq->state = true;
        }
        if(curIntReq->req.Op == HOST_WRITE){
            // if Host_write request is handled, IntReq is deleted.
            // Host_read request is lived until read response is completed.
            
            if (SUB_DEBUG) cout << DEV_AND_TIME << "[DivideSubReq] DeleteBuf is called"  << endl; 
            //PrintBufId();
            DeleteBuf(FindIntReqById(request.subReq->oriReq.iId));
            
        }
        
        wait();
    }
}

void
SubReqMan::WriteCmd_Write()
{
    // This thread send the SubReq to DRAM Ctrl.
    // And if subreq is write cmd, data is transmitted together.
    // SubReq which should be send to the NAND is transmitted in CMD Callback.
    // But this operation is not guaranteed if DRAM ctrl buffer is full.
    
    while(1){    
        if(SUB_DEBUG){
            cout<<DEV_AND_TIME<<"WriteCmd_Write is triggered"<<endl;
        }
        std::list<sSubReqWithData_t>::iterator iter;
        sSubReqWithData_t request;
          
        while(Cache_Queue.size()>0){
            iter=Cache_Queue.begin();
            SubReq_t temp;
            temp=(SubReq_t)*(iter->subReq);
            if (SUB_DEBUG) cout<<DEV_AND_TIME <<"[WriteCmd_Write] SubReqMan send the cache request on addr : " << hex <<0xd0000000 + CACHE_DATA_REGION_OFFSET << dec << "   Cache_Queue.size() = " << Cache_Queue.size() <<endl;
            //cout << DEV_AND_TIME << "start send req to DRAM" <<endl;

            if(g_initialize_end) testTrack.track(temp.Id, temp.iStartAddr, temp.iLen, SSDmetric::SubReqMan, sc_time_stamp().value()/1000.0); //track 4
            
            while(!CmdMaster.write(0xd0000000 + CACHE_DATA_REGION_OFFSET, (void*)&temp.cmd.cmd, sizeof(sCacheTrans_t))){
                
                assert(0 && "possible time out detected");
                //wait(CLOCK_PERIOD_NS, SC_NS);
            }

            //cout << DEV_AND_TIME << "end send req to DRAM" <<endl;
            if(((iter)->subReq->oriReq.Op) == HOST_WRITE ){
                //cout<<"data out : "<<iter->cpData<<endl;
                //cout << DEV_AND_TIME << "start send data to DRAM" <<endl;
                if (SUB_DEBUG) cout<<DEV_AND_TIME <<"[WriteCmd_Write] Sending data... length : " << iter->subReq->iLen <<endl;

                m_WaitCachePossesion.wait();
                while(!DataMaster.write(0x20000000, (void *)((iter)->cpData), (iter->subReq->iLen)*SECTOR_SIZE_BYTE )){
                    assert(0 && "possible time out detected2");
                }
                m_WaitCachePossesion.post();

                if(g_initialize_end) testTrack.track(temp.Id, temp.iStartAddr, temp.iLen, SSDmetric::SubReqMan, sc_time_stamp().value()/1000.0); //track 5

                if (SUB_DEBUG) cout << DEV_AND_TIME << "[WriteCmd_Write] Send complete!" <<endl;
                move_queue();
                delete[] iter->cpData;
                
                
                m_iUsedDataBuf -= (iter)->subReq->iLen * SECTOR_SIZE_BYTE;
                if(!(IsReqBufFull() || IsDataBufFull(0)) ) //modified by skhds
                    RnBMaster.write(1);                    
                //need to convert write request from Cache Queue to NAND Queue
                //And need to delete the m_iUsedbuf.
            }
            else if ( ((iter)->subReq->oriReq.Op) == HOST_READ ){


                if(g_initialize_end) testTrack.track(temp.Id, temp.iStartAddr, temp.iLen, SSDmetric::SubReqMan, sc_time_stamp().value()/1000.0); //cache read
            }
           
            delete iter->subReq;
            Cache_Queue.pop_front();
            wait(10, SC_NS);
        }
        wait();
    }
}

void
SubReqMan::ReadDataFromCache()
{
    while(1){
        // YM 180416 modify Notified Queue
        std::list<sReadSubResp_t>::iterator temp;
        if (SUB_DEBUG) cout<<DEV_AND_TIME<<"[ReadDataFromCache] notify "<<endl;
        while(Notified_Queue.size()>0){
            temp = Notified_Queue.begin();
            ReadSubResptemp.Id = temp->Id;
            ReadSubResptemp.logical_addr = temp->logical_addr;
            ReadSubResptemp.len = temp->len;

            ReadSubResptemp.cpData = new uchar[ReadSubResptemp.len];
            m_WaitCachePossesion.wait();
            DataMaster.read(0x20000000, ReadSubResptemp.cpData, ReadSubResptemp.len);
            m_WaitCachePossesion.post();
            ReadSubResptemp.len = ReadSubResptemp.len / SECTOR_SIZE_BYTE;
            if(SUB_DEBUG){
                cout<<DEV_AND_TIME<< "[ReadDataFromCache] Received Read response from Cache. Data = ";
                for(int i=0; i<ReadSubResptemp.len; i++)
                    cout<< ReadSubResptemp.cpData[i*SECTOR_SIZE_BYTE];
                cout<<endl;
            }
            SubReqResponse(ReadSubResptemp);
            delete[] ReadSubResptemp.cpData;
            Notified_Queue.pop_front();
            //m_seWaitCacheRead.post(); //180416 YM 
        }
        wait();
    }   
}

void                                                                            //%
SubReqMan::ReadManager()                                                        //%
                                                                                //%USERBEGIN ReadManager
{
	if ( dbgFlag[PORTS] ) DBG_MSG( "ReadManager invoked because of event on port " );

	// TODO: INSERT code for the ReadManager method here.

    while(1)
    {       // when a host read request is completely read, transfer it to the host

        sIntReq_t* curIntReq;
        m_CompleteReaddIter = FindCompleteIntReq();
        while(m_CompleteReaddIter != m_IntReqList.end()){

            if (SUB_DEBUG) cout << DEV_AND_TIME << "[ReadManager] notified, m_completeReaddIter iId : " << ((sIntReq_t *)(*m_CompleteReaddIter))->req.iId << endl; 
            sReadResp_t readResp;
            readResp.cpData = new uchar[MAX_REQ_LEN * 512];

            // compose response with data (sReadResp_t)
            readResp.hostReq = ((sIntReq_t *)(*m_CompleteReaddIter))->req;
            memcpy(readResp.cpData, ((sIntReq_t*)(*m_CompleteReaddIter))->cpDataBuf,
                    ((sIntReq_t*)(*m_CompleteReaddIter))->req.iLen * 512);        

            //if(g_initialize_end) testTrack.respTransfer(readResp.hostReq.iId, readResp.hostReq.iAddr, readResp.hostReq.iLen, SSDmetric::SubReqMan, sc_time_stamp().value()/1000.0);


            if (SUB_DEBUG) cout << DEV_AND_TIME << "[ReadManager] writing to read master iId : "  << ((sIntReq_t *)(*m_CompleteReaddIter))->req.iId << endl; 
            // transfer it to the host
            if(!ReadMaster.write(0x0, (unsigned char *)(&readResp), sizeof(sReadResp_t)))
            {
                ERROR_MSG("Read response should be accepted\n");
                sc_stop();
            }

            if (g_initialize_end) testTrack.done(readResp.hostReq.iId, readResp.hostReq.iAddr, readResp.hostReq.iLen, SSDmetric::SubReqMan, sc_time_stamp().value()/1000.0);
            // it is handled, so delete it from the buffer        
            //if(g_initialize_end) testTrack.respDone(readResp.hostReq.iId, readResp.hostReq.iAddr, readResp.hostReq.iLen, SSDmetric::SubReqMan, sc_time_stamp().value()/1000.0);


            // read port semaphore post for another read completion

            if (SUB_DEBUG) cout << DEV_AND_TIME << "[ReadManager] writing to RnB iId : "  << ((sIntReq_t *)(*m_CompleteReaddIter))->req.iId<< endl; 

            //PrintBufId();


            if(!(IsReqBufFull() || IsDataBufFull(0)) )    RnBMaster.write(1);//modified by skhds

            if (SUB_DEBUG) cout << DEV_AND_TIME << "[ReadManager] DeleteBuf is called iId : " << ((sIntReq_t *)(*m_CompleteReaddIter))->req.iId << endl; 
            delete[] readResp.cpData;
            DeleteBuf(m_CompleteReaddIter);

            //m_seReadPath.post();
            m_CompleteReaddIter = FindCompleteIntReq();
        }
        wait();
    }
}
void
SubReqMan::Copy_SFR_To_CMDList(uint size)
{
    scml2::memory_alias< unsigned int >& m = this->CmdSlaveBuf_writeBuf;   
    std::list < sSubReqWithData_t >::iterator req_itr;
    uint temp;
    for(uint i=0; i < size; i++){
        for(uint j=0; j<5; j++){
            ((unsigned int*)&cmd_list[i])[j] = m[i*5+j];
        }
        if(SUB_DEBUG) cout << DEV_AND_TIME << "[Copy_SFR_To_CMDList] controller cmd list op = " << cmd_list[i].op << "   slave addr = " << cmd_list[i].slave_addr << "   slave bitmap = " << cmd_list[i].slave_bitmap << "    dram_id = " << cmd_list[i].dram_id << "   dram bitmap = " << cmd_list[i].dram_bitmap << endl;

        temp = cmd_list[i].op;
    }
    //cout << temp <<endl;
        
    for(uint i=0; i < size; i++){
        
        sSubReqWithData_t nand_queue;
        sSubReqWithData_t sub_queue;
       
        switch(cmd_list[i].op){
            case 0:     //read dram
                req_itr = Read_Queue.begin();
                sub_queue.subReq = new sSubReq_t;
                SetCacheCmd(sub_queue, cmd_list[i], req_itr);
                Cache_Queue.push_back(sub_queue);
                if(SUB_DEBUG) cout << DEV_AND_TIME <<"Cache_Queue size : " << Cache_Queue.size() <<endl;
                delete req_itr->subReq;

                Read_Queue.pop_front();
                //Seq_Op_Queue.pop_front();

                break;

            case 1:     //write dram
                req_itr = Write_Queue.begin(); 
                sub_queue.subReq = new sSubReq_t;
                sub_queue.cpData = new uchar[SECTOR_PER_PAGE * SECTOR_SIZE_BYTE];
                SetCacheCmd(sub_queue, cmd_list[i], req_itr);
                Cache_Queue.push_back(sub_queue);

                if( g_initialize_end) testTrack.track(req_itr->subReq->Id, req_itr->subReq->iStartAddr, req_itr->subReq->iLen, SSDmetric::SubReqMan, sc_time_stamp().value()/1000.0 ); //track 3 

                delete req_itr->subReq;
                delete[] req_itr->cpData;

                Write_Queue.pop_front();
                //Seq_Op_Queue.pop_front();

                break;

            case 2:     //read nand
                req_itr = Read_Queue.begin();

                nand_queue.subReq = new sSubReq_t;
                SetNandCmd(nand_queue, cmd_list[i], req_itr);
                if(SUB_DEBUG) cout<<DEV_AND_TIME <<"NAND queue is pushed back."<<endl;
                Nand_Queue.push_back(nand_queue);

                delete req_itr->subReq;
                
                Read_Queue.pop_front();
                //Seq_Op_Queue.pop_front();

                break;
            
            case 3:     //read both
                req_itr = Read_Queue.begin();
                //read nand
                nand_queue.subReq = new sSubReq_t;
                SetNandCmd(nand_queue, cmd_list[i], req_itr);                                
                Nand_Queue.push_back(nand_queue);

                //read cache
                sub_queue.subReq = new sSubReq_t;
                SetCacheCmd(sub_queue, cmd_list[i], req_itr);
                //cout<<"[SubReqMan] receive dram addr from CPU "<<cmd_list[i] <<endl;
                Cache_Queue.push_back(sub_queue);
                                
                delete req_itr->subReq;

                Read_Queue.pop_front();
                //Seq_Op_Queue.pop_front();

                break;

            default:
                break;
        }

	    m_DRAMWrite.notify();
    }
    //cout << "endl" << endl;
    //Print_Queue_State();
}



//%USEREND ReadManager
void                                                                            //%
SubReqMan::WriteSlaveCB(                                                        //%
        tlm::tlm_generic_payload& trans, sc_core::sc_time& t)           //%
{                                                                               //%
    const char* dbgMsgText = "TRANSPORT handler for memory WriteSlaveBuf_writeBuf";//%
    scml2::memory_alias< unsigned int >& m = this->WriteSlaveBuf_writeBuf;//%
    tlm::tlm_command cmd = trans.get_command();                                 //%
    sc_dt::uint64 adr = trans.get_address();                                    //%
    unsigned char* ptr = trans.get_data_ptr();                                  //%
    unsigned int len = trans.get_data_length();                                 //%
    unsigned char* byt = trans.get_byte_enable_ptr();                           //%
    unsigned int wid = trans.get_streaming_width();                             //%
    //%USERBEGIN WriteSlaveCB

    if (SUB_DEBUG) cout << DEV_AND_TIME << "[WriteSlaveCB] " << endl; 

    if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );
    // TODO: INSERT code for the WriteSlaveCB method here.

    if (byt) { // Target unable to support byte enable attribute
        trans.set_response_status( tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE );
        return;
    }
    if (wid < len) { // Target unable to support streaming width attribute
        trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
        return;
    }

    if(cmd == tlm::TLM_READ_COMMAND)
    {
        ERROR_MSG("Read to this port is prohibited\n");
        sc_stop();
    }
    else if (cmd == tlm::TLM_WRITE_COMMAND){
        if(SUB_DEBUG){
            cout<< DEV_AND_TIME << "[WriteSlaveCB]data is arrived on SubReqMan, current state is : " << m_WSState  <<endl;
        }
    }

    if(SUB_DEBUG){

            
        if(m_IntReqList.size() >= MAX_OUTSTANDING_REQ_){
            cout << DEV_AND_TIME << "[WriteSlaveCB] outstanding request : " << m_IntReqList.size() << endl;
    
        
        } 
        if(((m_iUsedDataBuf) ) > MAX_DATA_BUF_KB_ * KILO){
            cout << DEV_AND_TIME << "[WriteSlaveCB] used data buffer : " << m_iUsedDataBuf << endl;
        
        }
     }
    //IsReqBufFull
    
    // local variables
    eWSState nextState;
    Req_t newReq;
    uchar *cpDest;
    int iLen; 
    std::list<sIntReq_t *>::iterator iterEnd;

    switch(m_WSState)
    {
        case IDLE:
            //wait( len/CMD_BUS_TOTAL_BW,SC_NS );
            memcpy(&newReq, ptr, sizeof(Req_t));
            
            if(g_initialize_end) testTrack.start(newReq.iId, newReq.iAddr, newReq.iLen, SSDmetric::SubReqMan, sc_time_stamp().value()/1000.0); 

            if (SUB_DEBUG) cout << DEV_AND_TIME << "[WriteSlaveCB] at IDLE... newReq : iID - " << newReq.iId << " OP - " <<  newReq.Op  <<  " iAddr - " <<  newReq.iAddr  <<  " iLen - " << newReq.iLen <<  " dIssueTime - " << newReq.dIssueTime << endl; 
            // insert incoming request to req buf
            InsertBuf(ComposeIntReq(newReq));

            if(newReq.Op == HOST_WRITE){    
                // write request waits for data
                nextState = CMD; 
                if(SUB_DEBUG)
                    cout<<DEV_AND_TIME<<"[WriteCallback] next state is CMD"<<endl;
            }
            else if(newReq.Op == HOST_READ){
                // reqd request goes to idle for next request
                nextState = IDLE;

            }

            if(IsReqBufFull() || IsDataBufFull(newReq.iLen)){
                // request or data buffer is full 
                if(SUB_DEBUG){
                    cout << DEV_AND_TIME << "[WriteCallback] request buffer is full"<<endl;
                }
                RnBMaster.write(0);
            }
            break;

        case CMD:
            //wait(len /(DATA_BUS_TOTAL_BW * 8), SC_NS); // 256 bitwidth
            // last req buf is the destination of incoming data
            // is there any way to point the last element easier? plz let me know!
            iterEnd = m_IntReqList.begin();
            std::advance(iterEnd, m_IntReqList.size()-1);

            if(SUB_DEBUG){
                cout<< DEV_AND_TIME <<"[WriteSlaveCB] at CMD" <<endl;
                //cout << " Received data : " << cpDest <<endl;
            }

            cpDest = ((sIntReq_t*)(*iterEnd))->cpDataBuf;  // data buffer address
            iLen = ((sIntReq_t*)(*iterEnd))->req.iLen * SECTOR_SIZE_BYTE;    // length (byte)


            memcpy(cpDest, ptr, iLen);      // copy the data
            if(SUB_DEBUG){
                cout<< DEV_AND_TIME << "[WriteSlaveCB] Received length :" << iLen <<endl;
                //cout << " Received data : " << cpDest <<endl;
            }
            
            nextState = IDLE;               // next state is idle for next request

//            if(g_initialize_end) testTrack.track( ((sIntReq_t*)(*iterEnd))->req.iId, ((sIntReq_t*)(*iterEnd))->req.iAddr, ((sIntReq_t*)(*iterEnd))->req.iLen, SSDmetric::SubReqMan, sc_time_stamp().value()/1000.0); //track 1 (write)

            if(IsReqBufFull() || IsDataBufFull(newReq.iLen)){
                // request or data buffer is full
                if(SUB_DEBUG){
                    cout << DEV_AND_TIME << "[WriteSlaveCB] data buffer is full"<<endl;
                }
                //trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
                RnBMaster.write(0);
            }

            break;
        default:
            ERROR_MSG("I cannot be in this state\n");
            sc_stop();
            break;
    }

    m_WSState = nextState;  // set my next state

    if(nextState == IDLE)
        m_eNewHostReq.notify();

    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
    //%USEREND WriteSlaveCB
} 

void
SubReqMan::CmdSlaveCB(                                                        //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)           //%
{                                                                               //%
    double BW_time_start = sc_time_stamp().value()/1000.0;
        
    const char* dbgMsgText = "TRANSPORT handler for memory R_DATA";              //%
    scml2::memory_alias< unsigned int >& m = this->CmdSlaveBuf_writeBuf;                             //%
    tlm::tlm_command cmd = trans.get_command();                                 //%
    sc_dt::uint64 adr = trans.get_address();                                    //%
    unsigned char* ptr = trans.get_data_ptr();                                  //%
    unsigned int len = trans.get_data_length();                                 //%
    unsigned char* byt = trans.get_byte_enable_ptr();                           //%
    unsigned int wid = trans.get_streaming_width();                             //%

                                                                                //%USERBEGIN CPU_Callback
    if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );
    // TODO: INSERT code for the CPU_Callback method here.

    if (byt) { // Target unable to support byte enable attribute
        trans.set_response_status( tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE );
        return;
    }
    if (wid < len) { // Target unable to support streaming width attribute
        trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
        return;
    }
    
    
    wait( len/CMD_BUS_TOTAL_BW, SC_NS);
    if (cmd == tlm::TLM_WRITE_COMMAND) { 
        // copy from 'ptr' to your target's memory.  e.g.: memcpy(&mem[adr], ptr, num_bytes);
        
        if(adr==0x0){ // this address notify information about software.
            if(*((unsigned int *)ptr)==1){
				if(SUB_DEBUG){
					cout<<DEV_AND_TIME<<"Cache buffer software is completed"<<endl;
				}
			    
                if(SOFTWARE_FLAG){
                    SOFTWARE_TIME[0] += sc_time_stamp().value()/1000.0 - SWtime_prev; 
                    SOFTWARE_count++;
                    SOFTWARE_FLAG = false;

                }
                Copy_SFR_To_CMDList(cmd_size);
			}
			else if(*((unsigned int *)ptr)==2){ //if Cache software initialization is completed
				if(SUB_DEBUG){
					cout<< "Cache software is ready"<<endl;
				}
				g_isCacheReady = true;
			}
			else if(*((unsigned int *)ptr)==3){ // if ftl software initialization is completed

				if(SUB_DEBUG){
					cout<< "Ftl software is ready"<<endl;
				}
				g_isFtlReady = true;
			}

        }
        else if(adr==0x14){ // write num of cmd which is completed in Cache buffer 
            cmd_size = *((unsigned int *)ptr);
        }
        else if(adr==0x18){
            
            //m_seWaitCacheRead.wait();
            ReadSubRespFromCache.Id = ((CacheCMD*)(ptr))->cmd.Id; 
            ReadSubRespFromCache.logical_addr = ((CacheCMD*)(ptr))->cmd.lba; 
            ReadSubRespFromCache.len = ((CacheCMD*)(ptr))->cmd.Len; 

            if(SUB_DEBUG){
                cout<<DEV_AND_TIME << "[CmdSlaveCB]Read Response from Cache " <<endl;
                //cout<<DEV_AND_TIME;
                cout<< DEV_AND_TIME << "[CmdSlaveCB]ReadSubRespFromCache Id = "<< ReadSubRespFromCache.Id << "\t";
                cout<<"  Lba = "<< ReadSubRespFromCache.logical_addr << "\t";
                cout<<"  Len = "<< ReadSubRespFromCache.len << "\t";
                cout<<endl;
            }
            Notified_Queue.push_back(ReadSubRespFromCache);
            m_Read_CacheData.notify(); // And notify ReadDataFromCache
        }
        else
        {
            //cout<<"cpu callback dram info"<<endl;   
            //cout<< *((unsigned int *)ptr) <<endl;
            CmdSlaveBuf_writeBuf[(adr/4)-7]= *((unsigned int *)ptr);
        }


    } else if (cmd == tlm::TLM_READ_COMMAND) {
        // copy from your target's memory to 'ptr', e.g.: memcpy(ptr, &mem[adr], num_bytes);
        
        std::list<sSubReq_t>::iterator iter;
        if(Seq_Op_Queue.size()>0){//if subreq is existed
            iter = Seq_Op_Queue.begin();
            //send to existed subreq send to CPU1
            //cout << "test : " << iter->iStartAddr << "  " << iter->iLen << "  " << iter->oriReq.Op << endl;
             

            if(adr==0x0){//Cmd size
                *((unsigned int*)ptr)=0xFF; // notify CPU1 that there is a request in the OP Queue
                SWtime_prev = sc_time_stamp().value()/1000.0;
                SOFTWARE_FLAG = true;
            }
            if(adr==0x4){//iAddr
                *((unsigned int*)ptr)=iter->iStartAddr;

                if (SUB_DEBUG) cout<<DEV_AND_TIME<<"[CmdSlaveCB]this operation iStartAddr = "<< iter->iStartAddr <<endl;
            }
            else if(adr==0x8){//iLen
                *((unsigned int*)ptr)=iter->iLen;
            }
            else if(adr==0xc){//Op
                *((unsigned int*)ptr)=iter->oriReq.Op;

                if(g_initialize_end) testTrack.track(iter->oriReq.iId, iter->iStartAddr, iter->iLen, SSDmetric::SubReqMan, sc_time_stamp().value()/1000.0 ); // track 2
                if (SUB_DEBUG) cout<<DEV_AND_TIME<<"[CmdSlaveCB]this operation code = "<< iter->oriReq.Op <<endl;
                Seq_Op_Queue.pop_front();
                if (SUB_DEBUG) cout<<DEV_AND_TIME<<"[CmdSlaveCB]Seq_Op_Queue is pop = "<< Seq_Op_Queue.size()<<endl;
            }
            else if(adr==0x20){//Cmd size
                *((unsigned int*)ptr)=Seq_Op_Queue.size();
                if(SUB_DEBUG)
                    cout<<DEV_AND_TIME<<"[CmdSlaveCB]Seq_Op_Queue & size = "<< Seq_Op_Queue.size()<<endl;

            }

        }
        else{
            //cout<<"No SubReq to handle!!! Wait for Queue"<<endl;
            if(adr==0x0){//Cmd size
                *((unsigned int*)ptr)=0xCC; // notify CPU1 that OP Queue is empty
            }
            if(adr==0x20){//Cmd size
                *((unsigned int*)ptr)=0; // this is not a request.
            }
        }
        //if nand queue is existed 
        //send to existed nand queue to CPU2
        std::list<sSubReqWithData_t>::iterator iter2;
        if(Nand_Queue.size()>0){
            if(SUB_DEBUG) cout<< DEV_AND_TIME << "[CmdSlaveCB] Nand Queue size : " << Nand_Queue.size()<<endl;
            iter2 = Nand_Queue.begin();
            if(adr==(CPU2_BASE + 0x0)){//OP
                *((unsigned int*)ptr)=iter2->subReq->oriReq.Op;
            }
            else if(adr==(CPU2_BASE + 0x4)){//locigal addr
                *((unsigned int*)ptr)=iter2->subReq->iStartAddr;
            }
            else if(adr==(CPU2_BASE + 0x8)){//length
                *((unsigned int*)ptr)=iter2->subReq->iLen;
            }
            else if(adr==(CPU2_BASE + 0xC)){//DRAM_addr
                *((unsigned int*)ptr)=iter2->subReq->cmd.cmd.Addr;
            }
            else if(adr==(CPU2_BASE + 0x10)){//DRAM_length
                *((unsigned int*)ptr)=iter2->subReq->cmd.cmd.Len;
            }
            else if(adr==(CPU2_BASE + 0x14)){//id
                *((unsigned int*)ptr)=iter2->subReq->Id;
                if(SUB_DEBUG)
                    cout<<DEV_AND_TIME<<"[CmdSlaveCB] iter2->subReq : oriReq.Op : "<< iter2->subReq->oriReq.Op << " iStartAddr "  << iter2->subReq->iStartAddr << " iLen " << iter2->subReq->iLen << " cmd.cmd.Addr  " << iter2->subReq->cmd.cmd.Addr << " cmd.cmd.Len  " << iter2->subReq->cmd.cmd.Len << " Id  " << iter2->subReq->Id <<endl;
               
                if(g_initialize_end) {
                    if (iter2->subReq->oriReq.Op == HOST_WRITE) testTrack.done(iter2->subReq->Id, iter2->subReq->iStartAddr, iter2->subReq->iLen, SSDmetric::SubReqMan, sc_time_stamp().value()/1000.0);
                } //write : should have total of 5 tracks
                  //read : donno yet

                delete iter2->subReq;
                Nand_Queue.pop_front( );
            }
        }
        else{
            //cout<<"No SubReq to handle!!! Wait for Queue"<<endl;
            if(adr==(CPU2_BASE + 0x0)){//OP
                *((unsigned int*)ptr)=2; // this is not a request.
            }
        }
    }
    
    if(g_isFtlReady && g_isCacheReady) CMD_BUS_TIME_CONSUMED[0] += sc_time_stamp().value()/1000.0 - BW_time_start;
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
    //%USEREND CPU_Callback
}   

void
SubReqMan::DataSlaveCB(                                                        //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)           //%
{                                                                               //%
    double BW_time_start = sc_time_stamp().value()/1000.0;
    
    const char* dbgMsgText = "TRANSPORT handler for memory R_DATA";              //%
    scml2::memory_alias< unsigned int >& m = this->DataSlaveBuf_writeBuf;                             //%
    tlm::tlm_command cmd = trans.get_command();                                 //%
    sc_dt::uint64 adr = trans.get_address();                                    //%
    unsigned char* ptr = trans.get_data_ptr();                                  //%
    unsigned int len = trans.get_data_length()/4;                                 //%
    unsigned char* byt = trans.get_byte_enable_ptr();                           //%
    unsigned int wid = trans.get_streaming_width();                             //%
    if (SUB_DEBUG) cout << DEV_AND_TIME << "[DataSlaveCB] with cmd : " << cmd << endl; 
                                                                                //%USERBEGIN CPU_Callback
    if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );
    // TODO: INSERT code for the CPU_Callback method here.

    if (byt) { // Target unable to support byte enable attribute
        trans.set_response_status( tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE );
        return;
    }
    if (wid < len) { // Target unable to support streaming width attribute
        trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
        return;
    }
    
    
    wait( len/DATA_BUS_TOTAL_BW, SC_NS); 
    if (cmd == tlm::TLM_WRITE_COMMAND) { 
        // copy from 'ptr' to your target's memory.  e.g.: memcpy(&mem[adr], ptr, num_bytes);

        
        if(adr==0x0){ // if read data is coming from NAND Manager
            ReadSubRespFromNand = *((sReadSubResp_t*)ptr);
            ReadSubRespFromNand.cpData = ((sReadSubResp_t *)ptr)->cpData;

            if (SUB_DEBUG) cout << DEV_AND_TIME << "[dataslavecb] from NAND " << endl; 
            
            SubReqResponse(ReadSubRespFromNand);
            if(SUB_DEBUG){
                if (SUB_DEBUG) cout<<DEV_AND_TIME<< "[dataslavecb] Recieved Read response from NAND. id : " << ReadSubRespFromNand.Id  <<" adr = "<< ReadSubRespFromNand.logical_addr <<"  Data = ";
                for(int i=0; i<ReadSubRespFromNand.len; i++)
                    if (SUB_DEBUG) cout<< ReadSubRespFromNand.cpData[i*SECTOR_SIZE_BYTE];
                if (SUB_DEBUG) cout<<endl;
            }

        }
        
    } else if (cmd == tlm::TLM_READ_COMMAND) {
        // copy from your target's memory to 'ptr', e.g.: memcpy(ptr, &mem[adr], num_bytes);
    }
    if(g_isFtlReady && g_isCacheReady) DATA_BUS_TIME_CONSUMED[0] += sc_time_stamp().value()/1000.0 - BW_time_start;
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
                                                                                //%USEREND CPU_Callback
}   


                                                                           //%
                                                                                //%
                                                                                //%USERBEGIN MEMBER_DEFNS
// TODO: Insert definitions of your member functions, etc. here.
                                                                                //%USEREND MEMBER_DEFNS
                                                                                //%
                                                                                //%
//----------------------------------------------------------------------        //%
// 'Orphaned' code section                                                      //%
//                                                                              //%
// This section contains code snippets saved from previous versions of          //%
// the model because they may have been user-written and they could             //%
// not be associated with any tagged code sections when the                     //%
// code was re-generated.                                                       //%
//                                                                              //%
// The most likely reason for this is that before the code was                  //%
// re-generated, a function name was changed. Now, the section of code          //%
// that was associated with the old name cannot be re-associated with           //%
// the new name.  In that case, the recommended course of actions is            //%
// to simply copy the relevant lines out of this section back into the          //%
// function whose name has been changed.                                        //%
#ifdef ORPHANED_CODE                                                            //%
                                                                                //%USERBEGIN ORPHANED_CODE

                                                                                //%USEREND ORPHANED_CODE
#endif // ORPHANED_CODE                                                         //%
                                                                                //%USERBEGIN FOOTER_H

                                                                                //%USEREND FOOTER_H
#endif // __SUBREQMAN_H__                                                       //%
