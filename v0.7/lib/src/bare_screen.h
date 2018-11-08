//--------------------------------------------------------------                //%
// Important:  The template for this file was generated                         //%
//  automatically and can only be re-generated if rules                         //%
//  are followed when editing:                                                  //%
// 1. Do not modify any line containing //%                                     //%
// 2. Only insert your lines of code between 2 lines                            //%
//    containing "//% user begin" and "//% user end"                            //%
//                                                                              //%
// :generate:date: Wed May 07 2014 15:36:23                                     //%
// :generate:version: 2010.1.0-dev                                              //%
//--------------------------------------------------------------                //%
                                                                                //%
#ifndef __BARE_SCREEN_H__                                                       //%
#define __BARE_SCREEN_H__                                                       //%
                                                                                //%USERBEGIN MODEL_DEBUG_ENABLE
#if defined(DEBUG) || defined(DBG_SCP)
//# define DBG_SCP_BARE_SCREEN
#endif
                                                                                //%USEREND MODEL_DEBUG_ENABLE
                                                                                //%
#include <scml2.h>                                                              //%
#include <systemc>                                                              //%
#include "scmlinc/scml_command_processor.h"                                     //%
                                                                                //%USERBEGIN HEADER_H
// TODO: Insert your includes, declarations, etc. here.

#include <stack>
                                                                                //%USEREND HEADER_H
                                                                                //%
