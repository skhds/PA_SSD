//--------------------------------------------------------------                //%
// Important:  The template for this file was generated                         //%
//  automatically and can only be re-generated if rules                         //%
//  are followed when editing:                                                  //%
// 1. Do not modify any line containing //%                                     //%
// 2. Only insert your lines of code between 2 lines                            //%
//    containing "//% user begin" and "//% user end"                            //%
//                                                                              //%
// :generate:date: Tue Jan 14 2014 16:15:37                                     //%
// :generate:version: 2010.1.0-dev                                              //%
//--------------------------------------------------------------                //%
                                                                                //%
#ifndef __HOST_IF_H__                                                           //%
#define __HOST_IF_H__                                                           //%
                                                                                //%USERBEGIN MODEL_DEBUG_ENABLE
#if defined(DEBUG) || defined(DBG_SCP)
# define DBG_SCP_HOST_IF
#endif

                                                                                //%USEREND MODEL_DEBUG_ENABLE
                                                                                //%
#include <scml2.h>                                                              //%
#include <systemc>                                                             //%
#include "scmlinc/scml_command_processor.h" 
                                                                                //%USERBEGIN HEADER_H
// TODO: Insert your includes, declarations, etc. here.
#include "./header/ssd_struct.h"
#include "./header/memcpy.h"
#include "./header/IF_Spec.h"
#include "./header/global_flag.h"
#include <fstream>
#include <sys/time.h>
#define NUM_OF_REQUESTS 1000000

//for data comparison
//char data_map[MEMORY_SIZE];
#define max_req_len 1024

char requestData[Q_SIZE][max_req_len*UNIT_OF_REQUEST];

                                                                                //%USEREND HEADER_H
                                                                                //%
