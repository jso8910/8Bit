# 8Bit
Documentation and code for my 8 bit computer


I have a lot of plans for upgrades! First, I am adding an EEPROM ram programmer that controls the bus and some control signals. Then, I will upgrade the ram to 16 bit addressing (64K). After that, I will upgrade to be able to support 32 instructions and 4 or 5 flags maximum and 16 clock cycles per instruction. The way these flags work means I only need one jump instruction for every possible combination of flags. The lower 3 bits of the instruction register will be connected to this EEPROM. 3 can be increased if I want 4 flags, but I can only think of 3 now (negative, zero, carry).

I will also add a 2 EEPROM component to the ALU to support 8 bitwise operations (not including shifts because shifts can be implemented in software pretty easily). The lower 3 bits of the instruction register will be connected to a 3 bit operation select of this logic thing.

I will add at least one more EEPROM to the control signals to expand control signals and use a demux to combine some control signals (I am thinking the active low outputs first since I don't have any active high demux chips lol).

I will also add a 256 byte (8 bit address) stack.

I have some drawings of the memory module I am designing (very abstract) including what instructions such as the fetch cycle and the JMP instruction will look like but I don't want to take a picture of them now.
