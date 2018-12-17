#ifndef __MEMORY_MODELING_H__
#define __MEMORY_MODELING_H__

/*
#define DRAM_PAGE 4096 // in B
#define DRAM_READ_BW 1.6 // in B/ns (DRAM : 1.6 GB/s, PRAM : 0.72 GB/s) note : PRAM bw is not exact 
#define DRAM_WRITE_BW 1.6 // in B/ns (DRAM : 1.6 GB/s, PRAM : 0.144 GB/s) PRAM bw = DRAM bw * DRAM lat / PRAM lat
#define PRAM_READ_BW 0.2 // in B/ns (DRAM : 1.6 GB/s, PRAM : 0.72 GB/s) note : PRAM bw is not exact 
#define PRAM_WRITE_BW 0.2 // in B/ns (DRAM : 1.6 GB/s, PRAM : 0.144 GB/s) PRAM bw = DRAM bw * DRAM lat / PRAM lat

#define DRAM_ACCESS_BYTE 32 //in B
#define DRAM_READ_LATENCY 45 // in ns (DRAM : 45 ns, PRAM : 100 ns)
#define DRAM_WRITE_LATENCY 45 // in ns (DRAM : 45 ns, PRAM : 100 ns)
#define PRAM_READ_LATENCY 100 // in ns (DRAM : 45 ns, PRAM : 500 ns)
#define PRAM_WRITE_LATENCY 500 // in ns (DRAM : 45 ns, PRAM : 500 ns)


#define CURRENT_DRAM_SIZE 1//MB

*/

typedef enum CACHE_METHOD{
    DRAM_ONLY=0,
    PRAM_ONLY=1,
    HYBRID=2
}cache_method;

CACHE_METHOD CACHE_BUFFER_METHOD=DRAM_ONLY;

enum eCacheStat{
  
    TOTAL_LEN = 0,
    TOTAL_ENERGY = 1,
    CacheStatMAX = 2   

};

enum eCacheType{
  
    BUFFER = 0,
    META = 1,
    CacheTypeMAX   

};

enum eCacheOp{
  
    cacheWRITE = 0,
    cacheREAD = 1,
    cacheOpMAX   

};

enum eMemType{

    memDRAM = 0,
    memPRAM = 1,
    memTypeMAX

};



unsigned int memLatency(uint len, eCacheType type, eCacheOp op){ //len is in bytes

    //sortMemType
    eMemType memtype;

    if( (CACHE_BUFFER_METHOD == DRAM_ONLY) ||
         ((CACHE_BUFFER_METHOD == HYBRID) && (type == BUFFER))  ){
        
        memtype = memDRAM;

    }
    else if( (CACHE_BUFFER_METHOD == PRAM_ONLY) ||
                ((CACHE_BUFFER_METHOD == HYBRID) && (type == META)))  {

        memtype = memPRAM;



    }
    else assert(0 && "no mem of this type");
    
    //
    int access_count = (len) ? ((len - 1)/DRAM_ACCESS_BYTE + 1) : 0;

    if(memtype == memDRAM){
        
        int dram_latency;
        if(op == cacheREAD){
            dram_latency = access_count*DRAM_READ_LATENCY;
        }
        else if(op == cacheWRITE){

            dram_latency = access_count*DRAM_WRITE_LATENCY;
        }
        else assert(0);
        return dram_latency;


    }
    else if (memtype == memPRAM){

        int pram_latency;
        if(op == cacheREAD){

            pram_latency = access_count*PRAM_READ_LATENCY;
        }
        else if(op == cacheWRITE){

            pram_latency = access_count*PRAM_WRITE_LATENCY;
        }
        else assert(0);
        return pram_latency;

    }


}





#endif
