//--------------------------------------------------------------                //%
// Important:  The template for this file was generated                         //%
//  automatically and can only be re-generated if rules                         //%
//  are followed when editing:                                                  //%
// 1. Do not modify any line containing //%                                     //%
// 2. Only insert your lines of code between 2 lines                            //%
//    containing "//% user begin" and "//% user end"                            //%
//                                                                              //%
// :generate:date: Wed Jan 15 2014 11:26:30                                     //%
// :generate:version: 2010.1.0-dev                                              //%
//--------------------------------------------------------------                //%
                                                                                //%
#ifndef __NAND_MANAGER_H__                                                      //%
#define __NAND_MANAGER_H__                                                      //%
                                                                                //%USERBEGIN MODEL_DEBUG_ENABLE
#if defined(DEBUG) || defined(DBG_SCP)
# define DBG_SCP_NAND_MANAGER
#endif

#include <deque>                                                                //%USEREND MODEL_DEBUG_ENABLE
                                                                                //%
#include <scml2.h>                                                              //%
#include <systemc>                                                              //%
#include "scmlinc/scml_property.h"                                              //%
#include "scmlinc/scml_command_processor.h"                                     //%
#include "tlm_utils/simple_target_socket.h"                                     //%
#include "tlm.h"                                                                //%

                                                                                //%USERBEGIN HEADER_H
// TODO: Insert your includes, declarations, etc. here.
//#include "NAND_Struct.h"
#include "./header/global_flag.h"
#include "./header/ssd_struct.h"
#include "./header/memcpy.h"
#include "./header/NAND_MAN_Spec.h"
#include "./header/buffer_ctrl.h"

#define ABORT(cond,msg) {\
    if(cond){ \
        std::cout << "[" << sc_core::sc_time_stamp() << "] " <<  __FILE__ << ":" << __LINE__ << " " << msg << std::endl;   \
        sc_stop();} }

#define DEBUG(msg) { /* std::cout << "[" << this->basename() << " - " << sc_core::sc_time_stamp() << "] [" << __FUNCTION__ << "] " << msg << std::endl; */ }

#define BACKGROUND_GC_INTERVAL 5000 // in Nand Second


char filename[100];

//#define MAP_TABLE_CHECK // defined by HJ for debugging ******************************************************
//#define MAP_TABLE_DATA_CHECK

                                                                                //%USEREND HEADER_H
                                                                                //%
//------------------------                                                      //%
// Module Interface                                                             //%
//------------------------                                                      //%
                                                                                //%
