#ifndef __BOTTLENECK_METRIC_H__
#define __BOTTLENECK_METRIC_H__


#include <scml2.h>                                                              
#include <systemc>  
#include <iostream>


    namespace BTN_Analysis{


        enum eModuleType{

            HOST_IF,
            DEV_IF,
            SubReqMan,
            DRAM_ctrl,
            NAND_Manager,
            NUM_MODULES

        };

        enum eWaitingState{

            IDLE,
            BUSY_WAITING,
            TRANSFER,
            ACTIVE

        };

        class waiting_time {

            private :

                double last_time;
                void updateTime();
                bool startFlag;

            protected : 
                

                double idle_time;
                double busy_waiting;
                double transfer_time;
                double active_time;

                eWaitingState state;

                               
            public :

                waiting_time(){
                    this->idle_time = 0;
                    this->busy_waiting = 0;
                    this->transfer_time = 0;
                    this->active_time = 0;
                    
                    state = IDLE;

                };

                ~waiting_time(){};                

                void start(); //start recording
                
                void end(); //end recording
                void change_state(eWaitingState cur_state);
            
            
                void printWaitingTime();


        };




        void waiting_time::updateTime(){

            double cur_time = sc_core::sc_time_stamp().value();
            double timediff = cur_time - this->last_time;
            this->last_time = cur_time;

            switch(this->state){

                case IDLE :
                    idle_time += timediff;
                    break;

                case BUSY_WAITING : 
                    busy_waiting += timediff;
                    break;

                case TRANSFER :
                    transfer_time += timediff;
                    break;

                case ACTIVE :
                    active_time += timediff;
                    break;

            }


        }



        void waiting_time::start(){

            double cur_time = sc_core::sc_time_stamp().value(); 
            this->last_time = cur_time; 


        }


        void waiting_time::end(){



            updateTime(); 


        }

        void waiting_time::change_state(eWaitingState cur_state){


            updateTime();
            this->state = cur_state;


        }


        void waiting_time::printWaitingTime(){

            cout <<  "Waiting times : " << endl;
            cout << "\tIDLE : " << this->idle_time << endl;
            cout << "\tBUSY WAITING : " << this->busy_waiting << endl;
            cout << "\tTRANSFER : " << this->transfer_time << endl;
            cout << "\tACTIVE : " << this->active_time << endl << endl;

        }

    }



#endif
