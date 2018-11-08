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
#include "./header/ssd_struct.h"
#include "./header/memcpy.h"
#include "./header/IF_Spec.h"
#include "./header/global_flag.h"
#include "./header/buffer_ctrl.h"

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
    void Scheduler();
    void NotifyDataRead();

    
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
    sc_core::sc_event e_TrigScheduler;
    sc_core::sc_event e_TrigTransData;
    sc_core::sc_event e_WriteCMDQPop;
    sc_core::sc_event e_ReadCMDQPop;


    //CMD Queue
    CacheCMD readCMDQ[READ_Q_SIZE];
    CacheCMD writeCMDQ[WRITE_Q_SIZE];

    //Data Queue
    uchar readDataQ[READ_Q_SIZE][DATA_PAGE_SIZE];
    uchar writeDataQ[WRITE_Q_SIZE][DATA_PAGE_SIZE];

    uint readQPoint;
    uint readQHead, readQTail;
    uint writeQHead, writeQTail;

    bool PushWriteCMDQ(CacheCMD req);
    bool PushReadCMDQ(CacheCMD req);

    bool PopWriteCMDQ(uint index);
    bool PopReadCMDQ(uint index);

    bool CheckReadCMDQEmpty();
    bool CheckWriteCMDQEmpty();

    uint CheckWriteQSize();
    uint CheckReadQSize();

    bool CheckExistTransCMD();
    uint PushDataQIndex(SOURCE_DEVICE source, DRAM_CMD op_type);
    
    uint FindWQindex();
    uint FindRQindex();
    uint FindRDQindex(SOURCE_DEVICE source);
    int FindWQindexByCmd(CacheTrans_t cmd);

    sc_core::sc_semaphore sem_RCMD;
    sc_core::sc_semaphore sem_WCMD;
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
 R_DATA("R_DATA", 10000ULL)                                                       //%

 // TODO: Add your class initialization here.
    ,sem_RCMD(1)
    ,sem_WCMD(1)
    ,sem_Mem(1)
    //%USEREND INITIALIZER
                                                                                //%
{                                                                               //%
 // declare processes and their sensitivities                                   //%
	SC_METHOD(rst_nHandler1);                                               //%
	 sensitive << rst_n.neg() ;                                             //%
	 dont_initialize();                                                     //%
 
    SC_THREAD(Scheduler);
     sensitive << e_TrigScheduler;
     dont_initialize();
     
    SC_THREAD(NotifyDataRead);
     sensitive << e_TrigTransData;
     dont_initialize();

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
}
                                                                                //%USEREND rst_nHandler1

bool
DRAM_ctrl::PushWriteCMDQ(CacheCMD req)
{
    sem_WCMD.wait();
    if(writeQHead == (writeQTail + 1) % WRITE_Q_SIZE){
        if(DRAM_DEBUG) cout << DEV_AND_TIME <<  "[PushWriteCMDQ] write q full writeQHead = " 
            <<writeQHead << " tail = " << writeQTail << endl;
        wait(e_WriteCMDQPop); // full
    }

    memcpy(&writeCMDQ[writeQTail], &req, sizeof(CacheCMD));
    writeCMDQ[writeQTail].isDataReady = false;
    writeQTail = (writeQTail + 1) % WRITE_Q_SIZE;
    sem_WCMD.post();
    return true;
    

}

/*
bool
DRAM_ctrl::PushWriteCMDQ(CacheCMD req)
{
    if(writeQHead == (writeQTail + 1) % WRITE_Q_SIZE){
        if(DRAM_DEBUG) cout << DEV_AND_TIME <<  "[PushWriteCMDQ] write q full writeQHead = " 
            <<writeQHead << " tail = " << writeQTail << endl;
        return false; // full
    }else{ 
        memcpy(&writeCMDQ[writeQTail], &req, sizeof(CacheCMD));
        writeCMDQ[writeQTail].isDataReady = false;
        writeQTail = (writeQTail + 1) % WRITE_Q_SIZE;
        return true;
    }
}
*/
bool
DRAM_ctrl::PushReadCMDQ(CacheCMD req)
{
    sem_RCMD.wait();
    if(readQHead == (readQTail + 1) % READ_Q_SIZE){
        if(DRAM_DEBUG) cout << DEV_AND_TIME <<  "[PushReadCMDQ] read q full readQHead = " 
            <<readQHead << " tail = " << readQTail << endl;
        wait(e_ReadCMDQPop); // full
    }

    memcpy(&readCMDQ[readQTail], &req, sizeof(CacheCMD));
    readCMDQ[readQTail].isCMDTrans = false;
    readCMDQ[readQTail].isDataReady = false;
    readQTail = (readQTail + 1) % READ_Q_SIZE;
    sem_RCMD.post();
    return true;
}