template < unsigned int NUM_CHANNEL=1 >                                         //%
class NAND_Manager :                                                            //%
	public sc_core::sc_module                                               //%
                                                                                //%USERBEGIN INHERITANCE
    // TODO: Specify class inheritance here. Start each line with a , character
                                                                                //%USEREND INHERITANCE
                                                                                //%
{                                                                               //%
public:                                                                         //%
	// constructor                                                          //%
	NAND_Manager( const sc_core::sc_module_name & n); //skhds : parameters set here                     //%
	SC_HAS_PROCESS( NAND_Manager );                                         //%
	// destructor                                                           //%
	virtual ~NAND_Manager();                                                //%
private:                                                                        //%
	// disable default and copy constructors by making them private         //%
	NAND_Manager() ;                                                        //%
	NAND_Manager( const NAND_Manager& ) ;                                   //%
	NAND_Manager & operator= ( const NAND_Manager& ) ;                      //%
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
	sc_core::sc_in< bool > rst_n;                                       //%
	tlm::tlm_target_socket< 32 > Data_Slave;                                 //%
	scml2::initiator_socket< 32 > Data_Master;                               //%
    //scml2::initiator_socket< 32 > Irq_Master;                                       //%
    scml2::initiator_socket< 32 > Cmd_Master;                                  //%
    tlm::tlm_target_socket< 32 > Cmd_Slave;
	scml2::initiator_socket< 32 > NAND_Master[NUM_CHANNEL];                 //%
	sc_core::sc_in< unsigned int > NAND_RnB[NUM_CHANNEL];                   //%
protected:                                                                      //%
                                                                                //%
	// Parameter/Property declarations                                      //%
                                                                                //%
protected:                                                                      //%
                                                                                //%
	// Memory declarations                                                  //%
	scml2::memory< unsigned int > R_cmd;                                    //%
	scml2::memory< unsigned int > R_data;                   //%
                                                                                //%
	scml2::tlm2_gp_target_adapter< 32 > R_cmd_adapter;            //%
	scml2::tlm2_gp_target_adapter< 32 > R_data_adapter;            //%
                                                                                //%
public:                                                                         //%
	// Process declarations                                                 //%
	void Reset_Neg_Handler();                                                    //%
    void Cmd_Queue_Handler();
    void data_queue_thread();
    void Read_Data_From_Cache();
    void Init_Copy_Data_From_Cache();
    //%
protected:                                                                      //%
	// Memory callback declarations                                         //%
	void                                                                    //%
    data_callback(                                                 //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t);          //%
	void
    cmd_callback(                                                 //%
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

    // TODO: Insert declarations of member data and functions here.
    

    //
   //events
    sc_core::sc_event data_queue_event[NUM_CHANNEL];
    sc_core::sc_event copy_event;
    sc_core::sc_event cmd_queue_event;
    sc_core::sc_event fromDram_event;
    sc_core::sc_event initialization_done;
    sc_core::sc_event init_copy_event;
    sc_core::sc_event databuf_delete;
    sc_core::sc_event eNandQueueDelete[NUM_CHANNEL];
    sc_core::sc_event eCmdQueueDelete; 

    //parameters
    unsigned int NAND_CMD_QUEUE_SIZE = 4;
    unsigned int CMD_QUEUE_MAX_SIZE = 16;

    // Write Queue (it's actually data queue)
    NAND_Cmd_Buffer *NandWriteQueue[NUM_CHANNEL]; //Command Queue implemented in FIFO
    unsigned int iNandWriteQueueCount[NUM_CHANNEL]; //current queue size
    unsigned int iNandWriteQueueOutgoingPtr[NUM_CHANNEL]; //current index in queue
    unsigned int iSelectedChannel; //channel # of data queue thread


    // Command Queue
    std::list<NAND_Cmd_Buffer> CmdQueue;
    unsigned int    iCmdQueueCount;
    unsigned int    iCmdQueueOutgoingPtr;

    // 180423 ympark count needcopy event
    std::deque<int> NeedCopyCounts;
    int RDFC_Count;

    
    // Temporary Buffers
    unsigned char cNandDataBuffer[NUM_CHANNEL][PAGE_SIZE]; //data buffer
    bool bNandBufferUsed[NUM_CHANNEL];

    unsigned char cMapTableBuffer[PAGE_SIZE];
    bool mapTableFlag = false; //to sync with CPU2 and NAND_Manager
    bool onMapTableCommand[NUM_CHANNEL] = {false}; //data queue is issuing map table commands
    bool duringCPUtransfer = false; //CPU2 is transferring data
    sc_core::sc_event e_flagMT;
    unsigned int prevIPHead;

    unsigned int UsedBufSize;

    NAND_Cmd_Buffer Prev_Command[NUM_CHANNEL];
    bool RNM_READ_FLAG = false;
    unsigned int RNM_ADDR;

    NAND_Cmd_Buffer NandCmdBuffer; //command buffer
 

    std::list<NAND_Cmd_Buffer>::iterator FindCmdBufferDataWaiting()
    {
        std::list<NAND_Cmd_Buffer>::iterator iter;
        for(iter = CmdQueue.begin(); iter != CmdQueue.end(); iter++){
            /*if(NAND_DEBUG){
                cout<<DEV_AND_TIME << "[FindCmdBufferNeedCopy] Cache CMD : " ;
                cout<< iter->DramInfo.cmd.Id <<"\t";
                cout<< iter->DramInfo.cmd.Addr <<"\t";
                cout<< iter->DramInfo.cmd.Op <<"\t";
                cout<< iter->DramInfo.cmd.Len <<"\t";
                cout<< iter->state <<endl;
            }*/
            if(((iter->cmd.opCode == Program)|| (iter->cmd.opCode == Copyback_Program) )&&  (iter->state == DATA_WAITING) )             {
                if(NAND_DEBUG)
                    cout<<DEV_AND_TIME<<" [FindCmdBufferNeedCopy] return is occured" <<endl;
                return iter;
            }
        }
    }
    
    std::list<NAND_Cmd_Buffer>::iterator FindCmdBufferNeedCopy()
    {
        std::list<NAND_Cmd_Buffer>::iterator iter;
        for(iter = CmdQueue.begin(); iter != CmdQueue.end(); iter++){
            /*if(NAND_DEBUG){
                cout<<DEV_AND_TIME << "[FindCmdBufferNeedCopy] Cache CMD : " ;
                cout<< iter->DramInfo.cmd.Id <<"\t";
                cout<< iter->DramInfo.cmd.Addr <<"\t";
                cout<< iter->DramInfo.cmd.Op <<"\t";
                cout<< iter->DramInfo.cmd.Len <<"\t";
                cout<< iter->state <<endl;
            }*/
            if(((iter->cmd.opCode == Program)|| (iter->cmd.opCode == Copyback_Program) )&&  (iter->state == NEED_COPY) ) {
                if(NAND_DEBUG)
                    cout<<DEV_AND_TIME<<" [FindCmdBufferNeedCopy] return is occured" <<endl;
                return iter;
            }
        }
    }

    std::list<NAND_Cmd_Buffer>::iterator FindCmdBufferToDataQ()
    {
        std::list<NAND_Cmd_Buffer>::iterator iter;
        for(iter = CmdQueue.begin(); iter != CmdQueue.end(); iter++){
            if(iter->cmd.opCode == Read ||
               iter->cmd.opCode == Erase ||
               iter->cmd.opCode == Copyback ||
               iter->cmd.opCode == Copyback_Read ||
               ( (iter->cmd.opCode == Copyback_Program) &&  (iter->state == BUFFER_READY) ) ||
               ( (iter->cmd.opCode == Program) && (iter->state == BUFFER_READY) ))
                return iter;                    // should not move cmd to data queue
        }
    }

    
    bool reset_CmdBuffer(NAND_Cmd_Buffer buf){
        
        buf.cmd.opCode = Nothing;
        buf.cmd.iAddr1 = -1;
        buf.cmd.iAddr2 = -1; 
        buf.cmd.iAddr3 = -1;
        buf.cmd.iAddr4 = -1;
        buf.DramInfo.cmd.Op = -1;
        buf.DramInfo.cmd.Addr = -1;
        buf.DramInfo.cmd.Len = -1;
        buf.DramInfo.cmd.Id = -1;
        buf.state = NEED_COPY;
        return 0;    
    }

    inline unsigned int get_NAND_Channel(unsigned int addr)
    {
        //cout<< DEV_AND_TIME <<"MASK_CHANNEL : " << MASK_CHANNEL <<"\t";
        //cout<< "BIT_FOR_CHANNEL : " << BIT_FOR_CHANNEL <<"\t";
        //cout<< "NUM_OF_CHANNEL : " << NUM_OF_CHANNEL <<endl;
        return (addr & MASK_CHANNEL) >> BIT_FOR_CHANNEL & (NUM_OF_CHANNEL-1);
        //return 0;
        
    }
    inline unsigned int get_NAND_Way(unsigned int addr)
    {
        return (addr & MASK_WAY) >> BIT_FOR_WAY & (NUM_OF_WAY-1);
    }
    inline unsigned int set_NAND_CMD_Status(unsigned int status)
    {
        /*switch(status)
        {
            case BUFFER_BUSY:
                R_Bus_nand_cmd_status = R_Bus_nand_cmd_status | (1<<0);
                break;
            case BUFFER_READY:
                R_Bus_nand_cmd_status = R_Bus_nand_cmd_status & (0xffffffff-(1<<0));
                break;
            case MAP_CMD_BUSY:
                R_Bus_nand_cmd_status = R_Bus_nand_cmd_status | (1<<1);
                break;
            case MAP_CMD_READY:
                R_Bus_nand_cmd_status = R_Bus_nand_cmd_status & (0xffffffff-(1<<1));
                break;
        }*/
    }

    uint ValidCmdQSize()
    {
        uint size =0 ;

        if(NAND_DEBUG) cout<<DEV_AND_TIME<<"[ValitCmdQSize] Init \t";
        if(NAND_DEBUG) cout << CmdQueue.size()<<endl;
        if(!CmdQueue.empty()){
            std::list<NAND_Cmd_Buffer>::iterator iter;
            for(iter = CmdQueue.begin(); iter != CmdQueue.end(); iter++){
                if(iter->cmd.opCode == Read ||
                   iter->cmd.opCode == Erase ||
                   iter->cmd.opCode == Copyback ||
                   iter->cmd.opCode == Copyback_Read ||
                   ( (iter->cmd.opCode == Copyback_Program) &&  (iter->state == BUFFER_READY) ) ||
                   ( (iter->cmd.opCode == Program) && (iter->state == BUFFER_READY) ))                   
                    size++;
            }
        }
        if(NAND_DEBUG) cout<<DEV_AND_TIME<<"[ValidCmdQSize], complete size = "<<size<<endl;
        return size;
    }
    
    void PrintfCMDQ()
    {
        std::list<NAND_Cmd_Buffer>::iterator iter;
        for(iter = CmdQueue.begin(); iter != CmdQueue.end(); iter++){
            
            cout<< DEV_AND_TIME <<"[PrintfCMDQ]\t";
            cout<< iter->cmd.opCode <<"\t";
            cout<< iter->cmd.iAddr1 <<"\t";
            cout<< iter->cmd.iAddr3 <<"\t";
            cout<< iter->state <<"\t";
            cout<< iter->DramInfo.cmd.Id <<"\t";
            cout<< iter->DramInfo.cmd.Addr <<"\t";
            cout<< iter->DramInfo.cmd.Op <<endl;

        }
    }
    
    uint SizeofWriteCompleteQ()
    {
        uint size = 0;
        if(!CmdQueue.empty()){
            std::list<NAND_Cmd_Buffer>::iterator iter;
            for(iter = CmdQueue.begin(); iter != CmdQueue.end(); iter++){
                if(iter->state == WRITE_COMPLETE)
                    size++;
            }
        }
        return size;
    }

    std::list<NAND_Cmd_Buffer>::iterator FindWriteCompleteQ()
    {
        std::list<NAND_Cmd_Buffer>::iterator iter;
        for(iter = CmdQueue.begin(); iter != CmdQueue.end(); iter++){
            if(iter->state == WRITE_COMPLETE)
                return iter;
        }
        if(NAND_DEBUG)
            cout<<DEV_AND_TIME<<"[FindWriteCompleteQ], WriteComplete Q is not existed "<<endl;
    }





    void cbRead(NAND_Cmd_Buffer &NandCmdBuffer){

        if(g_initialize_end) testTrack.start(NandCmdBuffer.DramInfo.cmd.Id, NandCmdBuffer.cmd.iAddr3, 
                NandCmdBuffer.cmd.iAddr4, SSDmetric::NAND_Manager, sc_time_stamp().value()/1000.0);  

        CmdQueue.push_back(NandCmdBuffer);
        cmd_queue_event.notify();

    }

    void cbProgram(NAND_Cmd_Buffer &NandCmdBuffer){


        if(g_initialize_end) testTrack.start(NandCmdBuffer.DramInfo.cmd.Id, NandCmdBuffer.cmd.iAddr3,  
                NandCmdBuffer.cmd.iAddr4, SSDmetric::NAND_Manager, sc_time_stamp().value()/1000.0);  
        NandCmdBuffer.data = new unsigned char[PAGE_SIZE];
        NandCmdBuffer.state=NEED_COPY;
        UsedBufSize += PAGE_SIZE;

        CmdQueue.push_back(NandCmdBuffer);
        NeedCopyCounts.push_back(1); 
        init_copy_event.notify();
        // 180423 ympark needcopycounts were modified


    }

    void cbErase(NAND_Cmd_Buffer &NandCmdBuffer){

        CmdQueue.push_back(NandCmdBuffer);
        cmd_queue_event.notify();

    }


    void cbCopyback(NAND_Cmd_Buffer &NandCmdBuffer){

        CmdQueue.push_back(NandCmdBuffer);
        cmd_queue_event.notify();

    }



    void cbCopybackRead(NAND_Cmd_Buffer &NandCmdBuffer){

        RNM_ADDR = NandCmdBuffer.cmd.iAddr1;
        RNM_READ_FLAG = true;

    }

    void cbCopybackProgram(NAND_Cmd_Buffer &NandCmdBuffer){

        if(g_initialize_end) testTrack.start(NandCmdBuffer.DramInfo.cmd.Id, NandCmdBuffer.cmd.iAddr3,  
                NandCmdBuffer.cmd.iAddr4, SSDmetric::NAND_Manager, sc_time_stamp().value()/1000.0);  
        NandCmdBuffer.data = new unsigned char[PAGE_SIZE];
        NandCmdBuffer.state=NEED_COPY;
        UsedBufSize += PAGE_SIZE;

        NandCmdBuffer.cmd.iAddr2 = RNM_ADDR;
        RNM_READ_FLAG = false;

        CmdQueue.push_back(NandCmdBuffer);
        NeedCopyCounts.push_back(1); 
        init_copy_event.notify();
        // 180423 ympark needcopycounts were modified
    }

    void cbMapTableRead(NAND_Cmd_Buffer &NandCmdBuffer){


        unsigned int iChannel = get_NAND_Channel(NandCmdBuffer.cmd.iAddr1);

        onMapTableCommand[iChannel] = true;
        //Map table read has priority, so it will bypass CmdQueue and be sent directly to data_queue.
        //The idea is to save the current header, move the head to this particular request, then restore
        //header after the request is complete. Note that priority does not matter for map table read/write
        //since they cannot occur simultaneously (due to isSyncRequired in software).

        prevIPHead = iNandWriteQueueOutgoingPtr[iChannel]; //save current head

        int queue_index= (iNandWriteQueueCount[iChannel] + iNandWriteQueueOutgoingPtr[iChannel])%NAND_CMD_QUEUE_SIZE;
        
        if(iNandWriteQueueCount[iChannel] > 0){ //there is a command running, so wait for it to finish
            
            wait(eNandQueueDelete[iChannel]); //wait for the current command to finish
                                              //wait(5, SC_NS) at the end of data_queue_thread() will ensure 
                                              //this command will get priority after waking up
        }
        iNandWriteQueueCount[iChannel]++;
        iNandWriteQueueOutgoingPtr[iChannel] =  queue_index; //make it so that this command executes first
        memcpy(&NandWriteQueue[iChannel][queue_index], &NandCmdBuffer, sizeof(NAND_Cmd_Buffer));


        data_queue_event[iChannel].notify(); //  



    }


    void cbMapTableProgram(NAND_Cmd_Buffer &NandCmdBuffer){

        //Map table write has priority, so it will bypass CmdQueue and be sent directly to data_queue.
        //The idea is to save the current header, move the head to this particular request, then restore
        //header after the request is complete. Note that priority does not matter for map table read/write
        //since they cannot occur simultaneously (due to isSyncRequired in software).

        
        //%%%%%%%%%%%% need data at NandCmdBuffer.data, not yet implemented in software so wait%%%
        unsigned int iChannel = get_NAND_Channel(NandCmdBuffer.cmd.iAddr1);
        
        onMapTableCommand[iChannel] = true;
        
        prevIPHead = iNandWriteQueueOutgoingPtr[iChannel]; //save current head

        int queue_index= (iNandWriteQueueCount[iChannel] + iNandWriteQueueOutgoingPtr[iChannel])%NAND_CMD_QUEUE_SIZE;
        
        if(iNandWriteQueueCount[iChannel] > 0){ //there is a command running, so wait for it to finish
            
            wait(eNandQueueDelete[iChannel]); //wait for the current command to finish
                                              //wait(5, SC_NS) at the end of data_queue_thread() will ensure 
                                              //this command will get priority after waking up
        }
        iNandWriteQueueCount[iChannel]++;
        iNandWriteQueueOutgoingPtr[iChannel] =  queue_index; //make it so that this command executes first
        memcpy(&NandWriteQueue[iChannel][queue_index], &NandCmdBuffer, sizeof(NAND_Cmd_Buffer));


        data_queue_event[iChannel].notify(); //  
    
    }


    void cbMapTableCopyback(NAND_Cmd_Buffer &NandCmdBuffer){
        
        unsigned int iChannel = get_NAND_Channel(NandCmdBuffer.cmd.iAddr1);

        onMapTableCommand[iChannel] = true;
        
        prevIPHead = iNandWriteQueueOutgoingPtr[iChannel]; //save current head

        int queue_index= (iNandWriteQueueCount[iChannel] + iNandWriteQueueOutgoingPtr[iChannel])%NAND_CMD_QUEUE_SIZE;
        
        if(iNandWriteQueueCount[iChannel] > 0){ //there is a command running, so wait for it to finish
            
            wait(eNandQueueDelete[iChannel]); //wait for the current command to finish
                                              //wait(5, SC_NS) at the end of data_queue_thread() will ensure 
                                              //this command will get priority after waking up
        }
        iNandWriteQueueCount[iChannel]++;
        iNandWriteQueueOutgoingPtr[iChannel] =  queue_index; //make it so that this command executes first
        memcpy(&NandWriteQueue[iChannel][queue_index], &NandCmdBuffer, sizeof(NAND_Cmd_Buffer));


        data_queue_event[iChannel].notify(); //  

    }

    void cbMapTableErase(NAND_Cmd_Buffer &NandCmdBuffer){
        
        unsigned int iChannel = get_NAND_Channel(NandCmdBuffer.cmd.iAddr1);

        onMapTableCommand[iChannel] = true;
        
        prevIPHead = iNandWriteQueueOutgoingPtr[iChannel]; //save current head

        int queue_index= (iNandWriteQueueCount[iChannel] + iNandWriteQueueOutgoingPtr[iChannel])%NAND_CMD_QUEUE_SIZE;
        
        if(iNandWriteQueueCount[iChannel] > 0){ //there is a command running, so wait for it to finish
            
            wait(eNandQueueDelete[iChannel]); //wait for the current command to finish
                                              //wait(5, SC_NS) at the end of data_queue_thread() will ensure 
                                              //this command will get priority after waking up
        }
        iNandWriteQueueCount[iChannel]++;
        iNandWriteQueueOutgoingPtr[iChannel] =  queue_index; //make it so that this command executes first
        memcpy(&NandWriteQueue[iChannel][queue_index], &NandCmdBuffer, sizeof(NAND_Cmd_Buffer));


        data_queue_event[iChannel].notify(); //  

    }


    uint CPU_data_start_addr;
    uint CPU_data_end_addr;

    void _init_CPUDataTransport(uint start_addr, uint len){
            
            //cout << "init CDFC" << endl;
            CPU_data_start_addr = start_addr;
            CPU_data_end_addr = start_addr + len;
    }


    bool CollectDataFromCPU(uchar* dst, uchar* src, uint adr){
        

        assert( (adr >= CPU_data_start_addr) && (adr < CPU_data_end_addr)  );//wrong adr
        memcpy(dst + adr - CPU_data_start_addr, src, 4); 
        return (adr + 4) == CPU_data_end_addr;
    }


    bool SendDataToCPU(uchar* dst, uchar* src, uint adr){
        

        assert( (adr >= CPU_data_start_addr) && (adr < CPU_data_end_addr)  );//wrong adr
        memcpy(dst,  src + adr - CPU_data_start_addr, 4); 
        return (adr + 4) == CPU_data_end_addr;
    }

};                                                                              //%
                                                                                //%
//------------------------                                                      //%
// Module Implementation                                                        //%
//------------------------                                                      //%
                                                                                //%USERBEGIN HEADER_CPP
// TODO: Insert your includes, declarations, etc. here.
                                                                                //%USEREND HEADER_CPP
                                                                                //%
                                                                                //%
template < unsigned int NUM_CHANNEL >                                           //%
NAND_Manager< NUM_CHANNEL >::NAND_Manager( const sc_core::sc_module_name & n)   //%
		: sc_core::sc_module(n)                                         //%
,                                                                               //%
 rst_n("rst_n"),                                                        //%
 Data_Slave("Data_Slave"),                                                        //%
 Data_Master("Data_Master"),                                                      //%
 //Irq_Master("Irq_Master"),                                                          //%
 Cmd_Master("Cmd_Master"),                                                //%
 Cmd_Slave("Cmd_Slave"),
 
    
 R_cmd_adapter("R_cmd_adapter", Cmd_Slave),                 //%
 R_data_adapter("R_data_adapter", Data_Slave),                 //%
 R_cmd("R_cmd", 40960ULL),         //20180704 increased command bus register (orig : 266ULL)//%
  R_data("R_data",4160ULL)                         //%
                                                                                //%
    // TODO: Add your class initialization here.
                                                                                //%USEREND INITIALIZER
                                                                                //%
{                                                                               //%
 // declare processes and their sensitivities                                   //%
	SC_THREAD(Reset_Neg_Handler);                                           //%
	 sensitive << rst_n.neg() ;                                         //%
	 dont_initialize();                                                     //%

    for(unsigned int i=0; i<NUM_CHANNEL; i++){
        SC_THREAD(data_queue_thread);
         sensitive << data_queue_event[i];
         dont_initialize();
    }

	SC_THREAD(Read_Data_From_Cache);
	 sensitive << fromDram_event;
	 dont_initialize();

	SC_THREAD(Cmd_Queue_Handler);
	 sensitive << cmd_queue_event;
	 dont_initialize();

    SC_THREAD(Init_Copy_Data_From_Cache); 
     sensitive << init_copy_event;
     dont_initialize();
//    SC_THREAD(FTL_Background_Operation_Handler);
//        sensitive << eNandFinish;
//        dont_initialize();
                                                                                //%
	// bind target ports to memories                                        //%
	R_cmd_adapter(R_cmd);                                         //%
	R_data_adapter(R_data);                                         //%
                                                                                //%
 // setup memory callbacks                                                      //%
	// - Memory: R_Bus                                                      //%
	scml2::set_callback(                                                    //%
		R_cmd,                                                      //%
		SCML2_CALLBACK(cmd_callback),                         //%
		scml2::AUTO_SYNCING);                                           //%
	
    scml2::set_callback(                                                    //%
		R_data,                                                      //%
		SCML2_CALLBACK(data_callback),                         //%
		scml2::AUTO_SYNCING);                                           //%
                                                                                //%
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

     
    for (int i=0; i<NUM_CHANNEL;i++){
        NandWriteQueue[i] = new NAND_Cmd_Buffer[NAND_CMD_QUEUE_SIZE];   
    }
                                                                                //%USEREND CONSTRUCTOR
                                                                                //%
}                                                                               //%
// destructor                                                                   //%
                                                                                //%
template < unsigned int NUM_CHANNEL >                                           //%
NAND_Manager< NUM_CHANNEL >::~NAND_Manager()                                    //%
{                                                                               //%
                                                                                //%USERBEGIN DESTRUCTOR

    for (int i=0; i<NUM_CHANNEL;i++){
        delete[] NandWriteQueue[i];   
    }
    // TODO: Add destructor code here.
                                                                                //%USEREND DESTRUCTOR
}                                                                               //%
// initialization method for ports                                              //%
                                                                                //%
template < unsigned int NUM_CHANNEL >                                           //%
void                                                                            //%
NAND_Manager< NUM_CHANNEL >::initPorts()                                        //%
{                                                                               //%
}                                                                               //%
// initialization method for registers                                          //%
                                                                                //%
template < unsigned int NUM_CHANNEL >                                           //%
void                                                                            //%
NAND_Manager< NUM_CHANNEL >::initRegisters()                                    //%
{                                                                               //%

	R_cmd.initialize(0x0);                                               //%
	R_data.initialize(0x0);                                          //%

}                                                                               //%
//This method is invoked after the design is fully elaborated, but              //%
// before simulation begins.                                                    //%
                                                                                //%
template < unsigned int NUM_CHANNEL >                                           //%
void                                                                            //%
NAND_Manager< NUM_CHANNEL >::end_of_elaboration()                               //%
{                                                                               //%
                                                                                //%USERBEGIN END_OF_ELAB
    // TODO: Add end_of_elaboration code here.
    this->initRegisters();
    this->initPorts();
                                                                                //%USEREND END_OF_ELAB
}                                                                               //%
// command processing method                                                    //%
//  During simulation, this command may be called to handle                     //%
//  interactive commands -- for dynamic instrumentation of the instance.        //%
                                                                                //%
template < unsigned int NUM_CHANNEL >                                           //%
std::string                                                                     //%
NAND_Manager< NUM_CHANNEL >::commandProcessor(const std::vector< std::string >& cmd)//%
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
        }                                                                //%
 // declare proces
    }
    return outs.str();
                                                                                //%USEREND CMD_PROCESSOR
}                                                                               //%
                                                                                //%
