#include "device_if.h"

Req_t deviceIFDATA[32];
Req_t tempData[32];
uint flag[32];

void initDeviceIF()
{
	uint i;

	for (i = 0; i < 32; i++){
		deviceIFDATA[i].dIssueTime = 0;
		deviceIFDATA[i].iAddr = 0;
		deviceIFDATA[i].iId = 0;
		deviceIFDATA[i].iLen = 0;
		deviceIFDATA[i].Op = 0;
		
		tempData[i].dIssueTime = 0;
		tempData[i].iAddr = 0;
		tempData[i].iId = 0;
		tempData[i].iLen = 0;
		tempData[i].Op = 0;

        flag[i] = 0;
	}
	
	return;
}

void callDeviceIFSched(uint bufferSize)
{
	
	uint i, j, tempDataPoint=0;
    memorySize +=1;

	for (i = 0; i < bufferSize; i++){

        flag[i] = 0;

	}


	// Read priority
	for (i = 0; i < bufferSize; i++){
		if (deviceIFDATA[i].Op == HOST_READ){	//1 = Read
			tempData[tempDataPoint].iAddr  		= deviceIFDATA[i].iAddr;
			tempData[tempDataPoint].iId 		= deviceIFDATA[i].iId;
			tempData[tempDataPoint].Op  		= deviceIFDATA[i].Op;			
			tempData[tempDataPoint].iLen  		= deviceIFDATA[i].iLen;
			tempData[tempDataPoint].dIssueTime  = deviceIFDATA[i].dIssueTime;
			flag[i] = 1;
			tempDataPoint++;
			
		}else if((deviceIFDATA[i].Op == HOST_WRITE)&&(CheckOverlap(bufferSize,deviceIFDATA,i)==3)){
			tempData[tempDataPoint].iAddr  		= deviceIFDATA[i].iAddr;
			tempData[tempDataPoint].iId 		= deviceIFDATA[i].iId;
			tempData[tempDataPoint].Op  		= deviceIFDATA[i].Op;			
			tempData[tempDataPoint].iLen  		= deviceIFDATA[i].iLen;
			tempData[tempDataPoint].dIssueTime  = deviceIFDATA[i].dIssueTime;
			flag[i] = 1;
			tempDataPoint++;
		}
	}

	for (i = 0; i < bufferSize; i++){
		if ((deviceIFDATA[i].Op == HOST_WRITE)&&(flag[i]==0)){
			tempData[tempDataPoint].iAddr  		= deviceIFDATA[i].iAddr;
			tempData[tempDataPoint].iId 		= deviceIFDATA[i].iId;
			tempData[tempDataPoint].Op  		= deviceIFDATA[i].Op;			
			tempData[tempDataPoint].iLen  		= deviceIFDATA[i].iLen;
			tempData[tempDataPoint].dIssueTime  = deviceIFDATA[i].dIssueTime;
			flag[i] = 1;
			tempDataPoint++;
		}
	}
	

	// Data copy to Device interface

	for (i = 0; i<bufferSize; i++)
	{
		Req_t *tmp = &tempData[i];
		unsigned int *tmp_int = (unsigned int *)tmp;

		for (j = 0; j<7; j++)
		{
			*((vuint *)((_ADDR_DEV_IF_BASE_ + _OFFSET_R_DATA_) + _INTERVAL_R_DATA_*j + _INTERVAL_R_DATA_*i * 7)) = tmp_int[j];
		}
	}

    memorySize -= 1;
	// Complete
	*((vuint *)(_ADDR_DEV_IF_BASE_ + _OFFSET_R_COMPLETE_)) = 0x1;
	
	return;
}

int CheckOverlap(uint buffersize, Req_t* temp, int offset){ ////////////////// Left/R = 1, Left/W = 2, Right/R = 3, Right/W = 4
	int i;

	for(i=0;i<buffersize;i++){
		if(i<offset){
			if((temp[i].iAddr<=temp[offset].iAddr)&&((temp[i].iAddr+temp[i].iLen)>temp[offset].iAddr)){ ////////////////// #1
				if(temp[i].Op == HOST_READ)
					return 1;
				else
					return 2;
			}
			else if((temp[offset].iAddr<=temp[i].iAddr)&&((temp[offset].iAddr+temp[offset].iLen)>temp[i].iAddr)){ ////////////////// #2
				if(temp[i].Op == HOST_READ)
					return 1;
				else
					return 2;
			}
			
		}else if(i>offset){
			if((temp[i].iAddr<=temp[offset].iAddr)&&((temp[i].iAddr+temp[i].iLen)>temp[offset].iAddr)){ ////////////////// #1
				if(temp[i].Op == HOST_READ)
					return 3;
				else
					return 4;
			}
			else if((temp[offset].iAddr<=temp[i].iAddr)&&((temp[offset].iAddr+temp[offset].iLen)>temp[i].iAddr)){ ////////////////// #2
				if(temp[i].Op == HOST_READ)
					return 3;
				else
					return 4;
			}
		}
	}

	return 0;

}
