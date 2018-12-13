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
#include "./header/global_header.h"
#include "./header/header_HOSTIF.h"


#include <fstream>
#include <sys/time.h>

//for data comparison
//char data_map[MEMORY_SIZE];

char requestData[Q_SIZE][MAX_REQ_LEN*SECTOR_BYTES];

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
    sc_core::sc_in<bool> HOST_RnB_Slave;                                                                            //%
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
   
    Req_t recievedReq;
    char recievedData[MAX_REQ_LEN*SECTOR_BYTES];

    Req_t transReq;


    //Flag
    bool transDataFlag;
    bool transReqFlag;
    uint my_state;
    uint next_state;


    // Event & semaphore
    sc_core::sc_event e_TrigHost;
    sc_core::sc_event e_DeleteReqBuffer;
    sc_core::sc_event e_TrigReqTrans;
    sc_core::sc_event e_TrigDataTrans;
    sc_core::sc_semaphore sem_Trans;
    sc_core::sc_event e_TransDataDone;
    sc_core::sc_event e_TransReqDone;


    inline uint Cal_repeatCount(uint length){
        return ((length - 1) / MAX_REQ_LEN) + 1;
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




    void Delete_Request(uint id);
    void Push_Request(int addr, uint len, uint op);
    void Create_Data(uint64 len);
    uint Find_Request(uint id);
    void insert_trace(FILE* fp, char* filename, SIMUL_MODE mode);

    double calculateBusTime(double* bus_time){
        
        double sum = 0;
        for(int i=0; i<3; i++){

            sum += bus_time[i];

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
 HOST_Slave("HOST_Slave"),
 HOST_RnB_Slave("HOST_RnB_Slave"), //%
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
    if (wid < len) { // Target unable to s  upport streaming width attribute
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
                    //memcpy(&transReq, R_HOST_req, len);
                    next_state = IDLE;
    
                    //if(HOSTIF_DEBUG) cout << DEV_AND_TIME << "[HOST_Callback] received write request, ID : " << transReq.iId << endl;
                    //e_TrigDataTrans.notify();
                }
                dBusDelay = (double)16.0/ (double)IF_BANDWIDTH * 1000;
                break;

            case CMD:

                if(HOSTIF_DEBUG) cout << DEV_AND_TIME << "[HOST_Callback] state is : CMD" << endl;
                next_state = IDLE;
                memcpy(R_HOST_data, ptr, len);
                memcpy(&recievedData, R_HOST_data, len);


                if(HOSTIF_DEBUG) cout << DEV_AND_TIME << "[HOST_Callback] id : " << recievedReq.iId << "  len :  " << recievedReq.iLen << endl;
                dBusDelay = ((double)recievedReq.iLen * (double)SECTOR_BYTES / (double)IF_BANDWIDTH) * 1000.0;
                
                if(g_initialize_end){
                    testTrack.done(recievedReq.iId, recievedReq.iAddr, recievedReq.iLen, SSDmetric::HOST_IF, sc_time_stamp().value()/1000.0); //skhds
                }
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
    FILE* traceFile; 
    cout << "--------------------------------------------------------------" << endl;
    cout << "---------------------- Simulation Start ----------------------" << endl;
    
    long long initialize_start_time = current_timestamp();


    while(1){

        while((g_isFtlReady == false) ||(g_isCacheReady == false )){
            wait(1000,SC_NS);
        }


        cout << "---------------------- Simulation Initialize ----------------------" << endl;
        
        insert_trace(traceFile, "./../trace/ex_trace.txt", SIMUL_RINIT);
#ifdef G_INITIALIZE
        insert_trace(traceFile, "./../trace/init_trace.txt", SIMUL_DINIT);
#endif
        
        
        cout << "initialize end, cleaning up..." << endl;
        wait(2000000000, SC_NS);
        g_initialize_end = true; 
        cout << "---------------------- Simulation Initialize End ----------------------" << endl;
        
        
        //initialize
        long long initialize_duration = current_timestamp() - initialize_start_time;
        long long simulation_start_time = current_timestamp(); 
        
        requestId = 0;
        startTime = sc_time_stamp().value()/1000.0;
        SSDmetric::g_startTime = startTime;
        operationCount = 0;

        insert_trace(traceFile, "./../trace/ex_trace.txt", SIMUL_ON);
        
        
        
        while(1){
            if(Check_Simulation_End() == true){

                testTrack.IDFinish();
                
                long long simulation_elapsed_time = current_timestamp() - simulation_start_time;
                double end_time = sc_time_stamp().value()/1000;
                double total_time = sc_time_stamp().value()/1000  - startTime;
               
                cout << "Simulation has ended, cleaning up..." << endl;
                
                for(int i=0; i<100000; i++) {
                    wait(10000, SC_NS);

                    testTrack.IDFinish();
                }

#ifdef DATA_COMPARE_ON
                  assert(DTCMP::confirm_data());
#endif


                cout << "Start time = " << startTime;
                cout << "   End time = " << end_time << endl;
                cout << "Total time = " << total_time << endl;
                cout << "--------------------------------------------------------------" << endl;
                cout << "---------------------- Simulation End ------------------------" << endl;
                cout << endl;
                
                cout << "###################### Simulation Result #####################" << endl << endl;
               

                cout << "Time Elapsed in Initialization = " << initialize_duration/1000.0 << " s" << endl; 
                cout << "Time Elapsed in Simulation = " << simulation_elapsed_time/1000.0 << " s" << endl;
                cout << "Average Time Elapsed per Operation = " << simulation_elapsed_time/(double)operationCount << " ms" << endl<<endl;
                cout << endl;

                

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
            memcpy(&transReq, &tmpReq, sizeof(Req_t));
            transReqFlag = false;
            e_TrigDataTrans.notify();
            if(transDataFlag == true){
                e_TransReqDone.notify();
                if(HOSTIF_DEBUG) cout << DEV_AND_TIME << "[HOST_To_Device_Req_Trans]HOST send the request to Device IF"<<endl;
            }
            else{
                transDataFlag = true;
            }
            wait(SC_ZERO_TIME);
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
        
        if(transDataFlag == false ) wait();
        if(dbgFlag[GENERAL])
            cout << "<1> HOST to Device write ID1   "<< transReq.iId<<endl;

        if(HOSTIF_DEBUG) cout << DEV_AND_TIME << "[HOST_To_Device_Data_Trans] ID : " << transReq.iId << endl;

        //transDataFlag = true;
        while(transReqFlag == true){
            wait(e_TransReqDone);
        }

        if(HOSTIF_DEBUG)cout << DEV_AND_TIME << "[HOST_To_Device_Data_Trans] transReqDone invoked " << endl;

        //cout << "trand id = " << transReq.iId << endl;
        //modified by skhds
        //HOST now sends one page (16KB) at a time

        int writeCount = 0; //# of page sends
        int sectorCount = 0;
        int remaining_len = transReq.iLen;
        int start_addr = transReq.iAddr;
        int tmp_offset; 
        uint transDataPoint = Find_Request(transReq.iId);
       /// do my own shit here

        do{
            tmp_offset = start_addr % SECTOR_PER_PAGE + remaining_len - SECTOR_PER_PAGE;

            if(tmp_offset > 0) {
                sectorCount = SECTOR_PER_PAGE - start_addr % SECTOR_PER_PAGE;
                remaining_len -= sectorCount;
                start_addr += sectorCount;
            }
            else{
                sectorCount = remaining_len; 
                remaining_len = 0;
            }
            
            while(HOST_RnB_Slave.read() == 0x0){
                 wait(HOST_RnB_Slave.posedge_event());
            }
            wait(((double)sectorCount * (double)SECTOR_BYTES / (double)IF_BANDWIDTH) * 1000.0 , SC_NS);

            if(!HOST_Master.write(DATA_ADDR, (void *)(requestData[transDataPoint] + (transReq.iLen - sectorCount - remaining_len)*SECTOR_BYTES ), (unsigned int)(sectorCount*SECTOR_BYTES))){
                ABORT(1, "Data trans fail");
            }
        
        
        }while(tmp_offset > 0);

       ///


        if(g_initialize_end && (transReq.Op == HOST_WRITE)){
            testTrack.done(transReq.iId, transReq.iAddr, transReq.iLen, SSDmetric::HOST_IF, sc_time_stamp().value()/1000.0);//skhds
        }

        Delete_Request(transReq.iId);
        transDataFlag = false;
        e_TransDataDone.notify();

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
    memset(&requestData[deletePoint], 0x0, MAX_REQ_LEN*SECTOR_BYTES);
    
    //cout<<"2"<<endl;
    //move request
    for(uint j=deletePoint; j != reqBufferHead; j = (j+Q_SIZE-1) %Q_SIZE){
        memcpy(&requestBuffer[j], &requestBuffer[(j+Q_SIZE-1)%Q_SIZE], sizeof(Req_t));
        memcpy(&requestData[j], &requestData[(j+Q_SIZE-1)%Q_SIZE], MAX_REQ_LEN*SECTOR_BYTES);
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
        
#ifdef DATA_COMPARE_ON

        DTCMP::writeData(DTCMP::mmLogical, addr, (uchar*)requestData[reqBufferTail], len);
#endif

    }else if(op == HOST_READ){
        requestBuffer[reqBufferTail].Op = HOST_READ;
        //memset
        memset(requestData[reqBufferTail], 0x00, SECTOR_BYTES*MAX_REQ_LEN);
    }

    reqBufferTail = (reqBufferTail + 1) % Q_SIZE;
    
}

void
HOST_IF::Create_Data(uint64 len)
{
    for(uint i=0; i < len*SECTOR_BYTES; i++){
        requestData[reqBufferTail][i] = 'A' + rand()%26;
    }
    

}
            




void
HOST_IF::insert_trace(FILE* fp, char* filename, SIMUL_MODE mode){

        double opTime;
        uint64 addr;
        uint len;
        uint op;
        uint repeatCount;
    //file open
        fp = fopen(filename, "r");
        
        
        while(!feof(fp)){
            
            fscanf(fp, "%lf\t%llu\t%u\t%u\n",&opTime,&addr, &len, &op);
            
            //align address
            addr %= MAX_ADDR;

            if(addr+len > MAX_ADDR)
            {
                addr = (addr + len) % MAX_ADDR;
            }

            if((operationCount % 100) == 0){
                cout << "[" << sc_core::sc_time_stamp() << "] " << __FILE__ << ":" << __LINE__ << " " << "Operation Count =  "<< operationCount << endl;
            }

            if (mode == SIMUL_ON) {
                testTrack.insertID(requestId, addr, len, (uint)op);
                testTrack.start(requestId, addr, len, SSDmetric::HOST_IF, sc_time_stamp().value()/1000);
            }

            operationCount++;
                        
            //////align request////////////
            ABORT(len == 0, "Request length is 0");
            
            if ( (mode != SIMUL_RINIT) || (op == HOST_READ)){
                repeatCount = Cal_repeatCount(len);

                do{ 
                    if(Check_Buffer_Full() == true)
                        wait(e_DeleteReqBuffer);
                    uint length = len > MAX_REQ_LEN ?  MAX_REQ_LEN : len;
                    uint address = ((addr + length) > MAX_ADDR) ? ((addr + length) % MAX_ADDR) : addr;

                    if(mode == SIMUL_RINIT) Push_Request(address, length, HOST_WRITE);
                    else Push_Request(address, length, op);
                    e_TrigReqTrans.notify();

                    addr = addr + MAX_REQ_LEN;
                    len  = len - MAX_REQ_LEN;

                    repeatCount--;

                }while(repeatCount > 0);
            }
            if(mode == SIMUL_ON) testTrack.IDFinish();
            
        }

        //file close
        fclose(fp);
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