/*bool
DRAM_ctrl::PushReadCMDQ(CacheCMD req)
{
    if(readQHead == (readQTail + 1) % READ_Q_SIZE){
        //if(DRAM_DEBUG) cout << DEV_AND_TIME <<  "[PushReadCMDQ] read q full readQHead = " 
        //    <<readQHead << " tail = " << readQTail << endl;
        return false; // full
    }else{
        memcpy(&readCMDQ[readQTail], &req, sizeof(CacheCMD));
        readCMDQ[readQTail].isCMDTrans = false;
        readCMDQ[readQTail].isDataReady = false;
        readQTail = (readQTail + 1) % READ_Q_SIZE;
        return true;
    }
}
*/
bool
DRAM_ctrl::PopWriteCMDQ(uint index)
{
    if(index == writeQHead){
        writeQHead = (writeQHead + 1) % WRITE_Q_SIZE;
    }else{
        for(int i = (index + 1)%WRITE_Q_SIZE; i != writeQTail; i = (i+1)%WRITE_Q_SIZE){
            memcpy(&writeCMDQ[(WRITE_Q_SIZE+i-1)%WRITE_Q_SIZE], &writeCMDQ[i], sizeof(CacheCMD));
            memcpy(writeDataQ[(WRITE_Q_SIZE+i-1)%WRITE_Q_SIZE], writeDataQ[i], DATA_PAGE_SIZE);
        }
        writeQTail = (writeQTail + WRITE_Q_SIZE - 1) % WRITE_Q_SIZE;
    }

    e_WriteCMDQPop.notify();
}

bool
DRAM_ctrl::PopReadCMDQ(uint index)
{
    if(index == readQHead){
        readQHead = (readQHead + 1) % READ_Q_SIZE;
    }else{
        for(int i = (index + 1)%READ_Q_SIZE; i != readQTail; i = (i+1)%READ_Q_SIZE){
            memcpy(&readCMDQ[(READ_Q_SIZE+i-1)%READ_Q_SIZE], &readCMDQ[i], sizeof(CacheCMD));
            memcpy(readDataQ[(READ_Q_SIZE+i-1)%READ_Q_SIZE], readDataQ[i], DATA_PAGE_SIZE);
        }
        readQTail = (readQTail + READ_Q_SIZE - 1) % READ_Q_SIZE;
    }

    e_ReadCMDQPop.notify();
}


bool
DRAM_ctrl::CheckReadCMDQEmpty()
{
    for(int i=readQHead; i != readQTail; i = (i + 1) % READ_Q_SIZE){
        if(readCMDQ[i].isDataReady == false && readCMDQ[i].isCMDTrans == false){
            return false;
        }
    }
    return true;
}

bool
DRAM_ctrl::CheckWriteCMDQEmpty()
{
    for(int i=writeQHead; i != writeQTail; i = (i + 1) % WRITE_Q_SIZE){
        if(writeCMDQ[i].isDataReady == true){
            return false;
        }
    }
    return true;
}

uint
DRAM_ctrl::CheckWriteQSize()
{
    uint tempCount = 0;
    for(int i=writeQHead; i != writeQTail; i = (i + 1) % WRITE_Q_SIZE){
        tempCount++;
    }
    return tempCount;
}

uint
DRAM_ctrl::CheckReadQSize()
{
    uint tempCount = 0;
    for(int i=readQHead; i != readQTail; i = (i + 1) % READ_Q_SIZE){
        if(readCMDQ[i].isDataReady == false)
            tempCount++;
    }
    return tempCount;
}

