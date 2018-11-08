//--------------------------------------------------------------                //%
// Important:  The template for this file was generated                         //%
//  automatically and can only be re-generated if rules                         //%
//  are followed when editing:                                                  //%
// 1. Do not modify any line containing //%                                     //%
// 2. Only insert your lines of code between 2 lines                            //%
//    containing "//% user begin" and "//% user end"                            //%
//                                                                              //%
// :generate:date: Wed Aug 03 2011 22:10:47                                     //%
// :generate:version: 2010.1.0-dev                                              //%
//--------------------------------------------------------------                //%
                                                                                //%
#ifndef __NAND_CTRL_H__                                                         //%
#define __NAND_CTRL_H__                                                         //%
                                                                                //%USERBEGIN MODEL_DEBUG_ENABLE
#if defined(DEBUG) || defined(DBG_SCP)
# define DBG_SCP_NAND_CTRL
#endif

                                                                                //%USEREND MODEL_DEBUG_ENABLE
                                                                                //%
#include <scml2.h>                                                              //%
#include <systemc>                                                              //%
#include "scmlinc/scml_command_processor.h"                                     //%
                                                                                //%USERBEGIN HEADER_H
#include "header/NAND_Ctrl_Spec.h"
                                                                                //%USEREND HEADER_H
#define CBR_QUEUE_SIZE 1000



//------------------------                                                      //%
// Module Interface                                                             //%
//------------------------                                                      //%
                                                                                //%
