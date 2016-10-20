#ifndef MY_SETJMP
#define MY_SETJMP


/*
 * Change to #define to enable the use of your own setjmp()/longjmp()
 * implementation instead of the standard one, or undefine it if you
 * want to stick with the standard one!
 */
#define ENABLE_MY_SETJMP


#ifdef ENABLE_MY_SETJMP

/*
 * We are daring and we're going to try out our own implementation of
 * setjmp() and longjmp()!  Watch out stack, here we come!
 */

/* This is the proper size for your jump-buffer. */
#define MY_JB_LEN 6
typedef int my_jmp_buf[MY_JB_LEN];

int my_setjmp(my_jmp_buf buf);
void my_longjmp(my_jmp_buf buf, int ret);

/*
 * Just in case these symbols were defined elsewhere, blow them away
 * and redefine them for ourselves.
 */

#undef jmp_buf
#undef setjmp
#undef longjmp

#define jmp_buf my_jmp_buf
#define longjmp my_longjmp
#define setjmp my_setjmp

/*
 * We are going to stick wit
#define longjmp my_longjmp

#elseh the standard implementation of setjmp()
 * and longjmp() for now...
 */
#else 
#include <setjmp.h>

#endif /* ENABLE_MY_SETJMP */

#endif /* MY_SETJMP */

