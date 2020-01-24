//Payload 2020 - Utils.ino
//Utility functions used by Reveiver.ino
//Last updated by Rohan Tuli 2020-01-23

// blinkLED ///////////////////////////////////////////////////////////////
void blinkLED(int delayTime) {
  digitalWrite(LED, HIGH);
  delay(delayTime);
  digitalWrite(LED, LOW);
  delay(delayTime);
}

// sdTest /////////////////////////////////////////////////////////////////
void sdTest() {
  Serial.print("Initializing SD card...");

  if (!SD.begin(SD_CS)) {
    Serial.println("initialization failed!");
    while (1);
  } else {
    Serial.println("initialization done.");
  }

  myFile = SD.open("test.txt", FILE_WRITE);

  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("");
    myFile.close();     // close the file:
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

// radioSetup ////////////////////////////////////////////////////////////
void radioSetup() {
  pinMode(RFM95_RST, OUTPUT);

  digitalWrite(RFM95_RST, HIGH); //I'm not sure if this line is necissary but it was in the code and you should figure out why it exists when it's getting set low immediately afterwards.

  // Reset the radio module
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  //if the radio isn't initialised, wait here
  while (!rf95.init());

  //blink the LED
  blinkLED(5000);

#ifdef DEBUG_MODE
  //Check if the radio module hasn't initilized
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1); //stop here and wait
  }
  //if it got this far, then the radio must have initialized
  Serial.println("LoRa radio init OK!");
#endif

  //If the frequency hasn't been set, blink the LED on a 5 second duty cycle
  if (!rf95.setFrequency(RF95_FREQ)) {
    while (1) {
      blinkLED(5000);
    }
  }

#ifdef DEBUG_MODE
  Serial.print("Set Freq to: ");
  Serial.println(RF95_FREQ);
#endif

  rf95.setTxPower(23, false); //set maximum power
}

// sdSetup //////////////////////////////////////////////////////////////////
void sdSetup() {
  #ifndef DEBUG_MODE //if not in debug mode
    //just initialize the SD card
    SD.begin(SD_CS);
  #endif

  #ifdef DEBUG_MODE //if in debug mode
    sdTest();
  #endif
}
