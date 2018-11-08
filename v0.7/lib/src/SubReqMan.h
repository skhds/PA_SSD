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
#include "./header/IF_Spec.h"

#include "./header/NAND_Spec.h" //temp
#include "./header/NAND_Struct.h" //temp
#include "./header/Queue.h"
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
    
    sc_core::sc_out<bool> CPU_IRQ;
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
    IRQThread();
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

    // TODO: Insert declarations of member data and functions here.

protected:
    

    // member methods
    //
    //
    //

        // debug methods

        // member variable
    //

    sc_core::sc_event eIRQThread;
    sc_core::sc_event eDataReady;
    sc_core::sc_event eCPUFinish;
    sc_core::sc_event eNANDRead;

    //hard coded for now
    M_CREATEQUEUE(SubReqQueue, SubReq_t, 128);
    M_CREATEQUEUE(cDataQueue, PageBuf_t, 4); 
    Req_t reqInQueue;
    bool isCPUFinish;

    bool bypass_cache;
    


 public:
    // member methods
    //
    
    void split_and_queue(Req_t req);
    bool CPUReadSubReq(sc_dt::uint64 adr, unsigned char* ptr, SubReq_t subreq);
    bool CPUWriteSubReq(sc_dt::uint64 adr, unsigned char* ptr, SubReq_t& subreq);
    void software_Init(unsigned char* ptr);
    void setCacheTrans(CacheTrans_t &cacheTrans, uint Id, uint Addr, uint Op, uint Len, uint lba);
    void setCacheCMD(SubReq_t &subreq, CacheTrans_t cacheTrans);
    
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
                                                                               //%
                                                                               //%USERBEGIN INITIALIZER
                                                                                //
    // TODO: Add your class initialization here.
 //m_WaitCachePossesion(1),
// m_seReadPath(1),
// m_seWaitCacheRead(1)
                                                                                //%USEREND INITIALIZER
                                                                                //%
{                                                                               //%
 // declare processes and their sensitivities                                   //%
	SC_METHOD(rst_nHandler);                                                //%
	 sensitive << rst_n.neg() ;                                             //%
	 dont_initialize();                                                     //%
    SC_THREAD(DivideSubReq);
     //sensitive << m_eNewHostReq;
     dont_initialize();
    SC_THREAD(IRQThread);
     sensitive << eIRQThread;
     dont_initialize();

    SC_THREAD(ReadDataFromCache);
     //sensitive << m_Read_CacheData;
     dont_initialize();
	SC_THREAD(ReadManager);                                                 //%
     //sensitive << m_eNewReadResp;
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
    bypass_cache = false;
    RnBMaster.write(1);
    eIRQThread.notify();
} 
                                                                                //%USEREND rst_nHandler

void 
SubReqMan::split_and_queue(Req_t req){

    int split_count = (req.iLen-1)/SECTOR_PER_PAGE + 1;
    int remaining_length = req.iLen;
    assert((split_count > 0)&& (split_count <= MAX_REQ_LEN/SECTOR_PER_PAGE)); 
    SubReq_t tmpSubReq;
    

    for(int i = 0; i < split_count; i++){ 
        
        //divide req into subreq
        memcpy(&(tmpSubReq.oriReq), &req, sizeof(Req_t));
        tmpSubReq.iStartAddr = req.iAddr + i*SECTOR_PER_PAGE;
        tmpSubReq.iLen = (remaining_length > SECTOR_PER_PAGE ) ? SECTOR_PER_PAGE : remaining_length;
        assert(tmpSubReq.iLen > 0);
        remaining_length -= SECTOR_PER_PAGE;
        tmpSubReq.Id = req.iId;

        //insert subreq into queue
        M_PUSH(SubReqQueue, &tmpSubReq);

    }
    //
    

}

void SubReqMan::DivideSubReq()
{
    while(1){
        wait();
    }
}

