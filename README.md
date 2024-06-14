# LethalLilyAnimatronic
Arduino code to talk to the Lethal Lily Swamp Witch animatronic head servos. This Halloween prop was sold in 2023 by Home Depot. Here is a demo on YouTube from Frightman Frights:

https://www.youtube.com/watch?v=NfH5UD395NY

Here is my blog series documenting my research and experiments:

https://subethasoftware.com/2024/06/12/hacking-home-depot-lethal-lily-animatronic-props/

# Lethal Lily Information
* Home Depot press release from 2023: https://corporate.homedepot.com/news/products/innovation-quality-and-value-introducing-home-depots-2023-halloween-product-lineup
* Unofficial Wiki: https://seasonalvisionslimited.fandom.com/wiki/Lethal_Lily
* Replacement parts (where you can order just the head, control box, power supply, sensor, etc.): https://sviservice.com/collections/2023/products/sv2323794

# Technical Specs
## Power Supply
The Lethal Lily Control Box uses a 5.9V 3A DC power supply.

* MNodel No: GQ24-059300-AU
* Input: 100-240V ~ 50/60Hz 1.0A Max
* Output: 5.9V --- 3.0A
* Barrel connector has center tip as POSITIVE.

## Control Box
The Control Box has the following:
* 5.9V Power Input (barrel connector).
* Volume knob.
* Three-Position Switch - Sensor, Off/Try Me, Lights Only
* Speaker
* "Try Me" switch input. (Unsure; probably sdomething like a 1/8" jack).

There are four cables coming out that run to square connectors:
* 2-Wire (red/black) - ??? lights ???
* 2-Wire w/plastic housing (red/black) - ??? lights ???
* 3-Wire (white/blue/green) - motion sensor
* 4-Wire (green/red/black/blue) - to head

## Protocol
The green wire sends out serial data at around 550 baud. When Lethal Lily is triggered, a start byte will be sent, and at the end of the sequence, and end byte will be sent.

The format of the byte is the right four bits as the number (1-7 for sequence start, and 8 for sequence end), and the left four bits are the inverted bits of the same value.

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

When sequence playback begins, the blue wire will begin pulsing. These are the signals the control the servo. Details TBD.

## I/O pins and 5.9V WARNING!!!
* The green wire serial appears to be 4.4V, so it should be fine reading on an Arduino with 5V input pins.
* The blue wire pulse output appears to be about 5.4V. This could damage hardware that can only handle 3.3V or 5V I/O. A voltage divider made of two resistors can be used to lower this to safe level for an Arduino to read. For my experiments, I used a 10K Ohm resistor between the blue wire and the Arduino digital input pin, then ran a 100K Ohm resistor from the same input pin over to ground. (ChatGPT suggestion; I will update this with more accurate information once I have tested it.)

# TO DO
More to come...
  
