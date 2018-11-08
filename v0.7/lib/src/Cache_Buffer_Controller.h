////--------------------------------------------------------------                //%
// Important:  The template for this file was generated                         //%
//  automatically and can only be re-generated if rules                         //%
//  are followed when editing:                                                  //%
// 1. Do not modify any line containing //%                                     //%
// 2. Only insert your lines of code between 2 lines                            //%
//    containing "//% user begin" and "//% user end"                            //%
//                                                                              //%
// :generate:date: Thu Apr 17 2014 14:08:01                                     //%
// :generate:version: 2010.1.0-dev                                              //%
//--------------------------------------------------------------                //%
                                                                                //%
#ifndef __CACHE_BUFFER_CONTROLLER_H__                                           //%
#define __CACHE_BUFFER_CONTROLLER_H__                                           //%
                                                                                //%USERBEGIN MODEL_DEBUG_ENABLE
#if defined(DBG_MSG) || defined(DBG_SCP)
# define DBG_SCP_CACHE_BUFFER_CONTROLLER
#endif
                                                                                //%USEREND MODEL_DEBUG_ENABLE
#include <scml2.h>                                                              //%
#include <systemc>                                                              //%
#include "scmlinc/scml_command_processor.h"                                     //%
#include "scmlinc/scml_property.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm.h"
                                                                                //%USERBEGIN HEADER_H
// TODO: Insert your includes, declarations, etc. here.
#include "./header/Cache_Buffer_Struct.h"
#include "./header/ssd_struct.h"
#include "./header/memcpy.h"
#include <math.h>

#include <list>
#include <cmath>

#define Host_Addr       0x0
#define Cache_Addr      0x0
#define NAND_Addr       0x0

#define CACHE_BUFFER_SIZE   1024
#define MAX_QUEUE_SIZE      64

#define Shift_Cache_Size    log(CACHE_BUFFER_SIZE)/log(2)

////////////// TH /////////////////
#define R_CPU_COMPLETE_ADDR 0x0
#define R_CPU_CMD_SIZE 0x10


#define CHANNEL 8
uint64 dram_read_count;
uint64 dram_write_count;
                                                                                //%USEREND HEADER_H
