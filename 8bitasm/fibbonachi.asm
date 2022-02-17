#include "rules.asm"

start:
    ldi 1
    sta x
    ldi 0
loop:
    sta y
    out
    lda x
    add y
    sta x
    out 
    lda y
    add x
    jc start
    jmp loop
    nop
x:
    #d8 0x00
y:
    #d8 0x00