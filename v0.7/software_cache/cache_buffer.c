#include "cache_buffer.h"

NODE	node[LIST_SIZE];
COMMAND cmd_list[100];
uint	cmd_count;
HASH_NODE hash_front[HASH_MAX];
HASH_NODE hash_node[ENTRY_SIZE];
HASH_NODE hash_free_front;

NODE* evict_table[EVICT_MAX];
uint evict_table_ptr = 0;

uint used_count = 0;
uint evict_count = 0;

unsigned long long dram_count = 0;
unsigned long long nand_count = 0;


M_CREATEQUEUE(EvictDone_Queue, uint, EVICT_MAX);


void Create_Command(OP op, uint slave_addr, uint slave_bitmap, uint dram_id, uint dram_bitmap)
{

//    barePrintf("[CPU1] op value : %d %d\n", op, WRITE_DRAM);
    cmd_list[cmd_count].op = op;
    cmd_list[cmd_count].slave_addr = slave_addr;
    cmd_list[cmd_count].slave_bitmap = slave_bitmap;
    cmd_list[cmd_count].dram_id = dram_id;
    cmd_list[cmd_count].dram_bitmap = dram_bitmap;

    //cmd_count++;
}

void List_Push_Back(NODE* back, NODE* n)
{
    n->prev = back->prev;
    n->next = back;
    back->prev->next = n;
    back->prev = n;
}

void List_Push_Front(NODE* front, NODE* n)
{
    n->prev = front;
    n->next = front->next;
    front->next->prev = n;
    front->next = n;
}

uint List_Is_Empty(NODE* front)
{
    return (front->next->next == (void*)NOTHING);
}


NODE* List_Pop_Middle(NODE* evict_node)
{
    evict_node->prev->next = evict_node->next;
    evict_node->next->prev = evict_node->prev;
    return evict_node;
}
NODE* List_Pop_Back(NODE* back)
{
    NODE* evict_node = back->prev;
    return List_Pop_Middle(evict_node);
}
NODE* List_Pop_Front(NODE* front)
{
    NODE* evict_node = front->next;
    NODE* ret = List_Pop_Middle(evict_node);
    return ret;
}





NODE* List_Find_Node(NODE* front, uint addr)
{
    NODE* ptr = front->next;
    while (ptr->addr != (uint)NOTHING && ptr->next != (void *)NOTHING)
    {
        if (ptr->addr == addr)
        {
            return ptr;
        }
        else
        {
            ptr = ptr->next;
        }
    }
    return (void *)NOTHING;
}



void Hash_Push_Front(HASH_NODE* front, HASH_NODE* node)
{
    if (front->next != (void*)NOTHING)
    {
        node->prev = front;
        node->next = front->next;
        front->next->prev = node;
        front->next = node;
    }
    else
    {
        front->next = node;
        node->prev = front;
        node->next = (void*)NOTHING;
    }
}

HASH_NODE* Hash_Pop_Front(HASH_NODE* front) 
{
    HASH_NODE* ret_ptr = front->next;
    if (ret_ptr != (void*)NOTHING)
    {
        if (ret_ptr->next != (void*)NOTHING)
        {
            front->next = front->next->next;
            front->next->prev = front;
        }
        else
            front->next = (void*)NOTHING;
    }
    return ret_ptr;
}



HASH_NODE* Hash_Delete_Node(HASH_NODE* node)
{
    if (node->next != (void*)NOTHING)
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    else
    {
        node->prev->next = (void*)NOTHING;
    }
    return node;
}


HASH_NODE* Hash_Find_And_Delete_Node(HASH_NODE* front, uint addr)
{
    HASH_NODE* ptr = front->next;
    while (ptr != (void*)NOTHING)
    {
        if (ptr->addr == addr)
        {
            return Hash_Delete_Node(ptr);
        }
        ptr = ptr->next;
    }
    return (void*)NOTHING;
}

HASH_NODE* Hash_Find_Node(HASH_NODE* front, uint addr)
{
    HASH_NODE* ptr = front->next;
    while (ptr != (void*)NOTHING)
    {
        if (ptr->addr == addr)
        {
            return ptr;
        }
        ptr = ptr->next;
    }
    return (void*)NOTHING;
}