// initialization method for logging messages                                   //%
                                                                                //%
template < unsigned int NUM_CHANNEL >                                           //%
                                                                                //%
void                                                                            //%
NAND_Manager< NUM_CHANNEL >::initMessageLogging()                               //%
{                                                                               //%
                                                                                //%USERBEGIN MODEL_DEBUG_INIT
    // toggle on/off debugging flags
    for (int i=0;i<sizeof(DbgCategories);i++) {
#ifdef DBG_SCP_NAND_MANAGER
        dbgFlag[i] = true;
#else
        dbgFlag[i] = false;
#endif
    }
                                                                                //%USEREND MODEL_DEBUG_INIT
                                                                                //%
}                                                                               //%
// method for logging messages                                                  //%
template < unsigned int NUM_CHANNEL >                                           //%
                                                                                //%
void                                                                            //%
NAND_Manager< NUM_CHANNEL >::REPORT_MSG(std::string sev, std::string msg)       //%
{                                                                               //%
                                                                                //%USERBEGIN REPORT_MSG
    std::cout << sev << ": " << msg << " [" << this->basename() << "@" << sc_core::sc_time_stamp() << "]" << std::endl;
                                                                                //%USEREND REPORT_MSG
                                                                                //%
}                                                                               //%
                                                                                //%
