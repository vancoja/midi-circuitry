#include <SPI.h>
#include <MIDI.h>

// set pin 10 as the slave select for the digital pot
#define SLAVE_SELECT_PIN 10
// SDI = D11 (MOSI pin)
// CLK = D13 (SCK pin)

// LED feedback pin
#define LED_PIN 3

// the MIDI channel to listen for (alternatively set this to MIDI_CHANNEL_OMNI)
#define EXP_CHANNEL 1

// the address of the expression pot
#define EXP_ADDRESS 0

// 4 or 6 per IC?
#define MAX_POT_NR 6

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  // set the slaveSelectPin as an output
  pinMode(SLAVE_SELECT_PIN, OUTPUT);
  // initialise SPI
  SPI.begin();
  // initialise MIDI, only listen on defined channel
  MIDI.begin(EXP_CHANNEL);
  // set handler for CC messages
  MIDI.setHandleControlChange(controlChangeHandler);
}

void loop() {
  MIDI.read();
}

void controlChangeHandler(byte channel, byte number, byte value) {
  analogWrite(LED_PIN, value * 2);
  if (number < MAX_POT_NR) {
    digitalPotWrite(number, value * 2);
  }
}

void digitalPotWrite(int address, int value) {
  // take the SS pin low to select the chip
  digitalWrite(SLAVE_SELECT_PIN, LOW);
  // send in the address and value via SPI
  SPI.transfer(address);
  SPI.transfer(value);
  // take the SS pin high to de-select the chip
  digitalWrite(SLAVE_SELECT_PIN, HIGH);
}
