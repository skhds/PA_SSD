//--------------------------------------------------------------                //%
// Important:  The template for this file was generated                         //%
//  automatically and can only be re-generated if rules                         //%
//  are followed when editing:                                                  //%
// 1. Do not modify any line containing //%                                     //%
// 2. Only insert your lines of code between 2 lines                            //%
//    containing "//% user begin" and "//% user end"                            //%
//                                                                              //%
// :generate:date: Thu Mar 12 2015 11:05:11                                     //%
// :generate:version: 2010.1.0-dev                                              //%
//--------------------------------------------------------------                //%
                                                                                //%
#ifndef __MEMORY_CONTROLLER_H__                                                 //%
#define __MEMORY_CONTROLLER_H__                                                 //%
                                                                                //%USERBEGIN MODEL_DEBUG_ENABLE
#if defined(DEBUG) || defined(DBG_SCP)
# define DBG_SCP_MEMORY_CONTROLLER
#endif
                                                                                //%USEREND MODEL_DEBUG_ENABLE
                                                                                //%
#include <scml2.h>                                                              //%
#include <systemc>                                                              //%
#include "scmlinc/scml_command_processor.h"                                     //%
                                                                                //%USERBEGIN HEADER_H
// TODO: Insert your includes, declarations, etc. here.
#include "./header/global_flag.h"
using namespace std;

#define REQUEST 0x0
#define GRANT   0x4
#define RELEASE 0x8
#define NUM_OF_CHANNEL  4
#define EMPTY           0xf

unsigned int requestChannel[NUM_OF_CHANNEL];
                                                                                //%USEREND HEADER_H
                                                                                //%
