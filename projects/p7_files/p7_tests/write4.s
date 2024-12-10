.data
.text
.globl main
fun_fn: 
pushq %rbp
movq %rsp, %rbp
addq $16, %rbp
subq $0, %rsp
#Fn body fn
#            setret 5
movq $5, %rbx
movq %rbx, %rax
#            goto lbl_1
jmp lbl_1
#Fn epilogue fn
lbl_1: 
addq $0, %rsp
popq %rbp
ret
main: 
pushq %rbp
movq %rsp, %rbp
addq $16, %rbp
subq $16, %rsp
#Fn body main
#            [a] := 4
movq $4, %rax
movq %rax, -16(%rbp)
#            call fn
call fun_fn
#            getret [tmp0]
movq %rax, -24(%rbp)
#            WRITE [tmp0]
movq -24(%rbp), %rdi
call printInt
#            setret [a]
movq -16(%rbp), %rbx
movq %rbx, %rax
#            goto lbl_2
jmp lbl_2
#Fn epilogue main
lbl_2: 
addq $16, %rsp
popq %rbp
ret
