#ifndef __DTL_MEMCPY_H__
#define __DTL_MEMCPY_H__

#define ABORT(cond,msg) {\
    if(cond){ \
        std::cout << "[" << sc_core::sc_time_stamp() << "] " <<  __FILE__ << ":" << __LINE__ << " " << msg << std::endl;   \
        sc_stop();} }

static inline void memcpy(void* target, scml2::memory<unsigned int> &src, int length)
{
    ABORT(length%4!=0, "length shoud be multiple of 4");
    unsigned int *target_i = (unsigned int*) target;
    for(unsigned int i=0; i<length/4; i++)
    {
        target_i[i] = src[i];
    }
}
static inline void memcpy(scml2::memory<unsigned int> &target, void* src, int length)
{
    ABORT(length%4!=0, "length shoud be multiple of 4");
    unsigned int *src_i = (unsigned int*) src;
    for(unsigned int i=0; i<length/4; i++)
    {
        target[i] = src_i[i];
    }
}
static inline void memcpy(void* target, scml2::memory_alias<unsigned int> &src, int length)
{
    ABORT(length%4!=0, "length: " << length << "  length shoud be multiple of 4");
    unsigned int *target_i = (unsigned int*) target;
    for(unsigned int i=0; i<length/4; i++)
    {
        target_i[i] = src[i];
    }
}
static inline void memcpy(scml2::memory_alias<unsigned int> &target, void* src, int length)
{
    ABORT(length%4!=0, "length shoud be multiple of 4");
    unsigned int *src_i = (unsigned int*) src;
    for(unsigned int i=0; i<length/4; i++)
    {
        target[i] = src_i[i];
    }
}
static inline void memcpy(void* target, scml2::reg<unsigned int> &src, int length)
{
    ABORT(length != 4, "length shoud be 4");
    *(unsigned int*)target = src;
}
static inline void memcpy(scml2::reg<unsigned int> target, void* &src, int length)
{
    ABORT(length != 4, "length shoud be 4");
    target = *(unsigned int*)src;
}
#endif
