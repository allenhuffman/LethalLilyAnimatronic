// LethalLilyMouthTest.ino
/*-----------------------------------------------------------------------------
 
 Lethal Lily Swamp Witch animatronic Control Box servo tester.
 By Allen C. Huffman (alsplace@pobox.com)
 www.subethasoftware.com
 
 This program writes the data to the Lethal Lily head over a
 serial and digital output.
 
 Hardware:
 
 https://sviservice.com/collections/2023/products/sv2323794
 
 Documentation:
 
 CONFIGURATION:
 1. Define the pins on the Arduino that will be used for TX in the
    SoftwareSerial library (connected to the green wire), and the pulse
    sending pin (connected to the blue wire).
 
 VERSION HISTORY:
 2024-06-14 0.00 allenh - Created, based on LethalLilyWriter.
 
 TODO:
 * TODO...
 
 TOFIX:
 * TODO...

 ---------------------------------------------------------------------------*/ 

/*--------------------------------------------------------------------------*/
// Configuration
/*--------------------------------------------------------------------------*/
#define BAUD_RATE       550
#define GREEN_WIRE_PIN  12  // Serial TX to HEAD
#define BLUE_WIRE_PIN   7   // Pulses OUT to HEAD
#define BUTTON_PIN      2

// HiLetgo 5V 4 Channel Relay Shield

typedef enum
{
  RELAY1 = 4,
  RELAY2 = 5,
  RELAY3 = 6,
  RELAY4 = 7,
} RelayEnum;

/*--------------------------------------------------------------------------*/
// Includes
/*--------------------------------------------------------------------------*/
#include <SoftwareSerial.h>

/*--------------------------------------------------------------------------*/
// RX pin connected to GREEN wire.
// TX pin not used.
/*--------------------------------------------------------------------------*/
SoftwareSerial mySerial(3, GREEN_WIRE_PIN); // RX, TX

/*--------------------------------------------------------------------------*/
// Globals
/*--------------------------------------------------------------------------*/

//Prototypes
uint8_t convertValue (unsigned int value);

/*--------------------------------------------------------------------------*/
// Setup
/*--------------------------------------------------------------------------*/
void setup() {
    // Arduino console port.
    Serial.begin(9600);
    while (!Serial);

    // Control Box serial baud rate.
    mySerial.begin(BAUD_RATE);

    pinMode(BLUE_WIRE_PIN, OUTPUT);
    
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    for (int idx=0; idx<10; idx++) Serial.println();
    
    Serial.println("LethalLilyServoTest - "__DATE__" "__TIME__);
}

/*--------------------------------------------------------------------------*/
// Main loop
/*--------------------------------------------------------------------------*/
void loop()
{
    mySerial.write (convertValue (8)); // End any sequence.

    delay (1000);

    // Start eyes/neck animation.
    mySerial.write (convertValue (1));

    while (1)
    {
        digitalWrite (BLUE_WIRE_PIN, !digitalRead(BUTTON_PIN));
    }

    Serial.println ();

    mySerial.write (convertValue (8)); // End any sequence.
} // end of loop()


/*--------------------------------------------------------------------------*/
// Convert number (1-8) to Lethal Lilly byte format (0011 1100).
/*--------------------------------------------------------------------------*/
uint8_t convertValue (unsigned int value)
{
    uint8_t returnByte = 0;

    if ((value < 1) || (value > 8))
    {
        Serial.println ("convertValue must be 1-7 (pattern) or 8 (end).");
        returnByte = 0;
    }
    else
    {
        returnByte = value | (~value << 4);
    }

    return returnByte;
}

// Created by ChatGPT.
void customDelayMicroseconds(unsigned long delayTimeMicros) {
    unsigned long delayTimeMillis = delayTimeMicros / 1000;
    unsigned long remainingMicros = delayTimeMicros % 1000;

    delay(delayTimeMillis); // This will not delay if delayTimeMillis is 0
    delayMicroseconds(remainingMicros); // This will handle the remaining microseconds
}


// End of LethalLilyMouthTest.ino
