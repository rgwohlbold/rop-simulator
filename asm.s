.globl gadget0
.globl gadget1
.globl gadget2
.globl gadget3
.globl gadget4

.section .text

gadget0:
    pop %rdi
    ret

gadget1:
    call *%rax
    ret

gadget2:
    pop %rax
    ret

gadget3:
    pop %rbx
    pop %rbp
    pop %r12
    pop %r13
    pop %r14
    pop %r15
    ret

gadget4:
    cmp %rbp, %rbx
    jne crash_program
    mov %r15, %rdx
    mov %r14, %rsi
    mov %r13d, %edi
    ret

crash_program:
    mov (0), %rax
    