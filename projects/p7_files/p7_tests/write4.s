.data
.text
.globl main
main: 
pushq %rbp
movq %rsp, %rbp
addq $16, %rbp
subq $16, %rsp
#Fn body main
#            [a] := 4
movq $4, %rax
movq %rax, -16(%rbp)
#            WRITE [a]
movq -16(%rbp), %rdi
call printInt
#Fn epilogue main
lbl_1: 
addq $16, %rsp
popq %rbp
ret
