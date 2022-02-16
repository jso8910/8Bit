#define BUS_0 0
#define BUS_7 7
#define CLK 8
#define MI 9    // Active low
#define RI 10   // Active low
#define HLT 11  // This should always be high

void set_address(int address) {
  for (int pin = BUS_0; pin <= BUS_7; ++pin) {
    digitalWrite(pin, address & 1);
    address = address >> 1;
  }


  digitalWrite(MI, LOW);

  delayMicroseconds(5);

  digitalWrite(CLK, HIGH);

  delayMicroseconds(5);

  digitalWrite(CLK, LOW);
  digitalWrite(MI, HIGH);
}

void write_ram(byte data, int address) {
  set_addess(address);
  for (int pin = BUS_0; pin <= BUS_7; ++pin) {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }

  digitalWrite(RI, LOW);

  delayMicroseconds(5);

  digitalWrite(CLK, HIGH);

  delayMicroseconds(5);

  digitalWrite(CLK, LOW);
  digitalWrite(RI, HIGH);
}

void setup() {
  for (int pin = BUS_0; pin <= BUS_7; pin += 1) {
    pinMode(pin, OUTPUT);
  }
  pinMode(CLK, OUTPUT);
  pinMode(MI, OUTPUT);
  pinMode(RI, OUTPUT);
  pinMode(HLT, OUTPUT);

  // Setting default state
  digitalWrite(MI, HIGH);
  digitalWrite(RI, HIGH);
  digitalWrite(HLT, HIGH);
  digitalWrite(CLK, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:

}
