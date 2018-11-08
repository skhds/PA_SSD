#ifndef _QUEUE_H_
#define _QUEUE_H_


//using macros because i dont like <><><> kind of shit (C++ templates)


#define REF_M_QUEUE(atype, asize) sQueue_##atype 

#define DEC_M_QUEUE(atype, asize) \
    typedef struct REF_M_QUEUE(atype, asize) \
{ \
    atype _queue[asize];  \
    int head; \
    int tail; \
    int size;\
    int elesize;\
}REF_M_QUEUE(atype, asize); \
void initMQueue##atype(REF_M_QUEUE(atype, asize) queue){\
}

#define REF_M_INIT(atype, asize, name) initMQueue##atype(name)

//the API

#define M_CREATEQUEUE(name, atype, asize) DEC_M_QUEUE(atype, asize); \
    REF_M_QUEUE(atype, asize) name

#define M_INITQUEUE(queue, atype, asize) queue.head = 0;\
    queue.tail = 0;\
    queue.size = asize;\
    queue.elesize = sizeof(atype)

#define M_CHECKFULL(queue) (queue.head == (queue.tail + 1) % queue.size)

#define M_CHECKEMPTY(queue) (queue.head == queue.tail)

#define M_PUSH(queue, eleptr) assert(!M_CHECKFULL(queue)); \
    memcpy(&(queue._queue[queue.tail]), eleptr, queue.elesize); \
    queue.tail = (queue.tail + 1) % queue.size

//no return i dont wanna burn my head anymore
#define M_POP(queue) assert(!M_CHECKEMPTY(queue)); \
    queue.head = (queue.head + 1) % queue.size

#define M_GETELE(queue) queue._queue[queue.head]


#endif

