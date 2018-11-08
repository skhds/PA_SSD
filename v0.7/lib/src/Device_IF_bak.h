//--------------------------------------------------------------                //%
// Important:  The template for this file was generated                         //%
//  automatically and can only be re-generated if rules                         //%
//  are followed when editing:                                                  //%
// 1. Do not modify any line containing //%                                     //%
// 2. Only insert your lines of code between 2 lines                            //%
//    containing "//% user begin" and "//% user end"                            //%
//                                                                              //%
// :generate:date: Tue Jan 14 2014 17:11:59                                     //%
// :generate:version: 2010.1.0-dev                                              //%
//--------------------------------------------------------------                //%
                                                                                //%
#ifndef __DEVICE_IF_H__                                                         //%
#define __DEVICE_IF_H__                                                         //%
                                                                                //%USERBEGIN MODEL_DEBUG_ENABLE
#if defined(DEBUG) || defined(DBG_SCP)
# define DBG_SCP_DEVICE_IF
#endif
                                                                                //%USEREND MODEL_DEBUG_ENABLE
                                                                                //%
#include <scml2.h>                                                              //%
#include <systemc>                                                              //%
#include "scmlinc/scml_command_processor.h"                                     //%
                                                                                //%USERBEGIN HEADER_H
// TODO: Insert your includes, declarations, etc. here.
#include "./header/ssd_struct.h"
#include "./header/memcpy.h"
#include "./header/IF_Spec.h"
#include "./header/global_flag.h"

#define CPU_SCHEDULING
#define R_CPU_SIZE_ADDR         0x0
#define R_CPU_COMPLETE_ADDR     0x4
#define R_CPU_DATA_ADDR         0x8
 
#define max_req_len 1024

                                                                                //%USEREND HEADER_H
                                                                                //%