template < unsigned int NUM_CHANNEL >                                           //%
void                                                                            //%
NAND_Manager< NUM_CHANNEL >::Reset_Neg_Handler()                                //%
                                                                                //%USERBEGIN Reset_Neg_Handler
{

    if ( dbgFlag[PORTS] ) DBG_MSG( "Reset_Neg_Handler invoked because of event on port Reset_Neg" );
    
    while(1){
        
        wait(SC_ZERO_TIME);
        
        //initialize variables
        //
        
        //iCmdQueueCount = 0;
        RNM_READ_FLAG = false;
        UsedBufSize = 0; 
        iCmdQueueCount = 0;
        iCmdQueueOutgoingPtr = 0;
        reset_CmdBuffer(NandCmdBuffer);
        RDFC_Count = 0;
        for(unsigned int i=0; i<NUM_CHANNEL; i++){
            
           // iNandQueueOutgoingPtr[i] = 0;
            iNandWriteQueueCount[i] = 0;
            iNandWriteQueueOutgoingPtr[i] = 0;
            
            //  bNandDataBufferUsed[i] = false;
        }     
        //wake up threads at the beginning
        
        for(int i=0; i<NUM_CHANNEL; i++){
            
            iSelectedChannel = i; //assign channel number to each threads
            data_queue_event[i].notify(); //start data queue threads
            wait(10, SC_NS);
        }
        wait();
    }



}

                                                                                //%USEREND Reset_Neg_Handler
                                                                                //%

