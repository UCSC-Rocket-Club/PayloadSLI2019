//Payload 2020 - Reviever.ino
//Last updated by Rohan Tuli 2020-01-23

// Use DEBUG_MODE 0 for debugging, and DEBUG_MODE 1 for actual flights
#define DEBUG_MODE 0 //When flight mode is enabled, do not run any serial code because it will not run anything else and waste energy. This will disable any serial logs.

// Libraries for Radio and SD card functionality
#include <SPI.h>
#include <SD.h>
#include <RH_RF95.h>
#include <string.h>

// Chip Select pins for radio and SD card
#define SD_CS 10
#define RADIO_CS 8

//LED output pin
#define LED 13

//Mosfet pin (gate)
#define GATE 2

// Other definitions for Feather32u4 RFM9xi radio
#define RFM95_RST 4
#define RFM95_INT 7
#define RF95_FREQ 915.0 //Radio frequency - must be the same between transmitter and reviever

// File for use on the SD card
File myFile;

// Singleton instance of the radio driver
RH_RF95 rf95(RADIO_CS, RFM95_INT);

static int receivedCharFlag = -1; //flag for what character is recieved. Starts at -1, becomes 0 if "f" recived, becomes 1 if "y" recieved

//Message strings to be used by the radio
static uint8_t confirmFireMsg[] = "?? -- Are you sure you want to fire? y/n -- ??";
static uint8_t abortingMsg[] = "!! -- Aborting -- !!";
static uint8_t fireInstructionMsg[] = "** -- Enter f before y to fire -- **";
static uint8_t deployingMsg[] = "!! -- Deploying Now -- !!";
static uint8_t finishedDeployingMsg[] = "!! -- Finished Deployment -- !!";
static uint8_t invalidInputMsg[] = "**-- Not a valid input,  send 'f' , 'y' , 'n' , or 'p'-- **";

//Function prototypes
void blinkLED(int delayTime);
void radioSetup();
void sdSetup();
void sdTest();
void sdWrite(char* logMessage);

void setup(){
  #ifdef DEBUG_MODE
    // Initialize serial
    Serial.begin(115200);
    while (!Serial) {
      delay(1); //ms delay
    }
    delay(100);
    Serial.println("Feather LoRa RX Test!");
  #endif

  //Set digital pin for the LED to output mode
  pinMode(LED, OUTPUT);

  radioSetup(); //call the radio setup function
  sdSetup(); //call the SD setup function
}

void loop() {
  if (rf95.available()) { //if the radio is available
    // initialization for message array
    uint8_t buf[3];
    uint8_t len = sizeof(buf);
    
    // if data received
    if (rf95.recv(buf, &len) ) {
      #ifdef DEBUG_MODE
        Serial.println("------------------------------------");
      #endif

      // writing and printing data block
      digitalWrite(LED, HIGH);
      RH_RF95::printBuffer("Received: ", buf, len);

      #ifdef DEBUG_MODE
        Serial.print("Got: ");
        Serial.println((char*)buf);
  
        Serial.print("RSSI: ");
        Serial.println(rf95.lastRssi(), DEC);
  
        Serial.println("------------------------------------");
      #endif

      char* input = (char*)buf;
      
      switch (*input) {
        //when f is received
        case 'f':
          rf95.send(confirmFireMsg, sizeof(confirmFireMsg));
          sdWrite("Confirm Fire - message receieved");
          receivedCharFlag = 0;
          break;
       //when n is received
        case 'n':
          rf95.send(abortingMsg, sizeof(abortingMsg));
          sdWrite("Aborting - message receieved");
          break;
        //when y is received
        case 'y':
          if (receivedCharFlag == 0) {
            rf95.send(deployingMsg, sizeof(deployingMsg));
            //trigger the e-match
            digitalWrite(GATE, HIGH);
            delay(2000);
            digitalWrite(GATE, LOW);
            rf95.send(finishedDeployingMsg, sizeof(finishedDeployingMsg));
            sdWrite("Deploying -message receieved");
            receivedCharFlag = 1;
          }
          else {
            rf95.send(fireInstructionMsg, sizeof(fireInstructionMsg));
            sdWrite("Message not recognized - message receieved");
          }
          break;
        //default case
        default:
          rf95.send(invalidInputMsg, sizeof(invalidInputMsg));
          break;
      }
      digitalWrite(LED, LOW);
    } else {
      #ifdef DEBUG_MODE
        Serial.println("!! -- Receive failed -- !!");
      #endif
    }
  }
}