//------------------------                                                      //%
// Module Interface                                                             //%
//------------------------                                                      //%
class Device_IF :                                                               //%
	public sc_core::sc_module                                               //%
                                                                                //%USERBEGIN INHERITANCE
    // TODO: Specify class inheritance here. Start each line with a , character
                                                                                //%USEREND INHERITANCE
                                                                                //%
{                                                                               //%
public:                                                                         //%
	// constructor                                                          //%
	Device_IF( const sc_core::sc_module_name & n ) ;                        //%
	SC_HAS_PROCESS( Device_IF );                                            //%
	// destructor                                                           //%
	virtual ~Device_IF();                                                   //%
private:                                                                        //%
	// disable default and copy constructors by making them private         //%
	Device_IF() ;                                                           //%
	Device_IF( const Device_IF& ) ;                                         //%
	Device_IF & operator= ( const Device_IF& ) ;                            //%
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
	scml2::initiator_socket< 32 > Device_IF_Master;                         //%
	tlm::tlm_target_socket< 32 > Device_IF_Slave;                           //%
	sc_core::sc_in< bool > Device_RnB_Slave;                                //%
	sc_core::sc_out< bool > CPU_IRQ;                                        //%
	sc_core::sc_in< bool > rst_n;                                           //%
	tlm::tlm_target_socket< 32 > CPU_Slave;                                 //%
	tlm::tlm_target_socket< 32 > H2D_Slave;                                 //%
	scml2::initiator_socket< 32 > H2D_Master;                               //%
                                                                                //%
protected:                                                                      //%
                                                                                //%
	// Memory declarations                                                  //%
	scml2::memory< unsigned int > R_Device;                                 //%
	  scml2::memory_alias< unsigned int > R_Device_req;                     //%
	  scml2::memory_alias< unsigned int > R_Device_data;                    //%
                                                                                //%
	scml2::tlm2_gp_target_adapter< 32 > Device_IF_Slave_R_Device_adapter;   //%
	scml2::memory< unsigned int > R_CPU;                                    //%
      scml2::memory_alias< unsigned int > R_CPU_size;
      scml2::memory_alias< unsigned int > R_CPU_complete;
      scml2::memory_alias< unsigned int > R_CPU_data;
                                                                                //%
	scml2::tlm2_gp_target_adapter< 32 > CPU_Slave_R_CPU_adapter;            //%
	scml2::memory< unsigned int > R_H2D;                                    //%
      scml2::memory_alias< unsigned int > R_H2D_req;                     //%
	  scml2::memory_alias< unsigned int > R_H2D_data;                    //%
                                                                                //%
	scml2::tlm2_gp_target_adapter< 32 > H2D_Slave_R_H2D_adapter;            //%
                                                                                //%
public:                                                                         //%
	// Process declarations                                                 //%
	void                                                                    //%
	rst_nHandler1();                                                        //%
    
    void 
    Scheduler();
    
    void
    Trans_Request_Module();

    void 
    RnB_Handler();
                                                                                //%
protected:                                                                      //%
	// Memory callback declarations                                         //%
	void                                                                    //%
	Device_Callback(                                                        //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t);          //%
	void                                                                    //%
	CPU_Callback(                                                           //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t);          //%
	void                                                                    //%
	H2DCallback(                                                            //%
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
    Req_t   recievedReq;
    char    recievedData[max_req_len*UNIT_OF_REQUEST];
    char    transData[max_req_len*UNIT_OF_REQUEST];
    uint reqBufferHead;
    uint reqBufferTail;
    uint reqBufferPoint;
    uint reqScheduleHead;
    uint reqScheduleTail;
    Req_t requestBuffer[Q_SIZE];

    Req_t transReq;
    uint bufferSize;
    uint my_state;
    uint next_state;
    bool schedulerFlag;
    FILE *readLatency;
    FILE *writeLatency;
    bool startFlag;
    bool secondFlag;
    
    //Event & Semaphore
    sc_core::sc_event e_TrigScheduler;
    sc_core::sc_event e_TrigTransReq;
    sc_core::sc_event e_ScheduleComplete;
    sc_core::sc_event e_TrigTransModule;
    sc_core::sc_event e_TrigTransHost;
    sc_core::sc_event e_TrigRnBHandler;
    sc_core::sc_event e_PopRequestComplete;
    sc_core::sc_event e_TransReady;
    sc_core::sc_semaphore sem_Trans;
    sc_core::sc_semaphore sem_Sub;
    sc_core::sc_event e_TransData;


    //Function
    inline bool Check_Buffer_Full(){
        if(reqBufferHead == (reqBufferTail + 1) % Q_SIZE){
            return true;        //full
        }else{
            return false;       //not full
        }
    }

    inline bool Check_Buffer_Empty(){
        if(reqBufferPoint == reqBufferTail){
            return true;    //empty
        }else{
            return false;   //not empty
        }
    }

    inline uint Cal_Buffer_Size(uint head, uint tail){
        if(tail > head)
            return tail - head;
        else
            return (tail + Q_SIZE  - head) % Q_SIZE;

    }

    inline void Scheduler_Flag_On(void){
        schedulerFlag = true;

        if(dbgFlag[GENERAL])
            cout << "< 2 > Scheduler flag on !!" << endl;
    }

    inline void Scheduler_Flag_Off(void){
        schedulerFlag = false;
        if(dbgFlag[GENERAL])
            cout <<"< 3 > Scheduler flag off!!" << endl; 
    }

    bool Push_Request(scml2::memory< unsigned int >& m, uint64 len);
    void Copy_Buffer_To_SFR();
    void Copy_SFR_To_Buffer();
    bool Check_Buffer_Schduled();
    Req_t Pop_Request();
    void Copy_Request_Buffer();
    void Copy_Data_Buffer();
    void Delete_Request(uint id);
    bool Check_New_Request();
    void Scheduling_Read_Priority();
    uint Find_Request(uint id);

    void Trans_Request_Host(HOST_REQ_OP op);


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
Device_IF::Device_IF( const sc_core::sc_module_name & n)                        //%
		: sc_core::sc_module(n)                                         //%
,                                                                               //%
 Device_IF_Master("Device_IF_Master"),                                          //%
 Device_IF_Slave("Device_IF_Slave"),                                            //%
 Device_RnB_Slave("Device_RnB_Slave"),                                          //%
 CPU_IRQ("CPU_IRQ"),                                                            //%
 rst_n("rst_n"),                                                                //%
 CPU_Slave("CPU_Slave"),                                                        //%
 H2D_Slave("H2D_Slave"),                                                        //%
 H2D_Master("H2D_Master"),                                                      //%
 Device_IF_Slave_R_Device_adapter("Device_IF_Slave_R_Device_adapter", Device_IF_Slave),//%
 CPU_Slave_R_CPU_adapter("CPU_Slave_R_CPU_adapter", CPU_Slave),                 //%
 H2D_Slave_R_H2D_adapter("H2D_Slave_R_H2D_adapter", H2D_Slave),                 //%
 R_Device("R_Device", 131079ULL),                                               //%
  R_Device_req("R_Device_req", R_Device, 0LL, 7LL ),                            //%
  R_Device_data("R_Device_data", R_Device, 7LL, 131072LL )                      //%
,                                                                               //%
 R_CPU("R_CPU", 3502ULL),                                                       //%
  R_CPU_size("R_CPU_size", R_CPU, 0LL, 1LL),
  R_CPU_complete("R_CPU_complete", R_CPU, 1LL, 1LL),
  R_CPU_data("R_CPU_data", R_CPU, 2LL, 3500LL)
,                                                                               //%
 R_H2D("R_H2D", 131079ULL),                                                           //%
  R_H2D_req("R_H2D_req", R_H2D, 0LL, 7LL),                                                                               //%
  R_H2D_data("R_H2D_data", R_H2D, 7LL, 131072LL)                                                                              //%USERBEGIN INITIALIZER
    // TODO: Add your class initialization here.
    ,sem_Trans(1)
    ,sem_Sub(1)
                                                                                //%USEREND INITIALIZER
                                                                                //%
{                                                                               //%
 // declare processes and their sensitivities                                   //%
	SC_METHOD(rst_nHandler1);                                               //%
	 sensitive << rst_n.neg() ;                                             //%
	 dont_initialize();                                                     //%
     
    SC_THREAD(Scheduler);                                                   //%
	 sensitive << e_TrigScheduler;                                          //%
	 dont_initialize();                                                     //%
    
    SC_THREAD(Trans_Request_Module);                                        //%
	 sensitive << e_TrigTransModule;                                        //%
	 dont_initialize();                                                     //% 
     
    SC_THREAD(RnB_Handler);                                          //%
	 sensitive << e_TrigRnBHandler << Device_RnB_Slave.pos();                                          //%
	 dont_initialize();                                                     //%




                                                                                //%
	// bind target ports to memories                                        //%
	Device_IF_Slave_R_Device_adapter(R_Device);                             //%
	CPU_Slave_R_CPU_adapter(R_CPU);                                         //%
	H2D_Slave_R_H2D_adapter(R_H2D);                                         //%
                                                                                //%
 // setup memory callbacks                                                      //%
	// - Memory: R_Device                                                   //%
	scml2::set_callback(                                                    //%
		R_Device,                                                       //%
		SCML2_CALLBACK(Device_Callback),                                //%
		scml2::AUTO_SYNCING);                                           //%
                                                                                //%
	// - Memory: R_CPU                                                      //%
	scml2::set_callback(                                                    //%
		R_CPU,                                                          //%
		SCML2_CALLBACK(CPU_Callback),                                   //%
		scml2::AUTO_SYNCING);                                           //%
                                                                                //%
	// - Memory: R_H2D                                                      //%
	scml2::set_callback(                                                    //%
		R_H2D,                                                          //%
		SCML2_CALLBACK(H2DCallback),                                    //%
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


    


                                                                                //%USEREND CONSTRUCTOR
                                                                                //%
}                                                                               //%
// destructor                                                                   //%
Device_IF::~Device_IF()                                                         //%
{                                                                               //%
                                                                                //%USERBEGIN DESTRUCTOR

    // TODO: Add destructor code here.
    fclose(readLatency);
    fclose(writeLatency);

                                                                                //%USEREND DESTRUCTOR
}                                                                               //%
// initialization method for ports                                              //%
void                                                                            //%
Device_IF::initPorts()                                                          //%
{                                                                               //%
	CPU_IRQ.initialize(0x0);                                                //%
}                                                                               //%
// initialization method for registers                                          //%
void                                                                            //%
Device_IF::initRegisters()                                                      //%
{                                                                               //%
    R_Device.initialize(0x0);                                                   //%
    R_Device_req.initialize(0x0);                                               //%
    R_Device_data.initialize(0x0);                                              //%
    R_CPU.initialize(0x0);                                                      //%
    R_CPU_size.initialize(0x0);
    R_CPU_complete.initialize(0x0);
    R_CPU_data.initialize(0x0);
    R_H2D.initialize(0x0);                                                      //%
}                                                                               //%
//This method is invoked after the design is fully elaborated, but              //%
// before simulation begins.                                                    //%
void                                                                            //%
Device_IF::end_of_elaboration()                                                 //%
{                                                                               //%
                                                                                //%USERBEGIN END_OF_ELAB
    // TODO: Add end_of_elaboration code here.
    this->initRegisters();
    this->initPorts();

    reqBufferPoint  = 0;
    reqBufferHead   = 0;
    reqBufferTail   = 0;
    schedulerFlag   = false;
    my_state        = IDLE;
    startFlag = false;
    //startFlag = true;
    secondFlag = false;

    readLatency = fopen("readLatency.txt", "w");
    writeLatency = fopen("writeLatency.txt", "w");


                                                                                //%USEREND END_OF_ELAB
}                                                                               //%
// command processing method                                                    //%
//  During simulation, this command may be called to handle                     //%
//  interactive commands -- for dynamic instrumentation of the instance.        //%
std::string                                                                     //%
Device_IF::commandProcessor(const std::vector< std::string >& cmd)              //%
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
Device_IF::initMessageLogging()                                                 //%
{                                                                               //%
                                                                                //%USERBEGIN MODEL_DEBUG_INIT
    // toggle on/off debugging flags
    for (int i=0;i<sizeof(DbgCategories);i++) {
#ifdef DBG_SCP_DEVICE_IF
        dbgFlag[i] = true;
#else
        dbgFlag[i] = false;
#endif
    }
                                                                                //%USEREND MODEL_DEBUG_INIT
                                                                                //%
}                                                                               //%
// method for logging messages                                                  //%
void                                              //%
Device_IF::REPORT_MSG(std::string sev, std::string msg)//%
{                                                 //%
                                                                                //%USERBEGIN REPORT_MSG
    std::cout << sev << ": " << msg << " [" << this->basename() << "@" << sc_core::sc_time_stamp() << "]" << std::endl;
                                                                                //%USEREND REPORT_MSG
                                                                                //%
}                                                                               //%
void                                                                            //%
Device_IF::rst_nHandler1()                                                      //%
                                                                                //%USERBEGIN rst_nHandler1
{
	if ( dbgFlag[PORTS] ) DBG_MSG( "rst_nHandler1 invoked because of event on port rst_n" );

	// TODO: INSERT code for the rst_nHandler1 method here.
    e_TrigRnBHandler.notify();
}
                                                                                //%USEREND rst_nHandler1
void                                                                            //%
Device_IF::Device_Callback(                                                     //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)           //%
{                                                                               //%
    const char* dbgMsgText = "TRANSPORT handler for memory R_Device";           //%
    scml2::memory< unsigned int >& m = this->R_Device;                          //%
    tlm::tlm_command cmd = trans.get_command();                                 //%
    sc_dt::uint64 adr = trans.get_address();                                    //%
    unsigned char* ptr = trans.get_data_ptr();                                  //%
    unsigned int len = trans.get_data_length();                                 //%
    unsigned char* byt = trans.get_byte_enable_ptr();                           //%
    unsigned int wid = trans.get_streaming_width();                             //%
                                                                                //%USERBEGIN Device_Callback
    if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );
    // TODO: INSERT code for the Device_Callback method here.

    if (byt) { // Target unable to support byte enable attribute
        trans.set_response_status( tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE );
        return;
    }
    if (wid < len) { // Target unable to support streaming width attribute
        trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
        return;
    }
    if (cmd == tlm::TLM_WRITE_COMMAND) { // Execute command
        // copy from 'ptr' to your target's memory.  e.g.: memcpy(&mem[adr], ptr, num_bytes);

        if(DEVIF_DEBUG) cout << DEV_AND_TIME << "[Device_Callback] TLM WRITE Received " << endl;
        
        ReadResp_t recievedReadReq;

        memcpy(&recievedReadReq, ptr, len);

        memcpy(&recievedReq, &(recievedReadReq.hostReq), sizeof(Req_t));

        memcpy(recievedData, recievedReadReq.cpData, UNIT_OF_REQUEST*recievedReadReq.hostReq.iLen);

        if(DEVIF_DEBUG) cout << DEV_AND_TIME << "[Device_Callback] Device IF recieved ID = " << recievedReadReq.hostReq.iId << endl;
        //wait((double)16.0/ (double)IF_BANDWIDTH * 1000, SC_NS);
        //wait(((double)recievedReq.iLen * (double)UNIT_OF_REQUEST / (double)IF_BANDWIDTH) * 1000.0 , SC_NS);
        //cout <<"start " << sc_time_stamp().value()/1000 << endl;
    
        
        Trans_Request_Host(recievedReadReq.hostReq.Op);


        //cout <<"end " << sc_time_stamp().value()/1000 << endl;
        Delete_Request(recievedReadReq.hostReq.iId);

        my_state = next_state;


    } else if (cmd == tlm::TLM_READ_COMMAND) {
        // copy from your target's memory to 'ptr', e.g.: memcpy(ptr, &mem[adr], num_bytes);
    }

    if(DEVIF_DEBUG) cout << DEV_AND_TIME << "[Device_Callback] Callback done, sending OK_RESPONSE..." << endl;
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
    //%USEREND Device_Callback
}//%

void                                                                            //%
Device_IF::CPU_Callback(                                                        //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)           //%
{                                                                               //%
    const char* dbgMsgText = "TRANSPORT handler for memory R_CPU";              //%
    scml2::memory< unsigned int >& m = this->R_CPU;                             //%
    scml2::memory_alias< unsigned int >& m_data = this->R_CPU_data;             //%
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
    if (cmd == tlm::TLM_WRITE_COMMAND) { // Execute command
        // copy from 'ptr' to your target's memory.  e.g.: memcpy(&mem[adr], ptr, num_bytes);
        if(adr == R_CPU_COMPLETE_ADDR){
            //cout << "Device IF scheduling end!" << endl;
            Copy_SFR_To_Buffer();

            CPU_IRQ.write(0x0);
            e_ScheduleComplete.notify();
        }else{
            R_CPU_data[(adr/4)-2] = *((unsigned int *)ptr);
        }

        
    } else if (cmd == tlm::TLM_READ_COMMAND) {
        // copy from your target's memory to 'ptr', e.g.: memcpy(ptr, &mem[adr], num_bytes);
        if(adr == R_CPU_SIZE_ADDR){
            memcpy(ptr, R_CPU_size, len);
        }else{ 
            *((unsigned int*)ptr) = R_CPU_data[(adr/4)-2];
        }
    }
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
                                                                                //%USEREND CPU_Callback
}                                                                               //%
void                                                                            //%
Device_IF::H2DCallback(                                                         //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)           //%
{                                                                               //%
    const char* dbgMsgText = "TRANSPORT handler for memory R_H2D";              //%
    scml2::memory< unsigned int >& m = this->R_H2D;                             //%
    tlm::tlm_command cmd = trans.get_command();                                 //%
    sc_dt::uint64 adr = trans.get_address();                                    //%
    unsigned char* ptr = trans.get_data_ptr();                                  //%
    unsigned int len = trans.get_data_length();                                 //%
    unsigned char* byt = trans.get_byte_enable_ptr();                           //%
    unsigned int wid = trans.get_streaming_width();                             //%
                                                                                //%USERBEGIN H2DCallback
    if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );
    // TODO: INSERT code for the H2DCallback method here.

    if (byt) { // Target unable to support byte enable attribute
        trans.set_response_status( tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE );
        return;
    }
    if (wid < len) { // Target unable to support streaming width attribute
        trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
        return;
    }
    if (cmd == tlm::TLM_WRITE_COMMAND) { // Execute command
        // copy from 'ptr' to your target's memory.  e.g.: memcpy(&mem[adr], ptr, num_bytes);
        
        if(adr == 0x0){
            memcpy(m, ptr, len);
            
            if(Push_Request(m, len) == false){
                trans.set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );
                return;
            }



            if(dbgFlag[GENERAL])
                cout << "Recieved request HOST to device IF " << endl; 
            e_TrigTransModule.notify();
            //e_TrigScheduler.notify();
        }else if(adr == 0x28){
            if(dbgFlag[GENERAL])
                cout << "Recieved Data HOST to device IF" << endl;

                //cout << sc_core::sc_time_stamp()<< "    Recieved Data HOST to device IF" << endl;
            
            memcpy(transData, ptr, len*UNIT_OF_REQUEST);
            
            e_TransData.notify();
            
        }else{
            if(DEVIF_DEBUG) cout << adr <<"       "<<len << "      " <<endl;
            ABORT(1, "Device Interface: H2D Callback recieve wrong address!!!");
        }

    } else if (cmd == tlm::TLM_READ_COMMAND) {
        // copy from your target's memory to 'ptr', e.g.: memcpy(ptr, &mem[adr], num_bytes);
    }
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
                                                                                //USEREND H2DCallback
}                                                                               //%
                                                                                //%
                                                                                //%USERBEGIN MEMBER_DEFNS
// TODO: Insert definitions of your member functions, etc. here.
void
Device_IF::Scheduler()
{
    while(1){
        wait(); //disabled schedule for now
        if(dbgFlag[GENERAL])
            cout << "<2> Scheduler involked " <<endl; 

        if(Cal_Buffer_Size(reqBufferPoint, reqBufferTail) != 1 && Check_New_Request() == true){
//            Scheduler_Flag_On();
//
//            //cout <<"device scheduling" <<endl;
//            Copy_Buffer_To_SFR();
//
//            CPU_IRQ.write(0x1);
//
//            wait(e_ScheduleComplete);
//            
//            Scheduler_Flag_Off();
//
//            wait(SC_ZERO_TIME);
        }

        if(DEVIF_DEBUG) cout <<DEV_AND_TIME << "1111111schedul wait "<<endl;
        //e_TrigTransModule.notify();

        if(DEVIF_DEBUG) cout <<DEV_AND_TIME << "schedul wait "<<endl;
        wait(e_PopRequestComplete);
        if(DEVIF_DEBUG) cout << DEV_AND_TIME<<"schedul wait end"<<endl;
        
        if(Check_Buffer_Empty() == true){
            wait();
        }else{
            wait(CLOCK_PERIOD,SC_NS);
        }

    }
}

void
Device_IF::Trans_Request_Module()
{

    while(1){
        if(DEVIF_DEBUG) cout << DEV_AND_TIME << "111Check Buffer Empty : " << Check_Buffer_Empty() << " schedulerFlag : " << schedulerFlag << endl;
        while(schedulerFlag == true || Check_Buffer_Empty() == true){
        if(DEVIF_DEBUG) cout << DEV_AND_TIME << "222Check Buffer Empty : " << Check_Buffer_Empty() << " schedulerFlag : " << schedulerFlag << endl;
            wait();
        }
        if(DEVIF_DEBUG) cout << DEV_AND_TIME << "Check Buffer Empty : " << Check_Buffer_Empty() << " schedulerFlag : " << schedulerFlag << endl;
        if(Check_Buffer_Empty() == false && schedulerFlag == false){

            transReq = Pop_Request();

            e_PopRequestComplete.notify();

            if(dbgFlag[GENERAL]){
                cout << "< 2 > Device to Module trans :  ";
                cout << "ID :  "<<transReq.iId << "  OP :  "  << transReq.Op << " Addr :   "  << transReq.iAddr << "  Length  :   "  << transReq.iLen << endl;
            }

            //Request trans

            if(transReq.Op == HOST_WRITE){
                //wait((double)16.0/ (double)IF_BANDWIDTH * 1000, SC_NS);
                Trans_Request_Host(transReq.Op);
                
                wait(e_TransData);

                if(DEVIF_DEBUG) cout << DEV_AND_TIME << "transReq.iId :  " << transReq.iId  << endl;
                if(DEVIF_DEBUG) cout << DEV_AND_TIME << "RnB Debugging RnB1 start "  << endl;

                if(g_initialize_end) {
                    testTrack.track(transReq.iId, transReq.iAddr, transReq.iLen, SSDmetric::DEV_IF, sc_time_stamp().value()/1000.0);// for write 
                }
                while(Device_RnB_Slave.read() == 0x0){
                    wait(Device_RnB_Slave.posedge_event());
                }

                if(DEVIF_DEBUG) cout << DEV_AND_TIME << "RnB Debugging RnB1 end "  << endl;

                sem_Sub.wait();
                //cout << "Device IF cmd start time = " << sc_time_stamp().value() << endl;              
                Device_IF_Master.write(0, (unsigned char *)(&transReq), sizeof(Req_t));
                //cout << "Device IF cmd start end = " << sc_time_stamp().value() << endl;              
                Device_IF_Master.write(0, (unsigned char *)transData, transReq.iLen*UNIT_OF_REQUEST);
                //cout << "Device IF data end = " << sc_time_stamp().value() << endl;              
                sem_Sub.post();

                if(g_initialize_end) {
                    testTrack.done(transReq.iId, transReq.iAddr, transReq.iLen, SSDmetric::DEV_IF, sc_time_stamp().value()/1000.0);// for write 
                }
                Delete_Request(transReq.iId);
                if(DEVIF_DEBUG) cout <<"From Device IF to Sub Write CMD"<<endl;

            }else{

                if(g_initialize_end) {
                    testTrack.track(transReq.iId, transReq.iAddr, transReq.iLen, SSDmetric::DEV_IF, sc_time_stamp().value()/1000.0);// for read 
                }
                
                if(DEVIF_DEBUG) cout << DEV_AND_TIME << "RnB Debugging RnB2 start "  << endl;
                while(Device_RnB_Slave.read() == 0x0){
                    wait(Device_RnB_Slave.posedge_event());
                }

                if(DEVIF_DEBUG) cout << DEV_AND_TIME << "RnB Debugging RnB2 end "  << endl;
                //wait((double)16.0/ (double)IF_BANDWIDTH * 1000, SC_NS);
                sem_Sub.wait();
                Device_IF_Master.write(0, (unsigned char *)(&transReq), sizeof(Req_t));
                sem_Sub.post();

                if(g_initialize_end) testTrack.track(transReq.iId, transReq.iAddr, transReq.iLen, SSDmetric::DEV_IF, sc_time_stamp().value()/1000.0);// for read
                
                if(DEVIF_DEBUG) cout << DEV_AND_TIME << "[Trans_Request_Module] From Device IF to Sub Read CMD"<<endl;
            }
            if(DEVIF_DEBUG) cout << DEV_AND_TIME << "[Trans_Request_Module] Device IF send the request to SubReqMan"<<endl;
        }
        wait(SC_ZERO_TIME);
    }
}

void
Device_IF::RnB_Handler()
{
    while(1){
        wait();
        e_TransReady.notify();
    }
}

bool
Device_IF::Check_New_Request()
{
    for(uint i = reqScheduleTail; i != reqBufferTail ; i = (i+1)%Q_SIZE){

        if(requestBuffer[i].Op == HOST_READ){
            return true;
        }
    }
    
    return false;


}

void
Device_IF::Trans_Request_Host(HOST_REQ_OP op)
{

    sem_Trans.wait();    
    if(DEVIF_DEBUG) cout << DEV_AND_TIME << "[Trans_Request_Host] op :  " << op << endl;
    if(op == HOST_WRITE){

        if(DEVIF_DEBUG) cout << DEV_AND_TIME << "[Trans_Request_Host] sending write to host. ID : " << transReq.iId << endl;
        //wait((double)sizeof(Req_t)/ (double)IF_BANDWIDTH * 1000, SC_NS);
        //wait((double)20.0/ (double)IF_BANDWIDTH * 1000, SC_NS);
        H2D_Master.write(0x0, (unsigned char *)&transReq, sizeof(Req_t));

        if(DEVIF_DEBUG) cout << DEV_AND_TIME << "[Trans_Request_Host] write complete" << endl;
    }else if(op == HOST_READ){
        if(DEVIF_DEBUG) cout << DEV_AND_TIME << "[Trans_Request_Host] sending read to host, ID : " << recievedReq.iId << endl;

        if(g_initialize_end) testTrack.track(recievedReq.iId, recievedReq.iAddr, recievedReq.iLen, SSDmetric::DEV_IF, sc_time_stamp().value()/1000.0); 
        
        //wait((double)sizeof(Req_t)/ (double)IF_BANDWIDTH * 1000, SC_NS);
        H2D_Master.write(0x0, (unsigned char *)&recievedReq, sizeof(Req_t));
        //wait(((double)recievedReq.iLen * (double)UNIT_OF_REQUEST / (double)IF_BANDWIDTH) * 1000.0 , SC_NS);
     
        if(DEVIF_DEBUG) cout << DEV_AND_TIME << "[Trans_Request_Host] read command sent, transporting data.. ID : " << recievedReq.iId << endl;
        
        
        H2D_Master.write(0x0, (unsigned char *)recievedData, recievedReq.iLen*UNIT_OF_REQUEST);
        
        if(g_initialize_end){
            testTrack.done(recievedReq.iId, recievedReq.iAddr, recievedReq.iLen, SSDmetric::DEV_IF, sc_time_stamp().value()/1000.0); 
        }
        if(DEVIF_DEBUG) cout << DEV_AND_TIME << "[Trans_Request_Host] read complete, ID : " << recievedReq.iId << endl;
    }else{
        if(DEVIF_DEBUG) cout << "Trans request host wrong operation" << endl;
        sc_stop();
    }
    
    sem_Trans.post();
}

void
Device_IF::Copy_Buffer_To_SFR()
{

    scml2::memory_alias< unsigned int >& m = this->R_CPU_data;                       //%
    reqScheduleHead = (reqBufferPoint + 1) % Q_SIZE;
    reqScheduleTail = reqBufferTail;
    uint k=0;

    bufferSize = Cal_Buffer_Size(reqScheduleHead, reqBufferTail);
    //cout << "SIZE  :  "  << reqScheduleHead << "  "  << reqScheduleTail << endl;
    //cout << "Device interface to CPU : Data size =  " << bufferSize << endl;

    memcpy(R_CPU_size, &bufferSize, sizeof(bufferSize));

    //cout << "Register SIZE data :  " << (unsigned int)R_CPU_size[0] << endl;

    //cout << "Q_SIZE = " << Q_SIZE <<endl;
    for(uint i=reqScheduleHead; i != reqScheduleTail; i = (i + 1)%Q_SIZE)
    {
        for(uint j=0; j<7; j++)
        {
            m[k*7+j] = ((unsigned int*)&requestBuffer[i])[j];
            //cout <<"register data = " << m[k*7+j] << endl;
        }
        k++;
    }
}
 
void
Device_IF::Copy_SFR_To_Buffer()
{

    scml2::memory_alias< unsigned int >& m = this->R_CPU_data;                       //%
    uint k=0;
    uint tempPoint = (reqBufferPoint+1)%Q_SIZE;

    for(uint i=0; i<bufferSize; i++){
        for(uint j=0;j<7;j++){
            ((unsigned int*)&requestBuffer[tempPoint])[j] = m[k*7+j];
        }
        k++;

        tempPoint = (tempPoint+1)%Q_SIZE;

    }

}
   
bool
Device_IF::Push_Request(scml2::memory< unsigned int >& m, uint64 len) 
{
    uint    addr;
    HOST_REQ_OP op;
    
    //ABORT(Check_Buffer_Full() == true, "Request Queue don't should be full");
    if(Check_Buffer_Full() == true){
        //cout << name() << " q full " <<endl;
        return false;
    }
    
    memcpy(&requestBuffer[reqBufferTail], m, len);
    
    if( g_initialize_end) testTrack.start(requestBuffer[reqBufferTail].iId, requestBuffer[reqBufferTail].iAddr, requestBuffer[reqBufferTail].iLen, SSDmetric::DEV_IF, sc_time_stamp().value()/1000.0);//skhds
    
    reqBufferTail = (reqBufferTail + 1) % Q_SIZE;
    return true;
}

Req_t
Device_IF::Pop_Request()
{
    Req_t tmpReq;
    //uint popReqPoint;

    tmpReq = requestBuffer[reqBufferPoint];
    //popReqPoint = reqBufferPoint;
    requestBuffer[reqBufferPoint].dIssueTime = (double)sc_time_stamp().value()/1000.0;
    //cout << "Pop request Id = " << requestBuffer[reqBufferPoint].iId << endl;

    reqBufferPoint = (reqBufferPoint + 1) % Q_SIZE;
    
    //return popReqPoint;
    return tmpReq;

}

bool 
Device_IF::Check_Buffer_Schduled()
{
    if(reqBufferPoint == reqScheduleHead && CPU_IRQ.read() == 0x01)
        return false;
    else
        return true;
}

void
Device_IF::Copy_Request_Buffer()
{
    memcpy(&recievedReq, R_Device_req, sizeof(Req_t));

}

void
Device_IF::Copy_Data_Buffer()
{
    memcpy(recievedData, R_Device_data, UNIT_OF_REQUEST*recievedReq.iLen);

}

void
Device_IF::Delete_Request(uint id)
{
    bool findFlag = false;
    uint deletePoint;

    if(DEVIF_DEBUG) cout << DEV_AND_TIME << "[Delete_Request] id :  " << id << endl;
    deletePoint = Find_Request(id);

    if (!startFlag) {
        if (secondFlag) {
            if ((requestBuffer[deletePoint].iId) == 0) {
                startFlag = true;
                fprintf(writeLatency, "%lf\n", (double)sc_time_stamp().value()/1000);
            }
        }

        if ((requestBuffer[deletePoint].iId) == 0) {
            secondFlag = true;
        }
    }

    if(startFlag){
        //cout << " Start time =  " << sc_time_stamp().value()/1000 << endl;
        if(requestBuffer[deletePoint].Op == HOST_WRITE){
            fprintf(writeLatency, "%lf\t%d\n", (double)sc_time_stamp().value()/1000 - requestBuffer[deletePoint].dIssueTime, requestBuffer[deletePoint].iLen);
        }else{
            fprintf(readLatency, "%lf\t%d\n", (double)sc_time_stamp().value()/1000 - requestBuffer[deletePoint].dIssueTime, requestBuffer[deletePoint].iLen);
        }
    }

    if(dbgFlag[GENERAL])
        cout << "< 2 > Delete request !!   " << id <<endl;

    memset(&requestBuffer[deletePoint], 0x0, sizeof(Req_t));
    
    //move request
    for(uint j=deletePoint; j != reqBufferHead; j = (j+Q_SIZE-1) % Q_SIZE){
        memcpy(&requestBuffer[j], &requestBuffer[(j+Q_SIZE-1) % Q_SIZE], sizeof(Req_t));
    }
    

    //cout << "Device IF delete point = " << id <<"        " <<reqBufferHead <<"  " << reqBufferTail <<"   " << reqBufferPoint <<endl; 
    reqBufferHead = (reqBufferHead + 1) % Q_SIZE;


    if(DEVIF_DEBUG) cout << DEV_AND_TIME << "[Delete_Request] Delete done id :  " << id << endl;
}

uint
Device_IF::Find_Request(uint Id)
{
    bool findFlag = false;
    uint findPoint;

    for(uint i=reqBufferHead; i != reqBufferPoint; i = (i+1)%Q_SIZE){
        if(requestBuffer[i].iId == Id){
            findPoint = i;
            findFlag = true;
            break;
        }
    }

    if(findFlag == false){
        if(dbgFlag[GENERAL])
            cout << "<2>Don't find recieved request!!" << endl;
        
            cout << "<2>Don't find recieved request!!" << endl;
            cout << "Device IF" << endl;
        sc_stop();
    }

    return findPoint;
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
//% begin section previously tagged: FOOTER_H

//% end section previously tagged: FOOTER_H
                                                                                //%USEREND ORPHANED_CODE
#endif // ORPHANED_CODE                                                         //%
                                                                                //%USERBEGIN FOOTER_H

                                                                                //%USEREND FOOTER_H
#endif // __DEVICE_IF_H__                                                       //%
