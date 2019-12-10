// Feather9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_TX
 
#include <SPI.h>
#include <RH_RF95.h>
#define FLIGHT_MODE  1
 
// for Feather32u4 RFM9x
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7
/* for feather m0 RFM9x
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
*/
 
/* for shield 
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 7
*/
 
/* Feather 32u4 w/wing
#define RFM95_RST     11   // "A"
#define RFM95_CS      10   // "B"
#define RFM95_INT     2    // "SDA" (only SDA/SCL/RX/TX have IRQ!)
*/
 
/* Feather m0 w/wing 
#define RFM95_RST     11   // "A"
#define RFM95_CS      10   // "B"
#define RFM95_INT     6    // "D"
*/
 
#if defined(ESP8266)
  /* for ESP w/featherwing */ 
  #define RFM95_CS  2    // "E"
  #define RFM95_RST 16   // "D"
  #define RFM95_INT 15   // "B"
 
#elif defined(ESP32)  
  /* ESP32 feather w/wing */
  #define RFM95_RST     27   // "A"
  #define RFM95_CS      33   // "B"
  #define RFM95_INT     12   //  next to A
 
#elif defined(NRF52)  
  /* nRF52832 feather w/wing */
  #define RFM95_RST     7   // "A"
  #define RFM95_CS      11   // "B"
  #define RFM95_INT     31   // "C"
  
#elif defined(TEENSYDUINO)
  /* Teensy 3.x w/wing */
  #define RFM95_RST     9   // "A"
  #define RFM95_CS      10   // "B"
  #define RFM95_INT     4    // "C"
#endif
 
 
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
  
  #ifdef FLIGHT_MODE
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

  #ifdef FLIGHT_MODE 
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
  #endif
  
  #ifdef FLIGHT_MODE
  Serial.println("LoRa radio init OK!");
  #endif
  
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  #ifdef FLIGHT_MODE
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  #endif

  #ifdef FLIGHT_MODE
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  #endif
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
 
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);




}

static int check = -1;
static uint8_t backf[] = "?? -- Are you sure you want to fire? y/n -- ??";
static uint8_t backcf[] = "** -- Enter f before y to fire -- **";
static uint8_t backy[] = "!! -- Deplyoying Now -- !!";
static uint8_t backsy[] = "!! -- Finished Deployment -- !!";
static uint8_t backp[] = "!! -- Pinged -- !!";
static uint8_t backbs[] = "\n** -- Send 'f' , 'y' , 'n' , 'p' for valid input -- **";

void loop()
{
 
 
  if (rf95.available())
  {    
    // initialization for message array
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    #ifdef FLIGHT_MODE
    if (rf95.recv(buf, &len) )
    {
      
      // writing and printing data block
      digitalWrite(LED, HIGH);
      RH_RF95::printBuffer("Received: ", buf, len);
      Serial.println("------------------------------------");
      
      //Serial.print("Got: ");
      
      Serial.println((char*)buf);

      Serial.println("------------------------------------");

      //Serial.print("RSSI: ");
      //Serial.println(rf95.lastRssi(), DEC);

      // ------------- input checking -------------
      
      char* input = (char*)buf;
        
      switch(*input){
         case 'f' :
             rf95.send(backf, sizeof(backf));
             check = 0;
             break;
             
         case 'y':
             if(check == 0){ 
                rf95.send(backy, sizeof(backy));
                digitalWrite(10,HIGH);
                delay(5000);
                digitalWrite(10,LOW);
 
                rf95.send(backsy, sizeof(backsy));
                check = 1;
             }
             else
                rf95.send(backcf, sizeof(backcf));
             break;
                
         case 'p' :
             rf95.send(backp, sizeof(backp));
             check = 2;
             break;

         case '\n' :
             break;

         
         default:
             //rf95.send(backbs, sizeof(backbs));
             //check = 3;
             break;
         
      }
      
      
      // ------------- Finish -------------
      
      // Send a reply
      //uint8_t data[] = "And hello back to you";
      //rf95.send(data, sizeof(data));
      //rf95.waitPacketSent();
      //Serial.println("Sent a reply");
      
      digitalWrite(LED, LOW);
    }
    #endif

    #ifdef FLIGHT_MODE
    else
    {
      Serial.println("!! -- Receive failed -- !!");
    }
    #endif


  }
}
