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
#include "./header/global_header.h"
#include "./header/header_NAND.h"

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
    sc_core::sc_out<bool> IRQ_Master_CPU1;
    sc_core::sc_out<bool> IRQ_Master_CPU2;


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
    void IRQ_Thread_CPU1();
    void IRQ_Thread_CPU2();
    void data_queue_thread();
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
    sc_core::sc_event eNandQueueDelete;
    sc_core::sc_event eIRQ1;
    sc_core::sc_event eIRQ2;
    sc_core::sc_event eIRQ1end;
    sc_core::sc_event eIRQ2end;
    sc_core::sc_event eRNM_finish;

    // Write Queue (it's actually data queue)
    NAND_Cmd_Buffer NandQueue[NUM_CHANNEL][NAND_CMD_QUEUE_SIZE]; //Command Queue implemented in FIFO
    unsigned int iNandQueueCount[NUM_CHANNEL]; //current queue size
    unsigned int iNandQueuePtr[NUM_CHANNEL]; //current index in queue
    unsigned int iSelectedChannel; //channel # of data queue thread

    // Temporary Buffers
    NANDData_t NandDataBuffer[NAND_BUF_ENTRY]; //data buffer
    int bufUsedCount;
    bool bNandBufferUsed[NUM_CHANNEL];

    //evict manager
    IRQReq_t reqIRQ;
    M_CREATEQUEUE(IRQ_Queue, IRQReq_t, SIZE_BUF_EVICT); //size should be the # of eviction entries
    M_CREATEQUEUE(EvictDone_Queue, int, SIZE_BUF_EVICT); //contains DRAM id's of finished evict requests 
  
    //direct NAND write
    directNANDReq_t reqHOST;

    //RNM flag
    bool RNM_flag;
    uint cur_buf_index;

    //functions

    bool collectIRQRequest(sc_dt::uint64 adr, unsigned int* ptr);
    bool sendIRQRequest(sc_dt::uint64 adr, unsigned int* ptr);
    bool collectFTLRequest(sc_dt::uint64 adr, unsigned int* ptr);
    bool collectHostRequest(sc_dt::uint64 adr, unsigned int* ptr);

    int findEntryByID(int id);
    int findFreeEntry();
    void mergeRNM(uchar*, uchar*, uint);

        
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

    inline unsigned int get_DRAM_addr(unsigned int id){
        
        return id * DATA_PAGE_SIZE;

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

	SC_THREAD(IRQ_Thread_CPU1);                                           //%
	 sensitive << eIRQ1;                                         //%
	 dont_initialize();                                                     //%
	
     SC_THREAD(IRQ_Thread_CPU2);                                           //%
	 sensitive << eIRQ2;                                         //%
	 dont_initialize();                                                     //%
    
     for(unsigned int i=0; i<NUM_CHANNEL; i++){
        SC_THREAD(data_queue_thread);
         sensitive << data_queue_event[i];
         dont_initialize();
    }

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

     
                                                                                //%USEREND CONSTRUCTOR
                                                                                //%
}                                                                               //%
// destructor                                                                   //%
                                                                                //%
template < unsigned int NUM_CHANNEL >                                           //%
NAND_Manager< NUM_CHANNEL >::~NAND_Manager()                                    //%
{                                                                               //%
                                                                                //%USERBEGIN DESTRUCTOR

    // TODO: Add destructor code here.
                                                                                //%USEREND DESTRUCTOR
}                                                                               //%
// initialization method for ports                                              //%
                                                                                //%
