#ifndef _MALLOC_PRACTICE
#define _MALLOC_PRACTICE

struct free_memory_node{
    struct free_memory_node *prevoius, *next;
    size_t size;
};
void *malloc_p(size_t sz);
void free_p(void *ptr);

#endif