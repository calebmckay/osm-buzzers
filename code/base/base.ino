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
int PIN_CE = 7;
int PIN_CSN = 8;
int PIN_SCK = 13;
int PIN_MOSI = 11;
int PIN_MISO = 12;
int PIN_IRQ = 2;

//Note: LEDs are active-low
int PIN_LED1 = 5;
int PIN_LED2 = 6;


//---------------------------------
RF24 radio(PIN_CE, PIN_CSN);

// Define addresses for each machine
byte buttonAddress[][6] = {"RESET", "BUZZ1", "BUZZ2"};
byte baseAddress[6] = {"BASE"};

int litLED = 0;

void setup() {
  // Configure pins for LEDs
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  
  radio.begin();
  radio.setRetries(10,15);
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1,baseAddress);

  radio.startListening();
}

void loop() {
  uint8_t receivedPayload;
  if(radio.available())
  {
    while(radio.available())
    {
      radio.read(&receivedPayload,sizeof(receivedPayload));
      switch(receivedPayload)
      {
        case 0:
          litLED = 0;
          break;
        case 1:
          if(litLED==0) litLED = 1;
          break;
        case 2:
          if(litLED==0) litLED = 2;
          break;
        default:
          //Possible error handling here
          break;
      }
    }
  }
  switch(litLED)
  {
    case 0:
      digitalWrite(PIN_LED1,HIGH);
      digitalWrite(PIN_LED2,HIGH);
      break;
    case 1:
      digitalWrite(PIN_LED1,LOW);
      digitalWrite(PIN_LED2,HIGH);
      break;
    case 2:
      digitalWrite(PIN_LED1,HIGH);
      digitalWrite(PIN_LED2,LOW);
      break;
    default:
      //Possible error handling here
      break;
  }
}
