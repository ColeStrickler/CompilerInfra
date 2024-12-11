.data
.text
.globl main
fun_fn: 
pushq %rbp
movq %rsp, %rbp
addq $16, %rbp
subq $16, %rsp
#Fn body fn
#            getarg 1 [a]
movq %rdi, -24(%rbp)
#            setret [a]
movq -24(%rbp), %rbx
movq %rbx, %rax
#            goto lbl_1
jmp lbl_1
#Fn epilogue fn
lbl_1: 
addq $16, %rsp
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
movq %rax, -24(%rbp)
#            READ [a]
call getInt
movq %rax, -24(%rbp)
#            setarg 1 [a]
movq -24(%rbp), %rdi
#            call fn
call fun_fn
#            getret [tmp0]
movq %rax, -32(%rbp)
#            WRITE [tmp0]
movq -32(%rbp), %rdi
call printInt
#            setret [a]
movq -24(%rbp), %rbx
movq %rbx, %rax
#            goto lbl_2
jmp lbl_2
#Fn epilogue main
lbl_2: 
addq $16, %rsp
popq %rbp
ret