//------------------------                                                      //%
// Module Interface                                                             //%
//------------------------                                                      //%
template < unsigned int NUM_CHANNEL=4 >
class Cache_Buffer_Controller :                                                 //%
	public sc_core::sc_module                                               //%
                                                                                //%USERBEGIN INHERITANCE
    // TODO: Specify class inheritance here. Start each line with a , character
                                                                                //%USEREND INHERITANCE
                                                                                //%
{                                                                               //%
public:                                                                         //%
	// constructor                                                          //%
	Cache_Buffer_Controller( const sc_core::sc_module_name & n);
	SC_HAS_PROCESS( Cache_Buffer_Controller );                              //%
	// destructor                                                           //%
	virtual ~Cache_Buffer_Controller();                                     //%
private:                                                                        //%
	// disable default and copy constructors by making them private         //%
	Cache_Buffer_Controller() ;                                             //%
	Cache_Buffer_Controller( const Cache_Buffer_Controller& ) ;             //%
	Cache_Buffer_Controller & operator= ( const Cache_Buffer_Controller& ) ;//%
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
	scml2::initiator_socket< 32 > Nandside_Mport[NUM_CHANNEL];              //%
	tlm::tlm_target_socket< 32 > Nandside_Sport_0;               //%
	tlm::tlm_target_socket< 32 > Nandside_Sport_1;               //%
	tlm::tlm_target_socket< 32 > Nandside_Sport_2;               //%
	tlm::tlm_target_socket< 32 > Nandside_Sport_3;               //%
	tlm::tlm_target_socket< 32 > Nandside_Sport_4;               //%
	tlm::tlm_target_socket< 32 > Nandside_Sport_5;               //%
	tlm::tlm_target_socket< 32 > Nandside_Sport_6;               //%
	tlm::tlm_target_socket< 32 > Nandside_Sport_7;               //%
	sc_core::sc_in< bool > Nandside_RnB[NUM_CHANNEL];                                    //%
	scml2::initiator_socket< 32 > Hostside_Mport;                           //%
	tlm::tlm_target_socket< 32 > Hostside_Sport;                            //%
	sc_core::sc_out< bool > Hostside_RnB;                                   //%
	scml2::initiator_socket< 32 > Cacheside_Mport;                          //%
	tlm::tlm_target_socket< 32 > SFR_port;                                  //%
	sc_core::sc_out< bool > Interrupt_port;                                 //%
protected:                                                                      //%
                                                                                //%
	// Memory declarations                                                  //%
	scml2::memory< unsigned int > Nandside_Sport_Reg_0;     
	scml2::memory< unsigned int > Nandside_Sport_Reg_1; 
	scml2::memory< unsigned int > Nandside_Sport_Reg_2; 
	scml2::memory< unsigned int > Nandside_Sport_Reg_3; 
	scml2::memory< unsigned int > Nandside_Sport_Reg_4; 
	scml2::memory< unsigned int > Nandside_Sport_Reg_5; 
	scml2::memory< unsigned int > Nandside_Sport_Reg_6; 
	scml2::memory< unsigned int > Nandside_Sport_Reg_7; 
                                                                                //%
	scml2::tlm2_gp_target_adapter< 32 > Nandside_Sport_0_Nandside_Sport_Reg_0_adapter;
	scml2::tlm2_gp_target_adapter< 32 > Nandside_Sport_1_Nandside_Sport_Reg_1_adapter;
	scml2::tlm2_gp_target_adapter< 32 > Nandside_Sport_2_Nandside_Sport_Reg_2_adapter;
	scml2::tlm2_gp_target_adapter< 32 > Nandside_Sport_3_Nandside_Sport_Reg_3_adapter;
	scml2::tlm2_gp_target_adapter< 32 > Nandside_Sport_4_Nandside_Sport_Reg_4_adapter;
	scml2::tlm2_gp_target_adapter< 32 > Nandside_Sport_5_Nandside_Sport_Reg_5_adapter;
	scml2::tlm2_gp_target_adapter< 32 > Nandside_Sport_6_Nandside_Sport_Reg_6_adapter;
	scml2::tlm2_gp_target_adapter< 32 > Nandside_Sport_7_Nandside_Sport_Reg_7_adapter;
	scml2::memory< unsigned int > Hostside_Sport_Reg;                       //%
                                                                                //%
	scml2::tlm2_gp_target_adapter< 32 > Hostside_Sport_Hostside_Sport_Reg_adapter;//%
                                                                                //%
	scml2::memory< unsigned int > SFR_port_Reg;                             //%
        scml2::memory_alias< unsigned int > SFR_port_Reg_complete;
        scml2::memory_alias< unsigned int > SFR_port_Reg_ADDR;
        scml2::memory_alias< unsigned int > SFR_port_Reg_LEN;
        scml2::memory_alias< unsigned int > SFR_port_Reg_OP;
        scml2::memory_alias< unsigned int > SFR_port_Reg_SIZE;
        scml2::memory_alias< unsigned int > SFR_port_Reg_CMD;
        scml2::memory_alias< unsigned int > SFR_port_Reg_QSIZE;
                                                                                //%
	scml2::tlm2_gp_target_adapter< 32 > SFR_port_SFR_port_Reg_adapter;      //%
                                                                                //%
public:                                                                         //%
	// Process declarations                                                 //%
	void rst_nHandler1();
    void Queue_Manager();
    void Queue_to_NAND();
    void Queue_to_Cache();
    void Send_RnB_flag();

    ////////////// TH /////////////////
    void Copy_CMD_To_SFR(uint addr, uint len, uint op);
    void Copy_SFR_To_CMDList(uint size);
    uint Find_channel(uint addr);
                                                                                //%
protected:                                                                      //%
	// Memory callback declarations                                         //%
	void                                                                    //%
	Nandside_Sport_Reg_0_Callback(                                            //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t);          //%
    void                                                                    //%
	Nandside_Sport_Reg_1_Callback(                                            //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t);          //%	
    void                                                                    //%
	Nandside_Sport_Reg_2_Callback(                                            //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t);          //%
    void                                                                    //%
	Nandside_Sport_Reg_3_Callback(                                            //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t);          //%
    void                                                                    //%
	Nandside_Sport_Reg_4_Callback(                                            //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t);          //%
    void                                                                    //%
	Nandside_Sport_Reg_5_Callback(                                            //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t);          //%
    void                                                                    //%
	Nandside_Sport_Reg_6_Callback(                                            //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t);          //%
    void                                                                    //%
	Nandside_Sport_Reg_7_Callback(                                            //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t);          //%



	void                                                                    //%
	Hostside_Sport_Reg_Callback(                                            //%
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t);          //%
	void                                                                    //%
	SFR_port_Reg_Callback(                                                  //%
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

    eWSState curr_state;
    eWSState next_state;

    /* Hostside Queue */
	std::list <sSubReqWithData_t>	Write_Queue;
	std::list <sSubReqWithData_t>	Read_Queue;
	std::list <sSubReq_t>		    Seq_Op_Queue;

    /* Cache and Nand side Queue */
    std::list <sSubReqWithData_t>   Cache_Queue;        // For Cache read / write
    std::list <mSubReqWithData_t>   Nand_Queue[NUM_CHANNEL]; 
    std::list <sSubReqWithValid_t>  Combine_Queue;      // Combine read data from cache and nand 

    BitMap_t Map_Table[CACHE_BUFFER_SIZE];
	uint MT_Start_Ptr;
	uint MT_Curr_Ptr;

    FILE *inputTrace;
    FILE *outputTrace;
    FILE *CacheQsize;
    FILE *BufferLatency;
    FILE *dramCount;

    //uchar* return_valid;
    sSubReq_t tmp_req;
    bool init_state;        // read queue : true, others : false
    bool combine_state;     // cache & nand queue : false, combine_queue : true

    uint new_id;

    sc_core::sc_event send_req_to_NAND[NUM_CHANNEL];
    sc_core::sc_event send_req_to_Cache;
    sc_core::sc_event trig_queue_manager;

    ////////// TH ////////////////////
    unsigned int cmd_size;
    unsigned int reqId;
    COMMAND cmd_list[10];
    sc_core::sc_event e_ScheduleComplete;

    unsigned int iSelectedChannel;


    void Nand_Call_Back(unsigned char* ptr);
    inline bool IsWriteQueue_Empty()
    {
        return (Write_Queue.size() <= 0);
    }

    inline bool IsReadQueue_Empty()
    {
        return (Read_Queue.size() <= 0) ;
    }

    inline bool IsWriteQueue_Full()
    {
        return (Write_Queue.size() >= MAX_QUEUE_SIZE);
    }

    inline bool IsReadQueue_Full()
    {
        return (Read_Queue.size() >= MAX_QUEUE_SIZE);
    }

    inline bool IsQueue_Full()
    {
        uint nandCount=0;
        for(uint i=0;i<NUM_CHANNEL;i++){
            nandCount += Nand_Queue[i].size();
        }
        return (Cache_Queue.size() + nandCount - Combine_Queue.size() >= MAX_QUEUE_SIZE) || (Seq_Op_Queue.size() >= MAX_QUEUE_SIZE);
    }

    inline void Map_Table_Ptr(uint ptr) {
        if (ptr >= CACHE_BUFFER_SIZE) ptr = 0;
        else ptr = ptr + 1;
    }

	inline uint Map_Table_Size()
	{	
        // '10' should be converted to formulation
        if (MT_Curr_Ptr >= MT_Start_Ptr) {
            return (MT_Curr_Ptr - MT_Start_Ptr);
        }
        else {
            return (2*CACHE_BUFFER_SIZE - MT_Start_Ptr + MT_Curr_Ptr);
        }
	}

    inline void Print_Queue_State()
    {

        fprintf(CacheQsize, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", Seq_Op_Queue.size(), Write_Queue.size(), Read_Queue.size(), Cache_Queue.size(), Combine_Queue.size(), Nand_Queue[0].size(), Nand_Queue[1].size(),Nand_Queue[2].size(),Nand_Queue[3].size(),Nand_Queue[4].size(),Nand_Queue[5].size(),Nand_Queue[6].size(),Nand_Queue[7].size());

        //cout << "< Queue Manager : " << Map_Table_Size() << " >" << endl;

        //cout << "\tSEQ SIZE : " << Seq_Op_Queue.size() << endl;
        //cout << "\tWRITE QUEUE SIZE : " << Write_Queue.size() << endl;
        //cout << "\tREAD QUEUE SIZE : " << Read_Queue.size() << endl << endl;

        //cout << "\tCACHE QUEUE SIZE : " << Cache_Queue.size() << endl;
 //       //cout << "\tNAND QUEUE SIZE : " << Nand_Queue.size() << endl;
        //cout << "\tCOMBINE QUEUE SIZE : " << Combine_Queue.size() << endl;
    }

	// Host side
	sSubReqWithData_t	curReq;

	// Nand side
    sReadSubResp_t      Nand_reg;

    // For Test
    uint num_req;
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
                                                                              
template < unsigned int NUM_CHANNEL>
Cache_Buffer_Controller<NUM_CHANNEL>::Cache_Buffer_Controller( const sc_core::sc_module_name & n)
		: sc_core::sc_module(n)                                         //%
,                                                                               //%
 rst_n("rst_n"),                                                                //%
// Nandside_Mport("Nandside_Mport"),                                              //%
 Nandside_Sport_0("Nandside_Sport_0"),
 Nandside_Sport_1("Nandside_Sport_1"),
 Nandside_Sport_2("Nandside_Sport_2"),
 Nandside_Sport_3("Nandside_Sport_3"),
 Nandside_Sport_4("Nandside_Sport_4"),
 Nandside_Sport_5("Nandside_Sport_5"),
 Nandside_Sport_6("Nandside_Sport_6"),
 Nandside_Sport_7("Nandside_Sport_7"),
// Nandside_RnB("Nandside_RnB"),                                                  //%
 Hostside_Mport("Hostside_Mport"),                                              //%
 Hostside_Sport("Hostside_Sport"),                                              //%
 Hostside_RnB("Hostside_RnB"),                                                  //%
 Cacheside_Mport("Cacheside_Mport"),                                            //%
 SFR_port("SFR_port"),                                                          //%
 Interrupt_port("Interrupt_port"),                                              //%
 Nandside_Sport_0_Nandside_Sport_Reg_0_adapter("Nandside_Sport_0_Nandside_Sport_Reg_0_adapter", Nandside_Sport_0),//%
 Nandside_Sport_1_Nandside_Sport_Reg_1_adapter("Nandside_Sport_1_Nandside_Sport_Reg_1_adapter", Nandside_Sport_1),//%
 Nandside_Sport_2_Nandside_Sport_Reg_2_adapter("Nandside_Sport_2_Nandside_Sport_Reg_2_adapter", Nandside_Sport_2),//%
 Nandside_Sport_3_Nandside_Sport_Reg_3_adapter("Nandside_Sport_3_Nandside_Sport_Reg_3_adapter", Nandside_Sport_3),//%
 Nandside_Sport_4_Nandside_Sport_Reg_4_adapter("Nandside_Sport_4_Nandside_Sport_Reg_4_adapter", Nandside_Sport_4),//%
 Nandside_Sport_5_Nandside_Sport_Reg_5_adapter("Nandside_Sport_5_Nandside_Sport_Reg_5_adapter", Nandside_Sport_5),//%
 Nandside_Sport_6_Nandside_Sport_Reg_6_adapter("Nandside_Sport_6_Nandside_Sport_Reg_6_adapter", Nandside_Sport_6),//%
 Nandside_Sport_7_Nandside_Sport_Reg_7_adapter("Nandside_Sport_7_Nandside_Sport_Reg_7_adapter", Nandside_Sport_7),//%
 Hostside_Sport_Hostside_Sport_Reg_adapter("Hostside_Sport_Hostside_Sport_Reg_adapter", Hostside_Sport),//%
 SFR_port_SFR_port_Reg_adapter("SFR_port_SFR_port_Reg_adapter", SFR_port),      //%
 Nandside_Sport_Reg_0("Nandside_Sport_Reg_0", 4096ULL),   
 Nandside_Sport_Reg_1("Nandside_Sport_Reg_1", 4096ULL),   
 Nandside_Sport_Reg_2("Nandside_Sport_Reg_2", 4096ULL),   
 Nandside_Sport_Reg_3("Nandside_Sport_Reg_3", 4096ULL),   
 Nandside_Sport_Reg_4("Nandside_Sport_Reg_4", 4096ULL),   
 Nandside_Sport_Reg_5("Nandside_Sport_Reg_5", 4096ULL),   
 Nandside_Sport_Reg_6("Nandside_Sport_Reg_6", 4096ULL),   
 Nandside_Sport_Reg_7("Nandside_Sport_Reg_7", 4096ULL),   
 Hostside_Sport_Reg("Hostside_Sport_Reg", 4096ULL),                             //%
 SFR_port_Reg("SFR_port_Reg", 166ULL),                                            //%
    SFR_port_Reg_complete("SFR_port_Reg_complete", SFR_port_Reg, 0LL, 1LL),
    SFR_port_Reg_ADDR("SFR_port_Reg_ADDR", SFR_port_Reg, 1LL, 1LL),
    SFR_port_Reg_LEN("SFR_port_Reg_LEN", SFR_port_Reg, 2LL, 1LL),
    SFR_port_Reg_OP("SFR_port_Reg_OP", SFR_port_Reg, 3LL, 1LL),
    SFR_port_Reg_SIZE("SFR_port_Reg_SIZE", SFR_port_Reg, 4LL, 1LL),
    SFR_port_Reg_CMD("SFR_port_Reg_CMD", SFR_port_Reg, 5LL, 160LL),
    SFR_port_Reg_QSIZE("SFR_port_Reg_QSIZE", SFR_port_Reg, 165LL, 1LL)

                                                                                //%
                                                                                //%USERBEGIN INITIALIZER
    // TODO: Add your class initialization here.
                                                                                //%USEREND INITIALIZER
                                                                                //%
{                                                                               //%
 // declare processes and their sensitivities                                   //%


    SC_THREAD(rst_nHandler1);                                               //%
	 sensitive << rst_n.neg() ;                                             //%
	 dont_initialize();                                                     //%

    SC_THREAD(Queue_Manager);
     sensitive << trig_queue_manager;
     dont_initialize();

    for(unsigned int i=0; i< NUM_CHANNEL; i++){
    SC_THREAD(Queue_to_NAND);
     sensitive << send_req_to_NAND[i];
     dont_initialize();
    }

    SC_THREAD(Queue_to_Cache);
     sensitive << send_req_to_Cache;
     dont_initialize();

	// bind target ports to memories                                        //%
	Hostside_Sport_Hostside_Sport_Reg_adapter(Hostside_Sport_Reg);          //%
	SFR_port_SFR_port_Reg_adapter(SFR_port_Reg);                            //%
    Nandside_Sport_0_Nandside_Sport_Reg_0_adapter(Nandside_Sport_Reg_0); 
    Nandside_Sport_1_Nandside_Sport_Reg_1_adapter(Nandside_Sport_Reg_1); 
    Nandside_Sport_2_Nandside_Sport_Reg_2_adapter(Nandside_Sport_Reg_2); 
    Nandside_Sport_3_Nandside_Sport_Reg_3_adapter(Nandside_Sport_Reg_3); 
    Nandside_Sport_4_Nandside_Sport_Reg_4_adapter(Nandside_Sport_Reg_4); 
    Nandside_Sport_5_Nandside_Sport_Reg_5_adapter(Nandside_Sport_Reg_5); 
    Nandside_Sport_6_Nandside_Sport_Reg_6_adapter(Nandside_Sport_Reg_6); 
    Nandside_Sport_7_Nandside_Sport_Reg_7_adapter(Nandside_Sport_Reg_7); 
                                                                                //%
 // setup memory callbacks                                                      //%
	// - Memory: Nandside_Sport_Reg                                         //%

    scml2::set_callback(                                                    //%
		Nandside_Sport_Reg_0,                                             //%
		SCML2_CALLBACK(Nandside_Sport_Reg_0_Callback),                    //%
		scml2::AUTO_SYNCING);                                           //% 
    scml2::set_callback(                                                    //%
		Nandside_Sport_Reg_1,                                             //%
		SCML2_CALLBACK(Nandside_Sport_Reg_1_Callback),                    //%
		scml2::AUTO_SYNCING);                                           //% 
    scml2::set_callback(                                                    //%
		Nandside_Sport_Reg_2,                                             //%
		SCML2_CALLBACK(Nandside_Sport_Reg_2_Callback),                    //%
		scml2::AUTO_SYNCING);                                           //% 
    scml2::set_callback(                                                    //%
		Nandside_Sport_Reg_3,                                             //%
		SCML2_CALLBACK(Nandside_Sport_Reg_3_Callback),                    //%
		scml2::AUTO_SYNCING);                                           //%
    scml2::set_callback(                                                    //%
		Nandside_Sport_Reg_4,                                             //%
		SCML2_CALLBACK(Nandside_Sport_Reg_4_Callback),                    //%
		scml2::AUTO_SYNCING);                                           //%
    scml2::set_callback(                                                    //%
		Nandside_Sport_Reg_5,                                             //%
		SCML2_CALLBACK(Nandside_Sport_Reg_5_Callback),                    //%
		scml2::AUTO_SYNCING);                                           //%
    scml2::set_callback(                                                    //%
		Nandside_Sport_Reg_6,                                             //%
		SCML2_CALLBACK(Nandside_Sport_Reg_6_Callback),                    //%
		scml2::AUTO_SYNCING);                                           //%
    scml2::set_callback(                                                    //%
		Nandside_Sport_Reg_7,                                             //%
		SCML2_CALLBACK(Nandside_Sport_Reg_7_Callback),                    //%
		scml2::AUTO_SYNCING);                                           //%







                                                                                //%
	// - Memory: Hostside_Sport_Reg                                         //%
	scml2::set_callback(                                                    //%
		Hostside_Sport_Reg,                                             //%
		SCML2_CALLBACK(Hostside_Sport_Reg_Callback),                    //%
		scml2::AUTO_SYNCING);                                           //%
                                                                                //%
	// - Memory: SFR_port_Reg                                               //%
	scml2::set_callback(                                                    //%
		SFR_port_Reg,                                                   //%
		SCML2_CALLBACK(SFR_port_Reg_Callback),                          //%
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
template < unsigned int NUM_CHANNEL >
Cache_Buffer_Controller<NUM_CHANNEL>::~Cache_Buffer_Controller()                             //%
{                                                                               //%
                                                                                //%USERBEGIN DESTRUCTOR

    // TODO: Add destructor code here.
    //delete return_valid;
    //
    fprintf(dramCount,"DRAM read count = %llu\t write count = %llu\n", dram_read_count, dram_write_count);
    fclose(inputTrace);
    fclose(outputTrace);   
    fclose(CacheQsize);
    fclose(BufferLatency);
    fclose(dramCount);
    //delete curReq.subReq;
    //delete curReq.cpData;

    //delete Nand_reg.cpData;
                                                                                //%USEREND DESTRUCTOR
                                                                                //
}                                                                               //%
// initialization method for ports                                              //%

template < unsigned int NUM_CHANNEL >
void                                                                            //%
Cache_Buffer_Controller<NUM_CHANNEL>::initPorts()                                            //%
{                                                                               //%
	Hostside_RnB.initialize(0x0);                                           //%
	Interrupt_port.initialize(0x0);                                         //%
}                                                                               //%
// initialization method for registers                                          //%
template < unsigned int NUM_CHANNEL >
void                                                                            //%
Cache_Buffer_Controller<NUM_CHANNEL>::initRegisters()                                        //%
{                                                                               //%
    Nandside_Sport_Reg_0.initialize(0x0);                 
//    Nandside_Sport_Reg_1.initialize(0x0);                 
//    Nandside_Sport_Reg_2.initialize(0x0);                 
//    Nandside_Sport_Reg_3.initialize(0x0);                 
    Hostside_Sport_Reg.initialize(0x0);                                         //%
    SFR_port_Reg.initialize(0x0);                                               //%
}                                                                               //%
//This method is invoked after the design is fully elaborated, but              //%
// before simulation begins.                                                    //%
template < unsigned int NUM_CHANNEL >
void                                                                            //%
Cache_Buffer_Controller<NUM_CHANNEL>::end_of_elaboration()                                   //%
{                                                                               //%
                                                                                //%USERBEGIN END_OF_ELAB
    // TODO: Add end_of_elaboration code here.
    this->initRegisters();
    this->initPorts();
    
    inputTrace = fopen("inputTrace.txt","w");    
    outputTrace = fopen("outputTrace.txt","w");    
    CacheQsize = fopen("cacheQsize.txt","w");
    BufferLatency = fopen("BufferLatency.txt","w");
    dramCount = fopen("DRAM_COUNT.txt","w");

    dram_read_count = 0;
    dram_write_count = 0;

    //%USEREND END_OF_ELAB
}                                                                               //%
// command processing method                                                    //%
//  During simulation, this command may be called to handle                     //%
//  interactive commands -- for dynamic instrumentation of the instance.        //%
template < unsigned int NUM_CHANNEL >
std::string                                                                     //%
Cache_Buffer_Controller<NUM_CHANNEL>::commandProcessor(const std::vector< std::string >& cmd)//%
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
template < unsigned int NUM_CHANNEL >
void                                                                            //%
Cache_Buffer_Controller<NUM_CHANNEL>::initMessageLogging()                                   //%
{                                                                               //%
                                                                                //%USERBEGIN MODEL_DEBUG_INIT
    // toggle on/off debugging flags
    for (int i=0;i<sizeof(DbgCategories);i++) {
#ifdef DBG_SCP_CACHE_BUFFER_CONTROLLER
        dbgFlag[i] = true;
#else
        dbgFlag[i] = false;
#endif
    }
                                                                                //%USEREND MODEL_DEBUG_INIT
                                                                                //%
}                                                                               //%
// method for logging messages                                                  //%
template < unsigned int NUM_CHANNEL >
void                                              //%
Cache_Buffer_Controller<NUM_CHANNEL>::REPORT_MSG(std::string sev, std::string msg)//%
{                                                 //%
                                                                                //%USERBEGIN REPORT_MSG
    std::cout << sev << ": " << msg << " [" << this->basename() << "@" << sc_core::sc_time_stamp() << "]" << std::endl;
                                                                                //%USEREND REPORT_MSG
                                                                                //%
}                                                                               //%
template < unsigned int NUM_CHANNEL >
void                                                                            //%
Cache_Buffer_Controller<NUM_CHANNEL>::rst_nHandler1()                                        //%
                                                                                //%USERBEGIN rst_nHandler1
{
	if ( dbgFlag[PORTS] ) DBG_MSG( "rst_nHandler1 invoked because of event on port rst_n" );

	// TODO: INSERT code for the rst_nHandler1 method here.
	curr_state = IDLE;
    
    curReq.subReq = new sSubReq_t;
    curReq.cpData = new uchar[SECTOR_PER_PAGE * SECTOR_SIZE_BYTE];

    Nand_reg.cpData = new uchar[SECTOR_PER_PAGE * SECTOR_SIZE_BYTE];

	MT_Start_Ptr = 0;
	MT_Curr_Ptr = 0;

    new_id = 0;
    init_state = true;
    combine_state = false;
    Hostside_RnB.write(1);

    // For Test
    num_req = 0;

    //TH
    reqId = 0;

    for(unsigned int i=0;i<NUM_CHANNEL;i++){
        iSelectedChannel = i;
        send_req_to_NAND[i].notify();
        wait(SC_ZERO_TIME);
    }
    wait();
}

/* Whenever request is come from Hostside
 * Request in Read and Write Queue is distributed to The OTHER QUEUES
 * (Cache_Queue, Nand_Queue, Combine_Queue) for multi-threading */
template < unsigned int NUM_CHANNEL >
void
Cache_Buffer_Controller<NUM_CHANNEL>::Queue_Manager()
{
    std::list < sSubReqWithData_t >::iterator req_itr;
    std::list < sSubReq_t >::iterator op_itr;
	
    HOST_REQ_OP curr_op;
	uint data_len;
	uint req_len;
	uint addr_idx;
    uint cache_idx;

	uint64 Start_Addr;
	uint64 tmp_addr;
	uint64 Page_Addr;

    uint init_ptr;
    uint fin_ptr;
    uint count_sequential_data;

//    uchar* valid_bit = new uchar[SECTOR_PER_PAGE];          // Required whole data sequence
//    uchar* nand_portion = new uchar[SECTOR_PER_PAGE];

    uint prev_num = 0;

    while(1) {

        op_itr = Seq_Op_Queue.begin();
        curr_op = op_itr ->oriReq.Op;

        //////////////////// TH /////////////////////
        Copy_CMD_To_SFR(op_itr->iStartAddr, op_itr->iLen, curr_op);

        Interrupt_port.write(1);
        //cout << "cache buffer sw wait" << endl;
        wait(e_ScheduleComplete);
        //Send_RnB_flag();

        if (Seq_Op_Queue.size() > 0) {
            wait(CLOCK_PERIOD_NS, SC_NS);
        }
        else {
            wait();
        }
    }
}


template < unsigned int NUM_CHANNEL >
void
Cache_Buffer_Controller<NUM_CHANNEL>::Queue_to_Cache()
{
    std::list <sSubReqWithValid_t>::iterator comb_itr;
    std::list <mSubReqWithData_t>::iterator nand_itr;
    std::list <sSubReqWithData_t>::iterator cache_itr;

    std::list <sSubReq_t>::iterator op_itr;
    uint64 Start_Addr;
    uint64 Page_Addr;
    uint req_len;
    uint addr_idx;
    uint cache_idx;
    uint count_sequential_data;
    uint data_seq;

    //HOST_REQ_OP curr_op;
    uint curr_op;

    bool prev_state;
    bool find_cache_data;


    while(1) {

        while(!(Cache_Queue.size() == 0)){
            cache_itr = Cache_Queue.begin();
            curr_op = cache_itr->cmd.Op;

            bool Init_State = init_state;
            bool Combine_nandData = false;
            bool Combine_Flag = false;

            if (curr_op == DRAM_WRITE) {        // Write Queue

                //cout << "DRAM write ADDR = " << cache_itr->cmd.Addr << "   length = " << cache_itr->cmd.Len << endl;
                Cacheside_Mport.write(cache_itr->cmd.Addr, (uchar *)cache_itr->cpData, cache_itr->cmd.Len);
                dram_write_count += cache_itr->cmd.Len /512;
                
                delete cache_itr->subReq;
                delete[] cache_itr->cpData;

                Cache_Queue.erase(cache_itr);
                Send_RnB_flag();
            }
            else if (curr_op == DRAM_READ) { 
                
                if(cache_itr->reqType == dummyReq){    //for evicted data
                    uint nandCh;
                    //cout << "For eviction DRAM read ADDR = " << cache_itr->cmd.Addr << "   length = " << cache_itr->cmd.Len <<"  id = "<<cache_itr->Id<< endl;
                    //read DRAM
                    Cacheside_Mport.read(cache_itr->cmd.Addr, cache_itr->cpData, cache_itr->cmd.Len);

                    dram_read_count += cache_itr->cmd.Len /512;
                    //copy data to nand queue
                    bool test = false;
                    for(nandCh=0;nandCh<NUM_CHANNEL;nandCh++){
                        for(nand_itr = Nand_Queue[nandCh].begin();nand_itr != Nand_Queue[nandCh].end(); nand_itr++){
                            //cout << "Nand id = " << nand_itr->Id << endl;
                            if(nand_itr->Id == cache_itr->Id){
                                test = true;
                                break;
                            }
                        }
                        if(test)
                            break;
                    }

                    if(test == false){
                        cout <<"Don't find duumy nand cmd"<<endl;
                        sc_stop();
                    }

                    //offset 확인해야 함
                    memcpy(nand_itr->cpData, cache_itr->cpData, cache_itr->cmd.Len);
                    nand_itr->readyTrans = true;
                    
                    delete cache_itr->subReq;
                    delete[] cache_itr->cpData;

                    Cache_Queue.erase(cache_itr);

                    send_req_to_NAND[nandCh].notify();

                }else if(cache_itr->reqType == combineReq){
                    //read DRAM
                    Cacheside_Mport.read(cache_itr->cmd.Addr, cache_itr->cpData, cache_itr->cmd.Len);

                    dram_read_count += cache_itr->cmd.Len /512;
                    //find dram bit map in combine Queue
                    for(comb_itr = Combine_Queue.begin(); comb_itr != Combine_Queue.end(); comb_itr++){
                        if(comb_itr->Id == cache_itr->Id)
                            break;
                    }

                    if(comb_itr->type == CombWrite){ //dram read & nand write (dram eviction)
                        //copy data to nand queue
                        uint j = 0;
                        uint dataPoint = 0;
                        uint startPointer = 0, endPointer = 0;
                        bool isFirst = true;
                        uint nandCh;
                        for(nandCh=0;nandCh<NUM_CHANNEL;nandCh++)
                        for(nand_itr = Nand_Queue[nandCh].begin(); nand_itr != Nand_Queue[nandCh].end(); nand_itr++){
                            if(nand_itr->Id == cache_itr->Id){
                                for(; j<SECTOR_PER_PAGE ; j++){
                                    if(comb_itr->Valid_bit & (1<<j)){
                                        if(isFirst){
                                            startPointer = j;
                                            isFirst = false;
                                        }
                                        endPointer = j;
                                    }else if(isFirst == false){
                                        break;
                                    }
                                }

                                memcpy(nand_itr->cpData, &(cache_itr->cpData[ startPointer * SECTOR_SIZE_BYTE]), (endPointer - startPointer + 1)* SECTOR_SIZE_BYTE);
                                nand_itr->readyTrans = true;
                                isFirst = true;
                            }
                        }
                        
                        delete cache_itr->subReq;
                        delete[] cache_itr->cpData;

                        delete comb_itr->subReq;
                        delete[] comb_itr->cpData;

                        Cache_Queue.erase(cache_itr);
                        Combine_Queue.erase(comb_itr);
                    }else{      //dram read & nand read
                        //copy data to combine queue
                        uint dataPoint = 0;
                        for(uint i=0; i < SECTOR_PER_PAGE; i++){
                            if(comb_itr->Valid_bit & (1<<i)){
                                memcpy(&(comb_itr->cpData[i * SECTOR_SIZE_BYTE]), &(cache_itr->cpData[dataPoint * SECTOR_SIZE_BYTE]), SECTOR_SIZE_BYTE);
                                dataPoint++;
                            }
                        }
                        

                        delete cache_itr->subReq;
                        delete[] cache_itr->cpData;

                        Cache_Queue.erase(cache_itr);

                    }
                    for(uint i=0;i<NUM_CHANNEL;i++){
                        send_req_to_NAND[i].notify();
                        wait(SC_ZERO_TIME);
                    }

                }else{      //for origin read request
                    //cout << "DRAM read ADDR = " << cache_itr->cmd.Addr << "   length = " << cache_itr->cmd.Len << endl;
                    Cacheside_Mport.read(cache_itr->cmd.Addr, (uchar*) cache_itr->cpData, cache_itr->cmd.Len);
                    dram_read_count += cache_itr->cmd.Len /512;

                    sReadSubResp_t Rreqdata;                // return to hostside
                    Rreqdata.cpData = new uchar[cache_itr->cmd.Len];

                    memcpy(&Rreqdata.subReq, cache_itr->subReq, sizeof(sSubReq_t));
                    memcpy(Rreqdata.cpData, cache_itr->cpData, cache_itr->cmd.Len);

                    Hostside_Mport.write(Host_Addr, (void*) &Rreqdata, sizeof(sReadSubResp_t));
                    
                    delete cache_itr->subReq;
                    delete[] cache_itr->cpData;
                    delete []Rreqdata.cpData;

                    Cache_Queue.erase(cache_itr);
                    Send_RnB_flag();
                }
            }
        }
        wait();
    }

}

template < unsigned int NUM_CHANNEL >
void
Cache_Buffer_Controller<NUM_CHANNEL>::Queue_to_NAND()
{
    unsigned int iChannel = iSelectedChannel;
    std::list < mSubReqWithData_t >::iterator nand_itr;
 
    uint64 Start_Addr;
    uint64 Page_Addr;

    uint init_ptr;
    uint fin_ptr;

    HOST_REQ_OP curr_op;

    while(1) {

        uint64 prev_addr = 0;

        bool Init_State = init_state;
        bool init = true;
        bool Combine_nandData = false;
        bool iter_nand_count = false;

        bool transReq = false;

        if (Nandside_RnB[iChannel].read()== 0) {
            wait(Nandside_RnB[iChannel].posedge_event());
        }

        for (nand_itr = Nand_Queue[iChannel].begin(); nand_itr != Nand_Queue[iChannel].end(); nand_itr++) {
            if ((nand_itr->check_trans == false) & (nand_itr->readyTrans == true)) {
                curr_op = nand_itr->subReq->oriReq.Op;
                transReq = true;
                break;
            }
        }



        //Print_Queue_State();
        if(transReq){
            if (curr_op == HOST_WRITE) {
                //cout << "NAND write addr = " << nand_itr->subReq->iStartAddr << "   length =   " << nand_itr->subReq->iLen <<"    ch = "<<iChannel<<"  ID = "<<nand_itr->subReq->oriReq.iId <<endl;
                Nandside_Mport[iChannel].write(NAND_Addr, nand_itr->subReq, sizeof(sSubReq_t));
                //fprintf(outputTrace, "%llu\t%u\t%u\n", nand_itr->subReq->iStartAddr, nand_itr->subReq->iLen, 0);
                wait(SC_ZERO_TIME);
                Nandside_Mport[iChannel].write(NAND_Addr, (uchar*) nand_itr->cpData, nand_itr->subReq->iLen*SECTOR_SIZE_BYTE);


                fprintf(BufferLatency, "2: %lf\n", (double)sc_time_stamp().value()/1000);
                wait(SC_ZERO_TIME);

                
                delete nand_itr->subReq;
                delete[] nand_itr->cpData;

                Nand_Queue[iChannel].erase(nand_itr);
                Send_RnB_flag();
            }
            else if (curr_op == HOST_READ) {

                //cout << "NAND read addr = " << nand_itr->subReq->iStartAddr << "   length =   " << nand_itr->subReq->iLen << "     ch = "<<iChannel<<endl;
                
                Nandside_Mport[iChannel].write(NAND_Addr, nand_itr->subReq, sizeof(sSubReq_t));
                nand_itr->check_trans = true;

            }
            else {
                //cout << "Nothing happend" << endl;
                //cout << "QUEUE TO NAND is raised at only READ OPERATION" << endl;
                sc_stop();
            }

            
        }
        for (nand_itr = Nand_Queue[iChannel].begin(); nand_itr != Nand_Queue[iChannel].end(); nand_itr++) {
            if (nand_itr->check_trans == false) {
                iter_nand_count = true;
                break;
            }
            else iter_nand_count = false;
        }
        if (iter_nand_count) {
            wait(CLOCK_PERIOD_NS, SC_NS);
        }
        else {
            wait();
        }
    }
}

template < unsigned int NUM_CHANNEL >
void
Cache_Buffer_Controller<NUM_CHANNEL>::Send_RnB_flag()        // Send RnB signal to host side device.
{
    ////cout << "\tQueue size : " << Seq_Op_Queue.size() << endl;

    if (IsQueue_Full()) {
        //cout << "\tHostside RnB : 0" << endl;
        Hostside_RnB.write(0);
    }
    else {
        //cout << "\tHostside RnB : 1" << endl;
        Hostside_RnB.write(1);
    }
}

///////////////// TH /////////////////////
template < unsigned int NUM_CHANNEL >
void
Cache_Buffer_Controller<NUM_CHANNEL>::Copy_CMD_To_SFR(uint addr, uint len, uint op)
{
    SFR_port_Reg_ADDR[0] = (unsigned int)addr;
    SFR_port_Reg_LEN[0] = (unsigned int)len;
    SFR_port_Reg_OP[0] = (unsigned int)op;
}

template < unsigned int NUM_CHANNEL >
uint
Cache_Buffer_Controller<NUM_CHANNEL>::Find_channel(uint addr)
{

    return ((addr >> 4) & ((uint)CHANNEL - 1));
    //return 0;

}

template < unsigned int NUM_CHANNEL >
void
Cache_Buffer_Controller<NUM_CHANNEL>::Copy_SFR_To_CMDList(uint size)
{
    scml2::memory_alias< unsigned int >& m = this->SFR_port_Reg_CMD;   
    std::list < sSubReqWithData_t >::iterator req_itr;
    

    for(uint i=0; i < size; i++){
        for(uint j=0; j<5; j++)
            ((unsigned int*)&cmd_list[i])[j] = m[i*5+j];

        //cout << "controller cmd list op = " << cmd_list[i].op << "   slave addr = " << cmd_list[i].slave_addr << "   slave bitmap = " << cmd_list[i].slave_bitmap << "    dram_id = " << cmd_list[i].dram_id << "   dram bitmap = " << cmd_list[i].dram_bitmap << endl;
    }
    
    
    //Print_Queue_State();
    for(uint i=0; i < size; i++){
        uint length=0;
        uint startPointer, endPointer;
        uint isFirst = true;
        uint createReq = false;
        uint numCh;
        
        mSubReqWithData_t nand_queue;
        sSubReqWithData_t sub_queue;
        sSubReqWithValid_t comb_queue;
       
        switch(cmd_list[i].op){
            case 0:     //read dram
                sub_queue.subReq = new sSubReq_t;
                sub_queue.cpData = new uchar[SECTOR_PER_PAGE * SECTOR_SIZE_BYTE];

                req_itr = Read_Queue.begin();
                memcpy(sub_queue.subReq, req_itr->subReq, sizeof(sSubReq_t));
                
                for(uint j=0; j<SECTOR_PER_PAGE ; j++){
                    if(cmd_list[i].dram_bitmap & (1<<j)){
                        if(isFirst){
                            isFirst = false;
                            startPointer = j;
                        }
                        length++;
                    }
                }

                sub_queue.cmd.Addr = cmd_list[i].dram_id * SECTOR_SIZE_BYTE * SECTOR_PER_PAGE + startPointer * SECTOR_SIZE_BYTE;
                sub_queue.cmd.Op = DRAM_READ;
                sub_queue.cmd.Len = length * SECTOR_SIZE_BYTE;

                sub_queue.Id = req_itr->Id;
                sub_queue.reqType = hostReq;

                Cache_Queue.push_back(sub_queue);
               
                delete req_itr->subReq;
                delete[] req_itr->cpData;

                Read_Queue.pop_front();
                Seq_Op_Queue.pop_front();

                break;

            case 1:     //write dram
                sub_queue.subReq = new sSubReq_t;
                sub_queue.cpData = new uchar[SECTOR_PER_PAGE * SECTOR_SIZE_BYTE];

                req_itr = Write_Queue.begin();
                //cout << "write dram address = " << req_itr->subReq->iStartAddr << "   ID = " <<req_itr->subReq->oriReq.iId << endl;
                memcpy(sub_queue.subReq, req_itr->subReq, sizeof(sSubReq_t));
                memcpy(sub_queue.cpData, req_itr->cpData, req_itr->subReq->iLen*SECTOR_SIZE_BYTE);

                for(uint j=0; j<SECTOR_PER_PAGE ; j++){
                    if(cmd_list[i].dram_bitmap & (1<<j)){
                        if(isFirst){
                            isFirst = false;
                            startPointer = j;
                        }
                        length++;
                    }
                }

                sub_queue.cmd.Addr = cmd_list[i].dram_id * SECTOR_SIZE_BYTE * SECTOR_PER_PAGE + startPointer * SECTOR_SIZE_BYTE;
                sub_queue.cmd.Op = DRAM_WRITE;
                sub_queue.cmd.Len = length * SECTOR_SIZE_BYTE;

                sub_queue.Id = req_itr->Id;
                sub_queue.reqType = hostReq;

                Cache_Queue.push_back(sub_queue);

                delete req_itr->subReq;
                delete[] req_itr->cpData;

                Write_Queue.pop_front();
                Seq_Op_Queue.pop_front();

                break;

            case 2:     //read nand
                nand_queue.subReq = new sSubReq_t;
                nand_queue.cpData = new uchar[SECTOR_PER_PAGE * SECTOR_SIZE_BYTE];

                req_itr = Read_Queue.begin();
                memcpy(nand_queue.subReq, req_itr->subReq, sizeof(sSubReq_t));

                for(uint j=req_itr->subReq->iStartAddr % SECTOR_PER_PAGE; j<  ((req_itr->subReq->iStartAddr % SECTOR_PER_PAGE) + req_itr->subReq->iLen ); j++){
                    if(cmd_list[i].slave_bitmap & (1<<j)){
                        if(isFirst){
                            isFirst = false;
                            startPointer = j;
                        }
                        length++;
                    }
                }
                
                nand_queue.subReq->iStartAddr = cmd_list[i].slave_addr * SECTOR_PER_PAGE + startPointer;
                nand_queue.subReq->iLen = length;

                nand_queue.Id = req_itr->Id;
                nand_queue.readyTrans = true;
                nand_queue.check_trans = false;
                //numCh =((nand_queue.subReq->iStartAddr >> 3) & 3);
                numCh = Find_channel(nand_queue.subReq->iStartAddr);
                Nand_Queue[numCh].push_back(nand_queue);

                delete req_itr->subReq;
                delete[] req_itr->cpData;
                
                Read_Queue.pop_front();
                Seq_Op_Queue.pop_front();

                break;

            case 3:     //dram read, nand write
                //DRAM read
                sub_queue.subReq = new sSubReq_t;
                sub_queue.cpData = new uchar[SECTOR_PER_PAGE * SECTOR_SIZE_BYTE];

                req_itr = Write_Queue.begin();
                memcpy(sub_queue.subReq, req_itr->subReq, sizeof(sSubReq_t));

                for(uint j=0; j<SECTOR_PER_PAGE ; j++){
                    if(cmd_list[i].dram_bitmap & (1<<j)){
                        if(isFirst){
                            isFirst = false;
                            startPointer = j;
                        }
                        length++;
                    }
                }

                sub_queue.cmd.Addr = cmd_list[i].dram_id * SECTOR_SIZE_BYTE * SECTOR_PER_PAGE + startPointer * SECTOR_SIZE_BYTE;
                sub_queue.cmd.Op = DRAM_READ;
                sub_queue.cmd.Len = length * SECTOR_SIZE_BYTE;
                
                sub_queue.Id = req_itr->Id;
                sub_queue.reqType = dummyReq;

                Cache_Queue.push_back(sub_queue);

                //NAND write
                nand_queue.subReq = new sSubReq_t;
                nand_queue.cpData = new uchar[SECTOR_PER_PAGE * SECTOR_SIZE_BYTE];

                memcpy(nand_queue.subReq, req_itr->subReq, sizeof(sSubReq_t));
                
                nand_queue.Id = req_itr->Id;
                nand_queue.readyTrans = false;
                nand_queue.check_trans = false;
                nand_queue.subReq->iStartAddr = cmd_list[i].slave_addr * SECTOR_PER_PAGE + startPointer;
                nand_queue.subReq->iLen = length;

                //cout << "nand q push id = " << nand_queue.subReq->oriReq.iId <<" address =  "<<nand_queue.subReq->iStartAddr <<endl;
                //numCh =((nand_queue.subReq->iStartAddr >> 3) & 3);
                numCh = Find_channel(nand_queue.subReq->iStartAddr);
                Nand_Queue[numCh].push_back(nand_queue);

                //Nand_Queue.push_back(nand_queue);
                //Print_Queue_State();
                
                break;

            case 4:     //read both
                //nand read
                nand_queue.subReq = new sSubReq_t;
                nand_queue.cpData = new uchar[SECTOR_PER_PAGE * SECTOR_SIZE_BYTE];

                req_itr = Read_Queue.begin();
                memcpy(nand_queue.subReq, req_itr->subReq, sizeof(sSubReq_t));
                                
                for(uint j=0; j<SECTOR_PER_PAGE ; j++){
                    if(cmd_list[i].slave_bitmap & (1<<j)){
                        if(isFirst){
                            isFirst = false;
                            startPointer = j;
                        }
                        endPointer = j;
                    }
                }

                nand_queue.subReq->iStartAddr = cmd_list[i].slave_addr * SECTOR_PER_PAGE + startPointer;
                nand_queue.subReq->iLen = endPointer - startPointer + 1;

                nand_queue.Id = req_itr->Id;
                nand_queue.readyTrans = true;
                nand_queue.check_trans = false;
                
                //numCh =((nand_queue.subReq->iStartAddr >> 3) & 3);
                numCh = Find_channel(nand_queue.subReq->iStartAddr);
                Nand_Queue[numCh].push_back(nand_queue);

                //Nand_Queue.push_back(nand_queue);

                //cache read
                sub_queue.subReq = new sSubReq_t;
                sub_queue.cpData = new uchar[SECTOR_PER_PAGE * SECTOR_SIZE_BYTE];

                memcpy(sub_queue.subReq, req_itr->subReq, sizeof(sSubReq_t));

                isFirst = true;
                for(uint j=0; j<SECTOR_PER_PAGE ; j++){
                    if(cmd_list[i].dram_bitmap & (1<<j)){
                        if(isFirst){
                            startPointer = j;
                            isFirst = false;
                        }
                        endPointer = j;
                    }
                }


                sub_queue.cmd.Addr = cmd_list[i].dram_id * SECTOR_SIZE_BYTE * SECTOR_PER_PAGE + startPointer * SECTOR_SIZE_BYTE;
                sub_queue.cmd.Op = DRAM_READ;
                sub_queue.cmd.Len = (endPointer - startPointer + 1) * SECTOR_SIZE_BYTE;

                sub_queue.Id = req_itr->Id;
                sub_queue.reqType = combineReq;
                Cache_Queue.push_back(sub_queue);

                //push combine queue
                comb_queue.subReq = new sSubReq_t;
                comb_queue.cpData = new uchar[SECTOR_PER_PAGE * SECTOR_SIZE_BYTE];

                memcpy(comb_queue.subReq, req_itr->subReq, sizeof(sSubReq_t));
                comb_queue.Valid_bit = (uchar)cmd_list[i].dram_bitmap;
                comb_queue.Nand_bit = (uchar)cmd_list[i].slave_bitmap;
                comb_queue.type = CombRead;
                comb_queue.Id = req_itr->Id;

                Combine_Queue.push_back(comb_queue);
                
                delete req_itr->subReq;
                delete[] req_itr->cpData;
                Read_Queue.pop_front();
                Seq_Op_Queue.pop_front();

                break;

            case 5:     //update nand
                //dram read
                sub_queue.subReq = new sSubReq_t;
                sub_queue.cpData = new uchar[SECTOR_PER_PAGE * SECTOR_SIZE_BYTE];

                req_itr = Write_Queue.begin();
                memcpy(sub_queue.subReq, req_itr->subReq, sizeof(sSubReq_t));
                
                for(uint j=0; j<SECTOR_PER_PAGE ; j++){
                    if(cmd_list[i].dram_bitmap & (1<<j)){
                        if(isFirst){
                            startPointer = j;
                            isFirst = false;
                        }
                        endPointer = j;
                    }
                }

                sub_queue.cmd.Addr = cmd_list[i].dram_id * SECTOR_SIZE_BYTE * SECTOR_PER_PAGE + startPointer * SECTOR_SIZE_BYTE;
                sub_queue.cmd.Op = DRAM_READ;
                sub_queue.cmd.Len = (endPointer - startPointer + 1) * SECTOR_SIZE_BYTE;

                sub_queue.Id = req_itr->Id;
                sub_queue.reqType = combineReq;
                Cache_Queue.push_back(sub_queue);


                //NAND write
//                nand_queue = new mSubReqWithData_t();
//                nand_queue->subReq = new sSubReq_t();
//                nand_queue->cpData = new uchar[SECTOR_PER_PAGE * SECTOR_SIZE_BYTE];
//
//                memcpy(nand_queue->subReq, req_itr->subReq, sizeof(sSubReq_t));
                
                isFirst = true;
                for(uint j=0; j<=SECTOR_PER_PAGE ; j++){
                    if(cmd_list[i].dram_bitmap & (1<<j)){
                        if(isFirst){
                            startPointer = j;
                            isFirst = false;
                        }
                        endPointer = j;
                        createReq = false;
                    }else{
                        createReq = true;
                    }
                    if(isFirst == false & createReq == true){
                        mSubReqWithData_t nand_queue;
                        nand_queue.subReq = new sSubReq_t;
                        nand_queue.cpData = new uchar[SECTOR_PER_PAGE * SECTOR_SIZE_BYTE];

                        memcpy(nand_queue.subReq, req_itr->subReq, sizeof(sSubReq_t));
                        nand_queue.Id = req_itr->Id;
                        nand_queue.readyTrans = false;
                        nand_queue.check_trans = false;
                        nand_queue.subReq->iStartAddr = cmd_list[i].slave_addr * SECTOR_PER_PAGE + startPointer;
                        nand_queue.subReq->iLen = (endPointer - startPointer + 1);
                        //numCh =((nand_queue.subReq->iStartAddr >> 3) & 3);
                        numCh = Find_channel(nand_queue.subReq->iStartAddr);
                        Nand_Queue[numCh].push_back(nand_queue);

                        //Nand_Queue.push_back(nand_queue);
                        isFirst = true;
                        createReq = false;
                    }
                }
                
                //push combine queue
                comb_queue.subReq = new sSubReq_t;
                comb_queue.cpData = new uchar[SECTOR_PER_PAGE * SECTOR_SIZE_BYTE];

                memcpy(comb_queue.subReq, req_itr->subReq, sizeof(sSubReq_t));
                comb_queue.Valid_bit = (uchar)cmd_list[i].dram_bitmap;
                comb_queue.Nand_bit = (uchar)cmd_list[i].slave_bitmap;
                comb_queue.type = CombWrite;
                comb_queue.Id = req_itr->Id;

                Combine_Queue.push_back(comb_queue);

                break;
            default:
                break;
        }

    }
    //cout << "endl" << endl;
    //Print_Queue_State();
}

/////////////////////////////////////////

template < unsigned int NUM_CHANNEL >
void Cache_Buffer_Controller<NUM_CHANNEL>::Nand_Call_Back(unsigned char* ptr)
{
    std::list <mSubReqWithData_t> ::iterator nand_itr;
    std::list <sSubReqWithData_t> ::iterator cache_itr;
    std::list <sSubReqWithValid_t> ::iterator comb_itr;

    uint set_data = 0;
    uint cp_idx = 0;
    uint cache_idx_1 = 0;
    uint cache_idx_2 = 0;

    bool Cache_Flag = false;
    bool Combine_Flag = false;
    uint numCh;
    bool findFlag=false;

    uchar NAND_Data[SECTOR_PER_PAGE * SECTOR_SIZE_BYTE];
    memcpy(&Nand_reg, ptr, sizeof(sReadSubResp_t));
    //cout << " Cache Buffer recieved ID  = " <<Nand_reg.subReq.oriReq.iId << endl;

    for (comb_itr = Combine_Queue.begin(); comb_itr != Combine_Queue.end(); comb_itr++) {
        if ((comb_itr->subReq->oriReq.iId == Nand_reg.subReq.oriReq.iId) && 
                (((uint64)(comb_itr->subReq->iStartAddr/SECTOR_PER_PAGE)*SECTOR_PER_PAGE) == ((uint64)(Nand_reg.subReq.iStartAddr/SECTOR_PER_PAGE)*SECTOR_PER_PAGE))) {
            Combine_Flag = true;
            break;
        }
        else {
            Combine_Flag = false;
        }
    }

    if (Combine_Flag) {
        for (cache_itr = Cache_Queue.begin(); cache_itr != Cache_Queue.end(); cache_itr++) {
            if ((cache_itr->subReq->oriReq.iId == Nand_reg.subReq.oriReq.iId) && 
                    (((uint64)(cache_itr->subReq->iStartAddr/SECTOR_PER_PAGE)*SECTOR_PER_PAGE) == ((uint64)(Nand_reg.subReq.iStartAddr/SECTOR_PER_PAGE)*SECTOR_PER_PAGE))) {
                Cache_Flag = true;
                break;
            }
            else {
                Cache_Flag = false;
            }
        }

        if (Cache_Flag) { //need to dram read
            memcpy(comb_itr->cpData, &(Nand_reg.cpData), Nand_reg.subReq.iLen*SECTOR_SIZE_BYTE);

            for(numCh=0;numCh<NUM_CHANNEL;numCh++){
                for (nand_itr = Nand_Queue[numCh].begin(); nand_itr != Nand_Queue[numCh].end(); nand_itr++) {
                    if ((nand_itr->subReq->oriReq.iId == Nand_reg.subReq.oriReq.iId) &&
                            (((uint64)(nand_itr->subReq->iStartAddr/SECTOR_PER_PAGE)*SECTOR_PER_PAGE) == ((uint64)(Nand_reg.subReq.iStartAddr/SECTOR_PER_PAGE)*SECTOR_PER_PAGE))) {
                        findFlag = true;
                        break;
                    }
                }
                if(findFlag)
                    break;
            }

            delete nand_itr->subReq;
            delete[] nand_itr->cpData;
            Nand_Queue[numCh].erase(nand_itr);
            Send_RnB_flag();
        }
        else {      //already, complete dram read

            for(numCh=0;numCh < NUM_CHANNEL;numCh++){
                for (nand_itr = Nand_Queue[numCh].begin(); nand_itr != Nand_Queue[numCh].end(); nand_itr++) {
                    if ((nand_itr->subReq->oriReq.iId == Nand_reg.subReq.oriReq.iId) &&
                            (((uint64)(nand_itr->subReq->iStartAddr/SECTOR_PER_PAGE)*SECTOR_PER_PAGE) == ((uint64)(Nand_reg.subReq.iStartAddr/SECTOR_PER_PAGE)*SECTOR_PER_PAGE))) {
                        findFlag = true;
                        break;
                    }
                }
                if(findFlag)
                    break;
            }

            uint dataPoint = 0;
            for(uint i=0; i < SECTOR_PER_PAGE; i++){
                if(comb_itr->Valid_bit & (1<<i)){
                    memcpy(&(comb_itr->cpData[i * SECTOR_SIZE_BYTE]), &(Nand_reg.cpData[dataPoint * SECTOR_SIZE_BYTE]), SECTOR_SIZE_BYTE);
                    dataPoint++;
                }
            }

            sReadSubResp_t Rreqdata;        // return to hostside
            memcpy(&Rreqdata.subReq, comb_itr->subReq, sizeof(sSubReq_t));
            Rreqdata.cpData = new uchar[comb_itr->subReq->iLen*SECTOR_SIZE_BYTE];
            memcpy(Rreqdata.cpData, comb_itr->cpData, Rreqdata.subReq.iLen*SECTOR_SIZE_BYTE);

            Hostside_Mport.write(Host_Addr, (void*) &Rreqdata, sizeof(sReadSubResp_t));

            delete comb_itr->subReq;
            delete[] comb_itr->cpData;

            delete nand_itr->subReq;
            delete[] nand_itr->cpData;

            delete[] Rreqdata.cpData;

            Combine_Queue.erase(comb_itr);
            Nand_Queue[numCh].erase(nand_itr);
            Send_RnB_flag();
        }
    }
    else {
        for(numCh=0;numCh < NUM_CHANNEL;numCh++){
            for (nand_itr = Nand_Queue[numCh].begin(); nand_itr != Nand_Queue[numCh].end(); nand_itr++) {
                if ((nand_itr->subReq->oriReq.iId == Nand_reg.subReq.oriReq.iId) && 
                        (nand_itr->subReq->iStartAddr == Nand_reg.subReq.iStartAddr)) {
                    findFlag = true;
                    break;
                }
            }
            if(findFlag)
                break;

        }
        sReadSubResp_t Rreqdata;        // return to hostside
        memcpy(&Rreqdata.subReq, nand_itr->subReq, sizeof(sSubReq_t));
        Rreqdata.cpData = new uchar[Rreqdata.subReq.iLen*SECTOR_SIZE_BYTE];
        memcpy(Rreqdata.cpData, &(Nand_reg.cpData), Rreqdata.subReq.iLen*SECTOR_SIZE_BYTE);
        Hostside_Mport.write(Host_Addr, (void*) &Rreqdata, sizeof(sReadSubResp_t));

        delete nand_itr->subReq;
        delete[] nand_itr->cpData;

        delete[] Rreqdata.cpData;

        Nand_Queue[numCh].erase(nand_itr);
        Send_RnB_flag();

    }
}
template < unsigned int NUM_CHANNEL >
void                                                                            //%
Cache_Buffer_Controller<NUM_CHANNEL>::Nandside_Sport_Reg_0_Callback(      
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)     
{                                                                    //%
    const char* dbgMsgText = "TRANSPORT handler for memory Nandside_Sport_Reg"; 
    scml2::memory< unsigned int >& m = this->Nandside_Sport_Reg_0;                //%
    tlm::tlm_command cmd = trans.get_command();                           //%
    sc_dt::uint64 adr = trans.get_address();                              //%
    unsigned char* ptr = trans.get_data_ptr();                            //%
    unsigned int len = trans.get_data_length();                           //%
    unsigned char* byt = trans.get_byte_enable_ptr();                     //%
    unsigned int wid = trans.get_streaming_width();                       //%
    //%USERBEGIN Nandside_Sport_Reg_Callback
    if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );
    // TODO: INSERT code for the Nandside_Sport_Reg_Callback method here.

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

    } else if (cmd == tlm::TLM_READ_COMMAND) {
        // copy from your target's memory to 'ptr', e.g.: memcpy(ptr, &mem[adr], num_bytes);
    }
    Nand_Call_Back(ptr);

    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
    //%USEREND Nandside_Sport_Reg_Callback
}   

template < unsigned int NUM_CHANNEL >
void                                                                            //%
Cache_Buffer_Controller<NUM_CHANNEL>::Nandside_Sport_Reg_1_Callback(   
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)           //%
{                                                                       //%
    const char* dbgMsgText = "TRANSPORT handler for memory Nandside_Sport_Reg";
    scml2::memory< unsigned int >& m = this->Nandside_Sport_Reg_1;                //%
    tlm::tlm_command cmd = trans.get_command();                       //%
    sc_dt::uint64 adr = trans.get_address();                         //%
    unsigned char* ptr = trans.get_data_ptr();                     //%
    unsigned int len = trans.get_data_length();                        //%
    unsigned char* byt = trans.get_byte_enable_ptr();                 //%
    unsigned int wid = trans.get_streaming_width();                  //%
                //%USERBEGIN Nandside_Sport_Reg_Callback
    if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );
    // TODO: INSERT code for the Nandside_Sport_Reg_Callback method here.

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

    } else if (cmd == tlm::TLM_READ_COMMAND) {
        // copy from your target's memory to 'ptr', e.g.: memcpy(ptr, &mem[adr], num_bytes);
    }

    Nand_Call_Back(ptr);
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
    //%USEREND Nandside_Sport_Reg_Callback
}
template < unsigned int NUM_CHANNEL >
void                                                                            //%
Cache_Buffer_Controller<NUM_CHANNEL>::Nandside_Sport_Reg_2_Callback( 
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)           //%
{                                                                    //%
    const char* dbgMsgText = "TRANSPORT handler for memory Nandside_Sport_Reg";
    scml2::memory< unsigned int >& m = this->Nandside_Sport_Reg_2;                //%
    tlm::tlm_command cmd = trans.get_command();                    //%
    sc_dt::uint64 adr = trans.get_address();                      //%
    unsigned char* ptr = trans.get_data_ptr();                    
    unsigned int len = trans.get_data_length();                      //%
    unsigned char* byt = trans.get_byte_enable_ptr();                //%
    unsigned int wid = trans.get_streaming_width();                  //%
                       //%USERBEGIN Nandside_Sport_Reg_Callback
    if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );
    // TODO: INSERT code for the Nandside_Sport_Reg_Callback method here.

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

    } else if (cmd == tlm::TLM_READ_COMMAND) {
        // copy from your target's memory to 'ptr', e.g.: memcpy(ptr, &mem[adr], num_bytes);
    }

    Nand_Call_Back(ptr);
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
    //%USEREND Nandside_Sport_Reg_Callback
}

template < unsigned int NUM_CHANNEL >
void                                                           
Cache_Buffer_Controller<NUM_CHANNEL>::Nandside_Sport_Reg_3_Callback(    
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)        
{                                                               
    const char* dbgMsgText = "TRANSPORT handler for memory Nandside_Sport_Reg";
    scml2::memory< unsigned int >& m = this->Nandside_Sport_Reg_3;    
    tlm::tlm_command cmd = trans.get_command();                //%
    sc_dt::uint64 adr = trans.get_address();                   //%
    unsigned char* ptr = trans.get_data_ptr();                 //%
    unsigned int len = trans.get_data_length();                //%
    unsigned char* byt = trans.get_byte_enable_ptr();          //%
    unsigned int wid = trans.get_streaming_width();            //%
    if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );

    if (byt) { // Target unable to support byte enable attribute
        trans.set_response_status( tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE );
        return;
    }
    if (wid < len) { // Target unable to support streaming width attribute
        trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
        return;
    }
    if (cmd == tlm::TLM_WRITE_COMMAND) { // Execute command

    } else if (cmd == tlm::TLM_READ_COMMAND) {
    }

    Nand_Call_Back(ptr);
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
}

template < unsigned int NUM_CHANNEL >
void                                                           
Cache_Buffer_Controller<NUM_CHANNEL>::Nandside_Sport_Reg_4_Callback(    
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)        
{                                                               
    const char* dbgMsgText = "TRANSPORT handler for memory Nandside_Sport_Reg";
    scml2::memory< unsigned int >& m = this->Nandside_Sport_Reg_3;    
    tlm::tlm_command cmd = trans.get_command();                //%
    sc_dt::uint64 adr = trans.get_address();                   //%
    unsigned char* ptr = trans.get_data_ptr();                 //%
    unsigned int len = trans.get_data_length();                //%
    unsigned char* byt = trans.get_byte_enable_ptr();          //%
    unsigned int wid = trans.get_streaming_width();            //%
    if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );

    if (byt) { // Target unable to support byte enable attribute
        trans.set_response_status( tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE );
        return;
    }
    if (wid < len) { // Target unable to support streaming width attribute
        trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
        return;
    }
    if (cmd == tlm::TLM_WRITE_COMMAND) { // Execute command

    } else if (cmd == tlm::TLM_READ_COMMAND) {
    }

    Nand_Call_Back(ptr);
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
}
template < unsigned int NUM_CHANNEL >
void                                                           
Cache_Buffer_Controller<NUM_CHANNEL>::Nandside_Sport_Reg_5_Callback(    
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)        
{                                                               
    const char* dbgMsgText = "TRANSPORT handler for memory Nandside_Sport_Reg";
    scml2::memory< unsigned int >& m = this->Nandside_Sport_Reg_3;    
    tlm::tlm_command cmd = trans.get_command();                //%
    sc_dt::uint64 adr = trans.get_address();                   //%
    unsigned char* ptr = trans.get_data_ptr();                 //%
    unsigned int len = trans.get_data_length();                //%
    unsigned char* byt = trans.get_byte_enable_ptr();          //%
    unsigned int wid = trans.get_streaming_width();            //%
    if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );

    if (byt) { // Target unable to support byte enable attribute
        trans.set_response_status( tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE );
        return;
    }
    if (wid < len) { // Target unable to support streaming width attribute
        trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
        return;
    }
    if (cmd == tlm::TLM_WRITE_COMMAND) { // Execute command

    } else if (cmd == tlm::TLM_READ_COMMAND) {
    }

    Nand_Call_Back(ptr);
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
}
template < unsigned int NUM_CHANNEL >
void                                                           
Cache_Buffer_Controller<NUM_CHANNEL>::Nandside_Sport_Reg_6_Callback(    
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)        
{                                                               
    const char* dbgMsgText = "TRANSPORT handler for memory Nandside_Sport_Reg";
    scml2::memory< unsigned int >& m = this->Nandside_Sport_Reg_3;    
    tlm::tlm_command cmd = trans.get_command();                //%
    sc_dt::uint64 adr = trans.get_address();                   //%
    unsigned char* ptr = trans.get_data_ptr();                 //%
    unsigned int len = trans.get_data_length();                //%
    unsigned char* byt = trans.get_byte_enable_ptr();          //%
    unsigned int wid = trans.get_streaming_width();            //%
    if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );

    if (byt) { // Target unable to support byte enable attribute
        trans.set_response_status( tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE );
        return;
    }
    if (wid < len) { // Target unable to support streaming width attribute
        trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
        return;
    }
    if (cmd == tlm::TLM_WRITE_COMMAND) { // Execute command

    } else if (cmd == tlm::TLM_READ_COMMAND) {
    }

    Nand_Call_Back(ptr);
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
}
template < unsigned int NUM_CHANNEL >
void                                                           
Cache_Buffer_Controller<NUM_CHANNEL>::Nandside_Sport_Reg_7_Callback(    
		tlm::tlm_generic_payload& trans, sc_core::sc_time& t)        
{                                                               
    const char* dbgMsgText = "TRANSPORT handler for memory Nandside_Sport_Reg";
    scml2::memory< unsigned int >& m = this->Nandside_Sport_Reg_3;    
    tlm::tlm_command cmd = trans.get_command();                //%
    sc_dt::uint64 adr = trans.get_address();                   //%
    unsigned char* ptr = trans.get_data_ptr();                 //%
    unsigned int len = trans.get_data_length();                //%
    unsigned char* byt = trans.get_byte_enable_ptr();          //%
    unsigned int wid = trans.get_streaming_width();            //%
    if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );

    if (byt) { // Target unable to support byte enable attribute
        trans.set_response_status( tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE );
        return;
    }
    if (wid < len) { // Target unable to support streaming width attribute
        trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
        return;
    }
    if (cmd == tlm::TLM_WRITE_COMMAND) { // Execute command

    } else if (cmd == tlm::TLM_READ_COMMAND) {
    }

    Nand_Call_Back(ptr);
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
}
template < unsigned int NUM_CHANNEL >
void                                                                            //%
Cache_Buffer_Controller<NUM_CHANNEL>::Hostside_Sport_Reg_Callback(                           //%
        tlm::tlm_generic_payload& trans, sc_core::sc_time& t)           //%
{                                                                               //%
    const char* dbgMsgText = "TRANSPORT handler for memory Hostside_Sport_Reg"; //%
    scml2::memory< unsigned int >& m = this->Hostside_Sport_Reg;                //%
    tlm::tlm_command cmd = trans.get_command();                                 //%
    sc_dt::uint64 adr = trans.get_address();                                    //%
    unsigned char* ptr = trans.get_data_ptr();                                  //%
    unsigned int len = trans.get_data_length();                                 //%
    unsigned char* byt = trans.get_byte_enable_ptr();                           //%
    unsigned int wid = trans.get_streaming_width();                             //%
    //%USERBEGIN Hostside_Sport_Reg_Callback
    if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );
    // TODO: INSERT code for the Hostside_Sport_Reg_Callback method here.

    if (byt) { // Target unable to support byte enable attribute
        trans.set_response_status( tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE );
        return;
    }
    if (wid < len) { // Target unable to support streaming width attribute
        trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
        return;
    }
    std::list < sSubReqWithData_t >::iterator req_itr;
    std::list < sSubReqWithData_t >::iterator data_itr;
    std::list < sSubReq_t >::iterator op_itr;

    sSubReqWithData_t read_request;
    sSubReqWithData_t write_request;

    bool notify_flag = false;
    bool isFull = false;

    //cout << " < Cache Buffer Controller > " << endl;
    if (Seq_Op_Queue.size() <= 0) {
        notify_flag = true;
    }

//    if(IsQueue_Full()){
//        trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
//        return;
//    }
    switch (curr_state)
    {
        case IDLE :
            memcpy(curReq.subReq, ptr, sizeof(sSubReq_t));

            Print_Queue_State();
            if (curReq.subReq->oriReq.Op == HOST_WRITE) {
                //cout << "WRITE IDLE" << endl;
                ///////TB
                //cout << "write recieved request address = " << curReq.subReq->iStartAddr <<endl;
                //cout <<"Cache reciev ID = " << curReq.subReq->oriReq.iId <<"  OP = " <<curReq.subReq->oriReq.Op <<  endl;
                fprintf(inputTrace, "%llu\t%u\t%u\n", curReq.subReq->iStartAddr, curReq.subReq->iLen, 0);
                next_state = CMD;
            }
            else if (curReq.subReq->oriReq.Op == HOST_READ) {
                fprintf(inputTrace, "%llu\t%u\t%u\n", curReq.subReq->iStartAddr, curReq.subReq->iLen, 1);
                read_request.subReq = new sSubReq_t;
                read_request.cpData = new uchar[SECTOR_PER_PAGE * SECTOR_SIZE_BYTE];
                memcpy(read_request.subReq, curReq.subReq, sizeof(sSubReq_t));

                /////////TH
                read_request.Id = reqId++;

                Read_Queue.push_back(read_request);
                Seq_Op_Queue.push_back(*read_request.subReq);

                next_state = IDLE;

                if (notify_flag) {
                    trig_queue_manager.notify();
                }
            }
            break;

        case CMD :
            {
                /*num_req++;
                //cout << "\t\t\t Num Cache Req : " << num_req << endl;*/
                //cout << "WRITE CMD" << endl;
                
                write_request.subReq = new sSubReq_t;
                write_request.cpData = new uchar[SECTOR_PER_PAGE * SECTOR_SIZE_BYTE];

                memcpy(curReq.cpData, ptr, curReq.subReq->iLen*SECTOR_SIZE_BYTE);

                memcpy(write_request.subReq, curReq.subReq, sizeof(sSubReq_t));
                memcpy(write_request.cpData, curReq.cpData, write_request.subReq->iLen * SECTOR_SIZE_BYTE);

                ///////TH
                write_request.Id = reqId++;

                Write_Queue.push_back(write_request);
                Seq_Op_Queue.push_back(*write_request.subReq);
                fprintf(BufferLatency, "1: %lf\n", (double)sc_time_stamp().value()/1000);
                next_state = IDLE;
                if (notify_flag) {
                    trig_queue_manager.notify();
                }
            }
            break;

        default :
            //cout << "\tCurr_state : default" << endl;
            sc_stop();
            break;
    }
    curr_state = next_state;
    Send_RnB_flag();

    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
    //%USEREND Hostside_Sport_Reg_Callback
}    

template < unsigned int NUM_CHANNEL >
void                                                                            //%
Cache_Buffer_Controller<NUM_CHANNEL>::SFR_port_Reg_Callback(                                 //%
        tlm::tlm_generic_payload& trans, sc_core::sc_time& t)           //%
{                                                                               //%
    const char* dbgMsgText = "TRANSPORT handler for memory SFR_port_Reg";       //%
    scml2::memory< unsigned int >& m = this->SFR_port_Reg;                      //%
    tlm::tlm_command cmd = trans.get_command();                                 //%
    sc_dt::uint64 adr = trans.get_address();                                    //%
    unsigned char* ptr = trans.get_data_ptr();                                  //%
    unsigned int len = trans.get_data_length();                                 //%
    unsigned char* byt = trans.get_byte_enable_ptr();                           //%
    unsigned int wid = trans.get_streaming_width();                             //%
    //%USERBEGIN SFR_port_Reg_Callback
    if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );
    // TODO: INSERT code for the SFR_port_Reg_Callback method here.

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
            
            //Copy_SFR_To_CMDList(cmd_size);
            
            //Interrupt_port.write(0);
            //cout << "cache buffer sw complete" << endl;
            e_ScheduleComplete.notify();
//            for(uint i=0;i<NUM_CHANNEL;i++)
//                send_req_to_NAND[i].notify();
//            send_req_to_Cache.notify();

        }else if(adr == R_CPU_CMD_SIZE){
            cmd_size = *((unsigned int *)ptr);
        }else{
            SFR_port_Reg_CMD[(adr/4) - 5] = *((unsigned int *)ptr);
        }
    } else if (cmd == tlm::TLM_READ_COMMAND) {
        // copy from your target's memory to 'ptr', e.g.: memcpy(ptr, &mem[adr], num_bytes);
        if(adr == 0x4)
             *((unsigned int*)ptr) = SFR_port_Reg_ADDR[0];
        else if(adr == 0x8)
             *((unsigned int*)ptr) = SFR_port_Reg_LEN[0];
        else if(adr == 0xc)
             *((unsigned int*)ptr) = SFR_port_Reg_OP[0];
        else if(adr == 0x18){
            Copy_SFR_To_CMDList(cmd_size);
            Interrupt_port.write(0);
            
            for(uint i=0;i<NUM_CHANNEL;i++)
                send_req_to_NAND[i].notify();
            send_req_to_Cache.notify();

            if(Seq_Op_Queue.size() > 0){
                std::list < sSubReq_t >::iterator op_itr;
                HOST_REQ_OP curr_op;
                op_itr = Seq_Op_Queue.begin();
                curr_op = op_itr ->oriReq.Op;

                //////////////////// TH /////////////////////
                Copy_CMD_To_SFR(op_itr->iStartAddr, op_itr->iLen, curr_op);
            }else{
                e_ScheduleComplete.notify();
            }

            *((unsigned int*)ptr) = Seq_Op_Queue.size();
        }

    }
    trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
    //%USEREND SFR_port_Reg_Callback
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
//% begin section previously tagged: FOOTER_H

