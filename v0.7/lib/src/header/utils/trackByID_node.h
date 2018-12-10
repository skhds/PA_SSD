#ifndef __TRACKBYID_NODE_H__
#define __TRACKBYID_NODE_H__


#include <scml2.h>                                                              
#include <systemc>  
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <iomanip>




    namespace SSDmetric{

        double      g_startTime;

        enum eModuleType{

            HOST_IF = 0,
            DEV_IF = 1,
            SubReqMan = 2,
            DRAM_ctrl = 3,
            NAND_Manager = 4,
            NUM_MODULES = 5

        };

        enum CMD_OP{
            
          CMD_WRITE = 0,
          CMD_READ = 1
            
        };




        class nodeModule {

            protected : 
                double startTime; //at callback
                std::vector<double> timings; //track points
                double finishTime; //at a request finish in a module
                bool end;
                eModuleType module;

            public :

                nodeModule(eModuleType module){
                    this->module = module;
                    this->end = false;
                    this->startTime = -1;
                    this->finishTime = -1;
                };

                void start(double time){
                    
                    this->startTime = time - g_startTime;
                    this->end = false;
                    
                }

                void track(double time){

                    this->timings.push_back(time - g_startTime);
                
                }

                void done(double time){

                    this->finishTime = time - g_startTime;
                    this->end = true;
                }
                void dumptrace(ofstream &fp){
                    
                    //just going to use time stamp for now
                    fp << std::setprecision( std::numeric_limits<double>::max_digits10 );
                    fp << module << "\t" << startTime;
                    for(int i=0; i < this->timings.size(); i++) fp << "\t" << this->timings[i];
                    fp << "\t" << finishTime;
                
                
                }
        };





        class nodeSubReq{

            private :

                int lba_offset;
                int SubReq_len;
                std::vector<nodeModule> TimeList;
                CMD_OP op;

            public :
                
                nodeSubReq(int offset, CMD_OP op, int len){
                    this->lba_offset = offset;
                    this->SubReq_len = len;

                    this->TimeList.reserve(NUM_MODULES);
                    for(int i=0; i<NUM_MODULES; i++){
                        this->TimeList.push_back(nodeModule((eModuleType) i));
                    } 

                    this->op = op;

                }


                void start(eModuleType module, double time){

                    assert((int)module < this->TimeList.size());
                    this->TimeList[(int)module].start(time);
                }


                void track(eModuleType module, double time){

                    assert((int)module < this->TimeList.size());
                    this->TimeList[(int)module].track(time);
                }

                void done(eModuleType module, double time){

                    assert((int)module < this->TimeList.size());
                    this->TimeList[module].done(time);//insert time

                }


                void dumptrace(ofstream &fp, int id, CMD_OP op){

                    for(int i=0; i<NUM_MODULES; i++){
                        fp << id <<"\t" << op << "\t" << this->lba_offset << "\t" << this->SubReq_len << "\t";
                        assert(i < this->TimeList.size());
                        this->TimeList[i].dumptrace(fp);
                        fp << endl;
                    }

                }


        };



        class nodeReqID {


            private :


                int id;
                int LBA;
                int numSubReq;
                int reqLen;
                std::vector<nodeSubReq> SubList;
                CMD_OP op;
                unsigned long long int completeReqFlags; //consider this as group of flags

                               
            public :

                nodeReqID(int id, int LBA, int len, CMD_OP op){
                    
                    int numSubID = (len + SECTOR_PER_PAGE - 1) / SECTOR_PER_PAGE ; //rounded up
                    this->id = id;
                    this->LBA = LBA;
                    this->reqLen = len;
                    this->SubList.reserve(numSubID);
                    this->numSubReq = numSubID;
                    this->op = op;

                    int remaining_len = this->reqLen;
                    int insert_len;
                    for(int i=0; i<numSubID; i++){
                        insert_len = remaining_len <= SECTOR_PER_PAGE ? remaining_len : SECTOR_PER_PAGE;
                        this->SubList.push_back(nodeSubReq( i*SECTOR_PER_PAGE, op, insert_len) );
                        remaining_len -= SECTOR_PER_PAGE;
                    }
                    this->completeReqFlags = 0;

                };


                    




                void start(int subID, eModuleType module, double time){
                    
                    assert(subID < this->SubList.size());
                    this->SubList[subID].start(module, time); 

                    
                    
                };

                void track(int subID, eModuleType module, double time){
                    
                    assert(subID < this->SubList.size());
                    this->SubList[subID].track(module, time); 

                    
                    
                };

                void done(int subID, eModuleType module, double time){
                    
                    assert(subID < this->SubList.size());
                    assert(subID < this->numSubReq);
                    this->SubList[subID].done(module, time);
                    
                    if ((this->op == CMD_WRITE)&&(module == HOST_IF)){ //write is done when it's finished on NAND
                        assert(subID < 64); //to prevent integer overflow
                        this->completeReqFlags |= 1ULL << subID; 
                    }
                    else if ((this->op == CMD_READ)&&(module == SubReqMan)){ //write is done when it's finished on NAND
                        assert(subID < 64); //to prevent integer overflow
                        this->completeReqFlags |= 1ULL << subID; 
                    }
                };


                void dumptrace(ofstream &fp){
                    
                    
                    for(int i=0; i<this->numSubReq; i++){
                        //fp << this->id << "\t" << this->op <<  "\t";
                        assert(i < this->SubList.size());
                        this->SubList[i].dumptrace(fp, this->id, this->op);
                        //fp << endl;

                    }
                    
                };


                bool isEnd(){

                    return (this->completeReqFlags + 1 == (1ULL << this->numSubReq));
                }

                int getLBA(){
                    return this->LBA;
                }



        };


    
    
    
    }



#endif
