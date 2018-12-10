//--------------------------------------------------------------                //%
// Important:  The template for this file was generated                         //%
//  automatically and can only be re-generated if rules                         //%
//  are followed when editing:                                                  //%
// 1. Do not modify any line containing //%                                     //%
// 2. Only insert your lines of code between 2 lines                            //%
//    containing "//% user begin" and "//% user end"                            //%
//                                                                              //%
// :generate:date: Tue Jul 05 2011 14:30:37                                     //%
// :generate:version: 2010.1.0-dev                                              //%
//--------------------------------------------------------------                //%
                                                                                //%
#ifndef __NAND_DEV_H__                                                          //%
#define __NAND_DEV_H__                                                          //%
                                                                                //%USERBEGIN MODEL_DEBUG_ENABLE
#if defined(DEBUG) || defined(DBG_SCP)
# define DBG_SCP_NAND_DEV
#endif

#define _FILE_OFFSET_BITS 64
                                                                                //%USEREND MODEL_DEBUG_ENABLE
                                                                                //%
#include <scml2.h>                                                              //%
#include <systemc>                                                              //%
#include "scmlinc/scml_command_processor.h"                                     //%
                                                                                //%USERBEGIN HEADER_H
#include "./header/global_header.h"
#include "./header/header_NAND.h"

extern unsigned int iFileNumber;
                                                                                //%USEREND HEADER_H
                                                                                //%
//------------------------                                                      //%
// Module Interface                                                             //%
//------------------------                                                      //%
                                                                                //%
