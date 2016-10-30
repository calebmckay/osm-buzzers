#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

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

int buttonState = 0;      // current state of button
int lastButtonState = 0;  // previous state of button

// Defines which buzzer code will run on - 0 = Reset, 1 = P1, 2 = P2
int BUZZER_NUM;

void setup() {
  pinMode(PIN_ID_H, INPUT);
  pinMode(PIN_ID_L, INPUT);
  BUZZER_NUM = (digitalRead(PIN_ID_H) << 1) | digitalRead(PIN_ID_L);
  // Set button as input
  pinMode(PIN_BUTTON, INPUT);

  // Radio setup
  radio.begin();
  radio.setRetries(9,15); // Retry every 2500us, up to 15 times
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1,baseAddress);

  radio.startListening();
}

void loop() {
  // Check for button press
  buttonState = digitalRead(PIN_BUTTON);
  if (buttonState != lastButtonState)
  {
    if (buttonState == HIGH)
    {
      uint8_t payload = BUZZER_NUM;
      radio.stopListening();
      radio.write(&payload,sizeof(payload));
      radio.startListening();
    }
  }

}
