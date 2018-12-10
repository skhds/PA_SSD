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
#ifndef __DRAM_CTRL_H__                                                         //%
#define __DRAM_CTRL_H__                                                         //%
                                                                                //%USERBEGIN MODEL_DEBUG_ENABLE
#if defined(DEBUG) || defined(DBG_SCP)
# define DBG_SCP_DRAM_CTRL
#endif
                                                                                //%USEREND MODEL_DEBUG_ENABLE
                                                                                //%
#include <scml2.h>                                                              //%
#include <systemc>                                                              //%
#include "scmlinc/scml_command_processor.h"                                     //%
                                                                                //%USERBEGIN HEADER_H
// TODO: Insert your includes, declarations, etc. here.
#include "./header/global_header.h"
#include "./header/header_DRAM.h"

//For debugging


                                                                                //%USEREND HEADER_H
                                                                                //%
//------------------------                                                      //%
// Module Interface                                                             //%
//------------------------                                                      //%
class DRAM_ctrl :                                                               //%
	public sc_core::sc_module                                               //%
                                                                                //%USERBEGIN INHERITANCE
    // TODO: Specify class inheritance here. Start each line with a , character
                                                                                //%USEREND INHERITANCE
                                                                                //%
{                                                                               //%
public:                                                                         //%
	// constructor                                                          //%
	DRAM_ctrl( const sc_core::sc_module_name & n ) ;                        //%
	SC_HAS_PROCESS( DRAM_ctrl );                                            //%
	// destructor                                                           //%
	virtual ~DRAM_ctrl();                                                   //%
private:                                                                        //%
	// disable default and copy constructors by making them private         //%
	DRAM_ctrl() ;                                                           //%
	DRAM_ctrl( const DRAM_ctrl& ) ;                                         //%
	DRAM_ctrl & operator= ( const DRAM_ctrl& ) ;                            //%
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
	scml2::initiator_socket< 32 > StateInform;                              //%
	scml2::initiator_socket< 32 > MemoryMasterPort;                         //%
	tlm::tlm_target_socket< 32 > CMD;                                       //%
	tlm::tlm_target_socket< 32 > DATA;                                      //%
	sc_core::sc_in< bool > rst_n;                                           //%
                                                                                //%
protected:                                                                      //%
                                                                                //%
	// Memory declarations                                                  //%
	scml2::memory< unsigned int > R_CMD;                                    //%
	scml2::tlm2_gp_target_adapter< 32 > CMD_R_CMD_adapter;                  //%

	scml2::memory< unsigned int > R_DATA;                                   //%
	scml2::tlm2_gp_target_adapter< 32 > DATA_R_DATA_adapter;                //%

                                                                                //%
public:                                                                         //%
	// Process declarations                                                 //%
	void rst_nHandler1();                                                   //%

    
                                                                                //%
protected:                                                                      //%
	// Memory callback declarations                                         //%
	void                                                                    //%
	CMD_Callback(                                                           //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t);              //%
	void                                                                    //%
	DATA_Callback(                                                          //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t);              //%
	
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
    //Event

    sc_core::sc_semaphore sem_Mem;


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
DRAM_ctrl::DRAM_ctrl( const sc_core::sc_module_name & n)                        //%
		: sc_core::sc_module(n)                                                 //%
,                                                                               //%
 CMD("CMD"),                                                                    //%
 DATA("DATA"),                                                                  //%
 rst_n("rst_n"),                                                                //%
 StateInform("StateInform"),                                                    //%
 MemoryMasterPort("MemoryMasterPort"),                                          //%
 
 CMD_R_CMD_adapter("CMD_R_CMD_adapter", CMD),                                   //%
 DATA_R_DATA_adapter("DATA_R_DATA_adapter", DATA),                              //%
 
 R_CMD("R_CMD", 268435456ULL),                                                     //%
 R_DATA("R_DATA", 1048576ULL)                                                       //%

 // TODO: Add your class initialization here.
    ,sem_Mem(1)
    //%USEREND INITIALIZER
                                                                                //%
{                                                                               //%
 // declare processes and their sensitivities                                   //%
	SC_METHOD(rst_nHandler1);                                               //%
	 sensitive << rst_n.neg() ;                                             //%
	 dont_initialize();                                                     //%
 
     

                                                                            //%
	// bind target ports to memories                                        //%
	CMD_R_CMD_adapter(R_CMD);                                               //%
	DATA_R_DATA_adapter(R_DATA);                                            //%
                                                                            //%
 // setup memory callbacks                                                  //%
	// - Memory: R_CMD                                                      //%
	scml2::set_callback(                                                    //%
		R_CMD,
		SCML2_CALLBACK(CMD_Callback),                                       //%
		scml2::AUTO_SYNCING);                                               //%
                                                                            //%
	// - Memory: R_DATA                                                     //%
	scml2::set_callback(                                                    //%
		R_DATA,                                                             //%
		SCML2_CALLBACK(DATA_Callback),                                      //%
		scml2::AUTO_SYNCING);                                               //%
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
DRAM_ctrl::~DRAM_ctrl()                                                         //%
{                                                                               //%
                                                                                //%USERBEGIN DESTRUCTOR
    // TODO: Add destructor code here.

                                                                                //%USEREND DESTRUCTOR
}                                                                               //%
// initialization method for ports                                              //%
void                                                                            //%
DRAM_ctrl::initPorts()                                                          //%
{                                                                               //%
}                                                                               //%
// initialization method for registers                                          //%
void                                                                            //%
DRAM_ctrl::initRegisters()                                                      //%
{                                                                               //%
    R_CMD.initialize(0x0);                                                      //%
    R_DATA.initialize(0x0);                                                     //%
}                                                                               //%
//This method is invoked after the design is fully elaborated, but              //%
// before simulation begins.                                                    //%
void                                                                            //%
DRAM_ctrl::end_of_elaboration()                                                 //%
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
DRAM_ctrl::commandProcessor(const std::vector< std::string >& cmd)              //%
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
DRAM_ctrl::initMessageLogging()                                                 //%
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
DRAM_ctrl::REPORT_MSG(std::string sev, std::string msg)//%
{                                                 //%
                                                                                //%USERBEGIN REPORT_MSG
    std::cout << sev << ": " << msg << " [" << this->basename() << "@" << sc_core::sc_time_stamp() << "]" << std::endl;
                                                                                //%USEREND REPORT_MSG
                                                                                //%
}                                                                               //%
void                                                                            //%
DRAM_ctrl::rst_nHandler1()                                                      //%
                                                                                //%USERBEGIN rst_nHandler1
{
	if ( dbgFlag[PORTS] ) DBG_MSG( "rst_nHandler1 invoked because of event on port rst_n" );

	// TODO: INSERT code for the rst_nHandler1 method here.
    buffer_write_count = 0;
    meta_write_count = 0;
}
                                                                                //%USEREND rst_nHandler1



void                                                                            //%
DRAM_ctrl::CMD_Callback(                                                     //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)           //%
{   
    
    
    double BW_time_start = sc_time_stamp().value()/1000.0;
    
    
    const char* dbgMsgText = "TRANSPORT handler for memory R_CMD";           //%
    scml2::memory< unsigned int >& m = this->R_CMD;                          //%
    tlm::tlm_command cmd = trans.get_command();                                 //%
    sc_dt::uint64 adr = trans.get_address();                                    //%
    unsigned int* ptr = (unsigned int*)trans.get_data_ptr();                                  //%
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

    //bus delay
    wait( len/CMD_BUS_TOTAL_BW, SC_NS);
    if (cmd == tlm::TLM_WRITE_COMMAND) {
        if(adr < CACHE_DATA_REGION_OFFSET ){
            if(g_initialize_end){            
                if(buffer_write_count >= (uint)(0x0 - 0x200)) cout << "WARNING : POSSIBLE OVERFLOW ON COUNTING BUFFER WRITE COUNT!!!" << endl;
                if(len) { // this part is a little bit fishy, I'm going to change this soon....
                    buffer_write_count += (len - 1)/32 + 1;
                    meta_write_count += (len - 1)/32 + 1;
                }
            }
            sem_Mem.wait();
            if(g_initialize_end) {

                wait(memLatency(len, META, cacheWRITE), SC_NS);

            }   
            MemoryMasterPort.write(adr, ptr, len);
            sem_Mem.post();
            trans.set_response_status( tlm::TLM_OK_RESPONSE );

        }else{
            
            cout << "somethings wrong... " << endl;
            assert(0);

            
        }
    } else if (cmd == tlm::TLM_READ_COMMAND) {

        
        
        if((adr < CACHE_DATA_REGION_OFFSET) && (adr >= 0x20)){
            
            sem_Mem.wait();
            if(g_initialize_end){
            
                wait(memLatency(len, META, cacheREAD), SC_NS);
            
            }
            MemoryMasterPort.read(adr, ptr, len);
            sem_Mem.post();
            trans.set_response_status( tlm::TLM_OK_RESPONSE );

        }
        else{

            cout << DEV_AND_TIME 
                <<"[CMD_Callback] ERROR: MEMORY ctrl don't receive TLM_READ_COMMAND!!" << endl;
            assert(0);
        }
    }

    
}//%

void                                                                            //%
DRAM_ctrl::DATA_Callback(                                                        //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)           //%
{                                                                               //%
    
    
    double BW_time_start = sc_time_stamp().value()/1000.0;
    
    const char* dbgMsgText = "TRANSPORT handler for memory R_DATA";              //%
    scml2::memory< unsigned int >& m = this->R_DATA;                             //%
    tlm::tlm_command cmd = trans.get_command();                                 //%
    sc_dt::uint64 adr = trans.get_address();                                    //%
    unsigned char* ptr = trans.get_data_ptr();                                  //%
    unsigned int len = trans.get_data_length()/SECTOR_BYTES;                                 //%
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
    
    //bus delay
    //simple DRAM controller
    //16KB page : 10us (bus delay) + 25us (DRAM access)
    //Pipelining is not implemented for simplicity (will do later)
    //DRAM bandwidth will be lowered from 640MB/s to 460MB/s as a result..   

    len = len*SECTOR_BYTES;
    wait( (len)/DATA_BUS_TOTAL_BW, SC_NS);
    if (cmd == tlm::TLM_WRITE_COMMAND) { 
        // copy from 'ptr' to your target's memory.  e.g.: memcpy(&mem[adr], ptr, num_bytes);
        
       
        
        


        sem_Mem.wait();
        MemoryMasterPort.write(adr + CACHE_DATA_REGION_OFFSET, ptr, len);
        
        if(g_initialize_end){

            wait(memLatency(len, BUFFER, cacheWRITE), SC_NS);

        }
        
        sem_Mem.post();
    
    
    } else if (cmd == tlm::TLM_READ_COMMAND) {
        // copy from your target's memory to 'ptr', e.g.: memcpy(ptr, &mem[adr], num_bytes);
      
         

        sem_Mem.wait();
        if(g_initialize_end){

            wait(memLatency(len, BUFFER, cacheREAD), SC_NS);

        }
        MemoryMasterPort.read(adr + CACHE_DATA_REGION_OFFSET, ptr, len);
        sem_Mem.post();
        
    }
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
}                                                                               //%
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
#endif // __DRAM_CTRL_H__                                                       //%