void List_Init()
{
    uint i;
    used_count = 0;
    evict_count = 0;

    M_INITQUEUE(EvictDone_Queue, uint, EVICT_MAX);
    for (i = 0; i < LIST_SIZE; i++)
    {
        node[i].id = i;
        node[i].next = (void *)NOTHING;
        node[i].prev = (void *)NOTHING;
        node[i].addr = (uint)NOTHING;
        node[i].bitmap = 0;

    }

    node[USED_FRONT_NODE].next = &node[USED_BACK_NODE];
    node[USED_BACK_NODE].prev = &node[USED_FRONT_NODE];

    node[FREE_FRONT_NODE].next = &node[FREE_BACK_NODE];
    node[FREE_BACK_NODE].prev = &node[FREE_FRONT_NODE];

    node[EVICT_FRONT_NODE].next = &node[EVICT_BACK_NODE];
    node[EVICT_BACK_NODE].prev = &node[EVICT_FRONT_NODE];
    
    for (i = 0; i < LIST_SIZE; i++)
    {
        if (i != FREE_FRONT_NODE && i != FREE_BACK_NODE && i != USED_FRONT_NODE && i != USED_BACK_NODE
                && i!= EVICT_FRONT_NODE && i != EVICT_BACK_NODE)
            List_Push_Back(&node[FREE_BACK_NODE], &node[i]);
    }

    hash_free_front.addr = NOTHING;
    hash_free_front.link = (void *)NOTHING;
    hash_free_front.prev = (void *)NOTHING;
    hash_free_front.next = (void *)NOTHING;

    for (i = 0; i < ENTRY_SIZE; i++)
    {
        hash_node[i].next = (void *)NOTHING;
        hash_node[i].prev = (void *)NOTHING;
        hash_node[i].link = (void *)NOTHING;
        hash_node[i].addr = (uint)NOTHING;
        Hash_Push_Front(&hash_free_front, &hash_node[i]);
    }

    for (i = 0; i < HASH_MAX; i++)
    {
        hash_front[i].prev = (void *)NOTHING;
        hash_front[i].next = (void *)NOTHING;
        hash_front[i].addr = NOTHING;
        hash_front[i].link = (void *)NOTHING;
    }


}



void Write_Page(uint addr, uint offset, uint size)
{
    uint i;
    NODE* ptr = (void *)NOTHING;
    HASH_NODE* hash_ptr;
    uint tmp_bitmap = 0;

    if ((hash_ptr = Hash_Find_Node(&hash_front[addr%HASH_MAX], addr)) == (void *)NOTHING) //new write
    {
        assert(!List_Is_Empty(&node[FREE_FRONT_NODE])); 
        if(used_count >= USED_MAX) //used full
        {

            //move "used" to "evict"
            ptr = List_Pop_Front(&node[USED_FRONT_NODE]);
            used_count--;
            Add_Evict_Page(ptr);
            

            hash_ptr = Hash_Find_And_Delete_Node(&hash_front[ptr->addr%HASH_MAX], ptr->addr);
            if (hash_ptr == (void*)NOTHING)
            {
                barePrintf("Cannot find hash pointer of entry for eviction\n");
                system("pause");
                exit(1);
            }
            
        }
        else{

            hash_ptr = Hash_Pop_Front(&hash_free_front);

        }
        ptr = List_Pop_Front(&node[FREE_FRONT_NODE]);

        ptr->addr = addr;
        ptr->bitmap = 0;

        hash_ptr->addr = ptr->addr;
        hash_ptr->link = ptr;
        Hash_Push_Front(&hash_front[ptr->addr%HASH_MAX], hash_ptr);
    }
    else //update
    {//note : no update for evict nodes
        ptr = hash_ptr->link; 
        ptr = List_Pop_Middle(ptr);
        used_count--;
    }

    for (i = offset; i < offset + size; i++)
        tmp_bitmap |= 1 << i;
    
    ptr->bitmap |= tmp_bitmap;

    List_Push_Back(&node[USED_BACK_NODE], ptr);
    used_count++;
    Create_Command(WRITE_DRAM, ptr->addr, 0, ptr->id, tmp_bitmap);
    //barePrintf("%d\t%d\n", ptr->addr, ptr->id);
    //
    //ptr : node to write
    //hash_ptr : delete node
}



void Read_Page(uint addr, uint offset, uint size)
{
    uint i = 0, dram_bitmap = 0, slave_bitmap = 0;
    NODE* ptr;
    if ((ptr = List_Find_Node(&node[USED_FRONT_NODE], addr)) == (void *)NOTHING)
    {
        Create_Command(READ_SLAVE, addr, (1 << SECTOR_PER_LINE) - 1, NOTHING, NOTHING);
    }
    else
    {

        for (i = offset; i < offset + size; i++)
        {
            if (ptr->bitmap & (1 << i))
                dram_bitmap |= (1 << i);
            else
                slave_bitmap |= (1 << i);
        }

        if (!dram_bitmap)
            Create_Command(READ_SLAVE, ptr->addr, slave_bitmap, ptr->id, dram_bitmap);
        else if (!slave_bitmap)
            Create_Command(READ_DRAM, ptr->addr, slave_bitmap, ptr->id, dram_bitmap);
        else
            Create_Command(READ_BOTH, ptr->addr, slave_bitmap, ptr->id, dram_bitmap);
    }
}

