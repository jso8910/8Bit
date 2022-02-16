#include "rules.asm"


addloop:
    out
    add 1
    jc subloop
    jmp addloop
subloop:
    out
    sub 1
    jz addloop
    jmp subloop