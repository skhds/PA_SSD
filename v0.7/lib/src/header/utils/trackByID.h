#ifndef __TRACKBYID_H__
#define __TRACKBYID_H__


#include "trackByID_node.h"
#include <string>


namespace SSDmetric{


    class trackByID {

        private :

            std::deque<nodeReqID> reqList;
            int id_front;
            ofstream fp;

        protected :

        public :


            trackByID(string const & filename);

            void insertID(int id, int LBA, int len, uint op); 

            void start(int id, int subLBA, int len, eModuleType module, double time);

            void track(int id, int subLBA, int len, eModuleType module, double time);

            void done(int id, int subLBA, int len, eModuleType module, double time);

            void IDFinish();

            void IDForceFinish(ofstream &fp);



    };

    trackByID::trackByID(string const & filename) : fp(filename.c_str(), ios::out){//warning : imported using const from web without understanding, may cause problems (that I can't solve) 

        this->id_front = 0;
    }

    void trackByID::insertID(int id, int LBA, int len, uint op){

        this->reqList.push_back(nodeReqID(id, LBA, len, (CMD_OP)op));


    }


    void trackByID::start(int id, int subLBA, int len, eModuleType module, double time){

        int index = id - this->id_front;
        assert(index < this->reqList.size());

        int lba_offset = subLBA - this->reqList[index].getLBA();
        assert( lba_offset >=0);

        for(int subID = lba_offset/SECTOR_PER_PAGE; 
                subID < (lba_offset + len + SECTOR_PER_PAGE - 1)/SECTOR_PER_PAGE; subID++){

            this->reqList[index].start(subID, module, time);

        }

    }


    void trackByID::track(int id, int subLBA, int len, eModuleType module, double time){

        int index = id - this->id_front;
        assert(index < this->reqList.size());

        int lba_offset = subLBA - this->reqList[index].getLBA();
        assert( lba_offset >=0);

        for(int subID = lba_offset/SECTOR_PER_PAGE; 
                subID < (lba_offset + len + SECTOR_PER_PAGE - 1)/SECTOR_PER_PAGE; subID++){

            this->reqList[index].track(subID, module, time);

        }

    }
    void trackByID::done(int id, int subLBA, int len, eModuleType module, double time){

        int index = id - this->id_front;
        assert(index < this->reqList.size());


        int lba_offset = subLBA - this->reqList[index].getLBA();
        assert( lba_offset >=0);

        assert( !this->reqList[index].isEnd() ); //done is called twice

        for(int subID = lba_offset/SECTOR_PER_PAGE; 
                subID < (lba_offset + len + SECTOR_PER_PAGE - 1)/SECTOR_PER_PAGE; subID++){

            this->reqList[index].done(subID, module, time);

        }
    }


    void trackByID::IDFinish(){
        //side note : planning on overthrowing every dumptrace functions (hard to configure the layout)
        //attempts to dump trace of an ID

        if(this->reqList.empty()) return;   

        while(this->reqList.front().isEnd()) {

            this->reqList.front().dumptrace(this->fp); //temp
            this->id_front += 1;
            this->reqList.pop_front();
            if(this->reqList.empty()) break;   

        }
    }

    void trackByID::IDForceFinish(ofstream &fp_debug){



        while(!this->reqList.empty()) {

            this->reqList.front().dumptrace(fp_debug); 
            this->id_front += 1;
            this->reqList.pop_front();
        } 

    }



}










#endif
