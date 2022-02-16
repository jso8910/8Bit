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
  delay(10);
}


/*
   Read the contents of the EEPROM and print them to the serial monitor.
*/
void printContents() {
  for (int base = 0; base <= 255; base += 16) {
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

/*
What will the EEPROM look like?

A0-A2: flags
A3-A5: mask
A6: require zero? (eg expected flag of 101 would mean that MSB and LSB have to be zero)
I/O 1: met condition?

Address for example and output:
0000000 1
0100000 0
1101010 1

for (int addr = 0; addr < 128; ++addr) {
  uint8_t flags = addr & 0b111
  uint8_t mask = addr & 0b111000 >> 3
  bool enforce_zero = addr >> 6

  if (enforce_zero == true) {
    writeEEPROM(addr, ((mask ^ flags) & mask) == mask ? 1 : 0);
  } else {
    writeEEPROM(addr, mask & flags == mask ? 1 : 0);
  }
}

l = []
for addr in range(128):
  flags = addr & 0b111
  mask = (addr & 0b111000) >> 3
  enforce_zero = bool(addr >> 6)
  if enforce_zero:
    if (mask ^ flags) & mask == mask:
      print(bin(mask), bin(flags), enforce_zero)
      l.append((1, bin(mask), bin(flags), enforce_zero))
    else:
      l.append((0, bin(mask), bin(flags), enforce_zero))
  else:
    if mask & flags == mask:
      print(bin(mask), bin(flags), enforce_zero)
      l.append((1, bin(mask), bin(flags), enforce_zero))
    else:
      l.append((0, bin(mask), bin(flags), enforce_zero))
*/


void setup() {
  // put your setup code here, to run once:
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  Serial.begin(57600);

//  Serial.print("Wiping EEPROM");
//  for (int addr = 0; addr <= 2047; ++addr) {
//    writeEEPROM(addr, 0xff);
//
//    if (addr % 64 == 0) {
//      Serial.print(".");
//    }
//  }
//  Serial.println(" done");

  Serial.print("Programming EEPROM");
\  for (int addr = 0; addr < 128; ++addr) {
    uint8_t flags = addr & 0b111;
    uint8_t mask = (addr & 0b111000) >> 3;
    bool enforce_zero = addr >> 6;

    if (enforce_zero == true) {
]      writeEEPROM(addr, ((mask ^ flags) & mask) == mask ? 1 : 0);
    } else {
]      writeEEPROM(addr, (mask & flags) == mask ? 1 : 0);
    }
  }
  Serial.println(" done");

  // Read and print out the contents of the EERPROM
  Serial.println("Reading EEPROM");
  printContents();
}


void loop() {
  // put your main code here, to run repeatedly:

}