bool
DRAM_ctrl::CheckExistTransCMD()
{
    for(int i=readQHead; i != readQTail; i = (i + 1) % READ_Q_SIZE){
        if(readCMDQ[i].isDataReady == true && readCMDQ[i].isCMDTrans == false){
            return true;
        }
    }
    return false;

}

uint
DRAM_ctrl::PushDataQIndex(SOURCE_DEVICE source, DRAM_CMD op_type)
{
    //if(DRAM_DEBUG)cout << DEV_AND_TIME <<  "[PushDataQIndex] head  = " << writeQHead << " tail = " << writeQTail << " op_type = "<< op_type <<endl;
    if(op_type == DRAM_WRITE){
        for(int i=writeQHead; i != writeQTail; i = (i + 1) % WRITE_Q_SIZE){
            if(writeCMDQ[i].SourceDevice == source && writeCMDQ[i].isDataReady == false){
                return i;
            }
        }
    }else{ //read data
        for(int i=readQHead; i != readQTail; i = (i + 1) % READ_Q_SIZE){
            if(readCMDQ[i].SourceDevice == source && readCMDQ[i].isDataReady == false){
                return i;
            }
        }
    }

    cout << DEV_AND_TIME <<"[PushDataQIndex] Don't find data q index!!" <<endl;
    assert(0);
        
    sc_stop();

}

uint 
DRAM_ctrl::FindWQindex()
{
    for(int i=writeQHead; i != writeQTail; i = (i + 1) % WRITE_Q_SIZE){
        if(writeCMDQ[i].isDataReady == true){
            return i;
        }
    }
    
    cout << DEV_AND_TIME <<"[FindWQindex] Don't find write cmd q index!!"<< endl;
    sc_stop();
}

int 
DRAM_ctrl::FindWQindexByCmd(CacheTrans_t cmd)
{
    for(int i=writeQHead; i != writeQTail; i = (i + 1) % WRITE_Q_SIZE){
        if((writeCMDQ[i].cmd.lba == cmd.lba) && (writeCMDQ[i].cmd.Id == cmd.Id)){
            if(writeCMDQ[i].isDataReady == true)
                return (int)i;
            else{
                //testTrack.done(cmd.Id, cmd.lba, cmd.Len/SECTOR_SIZE_BYTE, SSDmetric::SubReqMan , sc_time_stamp().value()/1000.0); 
             //   testTrack.done(cmd.Id, cmd.lba, cmd.Len/SECTOR_SIZE_BYTE, SSDmetric::DRAM_ctrl, sc_time_stamp().value()/1000.0); 
              //  testTrack.done(cmd.Id, cmd.lba, cmd.Len/SECTOR_SIZE_BYTE, SSDmetric::NAND_Manager, sc_time_stamp().value()/1000.0); 
               // testTrack.IDFinish();
                assert(0 && "Request came before data was transferred to DRAM_ctrl (check WriteCmd_Write at SubReqMan");
            }
        }
    }
    
    return -1;
}

uint 
DRAM_ctrl::FindRQindex()
{
    for(int i=readQHead; i != readQTail; i = (i + 1) % READ_Q_SIZE){
        if(readCMDQ[i].isDataReady == false){

            return i;
        }
    }
    
    cout << DEV_AND_TIME <<"[FindRQindex] Don't find read cmd q index!!" <<endl;
    sc_stop();

}

uint 
DRAM_ctrl::FindRDQindex(SOURCE_DEVICE source)
{
    for(int i=readQHead; i != readQTail; i = (i + 1) % READ_Q_SIZE){
        if(readCMDQ[i].SourceDevice == source){
            return i;
        }
    }
    cout << DEV_AND_TIME <<"[FindRDQindex] Don't find read data q index!!" <<endl;
    sc_stop();
}