void Cache_Buffer(uint addr, uint len, uint op)
{
    uint i, j;
    uint offset = addr % SECTOR_PER_PAGE;
    uint pageAddr = addr / SECTOR_PER_PAGE;
    NODE* ptr;
    COMMAND *tmp = &cmd_list[0];
    unsigned int *tmp_int = (unsigned int *)tmp;
    if (op == 0){
        Write_Page(pageAddr, offset, len);
    }
    else{
        Read_Page(pageAddr, offset, len);
    }


    //*((vuint*)(_ADDR_SUB_REQ_BASE_ + _OFFSET_CMD_SIZE_)) = cmd_count;

    //for (i = 0; i<cmd_count; i++)
    //{
    for (j = 0; j<5; j++) //Note : only write is available for now...
    {
        *((vuint *)((_ADDR_SUB_REQ_BASE_ + _OFFSET_SUB_REQ_CACHE_DATA_) + _INTERVAL_CACHE_DATA_*j)) = tmp_int[j];
        //barePrintf("[CPU1] %d : %d\n", j, tmp_int[j]);
               
    }
    
    cmd_count = 0;
    // Complete
    dram_count++;
    return;
}



uint Print_Node_Count(NODE* front){
    NODE* tmp = front->next;
    uint i;
    for(i=0; i<ENTRY_SIZE; i++){
        if(tmp->next == (void*) NOTHING) break;
        tmp = tmp->next;
    }

    return i;

}

void Print_Node_Counts(){
     //       barePrintf("[CPU1] List count? %d, %d, %d\n", Print_Node_Count(&node[FREE_FRONT_NODE]), 
     //               Print_Node_Count(&node[USED_FRONT_NODE]),
     //               Print_Node_Count(&node[EVICT_FRONT_NODE]));
}
uint Cache_Buffer_Full(){
    
    return List_Is_Empty(&node[FREE_FRONT_NODE]);

}

uchar flush_condition(){

     // return 0;

     if(List_Is_Empty(&node[EVICT_FRONT_NODE])) return 0; //flush when evict queue isn't empty
     else if(evict_table_ptr == 0) return 0;
     else return 1;

}

void Add_Evict_Page(NODE* ptr) {

    ptr->state = EVICT_WAITING;
    List_Push_Back(&node[EVICT_BACK_NODE], ptr);            
    
    evict_table[evict_table_ptr++] = ptr;    
    assert(evict_count < EVICT_MAX); 
    evict_count++;


}


uint issue_flush(){

        NODE* ptr;
        evict_table_ptr--;
        ptr = evict_table[evict_table_ptr];

        //barePrintf("[CPU1] evict request sent, ptr : %d, id : %d, addr : %d\n", evict_table_ptr, ptr->id, ptr->addr);

        *((vuint *)(_ADDR_NAND_MAN_BASE_ + _ADDR_CPU1_BASE_ + _OFFSET_IRQ_ID_)) = ptr->id; //node ID
        *((vuint *)(_ADDR_NAND_MAN_BASE_ + _ADDR_CPU1_BASE_ + _OFFSET_IRQ_ADDR_)) = ptr->addr; //LPA
        ptr->state = EVICT_SENT;
        
        //things to send :
        //node id (dram addr)
        //lsa (logical addr)

        //receiving process:
        //1. receives node ID that's completed (DRAM addr)
        //2. Derive node ptr address from node ID (i think nodes + id*sizeof(NODE))
        //3. Pop ptr from EVICT list
        //4. Insert ptr to FREE list
        //5. change state to FREE (not really needed)
        nand_count++;
        return 0;
}

void direct_NAND_write(uint addr, uint len, uint op){

    static uint id = 0;

    assert(op == 0); //read not supported yet
    id = (id + 1) % 256 + ENTRY_SIZE;  //8 comes from the NAND data buffer entry size
    //barePrintf("[CPU1] write directly to NAND, id : %d, addr : %d\n", id, addr);

    *((vuint *)(_ADDR_NAND_MAN_BASE_ + _ADDR_CPU1_BASE_ + _OFFSET_HOST_ID_)) = id; //node ID
    *((vuint *)(_ADDR_NAND_MAN_BASE_ + _ADDR_CPU1_BASE_ + _OFFSET_HOST_ADDR_)) = addr/32; //LPA

    nand_count++;

}

uint bypass_condition(){
    //read counts from buffer controller and nand manager 
    int gamma;
    uint _buf_count;
    uint _nand_count;
    double gamma_thres = 754.65;

    static int tmp_count = 0;

    _buf_count = *((vuint*)(_ADDR_NAND_MAN_BASE_ + _ADDR_CPU1_BASE_ + _OFFSET_RAM_COUNT_)); //read NAND coz for some stupid fucking reason, CPU1 wont read RAM
    _nand_count = *((vuint*)(_ADDR_NAND_MAN_BASE_ + _ADDR_CPU1_BASE_ + _OFFSET_NAND_COUNT_ ));

    gamma = (double)_buf_count / _nand_count;
    //return (gamma > gamma_thres);
    return 0;
}


uint EvictDoneEmpty(){

    return M_CHECKEMPTY(EvictDone_Queue);
}

void Insert_EvictDone(uint id){

    M_PUSH(EvictDone_Queue, &id);
}


void Free_Evict_Nodes(){

    uint id = M_GETELE(EvictDone_Queue);
    NODE* ptr = &node[id]; //translate id into node ptr
    
    ptr = List_Pop_Middle(ptr);
    evict_count--;
    List_Push_Back(&node[FREE_BACK_NODE], ptr);
    
    M_POP(EvictDone_Queue);


}



