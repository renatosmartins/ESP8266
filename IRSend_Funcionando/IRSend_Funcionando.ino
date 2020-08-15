/* IRremoteESP8266: IRsendDemo - demonstrates sending IR codes with IRsend.
 *
 * Version 1.0 April, 2017
 * Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009,
 * Copyright 2009 Ken Shirriff, http://arcfn.com
 *
 * An IR LED circuit *MUST* be connected to the ESP8266 on a pin
 * as specified by IR_LED below.
 *
 * TL;DR: The IR LED needs to be driven by a transistor for a good result.
 *
 * Suggested circuit:
 *     https://github.com/markszabo/IRremoteESP8266/wiki#ir-sending
 *
 * Common mistakes & tips:
 *   * Don't just connect the IR LED directly to the pin, it won't
 *     have enough current to drive the IR LED effectively.
 *   * Make sure you have the IR LED polarity correct.
 *     See: https://learn.sparkfun.com/tutorials/polarity/diode-and-led-polarity
 *   * Typical digital camera/phones can be used to see if the IR LED is flashed.
 *     Replace the IR LED with a normal LED if you don't have a digital camera
 *     when debugging.
 *   * Avoid using the following pins unless you really know what you are doing:
 *     * Pin 0/D3: Can interfere with the boot/program mode & support circuits.
 *     * Pin 1/TX/TXD0: Any serial transmissions from the ESP8266 will interfere.
 *     * Pin 3/RX/RXD0: Any serial transmissions to the ESP8266 will interfere.
 *   * ESP-01 modules are tricky. We suggest you use a module with more GPIOs
 *     for your first time. e.g. ESP-12 etc.
 */

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>

#define IR_LED 4  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(IR_LED);  // Set the GPIO to be used to sending the message.

uint16_t rawData[231] = {3020, 9006,  522, 524,  508, 1548,  522, 524,  518, 548, 
                          492, 552,  494, 550,  520, 524,  520, 526,  518, 548,  520, 1514, 
                          520, 548,  486, 558,  520, 1540,  466, 578,  490, 552,  522, 1566, 
                          520, 1516,  518, 1540,  520, 1566,  522, 1514,  522, 548,  490, 552,
                          520, 522,  520, 526,  518, 548,  518, 550,  522, 524,  520, 522,  520,
                          548,  492, 552,  520, 522,  520, 526,  516, 550,  492, 552,  520, 554,
                          512, 524,  506, 564,  546, 498,  520, 524,  520, 524,  518, 548,  490, 552, 
                          520, 522,  520, 526,  518, 548,  492, 552,  520, 524,  520, 524,  542, 552, 
                          492, 552,  520, 522,  522, 526,  510, 1546,  522, 1538,  496, 1540,  520, 1540,  
                          468, 1648,  3000, 9028,  522, 1516,  538, 556,  520, 522,  520, 522,  520, 526,  
                          518, 548,  492, 550,  520, 524,  520, 524,  518, 1540,  520, 524,  518, 526,  508,
                          1550,  520, 524,  520, 1568,  520, 1540,  494, 548,  520, 1540,  520, 1518,  518, 1540, 
                          522, 1540,  490, 1570,  518, 1542,  546, 1516,  494, 1566,  464, 606,  492, 550,  522, 526, 
                          518, 1540,  520, 1516,  510, 1548,  520, 526,  518, 1540,  520, 522,  572, 522,  520, 526,  518, 
                          548,  490, 554,  520, 522,  520, 526,  518, 1540,  520, 524,  516, 1542,  522, 524,  546, 1540,  
                          520, 522,  520, 576,  488, 554,  494, 550,  520, 522,  520, 524,  512, 556,  520, 1516,  586, 1498, 
                          522, 1540,  520, 1540,  494};  // UNKNOWN 969EB595

void setup() {
  irsend.begin();
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
}

void loop() {
#if SEND_RAW
  Serial.println("a rawData capture from IRrecvDumpV2");
  irsend.sendRaw(rawData, 231, 38);  // Send a raw data capture at 38kHz.
#endif  // SEND_RAW
  delay(2000);
}
