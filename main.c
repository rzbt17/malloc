#include <stdio.h>
#include <stdlib.h>
#include "malloc_practice.h"

int main(){
    printf("main test");
    
    int *lol = malloc_p(sizeof(int));
    int *lol2 = malloc_p(sizeof(int));
    *lol=9;
    *lol2=10;
    char *zs = malloc_p(20*sizeof(char));

    free_p(zs);
    int *lol3;
    free_p(lol2);
    lol3=malloc_p(sizeof(int));
    scanf("%d", lol3);
    printf("%d\n", *lol3);
}