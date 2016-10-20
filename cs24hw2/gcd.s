.globl gcd             /* Ignore everything beginning with a "." */
.text
    .align 4
gcd:                    /* Euclid GCD is in the format gcd(x, y) */
    pushl %ebp         /* Push old base pointer. */
    movl %esp,%ebp     /* Current stack is new base. */
    movl 8(%ebp), %eax /* Move x into %eax */
    cmpl $0,12(%ebp)    /* if y == 0 */
    je gcd_return       /* If it is, jump to return. */
                        /* else falls through to gcd_continue */
gcd_continue:
    movl 12(%ebp),%ecx  /* The remainder. edx = y*/
    movl $0, %edx       /* clearing edx because divide uses edx:eax */
    divl %ecx           /* compute x/y using unsigned divide*/
    pushl %edx          /* push remainder */
    pushl %ecx          /* push divisor */
    call gcd          /* Make recursive call. */

gcd_return:
    movl %ebp,%esp     /* Pop local stack. */
    popl %ebp          /* Pop old base of frame. */
    ret
