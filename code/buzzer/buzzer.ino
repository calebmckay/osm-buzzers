#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

#include <avr/power.h>
#include <avr/sleep.h>

//---------------------------------
// Parameters
//---------------------------------


//---------------------------------
// Pin numbers
//---------------------------------
int PIN_BUTTON = 3;
int PIN_CE = 7;
int PIN_CSN = 8;
int PIN_SCK = 13;
int PIN_MOSI = 11;
int PIN_MISO = 12;
int PIN_IRQ = 2;

//These pins identify which buzzer is being used
// 00 - Reset
// 01 - Buzzer 1
// 10 - Buzzer 2
// 11 - Invalid
int PIN_ID_H = 5;
int PIN_ID_L = 6;

//---------------------------------
RF24 radio(PIN_CE, PIN_CSN);

// Define addresses for each machine
byte buttonAddress[][6] = {"RESET", "BUZZ1", "BUZZ2"};
byte baseAddress[6] = {"BASE"};

// Defines which buzzer code will run on - 0 = Reset, 1 = P1, 2 = P2
int BUZZER_NUM;

volatile bool packetAvailable = 0;
volatile bool buttonPressed = 0;

//--------------------------------

void sleep() {
  // Set full power down and go to sleep.
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  // Enable interrupts to wake us up
  attachInterrupt(0,rfInterrupt,LOW);
  attachInterrupt(1,buttonInterrupt,LOW);
  // Go to sleep
  sleep_mode();
  // Device is now asleep, and will be woken up by
  // either button press or packet receipt.
  sleep_disable();
  // Disable interrupts during normal running code
  detachInterrupt(0);
  detachInterrupt(1);
  power_all_enable();
}

void rfInterrupt() {
  packetAvailable = 1;
}

void buttonInterrupt() {
  buttonPressed = 1;
}

void setup() {
  pinMode(PIN_ID_H, INPUT);
  pinMode(PIN_ID_L, INPUT);
  BUZZER_NUM = (digitalRead(PIN_ID_H) << 1) | digitalRead(PIN_ID_L);
  // Set button as input
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  // Radio setup
  radio.begin();
  radio.setRetries(9,15); // Retry every 2500us, up to 15 times
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1,baseAddress);

  radio.startListening();
}

void loop() {
  // Process button press following interrupt
  if (buttonPressed)
  {
    uint8_t payload = BUZZER_NUM;
    radio.stopListening();
    radio.write(&payload,sizeof(payload));
    radio.startListening();
    buttonPressed = 0;
  }
  else if (radio.available() || packetAvailable)
  {
    uint8_t payload;
    radio.read(&payload,sizeof(payload));
    // Process payload
    packetAvailable = 0;
  }
  // Go back to sleep once button is released
  // and there are no packets to handle
  if ((digitalRead(PIN_BUTTON) == HIGH) && (!packetAvailable))
  {
    sleep();
  }

}
