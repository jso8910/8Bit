#ruledef
{
    nop => 0b0000 @ 0b0000
    lda {addr} => 0b0001 @ addr`4
    add {addr} => 0b0010 @ addr`4
    sub {addr} => 0b0011 @ addr`4
    sta {addr} => 0b0100 @ addr`4
    ldi {val} => 0b0101 @ val`4
    jmp {addr} => 0b0110 @ addr`4
    jc {addr} => 0b0111 @ addr`4
    jz {addr} => 0b1000 @ addr`4

    out => 0b1110 @ 0b0000
    hlt => 0b1111 @ 0b0000
}
