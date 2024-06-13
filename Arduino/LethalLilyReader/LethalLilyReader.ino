// LethalLilyReader.ino
/*-----------------------------------------------------------------------------
 
 Lethal Lily Swamp Witch animatronic Control Box reader.
 By Allen C. Huffman (alsplace@pobox.com)
 www.subethasoftware.com
 
 This program reads the data come from the Lethal Lily Control Box over a
 serial and digital input.
 
 Hardware:
 
 https://sviservice.com/collections/2023/products/sv2323794
 
 Documentation:
 
 CONFIGURATION:
 1. Define the pins on the Arduino that will be used for RX in
 .  the Software Serial library (connected to the green wire), and the pulse
 .  counting pin (connected to the blue wire with 10K/100K resistors to drop
 .  the voltage from 5.4V down to below 5V).
 
 VERSION HISTORY:
 2024-06-12 0.00 allenh - Created basic serial (green wire) reader.
 2014-06-13 0.01 allenh - Adding pulse (blue wire) support.
 
 TODO:
 * Capture data and then playback to the head.
 
 TOFIX:
 * TODO...

 EXAMPLE OUTPUT:

[0xE1 - 11100001] PLAYING #1 ... [0x78 - 1111000] STOPPED.
Pulses: 65
[0xD2 - 11010010] PLAYING #2 ... [0x78 - 1111000] STOPPED.
Pulses: 71
[0xC3 - 11000011] PLAYING #3 ... [0x78 - 1111000] STOPPED.
Pulses: 59
[0xB4 - 10110100] PLAYING #4 ... [0x78 - 1111000] STOPPED.
Pulses: 68
[0xA5 - 10100101] PLAYING #5 ... [0x78 - 1111000] STOPPED.
Pulses: 75
[0x96 - 10010110] PLAYING #6 ... [0x78 - 1111000] STOPPED.
Pulses: 74
[0x87 - 10000111] PLAYING #7 ... [0x78 - 1111000] STOPPED.
Pulses: 57
[0xE1 - 11100001] PLAYING #1 ... [0x78 - 1111000] STOPPED.
Pulses: 65
----------------------------------------------------------------------------*/ 

/*--------------------------------------------------------------------------*/
// Configuration
/*--------------------------------------------------------------------------*/
#define BAUD_RATE       550
#define GREEN_WIRE_PIN  12  // Serial RX
#define BLUE_WIRE_PIN   2   // Pulses IN

/*--------------------------------------------------------------------------*/
// Includes
/*--------------------------------------------------------------------------*/
#include <SoftwareSerial.h>

/*--------------------------------------------------------------------------*/
// RX pin connected to GREEN wire.
// TX pin not used.
/*--------------------------------------------------------------------------*/
SoftwareSerial mySerial(GREEN_WIRE_PIN, 3); // RX, TX

/*--------------------------------------------------------------------------*/
// Globals
/*--------------------------------------------------------------------------*/
volatile unsigned int g_pulseCount = 0;

/*--------------------------------------------------------------------------*/
// Setup
/*--------------------------------------------------------------------------*/
void setup() {
    // Arduino console port.
    Serial.begin(9600);
    while (!Serial);

    // Control Box serial baud rate.
    mySerial.begin(BAUD_RATE);

    pinMode(BLUE_WIRE_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(BLUE_WIRE_PIN), countPulse, RISING);

    for (int idx=0; idx<10; idx++) Serial.println();
    Serial.println("LethalLillySerial - "__DATE__" "__TIME__);
}

/*--------------------------------------------------------------------------*/
// Main loop
/*--------------------------------------------------------------------------*/
void loop()
{
    static bool isPlaying = false;
    unsigned char ch;

    while (mySerial.available())
    {
        // Read byte from Control Box.
        ch = mySerial.read();

        // Display byte received as hex and binary.
        Serial.print ("[0x");
        Serial.print (ch, HEX);
        Serial.print (" - ");
        Serial.print (ch, BIN);
        Serial.print ("] ");

        // Byte will have the sequence number (1-7) as the right four bits,
        // and the inverse of those bits as the left four bits. At the end
        // of the sequence, a value of 8 is sent the same way.
        //
        //              Hex Binary
        //              --  ---- ----
        // Sequence 1 - E1  1110 0001 (1)
        // Sequence 2 - D2  1101 0010 (2)
        // Sequence 3 - C3  1100 0011 (3)
        // Sequence 4 - B4  1011 0100 (4)
        // Sequence 5 - A5  1010 0101 (5)
        // Sequence 6 - 96  1001 0110 (6)
        // Sequence 7 - E1  1000 0111 (7)
        // End of Seq - 78  0001 1000 (8)

        // Validate. Left nibble AND with right nibble should be zero.
        if ( ((ch & 0xf0 >> 8) & (ch & 0x0f)) != 0)
        {
            // If not, bad byte received.
            Serial.println ("INVALID.");
        }
        else // Good byte received.
        {
            // Check for end of sequence (8).
            if ((ch & 0x0f) == 8)
            {
                Serial.println ("STOPPED.");

                if (isPlaying == true)
                {
                    Serial.print ("Pulses: ");
                    Serial.println (g_pulseCount);

                    isPlaying = false;
                }
            }
            else
            {
                g_pulseCount = 0;

                Serial.print ("PLAYING #");
                Serial.print (ch & 0x0f);
                Serial.print (" ... ");

                isPlaying = true;
            }
        }
    } // end of while (mySerial.available())
} // end of loop()

/*--------------------------------------------------------------------------*/
// Pulse counter.
/*--------------------------------------------------------------------------*/
void countPulse() {
    g_pulseCount++;
}

// End of LethalLilyReader.ino