template < unsigned int NUM_CHANNEL >                                           //%
void                                                                            //%
NAND_Manager< NUM_CHANNEL >::cmd_callback(                            //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)           //%
{         
    double BW_time_start = sc_time_stamp().value()/1000.0;
    //pack subreq command and send it to NAND cmd queue
    //
    scml2::memory< unsigned int >& m = this->R_cmd;                            //%
    tlm::tlm_command cmd = trans.get_command();                                 //%
    sc_dt::uint64 adr = trans.get_address();                                    //%
    unsigned char* ptr = trans.get_data_ptr();   //command type : SubReq_t at header/ssd_struct.h        
    unsigned int len = trans.get_data_length();                                 //%
    unsigned char* byt = trans.get_byte_enable_ptr();  //not used                        //%
    unsigned int wid = trans.get_streaming_width();    //not used               //%
    
    if (byt) { // Target unable to support byte enable attribute
        trans.set_response_status( tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE );
        return;
    }
    if (wid < len) { // Target unable to support streaming width attribute
        trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
        return;
    }
  
    
    //wait( len/CMD_BUS_TOTAL_BW, SC_NS);
    if( cmd == tlm::TLM_WRITE_COMMAND) {
        if(NAND_DEBUG){
            cout<<DEV_AND_TIME<<"[CPU2 send] ftl result is arrived on NAND Manager";
            switch (adr){
                case _OFFSET_DRAM_ADDR_ : 
                    cout << " & DRAM ADDR = ";
                    break;
                case _OFFSET_DRAM_LENGTH_ : 
                    cout << " & DRAM LENGTH = ";
                    break;
                case _OFFSET_DRAM_REQ_ID_ : 
                    cout << " & Original ID = ";
                    break;
                case _OFFSET_NAND_CMD_ : 
                    cout << " & CMD opcode = ";
                    break;
                case _OFFSET_NAND_ADDR1_ : 
                    cout << " & PBA = ";
                    break;
                case _OFFSET_NAND_ADDR2_ : 
                    cout << " & Nothing = ";
                    break;
                case _OFFSET_NAND_ADDR3_ : 
                    cout << " & LBA = ";
                    break;
                case _OFFSET_NAND_ADDR4_ : 
                    cout << " & Sub Request Length = ";
                    break;
                default : 
                    cout << " & other = ";
                    break;
            }
            cout<<*((unsigned int*)ptr)<<endl;
        }
        if(adr == _ADDR_DRAM_STATEINFORM_) {// Command from DRAM notifying that it is ready to transmit data 
            RDFC_Count++;
            fromDram_event.notify(); //dram is ready... notify cmd thread 
        }
        else if((adr >= _OFFSET_MAP_TABLE_DATA_) && (adr < _OFFSET_MAP_TABLE_DATA_ + _OFFSET_MAP_TABLE_LEN_)){

            //receiving maptable data from CPU2
            //CPU2 seems to send one word at a time only, so data is going to be collected one word at a time.
            //%%get_streaming_width() might allow the callback to receive the whole struct (not sure tho) 
            
            if(adr == _OFFSET_MAP_TABLE_DATA_) {
                _init_CPUDataTransport(_OFFSET_MAP_TABLE_DATA_, _OFFSET_MAP_TABLE_LEN_);
                duringCPUtransfer = true;
            }
            if(CollectDataFromCPU(cMapTableBuffer, (uchar*)ptr, adr)) {
                duringCPUtransfer = false; 
                e_flagMT.notify(); //make sure this only operates on maptable program
            }
            
        }
        else if(adr==_OFFSET_DRAM_ADDR_) { //command packet is being received

            NandCmdBuffer.DramInfo.cmd.Addr = *((unsigned int*)ptr);
        }  
        else if(adr==_OFFSET_DRAM_LENGTH_) {

            NandCmdBuffer.DramInfo.cmd.Len = *((unsigned int*)ptr); 
        }    
        else if(adr==_OFFSET_DRAM_REQ_ID_) {
            
            NandCmdBuffer.DramInfo.cmd.Id = *((unsigned int*)ptr); 
        }
        else if(adr==_OFFSET_NAND_CMD_) {
        
            NandCmdBuffer.cmd.opCode = (NAND_CMD_SET) Erase; 
            NandCmdBuffer.DramInfo.cmd.Op = (NAND_CMD_SET)(*((unsigned int*)ptr)); 
            NandCmdBuffer.cmd.opCode = (NAND_CMD_SET)(*((unsigned int*)ptr)); 
        }   
        else if(adr==_OFFSET_NAND_ADDR1_) {
            
            NandCmdBuffer.cmd.iAddr1 = *((unsigned int*)ptr); 
        } 
        else if(adr==_OFFSET_NAND_ADDR2_) {
        
            NandCmdBuffer.cmd.iAddr2 = *((unsigned int*)ptr); 
        }
        else if(adr==_OFFSET_NAND_ADDR3_) { // include logical addr
        
            NandCmdBuffer.DramInfo.cmd.lba = *((unsigned int*)ptr); 
            NandCmdBuffer.cmd.iAddr3 = *((unsigned int*)ptr); 
        }
        else if(adr==_OFFSET_NAND_ADDR4_) { // include request length 
        //all packets for NAND command received
            NandCmdBuffer.cmd.iAddr4 = *((unsigned int*)ptr); 
            if(NAND_DEBUG){
                cout<<DEV_AND_TIME<<"[CPU2 send] Cmd Queue is inserted & this Cmd op = "<<NandCmdBuffer.cmd.opCode ;
                cout<< "  Cmd id = "<<NandCmdBuffer.DramInfo.cmd.Id ;
                cout<< "  Cmd addr = "<<NandCmdBuffer.cmd.iAddr1 << endl;
            }
           
            //if((NandCmdBuffer.cmd.opCode != 1) && (NandCmdBuffer.cmd.opCode != 2) ) cout << "[NAND Manager] Checking for op code.... " << NandCmdBuffer.cmd.opCode << endl;

            if(RNM_READ_FLAG && (NandCmdBuffer.cmd.opCode == Program)) NandCmdBuffer.cmd.opCode = Copyback_Program;
                
            switch(NandCmdBuffer.cmd.opCode){

                case Read : 
                    
                    cbRead(NandCmdBuffer);

                    break;
                
                case Program :
                    
                    cbProgram(NandCmdBuffer);
                    
                    break;

                case Erase :
                    
                    cbErase(NandCmdBuffer);
                
                    break;
                
                case Copyback :

                    cbCopyback(NandCmdBuffer);
                    
                    break;

                case Copyback_Read : 
                    
                    cbCopybackRead(NandCmdBuffer);

                    break;

                case Copyback_Program : 
                    
                    cbCopybackProgram(NandCmdBuffer);
                    
                    break;

                case MapTable_Read :

                    cbMapTableRead(NandCmdBuffer);

                    break;

                case MapTable_Program :
                    
                    cbMapTableProgram(NandCmdBuffer);
                    
                    break;

                case MapTable_Copyback :

                    cbMapTableCopyback(NandCmdBuffer);

                    break;

                case MapTable_Erase : 

                    cbMapTableErase(NandCmdBuffer);

                    break;

                default :

                    assert(0 && NandCmdBuffer.cmd.opCode); //opcode not considered

            }


            
        }else{
            cout << " adr = " << hex <<adr << dec <<endl;
            cout <<"errrrrrrrrrrrrrrrrrrrrrrrrrrrr"<<endl;
            assert(0);
            sc_stop();
        }
            
    } //transports only implemented in "write"
    else if ( cmd == tlm::TLM_READ_COMMAND ) {
        if(adr == 0x3C ){ // callback write complete request size;
            //if(NAND_DEBUG)
            //    cout<<DEV_AND_TIME<<"[cmd_callback] Write complete Cmd Queue size is read by CPU1" <<endl;
            *((unsigned int *)ptr) = SizeofWriteCompleteQ();
        }
        else if(adr == 0x40){ // callback lba of write complete request and erase request on CmdQueue
            if(SizeofWriteCompleteQ() > 0){
                std::list<NAND_Cmd_Buffer>::iterator iter;
                iter = FindWriteCompleteQ();
                *((unsigned int *)ptr) = iter->DramInfo.cmd.lba;
                CmdQueue.erase(iter);
                //eCmdQueueDelete.notify();
                //if(NAND_DEBUG) PrintfCMDQ();
            }
        }
        else if(adr == _OFFSET_R_BUS_NAND_FULL_){
            
             *((unsigned int *)ptr) = (CmdQueue.size() >= CMD_QUEUE_MAX_SIZE); // NAND full
        }
        else if((adr >= _OFFSET_MAP_TABLE_DATA_) && (adr < _OFFSET_MAP_TABLE_DATA_ + _OFFSET_MAP_TABLE_LEN_)){

            //receiving maptable data from CPU2
            //CPU2 seems to send one word at a time only, so data is going to be collected one word at a time.
            //%%get_streaming_width() might allow the callback to receive the whole struct (not sure tho) 
            
            if(adr == _OFFSET_MAP_TABLE_DATA_) {
                _init_CPUDataTransport(_OFFSET_MAP_TABLE_DATA_, _OFFSET_MAP_TABLE_LEN_);
            
            }
            if(SendDataToCPU((uchar*)ptr, cMapTableBuffer, adr)) e_flagMT.notify();
            
        
        }
        /*else if(adr == _OFFSET_MAP_TABLE_DATA_){ 

            cout << "data : " << ptr << endl;
            memcpy(ptr, cNandDataBuffer[mapTableReadChannel], 12);
            e_flagRead.notify();

        }*/
        else if(adr == _OFFSET_SYNC_REQUIRED_){

            //cout << "flag poll" << endl;
            *((unsigned int *) ptr) = mapTableFlag; // != 0x0
            mapTableFlag = false; 
        }
    
    }
    if(g_initialize_end) CMD_BUS_TIME_CONSUMED[2] += sc_time_stamp().value()/1000.0 - BW_time_start;
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
}                                                                               //%USEREND Bus_Interrupt_Callback

template < unsigned int NUM_CHANNEL >                                           //%
void                                                                            //%
NAND_Manager< NUM_CHANNEL >::data_callback(                            //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)           //%
{      

    
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // successful completion
                                                                                //%userend bus_interrupt_callback
}                                                                               //%
                                                                                //%
                                                                                //%
                                                                                //%USERBEGIN MEMBER_DEFNS
                                                                                //
template < unsigned int NUM_CHANNEL >
void NAND_Manager< NUM_CHANNEL >::Init_Copy_Data_From_Cache() // only for "HOST_WRITE"
{
    while(1){
        // ympark 180423 needcopycounts were modified
        while(NeedCopyCounts.size() > 0){
            
            std::list<NAND_Cmd_Buffer>::iterator iter;
            iter = FindCmdBufferNeedCopy();
            if(NAND_DEBUG){
                cout << DEV_AND_TIME << "[Init_Copy_Data_From_Cache] NeedCopyCounts are "<< NeedCopyCounts.size()<<endl;
                cout<<DEV_AND_TIME<<"[Init_Copy_Data_From_Cache] need to copy data from cache, logical addr : " << iter->cmd.iAddr3 <<endl;
                cout<<DEV_AND_TIME<<"[Init_Copy_Data_From_Cache] Cache CMD : " ;
                cout<< iter->DramInfo.cmd.Id <<"\t";
                cout<< iter->DramInfo.cmd.Addr <<"\t";
                cout<< iter->DramInfo.cmd.Op <<"\t";
                cout<< iter->DramInfo.cmd.Len <<"\t";
                cout<< iter->DramInfo.cmd.Id <<endl;
            }
            //if queue is empty, wait
            //if(!iCmdQueueCount) wait();

            //Data buffer reached the maximum.. lock it
            //if(UsedBufSize >= NAND_DATA_BUFFER_MAX_SIZE ) wait(databuf_delete);//
            if(!Cmd_Master.write(_ADDR_DRAM_BASE_ + _CMDMASTER_OFFSET_NANDSIDE_ + CACHE_DATA_REGION_OFFSET , (void*)&(iter->DramInfo.cmd) , sizeof(sCacheTrans_t))){
                //wait(SC_ZERO_TIME);
                //wait(5, SC_NS);
                assert(0);
            } //send request to dram

            iter->state = DATA_WAITING;

            NeedCopyCounts.pop_front();

        }
        wait();
    }
}

template < unsigned int NUM_CHANNEL >
void NAND_Manager< NUM_CHANNEL >::Cmd_Queue_Handler() {
    while(1){
        int queue_index;
        if(NAND_DEBUG)
            cout<< DEV_AND_TIME << "[Cmd_Queue_Handler] is awakend" <<endl;
        while(ValidCmdQSize() > 0){
            //cout<< DEV_AND_TIME << "Valid cmd q size : " << ValidCmdQSize() <<endl;

            if(NAND_DEBUG) cout<< DEV_AND_TIME << "[Cmd_Queue_Handler] is operated" <<endl;
            std::list<NAND_Cmd_Buffer>::iterator req_iter;
            req_iter=FindCmdBufferToDataQ();
            
           if(NAND_DEBUG)  cout<< DEV_AND_TIME << "[Cmd_Queue_Handler] is operated on iter id "<< req_iter->DramInfo.cmd.Id << ", logical addr : " << req_iter->cmd.iAddr3 <<endl;
            unsigned int iChannel = get_NAND_Channel(req_iter->cmd.iAddr1);
            unsigned int iWay = get_NAND_Way(req_iter->cmd.iAddr1);//not done
            if(NAND_DEBUG) cout<<DEV_AND_TIME <<"[Cmd_Queue_Handler] iChannel & iWay : "<<iChannel <<"\t"<< iWay<<endl;

            //block on queue full or during map table commands..          
            while((iNandWriteQueueCount[iChannel] >= NAND_CMD_QUEUE_SIZE) || onMapTableCommand[iChannel]){ 
                
                //cout << DEV_AND_TIME << "[Cmd_Queue_Handler] NAND Queue full" << endl;
                wait(eNandQueueDelete[iChannel]); 
                //sc_stop();

            }
            
            queue_index= (iNandWriteQueueCount[iChannel] + iNandWriteQueueOutgoingPtr[iChannel])%NAND_CMD_QUEUE_SIZE;
            iNandWriteQueueCount[iChannel]++;
            memcpy(&NandWriteQueue[iChannel][queue_index], &*req_iter, sizeof(NAND_Cmd_Buffer));
                       
            
            data_queue_event[iChannel].notify(); // warning, this point may make error, because of overlapped notify

            
            //cout<<DEV_AND_TIME << "opcode : "<< iter->cmd.opCode <<endl;
            if((req_iter->cmd.opCode == Program)||(req_iter->cmd.opCode == Copyback_Program)){
                req_iter->state = WRITE_COMPLETE;
                req_iter=CmdQueue.begin();
                if(NAND_DEBUG){
                    cout<<DEV_AND_TIME<<"this command state is modified to WRITE_COMPLETE"<<endl;
                    cout<<DEV_AND_TIME<<"CmdQueue begin state : "<<req_iter->state<<endl;
                }
            }
            else{
                CmdQueue.erase(req_iter);
                //eCmdQueueDelete.notify();
            }
            wait(5, SC_NS);   
            if(NAND_DEBUG) cout<< DEV_AND_TIME << "[Cmd_Queue_Handler] is completed "<< req_iter->DramInfo.cmd.Id <<endl;
        }
        wait();
    }
}

template < unsigned int NUM_CHANNEL >
void NAND_Manager< NUM_CHANNEL >::Read_Data_From_Cache() // only for "HOST_WRITE"
{
    while(1){
        std::list<NAND_Cmd_Buffer>::iterator iter;
        while(RDFC_Count){

            iter = FindCmdBufferDataWaiting();
            if(NAND_DEBUG)  
                cout<< DEV_AND_TIME <<"[Read_Data_From_Cache]data read operation is ready, ID : " << iter->DramInfo.cmd.Id << "  read addr(logical) : " << iter->cmd.iAddr3 << " read length : "<< iter->DramInfo.cmd.Len <<endl;
            unsigned char tempReadData[DATA_PAGE_SIZE+1]= {0,};
            Data_Master.read(_ADDR_DATA_DRAM_ + _DATAMASTER_OFFSET_NANDSIDE_ , tempReadData, iter->DramInfo.cmd.Len ); //read data from dram //    cNandDataBuffer is not protected with any locks... 

            memcpy(iter->data, tempReadData, iter->DramInfo.cmd.Len);
            if(NAND_DEBUG){
                cout<< DEV_AND_TIME <<"[Read_Data_From_Cache] ID : " << iter->DramInfo.cmd.Id << "  read addr(logical) : " << iter->cmd.iAddr3 << "  read data = ";
                for(int i =0; i < iter->DramInfo.cmd.Len/SECTOR_SIZE_BYTE; i++){
                    cout << iter->data[i * SECTOR_SIZE_BYTE];
                }
                cout<<endl;
                cout<< DEV_AND_TIME <<"[Read_Data_From_Cache]read data is completed "<<endl;
            }

            if((iter->cmd.opCode == Program)|| (iter->cmd.opCode == Copyback_Program)){
                iter->state = BUFFER_READY;
            }
            reset_CmdBuffer(NandCmdBuffer);
            RDFC_Count--;
            cmd_queue_event.notify();
            wait(SC_ZERO_TIME);

        }
        wait();
    }

}

// todo: insert definitions of your member functions, etc. here.
template < unsigned int NUM_CHANNEL >
void NAND_Manager< NUM_CHANNEL >::data_queue_thread()
{

     
    unsigned int iChannel = iSelectedChannel;
    unsigned int *ipCount = &iNandWriteQueueCount[iChannel];
    unsigned int *ipHead = &iNandWriteQueueOutgoingPtr[iChannel];
    
    double idle_time_end = sc_time_stamp().value()/1000.0;//skhds
    double idle_time_start = sc_time_stamp().value()/1000.0;//skhds
    double idle_time_interval = sc_time_stamp().value()/1000.0;//skhds
    while(1){

        //skhds
        if(!(*ipCount)) {
            
            idle_time_start = sc_time_stamp().value()/1000.0;
            wait(); //Queue is empty... wait for transport

            idle_time_interval = sc_time_stamp().value()/1000.0 - idle_time_end;
        
            idle_time_end = sc_time_stamp().value()/1000.0;
            //cout << DEV_AND_TIME << "Idle time at Channel " << iChannel << " : " << idle_time_end - idle_time_start << " , percentage : " << (idle_time_end - idle_time_start)/idle_time_interval << endl; 
        }
        //

        if(NAND_DEBUG)  
            cout << DEV_AND_TIME << "[data_queue_thread] being executed on channel " << iChannel << " at index " << *ipHead << " with Queue Count of " << *ipCount << endl;
        //pop cmd from NandQueue
        NAND_Cmd_Buffer* currentCmd = &NandWriteQueue[iChannel][*ipHead]; //get command from queue(FIFO)
        
        unsigned int iWay = get_NAND_Way(currentCmd->cmd.iAddr1);//not done
        while((NAND_RnB[iChannel].read()&(1<<iWay))==0){ //wait for RnB
            DEBUG("[Ch " << iChannel << " Wy " << iWay << "] Waiting for NAND RnB. Current RnB is " << NAND_RnB[iChannel].read());
            wait(NAND_RnB[iChannel].value_changed_event());
        }


        NAND_Cmd current_NAND_Cmd;
        uint offset;
        memcpy(&current_NAND_Cmd, &(currentCmd->cmd), sizeof(NAND_Cmd));

        switch(currentCmd->cmd.opCode){

            case Read : //not done
               
                
                if(NAND_DEBUG)  
                    cout << DEV_AND_TIME <<"[data_queue_thread] (Channel " << iChannel << ") ID : " << currentCmd->DramInfo.cmd.Id << "  Current command : read at logical address " << currentCmd->cmd.iAddr3 << " on Channel " << iChannel << endl;
                
                NAND_Master[iChannel].write(ADR_NAND_CMD, (void*)&current_NAND_Cmd, sizeof(NAND_Cmd));
                
                if(g_initialize_end) testTrack.done(currentCmd->DramInfo.cmd.Id, currentCmd->cmd.iAddr3, currentCmd->cmd.iAddr4, SSDmetric::NAND_Manager, sc_time_stamp().value()/1000.0);

                while((NAND_RnB[iChannel].read()&(1<<iWay))==0){ //wait for RnB
                    DEBUG("[Ch " << iChannel << " Wy " << iWay << "] Waiting for NAND RnB. Current RnB is " << NAND_RnB[iChannel].read());
                    wait(NAND_RnB[iChannel].value_changed_event());
                
                }

                
                while(!NAND_Master[iChannel].read(ADR_NAND_DATA, (void*)cNandDataBuffer[iChannel], PAGE_SIZE)){
                    wait(SC_ZERO_TIME);
                }
               
                //if(g_initialize_end) testTrack.respStart(currentCmd->DramInfo.cmd.Id, currentCmd->cmd.iAddr3, currentCmd->cmd.iAddr4, SSDmetric::NAND_Manager, sc_time_stamp().value()/1000.0);
                
                //pack the data to send to SubReqMan
                ReadSubResp_t rData;
                rData.Id = currentCmd->DramInfo.cmd.Id;
                rData.logical_addr = currentCmd->cmd.iAddr3;
                rData.len = currentCmd->cmd.iAddr4;
                //rData.cpData = cNandDataBuffer[iChannel];
                rData.cpData = new unsigned char[rData.len * SECTOR_SIZE_BYTE];
                offset = (rData.logical_addr % (DATA_PAGE_SIZE/SECTOR_SIZE_BYTE)) * SECTOR_SIZE_BYTE ;
                if(NAND_DEBUG){
                    cout<<DEV_AND_TIME<< "[data_queue_thread] (Channel " << iChannel << ") ID : " << currentCmd->DramInfo.cmd.Id << "  read addr(logical) : " << currentCmd->cmd.iAddr3 << "  Nand read data = ";
                    for(int i=0; i<rData.len; i++)
                        cout<< cNandDataBuffer[iChannel][i*SECTOR_SIZE_BYTE];
                    cout<<endl;
                }

                memcpy(rData.cpData, &cNandDataBuffer[iChannel][offset], rData.len * SECTOR_SIZE_BYTE );
                
                //if(g_initialize_end) testTrack.respTransfer(currentCmd->DramInfo.cmd.Id, currentCmd->cmd.iAddr3, currentCmd->cmd.iAddr4, SSDmetric::NAND_Manager, sc_time_stamp().value()/1000.0);

                // Send the structure to SubReqMan
                while(!Data_Master.write(_ADDR_DATA_SUBM_, (void*)&rData, (sizeof(ReadSubResp_t) + rData.len)*4)){

                    wait(10, SC_NS);
                }

                //if(g_initialize_end) testTrack.respDone(rData.Id, rData.logical_addr, rData.len, SSDmetric::NAND_Manager, sc_time_stamp().value()/1000.0);

                if(NAND_DEBUG){
                    cout<<DEV_AND_TIME<< "[data_queue_thread] (Channel " << iChannel << ") ID : " << currentCmd->DramInfo.cmd.Id << "  read addr(logical) : " << currentCmd->cmd.iAddr3 << "  Read send complete " << endl;
                }
                delete[] rData.cpData;
                DEBUG("[Ch " << iChannel << " Wy " << iWay << "] Read data is sent to Cache Buffer Module");
                
                break;

            
            case Program : //not done
                
                //send data to NAND
                
                if(NAND_DEBUG) {
                    cout << DEV_AND_TIME <<"[data_queue_thread] (Channel " << iChannel << ") Current command : write at address " << currentCmd->cmd.iAddr1 << " on Channel " << iChannel << " iWay"<< iWay<< endl;
                    cout << DEV_AND_TIME <<"[data_queue_thread] (Channel " << iChannel << ") ID : " << currentCmd->DramInfo.cmd.Id << "  read addr(logical) : " << currentCmd->cmd.iAddr3 << "  data = ";
                    for(uint i=0; i<currentCmd->cmd.iAddr4 ;i++)
                        cout<< currentCmd->data[i*SECTOR_SIZE_BYTE];
                    cout<<endl;
                }

                if(g_initialize_end) testTrack.track(currentCmd->DramInfo.cmd.Id, currentCmd->cmd.iAddr3, currentCmd->cmd.iAddr4, SSDmetric::NAND_Manager, sc_time_stamp().value()/1000.0);
                NAND_Master[iChannel].write(ADR_NAND_CMD, (void*)&current_NAND_Cmd, sizeof(NAND_Cmd));
                
                
                while((NAND_RnB[iChannel].read()&(1<<iWay))==0){ //wait for RnB
                    DEBUG("[Ch " << iChannel << " Wy " << iWay << "] Waiting for NAND RnB. Current RnB is " << NAND_RnB[iChannel].read());
                    wait(NAND_RnB[iChannel].value_changed_event());
                }
                
                NAND_Master[iChannel].write(ADR_NAND_DATA, (void*) currentCmd->data , PAGE_SIZE); 
               
                if(g_initialize_end) testTrack.done(currentCmd->DramInfo.cmd.Id, currentCmd->cmd.iAddr3, currentCmd->cmd.iAddr4, SSDmetric::NAND_Manager, sc_time_stamp().value()/1000.0);
                       

                //command done, delete data buffer in command
                delete[] currentCmd->data; 
                UsedBufSize -= PAGE_SIZE;
                databuf_delete.notify(); //UsedBufSize is no longer MAX_SIZE, if it was
                
                break;

            case Copyback_Program : //not done
                
                //send data to NAND
                
                if(g_initialize_end) testTrack.track(currentCmd->DramInfo.cmd.Id, currentCmd->cmd.iAddr3, currentCmd->cmd.iAddr4, SSDmetric::NAND_Manager, sc_time_stamp().value()/1000.0);
                
                if(NAND_DEBUG) {
                    cout << DEV_AND_TIME <<"[data_queue_thread] (Channel " << iChannel << ") Current command : Copyback Program at address " << currentCmd->cmd.iAddr1 << " on Channel " << iChannel << " iWay"<< iWay<< endl;
                    cout << DEV_AND_TIME <<"[data_queue_thread] (Channel " << iChannel << ") ID : " << currentCmd->DramInfo.cmd.Id << "  data = ";
                    for(uint i=0; i<currentCmd->cmd.iAddr4 ;i++)
                        cout<< currentCmd->data[i*SECTOR_SIZE_BYTE];
                    cout<<endl;
                }

                NAND_Master[iChannel].write(ADR_NAND_CMD, (void*)&current_NAND_Cmd, sizeof(NAND_Cmd));
                
                
                while((NAND_RnB[iChannel].read()&(1<<iWay))==0){ //wait for RnB
                    DEBUG("[Ch " << iChannel << " Wy " << iWay << "] Waiting for NAND RnB. Current RnB is " << NAND_RnB[iChannel].read());
                    wait(NAND_RnB[iChannel].value_changed_event());
                }
                
                NAND_Master[iChannel].write(ADR_NAND_DATA, (void*) currentCmd->data , PAGE_SIZE); 
                
                if(g_initialize_end) testTrack.done(currentCmd->DramInfo.cmd.Id, currentCmd->cmd.iAddr3, currentCmd->cmd.iAddr4, SSDmetric::NAND_Manager, sc_time_stamp().value()/1000.0);
               
                //command done, delete data buffer in command
                delete[] currentCmd->data; 
                UsedBufSize -= PAGE_SIZE;
                databuf_delete.notify(); //UsedBufSize is no longer MAX_SIZE, if it was
                
                break;
             
            case MapTable_Read : 
               
                 
                if(NAND_DEBUG)  
                    cout << DEV_AND_TIME <<"[data_queue_thread] (Channel " << iChannel << ") Current command : map table read at logical address " << currentCmd->cmd.iAddr3 << " on Channel " << iChannel << endl;
                
                current_NAND_Cmd.opCode = Read;
                NAND_Master[iChannel].write(ADR_NAND_CMD, (void*)&current_NAND_Cmd, sizeof(NAND_Cmd));
                

                while((NAND_RnB[iChannel].read()&(1<<iWay))==0){ //wait for RnB
                    DEBUG("[Ch " << iChannel << " Wy " << iWay << "] Waiting for NAND RnB. Current RnB is " << NAND_RnB[iChannel].read());
                    wait(NAND_RnB[iChannel].value_changed_event());
                
                }

                
                while(!NAND_Master[iChannel].read(ADR_NAND_DATA, (void*)cMapTableBuffer, PAGE_SIZE)){
                    wait(SC_ZERO_TIME);
                }
               
                
                if(NAND_DEBUG){
                    cout<<DEV_AND_TIME<< "[data_queue_thread] (Channel " << iChannel << ") map table read data = ";
                    for(uint i=0; i< DATA_PAGE_SIZE/SECTOR_SIZE_BYTE ;i++)
                        cout<< cMapTableBuffer[i*SECTOR_SIZE_BYTE];
                    cout<<endl;
                }

                //raise flag
                mapTableFlag = true;
                wait(e_flagMT);


                if(NAND_DEBUG){
                    cout<<DEV_AND_TIME<< "[data_queue_thread] (Channel " << iChannel << ") map table read send complete " << endl;
                }
                
                *ipHead = prevIPHead; //restore ipHead
                onMapTableCommand[iChannel] = false;

                break;

            case MapTable_Program :                 
                //send data to NAND
               
                while(duringCPUtransfer) wait(e_flagMT);


                if(NAND_DEBUG) {
                    cout << DEV_AND_TIME <<"[data_queue_thread] (Channel " << iChannel << ") Current command : map table write at address " << currentCmd->cmd.iAddr1 << " on Channel " << iChannel << " iWay"<< iWay<< endl;
                    cout << DEV_AND_TIME <<"[data_queue_thread] (Channel " << iChannel << ") data = ";
                    for(uint i=0; i< DATA_PAGE_SIZE/SECTOR_SIZE_BYTE ;i++)
                        cout<< cMapTableBuffer[i*SECTOR_SIZE_BYTE];
                    cout<<endl;
                }
                current_NAND_Cmd.opCode = Program;
                NAND_Master[iChannel].write(ADR_NAND_CMD, (void*)&current_NAND_Cmd, sizeof(NAND_Cmd));
                
                
                while((NAND_RnB[iChannel].read()&(1<<iWay))==0){ //wait for RnB
                    DEBUG("[Ch " << iChannel << " Wy " << iWay << "] Waiting for NAND RnB. Current RnB is " << NAND_RnB[iChannel].read());
                    wait(NAND_RnB[iChannel].value_changed_event());
                }
                
                NAND_Master[iChannel].write(ADR_NAND_DATA, (void*) cMapTableBuffer, PAGE_SIZE); 
               
                //raise flag
                mapTableFlag = true;


                if(NAND_DEBUG){
                    cout<<DEV_AND_TIME<< "[data_queue_thread] (Channel " << iChannel << ") map table write send complete " << endl;
                }
                
                *ipHead = prevIPHead; //restore ipHead
                onMapTableCommand[iChannel] = false;

                break;


            case MapTable_Copyback :
                
                if(NAND_DEBUG) {
                    cout << DEV_AND_TIME <<"[data_queue_thread] (Channel " << iChannel << ") Current command : map table copyback at address " << currentCmd->cmd.iAddr1 << " on Channel " << iChannel << " iWay"<< iWay<< endl;
                }

                current_NAND_Cmd.opCode = Copyback;
                NAND_Master[iChannel].write(ADR_NAND_CMD, (void*)&current_NAND_Cmd, sizeof(NAND_Cmd));
                
                
                while((NAND_RnB[iChannel].read()&(1<<iWay))==0){ //wait for RnB
                    DEBUG("[Ch " << iChannel << " Wy " << iWay << "] Waiting for NAND RnB. Current RnB is " << NAND_RnB[iChannel].read());
                    wait(NAND_RnB[iChannel].value_changed_event());
                }
                
                //raise flag
                mapTableFlag = true;


                if(NAND_DEBUG){
                    cout<<DEV_AND_TIME<< "[data_queue_thread] (Channel " << iChannel << ") map table copyback send complete " << endl;
                }
                
                *ipHead = prevIPHead; //restore ipHead
                onMapTableCommand[iChannel] = false;



                break;

            case MapTable_Erase :
                
                if(NAND_DEBUG) {
                    cout << DEV_AND_TIME <<"[data_queue_thread] (Channel " << iChannel << ") Current command : map table erase at address " << currentCmd->cmd.iAddr1 << " on Channel " << iChannel << " iWay"<< iWay<< endl;
                }

                current_NAND_Cmd.opCode = Erase;
                NAND_Master[iChannel].write(ADR_NAND_CMD, (void*)&current_NAND_Cmd, sizeof(NAND_Cmd));
                
                
                while((NAND_RnB[iChannel].read()&(1<<iWay))==0){ //wait for RnB
                    DEBUG("[Ch " << iChannel << " Wy " << iWay << "] Waiting for NAND RnB. Current RnB is " << NAND_RnB[iChannel].read());
                    wait(NAND_RnB[iChannel].value_changed_event());
                }
               
                //raise flag
                mapTableFlag = true;


                if(NAND_DEBUG){
                    cout<<DEV_AND_TIME<< "[data_queue_thread] (Channel " << iChannel << ") map table erase send complete " << endl;
                }
                
                *ipHead = prevIPHead; //restore ipHead
                onMapTableCommand[iChannel] = false;



                break;
                
            default :
                
                
                if(NAND_DEBUG) {
                    cout << DEV_AND_TIME <<"[data_queue_thread] (Channel " << iChannel << ") Current command : " << currentCmd->cmd.opCode << " on Channel " << iChannel << " iWay"<< iWay<< endl;}

                NAND_Master[iChannel].write(ADR_NAND_CMD, (void*)&current_NAND_Cmd, sizeof(NAND_Cmd));
                
                break;


        }


        //command finished, update counters
        //iNandWriteQueueCount(or ipCount) ensures that the logically "deleted" queue entries will not be used, so no seperate erase operations are needed
        *ipCount = *ipCount - 1;
        *ipHead =((*ipHead) + 1) % NAND_CMD_QUEUE_SIZE;
        eNandQueueDelete[iChannel].notify();

        if(NAND_DEBUG)  
            cout << DEV_AND_TIME <<"[data_queue_thread] (Channel " << iChannel << ") Command finished... ipCount : " << *ipCount << " ipHead : " << *ipHead << endl;

        wait(5, SC_NS); //for extra scheduling

        if(current_NAND_Cmd.opCode <= MapTable_Erase) {
            

            int blocknum = (current_NAND_Cmd.iAddr1 & MASK_BLOCK) >>BIT_FOR_BLOCK & (NUM_OF_BLOCK_PER_WAY-1);
            if(g_initialize_end) NAND_STATS[current_NAND_Cmd.opCode][iChannel][iWay][blocknum]++;
            
        }

    }
}



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

//% begin section previously tagged: FOOTER_H

//% end section previously tagged: FOOTER_H
                                                                                //%USEREND ORPHANED_CODE
#endif // ORPHANED_CODE                                                         //%
                                                                                //%USERBEGIN FOOTER_H

                                                                                //%USEREND FOOTER_H
#endif // __NAND_MANAGER_H__                                                    //%
