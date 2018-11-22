#ifndef __DATA_COMPARE_H__
#define __DATA_COMPARE_H__

#include "ssd_struct.h"
#include "IF_Spec.h"
#include "NAND_Spec.h"

#define DATA_COMPARE_ON

//ok, im going to have to clean up the headers, just wait...
//

#define DRAM_ENTRY_SIZE 64
#define SECTOR_BYTES 512
namespace DTCMP{
    typedef struct sMemory_map_entry{

        int  lpa = -1;
        uchar data[SECTOR_PER_PAGE] = {0};

    }memMapEntry;

    typedef enum sMemMapType{
        mmLogical = 0,
        mmDRAM = 1,
        mmNAND = 2
    }mmType;




    uchar logical_memory_map[MAX_ADDR] = {0};
    memMapEntry DRAM_memory_map[DRAM_ENTRY_SIZE];
    memMapEntry NAND_memory_map[NUM_OF_CHANNEL*NUM_OF_WAY*NUM_OF_BLOCK_PER_WAY*NUM_OF_PAGE_PER_BLOCK];

    void dataCopyBtP(uchar* dst, uchar* src, uint len){ //from byte-granularity to sector-granularity

        for(int i=0; i<len; i++){
            dst[i] = src[i*SECTOR_BYTES];

        }

    }

    bool confirm_data(){

        bool compare_true = true;
#ifdef DATA_COMPARE_ON
        cout << "Comparing DRAM Data...... " << endl;

        for(int i =0; i<DRAM_ENTRY_SIZE; i++){
            if(DRAM_memory_map[i].lpa >= 0 ){
                for(int j=0; j<SECTOR_PER_PAGE; j++){

                    if(DRAM_memory_map[i].data[j] != logical_memory_map[DRAM_memory_map[i].lpa  + j] ){
                        compare_true = false;
                        cout << "False entry detected! logical addr :\t" << (DRAM_memory_map[i].lpa + j ) << "\tDRAM addr : " << i*SECTOR_PER_PAGE + j << endl;//add false entry
                    }
                }
            }

        }
        cout << "Result was : " << compare_true << endl;
        cout << endl << endl << "Comparing NAND Data...... " << endl;
        for(int i =0; i< NUM_OF_CHANNEL*NUM_OF_WAY*NUM_OF_BLOCK_PER_WAY*NUM_OF_PAGE_PER_BLOCK; i++){

            if(NAND_memory_map[i].lpa >= 0) {
                for(int j=0; j<SECTOR_PER_PAGE; j++){
                    if(NAND_memory_map[i].data[j] != logical_memory_map[DRAM_memory_map[i].lpa + j] ){
                        compare_true = false;
                        std::cout << "False entry detected! logical addr :\t" << (NAND_memory_map[i].lpa + j ) << "\tNAND addr : " << i*SECTOR_PER_PAGE + j << endl;//add false entry
                    }
                }
            }

        }
        cout << "Result was : " << compare_true << endl;

#endif

        return compare_true;


    }


    void writeData(mmType memtype, uint ppa, uint lpa, int len, uchar* data){

#ifdef DATA_COMPARE_ON

        switch(memtype){

            case mmLogical :

                assert(lpa + len <= MAX_ADDR); 
                dataCopyBtP(logical_memory_map + lpa, data, len);

                break;
            case mmDRAM :

                DRAM_memory_map[ppa].lpa = lpa;
                dataCopyBtP(DRAM_memory_map[ppa].data, data, len); 

                // DRAM_memory_map[ppa].
                // logical_memory_map[lpa


                break;

            case mmNAND :

                NAND_memory_map[ppa].lpa = lpa;
                dataCopyBtP(DRAM_memory_map[ppa].data, data, len); 
                break;

        }

#endif

    }


}

#endif


