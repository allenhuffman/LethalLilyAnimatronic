// LethalLilyWriter.ino
/*-----------------------------------------------------------------------------
 
 Lethal Lily Swamp Witch animatronic Control Box writer.
 By Allen C. Huffman (alsplace@pobox.com)
 www.subethasoftware.com
 
 This program writes the data to the Lethal Lily head over a
 serial and digital input.
 
 Hardware:
 
 https://sviservice.com/collections/2023/products/sv2323794
 
 Documentation:
 
 CONFIGURATION:
 1. Define the pins on the Arduino that will be used for TX in
 .  the Software Serial library (connected to the green wire), and the pulse
 .  sending pin (connected to the blue wire).
 
 VERSION HISTORY:
 2024-06-13 0.00 allenh - Created based on LethalLilyReader.ino
 
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
#define BLUE_WIRE_PIN   2   // Pulses OUT to HEAD

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
volatile unsigned int g_pulseCount = 0;

// Pattern one ON and OFF pulse time in micros.
const unsigned long g_pattern1[] = {
    108336,
    185892,
    161408,
    328592,
    200140,
    156160,
    191696,
    161860,
    222340,
    507668,
    63712,
    236864,
    481116,
    97364,
    77224,
    230036,
    189148,
    128228,
    219716,
    153316,
    144596,
    55636,
    66692,
    92192,
    99984,
    73600,
    158388,
    205228,
    244916,
    141988,
    97352,
    114224,
    194472,
    103252,
    239248,
    575908,
    459728,
    16744,
    64608,
    59992,
    64688,
    59916,
    65168,
    59624,
    64788,
    59820,
    67140,
    287636,
    400560,
    325912,
    225148,
    145256,
    80220,
    109232,
    278032,
    175556,
    253068,
    156324,
    174984,
    381868,
    333784,
    158776,
    108328,
    164388,
    108324,
    242136,
    297388,
    195164,
    155604,
    100400,
    269976,
    153988,
    211304,
    136536,
    175524,
    133576,
    147248,
    83664,
    91740,
    279872,
    381224,
    105904,
    83312,
    69700,
    119660,
    108528,
    216900,
    319000,
    139236,
    153120,
    69424,
    55812,
    114036,
    91828,
    127924,
    184272,
    216976,
    105800,
    133452,
    181064,
    139080,
    128108,
    69424,
    67880,
    122192,
    58644,
    74852,
    81140,
    77672,
    142156,
    60888,
    119856,
    61256,
    156028,
    77752,
    122588,
    63792,
    103344,
    102604,
    92016,
    91632,
    83948,
    94256,
    125492,
    75016,
    97204,
    111228,
    80860,
    155744
};


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

    for (int idx=0; idx<10; idx++) Serial.println();
    Serial.println("LethalLillyWriter - "__DATE__" "__TIME__);

    for (unsigned int idx=0; idx<10; idx++)
    {
        Serial.print (idx);
        Serial.print (" -> ");
        uint8_t newByte = convertValue (idx);
        Serial.print (newByte, HEX);
        Serial.print (" - ");
        Serial.println (newByte, BIN);
    }
}

/*--------------------------------------------------------------------------*/
// Main loop
/*--------------------------------------------------------------------------*/
void loop()
{
    Serial.println ("Sending pattern 1...");

    mySerial.write (convertValue (8)); // End any sequence.

    delay (1000);

    mySerial.write (convertValue (1)); // Sequence 1
    
    delay (745);

    for (int idx=0; idx < sizeof(g_pattern1)/sizeof(g_pattern1[0]); idx++)
    {
        if (idx & 1)
        {
            Serial.print ("On for ");
        }
        else
        {
            Serial.print ("Off for ");
        }
        Serial.println (g_pattern1[idx]);
        delayMicroseconds (g_pattern1[idx]);
    }

    Serial.println ("Done.");

    while (1);
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
// End of LethalLilyWriter.ino