//% end section previously tagged: FOOTER_H
//% begin section previously tagged: Hostside_RnB_Reg
if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );
// TODO: INSERT code for the Hostside_RnB_Reg method here.

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
} else if (cmd == tlm::TLM_READ_COMMAND) {
    // copy from your target's memory to 'ptr', e.g.: memcpy(ptr, &mem[adr], num_bytes);
}
trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
//% end section previously tagged: Hostside_RnB_Reg
//% begin section previously tagged: FOOTER_H

//% end section previously tagged: FOOTER_H
//% begin section previously tagged: Nandside_RnB_Reg
if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );
// TODO: INSERT code for the Nandside_RnB_Reg method here.

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
} else if (cmd == tlm::TLM_READ_COMMAND) {
    // copy from your target's memory to 'ptr', e.g.: memcpy(ptr, &mem[adr], num_bytes);
}
trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
//% end section previously tagged: Nandside_RnB_Reg
//% begin section previously tagged: FOOTER_H

//% end section previously tagged: FOOTER_H
//% begin section previously tagged: FOOTER_H

//% end section previously tagged: FOOTER_H
//% begin section previously tagged: FOOTER_H

//% end section previously tagged: FOOTER_H
//% begin section previously tagged: Buffer_Sport_Reg
if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );
// TODO: INSERT code for the Buffer_Sport_Reg method here.

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
} else if (cmd == tlm::TLM_READ_COMMAND) {
    // copy from your target's memory to 'ptr', e.g.: memcpy(ptr, &mem[adr], num_bytes);
}
trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
//% end section previously tagged: Buffer_Sport_Reg
//% begin section previously tagged: SFR_port_Reg
if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );
// TODO: INSERT code for the SFR_port_Reg method here.

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
} else if (cmd == tlm::TLM_READ_COMMAND) {
    // copy from your target's memory to 'ptr', e.g.: memcpy(ptr, &mem[adr], num_bytes);
}
trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
//% end section previously tagged: SFR_port_Reg
//% begin section previously tagged: Nandside_Sport_Reg
if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );
// TODO: INSERT code for the Nandside_Sport_Reg method here.

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
} else if (cmd == tlm::TLM_READ_COMMAND) {
    // copy from your target's memory to 'ptr', e.g.: memcpy(ptr, &mem[adr], num_bytes);
}
trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
//% end section previously tagged: Nandside_Sport_Reg
//% begin section previously tagged: FOOTER_H

//% end section previously tagged: FOOTER_H
//% begin section previously tagged: Hostside_Sport_Reg
if ( dbgFlag[REGS] ) DBG_MSG( dbgMsgText );
// TODO: INSERT code for the Hostside_Sport_Reg method here.

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
} else if (cmd == tlm::TLM_READ_COMMAND) {
    // copy from your target's memory to 'ptr', e.g.: memcpy(ptr, &mem[adr], num_bytes);
}
trans.set_response_status( tlm::TLM_OK_RESPONSE ); // Successful completion
//% end section previously tagged: Hostside_Sport_Reg
//% begin section previously tagged: FOOTER_H

//% end section previously tagged: FOOTER_H
//%USEREND ORPHANED_CODE
#endif // ORPHANED_CODE                                                         //%
//%USERBEGIN FOOTER_H

//%USEREND FOOTER_H
#endif // __CACHE_BUFFER_CONTROLLER_H__                                         //%
