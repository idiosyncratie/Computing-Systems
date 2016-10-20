    .file   "example.c"
    .section    .text.unlikely,"ax",@progbits
.LCOLDB0:
    .text
.LHOTB0:
    .p2align 4,,15
    .globl  ex
    .type   ex, @function
ex:
.LFB0:
    .cfi_startproc
    movl    8(%esp), %eax       # moves b[8(%esp)] into eax
    subl    12(%esp), %eax      # subtract c[12(%esp)] from b, stores into eax
    imull   4(%esp), %eax       # multiply (b-c) with a[4(%esp)]
    addl    16(%esp), %eax      # add d[16(%esp)] with a*(b-c)
    ret
    .cfi_endproc
.LFE0:
    .size   ex, .-ex
    .section    .text.unlikely
.LCOLDE0:
    .text
.LHOTE0:
    .ident  "GCC: (GNU) 5.3.0"
    .section    .note.GNU-stack,"",@progbits