void
DRAM_ctrl::Scheduler()
{
    while(1){

        while(!CheckReadCMDQEmpty() || !CheckWriteCMDQEmpty()){
           
            //cout<<DEV_AND_TIME<<"write q size = " << CheckWriteQSize() <<" read q size = " << CheckReadQSize()<< endl;
            if(CheckReadCMDQEmpty() && !CheckWriteCMDQEmpty()){
                //operate write cmd
                //FIFO
                uint index = FindWQindex();
                if(DRAM_DEBUG){
                    cout << DEV_AND_TIME << "[Scheduler] DRAM WRITE : ID = " << writeCMDQ[index].cmd.Id 
                        << " OP = " << writeCMDQ[index].cmd.Op 
                        << " ADR = " << writeCMDQ[index].cmd.Addr 
                        << " LEN =  "  <<writeCMDQ[index].cmd.Len
                        << " SOURCE = " << writeCMDQ[index].SourceDevice  <<endl;
                    
                    cout << DEV_AND_TIME <<  "[Scheduler] DRAM write Data = ";
                    for(int i =0; i < writeCMDQ[index].cmd.Len/SECTOR_SIZE_BYTE; i++){
                        cout << writeDataQ[index][i*SECTOR_SIZE_BYTE];
                    }
                    cout << endl;
                }

                if(g_initialize_end) {
                    DRAM_STATS[TOTAL_LEN][BUFFER][cacheWRITE] += writeCMDQ[index].cmd.Len; //0 : Cache Buffer, 0 : Write
                    
                    if(writeCMDQ[index].cmd.Len <= DRAM_PAGE) DRAM_STATS[TOTAL_ENERGY][BUFFER][cacheWRITE] += DRAM_PAGE; 
                    else DRAM_STATS[TOTAL_ENERGY][BUFFER][cacheWRITE] += writeCMDQ[index].cmd.Len; 
                }

                
                    
                wait(memLatency(writeCMDQ[index].cmd.Len, BUFFER, cacheWRITE), SC_NS);
                sem_Mem.wait();
                MemoryMasterPort.write(writeCMDQ[index].cmd.Addr + CACHE_DATA_REGION_OFFSET , 
                        (uchar *)&writeDataQ[index], writeCMDQ[index].cmd.Len);
                sem_Mem.post();

                
                if(DRAM_DEBUG){
                    cout <<DEV_AND_TIME<<"[Scheduler] DRAM WRITE END" <<endl;
                }

                //remove cmd q
                PopWriteCMDQ(index);

            }else{
                //when write Q is filled with more than xx%(current: 80%), operate write cmd
//                if((CheckWriteQSize() >= WRITE_Q_SIZE * WRITE_Q_LIMIT_FACTOR && !CheckWriteCMDQEmpty()) 
//                        && (CheckReadQSize() <= READ_Q_SIZE * READ_Q_LIMIT_FACTOR)){
//                    //operate write cmd
//                    uint index = FindWQindex();
//                    if(DRAM_DEBUG){
//                        cout << DEV_AND_TIME << "[Scheduler] DRAM WRITE : ID = " << writeCMDQ[index].cmd.Id 
//                            << " OP = " << writeCMDQ[index].cmd.Op 
//                            << " ADR = " << writeCMDQ[index].cmd.Addr 
//                            << " LEN =  "  <<writeCMDQ[index].cmd.Len
//                            << " SOURCE = " << writeCMDQ[index].SourceDevice  <<endl;
//
//                        cout << DEV_AND_TIME <<  "[Scheduler] DRAM write Data = ";
//                        for(int i =0; i < writeCMDQ[index].cmd.Len/SECTOR_SIZE_BYTE; i++){
//                            cout << writeDataQ[index][i*SECTOR_SIZE_BYTE];
//                        }
//                        cout << endl;
//                    }
//   
//                    if(g_initialize_end)DRAM_STATS[0][0]+= writeCMDQ[index].cmd.Len; //0 : Cache Buffer, 0 : Write
//                    MemoryMasterPort.write(writeCMDQ[index].cmd.Addr + CACHE_DATA_REGION_OFFSET , 
//                       (uchar *)&writeDataQ[index], writeCMDQ[index].cmd.Len);
//
//                    cout <<DEV_AND_TIME<<"[Scheduler] DRAM WRITE END" <<endl;
//                    PopWriteCMDQ(index);

                //}else{
                    //operate read cmd
                    //find read request. check dependency
                uint index = FindRQindex();
                //cout<<DEV_AND_TIME<<"[Scheduler] debug id = "<<debug_id<<endl;

                if(DRAM_DEBUG){
                    cout << DEV_AND_TIME << " [Scheduler] DRAM READ : ID = " << readCMDQ[index].cmd.Id 
                        << " OP = " << readCMDQ[index].cmd.Op 
                        << " ADR = " << readCMDQ[index].cmd.Addr 
                        << " LEN =  "  <<readCMDQ[index].cmd.Len
                        << " SOURCE = " << readCMDQ[index].SourceDevice  <<endl;
                }

                if(g_initialize_end){
                    DRAM_STATS[TOTAL_LEN][BUFFER][cacheREAD]+= readCMDQ[index].cmd.Len; //0 : Cache Buffer, 1 : Read
                    if(readCMDQ[index].cmd.Len <= DRAM_PAGE) DRAM_STATS[TOTAL_ENERGY][BUFFER][cacheREAD] += DRAM_PAGE; 
                    else DRAM_STATS[TOTAL_ENERGY][BUFFER][cacheREAD] += readCMDQ[index].cmd.Len; 
                }

               // if( (readCMDQ[index].SourceDevice == NAND) && g_initialize_end) testTrack.track(readCMDQ[index].cmd.Id, readCMDQ[index].cmd.lba, readCMDQ[index].cmd.Len/SECTOR_SIZE_BYTE, SSDmetric::DRAM_ctrl, sc_time_stamp().value()/1000.0); //track 4

                //write q check
                int write_index = FindWQindexByCmd(readCMDQ[index].cmd);
                if(write_index >= 0){

                    //write data -> read q data
                    memcpy(readDataQ[index], writeDataQ[write_index], readCMDQ[index].cmd.Len); 

                }
                else{


                    index = FindRQindex();
                    wait(memLatency(readCMDQ[index].cmd.Len, BUFFER, cacheREAD), SC_NS);
                    sem_Mem.wait();
                    index = FindRQindex();
                    MemoryMasterPort.read(readCMDQ[index].cmd.Addr + CACHE_DATA_REGION_OFFSET , 
                            (uchar *)&readDataQ[index], readCMDQ[index].cmd.Len);
                    sem_Mem.post();

                }

                index=FindRQindex();
                
               // if((readCMDQ[index].SourceDevice == NAND) && g_initialize_end) testTrack.track(readCMDQ[index].cmd.Id, readCMDQ[index].cmd.lba, readCMDQ[index].cmd.Len/SECTOR_SIZE_BYTE, SSDmetric::DRAM_ctrl, sc_time_stamp().value()/1000.0); //track 5

                if(DRAM_DEBUG){
                    cout <<DEV_AND_TIME<<"  [Scheduler] DRAM read Data = ";
                    for(int i =0; i < readCMDQ[index].cmd.Len/SECTOR_SIZE_BYTE; i++){
                        cout << readDataQ[index][i*SECTOR_SIZE_BYTE];
                    }
                    cout << endl;
                }

                readCMDQ[index].isDataReady = true;

                e_TrigTransData.notify();
                //}
            }

            wait(SC_ZERO_TIME);
        }
        wait();
    }
}

