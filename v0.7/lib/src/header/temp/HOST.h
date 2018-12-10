HOST:


max_req_len 1024
Q_SIZE
UNIT_OF_REQUEST 512
Req_t
eWWState
SIMUL_MODE

HOST_REQ_OP


SubReq:





Queue parameters
Queue.h

SECTOR_PER_PAGE
MAX_REQ_LEN
DATA_PAGE_SIZE
UNIT_OF_REQUEST

HOST_REQ_OP
Req_t

SubReq_t
PageBuf_t

_ADDR_DRAM_DATA_


DRAM_ctrl:

//let's just start from scratch


READ_Q_SIZE
WRITE_Q_SIZE
DATA_PAGE_SIZE
SOURCE_DEVICE
CacheCMD
CacheTrans_t

mem modeling things

NAND_Manager:

NAND_CMD_QUEUE_SIZE
NAND_BUF_ENTRY

IRQReq_t
directNANDReq_t
NAND_Cmd
NAND_buf

reqHOSTState



NAND Spec
Queue.h

ADDR things



IRQ_Controller:


NAND_ctrl:


NAND_dev:



global:

DEV_AND_TIME

DEBUG flags

DATA_BUS_TOTAL_BW
CMD_BUS_TOTAL_BW

g_initialize_end
g_isFtlReady
g_isCacheReady

buffer_write_count
cache_bypass_count
total_req_count

SSDmetric things (trackbyID)
DTCMP::
