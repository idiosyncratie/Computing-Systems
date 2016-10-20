#include "my_setjmp.h"
#include <math.h>

#include <stdlib.h>
#include <stdio.h>

static my_jmp_buf env, fnv;

// testing jumping across multiple function invocations
int h(int n) {
    my_longjmp(env, n);
    return 0;
}
int h1(int n) {
    my_longjmp(fnv, n);
    return 0;
}
// test longjmp(buf, 0)
int test(int n) {
    int i = my_setjmp(env);
    printf("i: %d\n", i);
    if (i != 0) {
        return i;
    }
    h(n);
    return 0;
}

/* Testing multiple buffers on multiple function calls */ 
int test1(int n) {
    // testing for corrupted stack
    int a = 0;
    int i = my_setjmp(fnv);
    int b = 100;
    /* Local variables with known values
    * are on both sides of jmp_buf variable to ensure that
    * the setjmp() implementation doesn't go beyond the 
    * extent of the jmp_buf */
    if (i != 0) {
        return a + b;
    }
    h1(n);
    return 1;
}
int main() {

      printf("longjmp(buf, 0) returns 1:\t");
    if (test(0)  == 1) {
        printf("PASS\n");
    } else printf("FAIL\n");

    printf("longjmp(buf, 1) returns 1:\t");
    if (test(1)  == 1)
    {
        printf("PASS\n");
    } else printf("FAIL\n");
    
    printf("longjmp(buf, n) returns n for n != 0:\t");
    if (test(4)  == 4) {
        printf("PASS\n");

    } else printf("FAIL\n");

    printf("Testing setjmp doesnt go beyond jmp_buf\n");
    if (test1(2) == 100) {
        printf("PASS\n");
    } else printf("FAIL\n");
    return 0;
}