void
DRAM_ctrl::NotifyDataRead()
{
    while(1){
        if(DRAM_DEBUG) cout<<DEV_AND_TIME<< "[NotifyDataRead] Thread is started"<<endl;

        while(CheckExistTransCMD()){
            bool tempFlag = false;
            for(int i = readQHead; i != readQTail; i = (i+1)%READ_Q_SIZE){
                //cout<< readCMDQ[i].cmd.Addr<<"\t" << readCMDQ[i].isCMDTrans <<"\t"<< readCMDQ[i].isDataReady <<"\t"<< readCMDQ[i].SourceDevice << "\t"<<tempFlag<<endl;
                if((readCMDQ[i].isCMDTrans == true) && (readCMDQ[i].isDataReady == true) && (readCMDQ[i].SourceDevice == HOST)){
                    tempFlag = true;
                    continue;
                }
                if(!(tempFlag == true && readCMDQ[i].SourceDevice == HOST)){
                    if(readCMDQ[i].isCMDTrans == false && readCMDQ[i].isDataReady == true){

                        //cout<<DEV_AND_TIME<<"[NotifyDataRead] is running 222"<<endl;
                        if(DRAM_DEBUG){
                            //cout << DEV_AND_TIME << " [NotifyDataRead] SEND READ CMD : ID = " << debug_id <<endl;
                            cout << DEV_AND_TIME << " [NotifyDataRead] SEND READ CMD : ID = " << readCMDQ[i].cmd.Id 
                                << " OP = " << readCMDQ[i].cmd.Op 
                                << " ADR = " << readCMDQ[i].cmd.Addr 
                                << " LEN =  "  <<readCMDQ[i].cmd.Len
                                << " SOURCE = " << readCMDQ[i].SourceDevice  <<endl;
                        }

                        if(readCMDQ[i].SourceDevice == HOST){
                            while(!StateInform.write(0xe0000018, &readCMDQ[i], sizeof(CacheCMD))){
                                //wait(CLOCK_PERIOD_NS, SC_NS);
                                wait(1, SC_NS);
                            }

                        }else if(readCMDQ[i].SourceDevice == NAND){



                            while(!StateInform.write(0xf0000050, &readCMDQ[i], sizeof(CacheCMD))){
                                //wait(CLOCK_PERIOD_NS, SC_NS);
                                wait(1, SC_NS);
                            }

                        }else{
                            cout << DEV_AND_TIME <<" [NotifyDataRead] Wrong source device!!!" <<endl;
                            sc_stop();
                        }
                        if(DRAM_DEBUG){
                            cout << DEV_AND_TIME <<" [NotifyDataRead] SEND READ CMD END" <<endl;
                        }
                        readCMDQ[i].isCMDTrans = true;
                    }
                }
            }

            wait(10, SC_NS);
        }

        if(DRAM_DEBUG) cout<<DEV_AND_TIME<< "[NotifyDataRead] Thread is ended"<<endl;
        wait();
    }
}


