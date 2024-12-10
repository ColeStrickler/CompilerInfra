.data
.text
.globl main
main: 
pushq %rbp
movq %rsp, %rbp
addq $16, %rbp
subq $0, %rsp
#Fn body main
#Fn epilogue main
lbl_1: 
addq $0, %rsp
popq %rbp
ret
