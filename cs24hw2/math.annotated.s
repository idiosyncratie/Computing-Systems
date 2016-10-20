/* This file contains IA32 assembly-language implementations of three
 * basic, very common math operations.
 */

    .text

/*====================================================================
 * int f1(int x, int y)
 *  
 * the function compares x and y and returns the minimum
 * The code is annotated below
 */
.globl f1
f1:
    pushl   %ebp            # push the base pointer
    movl    %esp, %ebp      # moves the stack pointer into base pointer
    movl    8(%ebp), %edx   # moves x[8(%ebp)] into edx
    movl    12(%ebp), %eax  # moves y[12(%ebp)] into eax
    cmpl    %edx, %eax      # subtracts y from x and sets the status 
                            # flags ZF, SF depending on result
    cmovg   %edx, %eax      # moves y into x if one or more of the status 
                            # flags are set. Else, the move is not performed
    popl    %ebp            
    ret


/*====================================================================
 * int f2(int x) 
 *  The function gets the absolute value of x
 *  The mask shifts all 1s for a negative number and all 0s for a positive
 *  number, so mask XOR number will act like NOT for negative numbers and
 *  do no operation for positive numbers. When the mask is subtracted, 
 *  negatives will have 1 added, and +0 for positive numbers
 *      ex: 10101010 = -86
 *          10101010 XOR 11111111 = 01010101 = 85
 *          01010101 - 11111111 = 85 - (-1) = 86
 */
.globl f2
f2:
    pushl   %ebp            
    movl    %esp, %ebp      
    movl    8(%ebp), %eax   # moves x[8(%ebp)] into eax
    movl    %eax, %edx      # copy x into edx
    sarl    $31, %edx       # set a mask by right shifting 31 bits
    xorl    %edx, %eax      # mask XOR result to make result positive
    subl    %edx, %eax      # subtract the mask from result to correct it
    popl    %ebp
    ret


/*====================================================================
 * int f3(int x)
 * The function returns the sign of x: 
 *      If x > 0, return 1. 
 *      If x == 0, return 0.
 *      else, return -1.
 */
.globl f3
f3:
    pushl   %ebp            
    movl    %esp, %ebp      
    movl    8(%ebp), %edx   # moves x[8(%ebp)] into edx
    movl    %edx, %eax      # moves x[%edx] into eax
    sarl    $31, %eax       # gets the sign bit (1: neg, 0: pos or zero)
    testl   %edx, %edx      # computes x & x and sets the SF, ZF, and PF 
                            # status flags if it gets 0 (edx is positive)
    movl    $1, %edx        # moves 1 into [%edx]
    cmovg   %edx, %eax      # moves 1 into x if one or more of the status 
                            # flags are set. Else, the move is not performed
                            # eax only changes if x was positive
    popl    %ebp
    ret

