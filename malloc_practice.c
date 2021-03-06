#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "malloc_practice.h"
//DODAJ IFNDEF U .H


static int setup = 0;
static struct free_memory_node *start, *end;

void initialise_list(){
    setup = 1;
    start = sbrk(sizeof(struct free_memory_node));
    if(start == (void*)-1){
        fprintf(stderr, "ERROR ALLOCATING START %d", errno);
        exit(-1);
    }
    start->next = NULL;
    start->prevoius = NULL;
    start->size = sbrk(0) - ((void *) start );
    end=start;
}

void *malloc_p(size_t sz){
    
    if(sz<sizeof(struct free_memory_node)-sizeof(size_t))sz=16;

    if(!setup)
        initialise_list();

    struct free_memory_node *it;
    for(it = start; it!= NULL; it=it->next){

        if(it == end && it->size - sizeof(struct free_memory_node) < sz+sizeof(size_t)){
            size_t diff = (sz+sizeof(size_t)) - (it->size - sizeof(struct free_memory_node));
            void *result = sbrk(diff);
            if(result == (void *) -1){
                fprintf(stderr, "ERROR ALLOCATING RESIZE %d", errno);
                exit(-1);
            }
            it->size=sbrk(0)-((void*)it);
        }


        if(it->size - sizeof(struct free_memory_node) >= sz+sizeof(size_t)){
            struct free_memory_node *new_free_block_start;
            new_free_block_start = ((void *) it)+sz+sizeof(size_t);

            struct free_memory_node old_node_copy = *it;

            if(it == start)
                start = new_free_block_start;
            else
                (old_node_copy.prevoius)->next = new_free_block_start;
            
            if(it == end)
                end = new_free_block_start;
            else
                (old_node_copy.next)->prevoius = new_free_block_start;

            new_free_block_start->next = old_node_copy.next;
            new_free_block_start->prevoius = old_node_copy.prevoius;
            
            size_t *block_size = (size_t *) it;
            *block_size = sz;
            return ((void *)it) + sizeof(size_t);
        }else if(it->size == sz+sizeof(size_t) && ( it!= start || it!= end) ){
            struct free_memory_node old_node_copy = *it;

            if(it == start){
                start = old_node_copy.next;
                start->prevoius=NULL;
            }else
                old_node_copy.prevoius->next = old_node_copy.next;
            
            
            if(it==end){
                end = old_node_copy.prevoius;
                end->next=NULL;
            }else
                old_node_copy.next->prevoius = old_node_copy.prevoius;
            
            size_t *block_size = (size_t *) it;
            *block_size = sz;
            return ((void *)it) + sizeof(size_t);
        }
    }
    fprintf(stderr,"should not reach");
    exit(-1);

}

//Losing memory if its less tnan 16 bytes. Add malloc minimum. Add freeing a block between two allocated blcoks correctly.
void free_p(void *ptr){
    void *real_block_start = ptr-sizeof(size_t);
    size_t sz = *((size_t *)ptr)+sizeof(size_t);
    if(ptr < (void *)start){
        struct free_memory_node *new_start = (struct free_memory_node *) real_block_start;
        if(real_block_start+sz == (void *)start)
            *new_start = *start;
        else{
            new_start->next=start;
            new_start->prevoius=NULL;
            new_start->size=sz;
        }
        start=new_start;
        return;
    }
    struct free_memory_node *it=start;
    while(it->next!=NULL && (void *)it->next<ptr){
        if(((void *) it) + it->size == real_block_start){
            it->size+=sz;
            return;
        }
        it=it->next;
    }
    if(it->next!=NULL && real_block_start+sz == (void *)it){
        struct free_memory_node *updated_address = (struct free_memory_node *) real_block_start;
        struct free_memory_node old_node_copy = *it;
        if(old_node_copy.prevoius != NULL)
            old_node_copy.prevoius->next = updated_address;
        if(old_node_copy.next != NULL)
            old_node_copy.next->prevoius = updated_address;
        
        updated_address->next = old_node_copy.next;
        updated_address->prevoius = old_node_copy.prevoius;
    }else{
        struct free_memory_node *new_free_start = (struct free_memory_node *) real_block_start;
        struct free_memory_node *next = it->next;

        new_free_start->prevoius=it;
        new_free_start->next=next;
        it->next=new_free_start;
        if(next!=NULL)
            next->prevoius=new_free_start;

    }
}