void                                                                            //%
DRAM_ctrl::CMD_Callback(                                                     //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)           //%
{   
    
    
    double BW_time_start = sc_time_stamp().value()/1000.0;
    
    
    const char* dbgMsgText = "TRANSPORT handler for memory R_CMD";           //%
    scml2::memory< unsigned int >& m = this->R_CMD;                          //%
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

    //bus delay
    wait( len/CMD_BUS_TOTAL_BW, SC_NS);

    if (cmd == tlm::TLM_WRITE_COMMAND) {
        if(adr < CACHE_DATA_REGION_OFFSET ){
//            //Initialize FTL mapping table
//            if(g_isFtlReady == false){
//                MemoryMasterPort.write(adr, ptr, len);
//                trans.set_response_status( tlm::TLM_OK_RESPONSE );
//            }
            // ympark0225
            if(g_initialize_end) {

                wait(memLatency(len, META, cacheWRITE), SC_NS);

            }   
            sem_Mem.wait();
            MemoryMasterPort.write(adr, ptr, len);
            sem_Mem.post();
            //for result
            if(CACHE_BUFFER_METHOD == DRAM_ONLY){
                //if(g_initialize_end) DRAM_STATS[1][0]+= DRAM_PAGE; //0 : Metadata, 0 : Write
                if(g_initialize_end) {
                    DRAM_STATS[TOTAL_LEN][META][cacheWRITE]+= len; //0 : Metadata, 0 : Write
                    if(len <= DRAM_PAGE) DRAM_STATS[TOTAL_ENERGY][META][cacheWRITE] += DRAM_PAGE; 
                    else DRAM_STATS[TOTAL_ENERGY][META][cacheWRITE] += len; 
                
                }
            }
            else if(CACHE_BUFFER_METHOD == PRAM_ONLY || CACHE_BUFFER_METHOD == HYBRID){
                
                if(g_initialize_end) {

                    DRAM_STATS[TOTAL_LEN][META][cacheWRITE]+= len; //0 : Metadata, 0 : Write
                    DRAM_STATS[TOTAL_ENERGY][META][cacheWRITE] += len; // considering byte access enable
                
                }
            }
            //cout<<DEV_AND_TIME <<"meta data write, length = "<<len<<endl;
            trans.set_response_status( tlm::TLM_OK_RESPONSE );

        }else{
            
            CacheCMD tempReq;
            memcpy(&tempReq.cmd, ptr, len);

            if(adr == HOST_SIDE_CALLBACK_OFFSET + CACHE_DATA_REGION_OFFSET){
                tempReq.SourceDevice = HOST;
            }else if(adr == NAND_SIDE_CALLBACK_OFFSET + CACHE_DATA_REGION_OFFSET){
                tempReq.SourceDevice = NAND;
            }else{
                cout << DEV_AND_TIME << "[CMD_Callback] Don't know source device, addr = " << hex << adr << dec << endl;
                assert(0);
                sc_stop();
            }
            
            if(DRAM_DEBUG){
                cout << DEV_AND_TIME << "[CMD_Callback] Received CMD: ID = " << tempReq.cmd.Id 
                    << " OP = " << tempReq.cmd.Op << " ADR = " << tempReq.cmd.Addr 
                    << " LEN =  "  <<tempReq.cmd.Len <<" SOURCE = " << tempReq.SourceDevice <<endl;
            }

            tempReq.isDataReady = false;

            if(tempReq.cmd.Op == DRAM_WRITE){
                if(PushWriteCMDQ(tempReq)){

                    //if( (tempReq.SourceDevice == HOST) && g_initialize_end) testTrack.start(tempReq.cmd.Id, tempReq.cmd.lba, tempReq.cmd.Len/SECTOR_SIZE_BYTE, SSDmetric::DRAM_ctrl, sc_time_stamp().value()/1000.0);
                    trans.set_response_status( tlm::TLM_OK_RESPONSE );
                }else{
                    trans.set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );
                }
            }else{
                if(PushReadCMDQ(tempReq)){

                   // if( (tempReq.SourceDevice == NAND) && g_initialize_end) testTrack.track(tempReq.cmd.Id, tempReq.cmd.lba, tempReq.cmd.Len/SECTOR_SIZE_BYTE, SSDmetric::DRAM_ctrl, sc_time_stamp().value()/1000.0); //track 3
                    //else if ( (tempReq.SourceDevice == HOST) && g_initialize_end) testTrack.start(tempReq.cmd.Id, tempReq.cmd.lba, tempReq.cmd.Len/SECTOR_SIZE_BYTE, SSDmetric::DRAM_ctrl, sc_time_stamp().value()/1000.0);
                    
                    trans.set_response_status( tlm::TLM_OK_RESPONSE );
                    if(DRAM_DEBUG) {
                        if(tempReq.SourceDevice == HOST )
                            cout<< DEV_AND_TIME <<"Received read dram cmd from SubReqMan"<<endl;
                    }
                }else{
                    trans.set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );
                }
                //call scheduler
                e_TrigScheduler.notify();

            }
        }
    } else if (cmd == tlm::TLM_READ_COMMAND) {
        if(adr < CACHE_DATA_REGION_OFFSET){
//            //Initialize FTL mapping table
//            if(g_isFtlReady == false){
//                MemoryMasterPort.read(adr, ptr, len);
//                trans.set_response_status( tlm::TLM_OK_RESPONSE );
//            }
            // ympark0225
            if(g_initialize_end){
            
                wait(memLatency(len, META, cacheREAD), SC_NS);
            
            }
            
            sem_Mem.wait();
            MemoryMasterPort.read(adr, ptr, len);
            sem_Mem.post();
            //for result
            if(CACHE_BUFFER_METHOD == DRAM_ONLY){
                //if(g_initialize_end) DRAM_STATS[1][0]+= DRAM_PAGE; //0 : Metadata, 0 : Write
                if(g_initialize_end) {
                    DRAM_STATS[TOTAL_LEN][META][cacheREAD]+= len; //0 : Metadata, 0 : Write
                    if(len <= DRAM_PAGE) DRAM_STATS[TOTAL_ENERGY][META][cacheREAD] += DRAM_PAGE; 
                    else DRAM_STATS[TOTAL_ENERGY][META][cacheREAD] += len; 
                
                }
            }
            else if(CACHE_BUFFER_METHOD == PRAM_ONLY || CACHE_BUFFER_METHOD == HYBRID){
                if(g_initialize_end) {

                    DRAM_STATS[TOTAL_LEN][META][cacheREAD]+= len; //0 : Metadata, 0 : Write
                    DRAM_STATS[TOTAL_ENERGY][META][cacheREAD] += len; 
                }

            }

            //cout<<DEV_AND_TIME <<"meta data read, length = "<<len<<endl;
            trans.set_response_status( tlm::TLM_OK_RESPONSE );

        }else{
            cout << DEV_AND_TIME 
                <<"[CMD_Callback] ERROR: MEMORY ctrl don't receive TLM_READ_COMMAND!!" << endl;
            sc_stop();
        }
    }

    if(g_initialize_end) 
        CMD_BUS_TIME_CONSUMED[1] += sc_time_stamp().value()/1000.0 - BW_time_start;
    
    //%USEREND Device_Callback
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
    unsigned int len = trans.get_data_length()/SECTOR_SIZE_BYTE;                                 //%
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

    if (cmd == tlm::TLM_WRITE_COMMAND) { 
        // copy from 'ptr' to your target's memory.  e.g.: memcpy(&mem[adr], ptr, num_bytes);
        SOURCE_DEVICE source;
        uint index;

        wait( (len*SECTOR_SIZE_BYTE)/DATA_BUS_TOTAL_BW, SC_NS);
        if(adr == HOST_SIDE_CALLBACK_OFFSET){
            source = HOST;
        }else if(adr == NAND_SIDE_CALLBACK_OFFSET){
            source = NAND;
        }
        
        if(DRAM_DEBUG){
            cout << DEV_AND_TIME << "[DATA_Callback] Received DATA SOURCE = " << source << endl;
        }

        index = PushDataQIndex(source, DRAM_WRITE);
        
        memcpy(&writeDataQ[index], ptr, len*SECTOR_SIZE_BYTE );

        if(DRAM_DEBUG){
            cout <<DEV_AND_TIME<<"[DATA_Callback] DRAM ctrl received leng, Data = ";
            cout<< len  <<"\t";
            for(int i =0; i < len ; i++){
                cout << writeDataQ[index][i*SECTOR_SIZE_BYTE];
            }
            cout << endl;
        }
        
        writeCMDQ[index].isDataReady = true;

        //call scheduler
        e_TrigScheduler.notify();

    } else if (cmd == tlm::TLM_READ_COMMAND) {
        // copy from your target's memory to 'ptr', e.g.: memcpy(ptr, &mem[adr], num_bytes);
        SOURCE_DEVICE source;
        uint index;
        wait( (len*SECTOR_SIZE_BYTE)/DATA_BUS_TOTAL_BW, SC_NS);
        if(adr == HOST_SIDE_CALLBACK_OFFSET){
            source = HOST;

        }else if(adr == NAND_SIDE_CALLBACK_OFFSET){
            source = NAND;
        }
        
        index = FindRDQindex(source);
        if(DRAM_DEBUG){
            cout << DEV_AND_TIME << "[DATA_Callback] Send DATA: ID = " << readCMDQ[index].cmd.Id 
                << " OP = " << readCMDQ[index].cmd.Op << " ADR = " << readCMDQ[index].cmd.Addr 
                << " LEN =  "  <<readCMDQ[index].cmd.Len <<" SOURCE = " << readCMDQ[index].SourceDevice <<endl;
        }       
        if(DRAM_DEBUG){
            cout << DEV_AND_TIME << "[DATA_Callback] DRAM ctrl send Data = ";
            for(int i =0; i < readCMDQ[index].cmd.Len/SECTOR_SIZE_BYTE; i++){
                cout << readDataQ[index][i*SECTOR_SIZE_BYTE];
            }
            cout << endl;
        }

        memcpy(ptr, &readDataQ[index], readCMDQ[index].cmd.Len);

       // if((readCMDQ[index].SourceDevice == NAND) && g_initialize_end) testTrack.done(readCMDQ[index].cmd.Id, readCMDQ[index].cmd.lba, readCMDQ[index].cmd.Len/SECTOR_SIZE_BYTE, SSDmetric::DRAM_ctrl, sc_time_stamp().value()/1000.0);
        PopReadCMDQ(index);
        e_TrigTransData.notify();
    }
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
    if(g_initialize_end) DATA_BUS_TIME_CONSUMED[1] += sc_time_stamp().value()/1000.0 - BW_time_start;
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