template < unsigned int NUM_CHANNEL >                                           //%
void                                                                            //%
NAND_Manager< NUM_CHANNEL >::initPorts()                                        //%
{                      
    IRQ_Master_CPU1.initialize(0x0);
    IRQ_Master_CPU2.initialize(0x1);
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
        NAND_write_count = 1; //initialized to 1 in order to prevent 0 division
        reqHOST.id = -1;
        reqHOST.addr = -1;
        reqHOST.state = REQ_EMPTY;
        bufUsedCount = 0; 
        RNM_flag = false;
        for(unsigned int i=0; i<NUM_CHANNEL; i++){
            
            iNandQueueCount[i] = 0;
            iNandQueuePtr[i] = 0;
            
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
    unsigned int* ptr = (unsigned int*)trans.get_data_ptr();   //command type : SubReq_t at header/ssd_struct.h        
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

    //cout << "NAND cmd : " << adr << endl; 
    if( (adr >= _ADDR_CPU1_BASE_) && (adr < _ADDR_CPU1_BASE_ + _OFFSET_IRQ_MAX_)){ 
        //Eviction request received from CPU1
        if(cmd == tlm::TLM_WRITE_COMMAND){
            if(collectIRQRequest(adr, ptr)){
                eIRQ2.notify();
            }
        }
        else if(cmd == tlm::TLM_READ_COMMAND){
            *ptr = M_GETELE(EvictDone_Queue);
            M_POP(EvictDone_Queue);
            eIRQ1end.notify();
            if(NAND_DEBUG){
                cout << DEV_AND_TIME << "[cmd_callback] Eviction request read from CPU1, ID: " << *ptr << endl;
            }
        }
        else{
            cout << DEV_AND_TIME << "[cmd_callback] unexpected cmd.. " << cmd <<endl; 
            assert(0 && cmd);
        }
    }
    else if( (adr >= _ADDR_CPU1_BASE_ + _OFFSET_DIRECT_) && (adr < _ADDR_CPU1_BASE_ + _OFFSET_DIRECT_MAX_)){
        if(cmd == tlm::TLM_WRITE_COMMAND){ //direct write request from host
            if(collectHostRequest(adr, ptr)){
                eIRQ2.notify();
            }
        }
        else{
            
            cout << DEV_AND_TIME << "[cmd_callback] unexpected cmd.. " << cmd <<endl; 
            assert(0 && cmd);
        }
    }
    else if( (adr == _ADDR_CPU1_BASE_ + _OFFSET_NAND_COUNT_)){
        /*static int tmp = 0;
        if(tmp++ > 100){
        cout << "NAND count : "<< NAND_write_count << endl;
            tmp = 0;
        }*/
        *ptr = NAND_write_count;

    }
    else if( (adr == _ADDR_CPU1_BASE_ + _OFFSET_RAM_COUNT_)){
        /*static int tmp1 = 0;
        if(tmp1++ > 100){
            cout << "Cache count : "<< buffer_write_count << endl;
            tmp1 = 0;
        }*/
        *ptr = buffer_write_count;

    }
    else if( (adr >= _ADDR_CPU2_BASE_ + _OFFSET_IRQ_BASE_) && (adr < _ADDR_CPU2_BASE_ + _OFFSET_IRQ_BASE_ + _OFFSET_IRQ_MAX_) ){
        if(cmd == tlm::TLM_READ_COMMAND){
            //CPU2 reads eviction request
            if(sendIRQRequest(adr, ptr)){
                if(NAND_DEBUG){
                    cout << DEV_AND_TIME << "[cmd_callback] reqIRQ - id : " << reqIRQ.id << "  addr : " << reqIRQ.addr << endl;
                }
            }
        }
        else{
            cout << DEV_AND_TIME << "[cmd_callback] adr : " << adr << " val(ptr) : " << *ptr << endl;
            assert(0 && "undefined operation");
        }
    }
    else if ( (adr >= _ADDR_CPU2_BASE_) && (adr < _ADDR_CPU2_BASE_ + _OFFSET_FTL_MAX_ )) { 
        //CPU2 writes FTL request
        if(cmd == tlm::TLM_WRITE_COMMAND){
            if(collectFTLRequest(adr, ptr)){
                eIRQ2end.notify();
            }   
        }else{
            cout << DEV_AND_TIME << "[cmd_callback] adr : " << adr << " val(ptr) : " << *ptr << endl;
            assert(0 && "undefined operation");
        }
    }
    else{
        assert(0 && adr);
    }
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
}                                                                               //%USEREND Bus_Interrupt_Callback

template < unsigned int NUM_CHANNEL >                                           //%
bool                                                                            //%
NAND_Manager< NUM_CHANNEL >::collectHostRequest(sc_dt::uint64 adr, unsigned int* ptr){
        
        int offset = adr - _ADDR_CPU1_BASE_;
        switch(offset){
            case _OFFSET_HOST_ID_ : //DRAM node ID
                reqHOST.id = *ptr;
                return false;

            case _OFFSET_HOST_ADDR_ : //DRAM node ID
                reqHOST.addr = *ptr;
                return false;
            
            case _OFFSET_HOST_BITMAP_ : //LPA 
                reqHOST.bitmap = *ptr;
                assert(reqHOST.state == REQ_EMPTY);
                reqHOST.state = NOT_READY;
                if(NAND_DEBUG){

                    cout << DEV_AND_TIME << "[collectHostRequest] reqHOST - id : " << reqHOST.id << "  addr : " << reqHOST.addr << endl;
                }
                return true;
            
            default :

                assert(0 && offset);
                return false;
                
        }

}

template < unsigned int NUM_CHANNEL >                                           //%
bool                                                                            //%
NAND_Manager< NUM_CHANNEL >::collectIRQRequest(sc_dt::uint64 adr, unsigned int* ptr){
        
        int offset = adr - _ADDR_CPU1_BASE_;
        static IRQReq_t reqIRQ;
        switch(offset){
            case _OFFSET_IRQ_ID_ : //DRAM node ID
                reqIRQ.id = *ptr;
                return false;
            
            case _OFFSET_IRQ_ADDR_ : //DRAM node ID
                reqIRQ.addr = *ptr;
                return false;

            case _OFFSET_IRQ_BITMAP_ : //LPA 
                reqIRQ.bitmap = *ptr;
                M_PUSH(IRQ_Queue, &reqIRQ);
                if(NAND_DEBUG){

                    cout << DEV_AND_TIME << "[collectIRQRequest] reqIRQ - id : " << reqIRQ.id << "  addr : " << reqIRQ.addr << endl;
                }
                return true;
            
            default :

                assert(0 && offset);
                return false;
                
        }

}

template < unsigned int NUM_CHANNEL >                                           //%
bool                                                                            //%
NAND_Manager< NUM_CHANNEL >::collectFTLRequest(sc_dt::uint64 adr, unsigned int* ptr){
       
        //going to change later, this is just for testing...
        int offset = adr - _ADDR_CPU2_BASE_;
        static NAND_Cmd_Buffer tmpNAND;
        int index;
        int ch;
        switch(offset){
            case _OFFSET_NAND_CMD_ : //NAND OP
                if(*ptr == RNM_Read) {RNM_flag = true; tmpNAND.cmd.opCode = (NAND_CMD_SET)*ptr;}
                else if(RNM_flag && (*ptr == Program) ) {tmpNAND.cmd.opCode = RNM_Program; RNM_flag = false;}
                else tmpNAND.cmd.opCode = (NAND_CMD_SET)*ptr;
                return false;

            case _OFFSET_NAND_ADDR1_ : //dst addr
                tmpNAND.cmd.iAddr1 = *ptr;
                return false;
            case _OFFSET_NAND_ADDR2_ : //src addr (for copybacks)
                tmpNAND.cmd.iAddr2 = *ptr;
                return false;
            case _OFFSET_NAND_ADDR3_ : //DRAM ID         
                return false;
            case _OFFSET_NAND_ADDR4_ : //bitmap (not sure)
                tmpNAND.cmd.iAddr4 = *ptr;
            
                tmpNAND.DRAM_id = NandDataBuffer[cur_buf_index].DRAM_id;
                if(tmpNAND.cmd.opCode == RNM_Read) NandDataBuffer[cur_buf_index].state = RNM_WAITING;

                ch = get_NAND_Channel(tmpNAND.cmd.iAddr1);
                
                index = (iNandQueuePtr[ch] + iNandQueueCount[ch]++) % NAND_CMD_QUEUE_SIZE;
                assert(iNandQueueCount[ch] <= NAND_CMD_QUEUE_SIZE);
                memcpy(&NandQueue[ch][index], &tmpNAND, sizeof(NAND_Cmd_Buffer));       

                
                

                if(tmpNAND.cmd.opCode == RNM_Program) 

                if(NAND_DEBUG){
                    cout << DEV_AND_TIME << "[collectFTLRequest] NAND CMD : " << endl;
                    cout << "\tDRAM id : " << tmpNAND.DRAM_id << endl;
                    cout << "\topCode : " << tmpNAND.cmd.opCode << endl;
                    cout << "\tiAddr1 : " << tmpNAND.cmd.iAddr1 << endl;
                    cout << "\tiAddr2 : " << tmpNAND.cmd.iAddr2 << endl;
                    cout << "\tiAddr3 : " << tmpNAND.cmd.iAddr3 << endl;
                    cout << "\tiAddr4 : " << tmpNAND.cmd.iAddr4 << endl;
                }
                data_queue_event[ch].notify();
                
                return false;

            
            
            case _OFFSET_DRAM_REQ_ID_ : //LPA 
                
                
                
                assert(tmpNAND.DRAM_id == *ptr);

                return true;
            
            default :

                assert(0 && offset);
                return false;
                
        }

}

template < unsigned int NUM_CHANNEL >                                           //%
bool                                                                            //%
NAND_Manager< NUM_CHANNEL >::sendIRQRequest(sc_dt::uint64 adr, unsigned int* ptr){
        int offset = adr - (_ADDR_CPU2_BASE_ + _OFFSET_IRQ_BASE_);
        switch(offset){
            case _OFFSET_IRQ_ID_ : //DRAM node ID
                
                if(reqHOST.state != REQ_READY) *ptr = M_GETELE(IRQ_Queue).id;
                else *ptr = reqHOST.id;
                return false;

            case _OFFSET_IRQ_ADDR_ : //LPA
                
                if(reqHOST.state != REQ_READY) *ptr = M_GETELE(IRQ_Queue).addr;
                else *ptr = reqHOST.addr;
                return false;
            
            case _OFFSET_IRQ_BITMAP_ : //bitmap
                if(reqHOST.state != REQ_READY) {
                    *ptr = M_GETELE(IRQ_Queue).bitmap;
                    if(NAND_DEBUG){

                        cout << DEV_AND_TIME << "[sendIRQRequest] reqIRQ - id : " << M_GETELE(IRQ_Queue).id << "  addr : " << M_GETELE(IRQ_Queue).addr << endl;
                    }
                    M_POP(IRQ_Queue);
                }
                else{
                    *ptr = reqHOST.bitmap;
                    reqHOST.state = REQ_EMPTY;
                }
                return true;
            
            default :

                assert(0 && offset);
                return false;
                
        }
        
}


template < unsigned int NUM_CHANNEL >
void NAND_Manager< NUM_CHANNEL >::IRQ_Thread_CPU1(){

    while(1){

        while(!M_CHECKEMPTY(EvictDone_Queue)){ 
            if(NAND_DEBUG) cout << DEV_AND_TIME << "[IRQ_Thread_CPU1] Thread invoked!" << endl;
            IRQ_Master_CPU1.write(1);

            wait(eIRQ1end);
            if(NAND_DEBUG) cout << DEV_AND_TIME << "[IRQ_Thread_CPU1] IRQ1 done!" << endl;
            IRQ_Master_CPU1.write(0);

            wait(5, SC_NS);
        }
        wait();
    }

}


template < unsigned int NUM_CHANNEL >
void NAND_Manager< NUM_CHANNEL >::IRQ_Thread_CPU2(){

    IRQReq_t* tmpIRQ;
    int index;
    while(1){

        while(!(( M_CHECKEMPTY(IRQ_Queue) ) && (reqHOST.state == REQ_EMPTY)) ){ 
            while(bufUsedCount >= NAND_BUF_ENTRY) wait(eNandQueueDelete);

            if(reqHOST.state == REQ_EMPTY){ //normal flush requests
                if(NAND_DEBUG) cout << DEV_AND_TIME << "[IRQ_Thread_CPU2] Eviction Request" << endl;
                
                //read DRAM (for flush)
                tmpIRQ = &M_GETELE(IRQ_Queue);
                index = findFreeEntry();
                if(NAND_DEBUG) cout << DEV_AND_TIME << "[IRQ_Thread_CPU2] Send DRAM read" << endl;
                Data_Master.read(_ADDR_DATA_DRAM_ + (tmpIRQ->id)*DATA_PAGE_SIZE, \
                        (void*)NandDataBuffer[index].data, PAGE_BYTES); 
                if(NAND_DEBUG) cout << DEV_AND_TIME << "[IRQ_Thread_CPU2] Send DRAM read done" << endl;
               
                //set buffer states
                NandDataBuffer[index].bitmap = tmpIRQ->bitmap;
                NandDataBuffer[index].DRAM_id = tmpIRQ->id;
                NandDataBuffer[index].state = OCCUPIED; 

                cur_buf_index = index;

                M_PUSH(EvictDone_Queue, &tmpIRQ->id);
                eIRQ1.notify();//notify CPU1 
                
                //send CPU2 write request
                IRQ_Master_CPU2.write(0);
                if(NAND_DEBUG) cout << DEV_AND_TIME << "[IRQ_Thread_CPU2] Send IRQ to CPU2" << endl;
                wait(eIRQ2end);
                IRQ_Master_CPU2.write(1);
            }
            else{ //direct NAND write request from HOST

                if(NAND_DEBUG) cout << DEV_AND_TIME << "[IRQ_Thread_CPU2] Direct NAND request" << endl;
                index = findFreeEntry();
                NandDataBuffer[index].DRAM_id = reqHOST.id;
                NandDataBuffer[index].state = OCCUPIED;

                reqHOST.state = REQ_READY;
                IRQ_Master_CPU2.write(0);
                if(NAND_DEBUG) cout << DEV_AND_TIME << "[IRQ_Thread_CPU2] Send IRQ to CPU2" << endl;
                wait(eIRQ2end);
                IRQ_Master_CPU2.write(1);

                if(NAND_DEBUG) cout << DEV_AND_TIME << "[IRQ_Thread_CPU2] Send HOST read" << endl;
                Data_Master.read(_ADDR_DATA_SUBM_, (void*)NandDataBuffer[index].data, PAGE_BYTES); 
                if(NAND_DEBUG) cout << DEV_AND_TIME << "[IRQ_Thread_CPU2] Send HOST read done" << endl;
            }

            NAND_write_count++;

            wait(5, SC_NS);
        }
        wait();
    }




}
    
template < unsigned int NUM_CHANNEL >
int NAND_Manager< NUM_CHANNEL >::findFreeEntry(){

    for(int i=0; i<NAND_BUF_ENTRY; i++){ //search one by one... 
                                        //only 8 entries it wont take much
            
        if(NandDataBuffer[i].state == BUFFER_FREE){
            bufUsedCount++;
            return i;
        }

    }
    assert(0 && "no free entries");
    return 0;

}

template < unsigned int NUM_CHANNEL >
int NAND_Manager< NUM_CHANNEL >::findEntryByID(int id){

    for(int i=0; i<NAND_BUF_ENTRY; i++){ //search one by one... 
                                        //only 8 entries it wont take much
            
        if(NandDataBuffer[i].DRAM_id == id){
            return i;
        }

    }
    assert(0 && "entry not found");
    return 0;

}
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
void NAND_Manager< NUM_CHANNEL >::mergeRNM(uchar* dst, uchar* src, uint bitmap){

     //note : do we need to modify bitmap? (coz of endianness)
     bitmap = ~bitmap; //inverse bitmap


     for(int i=0; bitmap; i++){
        
         if(bitmap%2) memcpy(dst + i*SECTOR_BYTES, src + i*SECTOR_BYTES, SECTOR_BYTES);
         bitmap >>= 1;
         
     }



}
 
    
    // todo: insert definitions of your member functions, etc. here.
template < unsigned int NUM_CHANNEL >
void NAND_Manager< NUM_CHANNEL >::data_queue_thread()
{


    unsigned int iChannel = iSelectedChannel;
    unsigned int *ipCount = &iNandQueueCount[iChannel];
    unsigned int *ipHead = &iNandQueuePtr[iChannel];

    while(1){

        while((*ipCount)){ 
            NAND_Cmd_Buffer* currentCmd = &NandQueue[iChannel][*ipHead]; //get command from queue(FIFO)
            int bufIdx;    
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

                    assert(0 && "read not supported");

                    break;

                case Program : //not done

                    if(NAND_DEBUG)cout << DEV_AND_TIME <<"[data_queue_thread] (Channel " << iChannel << ") Current command : " << currentCmd->cmd.opCode << " on Channel " << iChannel << " iWay"<< iWay<< endl;

#ifdef DATA_COMPARE_ON
                    //DTCMP::writeData(DTCMP::mmNAND, current_NAND_Cmd.iAddr1, current_NAND_Cmd.iAddr3, SECTOR_PER_PAGE, (uchar*)NandDataBuffer[bufIdx].data);
#endif
                    
                    bufIdx =  findEntryByID(currentCmd->DRAM_id);  
                    assert(NandDataBuffer[bufIdx].state == OCCUPIED);
                    
                    NAND_Master[iChannel].write(ADR_NAND_CMD, (void*)&current_NAND_Cmd, sizeof(NAND_Cmd));
                    while((NAND_RnB[iChannel].read()&(1<<iWay))==0){ //wait for RnB
                        DEBUG("[Ch " << iChannel << " Wy " << iWay << "] Waiting for NAND RnB. Current RnB is " << NAND_RnB[iChannel].read());
                        wait(NAND_RnB[iChannel].value_changed_event());
                    }
                    NAND_Master[iChannel].write(ADR_NAND_DATA, (void*) NandDataBuffer[bufIdx].data , PAGE_SIZE); 

                    NandDataBuffer[bufIdx].state = BUFFER_FREE;
                    bufUsedCount--; 
                    eNandQueueDelete.notify();

                    if(NAND_DEBUG) {
                    }

                    break;

                case RNM_Program : //not done


                    //RNM write
                    current_NAND_Cmd.opCode = Program;
                    bufIdx =  findEntryByID(currentCmd->DRAM_id);  
                    //wait for RNM read finish
                    
                    while(NandDataBuffer[bufIdx].state == RNM_WAITING){
                        wait(eRNM_finish);
                    }
                    assert(NandDataBuffer[bufIdx].state == OCCUPIED);
                    
                    while((NAND_RnB[iChannel].read()&(1<<iWay))==0){ //wait for RnB
                        DEBUG("[Ch " << iChannel << " Wy " << iWay << "] Waiting for NAND RnB. Current RnB is " << NAND_RnB[iChannel].read());
                        wait(NAND_RnB[iChannel].value_changed_event());
                    }
                    NAND_Master[iChannel].write(ADR_NAND_CMD, (void*)&current_NAND_Cmd, sizeof(NAND_Cmd));
                    while((NAND_RnB[iChannel].read()&(1<<iWay))==0){ //wait for RnB
                        DEBUG("[Ch " << iChannel << " Wy " << iWay << "] Waiting for NAND RnB. Current RnB is " << NAND_RnB[iChannel].read());
                        wait(NAND_RnB[iChannel].value_changed_event());
                    }
                    
                    NAND_Master[iChannel].write(ADR_NAND_DATA, (void*) NandDataBuffer[bufIdx].data , PAGE_SIZE); 

                    NandDataBuffer[bufIdx].state = BUFFER_FREE;
                    bufUsedCount--; 
                    eNandQueueDelete.notify();

                    if(NAND_DEBUG) {
                    }

                    break;


                case RNM_Read:
                    
                    if(NAND_DEBUG)cout << DEV_AND_TIME <<"[data_queue_thread] (Channel " << iChannel << ") Current command : " << currentCmd->cmd.opCode << " on Channel " << iChannel << " iWay"<< iWay<< endl;

#ifdef DATA_COMPARE_ON
                    //DTCMP::writeData(DTCMP::mmNAND, current_NAND_Cmd.iAddr1, current_NAND_Cmd.iAddr3, SECTOR_PER_PAGE, (uchar*)NandDataBuffer[bufIdx].data);
#endif

                    //RNM read
                    uchar tmpBuffer[PAGE_SIZE];
                    current_NAND_Cmd.opCode = Read;

                    NAND_Master[iChannel].write(ADR_NAND_CMD, (void*)&current_NAND_Cmd, sizeof(NAND_Cmd));
                    while((NAND_RnB[iChannel].read()&(1<<iWay))==0){ //wait for RnB
                        DEBUG("[Ch " << iChannel << " Wy " << iWay << "] Waiting for NAND RnB. Current RnB is " << NAND_RnB[iChannel].read());
                        wait(NAND_RnB[iChannel].value_changed_event());
                    }
                    bufIdx =  findEntryByID(currentCmd->DRAM_id);  
                    NAND_Master[iChannel].read(ADR_NAND_DATA, (void*) tmpBuffer, PAGE_SIZE); 
                   
                    //merge data
                    mergeRNM(NandDataBuffer[bufIdx].data, tmpBuffer, NandDataBuffer[bufIdx].bitmap);
                    NandDataBuffer[bufIdx].state = OCCUPIED;
                    //notify event
                    eRNM_finish.notify();
                    
                    break;

                case Nothing :

                    assert(0 && "undefined opcode detected..");
                    break;

                default :

                    if(NAND_DEBUG) {
                        cout << DEV_AND_TIME <<"[data_queue_thread] (Channel " << iChannel << ") Current command : " << currentCmd->cmd.opCode << " on Channel " << iChannel << " iWay"<< iWay<< endl;
                    }

                    NAND_Master[iChannel].write(ADR_NAND_CMD, (void*)&current_NAND_Cmd, sizeof(NAND_Cmd));

                    break;


            }


            //command finished, update counters
            //iNandWriteQueueCount(or ipCount) ensures that the logically "deleted" queue entries will not be used, so no seperate erase operations are needed
            *ipCount = *ipCount - 1;
            *ipHead =((*ipHead) + 1) % NAND_CMD_QUEUE_SIZE;

            if(NAND_DEBUG)  
                cout << DEV_AND_TIME <<"[data_queue_thread] (Channel " << iChannel << ") Command finished... ipCount : " << *ipCount << " ipHead : " << *ipHead << endl;

            wait(5, SC_NS); //for extra scheduling

        }
        wait();

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