void SubReqMan::IRQThread()
{
    // This thread send the SubReq to DRAM Ctrl.
    // And if subreq is write cmd, data is transmitted together.
    // SubReq which should be send to the NAND is transmitted in CMD Callback.
    // But this operation is not guaranteed if DRAM ctrl buffer is full.
    static double time_stamp = 0;
    while(1){    
        wait();
        while(!M_CHECKEMPTY(SubReqQueue)){
            if(reqInQueue.Op == HOST_WRITE ){ //write
                //1. send IRQ to CPU1
                if(SUB_DEBUG) cout << DEV_AND_TIME << "[IRQThread] IRQ Send, waiting for CPU... " << time_stamp << endl;
                CPU_IRQ.write(1); 
                //2. CPU1 reads cmd (CmdCB)
                //3. cmd from CPU1 (CmdCB)
                if(!isCPUFinish) wait(eCPUFinish); //wait for data to arrive
                isCPUFinish = false;
                CPU_IRQ.write(0);
                
                if(SUB_DEBUG) cout << DEV_AND_TIME << "[IRQThread] CPU done, waiting for data... " << endl;
                //4. thread sends cmd & data
                if(M_CHECKEMPTY(cDataQueue)) wait(eDataReady); //wait for data to arrive
                if(!bypass_cache){ //dest cache //testing for now... must change this later
                    SubReq_t* tmpSubReq = &M_GETELE(SubReqQueue);
                    PageBuf_t* tmpDataBuf = &M_GETELE(cDataQueue);
                    if(SUB_DEBUG){
                        cout << DEV_AND_TIME << "[IRQThread] cmd : " << endl;
                        cout << DEV_AND_TIME << "[IRQThread] \tLBA(SubReq) : "<< tmpSubReq->iStartAddr <<endl;
                        cout << DEV_AND_TIME << "[IRQThread] \tLBA(CacheTrans_t) : " << tmpSubReq->cmd.cmd.lba << endl;
                        cout << DEV_AND_TIME << "[IRQThread] \tDRAM addr : "<< tmpSubReq->cmd.cmd.Addr << endl;
                        cout << DEV_AND_TIME << "[IRQThread] \tDRAM id : "<< tmpSubReq->cmd.cmd.Id << endl;
                        cout << DEV_AND_TIME << "[IRQThread] \tDRAM op : "<< tmpSubReq->cmd.cmd.Op << endl;
                    }   
                    DataMaster.write(_ADDR_DRAM_DATA_ + tmpSubReq->cmd.cmd.Id * DATA_PAGE_SIZE, (void*)tmpDataBuf, SECTOR_PER_PAGE);
                
                
                
                    if(buffer_write_count >= (uint)(0x0 - 0x200)) cout << "WARNING : POSSIBLE OVERFLOW ON COUNTING BUFFER WRITE COUNT!!!" << endl;
                    buffer_write_count += 512;
                }
                else{ //dest NAND
                
                    cache_bypass_count++; 
                    
                    wait(eNANDRead); 
                
                }
                total_req_count++; 
                
                
                M_POP(SubReqQueue);
                M_POP(cDataQueue);
                RnBMaster.write(1);
            }
            else if(reqInQueue.Op == HOST_READ){ //read

                //send IRQ to CPU1
                //.write();

            }
            else{ //unknown cmd
                assert(0 && "Unknown cmd");
            }
            
            wait(5, SC_NS); //this HAS to be above SC_ZERO_TIME, or IRQ write 0 will be ignored in IRQ controller...
        }
    
    
    }
}

void
SubReqMan::ReadDataFromCache()
{
    while(1){
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

        
        wait();
    }
}
void
SubReqMan::Copy_SFR_To_CMDList(uint size)
{


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
        /*if(SUB_DEBUG){
            cout<< DEV_AND_TIME << "[WriteSlaveCB] data arrives on SubReqMan, current state is : " << adr  <<endl;
        }*/
    }
    Req_t newReq;

    switch(adr){

        case REQUEST_ADDR:
            //expected to receive cmd
            //divide cmd into subreq
            memcpy(&newReq, ptr, sizeof(Req_t));
            assert((newReq.Op == HOST_READ) || (newReq.Op == HOST_WRITE)); 
            split_and_queue(newReq);
            
            //notify thread
            eIRQThread.notify(); 
            break;

        case DATA_ADDR:
            //expected to receive data
            //receive one page at a time
            assert(len <= DATA_PAGE_SIZE);
            //wait?
            M_PUSH(cDataQueue, (PageBuf_t*)ptr);
            if(M_CHECKFULL(cDataQueue)) RnBMaster.write(0);  
            
            eDataReady.notify();
            //write to RnB Master
            break;

        default:
            assert(0 && "invalid address, addr : " && adr);
            break;

    }



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
    if (cmd == tlm::TLM_READ_COMMAND) {
    
        //reads info of current cmd
        if(SUB_DEBUG) cout << DEV_AND_TIME << "[CmdSlaveCB] CPU read invoked, adr : " << adr << endl;   
        CPUReadSubReq(adr, ptr, M_GETELE(SubReqQueue)); 
        //note : CPU might not be able to read the full struct at once.... 
    } 
    else if (cmd == tlm::TLM_WRITE_COMMAND) {
    
        //sends dest of cmd in HOST
        if((g_isFtlReady == false) ||(g_isCacheReady == false )) software_Init(ptr);
        else{
            //if(SUB_DEBUG) cout << DEV_AND_TIME << "[CmdSlaveCB] CPU write invoked, adr : " << adr << endl;   
            if(CPUWriteSubReq(adr, ptr, M_GETELE(SubReqQueue))){
                isCPUFinish = true;
                eCPUFinish.notify();
            }
        }

    } 
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
    //%USEREND CPU_Callback
}   