template < unsigned int NUM_WAY=1 >                                             //%
class NAND_CTRL :                                                               //%
	public sc_core::sc_module                                               //%
                                                                                //%USERBEGIN INHERITANCE
    // TODO: Specify class inheritance here. Start each line with a , character
                                                                                //%USEREND INHERITANCE
                                                                                //%
{                                                                               //%
public:                                                                         //%
	// constructor                                                          //%
	NAND_CTRL( const sc_core::sc_module_name & n ) ;                        //%
	SC_HAS_PROCESS( NAND_CTRL );                                            //%
	// destructor                                                           //%
	virtual ~NAND_CTRL();                                                   //%
private:                                                                        //%
	// disable default and copy constructors by making them private         //%
	NAND_CTRL() ;                                                           //%
	NAND_CTRL( const NAND_CTRL& ) ;                                         //%
	NAND_CTRL & operator= ( const NAND_CTRL& ) ;                            //%
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
	sc_core::sc_in< bool > clk_p;                                           //%
	sc_core::sc_in< bool > NAND_RnB[NUM_WAY];                                     //%
	scml2::initiator_socket< 32 > NAND_TRANS;                               //%
	tlm::tlm_target_socket< 32 > Sw_TRANS;                                  //%
	sc_core::sc_out< unsigned int > Sw_RnB;                                 //%
                                                                                //%
protected:                                                                      //%
                                                                                //%
	// Memory declarations                                                  //%
	scml2::memory< unsigned int > Sw_SFR;                                   //%
	  scml2::memory_alias< unsigned int > Sw_SFR_CMD;                       //%
	    scml2::reg< unsigned int > Sw_SFR_CMD_OPCODE;                       //%
	    scml2::reg< unsigned int > Sw_SFR_CMD_ADDR1;                        //%
	    scml2::reg< unsigned int > Sw_SFR_CMD_ADDR2;                        //%
	    scml2::reg< unsigned int > Sw_SFR_CMD_ADDR3;                        //%
	    scml2::reg< unsigned int > Sw_SFR_CMD_ADDR4;                        //%
	  scml2::memory_alias< unsigned int > Sw_SFR_DATA;                      //%
                                                                                //%
	scml2::tlm2_gp_target_adapter< 32 > Sw_TRANS_Sw_SFR_adapter;            //%
                                                                                //%
public:                                                                         //%
	// Process declarations                                                 //%
	void                                                                    //%
	rst_nHandler1();                                                        //%
	void                                                                    //%
	NAND_RnBHandler1();                                                     //%
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


    // MEMBER VARIABLE  //
    unsigned int selectedWay;
    bool RnB_Hold[NUM_WAY];
    NAND_Cmd CMD_Buffer[NUM_WAY];
    unsigned int iaBuffer[NUM_WAY][PAGE_SIZE/4];
    sc_event newCommand[NUM_WAY];
    sc_semaphore m_CommandSemaphore;
    unsigned int Reg_RnB[1];
   
    unsigned int iRNM_Buffer[NUM_WAY][PAGE_SIZE/4]; //for copyback read

    // MEMBER FUNCTION //
    inline int getWayAddr(int addr){
        if(dbgFlag[GENERAL])
            cout << DEV_AND_TIME << "#### ADDRESS: " << addr << endl;
        return (addr & MASK_WAY) >> BIT_FOR_WAY & (NUM_OF_WAY-1);
    };

    inline int DataTransportTime(int iLength){
        unsigned int iDataSize = iLength * 8;
        unsigned int iClockCycle = (int)(((double)iDataSize / BITWIDTH_NAND_CTRL) + 0.5);
        
        return iClockCycle * NAND_CTRL_CLK_PERIOD;
    };


    inline int getRnB(int way){
        unsigned int temp = Reg_RnB[0];
        return (temp >> way) & 0x01;
    }

    inline void setRnB(int way, int value){
        if(value==0)
            Reg_RnB[0] &= ~(1 << way);
        else
            Reg_RnB[0] |= (1 << way);
        if(dbgFlag[REGS]){
            cout << DEV_AND_TIME ;
            for(int i=0; i<NUM_WAY; i++)
                cout << "RnB[" << i << "] = " << getRnB(i) << "     ";
            cout << endl;
        }
        if(dbgFlag[GENERAL])
            cout << DEV_AND_TIME << "Set RnB - " << "#### WAY: " << way << ", RnB: " << value << endl;

        Sw_RnB.write(Reg_RnB[0]);

    }

    inline void holdRnB(int way, bool value){
        RnB_Hold[way] = value;

        if(dbgFlag[GENERAL])
            cout << DEV_AND_TIME << "Hold RnB - " << "#### WAY: " << way << ", RnB: " << value << endl;
    }

    inline void CommandWrite(NAND_Cmd iCMD){
        int COMMAND[5];
        COMMAND[0] = iCMD.opCode;
        COMMAND[1] = iCMD.iAddr1;
        COMMAND[2] = iCMD.iAddr2;
        COMMAND[3] = iCMD.iAddr3;
        COMMAND[4] = iCMD.iAddr4;
        NAND_TRANS.write(ADR_NAND_CMD,COMMAND,5);
    }

    inline void DataWrite(int way){
        NAND_TRANS.write(ADR_NAND_DATA,iaBuffer[way],PAGE_SIZE/4);
    }


    inline void DataRead(int way){
        int COMMAND[5];
        COMMAND[0] = Chip_Select;
        COMMAND[1] = way << BIT_FOR_WAY;
        //cout<<DEV_AND_TIME<< COMMAND[0]<<"\t" << COMMAND[1] <<"\t" << COMMAND[2] <<"\t" << COMMAND[3] <<"\t" << COMMAND[4] << endl;
        NAND_TRANS.write(ADR_NAND_CMD,COMMAND,5);
        
        NAND_TRANS.read(ADR_NAND_DATA,iaBuffer[way],PAGE_SIZE/4);
    }

    inline void RNM_DataRead(int way){
        int COMMAND[5];
        COMMAND[0] = Chip_Select;
        COMMAND[1] = way << BIT_FOR_WAY;
        //cout<<DEV_AND_TIME<< COMMAND[0]<<"\t" << COMMAND[1] <<"\t" << COMMAND[2] <<"\t" << COMMAND[3] <<"\t" << COMMAND[4] << endl;
        NAND_TRANS.write(ADR_NAND_CMD,COMMAND,5);
        
        NAND_TRANS.read(ADR_NAND_DATA, iRNM_Buffer[way],PAGE_SIZE/4);
    }
    
    inline void RNM_Merge(int way, int len){ 

        if(NANDCtrl_DEBUG){


            cout << DEV_AND_TIME << "[RNM_Merge] CR length : " << len << endl;
            cout << DEV_AND_TIME << "[RNM_Merge] RnM Before Merge... data : ";
            for(int i=0; i<32; i++)
                cout<< iaBuffer[way][i*SECTOR_SIZE_BYTE/4] << " ";
            cout<<endl;


            cout << DEV_AND_TIME << "[RnM_Merge] RnM Before Merge... RNM : ";
            for(int i=0; i<32; i++)
                cout<< iRNM_Buffer[way][i*SECTOR_SIZE_BYTE/4] << " ";
            cout<<endl;
        }

        
        
        
        
        for(int i=0; i<DATA_PAGE_SIZE/4; i++){

            if(i>=SECTOR_SIZE_BYTE/4*len){ //assuming all addresses are aligned to a page....

                iaBuffer[way][i] = iRNM_Buffer[way][i];
            }

        }

        if(NANDCtrl_DEBUG){
            cout << DEV_AND_TIME << "[RNM_Merge] RnM Merge completed... data : ";
            for(int i=0; i<32; i++)
                cout<< iaBuffer[way][i*SECTOR_SIZE_BYTE/4] << " ";
            cout<<endl;
        }




    }


    void Command_Handler();

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
NAND_CTRL< NUM_WAY >::NAND_CTRL( const sc_core::sc_module_name & n)             //%
		: sc_core::sc_module(n)                                         //%
,                                                                               //%
 rst_n("rst_n"),                                                                //%
 clk_p("clk_p"),                                                                //%
 NAND_TRANS("NAND_TRANS"),                                                      //%
 Sw_TRANS("Sw_TRANS"),                                                          //%
 Sw_RnB("Sw_RnB"),                                                              //%
 Sw_TRANS_Sw_SFR_adapter("Sw_TRANS_Sw_SFR_adapter", Sw_TRANS),                  //%
 //Sw_SFR("Sw_SFR", 2085ULL),                                                     //%
 Sw_SFR("Sw_SFR", 4165ULL),                                                     //%
  Sw_SFR_CMD("Sw_SFR_CMD", Sw_SFR, 0LL, 5LL ),                                  //%
   Sw_SFR_CMD_OPCODE("Sw_SFR_CMD_OPCODE", Sw_SFR_CMD, 0LL ),                    //%
   Sw_SFR_CMD_ADDR1("Sw_SFR_CMD_ADDR1", Sw_SFR_CMD, 1LL ),                      //%
   Sw_SFR_CMD_ADDR2("Sw_SFR_CMD_ADDR2", Sw_SFR_CMD, 2LL ),                      //%
   Sw_SFR_CMD_ADDR3("Sw_SFR_CMD_ADDR3", Sw_SFR_CMD, 3LL ),                      //%
   Sw_SFR_CMD_ADDR4("Sw_SFR_CMD_ADDR4", Sw_SFR_CMD, 4LL ),                      //%
  //Sw_SFR_DATA("Sw_SFR_DATA", Sw_SFR, 5LL, 2080LL )                              //%
  Sw_SFR_DATA("Sw_SFR_DATA", Sw_SFR, 5LL, 4160LL )                              //%
  ,m_CommandSemaphore(1)
                                                                                //%
                                                                                //%USERBEGIN INITIALIZER
                                                                                //%USEREND INITIALIZER
                                                                                //%
{                                                                               //%
 // declare processes and their sensitivities                                   //%
   // cout << "Nand_ctrl constructor" << endl;
   // cout << "NUM_WAY is " << NUM_WAY << endl;
	SC_THREAD(rst_nHandler1);                                               //%
	 sensitive << rst_n.neg() ;                                             //%
	 dont_initialize();                                                     //%
	SC_THREAD(NAND_RnBHandler1);                                            //%
	 for (unsigned int i = 0; i < NUM_WAY; i++) {                                 //%
		sensitive << NAND_RnB[i].value_changed() ;                      //%
	 }                                                                      //%
	 dont_initialize();                                                     //%
	
    for(unsigned int i=0; i<NUM_WAY; i++){
        SC_THREAD(Command_Handler);
         sensitive << newCommand[i];
         dont_initialize();
    }
	// bind target ports to memories                                        //%
	Sw_TRANS_Sw_SFR_adapter(Sw_SFR);                                        //%
                                                                                //%
 // setup memory callbacks                                                      //%
	// - Memory: Sw_SFR                                                     //%
	scml2::set_callback(                                                    //%
		Sw_SFR,                                                     //%
        SCML2_CALLBACK(CMD_Callback),
//		this,                                                           //%
//		&SC_CURRENT_USER_MODULE::CMD_Callback,                          //%
		scml2::AUTO_SYNCING);                                           //%
	scml2::set_callback(                                                    //%
		Sw_SFR_DATA,                                                    //%
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
    if ( dbgFlag[GENERAL] ) DBG_MSG("Created " + (std::string)this->basename());

   // cout << "Nand_ctrl constructor" << endl;

                                                                                //%USEREND CONSTRUCTOR
                                                                                //%
}                                                                               //%
// destructor                                                                   //%
                                                                                //%
template < unsigned int NUM_WAY >                                               //%
NAND_CTRL< NUM_WAY >::~NAND_CTRL()                                              //%
{                                                                               //%
                                                                                //%USERBEGIN DESTRUCTOR

    // TODO: Add destructor code here.

                                                                                //%USEREND DESTRUCTOR
}                                                                               //%
// initialization method for ports                                              //%
                                                                                //%
template < unsigned int NUM_WAY >                                               //%
void                                                                            //%
NAND_CTRL< NUM_WAY >::initPorts()                                               //%
{                                                                               //%
	Sw_RnB.initialize(0x0);                                                 //%
}                                                                               //%
// initialization method for registers                                          //%
                                                                                //%
template < unsigned int NUM_WAY >                                               //%
void                                                                            //%
NAND_CTRL< NUM_WAY >::initRegisters()                                           //%
{                                                                               //%
    Sw_SFR.initialize(0x0);                                                     //%
    Sw_SFR_CMD.initialize(0x0);                                                 //%
    Sw_SFR_CMD_OPCODE.initialize(0x0);                                          //%
    Sw_SFR_CMD_ADDR1.initialize(0x0);                                           //%
    Sw_SFR_CMD_ADDR2.initialize(0x0);                                           //%
    Sw_SFR_CMD_ADDR3.initialize(0x0);                                           //%
    Sw_SFR_CMD_ADDR4.initialize(0x0);                                           //%
    Sw_SFR_DATA.initialize(0x0);                                                //%
}                                                                               //%
//This method is invoked after the design is fully elaborated, but              //%
// before simulation begins.                                                    //%
                                                                                //%
template < unsigned int NUM_WAY >                                               //%
void                                                                            //%
NAND_CTRL< NUM_WAY >::end_of_elaboration()                                      //%
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
NAND_CTRL< NUM_WAY >::commandProcessor(const std::vector< std::string >& cmd)   //%
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
NAND_CTRL< NUM_WAY >::initMessageLogging()                                      //%
{                                                                               //%
                                                                                //%USERBEGIN MODEL_DEBUG_INIT
    // toggle on/off debugging flags
    for (int i=0;i<sizeof(DbgCategories);i++) {
#ifdef DBG_SCP_NAND_CTRL
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
NAND_CTRL< NUM_WAY >::REPORT_MSG(std::string sev, std::string msg)              //%
{                                                                               //%
                                                                                //%USERBEGIN REPORT_MSG
    std::cout << DEV_AND_TIME << sev << ": " << msg <<  std::endl;
                                                                                //%USEREND REPORT_MSG
                                                                                //%
}                                                                               //%
                                                                                //%
template < unsigned int NUM_WAY >                                               //%
void                                                                            //%
NAND_CTRL< NUM_WAY >::rst_nHandler1()                                           //%
                                                                                //%USERBEGIN rst_nHandler1
{
	if ( dbgFlag[PORTS] ) DBG_MSG( "rst_nHandler1 invoked because of event on port rst_n" );

    while(1){
       
        Reg_RnB[0] = 0;
        for(int i=0;i<NUM_WAY;i++){
            setRnB(i,0);
            holdRnB(i,0);
        }

        for(int i=0;i<NUM_WAY;i++){
            selectedWay = i;
            newCommand[i].notify();
            wait(SC_ZERO_TIME);
        }
        wait();
    }

}
                                                                                //%USEREND rst_nHandler1
                                                                                //%
template < unsigned int NUM_WAY >                                               //%
void                                                                            //%
NAND_CTRL< NUM_WAY >::NAND_RnBHandler1()                                        //%
                                                                                //%USERBEGIN NAND_RnBHandler1
{
	if ( dbgFlag[PORTS] ) DBG_MSG( "NAND_RnBHandler1 invoked because of event on port NAND_RnB" );

    while(1){
        for(int i=0;i<NUM_WAY;i++){
            if((NAND_RnB[i] != getRnB(i)) && (RnB_Hold[i]==false))
                setRnB(i,NAND_RnB[i]);
        }
        wait();
    }
}
                                                                                //%USEREND NAND_RnBHandler1
                                                                                //%
template < unsigned int NUM_WAY >                                               //%
void                                                                            //%
NAND_CTRL< NUM_WAY >::CMD_Callback(                                             //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)           //%
{                                                                               //%
    const char* dbgMsgText = "TRANSPORT handler for memory Sw_SFR_CMD";         //%
    scml2::memory_alias< unsigned int >& m = this->Sw_SFR_CMD;                  //%
    tlm::tlm_command cmd = trans.get_command();                                 //%
    sc_dt::uint64 adr = trans.get_address();                                    //%
    unsigned char* ptr = trans.get_data_ptr();                                  //%
    unsigned int len = trans.get_data_length();                                 //%
    unsigned char* byt = trans.get_byte_enable_ptr();                           //%
    unsigned int wid = trans.get_streaming_width();                             //%
    //%USERBEGIN CMD_Callback
    //
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

        selectedWay = getWayAddr(Sw_SFR_CMD_ADDR1);
       // cout<<DEV_AND_TIME << "selectedWay : "<<selectedWay<<endl;

        //cout << name() << "  WAY : " << selectedWay <<endl;
        if(dbgFlag[GENERAL])
        {
            cout << DEV_AND_TIME << "#### SFR: " << dec << Sw_SFR_CMD_OPCODE << "  " << Sw_SFR_CMD_ADDR1 << endl;
            cout << DEV_AND_TIME << "#### WAY : " << dec << selectedWay << "###### RnB : " << NAND_RnB[selectedWay] << endl;
        }

        if(dbgFlag[GENERAL])
            cout << DEV_AND_TIME << "Command Transport: OPCODE is " << Sw_SFR_CMD_OPCODE << ", Selected way is " << selectedWay << endl;


        if(NAND_RnB[selectedWay]==0){ // if RnB on selected Way is busy, alerts error message and stops simulation
            cout << DEV_AND_TIME << "ERROR: Transporting while RnB is busy is prohibited!!" << endl;
            //sc_stop();
        }


        if(Sw_SFR_CMD_OPCODE==Chip_Select);
        else{

            CMD_Buffer[selectedWay].opCode = NAND_CMD_SET((unsigned int)Sw_SFR_CMD_OPCODE);
            CMD_Buffer[selectedWay].iAddr1 = Sw_SFR_CMD_ADDR1;
            CMD_Buffer[selectedWay].iAddr2 = Sw_SFR_CMD_ADDR2;
            CMD_Buffer[selectedWay].iAddr3 = Sw_SFR_CMD_ADDR3;
            CMD_Buffer[selectedWay].iAddr4 = Sw_SFR_CMD_ADDR4;

            //cout << "addr = " << CMD_Buffer[0].iAddr1 << "  WAY = " <<selectedWay << endl;
            wait(SC_ZERO_TIME);
            wait(clk_p.posedge_event());

            int iCMD = Sw_SFR_CMD_OPCODE;
            //cout << "NAND ctrl CMD = " << iCMD << endl;
            if(iCMD == Program || iCMD == Copyback_Program );
            else if(iCMD == Read || iCMD == Erase || iCMD == Copyback_Read || iCMD == Copyback)
                newCommand[selectedWay].notify();
            else{
                cout << DEV_AND_TIME << "Error: Undefined Command!!" << (unsigned int)Sw_SFR_CMD_OPCODE <<  endl;
                sc_stop();
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
//                                                                                //%
template < unsigned int NUM_WAY >                                               //%
void                                                                            //%
NAND_CTRL< NUM_WAY >::DATA_Callback(                                            //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)           //%
{                                                                               //%
    const char* dbgMsgText = "TRANSPORT handler for memory Sw_SFR_DATA";        //%
    scml2::memory_alias< unsigned int >& m = this->Sw_SFR_DATA;                 //%
    tlm::tlm_command cmd = trans.get_command();                                 //%
    sc_dt::uint64 adr = trans.get_address();                                    //%
    unsigned char* ptr = trans.get_data_ptr();                                  //%
    unsigned int len = trans.get_data_length();                                 //%
    unsigned char* byt = trans.get_byte_enable_ptr();                           //%
    unsigned int wid = trans.get_streaming_width();                             //%
                                                                                //%USERBEGIN DATA_Callback
    if ( dbgFlag[REGS] ) {
        DBG_MSG( dbgMsgText );
        cout << name() << " cmd: " << cmd << "  addr: " << adr << "  len: " << len << "  ptr: " << (void*)ptr << endl;
    }
    // TODO: INSERT code for the DATA_Callback method here.

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
    unsigned int *ptr_int = (unsigned int*)ptr;

    if (cmd == tlm::TLM_WRITE_COMMAND) { // Execute command
        for(int i=0; i<len/4; i++){
            memcpy(&iTemp, &(ptr_int[i]), sizeof(int));
            m[iStartAddr + i] = iTemp;
        }                                                                                
        if((CMD_Buffer[selectedWay].opCode==Program) || (CMD_Buffer[selectedWay].opCode==Copyback_Program) ){
	    	for(int i=0; i < PAGE_SIZE/sizeof(int); i++)
                iaBuffer[selectedWay][i] = Sw_SFR_DATA[i];
            
            int waitTime = DataTransportTime(len);
            wait(waitTime,SC_NS);

            newCommand[selectedWay].notify();
        }
    } else if (cmd == tlm::TLM_READ_COMMAND) {
        int waitTime = DataTransportTime(len);
        wait(waitTime,SC_NS);

     
        for(int i=0 ; i < len/4 ; i++)
            ptr_int[i] = iaBuffer[selectedWay][i];

//
//	    for(int i=0; i < PAGE_SIZE/4 ; i++)
//            Sw_SFR_DATA[i] = iaBuffer[selectedWay][i];
//        for(int i=0; i<len/4; i++){
//            iTemp = m[iStartAddr + i];
//            memcpy(&(ptr_int[i]), &iTemp, sizeof(int));
       // }
    }

    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
                                                                                //%USEREND DATA_Callback
}                                                                               //%
                                                                                //%
                                                                                //%USERBEGIN MEMBER_DEFNS
                                                                                //

// TODO: Insert definitions of your member functions, etc. here.
template < unsigned int NUM_WAY >                                               //%
void NAND_CTRL<NUM_WAY>::Command_Handler()
{
    int iWay = selectedWay;
    
    //cout << "My Handler id is " << iWay << endl;

    while(1){
        wait();
        int iCMD = CMD_Buffer[iWay].opCode;
        setRnB(iWay,0);
        holdRnB(iWay,true);
        if(iCMD==Program){
            m_CommandSemaphore.wait();
            CommandWrite(CMD_Buffer[iWay]);
            DataWrite(iWay);
            m_CommandSemaphore.post();
            wait(NAND_RnB[iWay].posedge_event());
        }
        else if(iCMD==Read){
            m_CommandSemaphore.wait();
            CommandWrite(CMD_Buffer[iWay]);
            m_CommandSemaphore.post();
            wait(NAND_RnB[iWay].posedge_event());
            m_CommandSemaphore.wait();
            DataRead(iWay);
            m_CommandSemaphore.post();
        }
        else if(iCMD==Erase){
            m_CommandSemaphore.wait();
            CommandWrite(CMD_Buffer[iWay]);
            m_CommandSemaphore.post();
            wait(NAND_RnB[iWay].posedge_event());
        }
        else if(iCMD==Copyback){
            NAND_Cmd temp;
            temp.opCode = Copyback_Read;
            temp.iAddr1 = CMD_Buffer[iWay].iAddr2;
            m_CommandSemaphore.wait();
            CommandWrite(temp);
            m_CommandSemaphore.post();
            wait(NAND_RnB[iWay].posedge_event());
    
            temp.opCode = Copyback_Program;
            temp.iAddr1 = CMD_Buffer[iWay].iAddr1;
            m_CommandSemaphore.wait();
            CommandWrite(temp);
            m_CommandSemaphore.post();
            wait(NAND_RnB[iWay].posedge_event());
        }
        else if(iCMD==Copyback_Program){
        
            int RNMlen = CMD_Buffer[iWay].iAddr4;
            NAND_Cmd temp;
            temp.opCode = Read;
            temp.iAddr1 = CMD_Buffer[iWay].iAddr1;
            temp.iAddr4 = RNMlen; 
            m_CommandSemaphore.wait();
            CommandWrite(temp);
            m_CommandSemaphore.post();
            wait(NAND_RnB[iWay].posedge_event());

            m_CommandSemaphore.wait();
            RNM_DataRead(iWay);
            m_CommandSemaphore.post();

    
            temp.opCode = Program;
            temp.iAddr1 = CMD_Buffer[iWay].iAddr1;
            m_CommandSemaphore.wait();
            CommandWrite(temp);
            RNM_Merge(iWay, RNMlen);
            DataWrite(iWay);
            m_CommandSemaphore.post();
            wait(NAND_RnB[iWay].posedge_event());
  

        }
        else if(iCMD==Copyback_Read){
            
            m_CommandSemaphore.wait();
            CommandWrite(CMD_Buffer[iWay]);
            m_CommandSemaphore.post();
            wait(NAND_RnB[iWay].posedge_event());

        }
        else{
            cout << DEV_AND_TIME << "Error: Undefined Command: " << (unsigned int)iCMD << endl;
            sc_stop();
        }


        setRnB(iWay,1);
        holdRnB(iWay,false);
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
                                                                                //%USEREND ORPHANED_CODE
#endif // ORPHANED_CODE                                                         //%
                                                                                //%USERBEGIN FOOTER_H

                                                                                //%USEREND FOOTER_H
#endif // __NAND_CTRL_H__                                                       //%
