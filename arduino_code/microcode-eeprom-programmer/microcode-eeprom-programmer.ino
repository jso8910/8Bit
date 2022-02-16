/**
   This sketch is specifically for programming the EEPROM used in the 8-bit
   decimal display decoder described in https://youtu.be/dLh1n2dErzE
*/
#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

/*
   Output the address bits and outputEnable signal using shift registers.
*/
void setAddress(int address, bool outputEnable) {
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}


/*
   Read a byte from the EEPROM at the specified address.
*/
byte readEEPROM(int address) {
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, INPUT);
  }
  setAddress(address, /*outputEnable*/ true);

  byte data = 0;
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1) {
    data = (data << 1) + digitalRead(pin);
  }
  return data;
}


/*
   Write a byte to the EEPROM at the specified address.
*/
void writeEEPROM(int address, byte data) {
  setAddress(address, /*outputEnable*/ false);
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, OUTPUT);
  }

  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }
  digitalWrite(WRITE_EN, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_EN, HIGH);
  delay(5);
}


/*
   Read the contents of the EEPROM and print them to the serial monitor.
*/
void printContents(int start, int length) {
  for (int base = start; base <= length; base += 16) {
    byte data[16];
    for (int offset = 0; offset <= 15; offset += 1) {
      data[offset] = readEEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }
}

// This is all signals set to off. Some are active high, which is 0 off, some are active low. Last bit is for idk
#define OFF 0b0101111110100111
// Bitwise XOR (^) will be used to toggle the bits on and off.
#define HLT 0b1000000000000000    // Halt the clock
#define MI  0b0100000000000000    // Address register in
#define RI  0b0010000000000000    // RAM in (aka write)
#define RO  0b0001000000000000    // RAM out
#define IO  0b0000100000000000    // Instruction register out
#define II  0b0000010000000000    // Instruction register in
#define AI  0b0000001000000000    // A register in
#define AO  0b0000000100000000    // A register out
#define EO  0b0000000010000000    // ALU/sum out
#define SU  0b0000000001000000    // Subtract
#define BI  0b0000000000100000    // B register in
#define OI  0b0000000000010000    // Output register in
#define CE  0b0000000000001000    // Program counter enable
#define CO  0b0000000000000100    // Program counter out
#define J   0b0000000000000010    // Jump (program counter in)
#define FI  0b0000000000000001    // Flag register in

#define FLAGS_Z0C0 0
#define FLAGS_Z0C1 1
#define FLAGS_Z1C0 2
#define FLAGS_Z1C1 3

#define JC 0b0111
#define JZ 0b1000

const PROGMEM uint16_t UCODE_TEMPLATE[16][8] = {
      { OFF^MI^CO,  OFF^RO^II^CE,  OFF,        OFF,        OFF,              OFF,  OFF,  OFF, },   // 0000 - NOP, literally does nothing lmfao
      { OFF^MI^CO,  OFF^RO^II^CE,  OFF^IO^MI,  OFF^RO^AI,  OFF,              OFF,  OFF,  OFF, },   // 0001 - LDA, takes the value at memory address X and puts it in the A register
      { OFF^MI^CO,  OFF^RO^II^CE,  OFF^IO^MI,  OFF^RO^BI,  OFF^EO^AI^FI,     OFF,  OFF,  OFF, },   // 0010 - ADD
      { OFF^MI^CO,  OFF^RO^II^CE,  OFF^IO^MI,  OFF^RO^BI,  OFF^SU^EO^AI^FI,  OFF,  OFF,  OFF, },   // 0011 - SUB
      { OFF^MI^CO,  OFF^RO^II^CE,  OFF^IO^MI,  OFF^AO^RI,  OFF,              OFF,  OFF,  OFF, },   // 0100 - STA, store contents of A register into memory address X
      { OFF^MI^CO,  OFF^RO^II^CE,  OFF^IO^AI,  OFF,        OFF,              OFF,  OFF,  OFF, },   // 0101 - LDI
      { OFF^MI^CO,  OFF^RO^II^CE,  OFF^IO^J,   OFF,        OFF,              OFF,  OFF,  OFF, },   // 0110 - JMP
      { OFF^MI^CO,  OFF^RO^II^CE,  OFF,        OFF,        OFF,              OFF,  OFF,  OFF, },   // 0111 - JC
      { OFF^MI^CO,  OFF^RO^II^CE,  OFF,        OFF,        OFF,              OFF,  OFF,  OFF, },   // 1000 - JZ
      { OFF^MI^CO,  OFF^RO^II^CE,  OFF,        OFF,        OFF,              OFF,  OFF,  OFF, },   // 1001 - NOP
      { OFF^MI^CO,  OFF^RO^II^CE,  OFF,        OFF,        OFF,              OFF,  OFF,  OFF, },   // 1010 - NOP
      { OFF^MI^CO,  OFF^RO^II^CE,  OFF,        OFF,        OFF,              OFF,  OFF,  OFF, },   // 1011 - NOP
      { OFF^MI^CO,  OFF^RO^II^CE,  OFF,        OFF,        OFF,              OFF,  OFF,  OFF, },   // 1100 - NOP
      { OFF^MI^CO,  OFF^RO^II^CE,  OFF,        OFF,        OFF,              OFF,  OFF,  OFF, },   // 1101 - NOP
      { OFF^MI^CO,  OFF^RO^II^CE,  OFF^AO^OI,  OFF,        OFF,              OFF,  OFF,  OFF, },   // 1110 - OUT, take the A register and display it on a 7 segment display
      { OFF^MI^CO,  OFF^RO^II^CE,  OFF^HLT,    OFF,        OFF,              OFF,  OFF,  OFF, },   // 1111 - HLT
      
};

uint16_t ucode[4][16][8];

void initUCode() {
  // ZF = 0, CF = 0
  memcpy_P(ucode[FLAGS_Z0C0], UCODE_TEMPLATE, sizeof(UCODE_TEMPLATE));

  // ZF = 0, CF = 1
  memcpy_P(ucode[FLAGS_Z0C1], UCODE_TEMPLATE, sizeof(UCODE_TEMPLATE));
  ucode[FLAGS_Z0C1][JC][2] = OFF^IO^J;

  // ZF = 1, CF = 0
  memcpy_P(ucode[FLAGS_Z1C0], UCODE_TEMPLATE, sizeof(UCODE_TEMPLATE));
  ucode[FLAGS_Z1C0][JZ][2] = OFF^IO^J;

  // ZF = 1, CF = 1
  memcpy_P(ucode[FLAGS_Z1C1], UCODE_TEMPLATE, sizeof(UCODE_TEMPLATE));
  ucode[FLAGS_Z1C1][JC][2] = OFF^IO^J;
  ucode[FLAGS_Z1C1][JZ][2] = OFF^IO^J;
}

void setup() {
  initUCode();

  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  Serial.begin(57600);

  
//  Serial.print("Wiping EEPROM");
//  for (int i = 0; i < 2048; ++i) {
//    writeEEPROM(i, 0xff);
//    if (i % 64 == 0) {
//      Serial.print(".");
//    }
//  }
//  Serial.println(" done");

  Serial.print("Programming EEPROM");
//  for (int address = 0; address < 1024; ++address) {
//    
//    int flags       = (address & 0b1100000000) >> 8;  // First two bits of address
//    int byte_sel    = (address & 0b0010000000) >> 7;  // Byte select bit
//    int instruction = (address & 0b0001111000) >> 3;  // Middle 4 bits for opcode
//    int step        = (address & 0b0000000111) >> 0;  // Last 3 bits for step
//
//    if (byte_sel) {
//      writeEEPROM(address, ucode[flags][instruction][step]);
//    } else {
//      writeEEPROM(address, ucode[flags][instruction][step] >> 8);
//    }
//    if (address % 64 == 0) {
//      Serial.print(".");
//    }
//  }
  for (int address = 0; address < 1024; ++address) {
    
    int flags       = (address & 0b1100000000) >> 8;  // First two bits of address
    int byte_sel    = (address & 0b0010000000) >> 7;  // Byte select bit
    int instruction = (address & 0b0001111000) >> 3;  // Middle 4 bits for opcode
    int step        = (address & 0b0000000111) >> 0;  // Last 3 bits for step

    if (byte_sel) {
      writeEEPROM(address, ucode[flags][instruction][step]);
    } else {
      writeEEPROM(address, ucode[flags][instruction][step] >> 8);
    }
    if (address % 64 == 0) {
      Serial.print(".");
    }
  }
  Serial.println(" done");

  

  // Read and print out the contents of the EERPROM
  Serial.println("Reading EEPROM");
  printContents(0, 1024);
}


void loop() {
  // put your main code here, to run repeatedly:

}
