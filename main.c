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

    printf("%d", *lol + *lol2);
    scanf("%s",zs);
    
    printf("%s",zs);
}