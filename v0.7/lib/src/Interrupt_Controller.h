//--------------------------------------------------------------                //%
// Important:  The template for this file was generated                         //%
//  automatically and can only be re-generated if rules                         //%
//  are followed when editing:                                                  //%
// 1. Do not modify any line containing //%                                     //%
// 2. Only insert your lines of code between 2 lines                            //%
//    containing "//% user begin" and "//% user end"                            //%
//                                                                              //%
// :generate:date: Tue Jan 14 2014 16:57:28                                     //%
// :generate:version: 2010.1.0-dev                                              //%
//--------------------------------------------------------------                //%
                                                                                //%
#ifndef __INTERRUPT_CONTROLLER_H__                                              //%
#define __INTERRUPT_CONTROLLER_H__                                              //%
                                                                                //%USERBEGIN MODEL_DEBUG_ENABLE
#if defined(DEBUG) || defined(DBG_SCP)
# define DBG_SCP_INTERRUPT_CONTROLLER
#endif
                                                                                //%USEREND MODEL_DEBUG_ENABLE
                                                                                //%
#include <scml2.h>                                                              //%
#include <systemc>                                                              //%
#include "scmlinc/scml_command_processor.h"                                     //%
                                                                                //%USERBEGIN HEADER_H
// TODO: Insert your includes, declarations, etc. here.

//////////////////////////////////////////////////////////////////
///////////////////////////// modified by rab *
//////////////////////////////////////////////////////////////////

#include <list>
#include "./header/global_header.h"


using namespace std;

bool test;

typedef struct sInterruptSources{
    unsigned int    iId;                // integer ID
    double          dIssuedTime;        // issued time
    unsigned int    iSrc;               // source num;
    unsigned int    iTargetCore;        // target core id;
}sIntSrc;

typedef enum{
    Empty = 15,
    DeviceIF = 0,
    CacheBuffer = 4,
    FTL_0 = 2,
    FTL_1 = 5,
    FTL_2 = 6,
    FTL_3 = 7,
    FTL_4 = 8,
    FTL_5 = 9,
    FTL_6 = 10,
    FTL_7 = 11,
    FTL_8 = 12,
    FTL_9 = 13,
    INIT = 14,
    FTL = 16,
    NANDFIN  = 17
}InterruptType;

#define DEVICEIF_ADDR 0xc
//#define CACHEBUFFER_ADDR 0x10
#define FTL_0_ADDR         0x14
#define FTL_1_ADDR         0x18
#define FTL_2_ADDR         0x1c
#define FTL_3_ADDR         0x20
#define FTL_4_ADDR         0x24
#define FTL_5_ADDR         0x28
#define FTL_6_ADDR         0x2c
#define FTL_7_ADDR         0x30
#define FTL_8_ADDR         0x34
#define FTL_9_ADDR         0x38
#define INIT_COMPLETE      0x3c
#define NAND_COMPLETE      0x10 
#define NUM_OF_TASK 3
#define NUM_OF_CORE 1
unsigned int CPUState[NUM_OF_CORE];
unsigned int TaskPriority[NUM_OF_TASK];
#define IRQ_PERIOD (10)

double cpu_time[10];

//////////////////////////////////////////////////////////////////

                                                                                //%USEREND HEADEH
                                                                                //%
//------------------------                                                      //%
// Module Interface                                                             //%
//------------------------                                                      //%
                                                                                //%
