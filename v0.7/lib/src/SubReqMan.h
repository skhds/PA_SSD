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

#include "./header/header_SUB.h"
#include "./header/global_header.h"


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
	SubReqMan( const sc_core::sc_module_name & n);                          //%
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
    IRQThread();
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
    
    sc_core::sc_event eIRQThread;
    sc_core::sc_event eDataReady;
    sc_core::sc_event eCPUFinish;
    sc_core::sc_event eNANDRead;

    //hard coded for now
    M_CREATEQUEUE(SubReqQueue, SubReq_t, SUBREQ_QUEUE_SIZE);
    M_CREATEQUEUE(cDataQueue, PageBuf_t, DATA_QUEUE_SIZE); 
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
    void write_to_DRAM(uint LPA, uint bitmap, uchar* ptr); 
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
SubReqMan::SubReqMan( const sc_core::sc_module_name & n)                        //%
		: sc_core::sc_module(n)                                                 //%
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
    SC_THREAD(IRQThread);
     sensitive << eIRQThread;
     dont_initialize();

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
/*
    int split_count = (req.iLen-1)/SECTOR_PER_PAGE + 1;
    int remaining_length = req.iLen;
    assert((split_count > 0)&& (split_count <= MAX_REQ_LEN/SECTOR_PER_PAGE)); 
    SubReq_t tmpSubReq;
    

    for(int i = 0; i < split_count; i++){ 
        
        //divide req into subreq
        tmpSubReq.op = (SUB_REQ_OP)req.Op;
        tmpSubReq.iId = req.iId;
        tmpSubReq.iStartAddr = req.iAddr + i*SECTOR_PER_PAGE;
        tmpSubReq.iLen = (remaining_length > SECTOR_PER_PAGE ) ? SECTOR_PER_PAGE : remaining_length;
        assert(tmpSubReq.iLen > 0);
        remaining_length -= SECTOR_PER_PAGE;

        //insert subreq into queue
        M_PUSH(SubReqQueue, &tmpSubReq);

    }
    //
 */   


    //modifications : requests split are alligned
    
    int remaining_length = req.iLen;
    int split_count = 0;
    int starting_addr = req.iAddr;
    int addr_offset;
    
    SubReq_t tmpSubReq;
    
    do{
       

       tmpSubReq.op = (SUB_REQ_OP)req.Op;
       tmpSubReq.iId = req.iId;
       tmpSubReq.iStartAddr = starting_addr;

       addr_offset = starting_addr % SECTOR_PER_PAGE + remaining_length - SECTOR_PER_PAGE; //determines for extra page
       if(addr_offset > 0){
         tmpSubReq.iLen = SECTOR_PER_PAGE - starting_addr % SECTOR_PER_PAGE;
         remaining_length -= tmpSubReq.iLen;
         starting_addr += tmpSubReq.iLen;

       }
       else{
         tmpSubReq.iLen = remaining_length; 
       }


       M_PUSH(SubReqQueue, &tmpSubReq);

    }while(addr_offset > 0);

    
    /* I honestly think spliting requests into page-sized requests can be done
     * way more efficiently in software. The interface inside the software is
     * actually already there, but somehow was not considered. I will make it 
     * so that the cache software will split the requests, instead of making the
     * hardware do it. For now, the request is decoded and split through this function.
     */
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
                    
                   
#ifdef DATA_COMPARE_ON             
                    DTCMP::updateMap(DTCMP::mmDRAM, tmpSubReq->dst, tmpSubReq->iStartAddr/SECTOR_PER_PAGE, tmpSubReq->bitmap); 
#endif

                    write_to_DRAM(tmpSubReq->dst, tmpSubReq->bitmap, tmpDataBuf->buf);
                    
                    
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
SubReqMan::write_to_DRAM(uint LPA, uint bitmap, uchar* tmpDataBuf){

    int base_addr = _ADDR_DRAM_DATA_ + LPA*PAGE_BYTES; 
    int j = 0;
    for(int i=0; bitmap != 0; i++){

        if(bitmap % 2) DataMaster.write(base_addr + i*SECTOR_BYTES, (void*)(tmpDataBuf + j++*SECTOR_BYTES), SECTOR_BYTES); //one sector at a time
        bitmap >>= 1;
    }



}

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
            assert(len <= PAGE_BYTES);
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
        else if(adr < 0x50) {
            //if(SUB_DEBUG) cout << DEV_AND_TIME << "[CmdSlaveCB] CPU write invoked, adr : " << adr << endl;   
            if(CPUWriteSubReq(adr, ptr, M_GETELE(SubReqQueue))){
                isCPUFinish = true;
                eCPUFinish.notify();
            }
        }
        else if( adr == 0x50 ){ //notified from NAND

            eNANDRead.notify();
        }
        else assert(0);
    } 
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
    //%USEREND CPU_Callback
}   

bool
SubReqMan::CPUReadSubReq(sc_dt::uint64 adr, unsigned char* ptr, SubReq_t subreq){

    int offset = adr;
    switch (offset){
            

        case 0 : // op
            *((uint*)ptr) = subreq.op;//check
            return false;
        case 4 : //starting addr
            *((uint*)ptr) = subreq.iStartAddr;
            return false;

        case 8: //length
            *((uint*)ptr) = subreq.iLen;
            return true;

        default : //undefined 
            assert(0);
            break;
    
    }

}


bool
SubReqMan::CPUWriteSubReq(sc_dt::uint64 adr, unsigned char* ptr,SubReq_t& subreq){

    int offset = adr - 0x1C;


    switch (offset){
            
        case 0 : // op
            subreq.op = (SUB_REQ_OP)*((uint*)ptr);
            return false;
        case 4 : //LPA
            subreq.iStartAddr = *((uint*)ptr) * SECTOR_PER_PAGE;
            return false;
        case 8 :
            return false;
        case 12 ://DRAM id (# of node) 
            subreq.dst = *((uint*)ptr);
            assert(subreq.dst < DRAM_ENTRY_SIZE);
            return false;
        case 16 ://DRAM bitmap 
            subreq.bitmap = *((uint*)ptr);
            return false;
        case 20 ://everything went through

            bypass_cache = *((uint*)ptr);            

            return true;
        case 24 : //bypass cache;
            if(SUB_DEBUG) cout <<DEV_AND_TIME << "[CPUWriteSubReq] bypass cache" << endl; 
            bypass_cache = true;
            return true;
        default : //undefined 
            assert(0);
            return false;
    
    }

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
    
    if(cmd == tlm::TLM_READ_COMMAND){

        if(M_CHECKEMPTY(cDataQueue)) wait(eDataReady); //wait for data to arrive
        wait(len/DATA_BUS_TOTAL_BW, SC_NS);
        if (SUB_DEBUG) cout << DEV_AND_TIME << "[DataSlaveCB] with cmd : " << cmd << endl; 
        PageBuf_t* tmpDataBuf = &M_GETELE(cDataQueue);
        memcpy((void*)ptr, (void*)tmpDataBuf, len);
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
