
#define _ADDR_DRAM_CMD_ 0xd0000000 
#define _ADDR_DRAM_DATA_ 0x20000000
#define _ADDR_NAND_CMD_ 0xf0000000
#define _ADDR_NAND_DATA_ 0x30000000
#define _OFFSET_DATA_REGION_ 0x4000000

#define REQUEST_ADDR    0x0
#define DATA_ADDR       0x28


typedef enum eSimulMode{

    SIMUL_RINIT = 0,
    SIMUL_DINIT,
    SIMUL_ON

}SIMUL_MODE;

typedef enum eWriteSlaveState{
    IDLE        =       0,
    CMD                 ,
    NOTHING     =       63
}eWSState;
