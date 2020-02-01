//Payload 2020 - Utils.ino
//Utility functions used by Reveiver.ino
//Last updated by Rohan Tuli 2020-01-31
//Radio functionality only - no SD card

// blinkLED ///////////////////////////////////////////////////////////////
void blinkLED(int delayTime) {
  digitalWrite(LED, HIGH);
  delay(delayTime);
  digitalWrite(LED, LOW);
  delay(delayTime);
}

// sdTest /////////////////////////////////////////////////////////////////
//test code to make sure the SD card is working properly
void sdTest() {
  Serial.println("Running sdTest()--------------------------------");

  //initialize the SD card
  Serial.println("Beginning initialization...");
  if (!SD.begin(SD_CS)) {
    Serial.println("SD.begin(SD_CS) failed - did not initialize :(");
    while (1);
  } else {
    Serial.println("SD.begin(SD_CS) passed - succesfully initialized");
  }

  //check to see if a test file already exists
  int fileExists = 0;
  Serial.println("\nChecking for an existing file...");
  if (SD.exists("test.txt")) {
    fileExists = 1;
    Serial.println("SD.exists(\"test.txt\") passed - test.txt already exists.");
  } else {
    fileExists = 0;
    Serial.println("SD.exists(\"test.txt\")failed - test.txt does not exist");
  }

  Serial.println("\nOpening, writing to, and closing the file multiple times...");

  for (int i = 0; i < 3; i++) {
    //prepare string to put on SD card
    char* message = "Chip select turns me on";
    char* writeString = calloc(4 + strlen(message), sizeof(char)); //length of message + space: + maximum two digits
    sprintf(writeString, "%d: %s", i, message);

    //open the file and write to it
    Serial.println("Writing...");
    File testFile = SD.open("test.txt", FILE_WRITE);
    testFile.println(writeString);
    testFile.close();

    free(writeString); //free string
  }

  Serial.println("\nCompleted sdTest(). View the contents of test.txt on a normal computer.");
}

// sdWrite ////////////////////////////////////////////////////////////////
//deal with chip select and write to the SD card
void sdWrite(char* logMessage) {
  //deactivate radio
  //digitalWrite(RADIO_CS, HIGH);
  
  //digitalWrite(SD_CS, LOW);
  
  //open file
  //File logFile = SD.open("log.txt", FILE_WRITE);

  //create a string that has the time and log message
  char* outputMessage = calloc(20 + strlen(logMessage), sizeof(char)); //20 = 19(maximum size of long int) + 1 (space)
  sprintf(outputMessage, "%d %s", millis(), logMessage);

  #ifdef DEBUG_MODE
    Serial.println(outputMessage);
  #endif
  /*
  //write to the SD card
  if (logFile) {
    logFile.println(outputMessage);
      //close the file
      logFile.close();
  }
*/
  //free string memory
  free(outputMessage);
  

  
  //deactivate the SD card
  //digitalWrite(SD_CS, HIGH);
  
  //activate the radio
  //digitalWrite(RADIO_CS, LOW);
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
    //SD.begin(SD_CS);
  #endif

  #ifdef DEBUG_MODE //if in debug mode
    sdTest();
  #endif

  //deactivate the SD card chip select
  digitalWrite(SD_CS, HIGH);
}
