#include "bare_utils.h"
///////////CASTING
unsigned int FLOAT(float f){
    unsigned int* uint_p;

    uint_p = (unsigned int*)&f;
    return *uint_p;
}
unsigned int UINT64_(unsigned long long int uint64, unsigned int b){
    unsigned int* uint_p;

    if(!b)
        uint_p = (unsigned int*)&uint64;
    else
        uint_p = (unsigned int*)&uint64+1;
    return *uint_p;
}
unsigned int DOUBLE_(double d, unsigned int b){
    unsigned int* uint_p;

    if(!b)
        uint_p = (unsigned int*)&d;
    else
        uint_p = (unsigned int*)&d+1;
    return *uint_p;
}

void barePrintf(const char* sentence, ...){

	unsigned int i;
	va_list values;
    unsigned int iInt;
    //float iFloat;
    float* iFloat_p;
    //double iDouble;
    double* iDouble_p;
    unsigned long long int iInt64;
    unsigned int* uint_ptr;

	va_start(values, sentence);

	for (i = 0;; i++)
	{
		if (sentence[i] == 0){
			break;
		}
		else if (sentence[i] == '%'){
			i++;
			if (sentence[i] == 'c'){	// character
#if(_SOFTWARE_SIM_==0)
#ifdef	_MCSSD_ARM_BARE_METAL_
				*(vuint *)(_ADDR_PRINT_CHAR_) = ((unsigned int)va_arg(values, unsigned int));
#endif
#else
				printf("%c", va_arg(values, unsigned char));
#endif
			}
			else if (sentence[i] == 'd'){	// integer
#if(_SOFTWARE_SIM_==0)
#ifdef	_MCSSD_ARM_BARE_METAL_
				*(vuint *)(_ADDR_PRINT_INT_) = ((unsigned int)va_arg(values, unsigned int));
#endif
#else
				printf("%d", va_arg(values, int));
#endif
			}
			else if (sentence[i] == 'f'){	// float
#if(_SOFTWARE_SIM_==0)
#ifdef	_MCSSD_ARM_BARE_METAL_
				*(vuint *)(_ADDR_PRINT_FLOAT_) = va_arg(values, unsigned int);
#endif
#else
                iInt = (unsigned int)va_arg(values, unsigned int);
                iFloat_p = (float*)&iInt;
				printf("%f", *iFloat_p);
#endif
			}
			else if (sentence[i] == '%'){
#if(_SOFTWARE_SIM_==0)
#ifdef	_MCSSD_ARM_BARE_METAL_
				*(vuint *)(_ADDR_PRINT_CHAR_) = sentence[i];
#endif
#else
				printf("%c", '%');
#endif
			}
            else if (sentence[i] == 'x' || sentence[i] == 'X'){
#if(_SOFTWARE_SIM_==0)
#ifdef	_MCSSD_ARM_BARE_METAL_
                *(vuint *)(_ADDR_PRINT_HEX_) = ((unsigned int)va_arg(values, unsigned int));
#endif
#else
                printf("%x", va_arg(values, unsigned int));
#endif                
            }
			else if (sentence[i] == 'l' && sentence[i+1] == 'd'){
                i++;
#if(_SOFTWARE_SIM_==0)
#ifdef	_MCSSD_ARM_BARE_METAL_
				*(vuint *)(_ADDR_PRINT_INT64_MSB_) = va_arg(values, unsigned int);
                *(vuint *)(_ADDR_PRINT_INT64_LSB_) = va_arg(values, unsigned int);
#endif
#else
                iInt = va_arg(values, unsigned int);
                iInt64 = (unsigned long long int)iInt << 32;
                iInt = va_arg(values, unsigned int);
                iInt64 += (unsigned long long int)iInt & 0xFFFFFFFF;
				printf("%l", iInt64);
#endif
			}
			else if (sentence[i] == 'l' && sentence[i+1] == 'f'){
                i++;
#if(_SOFTWARE_SIM_==0)
#ifdef	_MCSSD_ARM_BARE_METAL_
				*(vuint *)(_ADDR_PRINT_DOUBLE_MSB_) = va_arg(values, unsigned int);
				*(vuint *)(_ADDR_PRINT_DOUBLE_LSB_) = va_arg(values, unsigned int);
#endif
#else
                iInt = va_arg(values, unsigned int);
                iInt64 = (unsigned long long int)iInt << 32;
                iInt = va_arg(values, unsigned int);
                iInt64 += (unsigned long long int)iInt & 0xFFFFFFFF;
                iDouble_p = (double*)&iInt64;
				printf("%lf", *iDouble_p);
#endif
			}
			else if (sentence[i] == 'l' && sentence[i+1] == 'x'){
                i++;
#if(_SOFTWARE_SIM_==0)
#ifdef	_MCSSD_ARM_BARE_METAL_
				*(vuint *)(_ADDR_PRINT_INT64HEX_MSB_) = va_arg(values, unsigned int);
                *(vuint *)(_ADDR_PRINT_INT64HEX_LSB_) = va_arg(values, unsigned int);
#endif
#else
                iInt = va_arg(values, unsigned int);
				printf("%x%x", iInt, va_arg(values, unsigned int));
#endif
			}
			else{
				barePrintf(" ### Unknown identifier for barePrint.\n");
				exit(1);
			}
		}
		else{
			// just print this;
#if(_SOFTWARE_SIM_==0)
#ifdef	_MCSSD_ARM_BARE_METAL_
			*(vuint *)(_ADDR_PRINT_CHAR_) = sentence[i];
#endif
#else
			printf("%c", sentence[i]);
#endif
		}
	}

	va_end(values);
}
/*
void bareSetStart(unsigned int iId){
	barePrintf("_START%d_", iId);
	*(vuint *)(_ADDR_SW_ROUTINE_START_) = iId;
};

void bareSetEnd(unsigned int iId){
	barePrintf("_END%d_", iId);
	*(vuint *)(_ADDR_SW_ROUTINE_END_) = iId;
};
*/
unsigned __user_heap_extend(int a, void **base, unsigned requested_size){
    
    static unsigned int curHeapEnd = 0x1300000;

    if(_ADDR_HEAP_LIMIT_ - curHeapEnd < requested_size)
    {
        barePrintf(" ### no more heap space is available\n");
        return 0;
    }

    if(requested_size > 1024*1024){
        barePrintf(" ### Requested size is too big to allocate\n");
        return 0;
    }

    curHeapEnd += 1024*1024;
    return 1024*1024;
    
}

//unsigned __rt_heap_extend(unsigned size, void **block){
//    barePrintf(" ## heap is full!!!!\n");
//    exit(1);
//    return 0;
//}

#ifdef _MCSSD_ARM_BARE_METAL_

__value_in_regs struct __heap_extent __user_heap_extent(unsigned ignore1, unsigned ignore2){
    sHeap new_heap;

    new_heap.base = _ADDR_HEAP_BASE_;
    new_heap.range = _ADDR_HEAP_LIMIT_;

    return new_heap;
}

#endif