//------------------------                                                      //%
// Module Interface                                                             //%
//------------------------                                                      //%
class bare_screen :                                                             //%
	public sc_core::sc_module                                               //%
                                                                                //%USERBEGIN INHERITANCE
    // TODO: Specify class inheritance here. Start each line with a , character
                                                                                //%USEREND INHERITANCE
                                                                                //%
{                                                                               //%
public:                                                                         //%
	// constructor                                                          //%
	bare_screen( const sc_core::sc_module_name & n ) ;                      //%
	SC_HAS_PROCESS( bare_screen );                                          //%
	// destructor                                                           //%
	virtual ~bare_screen();                                                 //%
private:                                                                        //%
	// disable default and copy constructors by making them private         //%
	bare_screen() ;                                                         //%
	bare_screen( const bare_screen& ) ;                                     //%
	bare_screen & operator= ( const bare_screen& ) ;                        //%
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
	tlm::tlm_target_socket< 32 > nums;                                      //%
	sc_core::sc_in< bool > rst_n;                                           //%
                                                                                //%
protected:                                                                      //%
                                                                                //%
	// Memory declarations                                                  //%
	scml2::memory< unsigned int > numRegs;                                  //%
	  scml2::reg< unsigned int > numRegs_charReg;                           //%
	  scml2::reg< unsigned int > numRegs_intReg;                            //%
	  scml2::reg< unsigned int > numRegs_floatReg;                          //%
	  scml2::reg< unsigned int > numRegs_hexReg;                         //%
	  scml2::reg< unsigned int > numRegs_int64MReg;                         //%
	  scml2::reg< unsigned int > numRegs_int64LReg;                         //%
	  scml2::reg< unsigned int > numRegs_doubleMReg;                         //%
	  scml2::reg< unsigned int > numRegs_doubleLReg;                         //%
	  scml2::reg< unsigned int > numRegs_int64hexMReg;                         //%
	  scml2::reg< unsigned int > numRegs_int64hexLReg;                         //%
                                                                                //%
	scml2::tlm2_gp_target_adapter< 32 > nums_numRegs_adapter;               //%
                                                                                //%
public:                                                                         //%
	// Process declarations                                                 //%
	void                                                                    //%
	rst_nHandler();                                                         //%
                                                                                //%
protected:                                                                      //%
	// Memory callback declarations                                         //%
	void                                                                    //%
	screenCallback(                                                         //%
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

    unsigned int m_iDoubleBuffer;

    typedef enum eSoftwareRoutineId{
        SW_RT_FTL = 0,
        SW_RT_ADDR_TRANS,
        SW_RT_FORE_GC,
        SW_RT_DYN_WL,

        SW_RT_ID_CNT

    }eSwRtId;

    typedef struct sSoftwareTimeMeasure{
        eSwRtId id;
        double dStartTime;
    }sSwRtTime;

    double m_daSwRtTime[SW_RT_ID_CNT];
    std::stack<sSwRtTime> m_swIdStack;
    ofstream m_fpSwRtTime;
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
bare_screen::bare_screen( const sc_core::sc_module_name & n)                    //%
		: sc_core::sc_module(n)                                         //%
,                                                                               //%
 nums("nums"),                                                                  //%
 rst_n("rst_n"),                                                                //%
 nums_numRegs_adapter("nums_numRegs_adapter", nums),                            //%
 numRegs("numRegs", 10ULL),                                                      //%
  numRegs_charReg("numRegs_charReg", numRegs, 0LL ),                            //%
  numRegs_intReg("numRegs_intReg", numRegs, 1LL ),                              //%
  numRegs_floatReg("numRegs_floatReg", numRegs, 2LL ),                          //%
  numRegs_hexReg("numRegs_hexReg", numRegs, 3LL ),                         //%
  numRegs_int64MReg("numRegs_int64MReg", numRegs, 4LL ),                         //%
  numRegs_int64LReg("numRegs_int64LReg", numRegs, 5LL ),                         //%
  numRegs_doubleMReg("numRegs_doubleMReg", numRegs, 6LL ),                         //%
  numRegs_doubleLReg("numRegs_doubleLReg", numRegs, 7LL ),                         //%
  numRegs_int64hexMReg("numRegs_int64hexMReg", numRegs, 8LL ),                         //%
  numRegs_int64hexLReg("numRegs_int64hexLReg", numRegs, 9LL )                         //%
                                                                                //%
                                                                                //%USERBEGIN INITIALIZER
    // TODO: Add your class initialization here.
                                                                                //%USEREND INITIALIZER
                                                                                //%
{                                                                               //%
 // declare processes and their sensitivities                                   //%
	SC_METHOD(rst_nHandler);                                                //%
	 sensitive << rst_n.neg() ;                                             //%
	 dont_initialize();                                                     //%
                                                                                //%
	// bind target ports to memories                                        //%
	nums_numRegs_adapter(numRegs);                                          //%
                                                                                //%
 // setup memory callbacks                                                      //%
	// - Memory: numRegs                                                    //%
	scml2::set_callback(                                                    //%
		numRegs,                                                        //%
		SCML2_CALLBACK(screenCallback),                                 //%
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

    memset(m_daSwRtTime, 0x00, sizeof(double) * SW_RT_ID_CNT);
    m_fpSwRtTime.open("sw_rt_time.txt");

                                                                                //%USEREND CONSTRUCTOR
                                                                                //%
}                                                                               //%
// destructor                                                                   //%
bare_screen::~bare_screen()                                                     //%
{                                                                               //%
                                                                                //%USERBEGIN DESTRUCTOR

    // TODO: Add destructor code here.
    
    m_fpSwRtTime.close();

                                                                                //%USEREND DESTRUCTOR
}                                                                               //%
// initialization method for ports                                              //%
void                                                                            //%
bare_screen::initPorts()                                                        //%
{                                                                               //%
}                                                                               //%
// initialization method for registers                                          //%
void                                                                            //%
bare_screen::initRegisters()                                                    //%
{                                                                               //%
    numRegs.initialize(0x0);                                                    //%
    numRegs_charReg.initialize(0x0);                                            //%
    numRegs_intReg.initialize(0x0);                                             //%
    numRegs_floatReg.initialize(0x0);                                           //%
    numRegs_hexReg.initialize(0x0);                                          //%
    numRegs_int64MReg.initialize(0x0);                                          //%
    numRegs_int64LReg.initialize(0x0);                                          //%
    numRegs_doubleMReg.initialize(0x0);                                          //%
    numRegs_doubleLReg.initialize(0x0);                                          //%
    numRegs_int64hexMReg.initialize(0x0);                                          //%
    numRegs_int64hexLReg.initialize(0x0);                                          //%
}                                                                               //%
//This method is invoked after the design is fully elaborated, but              //%
// before simulation begins.                                                    //%
void                                                                            //%
bare_screen::end_of_elaboration()                                               //%
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
bare_screen::commandProcessor(const std::vector< std::string >& cmd)            //%
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
bare_screen::initMessageLogging()                                               //%
{                                                                               //%
                                                                                //%USERBEGIN MODEL_DEBUG_INIT
    // toggle on/off debugging flags
    for (int i=0;i<sizeof(DbgCategories);i++) {
#ifdef DBG_SCP_BARE_SCREEN
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
bare_screen::REPORT_MSG(std::string sev, std::string msg)//%
{                                                 //%
                                                                                //%USERBEGIN REPORT_MSG
    std::cout << sev << ": " << msg << " [" << this->basename() << "@" << sc_core::sc_time_stamp() << "]" << std::endl;
                                                                                //%USEREND REPORT_MSG
                                                                                //%
}                                                                               //%
void                                                                            //%
bare_screen::rst_nHandler()                                                     //%
                                                                                //%USERBEGIN rst_nHandler
{
	if ( dbgFlag[PORTS] ) DBG_MSG( "rst_nHandler invoked because of event on port rst_n" );

	// TODO: INSERT code for the rst_nHandler method here.
}
                                                                                //%USEREND rst_nHandler
void                                                                            //%
bare_screen::screenCallback(                                                    //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)           //%
{                                                                               //%
    const char* dbgMsgText = "TRANSPORT handler for memory numRegs";            //%
    scml2::memory< unsigned int >& m = this->numRegs;                           //%
    tlm::tlm_command cmd = trans.get_command();                                 //%
    sc_dt::uint64 adr = trans.get_address();                                    //%
    unsigned char* ptr = trans.get_data_ptr();                                  //%
    unsigned int len = trans.get_data_length();                                 //%
    unsigned char* byt = trans.get_byte_enable_ptr();                           //%
    unsigned int wid = trans.get_streaming_width();                             //%
                                                                                //%USERBEGIN screenCallback
    if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );
    // TODO: INSERT code for the screenCallback method here.

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
        unsigned long long int inputInt64;
        unsigned long long int inputDouble_int;
        if(adr == 0x0){ //_ADDR_PRINT_CHAR_
            char inputChar = ptr[0];
            std::cout << inputChar;
            std::cout.flush();
        }else if(adr == 0x4){ //_ADDR_PRINT_INT_
            unsigned int inputInt = *((unsigned int *)ptr);
            std::cout << inputInt;
            std::cout.flush();
        }else if(adr == 0x8){ //_ADDR_PRINT_FLOAT_
            /* added by BryanKim 171208 */
            unsigned int inputFloat_int = *((unsigned int*)ptr);
            float* inputFloat_p = (float*)&inputFloat_int;
            std::cout << *inputFloat_p;
            std::cout.flush();
            /* deleted by BryanKim 171208
                            // sw routine start;
            unsigned int rtId = *((unsigned int *)ptr);

            if(rtId == 0 && m_swIdStack.size() != 0){
                cout << "SW stack should be empty !" << endl;
                sc_stop();
            }

            if(rtId == 0){
                memset(m_daSwRtTime, 0x00, sizeof(double) * SW_RT_ID_CNT);
            }

            sSwRtTime newSwRt;
            newSwRt.id = (eSwRtId)rtId;
            newSwRt.dStartTime = sc_time_stamp().to_double();
 
            m_swIdStack.push(newSwRt);
            */

        }else if(adr == 0xC){ //_ADDR_PRINT_HEX_
            /* added by BryanKim 171208 */ 
            unsigned int inputInt = *((unsigned int *)ptr);
            std::cout << std::hex << inputInt;
            std::cout.flush();
            /* deleted by BryanKim 171208               
                            // sw routine end;

            unsigned int rtId = *((unsigned int *)ptr);

            if(rtId != (unsigned int)(m_swIdStack.top().id))
            {
                cout << "Stack top value is not matched with incoming value" << endl;
                sc_stop();
            }

            double dCurTime = sc_time_stamp().to_double() - m_swIdStack.top().dStartTime;
            m_daSwRtTime[rtId] += dCurTime;

            if(rtId == 0){
                for(int i=0; i < SW_RT_ID_CNT ; i++){
                    m_fpSwRtTime << m_daSwRtTime[i] << "\t";
                }
                m_fpSwRtTime << "\n";
                m_fpSwRtTime.flush();
            }

            m_swIdStack.pop();
            */

        }
        //added by BryanKim 171208
        else if(adr == 0x10){ //_ADDR_PRINT_INT64_MSB_
            unsigned int inputInt64_MSB = *((unsigned int *)ptr);
            inputInt64 = (unsigned long long int)0xFFFFFFFF00000000 & ((unsigned long long int)inputInt64_MSB << 32);
        }
        else if(adr == 0x14){ //_ADDR_PRINT_INT64_LSB_
            unsigned int inputInt64_LSB = *((unsigned int *)ptr);
            inputInt64 += (unsigned long long int)0xFFFFFFFF & (unsigned long long int)inputInt64_LSB;
            std::cout << std::dec << inputInt64;
            std::cout.flush();
        }
        else if(adr == 0x18){ //_ADDR_PRINT_DOUBLE_MSB_
            unsigned int inputDouble_int_MSB = *((unsigned int *)ptr);
            inputDouble_int = (unsigned long long int)0xFFFFFFFF00000000 & ((unsigned long long int)inputDouble_int_MSB << 32);

        }
        else if(adr == 0x1C){ //_ADDR_PRINT_DOUBLE_LSB_
            unsigned int inputDouble_int_LSB = *((unsigned int *)ptr);
            inputDouble_int += ((unsigned long long int)0xFFFFFFFF & ((unsigned long long int)inputDouble_int_LSB));
            double* inputDouble_p = (double*)&inputDouble_int;
            std::cout << *inputDouble_p;
            std::cout.flush();
        }
        else if(adr == 0x20){ //_ADDR_PRINT_INT64HEX_MSB_
            unsigned int inputInt64_MSB = *((unsigned int *)ptr);
            inputInt64 = (unsigned long long int)0xFFFFFFFF00000000 & ((unsigned long long int)inputInt64_MSB << 32);
        }
        else if(adr == 0x24){ //_ADDR_PRINT_INT64HEX_LSB_
            unsigned int inputInt64_LSB = *((unsigned int *)ptr);
            inputInt64 += (unsigned long long int)0xFFFFFFFF & (unsigned long long int)inputInt64_LSB;
            std::cout << std::hex << inputInt64;
            std::cout.flush();
        }
        else{
            DBG_MSG("Port address cannot be accepted\n");                    
        }

    } else if (cmd == tlm::TLM_READ_COMMAND) {
        // copy from your target's memory to 'ptr', e.g.: memcpy(ptr, &mem[adr], num_bytes);
        DBG_MSG("This port cannot be read\n");
    }
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
                                                                                //%USEREND screenCallback
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

//% begin section previously tagged: FOOTER_H

//% end section previously tagged: FOOTER_H
                                                                                //%USEREND ORPHANED_CODE
#endif // ORPHANED_CODE                                                         //%
                                                                                //%USERBEGIN FOOTER_H

                                                                                //%USEREND FOOTER_H
#endif // __BARE_SCREEN_H__                                                     //%
