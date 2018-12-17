#ifndef __DATA_COMPARE_H__
#define __DATA_COMPARE_H__


//#define DATA_COMPARE_ON

//ok, im going to have to clean up the headers, just wait...
//

//#define DRAM_ENTRY_SIZE 64
#define SECTOR_BYTES 512
namespace DTCMP{
    typedef struct sMemory_map_entry{ //page
        uint lpa;
        uint bitmap;

    }memMapEntry;

    typedef enum sMemMapType{
        mmLogical = 0,
        mmDRAM = 1,
        mmNAND = 2
    }mmType;

    uchar logical_data[MAX_ADDR] = {0};
    uchar RAM_data[DRAM_ENTRY_SIZE * SECTOR_PER_PAGE] = {0};
    //uchar NAND_data[DRAM_ENTRY_SIZE * SECTOR_PER_PAGE] = {0};
    memMapEntry RAM_memMap[DRAM_ENTRY_SIZE];

    void dataCopyBtP(uchar* dst, uchar* src, uint len){ //from byte to page

        for(int i=0; i<len; i++){
            dst[i] = src[i*SECTOR_BYTES];

        }

    }


    void updateMap(mmType memtype, uint ppa, uint lpa, uint bitmap){
        
        
        switch(memtype){

            case mmLogical :

                assert(0); 
                break;
            
            case mmDRAM :

                assert(ppa < DRAM_ENTRY_SIZE);
                RAM_memMap[ppa].lpa = lpa; 
                RAM_memMap[ppa].bitmap |= bitmap; 


                break;

            case mmNAND :
                //not yet man
                assert(0);
                //NAND_memory_map[ppa].lpa = lpa;
                //dataCopyBtP(DRAM_memory_map[ppa].data, data, len); 
                break;

        }
        
        

    }

    void writeData(mmType memtype, uint ppa, uchar* data, int len){

#ifdef DATA_COMPARE_ON

        switch(memtype){

            case mmLogical :

                assert(ppa + len <= MAX_ADDR); 
                dataCopyBtP(logical_data + ppa, data, len);

                break;
            case mmDRAM :

                assert(ppa + len <= DRAM_ENTRY_SIZE * SECTOR_PER_PAGE);
                dataCopyBtP(RAM_data + ppa, data, len); 

                // DRAM_memory_map[ppa].
                // logical_memory_map[lpa


                break;

            case mmNAND : //not yet
                assert(0);
                //NAND_memory_map[ppa].lpa = lpa;
                //dataCopyBtP(DRAM_memory_map[ppa].data, data, len); 
                break;

        }

#endif

    }

    bool confirm_data(){

        bool compare_true = true;
#ifdef DATA_COMPARE_ON
        cout << "Comparing DRAM Data...... " << endl;

        for(int i =0; i<DRAM_ENTRY_SIZE; i++){
            if(RAM_memMap[i].bitmap != 0 ){
                uint bitmap = RAM_memMap[i].bitmap;
                
                for(int j=0; bitmap; j++){
                    
                    if(bitmap % 2){

                        if(! (RAM_data[i*SECTOR_PER_PAGE + j] \
                                == logical_data[SECTOR_PER_PAGE * RAM_memMap[i].lpa  + j])){
                          cout << "False entry, logical addr :\t" << (RAM_memMap[i].lpa + j ) << "\tDRAM addr : " << i*SECTOR_PER_PAGE + j << endl;//add false entry
                        
                           compare_true = false;
                        }
                    }
                    bitmap >>=1;
                    assert(j<=SECTOR_PER_PAGE);
                }
            }

        }
        cout << "Result was : " << compare_true << endl;
        
        /*
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
*/
#endif

        return compare_true;


    }

}

#endif