template < unsigned int NUM_PORT=4 >                                            //%
class Interrupt_Controller :                                                    //%
	public sc_core::sc_module                                               //%
                                                                                //%USERBEGIN INHERITANCE
    // TODO: Specify class inheritance here. Start each line with a , character
                                                                                //%
{                                                                               //%
public:                                                                         //%
	// constructor                                                          //%
	Interrupt_Controller( const sc_core::sc_module_name & n ) ;             //%
	SC_HAS_PROCESS( Interrupt_Controller );                                 //%
	// destructor                                                           //%
	virtual ~Interrupt_Controller();                                        //%
private:                                                                        //%
	// disable default and copy constructors by making them private         //%
	Interrupt_Controller() ;                                                //%
	Interrupt_Controller( const Interrupt_Controller& ) ;                   //%
	Interrupt_Controller & operator= ( const Interrupt_Controller& ) ;      //%
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
	sc_core::sc_out< bool > nIRQ;                                           //%
	sc_core::sc_out< bool > nFIQ;                                           //%
    sc_core::sc_out< bool > nIRQ_1;                                           //%
	sc_core::sc_out< bool > nFIQ_1;                                           //%
    sc_core::sc_out< bool > nIRQ_2;                                           //%
	sc_core::sc_out< bool > nFIQ_2;                                           //%
    sc_core::sc_out< bool > nIRQ_3;                                           //%
	sc_core::sc_out< bool > nFIQ_3;                                           //%



	sc_core::sc_in< bool > INTR[NUM_PORT];                                  //%
	tlm::tlm_target_socket< 32 > memory;                                    //%
                                                                                //%
protected:                                                                      //%
                                                                                //%
	// Memory declarations                                                  //%
	scml2::memory< unsigned int > REG;                                      //%
	  scml2::reg< unsigned int > REG_INT_MASK;                              //%
	  scml2::reg< unsigned int > REG_INT_TYPE;                              //%
	  scml2::reg< unsigned int > REG_INT_SOURCE;                            //%
	  scml2::reg< unsigned int > REG_DeviceIF_COMPLETE;                     //%
	  scml2::reg< unsigned int > REG_CacheBuffer_COMPLETE;                  //%
	  scml2::reg< unsigned int > REG_FTL_0_COMPLETE;                     //%
	  scml2::reg< unsigned int > REG_FTL_1_COMPLETE;                     //%
	  scml2::reg< unsigned int > REG_FTL_2_COMPLETE;                     //%
	  scml2::reg< unsigned int > REG_FTL_3_COMPLETE;                     //%
	  scml2::reg< unsigned int > REG_FTL_4_COMPLETE;                     //%
	  scml2::reg< unsigned int > REG_FTL_5_COMPLETE;                     //%
	  scml2::reg< unsigned int > REG_FTL_6_COMPLETE;                     //%
	  scml2::reg< unsigned int > REG_FTL_7_COMPLETE;                     //%
	  scml2::reg< unsigned int > REG_FTL_8_COMPLETE;                     //%
	  scml2::reg< unsigned int > REG_FTL_9_COMPLETE;                     //%
                                                                                //%
	scml2::tlm2_gp_target_adapter< 32 > memory_REG_adapter;                 //%
                                                                                //%
public:                                                                         //%
	// Process declarations                                                 //%
	void                                                                    //%
	INTRHandler1();                                                         //%
                                                                                //%
protected:                                                                      //%
	// Memory callback declarations                                         //%
	void                                                                    //%
	input_callback(                                                         //%
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
    ///////  JY ////////////
    unsigned int port_num[NUM_PORT];

    //////// TH ////////////
    bool FTLFlag;
    bool DeviceSchedFlag;

    FILE *DeviceSWTime;
    FILE *DeviceSWDelayTime;
    FILE *CacheSWTime;
    FILE *CacheSWDelayTime;
    FILE *FTLSWTime;
    FILE *FTLSWDelayTime;
    FILE *FTLMAPSWTime;
    FILE *FTLMAPSWDelayTime;


    ////////////////////////////////////////////////////////
    //// modified by rab* 
    ////////////////////////////////////////////////////////
    //

    unsigned int m_iCurId;
    list<sIntSrc> m_intList;
    sc_core::sc_event   m_eNewIrq;
    sc_core::sc_event   e_swEnd;
    sc_core::sc_event   e_init;
    sc_core::sc_event   e_readRegister;
    bool flag_software;

    void sendIrq();
    void systemInit();
    unsigned int scheduleIrq();

    void logSWTime(uint x, uint irqType);
    void logDelayTime(uint src);

    double intSrcTime[20];

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
template < unsigned int NUM_PORT >                                              //%
Interrupt_Controller< NUM_PORT >::Interrupt_Controller( const sc_core::sc_module_name & n)//%
		: sc_core::sc_module(n)                                         //%
,                                                                               //%
 nIRQ("nIRQ"),                                                                  //%
 nFIQ("nFIQ"),                                                                  //%
 nIRQ_1("nIRQ_1"),                                                         //%
 nFIQ_1("nFIQ_1"),                                                         //%
 nIRQ_2("nIRQ_2"),                                                         //%
 nFIQ_2("nFIQ_2"),                                                         //%
 nIRQ_3("nIRQ_3"),                                                         //%
 nFIQ_3("nFIQ_3"),                                                         //%


 memory("memory"),                                                              //%
 memory_REG_adapter("memory_REG_adapter", memory),                              //%
 REG("REG", 15ULL),                                                              //%
  REG_INT_MASK("REG_INT_MASK", REG, 0LL ),                                      //%
  REG_INT_TYPE("REG_INT_TYPE", REG, 1LL ),                                      //%
  REG_INT_SOURCE("REG_INT_SOURCE", REG, 2LL ),                                  //%
  REG_DeviceIF_COMPLETE("REG_DeviceIF_COMPLETE", REG, 3LL),
  REG_CacheBuffer_COMPLETE("REG_CacheBuffer_COMPLETE", REG, 4LL),
  REG_FTL_0_COMPLETE("REG_FTL_0_COMPLETE", REG, 5LL),
  REG_FTL_1_COMPLETE("REG_FTL_1_COMPLETE", REG, 6LL),
  REG_FTL_2_COMPLETE("REG_FTL_2_COMPLETE", REG, 7LL),
  REG_FTL_3_COMPLETE("REG_FTL_3_COMPLETE", REG, 8LL),
  REG_FTL_4_COMPLETE("REG_FTL_4_COMPLETE", REG, 9LL),
  REG_FTL_5_COMPLETE("REG_FTL_5_COMPLETE", REG, 10LL),
  REG_FTL_6_COMPLETE("REG_FTL_6_COMPLETE", REG, 11LL),
  REG_FTL_7_COMPLETE("REG_FTL_7_COMPLETE", REG, 12LL),
  REG_FTL_8_COMPLETE("REG_FTL_8_COMPLETE", REG, 13LL),
  REG_FTL_9_COMPLETE("REG_FTL_9_COMPLETE", REG, 14LL)

                                                                                //%
                                                                                //%USERBEGIN INITIALIZER
    // TODO: Add your class initialization here.
                                                                                //%USEREND INITIALIZER
                                                                                //%
{                                                                               //%
 // declare processes and their sensitivities                                   //%
	SC_THREAD(INTRHandler1);                                               //%
	 for (unsigned int i = 0; i < NUM_PORT; i++) {                          //%
		sensitive << INTR[i].value_changed() ;                              //%
	 }                                                                      //%
	 dont_initialize();                                                     //%

	SC_THREAD(sendIrq);                                               //%
     sensitive << m_eNewIrq;
	 dont_initialize();                                                     //%
 
    SC_THREAD(systemInit);                                               //%
     sensitive << e_init;
	 dont_initialize();                                                     //%

                                                                            //%
	// bind target ports to memories                                        //%
	memory_REG_adapter(REG);                                                //%
                                                                                //%
 // setup memory callbacks                                                      //%
	// - Memory: REG                                                        //%
	scml2::set_callback(                                                    //%
		REG,                                                            //%
		SCML2_CALLBACK(input_callback),                                 //%
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

    if(IRQ_DEBUG) cout << "interrupt constructor" << endl;

                                                                                //%USEREND CONSTRUCTOR
                                                                                //%
}                                                                               //%
// destructor                                                                   //%
                                                                                //%
template < unsigned int NUM_PORT >                                              //%
Interrupt_Controller< NUM_PORT >::~Interrupt_Controller()                       //%
{                                                                               //%
                                                                                //%USERBEGIN DESTRUCTOR

    // TODO: Add destructor code here.
    
    fclose(DeviceSWTime);
    fclose(DeviceSWDelayTime);
    fclose(CacheSWTime);
    fclose(CacheSWDelayTime);
    fclose(FTLSWTime);
    fclose(FTLSWDelayTime);
    fclose(FTLMAPSWTime);
    fclose(FTLMAPSWDelayTime);
                                                                                //%USEREND DESTRUCTOR
}                                                                               //%
// initialization method for ports                                              //%
                                                                                //%
template < unsigned int NUM_PORT >                                              //%
void                                                                            //%
Interrupt_Controller< NUM_PORT >::initPorts()                                   //%
{ 
//////// JY///////////
//	nIRQ.initialize(0x0);                                                   //%
//	nFIQ.initialize(0x0);                                                   //%

    nIRQ.initialize(0x1);
    nFIQ.initialize(0x1);
    nIRQ_1.initialize(0x1);
    nFIQ_1.initialize(0x1);
    nIRQ_2.initialize(0x1);
    nFIQ_2.initialize(0x1);
    nIRQ_3.initialize(0x1);
    nFIQ_3.initialize(0x1);

    flag_software = false;
    test = true;

}                                                                               //%
// initialization method for registers                                          //%
                                                                                //%
template < unsigned int NUM_PORT >                                              //%
void                                                                            //%
Interrupt_Controller< NUM_PORT >::initRegisters()                               //%
{                                                                               //%
    REG.initialize(0x0);                                                        //%
    REG_INT_MASK.initialize(0x0);                                               //%
    REG_INT_TYPE.initialize(0x0);                                               //%
    REG_INT_SOURCE.initialize(0x0);                                             //%
    REG_DeviceIF_COMPLETE.initialize(0x0);                                   //%
    REG_CacheBuffer_COMPLETE.initialize(0x0);                                   //%
    REG_FTL_0_COMPLETE.initialize(0x0); 
    REG_FTL_1_COMPLETE.initialize(0x0); 
    REG_FTL_2_COMPLETE.initialize(0x0); 
    REG_FTL_3_COMPLETE.initialize(0x0); 
    REG_FTL_4_COMPLETE.initialize(0x0); 
    REG_FTL_5_COMPLETE.initialize(0x0); 
    REG_FTL_6_COMPLETE.initialize(0x0); 
    REG_FTL_7_COMPLETE.initialize(0x0); 
    REG_FTL_8_COMPLETE.initialize(0x0); 
    REG_FTL_9_COMPLETE.initialize(0x0); 

}                                                                               //%
//This method is invoked after the design is fully elaborated, but              //%
// before simulation begins.                                                    //%
                                                                                //%
template < unsigned int NUM_PORT >                                              //%
void                                                                            //%
Interrupt_Controller< NUM_PORT >::end_of_elaboration()                          //%
{                                                                               //%
                                                                                //%USERBEGIN END_OF_ELAB
    // TODO: Add end_of_elaboration code here.
    this->initRegisters();
    this->initPorts();

    FTLFlag = false;
    DeviceSchedFlag = false;
    DeviceSWTime = fopen("deviceswtime.txt","w");
    DeviceSWDelayTime = fopen("deviceswdelaytime.txt","w");
    CacheSWTime = fopen("cacheswtime.txt","w");
    CacheSWDelayTime = fopen("cacheswdelaytime.txt","w");
    FTLSWTime = fopen("ftlswtime.txt","w");
    FTLSWDelayTime = fopen("ftlswdelaytime.txt","w");
    FTLMAPSWTime = fopen("ftlmapswtime.txt","w");
    FTLMAPSWDelayTime = fopen("ftlmapswdelaytime.txt","w");

    for(uint i=0;i <NUM_OF_CORE;i++)
        CPUState[i] = Empty;

    TaskPriority[0] = CacheBuffer;
    TaskPriority[1] = FTL;
    TaskPriority[2] = DeviceIF;
    
                                                                               //%USEREND END_OF_ELAB
}                                                                               //%
// command processing method                                                    //%
//  During simulation, this command may be called to handle    /                 //%
//  interactive commands -- for dynamic instrumentation of the instance.        //%
                                                                                //%
template < unsigned int NUM_PORT >                                              //%
std::string                                                                     //%
Interrupt_Controller< NUM_PORT >::commandProcessor(const std::vector< std::string >& cmd)//%
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
template < unsigned int NUM_PORT >                                              //%
                                                                                //%
void                                                                            //%
Interrupt_Controller< NUM_PORT >::initMessageLogging()                          //%
{                                                                               //%
                                                                                //%USERBEGIN MODEL_DEBUG_INIT
    // toggle on/off debugging flags
    for (int i=0;i<sizeof(DbgCategories);i++) {
#ifdef DBG_SCP_INTERRUPT_CONTROLLER
        dbgFlag[i] = true;
#else
        dbgFlag[i] = false;
#endif
    }
                                                                                //%USEREND MODEL_DEBUG_INIT
                                                                                //%
}                                                                               //%
// method for logging messages                                                  //%
template < unsigned int NUM_PORT >                                              //%
                                                                                //%
void                                                                            //%
Interrupt_Controller< NUM_PORT >::REPORT_MSG(std::string sev, std::string msg)  //%
{                                                                               //%
                                                                                //%USERBEGIN REPORT_MSG
    std::cout << sev << ": " << msg << " [" << this->basename() << "@" << sc_core::sc_time_stamp() << "]" << std::endl;
                                                                                //%USEREND REPORT_MSG
                                                                                //%
}                                                                               //%
                                                                                //%
template < unsigned int NUM_PORT >                                              //%
void                                                                            //%
Interrupt_Controller< NUM_PORT >::INTRHandler1()                                //%
                                                                                //%USERBEGIN INTRHandler1
{
    if ( dbgFlag[PORTS] ) DBG_MSG( "INTRHandler1 invoked because of event on port INTR" );

	// TODO: INSERT code for the INTRHandler1 method here. 

    //////////////////////////////////////////////////////////////////////
    ///////////////// modified by rab* 141110 //////////
    //////////////////////////////////////////////////////////////////////

    bool bOldPinVal[NUM_PORT];
    bool bNewPinVal[NUM_PORT];
    bool newIrqExist;



    while(1)
    {
        //////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////
        ///////////////// modified by rab* 141110 //////////
        ///////////////// START ////////////////////////////
        ///////////////////////////////////////////////////////////////////


        if(dbgFlag[GENERAL])  cout << "New irq" << endl;
       
        if(IRQ_DEBUG) cout << DEV_AND_TIME << "[INTRHandler1] New IRQ" << endl; 
          
        newIrqExist = false;

        // sc_zero_time for simultanous irqs 
        for(unsigned int i=0; i < (NUM_PORT) ; i++)
            wait(SC_ZERO_TIME);

        for(unsigned int i=0; i < (NUM_PORT) ; i++)
            bNewPinVal[i] = (INTR[i].read() == 0) ? false : true;

        for(unsigned int i=0; i < (NUM_PORT) ; i++){
            if(bNewPinVal[i] != bOldPinVal[i] &&
                    bNewPinVal[i] == true)
            {
                // new interrupt arrived
                sIntSrc newInt;
                newInt.iId = m_iCurId++;
                newInt.dIssuedTime = sc_core::sc_time_stamp().to_double()/1000;
                newInt.iSrc = i;
                m_intList.push_back(newInt);

                newIrqExist = true;

                intSrcTime[i] = sc_time_stamp().value()/1000;
            }
        }

        memcpy(bOldPinVal, bNewPinVal, sizeof(bool) * NUM_PORT);

        if(m_intList.size() > 0 && newIrqExist){
            m_eNewIrq.notify();
            wait(SC_ZERO_TIME);
        }

        wait();

        /////////////////////////////////////////////////////
        ///////////////// modified by rab* 141110 //////////
        ///////////////// END ////////////////////////////
    }
}
                                                                                //%USEREND INTRHandler1
                                                                                //%
template < unsigned int NUM_PORT >                                              //%
void                                                                            //%
Interrupt_Controller< NUM_PORT >::input_callback(                               //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)           //%
{                                                                               //%
    const char* dbgMsgText = "TRANSPORT handler for memory REG";                //%
    scml2::memory< unsigned int >& m = this->REG;                               //%
    tlm::tlm_command cmd = trans.get_command();                                 //%
    sc_dt::uint64 adr = trans.get_address();                                    //%
    unsigned char* ptr = trans.get_data_ptr();                                  //%
    unsigned int len = trans.get_data_length();                                 //%
    unsigned char* byt = trans.get_byte_enable_ptr();                           //%
    unsigned int wid = trans.get_streaming_width();                             //%
                                                                                //%USERBEGIN input_callback
    if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );
    // TODO: INSERT code for the input_callback method here.

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

            if(adr== DEVICEIF_ADDR || adr == NAND_COMPLETE){ 
            nIRQ.write(1);
            //wait(1, SC_NS);
            e_swEnd.notify();
            flag_software = false;
            }
        else {
            cout << "undefined adr, adr : " << adr << endl;
            assert(0);
        }
    } else if (cmd == tlm::TLM_READ_COMMAND) {
        // copy from your target's mmememory to 'ptr', e.g.: memcpy(ptr, &mem[adr], num_bytes);

        if(adr == 0x8){
            //cout << sc_time_stamp().value() <<"   : cpu read register time  source = "<< REG_INT_SOURCE <<endl;
            *(unsigned int*)ptr = REG_INT_SOURCE;
            e_readRegister.notify();
            flag_software = true;
        }else{
            if(IRQ_DEBUG) cout << "Interrupt controller read call back: adr = " << adr << endl;
            sc_stop();
        }

    }
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
    
   if(dbgFlag[GENERAL]) cout << "call_back_completed" << endl;
                                                                                //%USEREND input_callback
}                                                                               //%
                                                                                //%
                                                                                //%USERBEGIN MEMBER_DEFNS
// TODO: Insert definitions of your member functions, etc. here.
template < unsigned int NUM_PORT >                                              //%
void                                                                            //%
Interrupt_Controller< NUM_PORT >::systemInit()                                //%
{

    while(1){
        wait(1000000, SC_NS);
        REG_INT_SOURCE = INIT;
        nIRQ.write(0);
        wait(SC_ZERO_TIME);
        nIRQ.write(1);
        wait();
    }
}
//
/////////////////////////////////////////////////////////////////////////////////
///////////////////// START: modified by rab*
/////////////////////////////////////////////////////////////////////////////////

template < unsigned int NUM_PORT >                                              //%
void                                                                            //%
Interrupt_Controller< NUM_PORT >::sendIrq()                                //%
{

    sIntSrc curIrq;
    unsigned int numCore;

    while(1){

        if(IRQ_DEBUG) cout << DEV_AND_TIME << "[sendIRQ] Thread notified" << endl; 
        while(m_intList.size() > 0 ){

            if(IRQ_DEBUG) cout << DEV_AND_TIME << "[sendIRQ] Sending IRQ...." << endl; 
            curIrq = m_intList.front();
            if(flag_software) wait(e_swEnd);
            
            assert((curIrq.iSrc == 0) || (curIrq.iSrc == 1));

            nIRQ.write(0);
            REG_INT_SOURCE = curIrq.iSrc; 
            
            wait(e_readRegister);
            m_intList.pop_front();
        }
        wait();
    }
}
template < unsigned int NUM_PORT >                                              //%
unsigned int                                                                     //%
Interrupt_Controller< NUM_PORT >::scheduleIrq(){

    ////// re-order the interrupts
    ////// set the target ID
    uint i, j;
    uint count =0;
    std::list<sIntSrc>::iterator intIter;
    sIntSrc curIrq;
    unsigned int src;
    unsigned int numCore;
    bool findTask = false;

    for(i=0;i<NUM_OF_CORE;i++){
        if(CPUState[i] == Empty){
            for(uint k=0; k < NUM_OF_TASK; k++){
                if(findTask == false){
                    for(intIter = m_intList.begin() ; intIter != m_intList.end() ; intIter++){
                        if(TaskPriority[k] == FTL){
                            if(DeviceIF != intIter->iSrc && CacheBuffer != intIter->iSrc){
                                findTask = true;
                                numCore = i;
                                src = intIter->iSrc;
                                CPUState[i] = src;
                                REG_INT_SOURCE = src;

                                for(j=0;j<NUM_OF_CORE;j++){
                                    if(CPUState[j] == src)
                                        count++;
                                }
                                if(count >=2){
                                    CPUState[i] = Empty;
                                    numCore = NUM_OF_CORE;
                                    findTask = false;
                                    count = 0;
                                }else{
                                    m_intList.erase(intIter);
                                    break;
                                }
                            }
                        }else{
                            if(TaskPriority[k] == intIter->iSrc){
                                findTask = true;
                                numCore = i;
                                src = intIter->iSrc;
                                CPUState[i] = src;
                                REG_INT_SOURCE = src;

                                for(j=0;j<NUM_OF_CORE;j++){
                                    if(CPUState[j] == src)
                                        count++;
                                }
                                if(count >=2){
                                    CPUState[i] = Empty;
                                    numCore = NUM_OF_CORE;
                                    findTask = false;
                                    count = 0;
                                }else{
                                    m_intList.erase(intIter);
                                    break;
                                }
                            }
                        }
                    }
                }else{
                    break;
                }
            }
        }
        if(findTask == true)
            break;
    }

    if(findTask == false)
        numCore = NUM_OF_CORE;
  
    //cout <<"Interrupt ctrl selected core = " << numCore << endl;
    return numCore;
}

template < unsigned int NUM_PORT >                                              //%
void
Interrupt_Controller< NUM_PORT >::logDelayTime(uint src){

    switch(src){
        case 0:     //device
            fprintf(DeviceSWDelayTime, "%lf\t%lf\n", (double)sc_time_stamp().value()/1000, (double)sc_time_stamp().value()/1000 - intSrcTime[src]);
            break;

        case 1:
            break;

        case 2:  //ftl
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            fprintf(FTLMAPSWDelayTime, "%lf\t%lf\n", (double)sc_time_stamp().value()/1000, (double)sc_time_stamp().value()/1000 - intSrcTime[src]);
            break;

        case 3: 
            fprintf(FTLSWDelayTime, "%lf\t%lf\n", (double)sc_time_stamp().value()/1000, (double)sc_time_stamp().value()/1000 - intSrcTime[src]);

            break;
        case 4:     //cache
            fprintf(CacheSWDelayTime, "%lf\t%lf\n", (double)sc_time_stamp().value()/1000, (double)sc_time_stamp().value()/1000 - intSrcTime[src]);

            break;
        default:
            break;
    }

    //curIrq.dIssuedTime = (sc_time_stamp().value()/1000);
}

template < unsigned int NUM_PORT >                                              //%
void
Interrupt_Controller< NUM_PORT >::logSWTime(uint x, uint irqType){
    switch(irqType){
        case 0:     //device
            fprintf(DeviceSWTime, "%u\t%lf\t%lf\n", x,(double)sc_time_stamp().value()/1000, (double)sc_time_stamp().value()/1000 - cpu_time[x]);
            break;

        case 1:
            break;

        case 2:  //ftl
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            fprintf(FTLSWTime, "%u\t%lf\t%lf\n", x, (double)sc_time_stamp().value()/1000, (double)sc_time_stamp().value()/1000 - cpu_time[x]);
            break;
        case 3: 
            fprintf(FTLMAPSWTime, "%u\t%lf\t%lf\n", x, (double)sc_time_stamp().value()/1000, (double)sc_time_stamp().value()/1000 - cpu_time[x]);
            break;
        case 4:     //cache
            fprintf(CacheSWTime, "%u\t%lf\t%lf\n", x, (double)sc_time_stamp().value()/1000, (double)sc_time_stamp().value()/1000 - cpu_time[x]);

            break;
        default:
            break;
    }

}
/////////////////////////////////////////////////////////////////////////////////
///////////////////// END: modified by rab*
/////////////////////////////////////////////////////////////////////////////////
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
//% begin section previously tagged: FOOTER_H

//% end section previously tagged: FOOTER_H
//% begin section previously tagged: FOOTER_H

//% end section previously tagged: FOOTER_H
                                                                                //%USEREND ORPHANED_CODE
#endif // ORPHANED_CODE                                                         //%
                                                                                //%USERBEGIN FOOTER_H

                                                                                //%USEREND FOOTER_H
#endif // __INTERRUPT_CONTROLLER_H__                                            //%
