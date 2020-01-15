# Payload SLI 2019

## Summary
This is the code for deployment of the Payload for NASA's 2019 SLI Payload challenge. During flight, at approximately 400 feet, we will remotely trigger separation of the payload section via radio; and later trigger release of the drone. 

We are using the [Adafruit Feather LoRa 32u4 (RFM9x) with the RadioHead (RFM9x) library](https://learn.adafruit.com/adafruit-feather-32u4-radio-with-lora-radio-module/using-the-rfm-9x-radio). 

## Instructions to run
* Open the code in the Arduino IDE
* Follow [these steps](https://learn.adafruit.com/adafruit-feather-32u4-radio-with-lora-radio-module/setup) for installing the drivers for the Feather
* In `Arduino > Preferences`, under "Show verbose mode during" check both compilation and upload (this solves some issues with the board)
* Install the [RadioHead library](https://learn.adafruit.com/adafruit-feather-32u4-radio-with-lora-radio-module/using-the-rfm-9x-radio) (Note: Find the folder to put this in by going to Arduino -> Preferences -> Sketchbook location. Open this folder and there should be a folder called libraries in there. Put the unzipped library inside there.)


## Rules for this repo and all Payload code
* DO NOT SEND CODE OVER SLACK/IMESSAGE/FB
* Use branches and PRs -- this way we can do code reviews and learn git the proper way
* Write clean code
* Heavily comment your code, so that it's easier for people who aren't familiar with C/C++/coding in general to understand it
* Make flowcharts [with proper conventions](https://en.wikibooks.org/wiki/Programming_Fundamentals/Flowcharts) for the code you write so we can A) help visualize it and B) put in documents
* Record known issues in known issues
* The curly bracket must be on the same line as the function declaration

```
void myFunction() {
// Good
} 

void myShittyFunction() 
{
// Bad
}
```

## Troubleshooting 
### If the board is not recognized, follow these steps:
In Mac this state looks like nothing showing up under ports, on the PC it gives an error like "device not recognized". Check out the [Feather troubleshooting guide](https://learn.adafruit.com/adafruit-feather-32u4-radio-with-lora-radio-module/faq) for more info. 

1. First, double check that you have verbose mode enabled (see Instructions to run). 
2. Double check that your USB cable is not charge-only
3. Upload Blink (File -> Examples -> 0.1 Basics -> Blink), and hit the reset button shortly after clicking upload. The builtin LED should begin pulsing. 
3. If this doesn't work, follow the following steps to re-burn the bootloader:

### How to re-burn the bootloader:
The bootloader may need to be burned if you flash code that causes a program halt. This will result in the "board not recognized" issue. 

1. Obtain the bootloader ([Pololu USB AVR Programmer v2.1](https://www.pololu.com/docs/0J67/3)), a micro-USB cable, and a computer with Arduino IDE installed
2. Using a breadboard, connect the following: 


| Bootloader | Feather |
|------------|---------|
| GND        | GND     |
| MOSI       | MO      |
| VCC        | 3v      |
| RST        | RST     |
| SCK        | SCK     |
| MISO       | MI      |

3. Plug in the USB cable to the bootloader (NOT the feather) and your computer
4. Under ports, there should be two USB devices showing up. Select the first one (I'm not sure it matters, but this is what has always worked for me)
4. In the Arduino IDE, under `Tools > Programmer`, select "Atmel STK500 development board"
5. Click "Burn bootloader"

The board should now be recognized by your computer and able to flash. You can verify this by uploading Blink (`File -> Examples -> 0.1 Basics -> Blink`)

Be sure to not flash your bad code again so you don't have to repeat this!