template < unsigned int NUM_WAY=4 >                                             //%
class NAND_Dev :                                                                //%
	public sc_core::sc_module                                               //%
                                                                                //%USERBEGIN INHERITANCE
    // TODO: Specify class inheritance here. Start each line with a , character
                                                                                //%USEREND INHERITANCE
                                                                                //%
{                                                                               //%
public:                                                                         //%
	// constructor                                                          //%
	NAND_Dev( const sc_core::sc_module_name & n ) ;                         //%
	SC_HAS_PROCESS( NAND_Dev );                                             //%
	// destructor                                                           //%
	virtual ~NAND_Dev();                                                    //%
private:                                                                        //%
	// disable default and copy constructors by making them private         //%
	NAND_Dev() ;                                                            //%
	NAND_Dev( const NAND_Dev& ) ;                                           //%
	NAND_Dev & operator= ( const NAND_Dev& ) ;                              //%
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
	sc_core::sc_in< bool > rst_n;                                           //%
	sc_core::sc_out< bool > NAND_RnB[NUM_WAY];                                    //%
	tlm::tlm_target_socket< 32 > NAND_TRANS;                                //%
                                                                                //%
protected:                                                                      //%
                                                                                //%
	// Memory declarations                                                  //%
	scml2::memory< unsigned int > ND_SFR;                                   //%
	  scml2::memory_alias< unsigned int > ND_SFR_CMD;                       //%
	    scml2::reg< unsigned int > ND_SFR_CMD_OPCODE;                       //%
	    scml2::reg< unsigned int > ND_SFR_CMD_ADDR1;                        //%
	    scml2::reg< unsigned int > ND_SFR_CMD_ADDR2;                        //%
	    scml2::reg< unsigned int > ND_SFR_CMD_ADDR3;                        //%
	    scml2::reg< unsigned int > ND_SFR_CMD_ADDR4;                        //%
	  scml2::memory_alias< unsigned int > ND_SFR_DATA;                      //%
                                                                                //%
	scml2::tlm2_gp_target_adapter< 32 > NAND_TRANS_ND_SFR_adapter;          //%
                                                                                //%
public:                                                                         //%
	// Process declarations                                                 //%
	void                                                                    //%
	rst_nHandler();                                                         //%
	void                                                                    //%
	Way();                                                                  //%
                                                                                //%
protected:                                                                      //%
	// Memory callback declarations                                         //%
	void                                                                    //%
	CMD_Callback(                                                           //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t);          //%
	void                                                                    //%
	DATA_Callback(                                                          //%
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

    
    // MEMBER VARIABLE //

    FILE *m_fp_NAND_Storage;

    sc_core::sc_event e_TrigWay[NUM_WAY];
    NAND_Cmd WAY_CMD[NUM_WAY];
    unsigned int iaBuffer[NUM_WAY][PAGE_SIZE/4];

    unsigned int selectedWay;

    // MEMBER FUNCTION //
    inline int getWayAddr(int addr){
        return (addr & MASK_WAY) >> BIT_FOR_WAY & (NUM_OF_WAY-1) ;
    };

    inline int CommandTransportTime(int CMD){
        if(CMD==Program)
            return 1 + 3 + 2;     // start cmd + row + col
        else if(CMD==Read)
            return 1 + 3 + 2 + 1; // start cmd + row + col + end cmd
        else if(CMD==Erase)
            return 1 + 3 + 1;     // start cmd + row + end cmd
        else if(CMD==Copyback_Read)
            return 1 + 3 + 2 + 1; // start cmd + row + col + end cmd
        else if(CMD==Copyback_Program)
            return 1 + 3 + 2 + 1; // start cmd + row + col + end cmd

        else
            return 0;
    };

    inline int DataTransportTime(int iLength){
        return ((int)(((double)(iLength*8) + 0.5)/NAND_I_O_BITWIDTH)) * NAND_CLK_PERIOD;
    };

    inline int CellHandlingTime(int myway){
        int iCMD = WAY_CMD[myway].opCode;
        if(iCMD==Program)               return T_PROG;
        else if(iCMD==Read)             return T_R;
        else if(iCMD==Erase)            return T_BER;
        else if(iCMD==Copyback_Program) return T_PROG;
        else if(iCMD==Copyback_Read)    return T_R;

        else
            return 0;
    }

	inline void FindFilePosition(int iWayAddr, int iBlockAddr, int iPageAddr)
	{
#ifndef _MAP_CACHING_
   //     cout << DEV_AND_TIME << "Way: " << iWayAddr << "  Blk: " << iBlockAddr << "  Page: " << iPageAddr << endl;
		fseek(m_fp_NAND_Storage, (uint64)iWayAddr * (uint64)TOTAL_WAY_SIZE + (uint64)iBlockAddr * (uint64)BLOCK_SIZE + (uint64)iPageAddr * (uint64)PAGE_SIZE, SEEK_SET);
#endif
	};

	inline void ReadPage(unsigned int *cBuf)
	{
#ifndef _MAP_CACHING_
		fread(cBuf, sizeof(unsigned int), PAGE_SIZE/4, m_fp_NAND_Storage);
#endif

        if(dbgFlag[REGS])
        {
            cout << DEV_AND_TIME << "**** Data READ: ";
            for(int i=0; i < 5 ; i++)
                cout << hex << "0x" << cBuf[i] <<", ";
            cout << " ........ ";
            for(int i=(PAGE_SIZE/4)-5; i<PAGE_SIZE/4; i++)
                cout << hex << "0x" << cBuf[i] <<", ";
            cout << endl;
        }

	};

	inline void WritePage(unsigned int *cBuf)
	{
        if(dbgFlag[REGS])
        {
            cout << DEV_AND_TIME << "**** Data WRITE: " ;
            for(int i=0; i < 5 ; i++)
                cout << hex << "0x" << cBuf[i] <<", ";
            cout << " ........ ";
            for(int i=(PAGE_SIZE/4)-5; i<(PAGE_SIZE/4); i++)
                cout << hex << "0x" << cBuf[i] <<", ";
            cout << endl;
        }
#ifndef _MAP_CACHING_
		fwrite(cBuf, sizeof(unsigned int), PAGE_SIZE/4, m_fp_NAND_Storage);
#endif
	};


    int MaskingAddress(int iAddr, int& iWayAddr, int& iBlockAddr, int& iPageAddr);
    void CellHandling(int myway);


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
                                                                                //%
template < unsigned int NUM_WAY >                                               //%
NAND_Dev< NUM_WAY >::NAND_Dev( const sc_core::sc_module_name & n)               //%
		: sc_core::sc_module(n)                                         //%
,                                                                               //%
 rst_n("rst_n"),                                                                //%
 NAND_TRANS("NAND_TRANS"),                                                      //%
 NAND_TRANS_ND_SFR_adapter("NAND_TRANS_ND_SFR_adapter", NAND_TRANS),            //%
 ND_SFR("ND_SFR", 4165ULL),                                                     //%
  ND_SFR_CMD("ND_SFR_CMD", ND_SFR, 0LL, 5LL ),                                  //%
   ND_SFR_CMD_OPCODE("ND_SFR_CMD_OPCODE", ND_SFR_CMD, 0LL ),                    //%
   ND_SFR_CMD_ADDR1("ND_SFR_CMD_ADDR1", ND_SFR_CMD, 1LL ),                      //%
   ND_SFR_CMD_ADDR2("ND_SFR_CMD_ADDR2", ND_SFR_CMD, 2LL ),                      //%
   ND_SFR_CMD_ADDR3("ND_SFR_CMD_ADDR3", ND_SFR_CMD, 3LL ),                      //%
   ND_SFR_CMD_ADDR4("ND_SFR_CMD_ADDR4", ND_SFR_CMD, 4LL ),                      //%
  //ND_SFR_DATA("ND_SFR_DATA", ND_SFR, 5LL, 2080LL )                              //%
  ND_SFR_DATA("ND_SFR_DATA", ND_SFR, 5LL, 4160LL )                              //%
                                                                                //%
                                                                                //%USERBEGIN INITIALIZER
    // TODO: Add your class initialization here.
                                                                                //%USEREND INITIALIZER
                                                                                //%
{                                                                               //%
 // declare processes and their sensitivities                                   //%
    //cout << "nand_dev constructor" << endl;
 	SC_THREAD(rst_nHandler);                                                //%
	 sensitive << rst_n.neg() ;                                             //%
	 dont_initialize();                                                     //%
    for(int i=0;i<NUM_WAY;i++){
    	SC_THREAD(Way);                                                         //%
    	 dont_initialize();                                                     //%
         sensitive << e_TrigWay[i];
    }                                                                               //%
	// bind target ports to memories                                        //%
	NAND_TRANS_ND_SFR_adapter(ND_SFR);                                      //%
                                                                                //%
 // setup memory callbacks                                                      //%
	// - Memory: ND_SFR                                                     //%
	scml2::set_callback(                                                    //%
		ND_SFR_CMD,                                                     //%
        SCML2_CALLBACK(CMD_Callback),
//		this,                                                           //%
//		&SC_CURRENT_USER_MODULE::CMD_Callback,                          //%
		scml2::AUTO_SYNCING);                                           //%
	scml2::set_callback(                                                    //%
		ND_SFR_DATA,                                                    //%
        SCML2_CALLBACK(DATA_Callback),
//		this,                                                           //%
//		&SC_CURRENT_USER_MODULE::DATA_Callback,                         //%
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
    
    char cFileName[999];

    iFileNumber++;

    sprintf(cFileName, "%d_%s_NAND_contents.in", iFileNumber, sc_gen_unique_name(this->basename()));
   
    //cout << DEV_AND_TIME << "My file name is " << cFileName << endl;

    m_fp_NAND_Storage = fopen(cFileName, "wb+");
    
    if(m_fp_NAND_Storage == NULL){
        cout << "Fail to create a file for " << this->basename() << endl;
        exit(1);
    }


    if ( dbgFlag[GENERAL] ) DBG_MSG("Created " + (std::string)this->basename());
    //cout << "nand_dev constructor" << endl;

                                                                                //%USEREND CONSTRUCTOR
                                                                                //%
}                                                                               //%
// destructor                                                                   //%
                                                                                //%
template < unsigned int NUM_WAY >                                               //%
NAND_Dev< NUM_WAY >::~NAND_Dev()                                                //%
{                                                                               //%
                                                                                //%USERBEGIN DESTRUCTOR

    // TODO: Add destructor code here.

                                                                                //%USEREND DESTRUCTOR
}                                                                               //%
// initialization method for ports                                              //%
                                                                                //%
template < unsigned int NUM_WAY >                                               //%
void                                                                            //%
NAND_Dev< NUM_WAY >::initPorts()                                                //%
{                                                                               //%
	for (unsigned int i = 0; i < NUM_WAY; i++) {                                  //%
		NAND_RnB[i].initialize(0x0);                                    //%
	}                                                                       //%
}                                                                               //%
// initialization method for registers                                          //%
                                                                                //%
template < unsigned int NUM_WAY >                                               //%
void                                                                            //%
NAND_Dev< NUM_WAY >::initRegisters()                                            //%
{                                                                               //%
    ND_SFR.initialize(0x0);                                                     //%
    ND_SFR_CMD.initialize(0x0);                                                 //%
    ND_SFR_CMD_OPCODE.initialize(0x0);                                          //%
    ND_SFR_CMD_ADDR1.initialize(0x0);                                           //%
    ND_SFR_CMD_ADDR2.initialize(0x0);                                           //%
    ND_SFR_CMD_ADDR3.initialize(0x0);                                           //%
    ND_SFR_CMD_ADDR4.initialize(0x0);                                           //%
    ND_SFR_DATA.initialize(0x0);                                                //%
}                                                                               //%
//This method is invoked after the design is fully elaborated, but              //%
// before simulation begins.                                                    //%
                                                                                //%
template < unsigned int NUM_WAY >                                               //%
void                                                                            //%
NAND_Dev< NUM_WAY >::end_of_elaboration()                                       //%
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
template < unsigned int NUM_WAY >                                               //%
std::string                                                                     //%
NAND_Dev< NUM_WAY >::commandProcessor(const std::vector< std::string >& cmd)    //%
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
template < unsigned int NUM_WAY >                                               //%
                                                                                //%
void                                                                            //%
NAND_Dev< NUM_WAY >::initMessageLogging()                                       //%
{                                                                               //%
                                                                                //%USERBEGIN MODEL_DEBUG_INIT
    // toggle on/off debugging flags
    for (int i=0;i<sizeof(DbgCategories);i++) {
#ifdef DBG_SCP_NAND_DEV
        dbgFlag[i] = true;
#else
        dbgFlag[i] = false;
#endif
    }
                                                                                //%USEREND MODEL_DEBUG_INIT
                                                                                //%
}                                                                               //%
// method for logging messages                                                  //%
template < unsigned int NUM_WAY >                                               //%
                                                                                //%
void                                                                            //%
NAND_Dev< NUM_WAY >::REPORT_MSG(std::string sev, std::string msg)               //%
{                                                                               //%
                                                                                //%USERBEGIN REPORT_MSG
    std::cout << DEV_AND_TIME << sev << ": " << msg << endl;
                                                                                //%USEREND REPORT_MSG
                                                                                //%
}                                                                               //%
                                                                                //%
template < unsigned int NUM_WAY >                                               //%
void                                                                            //%
NAND_Dev< NUM_WAY >::rst_nHandler()                                             //%
                                                                                //%USERBEGIN rst_nHandler
{
	if ( dbgFlag[PORTS] ) DBG_MSG( "rst_nHandler invoked because of event on port rst_n" );

    //    wait(10000,SC_NS); // device may require 10us to boot
    //
    //wait(100,SC_NS);
    while(1){
        for(int i=0;i<NUM_WAY;i++)
            NAND_RnB[i].write(1);

        for(int i=0;i<NUM_WAY;i++){
            selectedWay = i;
            e_TrigWay[i].notify();
            wait(SC_ZERO_TIME);
        }
        wait();
    }

}
                                                                                //%USEREND rst_nHandler
                                                                                //%
template < unsigned int NUM_WAY >                                               //%
void                                                                            //%
NAND_Dev< NUM_WAY >::Way()                                                      //%
                                                                                //%USERBEGIN Way
{
    int myway = selectedWay;
	if ( dbgFlag[PORTS] ) DBG_MSG( "Way invoked because of event on port " );
    //cout << ">>>>>>>>>> My way number is " << myway << " <<<<<<<<<<" << endl;

    while(1){
        wait();
        wait(CellHandlingTime(myway), SC_NS);
        CellHandling(myway);
        NAND_RnB[myway].write(1);
    }
}
                                                                                //%USEREND Way
                                                                                //%
template < unsigned int NUM_WAY >                                               //%
void                                                                            //%
NAND_Dev< NUM_WAY >::CMD_Callback(                                              //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)           //%
{                                                                               //%
    const char* dbgMsgText = "TRANSPORT handler for memory ND_SFR_CMD";         //%
    scml2::memory_alias< unsigned int >& m = this->ND_SFR_CMD;                  //%
    tlm::tlm_command cmd = trans.get_command();                                 //%
    sc_dt::uint64 adr = trans.get_address();                                    //%
    unsigned char* ptr = trans.get_data_ptr();                                  //%
    unsigned int len = trans.get_data_length();                                 //%
    unsigned char* byt = trans.get_byte_enable_ptr();                           //%
    unsigned int wid = trans.get_streaming_width();                             //%
                                                                                //%USERBEGIN CMD_Callback
    if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );
    // TODO: INSERT code for the CMD_Callback method here.
    if (byt) { // Target unable to support byte enable attribute
        trans.set_response_status( tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE );
        return;
    }
    if (wid < len) { // Target unable to support streaming width attribute
        trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
        return;
    }

    if (cmd == tlm::TLM_WRITE_COMMAND) { // Execute command
        for(int i=0; i<len/4; i++)
            m[i] = ((unsigned int *)ptr)[i];
        selectedWay = getWayAddr(ND_SFR_CMD_ADDR1);
        
        if(NAND_RnB[selectedWay]==0){ // if RnB on selected Way is busy, alerts error message and stops simulation
            cout << DEV_AND_TIME << "ERROR: Transporting while RnB is busy is prohibited!!" << endl;
            sc_stop();
        }
        
        if(dbgFlag[GENERAL])
            cout << DEV_AND_TIME << "Command Transport: OPCODE is " << ND_SFR_CMD_OPCODE << ", Selected way is " << selectedWay << endl;

        if(ND_SFR_CMD_OPCODE==Chip_Select);
        else{
            WAY_CMD[selectedWay].opCode = NAND_CMD_SET((unsigned int)ND_SFR_CMD_OPCODE);
            WAY_CMD[selectedWay].iAddr1 = ND_SFR_CMD_ADDR1;
            WAY_CMD[selectedWay].iAddr2 = ND_SFR_CMD_ADDR2;
            WAY_CMD[selectedWay].iAddr3 = ND_SFR_CMD_ADDR3;
            WAY_CMD[selectedWay].iAddr4 = ND_SFR_CMD_ADDR4;

            int iCMD = ND_SFR_CMD_OPCODE;

            wait(CommandTransportTime(iCMD),SC_NS);
            if(iCMD==Program){ // RnB is not changed until data transmission
                ;
            }
            else if(iCMD==Read || iCMD==Erase || iCMD==Copyback_Read || iCMD==Copyback_Program){
                NAND_RnB[selectedWay].write(0);
                e_TrigWay[selectedWay].notify();
            }
        }
    }
    else if (cmd == tlm::TLM_READ_COMMAND) {
        for(int i=0; i<len/4; i++)
            ((unsigned int *)ptr)[i] = m[i];
        cout << DEV_AND_TIME << "Warning: Transforting from CMD_Callback is prohibited!!" << endl;
    }
    
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
                                                                                //%USEREND CMD_Callback
}                                                                               //%
                                                                                //%
template < unsigned int NUM_WAY >                                               //%
void                                                                            //%
NAND_Dev< NUM_WAY >::DATA_Callback(                                             //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)           //%
{                                                                               //%
    const char* dbgMsgText = "TRANSPORT handler for memory ND_SFR_DATA";        //%
    scml2::memory_alias< unsigned int >& m = this->ND_SFR_DATA;                 //%
    tlm::tlm_command cmd = trans.get_command();                                 //%
    sc_dt::uint64 adr = trans.get_address();                                    //%
    unsigned char* ptr = trans.get_data_ptr();                                  //%
    unsigned int len = trans.get_data_length();                                 //%
    unsigned char* byt = trans.get_byte_enable_ptr();                           //%
    unsigned int wid = trans.get_streaming_width();                             //%
                                                                                //%USERBEGIN DATA_Callback
    if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );                                                                                
    // TODO: INSERT code for the DATAWay0_Callback method here.

    if (byt) { // Target unable to support byte enable attribute
        trans.set_response_status( tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE );
        return;
    }
    if (wid < len) { // Target unable to support streaming width attribute
        trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
        return;
    }

    unsigned int iTemp;
    unsigned int iStartAddr = adr/4;
    unsigned int *ptr_int = (unsigned int *)ptr;
    
    if(dbgFlag[REGS]){
        if(cmd == tlm::TLM_WRITE_COMMAND)
            cout << DEV_AND_TIME << "Data Transport: " << (cmd==tlm::TLM_WRITE_COMMAND?"write":"read") << ", Way: " << selectedWay << endl;
    }

    if (cmd == tlm::TLM_WRITE_COMMAND) { // Execute command
        for(int i=0; i<len/4; i++){
            memcpy(&iTemp, &(ptr_int[i]), sizeof(int));
            m[iStartAddr + i] = iTemp;
        }                                                                                
        if(WAY_CMD[selectedWay].opCode==Program){
            wait(1,SC_NS);  // program end command time
	    	for(int i=0; i < PAGE_SIZE/sizeof(int); i++)
                iaBuffer[selectedWay][i] = ND_SFR_DATA[i];
            
            /*
            {
                cout << DEV_AND_TIME << "nand program: " << hex;
                for(int i=0; i<10; i++)
                    cout << iaBuffer[selectedWay][i];
                cout << dec;
                cout << endl;
            }
            */

            wait(DataTransportTime(len),SC_NS);

            NAND_RnB[selectedWay].write(0);
            e_TrigWay[selectedWay].notify();
        }
        else
            cout << DEV_AND_TIME << "Error: DATA write is available only for Program command!!" << endl;
    } else if (cmd == tlm::TLM_READ_COMMAND) {
        wait(DataTransportTime(len),SC_NS);
	    for(int i=0; i < PAGE_SIZE/4 ; i++)
            ND_SFR_DATA[i] = iaBuffer[selectedWay][i];

        /*
        {
            cout << "nand read   : " << hex;
            for(int i=0; i<10; i++)
                cout << iaBuffer[selectedWay][i];
            cout << endl;
        }
        */

        for(int i=0; i<len/4; i++){

            iTemp = m[iStartAddr + i];
            memcpy(&(ptr_int[i]), &iTemp, sizeof(int));
        }
    }
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
                                                                                //%USEREND DATA_Callback
}                                                                               //%
                                                                                //%
                                                                                //%USERBEGIN MEMBER_DEFNS
template < unsigned int NUM_WAY >                                               //%
int NAND_Dev<NUM_WAY>::MaskingAddress(int iAddr, int& iWayAddr, int& iBlockAddr, int& iPageAddr){
    iWayAddr = ((iAddr & MASK_WAY) >> BIT_FOR_WAY) & (NUM_OF_WAY-1);
    iBlockAddr = (iAddr & MASK_BLOCK) >> BIT_FOR_BLOCK;
    iPageAddr = (iAddr & MASK_PAGE) >> BIT_FOR_PAGE;
        
    if(dbgFlag[REGS])
        cout << DEV_AND_TIME << "Way: " << iWayAddr << ", Block: " << iBlockAddr << ", Page: " << iPageAddr << endl;
}

template < unsigned int NUM_WAY >                                               //%
void NAND_Dev<NUM_WAY>::CellHandling(int myway)
{
	int iWayAddr, iBlockAddr, iPageAddr;
//	unsigned int iStartAddr = (iWayAddr * PAGE_SIZE) / 4;
//    unsigned int iStartAddrChar = (iWayAddr * PAGE_SIZE);    
//	unsigned int iTemp = 0;
	int cmd = WAY_CMD[myway].opCode;

	if(cmd == Program)
	{
    	this->MaskingAddress(WAY_CMD[myway].iAddr1, iWayAddr, iBlockAddr, iPageAddr);
		FindFilePosition(iWayAddr, iBlockAddr, iPageAddr);
        WritePage(iaBuffer[iWayAddr]);
	}
    else if(cmd == Copyback)
    {
    	this->MaskingAddress(WAY_CMD[myway].iAddr2, iWayAddr, iBlockAddr, iPageAddr);
		FindFilePosition(iWayAddr, iBlockAddr, iPageAddr);
        ReadPage(iaBuffer[iWayAddr]);
        
    	this->MaskingAddress(WAY_CMD[myway].iAddr1, iWayAddr, iBlockAddr, iPageAddr);

		FindFilePosition(iWayAddr, iBlockAddr, iPageAddr);
        WritePage(iaBuffer[iWayAddr]);
    }
	else if(cmd == Copyback_Read)
	{
    	this->MaskingAddress(WAY_CMD[myway].iAddr1, iWayAddr, iBlockAddr, iPageAddr);
		FindFilePosition(iWayAddr, iBlockAddr, iPageAddr);
        ReadPage(iaBuffer[iWayAddr]);
    }
    else if(cmd == Copyback_Program)
    {
    	this->MaskingAddress(WAY_CMD[myway].iAddr1, iWayAddr, iBlockAddr, iPageAddr);
		FindFilePosition(iWayAddr, iBlockAddr, iPageAddr);
        WritePage(iaBuffer[iWayAddr]);
	}
	else if(cmd == Read)
	{
    	this->MaskingAddress(WAY_CMD[myway].iAddr1, iWayAddr, iBlockAddr, iPageAddr);
		FindFilePosition(iWayAddr, iBlockAddr, iPageAddr);
        ReadPage(iaBuffer[iWayAddr]);
	}
	else if(cmd == Erase)
	{
    	this->MaskingAddress(WAY_CMD[myway].iAddr1, iWayAddr, iBlockAddr, iPageAddr);
//        cout << "Block Erase: " << WAY_CMD[myway].iAddr1 << endl;
        for(int i=0;i<PAGE_SIZE/4;i++)
            iaBuffer[iWayAddr][i] = 0;
        for(int i=0;i<NUM_OF_PAGE_PER_BLOCK;i++){
    		FindFilePosition(iWayAddr, iBlockAddr, i);
#ifndef _MAP_CACHING_
    		fwrite(iaBuffer[iWayAddr], sizeof(unsigned int), PAGE_SIZE/4, m_fp_NAND_Storage);
#endif
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
//% begin section previously tagged: FOOTER_H

//% end section previously tagged: FOOTER_H
//% begin section previously tagged: FOOTER_H

//% end section previously tagged: FOOTER_H
//% begin section previously tagged: Way0
{
	if ( dbgFlag[PORTS] ) DBG_MSG( "Way0 invoked because of event on port " );

    int myway = 0;
    
    while(1){
        wait(CellHandlingTime(myway), SC_NS);
        CellHandling(myway);
        NAND_RnB[myway].write(1);
        wait();
    }
}

//% end section previously tagged: Way0
//% begin section previously tagged: Way1
{
	if ( dbgFlag[PORTS] ) DBG_MSG( "Way1 invoked because of event on port " );

    int myway = 1;

    while(1){
        wait(CellHandlingTime(myway), SC_NS);
        CellHandling(myway);
        NAND_RnB[myway].write(1);
        wait();
    }
}
//% end section previously tagged: Way1
//% begin section previously tagged: Way2
{
	if ( dbgFlag[PORTS] ) DBG_MSG( "Way2 invoked because of event on port " );

    int myway = 2;

    while(1){
        wait(CellHandlingTime(myway), SC_NS);
        CellHandling(myway);
        NAND_RnB[myway].write(1);
        wait();
    }
}
//% end section previously tagged: Way2
//% begin section previously tagged: Way3
{
	if ( dbgFlag[PORTS] ) DBG_MSG( "Way3 invoked because of event on port " );

    int myway = 3;

    while(1){
        wait(CellHandlingTime(myway), SC_NS);
        CellHandling(myway);
        NAND_RnB[myway].write(1);
        wait();   
    }
}

//% end section previously tagged: Way3
//% begin section previously tagged: FOOTER_H

//% end section previously tagged: FOOTER_H
//% begin section previously tagged: FOOTER_H

//% end section previously tagged: FOOTER_H
                                                                                //%USEREND ORPHANED_CODE
#endif // ORPHANED_CODE                                                         //%
                                                                                //%USERBEGIN FOOTER_H

                                                                                //%USEREND FOOTER_H
#endif // __NAND_DEV_H__                                                        //%