bool
SubReqMan::CPUReadSubReq(sc_dt::uint64 adr, unsigned char* ptr, SubReq_t subreq){

    int offset = adr;
    switch (offset){
            

        case 0 : // op
            *((uint*)ptr) = subreq.oriReq.Op;
            return false;
            break;
        case 4 : //starting addr
            *((uint*)ptr) = subreq.iStartAddr;
            return false;
            break;

        case 8: //length
            *((uint*)ptr) = subreq.iLen;
            return true;
            break;

        default : //undefined 
            assert(0);
            break;
    
    }

}


bool
SubReqMan::CPUWriteSubReq(sc_dt::uint64 adr, unsigned char* ptr,SubReq_t& subreq){

    int offset = adr - 0x1C;
    CacheTrans_t cacheTrans;
    static uint Id;
    static uint Addr;
    static uint Op;
    static uint Len;
    static uint lba;


    switch (offset){
            
        case 0 : // op
            Op = *((uint*)ptr);
            return false;
            break;
        case 4 : //LPA
            lba = *((uint*)ptr) * SECTOR_PER_PAGE;
            return false;
            break;
        case 8 :
            return false;
            break;
        case 12 ://DRAM id (# of node) 
            Addr = *((uint*)ptr) * DATA_PAGE_SIZE;
            return false;
            break;
        case 16 : 
            
            return false;
            break;
        case 20 ://everything went through, set cache cmd

            Len = subreq.iLen * SECTOR_SIZE_BYTE;
            Id = subreq.oriReq.iId;
            setCacheTrans(cacheTrans, Id, Addr, Op, Len, lba); 
            setCacheCMD(subreq, cacheTrans);
            bypass_cache = *((uint*)ptr);            
            if(SUB_DEBUG){

               cout << DEV_AND_TIME << "[CPUWriteSubReq] \tbypass? : " << bypass_cache << endl;
               cout << DEV_AND_TIME << "[CPUWriteSubReq] CacheTrans (received, inserted): " << endl;
               cout << DEV_AND_TIME << "[CPUWriteSubReq] \tId : " << Id << "\t" << subreq.cmd.cmd.Id << endl;
               cout << DEV_AND_TIME << "[CPUWriteSubReq] \tAddr : " << Addr << "\t" << subreq.cmd.cmd.Addr << endl;
               cout << DEV_AND_TIME << "[CPUWriteSubReq] \tLen : " << Len << "\t" << subreq.cmd.cmd.Len << endl;
               cout << DEV_AND_TIME << "[CPUWriteSubReq] \tlba : " << lba << "\t" << subreq.cmd.cmd.lba << endl;


            }

            return true;
            break;
        case 24 : //bypass cache;
            if(SUB_DEBUG) cout <<DEV_AND_TIME << "[CPUWriteSubReq] bypass cache" << endl; 
            bypass_cache = true;
            return true;
            break;
        default : //undefined 
            assert(0);
            return false;
            break;
    
    }

}

void
SubReqMan::setCacheTrans(CacheTrans_t &cacheTrans, uint Id, uint Addr, uint Op, uint Len, uint lba){
    
    cacheTrans.Id = Id;
    cacheTrans.Addr = Addr;
    cacheTrans.Op = Op;
    cacheTrans.Len = Len;
    cacheTrans.lba = lba;
        
}


void
SubReqMan::setCacheCMD(SubReq_t &subreq, CacheTrans_t cacheTrans){

    CacheCMD cacheCMD;
    memcpy(&cacheCMD, &cacheTrans, sizeof(CacheTrans_t));
    cacheCMD.SourceDevice = HOST;
    cacheCMD.isDataReady = false;
    //isCMDTrans is not considered..

    memcpy(&subreq.cmd, &cacheCMD, sizeof(CacheCMD));
}


void
SubReqMan::software_Init(unsigned char* ptr){


    if(*((unsigned int *)ptr)==2){ //if Cache software initialization is completed
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
    else{
        assert(0);
    }

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
    len = len*512; 
    if(cmd == tlm::TLM_READ_COMMAND){

        if(M_CHECKEMPTY(cDataQueue)) wait(eDataReady); //wait for data to arrive
        wait(len/DATA_BUS_TOTAL_BW, SC_NS);
        if (SUB_DEBUG) cout << DEV_AND_TIME << "[DataSlaveCB] with cmd : " << cmd << endl; 
        PageBuf_t* tmpDataBuf = &M_GETELE(cDataQueue);
        memcpy((void*)ptr, (void*)tmpDataBuf, len);
        eNANDRead.notify();
    }
    else{

        cout << "Unknown cmd" << endl;
        assert(0);
    }
    
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
