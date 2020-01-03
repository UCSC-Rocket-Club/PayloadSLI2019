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
2. Hit the reset button, and hold it for like a second. The builtin LED should begin pulsing, then it might show up.
3. (Optional) If you think it may be acting this way because you put on bad code, upload Blink (File -> Examples -> 0.1 Basics -> Blink).
3. Rinse and repeat. It may take a couple dozen tries.


## Known issues
* Sometimes the boards aren't recognized by the computers (tried on Mac and Linux and resetting). It usually works the first time we plug the board into a new computer, then shortly stops working, so this may be a udev issue - still need to investigate this more
