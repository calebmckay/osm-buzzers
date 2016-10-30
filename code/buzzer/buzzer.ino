#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

//---------------------------------
// Parameters
//---------------------------------
// Defines which buzzer code will run on - 0 = Reset, 1 = P1, 2 = P2
int BUZZER_NUM = 0;

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


//---------------------------------
RF24 radio(PIN_CE, PIN_CSN);

// Define addresses for each machine
byte buttonAddress[][6] = {"RESET", "BUZZ1", "BUZZ2"};
byte baseAddress[6] = {"BASE"};

int buttonState = 0;      // current state of button
int lastButtonState = 0;  // previous state of button

void setup() {
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
