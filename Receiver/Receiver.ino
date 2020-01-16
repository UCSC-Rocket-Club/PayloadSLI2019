// Feather9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_TX
 
#include <SPI.h>
#include <RH_RF95.h>
//#define DEBUG_MODE  0
// When flight mode is enabled, do not run any serial code because it will not run anything else 
// and waste energy. This will disable any serial logs, so 
// use DEBUG_MODE 0 for debugging, and DEBUG_MODE 1 for actual flights
 
// for Feather32u4 RFM9x
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7
 
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
 
// Blinky on receipt
#define LED 13
 
void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  
  #ifdef DEBUG_MODE
  // initialize serial
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
  delay(100);
 
  Serial.println("Feather LoRa RX Test!");
  #endif
 
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    ;
  }

    delay(5000);
    digitalWrite(LED, HIGH);
    delay(5000);
    digitalWrite(LED, LOW);

  #ifdef DEBUG_MODE 
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
  #endif
  
  #ifdef DEBUG_MODE
  Serial.println("LoRa radio init OK!");
  #endif
  
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  //#ifdef DEBUG_MODE
  if (!rf95.setFrequency(RF95_FREQ)) {
    delay(5000);
    digitalWrite(LED, HIGH);
    delay(5000);
    digitalWrite(LED, LOW);
    //Serial.println("setFrequency failed");
    while (1);
  }
  //#endif

  #ifdef DEBUG_MODE
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  #endif
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
 
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}

static int check = -1;
static uint8_t confirmFireMsg[] = "?? -- Are you sure you want to fire? y/n -- ??";
static uint8_t abortingMsg[] = "!! -- Aborting -- !!";
static uint8_t fireInstructionMsg[] = "** -- Enter f before y to fire -- **";
static uint8_t deployingMsg[] = "!! -- Deploying Now -- !!";
static uint8_t finishedDeployingMsg[] = "!! -- Finished Deployment -- !!";
static uint8_t invalidInputMsg[] = "**-- Not a valid input,  send 'f' , 'y' , 'n' , or 'p'-- **";

void loop() {
 

  if (rf95.available()) {    
    // initialization for message array
    uint8_t buf[3];
    uint8_t len = sizeof(buf);

    // if data received
    if (rf95.recv(buf, &len) ) {

      Serial.println("------------------------------------");
      
      // writing and printing data block
      digitalWrite(LED, HIGH);
      RH_RF95::printBuffer("Received: ", buf, len);
      //Serial.println(Serial.parseInt(buf));

      Serial.print("Got: ");
      Serial.println((char*)buf);

      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);

      Serial.println("------------------------------------");

      // ------------- input checking -------------
      
      char* input = (char*)buf;
        
      switch(*input){
         case 'f':
             rf95.send(confirmFireMsg, sizeof(confirmFireMsg));
             check = 0;
             break;

         case 'n':
              rf95.send(abortingMsg, sizeof(abortingMsg));
              break;
             
         case 'y':
             if(check == 0){ 
                rf95.send(deployingMsg, sizeof(deployingMsg));
                digitalWrite(10,HIGH);
                delay(2000);
                digitalWrite(10,LOW);

                rf95.send(finishedDeployingMsg, sizeof(finishedDeployingMsg));
                check = 1;
             }
             else
                rf95.send(fireInstructionMsg, sizeof(fireInstructionMsg));
             break;
             
         default:
             //rf95.send(backbs, sizeof(backbs));
             //check = 3;
             rf95.send(invalidInputMsg, sizeof(invalidInputMsg));
             break;
         
      }
      digitalWrite(LED, LOW);
    }

    #ifdef DEBUG_MODE
    else {
      Serial.println("!! -- Receive failed -- !!");
    }
    #endif


  }
}