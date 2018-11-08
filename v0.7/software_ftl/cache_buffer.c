#include "cache_buffer.h"

NODE	node[LIST_SIZE];
COMMAND cmd_list[100];
uint	cmd_count;
uint	count_entry;
HASH_NODE hash_front[HASH_MAX];
HASH_NODE hash_node[ENTRY_SIZE];
HASH_NODE hash_free_front;


void Create_Command(OP op, uint slave_addr, uint slave_bitmap, uint dram_id, uint dram_bitmap)
{
    cmd_list[cmd_count].op = op;
    cmd_list[cmd_count].slave_addr = slave_addr;
    cmd_list[cmd_count].slave_bitmap = slave_bitmap;
    cmd_list[cmd_count].dram_id = dram_id;
    cmd_list[cmd_count].dram_bitmap = dram_bitmap;

    cmd_count++;
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

    for (i = 0; i < LIST_SIZE; i++)
    {
        if (i != FREE_FRONT_NODE && i != FREE_BACK_NODE && i != USED_FRONT_NODE && i != USED_BACK_NODE)
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

    if ((hash_ptr = Hash_Find_Node(&hash_front[addr%HASH_MAX], addr)) == (void *)NOTHING)
    {
        if (List_Is_Empty(&node[FREE_FRONT_NODE])) 
        {
            ptr = List_Pop_Front(&node[USED_FRONT_NODE]);

            hash_ptr = Hash_Find_And_Delete_Node(&hash_front[ptr->addr%HASH_MAX], ptr->addr);
            if (hash_ptr == (void*)NOTHING)
            {
                printf("Cannot find hash pointer of entry for eviction\n");
                system("pause");
                exit(1);
            }


            if (ptr->bitmap == ((1 << SECTOR_PER_LINE) - 1))
            {

                Create_Command(WRITE_SLAVE, ptr->addr, (1 << SECTOR_PER_LINE) - 1 - ptr->bitmap, ptr->id, ptr->bitmap);
            }
            else
            {

                Create_Command(UPDATE_SLAVE, ptr->addr, (1 << SECTOR_PER_LINE) - 1 - ptr->bitmap, ptr->id, ptr->bitmap);
            }
        }
        else
        {
            ptr = List_Pop_Front(&node[FREE_FRONT_NODE]);

            hash_ptr = Hash_Pop_Front(&hash_free_front);
        }
        ptr->addr = addr;
        ptr->bitmap = 0;

        hash_ptr->addr = ptr->addr;
        hash_ptr->link = ptr;
        Hash_Push_Front(&hash_front[ptr->addr%HASH_MAX], hash_ptr);
    }
    else 
    {
        ptr = hash_ptr->link; 
        ptr = List_Pop_Middle(ptr);
    }

    for (i = offset; i < offset + size; i++)
        ptr->bitmap |= 1 << i;

    List_Push_Back(&node[USED_BACK_NODE], ptr);


    Create_Command(WRITE_DRAM, ptr->addr, 0, ptr->id, ptr->bitmap);
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
    if (op == 0){
        Write_Page(pageAddr, offset, len);
    }
    else{
        Read_Page(pageAddr, offset, len);
    }


    *((vuint*)(_ADDR_CACHE_BUFFER_BASE_ + _OFFSET_CACHE_SIZE_)) = cmd_count;

    for (i = 0; i<cmd_count; i++)
    {
        COMMAND *tmp = &cmd_list[i];
        unsigned int *tmp_int = (unsigned int *)tmp;

        for (j = 0; j<5; j++)
        {
            *((vuint *)((_ADDR_CACHE_BUFFER_BASE_ + _OFFSET_CACHE_DATA_) + _INTERVAL_CACHE_DATA_*j + _INTERVAL_CACHE_DATA_*i * 5)) = tmp_int[j];
        }
    }
    cmd_count = 0;
    // Complete

    return;
}