//------------------------                                                      //%
// Module Interface                                                             //%
//------------------------                                                      //%
class Memory_Controller :                                                       //%
	public sc_core::sc_module                                               //%
                                                                                //%USERBEGIN INHERITANCE
    // TODO: Specify class inheritance here. Start each line with a , character
                                                                                //%USEREND INHERITANCE
                                                                                //%
{                                                                               //%
public:                                                                         //%
	// constructor                                                          //%
	Memory_Controller( const sc_core::sc_module_name & n ) ;                //%
	SC_HAS_PROCESS( Memory_Controller );                                    //%
	// destructor                                                           //%
	virtual ~Memory_Controller();                                           //%
private:                                                                        //%
	// disable default and copy constructors by making them private         //%
	Memory_Controller() ;                                                   //%
	Memory_Controller( const Memory_Controller& ) ;                         //%
	Memory_Controller & operator= ( const Memory_Controller& ) ;            //%
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
	tlm::tlm_target_socket< 32 > SFR;                                       //%
                                                                                //%
protected:                                                                      //%
                                                                                //%
	// Memory declarations                                                  //%
	scml2::memory< unsigned int > SFRRegBank;                               //%
	  scml2::reg< unsigned int > SFRRegBank_RequestCPU;                     //%
	  scml2::reg< unsigned int > SFRRegBank_GrantCPU;                       //%
	  scml2::reg< unsigned int > SFRRegBank_ReleaseCPU;                     //%
                                                                                //%
	scml2::tlm2_gp_target_adapter< 32 > SFR_SFRRegBank_adapter;             //%
                                                                                //%
                                                                                //%
protected:                                                                      //%
	// Memory callback declarations                                         //%
	void                                                                    //%
	Mutual_Exclustion_Callback(                                             //%
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
Memory_Controller::Memory_Controller( const sc_core::sc_module_name & n)        //%
		: sc_core::sc_module(n)                                         //%
,                                                                               //%
 SFR("SFR"),                                                                    //%
 SFR_SFRRegBank_adapter("SFR_SFRRegBank_adapter", SFR),                         //%
 SFRRegBank("SFRRegBank", 3ULL),                                                //%
  SFRRegBank_RequestCPU("SFRRegBank_RequestCPU", SFRRegBank, 0LL ),             //%
  SFRRegBank_GrantCPU("SFRRegBank_GrantCPU", SFRRegBank, 1LL ),                 //%
  SFRRegBank_ReleaseCPU("SFRRegBank_ReleaseCPU", SFRRegBank, 2LL )              //%
                                                                                //%
                                                                                //%USERBEGIN INITIALIZER
    // TODO: Add your class initialization here.
                                                                                //%USEREND INITIALIZER
                                                                                //%
{                                                                               //%
 // declare processes and their sensitivities                                   //%
                                                                                //%
	// bind target ports to memories                                        //%
	SFR_SFRRegBank_adapter(SFRRegBank);                                     //%
                                                                                //%
 // setup memory callbacks                                                      //%
	// - Memory: SFRRegBank                                                 //%
	scml2::set_callback(                                                    //%
		SFRRegBank,                                                     //%
		SCML2_CALLBACK(Mutual_Exclustion_Callback),                     //%
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
Memory_Controller::~Memory_Controller()                                         //%
{                                                                               //%
                                                                                //%USERBEGIN DESTRUCTOR

    // TODO: Add destructor code here.

                                                                                //%USEREND DESTRUCTOR
}                                                                               //%
// initialization method for ports                                              //%
void                                                                            //%
Memory_Controller::initPorts()                                                  //%
{                                                                               //%
}                                                                               //%
// initialization method for registers                                          //%
void                                                                            //%
Memory_Controller::initRegisters()                                              //%
{                                                                               //%
    SFRRegBank.initialize(0x0);                                                 //%
    SFRRegBank_RequestCPU.initialize(0x0);                                      //%
    SFRRegBank_GrantCPU.initialize(0x0);                                        //%
    SFRRegBank_ReleaseCPU.initialize(0x0);                                      //%
}                                                                               //%
//This method is invoked after the design is fully elaborated, but              //%
// before simulation begins.                                                    //%
void                                                                            //%
Memory_Controller::end_of_elaboration()                                         //%
{                                                                               //%
                                                                                //%USERBEGIN END_OF_ELAB
    // TODO: Add end_of_elaboration code here.
    this->initRegisters();
    this->initPorts();

    for(unsigned int i = 0; i < NUM_OF_CHANNEL; i++)
        requestChannel[i] = 0;

    SFRRegBank_GrantCPU = EMPTY;
                                                                                //%USEREND END_OF_ELAB
}                                                                               //%
// command processing method                                                    //%
//  During simulation, this command may be called to handle                     //%
//  interactive commands -- for dynamic instrumentation of the instance.        //%
std::string                                                                     //%
Memory_Controller::commandProcessor(const std::vector< std::string >& cmd)      //%
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
Memory_Controller::initMessageLogging()                                         //%
{                                                                               //%
                                                                                //%USERBEGIN MODEL_DEBUG_INIT
    // toggle on/off debugging flags
    for (int i=0;i<sizeof(DbgCategories);i++) {
#ifdef DBG_SCP_MEMORY_CONTROLLER
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
Memory_Controller::REPORT_MSG(std::string sev, std::string msg)//%
{                                                 //%
                                                                                //%USERBEGIN REPORT_MSG
    std::cout << sev << ": " << msg << " [" << this->basename() << "@" << sc_core::sc_time_stamp() << "]" << std::endl;
                                                                                //%USEREND REPORT_MSG
                                                                                //%
}                                                                               //%
void                                                                            //%
Memory_Controller::Mutual_Exclustion_Callback(                                  //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)           //%
{                                                                               //%
    const char* dbgMsgText = "TRANSPORT handler for memory SFRRegBank";         //%
    scml2::memory< unsigned int >& m = this->SFRRegBank;                        //%
    tlm::tlm_command cmd = trans.get_command();                                 //%
    sc_dt::uint64 adr = trans.get_address();                                    //%
    unsigned char* ptr = trans.get_data_ptr();                                  //%
    unsigned int len = trans.get_data_length();                                 //%
    unsigned char* byt = trans.get_byte_enable_ptr();                           //%
    unsigned int wid = trans.get_streaming_width();                             //%
                                                                                //%USERBEGIN Mutual_Exclustion_Callback
    static bool isGranted = false;

    if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );
    // TODO: INSERT code for the Mutual_Exclustion_Callback method here.

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
        unsigned int ch = *(unsigned int *)ptr;
        if(adr == REQUEST){ // 0x00
            //cout << "Mem ctrl request channel = " << ch << endl;
            requestChannel[ch] = 1;
        }else if(adr == RELEASE){ // 0x08
            //cout << "Mem ctrl release channel = " << ch << endl;
            if(requestChannel[ch] == 0)
            {
                cout << "error" << endl;
                sc_stop();
            }
            requestChannel[ch] = 0;
            isGranted = false;

//            SFRRegBank_GrantCPU = EMPTY;
        }else{
            cout << "Mem ctrl is received wrong write address = " << adr << endl;
            sc_stop();
        }
    } else if (cmd == tlm::TLM_READ_COMMAND) {
        // copy from your target's memory to 'ptr', e.g.: memcpy(ptr, &mem[adr], num_bytes);
        if(adr == GRANT)
        {
            if(isGranted == false)
            {
                *(unsigned int*)ptr = EMPTY;

                unsigned int i = (SFRRegBank_GrantCPU + 1) % NUM_OF_CHANNEL;
                do
                {
                    if(requestChannel[i] == 1){
                        SFRRegBank_GrantCPU = i;
                        //cout << "GRANT CH = " << SFRRegBank_GrantCPU << endl;
                        isGranted = true;
                        *(unsigned int*)ptr = SFRRegBank_GrantCPU;
                        break;
                    }
                    i = (i + 1) % NUM_OF_CHANNEL;
                }while (i != ((SFRRegBank_GrantCPU + 1) % NUM_OF_CHANNEL));
            }
            else
            {
                *(unsigned int*)ptr = SFRRegBank_GrantCPU;
            }
        }else{
            cout << "Mem ctrl is received wrong read address = " << adr << endl;
            sc_stop();
        }
/*
        if(adr == GRANT){ // 0x04
            if(SFRRegBank_GrantCPU == EMPTY){
                unsigned int i, j;
                for(i = SFRRegBank_GrantCPU, j=0; j < NUM_OF_CHANNEL; i = (i+1)%NUM_OF_CHANNEL, j++){
                    if(requestChannel[i] == 1){
                        SFRRegBank_GrantCPU = i;
                        cout << "GRANT CH = " << SFRRegBank_GrantCPU << endl;
                        break;
                    }
                }

            }
            *(unsigned int*)ptr = SFRRegBank_GrantCPU;
        }else{
            cout << "Mem ctrl is received wrong read address = " << adr << endl;
            sc_stop();
        }
        */

    }
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
                                                                                //%USEREND Mutual_Exclustion_Callback
}                                                                               //%
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
#endif // __MEMORY_CONTROLLER_H__                                               //%
