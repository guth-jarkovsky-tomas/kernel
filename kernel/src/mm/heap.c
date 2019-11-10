// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include <mm/heap.h>

// implementace bere rady z několik webů a odpovědí na stack overflow, ale implementace je naprosto moje (a také odpovídajícně chybná)

//struct na popisování bloku pameti v haldě 
//důležité je že tato struktura je v paměti NAD samotnou pamětí na kterou odkazuje, protože proč ne
//až na data_block úplně na spodku haldy, který nikam neodkazuje a nic nepopisuje

struct data_block {
    size_t size;
    bool free;
    struct data_block* next;
    struct data_block* prev; 
}

void heap_init()
{
    
}

void* kmalloc(size_t size) {   

    //basic safety
    if (size <= 0) {
        return NULL;
    }

    //get pointer to the top of the heap
    struct data_block* top = sbrk(0);
    

    //search the already existant linked list for free blocks;
    struct data_block* searcher = top;
    while (true) {

        // this would mean no free block was found
        if (searcher->prev == NULL) {
            break;
        }

        // if there is a free block, its yours
        if (searcher->free && searcher->size => size) {
            searcher->free = false;
            return searcher->prev+sizeof(struct data_block);
        }
        //and if not, keep searching deeper
        else {
            searcher = searcher->prev;
        }
    }


    //first allocate emomory, but top remains the same
    top = sbrk(size);

    //create a backup
    struct data_block* temp = top;

    //generate a return pointer right above top
    void* returnpointer = top+sizeof(struct data_block);

    //move top ABOVE the allocated memory 
    top = sbrk(sizeof(struct data_block));

    //link the list
    top->size = size;
    top->prev = temp;
    temp->next = top;

    return returnpointer; 
}

void kfree(void* ptr) {

    //find top of current allocated memory
    struct data_block* searcher = sbrk(0);
    void* temp;

    //basic safety
    if (ptr == NULL || searcher->prev == NULL) {
        return;
    }    
    
    //walk down the list searching for pointer    
    while (true) {
        temp = searcher;
        searcher = searcher->prev; 
        
        if (ptr == searcher+sizeof(struct data_block)) {
            temp->free = true;
            return;
        }
        if (searcher->prev == NULL) {
            return;
        }
    }
}
