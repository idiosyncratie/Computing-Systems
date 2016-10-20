.global my_setjmp
.global my_longjmp

my_setjmp:

    mov $0, %eax        # put 0 for return
    mov 4(%esp), %edx   # my_jmp_buf

    # save callee-save registers: 
    mov %esp, (%edx)    # esp
    mov %ebx, 4(%edx)   # ebx
    mov %esi, 8(%edx)   # esi
    mov %edi, 12(%edx)  # edi
    mov %ebp, 16(%edx)  # save caller's ebp
    mov (%esp), %ecx    # save return address
    movl %ecx, 20(%edx) # move return address into jmp_buf

    ret


my_longjmp:
    # callee-save registers: move buffers
    mov     4(%esp), %ecx   # get my_jmp_buf
    mov     8(%esp), %eax   # else return value placed into eax

    # get callee-save
    mov     (%ecx), %esp    # esp
    mov     4(%ecx), %ebx   # ebx
    mov     8(%ecx), %esi   # esi
    mov     12(%ecx), %edi  # edi
    mov     16(%ecx), %ebp  # caller's base pointer
    mov     20(%ecx), %edx  # return address
    mov     %edx, (%esp)    

    mov     $1, %edx        # set register as value 1
    cmp     $0, %eax        # if ret val is 0, return 1 
    cmovle   %edx, %eax      

    ret