//------------------------                                                      //%
// Module Interface                                                             //%
//------------------------                                                      //%
class HOST_IF :                                                                 //%
	public sc_core::sc_module                                               //%
                                                                                //%USERBEGIN INHERITANCE
    // TODO: Specify class inheritance here. Start each line with a , character
                                                                                //%USEREND INHERITANCE
                                                                                //%
{                                                                               //%
public:                                                                         //%
	// constructor                                                          //%
	HOST_IF( const sc_core::sc_module_name & n ) ;                          //%
	SC_HAS_PROCESS( HOST_IF );                                              //%
	// destructor                                                           //%
	virtual ~HOST_IF();                                                     //%
private:                                                                        //%
	// disable default and copy constructors by making them private         //%
	HOST_IF() ;                                                             //%
	HOST_IF( const HOST_IF& ) ;                                             //%
	HOST_IF & operator= ( const HOST_IF& ) ;                                //%
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
	scml2::initiator_socket< 32 > HOST_Master;                              //%
	tlm::tlm_target_socket< 32 > HOST_Slave;                                //%
	sc_core::sc_in< bool > rst_n;                                           //%
                                                                                //%
protected:                                                                      //%
                                                                                //%
	// Memory declarations                                                  //%
	scml2::memory< unsigned int > R_HOST;                                   //%
	  scml2::memory_alias< unsigned int > R_HOST_req;                       //%
	  scml2::memory_alias< unsigned int > R_HOST_data;                      //%
                                                                                //%
	scml2::tlm2_gp_target_adapter< 32 > HOST_Slave_R_HOST_adapter;          //%
                                                                                //%
public:                                                                         //%
	// Process declarations                                                 //%
	void                                                                    //%
	rst_nHandler1();                                                        //%
    void
    HOST_monitor();
    void
    HOST_Interface();
    void
    HOST_To_Device_Req_Trans();
    void
    HOST_To_Device_Data_Trans();

protected:                                                                      //%
	// Memory callback declarations                                         //%
	void                                                                    //%
	HOST_Callback(                                                          //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t);          //%

                                                                                //%
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
    //Debugging Parameter
    uint64 operationCount;

    //constructor parameter
    Req_t requestBuffer[Q_SIZE];
    
    uint reqBufferHead;
    uint reqBufferTail;
    uint reqBufferPoint;
    uint requestId;
   
    FILE *traceFile;
    FILE *initFile;  
    Req_t recievedReq;
    char recievedData[max_req_len*UNIT_OF_REQUEST];

    Req_t transReq;
    char transData[max_req_len*UNIT_OF_REQUEST];

    double* latency_list;

    //Flag
    bool transDataFlag;
    bool transReqFlag;
    uint my_state;
    uint next_state;

    uint totalWriteLength = 0;
    uint totalReadLength = 0;
    uint totalWriteCount = 0;
    uint totalReadCount = 0;

    // Event & semaphore
    sc_core::sc_event e_TrigMonitor;
    sc_core::sc_event e_TrigHost;
    sc_core::sc_event e_DeleteReqBuffer;
    sc_core::sc_event e_TrigReqTrans;
    sc_core::sc_event e_TrigDataTrans;
    sc_core::sc_semaphore sem_Trans;
    sc_core::sc_event e_TransDataDone;
    sc_core::sc_event e_TransReqDone;

    //inline Function
    inline void Trace_File_Open(){
        traceFile = fopen("./../trace/ex_trace.txt","r");
    }

    inline void Trace_File_Close(){
        fclose(traceFile);
    }

    inline uint Cal_repeatCount(uint length){
        return ((length - 1) / max_req_len) + 1;
    }

    inline bool Check_Buffer_Full(){
        if(reqBufferHead == (reqBufferTail + 1) % Q_SIZE){
            return true;      
        }else{
            return false;    
        }
    }

    inline bool Check_Buffer_Empty(){
        if(reqBufferPoint == reqBufferTail){
            
            return true;    
        }else{
            return false;   
        }
    }

    inline bool Check_Simulation_End(){
        if(reqBufferHead == reqBufferTail)
            return true;
        else
            return false;
    }


    double calculateAvgLatency(double* list){

        int index = 0;
        double sum = 0;
        for (int i = 0; i< NUM_OF_REQUESTS; i++){

            if(list[i] != 0){

                index++;
                sum += list[i];
            }

        }


        return sum/index;



    }

    double calculateMaxLatency(double* list){

        double max = 0;
        for (int i = 0; i< NUM_OF_REQUESTS; i++){

            if(list[i] > max){

                max = list[i];
            }

        }


        return max;

    }

    void view_latency_list(double* list){

        cout << "******************View Latency List******************************" << endl << endl;
        for (int i = 0; i< NUM_OF_REQUESTS; i++)
            if(list[i] != 0 )cout << "At " << i << ", " << list[i] << endl;
    }

    void Delete_Request(uint id);
    void Push_Request(int addr, uint len, uint op);
    void Create_Data(uint64 len);
    uint Find_Request(uint id);
    void Write_Data_Map(uint addr, uint64 len);
    void Compare_Data();


    double calculateBusTime(double* bus_time){
        
        double sum = 0;
        for(int i=0; i<3; i++){

            sum += bus_time[i];

        }
        return sum;
    }


    double calculateDRAMEnergy(long long int stats[2][2][2], double exec_time){

        double dram_read_energy = 0.00000936; //uJ per 1B
        double dram_write_energy = 0.00000936; //uJ per 1B
//        double dram_leakage_energy = 1725 + 108; //(uW) 27.6mW per 256MB... current DRAM size is 16MB (1725uW) + cache (0.5MB ~ 8GB) 
        double dram_leakage_energy = CURRENT_DRAM_SIZE * 108; //(uW) 27.6mW per 256MB... current DRAM size is 16MB (1725uW) + cache (0.5MB ~ 8GB) 
        double sum = 0;
        double ns_to_s = 1000000000;
        double dram_dynamic_energy = 0;
        double dram_static_energy = 0;

        if(CACHE_BUFFER_METHOD == DRAM_ONLY){
            dram_dynamic_energy = dram_read_energy*stats[TOTAL_ENERGY][0][0]+dram_read_energy*stats[TOTAL_ENERGY][1][0]+dram_write_energy*stats[TOTAL_ENERGY][0][1]+dram_write_energy*stats[TOTAL_ENERGY][1][1];
            dram_static_energy = exec_time*dram_leakage_energy/ns_to_s;
        }
        else if(CACHE_BUFFER_METHOD == PRAM_ONLY){
            dram_dynamic_energy = 0;
            dram_static_energy = 0;
        }
        else if(CACHE_BUFFER_METHOD == HYBRID){
             dram_dynamic_energy = dram_read_energy*stats[TOTAL_ENERGY][0][0]+dram_write_energy*stats[TOTAL_ENERGY][0][1];
            dram_static_energy = exec_time*dram_leakage_energy/ns_to_s; // need to modify dram leakage energy according to modified dram size
        }
        return (dram_dynamic_energy + dram_static_energy)/exec_time*ns_to_s;//uW 
    }

     double calculatePRAMEnergy(long long int stats[2][2][2], double exec_time){

        double pram_read_energy = 0.00001976; //uJ per 1B
        double pram_write_energy = 0.00013456; //uJ per 1B
        double pram_leakage_energy = 0; //(uW) 27.6mW per 256MB... current DRAM size is 16MB (1725uW) + cache (0.5MB ~ 8GB) 
        double sum = 0;
        double ns_to_s = 1000000000;
        double pram_dynamic_energy = 0;
        double pram_static_energy = 0;


        if(CACHE_BUFFER_METHOD == DRAM_ONLY){
            pram_dynamic_energy = 0;
            pram_static_energy = 0;
        }
        else if(CACHE_BUFFER_METHOD == PRAM_ONLY){
            pram_dynamic_energy = pram_read_energy*stats[TOTAL_ENERGY][0][0]+pram_read_energy*stats[TOTAL_ENERGY][1][0]+pram_write_energy*stats[TOTAL_ENERGY][0][1]+pram_write_energy*stats[TOTAL_ENERGY][1][1];
            pram_static_energy = exec_time*pram_leakage_energy/ns_to_s;
        }
        else if(CACHE_BUFFER_METHOD == HYBRID){
            pram_dynamic_energy = pram_read_energy*stats[TOTAL_ENERGY][1][0]+pram_write_energy*stats[TOTAL_ENERGY][1][1];
            pram_static_energy = exec_time*pram_leakage_energy/ns_to_s;
        }

        return (pram_dynamic_energy + pram_static_energy)/exec_time*ns_to_s;//uW 
    }

   
    double calculateNANDEnergy(){

        double PROG_e = 20; //uW
        double READ_e = 1.5; //uW
        double BER_e = 230;
        double sum = 0;
        
        for(int i=0; i<4; i++){

            for(int j=0; j<4; j++){

                for(int k=0; k<64; k++){

                    sum += 0*NAND_STATS[0][i][j][k]; //nothing
                    sum += PROG_e*NAND_STATS[1][i][j][k]; //program
                    sum += READ_e*NAND_STATS[2][i][j][k]; //read
                    sum += BER_e*NAND_STATS[3][i][j][k]; //erase
                    sum += (READ_e + PROG_e)*NAND_STATS[4][i][j][k]; //copyback
                    sum += PROG_e*NAND_STATS[5][i][j][k]; //copyback program
                    sum += READ_e*NAND_STATS[6][i][j][k]; //copyback read
                    
                }

            }
        }

        return sum;


    }
   
    long long current_timestamp() {
        struct timeval te; 
        gettimeofday(&te, NULL); // get current time
        long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
        // printf("milliseconds: %lld\n", milliseconds);
        return milliseconds;
    }
    
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
HOST_IF::HOST_IF( const sc_core::sc_module_name & n)                            //%
		: sc_core::sc_module(n)                                         //%
,                                                                               //%
 HOST_Master("HOST_Master"),                                                    //%
 HOST_Slave("HOST_Slave"),                                                      //%
 rst_n("rst_n"),                                                                //%
 HOST_Slave_R_HOST_adapter("HOST_Slave_R_HOST_adapter", HOST_Slave),            //%
 R_HOST("R_HOST", 131079ULL),                                                   //%
  R_HOST_req("R_HOST_req", R_HOST, 0LL, 7LL ),                                  //%
  R_HOST_data("R_HOST_data", R_HOST, 7LL, 131072LL )                            //%
                                                                                //%
                                                                                //%USERBEGIN INITIALIZER
    // TODO: Add your class initialization here.
    ,sem_Trans(1)
                                                                                //%
{                                                                               //%
 // declare processes and their sensitivities                                   //%
	SC_THREAD(rst_nHandler1);                                               //%
	 sensitive << rst_n.neg() ;                                             //%
	 dont_initialize();                                                     //%
    
    SC_THREAD(HOST_monitor);                                              //%
	 sensitive << e_TrigMonitor;                                               //%
	 dont_initialize();                                                     //%

    SC_THREAD(HOST_Interface);                                              //%
	 sensitive << e_TrigHost;                                               //%
	 dont_initialize();                                                     //%

    SC_THREAD(HOST_To_Device_Req_Trans);                                        //%
     sensitive << e_TrigReqTrans;                                              //%
     dont_initialize();                                                     //%

    SC_THREAD(HOST_To_Device_Data_Trans);
     sensitive << e_TrigDataTrans;
     dont_initialize();

                                                                                //%
	// bind target ports to memories                                        //%
	HOST_Slave_R_HOST_adapter(R_HOST);                                      //%
                                                                                //%
 // setup memory callbacks                                                      //%
	// - Memory: R_HOST                                                     //%
   scml2::set_callback(                                                     //%
       R_HOST,
       SCML2_CALLBACK(HOST_Callback),
       scml2::AUTO_SYNCING);                                                                             //%
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
HOST_IF::~HOST_IF()                                                             //%
{                                                                               //%
                                                                                //%USERBEGIN DESTRUCTOR

    // TODO: Add destructor code here
    fclose(traceFile);


                                                                                //%USEREND DESTRUCTOR
}                                                                               //%
// initialization method for ports                                              //%
void                                                                            //%
HOST_IF::initPorts()                                                            //%
{                                                                               //%
}                                                                               //%
// initialization method for registers                                          //%
void                                                                            //%
HOST_IF::initRegisters()                                                        //%
{                                                                               //%
    R_HOST.initialize(0x0);                                                     //%
    R_HOST_req.initialize(0x0);                                                 //%
    R_HOST_data.initialize(0x0);                                                //%
}                                                                               //%
//This method is invoked after the design is fully elaborated, but              //%
// before simulation begins.                                                    //%
void                                                                            //%
HOST_IF::end_of_elaboration()                                                   //%
{                                                                               //%
                                                                                //%USERBEGIN END_OF_ELAB
    // TODO: Add end_of_elaboration code here.
    this->initRegisters();
    this->initPorts();

    Trace_File_Open();

    requestId = 0;
    operationCount = 0;
    reqBufferHead = 0;
    reqBufferTail = 0;
    reqBufferPoint = 0;
    my_state = IDLE;
    transDataFlag = false;
    transReqFlag = false;

                                                                                //%USEREND END_OF_ELAB
}                                                                               //%
// command processing method                                                    //%
//  During simulation, this command may be called to handle                     //%
//  interactive commands -- for dynamic instrumentation of the instance.        //%
std::string                                                                     //%
HOST_IF::commandProcessor(const std::vector< std::string >& cmd)                //%
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
HOST_IF::initMessageLogging()                                                   //%
{                                                                               //%
                                                                                //%USERBEGIN MODEL_DEBUG_INIT
    // toggle on/off debugging flags
    for (int i=0;i<sizeof(DbgCategories);i++) {
#ifdef DBG_SCP_HOST_IF
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
HOST_IF::REPORT_MSG(std::string sev, std::string msg)//%
{                                                 //%
                                                                                //%USERBEGIN REPORT_MSG
    std::cout << sev << ": " << msg << " [" << this->basename() << "@" << sc_core::sc_time_stamp() << "]" << std::endl;
                                                                                //%USEREND REPORT_MSG
                                                                                //%
}                                                                               //%
void                                                                            //%
HOST_IF::rst_nHandler1()                                                        //%
                                                                                //%USERBEGIN rst_nHandler1
{
	if ( dbgFlag[PORTS] ) DBG_MSG( "rst_nHandler1 invoked because of event on port rst_n" );

	// TODO: INSERT code for the rst_nHandler1 method here.
    while(1){
        
        latency_list = (double*)malloc(sizeof(double) * NUM_OF_REQUESTS);
        for(int i=0; i<NUM_OF_REQUESTS; i++){

            latency_list[i] = 0;

        }
        
        wait(SC_ZERO_TIME);
        e_TrigHost.notify(); 
        wait();
    }
}
                                                                                //%USEREND rst_nHandler1
                                                                                //
void                                                                            //%
HOST_IF::HOST_Callback(                                                         //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)           //%
{                                                                               //%
    const char* dbgMsgText = "TRANSPORT handler for memory R_HOST";             //%
    scml2::memory< unsigned int >& m = this->R_HOST;                            //%
    tlm::tlm_command cmd = trans.get_command();                                 //%
    sc_dt::uint64 adr = trans.get_address();                                    //%
    unsigned char* ptr = trans.get_data_ptr();                                  //%
    unsigned int len = trans.get_data_length();                                 //%
    unsigned char* byt = trans.get_byte_enable_ptr();                           //%
    unsigned int wid = trans.get_streaming_width();                             //%
                                                                                //%USERBEGIN HOST_Callback
    double dBusDelay;
    
    if(HOSTIF_DEBUG) cout << DEV_AND_TIME << "[HOST_Callback] callback received, cmd : " << cmd << endl;
    
    if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );
    // TODO: INSERT code for the HOST_Callback method here.

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
        Req_t tmpReq;
        uint deletePoint;
        switch(my_state)
        {
            case IDLE:

                
                if(HOSTIF_DEBUG) cout << DEV_AND_TIME << "[HOST_Callback] state is : IDLE" << endl;
                memcpy(R_HOST_req, ptr, len);

                memcpy(&tmpReq, R_HOST_req, len);
                
                if(tmpReq.Op == HOST_READ){   
                    memcpy(&recievedReq, R_HOST_req, len);
                    next_state = CMD;
                    if(g_initialize_end) 
                        //testTrack.track(tmpReq.iId, tmpReq.iAddr , tmpReq.iLen, SSDmetric::HOST_IF, sc_time_stamp().value()/1000.0); //skhds

                    if(HOSTIF_DEBUG) cout << DEV_AND_TIME << "[HOST_Callback] received read request ID : " << recievedReq.iId << endl;
                }else{
                    memcpy(&transReq, R_HOST_req, len);
                    next_state = IDLE;
    
                    if(HOSTIF_DEBUG) cout << DEV_AND_TIME << "[HOST_Callback] received write request, ID : " << transReq.iId << endl;
                    e_TrigDataTrans.notify();
                }
                dBusDelay = (double)16.0/ (double)IF_BANDWIDTH * 1000;
                break;

            case CMD:

                if(HOSTIF_DEBUG) cout << DEV_AND_TIME << "[HOST_Callback] state is : CMD" << endl;
                next_state = IDLE;
                memcpy(R_HOST_data, ptr, len);
                memcpy(&recievedData, R_HOST_data, len);

                if(COMPARISON)
                    Compare_Data();

                if(HOSTIF_DEBUG) cout << DEV_AND_TIME << "[HOST_Callback] id : " << recievedReq.iId << "  len :  " << recievedReq.iLen << endl;
                dBusDelay = ((double)recievedReq.iLen * (double)UNIT_OF_REQUEST / (double)IF_BANDWIDTH) * 1000.0;
                
                if(g_initialize_end){
                    testTrack.done(recievedReq.iId, recievedReq.iAddr, recievedReq.iLen, SSDmetric::HOST_IF, sc_time_stamp().value()/1000.0); //skhds
                }
                latency_list[recievedReq.iId] += sc_time_stamp().value()/1000.0;
                Delete_Request(recievedReq.iId);
                break;

            default:
	            if ( dbgFlag[PORTS] ) DBG_MSG( "Received request is wrong" );
                break;
        }

        my_state = next_state;

    } else if (cmd == tlm::TLM_READ_COMMAND) {
        // copy from your target's memory to 'ptr', e.g.: memcpy(ptr, &mem[adr], num_bytes);
    }
    sc_core::sc_time busDelay((uint)dBusDelay, SC_NS);
    t = busDelay;
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
                                                                                //%USEREND HOST_Callback
}                                                                               //%

                                                                                //%USERBEGIN MEMBER_DEFNS
// TODO: Insert definitions of your member functions, etc. here.
void
HOST_IF::HOST_monitor()
{
    uint duration_time = 1000000;
    uint prev_totalReadLength = 0;
    uint prev_totalWriteLength = 0;
    double cur_time = 0;
    double accum_readBandwidth = 0;
    double accum_writeBandwidth = 0;
    double realtime_readBandwidth = 0;
    double realtime_writeBandwidth = 0;
    double start_time = sc_time_stamp().value()/1000;
    ofstream REALTIME_BW("./../results/realtime_bw.txt");

    REALTIME_BW <<"The time stamp is the time measured since the start of the simulation." <<endl;
    REALTIME_BW <<"TimeStamp"<<"\t" <<"accum_readBandwidth" <<"\t"<< "accum_writeBandwidth" <<"\t" <<"realtime_readBandwidth" <<"\t"<< "realtime_writeBandwidth" <<endl;
    while(1){
        cur_time = sc_time_stamp().value()/1000 - start_time;
        accum_readBandwidth = (double)((double)totalReadLength * 512 /1024/1024) / ((cur_time)/1000000000);
        accum_writeBandwidth = (double)((double)totalWriteLength * 512 /1024/1024) / ((cur_time)/1000000000.0);
        realtime_readBandwidth = (double)((double)(totalReadLength - prev_totalReadLength) * 512 /1024/1024) /((double)(duration_time)/1000000000);
        realtime_writeBandwidth= (double)((double)(totalWriteLength - prev_totalWriteLength) * 512 /1024/1024) /((double)(duration_time)/1000000000);
        prev_totalReadLength = totalReadLength;
        prev_totalWriteLength = totalWriteLength;
        REALTIME_BW <<cur_time<<"\t" <<accum_readBandwidth <<"\t"<< accum_writeBandwidth <<"\t" <<realtime_readBandwidth <<"\t"<< realtime_writeBandwidth <<endl;

        wait(duration_time, SC_NS);
    }
}    
void
HOST_IF::HOST_Interface()
{
	if ( dbgFlag[PORTS] ) DBG_MSG( "HOST_Interface invoked because of e_TrigHost event" );
    
    uint    i;
    uint    repeatCount;
    uint    op;
    uint64  addr;
    uint    len;
    double opTime = 0;
    double startTime = 0;
    
    cout << "--------------------------------------------------------------" << endl;
    cout << "---------------------- Simulation Start ----------------------" << endl;
    
    long long initialize_start_time = current_timestamp();


    while(1){

        while((g_isFtlReady == false) ||(g_isCacheReady == false )){
            wait(1000,SC_NS);
        }


        cout << "---------------------- Simulation Initialize ----------------------" << endl;
        while(!feof(traceFile)){
            
            fscanf(traceFile, "%lf\t%llu\t%u\t%u\n",&opTime,&addr, &len, &op);
            
            //align address
            addr %= MAX_ADDR;

            if(addr+len > MAX_ADDR)
            {
                addr = (addr + len) % MAX_ADDR;
            }

            if((operationCount % 100) == 0){
                cout << "[" << sc_core::sc_time_stamp() << "] " << __FILE__ << ":" << __LINE__ << " " << "Operation Count =  "<< operationCount << endl;
            }
            operationCount++;
                        
            //////align request////////////
            ABORT(len == 0, "Request length is 0");
            if(op == HOST_READ){
                repeatCount = Cal_repeatCount(len);

                do{ 
                    if(Check_Buffer_Full() == true)
                        wait(e_DeleteReqBuffer);
                    uint length = len > max_req_len ?  max_req_len : len;
                    uint address = ((addr + length) > MAX_ADDR) ? ((addr + length) % MAX_ADDR) : addr;

                    Push_Request(address, length, HOST_WRITE);

                    e_TrigReqTrans.notify();

                    addr = addr + max_req_len;
                    len  = len - max_req_len;

                    repeatCount--;

                }while(repeatCount > 0);
            }

            
        }

        Trace_File_Close();
        initFile = fopen("./../trace/init_trace.txt","r"); 
        while(!feof(initFile)){
            
            fscanf(initFile, "%lf\t%llu\t%u\t%u\n",&opTime,&addr, &len, &op);
            
            //align address
            addr %= MAX_ADDR;

            if(addr+len > MAX_ADDR)
            {
                addr = (addr + len) % MAX_ADDR;
            }

            if((operationCount % 100) == 0){
                cout << "[" << sc_core::sc_time_stamp() << "] " << __FILE__ << ":" << __LINE__ << " " << "Operation Count =  "<< operationCount << endl;
            }
            operationCount++;
                        
            //////align request////////////
            ABORT(len == 0, "Request length is 0");
            repeatCount = Cal_repeatCount(len);

            do{ 
                if(Check_Buffer_Full() == true)
                    wait(e_DeleteReqBuffer);
                uint length = len > max_req_len ?  max_req_len : len;
                uint address = ((addr + length) > MAX_ADDR) ? ((addr + length) % MAX_ADDR) : addr;

                Push_Request(address, length, HOST_WRITE);

                e_TrigReqTrans.notify();

                addr = addr + max_req_len;
                len  = len - max_req_len;

                repeatCount--;

            }while(repeatCount > 0);

            
        }

        fclose(initFile);
        Trace_File_Open();
        //wait(1000000, SC_NS);
        cout << "initialize end, cleaning up..." << endl;
        wait(2000000000, SC_NS);
        g_initialize_end = true; 
        cout << "---------------------- Simulation Initialize End ----------------------" << endl;
        
        e_TrigMonitor.notify();
        //initialize
        long long initialize_duration = current_timestamp() - initialize_start_time;
        long long simulation_start_time = current_timestamp(); 
        
        requestId = 0;
        startTime = sc_time_stamp().value()/1000.0;
        SSDmetric::g_startTime = startTime;
        operationCount = 0;

        
        while(!feof(traceFile)){
            fscanf(traceFile, "%lf\t%llu\t%u\t%u\n",&opTime,&addr, &len, &op);

            wait(opTime, SC_NS);
            
            //align address
            addr %= MAX_ADDR;
            if(addr+len > MAX_ADDR)
            {
                addr = (addr + len) % MAX_ADDR;
            }

            //for debug
            if((operationCount % 100) == 0){
                cout << "[" << sc_core::sc_time_stamp() << "] " << __FILE__ << ":" << __LINE__ << " " << "Operation Count =  "<< operationCount << endl;
            }

            testTrack.insertID(requestId, addr, len, (uint)op);
            testTrack.start(requestId, addr, len, SSDmetric::HOST_IF, sc_time_stamp().value()/1000);

            operationCount++;

                              //%           //for result
            if( op == HOST_READ){ 
                latency_list[operationCount] -= sc_time_stamp().value()/1000.0; 
            }

            //////align request////////////
            ABORT(len == 0, "Request length is 0");
            repeatCount = Cal_repeatCount(len);
            do{

                if(Check_Buffer_Full() == true)
                    wait(e_DeleteReqBuffer);

                //testTrack.track(requestId, addr, len, SSDmetric::HOST_IF, sc_time_stamp().value()/1000);//track1
                
                uint length = len > max_req_len ?  max_req_len : len;
                uint address = ((addr + length) > MAX_ADDR) ? ((addr + length) % MAX_ADDR) : addr;

                if(HOSTIF_DEBUG) cout << DEV_AND_TIME <<  "[START] req #:  " << operationCount << " repeatCount : "<< repeatCount <<  " op: " << op << "  addr: " << address << "  length: " << length <<"   "  <<repeatCount <<endl;

                if(op == HOST_WRITE){
                    totalWriteLength += length;
                    totalWriteCount++;
                }else{
                    totalReadLength += length;
                    totalReadCount++;
                }
                Push_Request(address, length, op);

                wait(CLOCK_PERIOD, SC_NS);
                e_TrigReqTrans.notify();

                addr = addr + max_req_len;
                len  = len - max_req_len;

                repeatCount--;

            }while(repeatCount > 0);


           testTrack.IDFinish();
        }

        while(1){
            if(Check_Simulation_End() == true){

                testTrack.IDFinish();
                
                
                
                long long simulation_elapsed_time = current_timestamp() - simulation_start_time;
                //time(&simulation_end_timer);                
                double end_time = sc_time_stamp().value()/1000;
                double total_time = sc_time_stamp().value()/1000  - startTime;
               
                cout << "Simulation has ended, cleaning up..." << endl;
                
                for(int i=0; i<100000; i++) {
                    wait(10000, SC_NS);

                    testTrack.IDFinish();
                }

//                ofstream temp_debug("./../results/test_result_debug.txt");
//                testTrack.IDForceFinish(temp_debug);

                cout << "Start time = " << startTime;
                cout << "   End time = " << end_time << endl;
                cout << "Total time = " << total_time << endl;
                cout << "--------------------------------------------------------------" << endl;
                cout << "---------------------- Simulation End ------------------------" << endl;
                cout << endl;
                
                cout << "###################### Simulation Result #####################" << endl << endl;
               

                cout << "Time Elapsed in Initialization = " << initialize_duration/1000.0 << " s" << endl;//difftime(simulation_end_timer, simulation_start_timer ) << endl;
                cout << "Time Elapsed in Simulation = " << simulation_elapsed_time/1000.0 << " s" << endl;//difftime(simulation_end_timer, simulation_start_timer i << endl;
                cout << "Average Time Elapsed per Operation = " << simulation_elapsed_time/(double)operationCount << " ms" << endl<<endl;//difftime(simulation_end_timer, simulation_start_timer ) << endl;
                cout << "Start time = " << startTime<<" ns" << endl;
                cout << "End time = " << sc_time_stamp().value()/1000 << " ns"<<endl;
                cout << "Total time = " << sc_time_stamp().value()/1000  - startTime<<" ns"<< endl;
                cout << endl;

                double readBandwidth = (double)((double)totalReadLength * 512 /1024/1024) / ((total_time)/1000000000);
                double writeBandwidth = (double)((double)totalWriteLength * 512 /1024/1024) / ((total_time)/1000000000.0);
                double readIOPS = ((double)totalReadCount) / ((total_time)/1000000000.0);
                double writeIOPS = ((double)totalWriteCount) / ((total_time)/1000000000.0);
                cout << "Write  : Bandwidth = " << writeBandwidth << " MB/s " << "  IOPS = " << writeIOPS << endl;
                cout << "Read  : Bandwidth = " << readBandwidth << " MB/s " << "  IOPS = " << readIOPS << endl;
                cout << "Read latency : mean - " << calculateAvgLatency(latency_list) << " ns\tmax - " << calculateMaxLatency(latency_list) << " ns" << endl;

                // ympark0225
                double meta_readBandwidth = (double)((double)(DRAM_STATS[TOTAL_LEN][1][1]) /1024/1024) / ((total_time)/1000000000);
                double meta_writeBandwidth = (double)((double)(DRAM_STATS[TOTAL_LEN][1][0]) /1024/1024) / ((total_time)/1000000000);
                double data_readBandwidth = (double)((double)(DRAM_STATS[TOTAL_LEN][0][1]) /1024/1024) / ((total_time)/1000000000);
                double data_writeBandwidth = (double)((double)(DRAM_STATS[TOTAL_LEN][0][0]) /1024/1024) / ((total_time)/1000000000);
                double cache_readBandwidth = (double)((double)(DRAM_STATS[TOTAL_LEN][0][1]+DRAM_STATS[TOTAL_LEN][1][1]) /1024/1024) / ((total_time)/1000000000);
                double cache_writeBandwidth = (double)((double)(DRAM_STATS[TOTAL_LEN][0][0]+DRAM_STATS[TOTAL_LEN][1][0]) /1024/1024) / ((total_time)/1000000000);
                cout << "Metadata  Bandwidth  : write =" << meta_writeBandwidth << " MB/s"<<"\t"<< ",Read = "<< meta_readBandwidth << " MB/s"<<endl;
                cout << "databuffer  Bandwidth  : write =" << data_writeBandwidth << " MB/s"<<"\t"<< ",Read = "<< data_readBandwidth << " MB/s"<<endl;
                cout << "Cache  Bandwidth  : write =" << cache_writeBandwidth << " MB/s"<<"\t"<< ",Read = "<< cache_readBandwidth << " MB/s"<<endl;

                cout << "Bus times : CMD_BUS - " << calculateBusTime(CMD_BUS_TIME_CONSUMED) << " ns\t DATA_BUS - " << calculateBusTime(DATA_BUS_TIME_CONSUMED) << " ns\t Ratio(CMD/DATA) - "<< calculateBusTime(CMD_BUS_TIME_CONSUMED)/calculateBusTime(DATA_BUS_TIME_CONSUMED) << " : 1"  << endl;

                //cout << "Bus times(SubReqMan) : CMD_BUS - " << CMD_BUS_TIME_CONSUMED[0] << " ns\t DATA_BUS - " << DATA_BUS_TIME_CONSUMED[0] << " ns" << endl;

                //cout << "Bus times(DRAM_Ctrl) : CMD_BUS - " << CMD_BUS_TIME_CONSUMED[1] << " ns\t DATA_BUS - " << DATA_BUS_TIME_CONSUMED[1] << " ns" << endl;
                //cout << "Bus times(NAND_Manager) : CMD_BUS - " << CMD_BUS_TIME_CONSUMED[2] << " ns\t DATA_BUS - " << DATA_BUS_TIME_CONSUMED[2] << " ns" << endl;
                
                
                
                //cout << "Power Consumption : Cache - " << calculateDRAMEnergy(DRAM_STATS, total_time)/1000.0  << " mW\tNAND - " << calculateNANDEnergy()/total_time*1000000 << " mW" << endl<<endl;
                cout << "Power Consumption : Cache DRAM - " << calculateDRAMEnergy(DRAM_STATS, total_time)/1000.0  << " mW " <<endl;
                cout << "Power Consumption : Cache PRAM - " << calculatePRAMEnergy(DRAM_STATS, total_time)/1000.0  << " mW " <<endl;
                cout<< "Power Consumption : NAND - " << calculateNANDEnergy()/total_time*1000000 << " mW" << endl<<endl;

                cout << "Cache Stats : " << endl << "\tCache buffer usage... write -  " << DRAM_STATS[TOTAL_LEN][0][0] << "  read - " <<  DRAM_STATS[TOTAL_LEN][0][1] << endl << "\tmetadata usage... write - " << DRAM_STATS[TOTAL_LEN][1][0] << "  read - " << DRAM_STATS[TOTAL_LEN][1][1] << endl;
                cout << "###################### Simulation Result END #################" << endl << endl << endl;

                cout << "Software time .... CPU1 : " << SOFTWARE_TIME[0] << "  count : " << SOFTWARE_count << " average : " << SOFTWARE_TIME[0]/SOFTWARE_count <<  endl;

                cout << "" <<endl;

                ofstream SIMUL_RESULT("./../results/simul_result.txt");

                SIMUL_RESULT << "Time Elapsed in Initialization = " << initialize_duration/1000.0 << endl;//difftime(simulation_end_timer, simulation_start_timer ) << endl;
                SIMUL_RESULT << "Time Elapsed in Simulation = " << simulation_elapsed_time/1000.0 << endl;//difftime(simulation_end_timer, simulation_start_timer i << endl;
                SIMUL_RESULT << "Average Time Elapsed per Operation = " << simulation_elapsed_time/(double)operationCount << endl<<endl;//difftime(simulation_end_timer, simulation_start_timer ) << endl;
                SIMUL_RESULT << "Start time = " << startTime << endl;
                SIMUL_RESULT << "End time = " << sc_time_stamp().value()/1000 <<endl;
                SIMUL_RESULT << "Total time = " << sc_time_stamp().value()/1000  - startTime<< endl;

                SIMUL_RESULT << "Write  : Bandwidth = " << writeBandwidth <<  endl;
                SIMUL_RESULT << "Write  : IOPS = " << writeIOPS << endl;
                SIMUL_RESULT << "Read  : Bandwidth = " << readBandwidth << endl;
                SIMUL_RESULT << "Read  : IOPS = " << readIOPS << endl;
                SIMUL_RESULT << "Read latency : mean = " << calculateAvgLatency(latency_list) <<  endl;
                SIMUL_RESULT << "Read latency : max = " << calculateMaxLatency(latency_list) <<  endl;
                SIMUL_RESULT << "Metadata  Bandwidth  : write = " << meta_writeBandwidth <<endl;
                SIMUL_RESULT << "Metadata  Bandwidth  : Read = "<< meta_readBandwidth << endl;
                SIMUL_RESULT << "databuffer  Bandwidth  : write =" << data_writeBandwidth << endl;
                SIMUL_RESULT << "databuffer  Bandwidth  : Read = "<< data_readBandwidth <<endl;
                SIMUL_RESULT << "Cache  Bandwidth  : write =" << cache_writeBandwidth << endl;
                SIMUL_RESULT << "Cache  Bandwidth  : Read =" << cache_readBandwidth <<endl;

                SIMUL_RESULT << "Bus times : CMD_BUS = " << calculateBusTime(CMD_BUS_TIME_CONSUMED) <<  endl;
                SIMUL_RESULT << "Bus times : DATA_BUS = " << calculateBusTime(DATA_BUS_TIME_CONSUMED) << endl;
                SIMUL_RESULT << "Bus times : Ratio(CMD/DATA) = "<< calculateBusTime(CMD_BUS_TIME_CONSUMED)/calculateBusTime(DATA_BUS_TIME_CONSUMED)  << endl;

                //SIMUL_RESULT << "Bus times(SubReqMan) : CMD_BUS - " << CMD_BUS_TIME_CONSUMED[0] << " ns\t DATA_BUS - " << DATA_BUS_TIME_CONSUMED[0] << " ns" << endl;

                //SIMUL_RESULT << "Bus times(DRAM_Ctrl) : CMD_BUS - " << CMD_BUS_TIME_CONSUMED[1] << " ns\t DATA_BUS - " << DATA_BUS_TIME_CONSUMED[1] << " ns" << endl;
                //SIMUL_RESULT << "Bus times(NAND_Manager) : CMD_BUS - " << CMD_BUS_TIME_CONSUMED[2] << " ns\t DATA_BUS - " << DATA_BUS_TIME_CONSUMED[2] << " ns" << endl;
                
                
                
                //SIMUL_RESULT << "Power Consumption : Cache - " << calculateDRAMEnergy(DRAM_STATS, total_time)/1000.0  << " mW\tNAND - " << calculateNANDEnergy()/total_time*1000000 << " mW" << endl<<endl;
                SIMUL_RESULT << "Power Consumption : Cache DRAM = " << calculateDRAMEnergy(DRAM_STATS, total_time)/1000.0  <<endl;
                SIMUL_RESULT << "Power Consumption : Cache PRAM = " << calculatePRAMEnergy(DRAM_STATS, total_time)/1000.0  <<endl;
                SIMUL_RESULT << "Power Consumption : NAND = " << calculateNANDEnergy()/total_time*1000000 <<endl;
                SIMUL_RESULT << "Cache buffer usage : write =  " << DRAM_STATS[TOTAL_LEN][0][0] << endl;
                SIMUL_RESULT << "Cache buffer usage : read = " <<  DRAM_STATS[TOTAL_LEN][0][1] << endl; 
                SIMUL_RESULT << "metadata usage : write = " << DRAM_STATS[TOTAL_LEN][1][0] << endl;
                SIMUL_RESULT << "metadata usage : read = " << DRAM_STATS[TOTAL_LEN][1][1] << endl;


                ofstream ANALYSIS_FILE("./../results/analysis.txt");
                ofstream NAND_STATS_FILE("./../results/nand_stats.txt");
                ofstream DRAM_STATS_FILE("./../results/dram_stats.txt");
               

                ANALYSIS_FILE << "Software time .... CPU1 : " << SOFTWARE_TIME[0] << "  count : " << SOFTWARE_count << " average : " << SOFTWARE_TIME[0]/SOFTWARE_count <<  endl;



                NAND_STATS_FILE << "Channel\tWay\t\tNothing\tProgram\tRead\tErase\tCopyback\tCopyback_Program\tCopyback_Read" << endl;


                for(int i = 0; i < 4; i++){
                    
                    for(int j = 0; j < 4; j++){
                        NAND_STATS_FILE <<  i << "\t" << j << "\t\t";
                        for(int h =0; h<7; h++){
                            int sum = 0;
                            
                            for(int k = 0; k< 1024; k++){
                                sum += NAND_STATS[h][i][j][k];
                            }


                            NAND_STATS_FILE << sum << "\t";    
                        }
                        NAND_STATS_FILE << endl;
                    }
                    NAND_STATS_FILE << endl;

                }

                DRAM_STATS_FILE << ".\tWrite\tRead" << endl;
                
                DRAM_STATS_FILE << "CacheBuffer\t\t";

                for(int j = 0; j<2; j++){

                    DRAM_STATS_FILE << DRAM_STATS[0][j] << "\t";
                }


                DRAM_STATS_FILE << endl;
                
                
                DRAM_STATS_FILE << "Metadata\t\t";

                for(int j = 0; j<2; j++){

                    DRAM_STATS_FILE << DRAM_STATS[1][j] << "\t";
                }


                DRAM_STATS_FILE << endl;
                
                
                //view_latency_list(latency_list); 
                //cout << "Write : Bandwidth = " << writeBandwidth << " MB/s " << "  IOPS = " << writeIOPS << endl;

                sc_stop();

            }
            wait(1000, SC_NS);
        }

        wait();
    }
}

void
HOST_IF::HOST_To_Device_Req_Trans()
{
    Req_t tmpReq;

    while(1){
                    
        if(Check_Buffer_Empty() == false){

            while(transDataFlag == true){
                e_TransReqDone.notify();
                wait(e_TransDataDone);
            }

            transReqFlag = true;
            tmpReq = requestBuffer[reqBufferPoint];

            //if(g_initialize_end) testTrack.track(tmpReq.iId, tmpReq.iAddr, tmpReq.iLen, SSDmetric::HOST_IF, sc_time_stamp().value()/1000.0);//track 
            //wait((double)sizeof(Req_t)/(double)IF_BANDWIDTH * 1000, SC_NS);
            //cout <<"ID : " << tmpReq.iId <<"  addr: " << tmpReq.iAddr << "  length: " << tmpReq.iLen <<endl;
            wait((double)16.0/(double)IF_BANDWIDTH * 1000, SC_NS);

            if(!HOST_Master.write(REQUEST_ADDR, (void *)&tmpReq, sizeof(Req_t))){
                wait(100, SC_NS);
                //sc_stop();
            }else{
                reqBufferPoint = (reqBufferPoint + 1) % Q_SIZE;
            }
            //if( (tmpReq.Op == HOST_READ) && g_initialize_end) 
                //testTrack.track(tmpReq.iId, tmpReq.iAddr, tmpReq.iLen, SSDmetric::HOST_IF, sc_time_stamp().value()/1000.0);//track3 (read)

            transReqFlag = false;
            if(transDataFlag == true){
                e_TransReqDone.notify();
                if(HOSTIF_DEBUG) cout << DEV_AND_TIME << "[HOST_To_Device_Req_Trans]HOST send the request to Device IF"<<endl;
            }
        }

        if(Check_Buffer_Empty() == true){
            wait();
        }
    }
}

void
HOST_IF::HOST_To_Device_Data_Trans()
{
    Req_t tmpReq;

    while(1){
        if(dbgFlag[GENERAL])
            cout << "<1> HOST to Device write ID1   "<< transReq.iId<<endl;

        if(HOSTIF_DEBUG) cout << DEV_AND_TIME << "[HOST_To_Device_Data_Trans] ID : " << transReq.iId << endl;

        transDataFlag = true;
        while(transReqFlag == true){
            wait(e_TransReqDone);
        }

        if(HOSTIF_DEBUG)cout << DEV_AND_TIME << "[HOST_To_Device_Data_Trans] transReqDone invoked " << endl;

        //cout << "trand id = " << transReq.iId << endl;
        wait(((double)transReq.iLen * (double)UNIT_OF_REQUEST / (double)IF_BANDWIDTH) * 1000.0 , SC_NS);

        uint transDataPoint = Find_Request(transReq.iId);

        if(!HOST_Master.write(DATA_ADDR, (void *)requestData[transDataPoint], (unsigned int)(transReq.iLen))){
            ABORT(1, "Data trans fail");
        }

        if(g_initialize_end && (transReq.Op == HOST_WRITE)){
            testTrack.done(transReq.iId, transReq.iAddr, transReq.iLen, SSDmetric::HOST_IF, sc_time_stamp().value()/1000.0);//skhds
        }


        Delete_Request(transReq.iId);
        transDataFlag = false;
        e_TransDataDone.notify();

        wait();
    }
}


void
HOST_IF::Delete_Request(uint point)
{
    bool findFlag = false;
    uint deletePoint;
    
    deletePoint = Find_Request(point);
    if(HOSTIF_DEBUG) cout << DEV_AND_TIME << "[Delete_Request] point : " << deletePoint << endl;
    memset(&requestBuffer[deletePoint], 0x0, sizeof(Req_t));

    //cout<<"1"<<endl;
    memset(&requestData[deletePoint], 0x0, max_req_len*UNIT_OF_REQUEST);
    
    //cout<<"2"<<endl;
    //move request
    for(uint j=deletePoint; j != reqBufferHead; j = (j+Q_SIZE-1) %Q_SIZE){
        memcpy(&requestBuffer[j], &requestBuffer[(j+Q_SIZE-1)%Q_SIZE], sizeof(Req_t));
        memcpy(&requestData[j], &requestData[(j+Q_SIZE-1)%Q_SIZE], max_req_len*UNIT_OF_REQUEST);
    }

    //cout<<"3"<<endl;
    reqBufferHead = (reqBufferHead + 1) % Q_SIZE;

    //cout<<"4"<<endl;
    //cout <<"delet point = " << deletePoint<<endl;
    e_DeleteReqBuffer.notify();
    e_TrigReqTrans.notify();
}

uint
HOST_IF::Find_Request(uint Id)
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
        cout << "received ID : " << Id << endl;
        ABORT(1," Could not find received request!!");
    }

    return findPoint;
}


void 
HOST_IF::Push_Request(int addr, uint len, uint op)
{
    ABORT(Check_Buffer_Full() == true, "Request Queue don't should be full");
    
    requestBuffer[reqBufferTail].iId        = requestId++; 
    requestBuffer[reqBufferTail].iAddr      = addr;        
    requestBuffer[reqBufferTail].iLen       = len;        
    requestBuffer[reqBufferTail].dIssueTime =(double)sc_time_stamp().value()/1000; //nano second

    
    if(op == HOST_WRITE){
        requestBuffer[reqBufferTail].Op = HOST_WRITE;        
        //create data
        Create_Data(len);

        if(COMPARISON)
            Write_Data_Map(addr, len);            

    }else if(op == HOST_READ){
        requestBuffer[reqBufferTail].Op = HOST_READ;
        //memset
        memset(requestData[reqBufferTail], 0x00, UNIT_OF_REQUEST*max_req_len);
    }

    reqBufferTail = (reqBufferTail + 1) % Q_SIZE;
    
}

void
HOST_IF::Create_Data(uint64 len)
{
    for(uint i=0; i < len*UNIT_OF_REQUEST; i++){
        requestData[reqBufferTail][i] = 'A' + rand()%26;
    }
    

}


void
HOST_IF::Compare_Data()
{
    uint addr = recievedReq.iAddr;
    uint len  = recievedReq.iLen;
    bool passFlag = false;
    
//    for(uint j=reqBufferHead; j != reqBufferPoint; j = (j+1) % Q_SIZE){
//        if((requestBuffer[j].iAddr <= addr && requestBuffer[j].iAddr + requestBuffer[j].iLen >= addr)||(requestBuffer[j].iAddr <= addr+len && requestBuffer[j].iAddr + requestBuffer[j].iLen >= addr+len)){
//            passFlag=true;
//            break;
//        }
//    }

    if(passFlag == true){
//        for(uint i=0;i < len*UNIT_OF_REQUEST;i++){
//
//            if(data_map[addr*512+i] != recievedData[i]){
//
//                if(dbgFlag[GENERAL]){
//                    cout << "Recieved data error!! addr  : " << addr << "  Length  :  " << len<< "  ADDR : " << addr+i/512 << endl;
//                    cout << "Original Data  : "<<data_map[addr*512+i] << "   reciedved Data  : " << recievedData[i] << endl;  
//                }
//                wait(SC_ZERO_TIME);
//                sc_stop();
//
//            }
//        }
    }

    if(dbgFlag[GENERAL]){
        cout << "Complete Reqeust ID : " << recievedReq.iId << endl;
    }
}


void
HOST_IF::Write_Data_Map(uint addr, uint64 len)
{
    for(uint i=0; i < len*UNIT_OF_REQUEST;i++){
        //data_map[addr*UNIT_OF_REQUEST+i] = requestData[reqBufferTail][i];
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
#endif // __HOST_IF_H__                                                         //%
