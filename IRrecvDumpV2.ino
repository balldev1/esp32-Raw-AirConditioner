// remote raw haier
// ON
// uint16_t rawData[229] = {3094, 2998,  3080, 4464,  564, 1636,  564, 560,  566, 1634,  566, 560,  566, 560,  566, 1634,  566, 1634,  564, 562,  564, 1634,  566, 562,  564, 1634,  566, 560,  566, 1634,  564, 562,  564, 562,  564, 562,  564, 1634,  568, 1632,  568, 1632,  568, 558,  566, 558,  566, 558,  566, 560,  564, 560,  566, 562,  564, 562,  566, 558,  568, 560,  566, 560,  564, 560,  564, 562,  566, 560,  566, 560,  564, 1636,  564, 560,  566, 560,  566, 560,  566, 562,  564, 560,  568, 560,  562, 1636,  566, 558,  566, 1636,  564, 562,  564, 560,  566, 560,  566, 560,  566, 558,  566, 560,  564, 562,  564, 560,  566, 560,  566, 560,  564, 560,  566, 560,  566, 562,  564, 560,  566, 560,  568, 1632,  566, 562,  564, 562,  566, 560,  566, 558,  566, 560,  564, 560,  566, 560,  566, 560,  566, 560,  566, 560,  566, 560,  566, 560,  566, 560,  566, 560,  566, 560,  568, 558,  568, 560,  566, 560,  566, 558,  566, 558,  564, 562,  566, 560,  566, 560,  568, 558,  566, 562,  564, 560,  564, 562,  566, 558,  566, 558,  566, 560,  566, 560,  566, 560,  566, 560,  564, 562,  564, 562,  564, 562,  562, 562,  566, 560,  566, 560,  564, 562,  564, 560,  564, 562,  516, 1684,  566, 560,  566, 1634,  566, 560,  564, 560,  566, 1634,  566, 1634,  568, 560,  566, 560,  566, 1634,  566, 1634,  564};  // HAIER_AC_YRW02

// OFF
// uint16_t rawData[229] = {3094, 2996,  3078, 4464,  510, 1688,  536, 592,  564, 1634,  566, 558,  566, 560,  566, 1634,  566, 1634,  510, 616,  566, 1632,  566, 560,  566, 1634,  566, 560,  564, 562,  566, 560,  514, 612,  566, 560,  564, 1634,  512, 1686,  566, 1634,  566, 558,  566, 558,  536, 590,  568, 558,  566, 560,  564, 558,  568, 558,  568, 558,  566, 562,  564, 562,  540, 584,  536, 590,  566, 558,  566, 560,  564, 560,  534, 590,  568, 558,  510, 614,  566, 560,  566, 560,  566, 560,  566, 1634,  564, 562,  566, 1632,  568, 558,  568, 558,  566, 560,  564, 560,  514, 612,  566, 560,  568, 558,  534, 592,  534, 592,  536, 588,  566, 560,  512, 612,  566, 560,  566, 560,  556, 570,  536, 1662,  566, 560,  566, 560,  536, 588,  538, 588,  568, 560,  564, 560,  564, 562,  536, 590,  566, 560,  564, 562,  564, 560,  536, 590,  566, 560,  564, 560,  566, 560,  568, 558,  564, 560,  566, 560,  564, 560,  566, 560,  566, 560,  568, 558,  534, 592,  564, 560,  564, 560,  566, 560,  566, 558,  538, 588,  566, 560,  512, 614,  566, 560,  512, 614,  514, 610,  510, 616,  564, 560,  566, 558,  566, 560,  566, 560,  512, 614,  534, 592,  564, 560,  566, 560,  566, 1634,  564, 560,  566, 1634,  564, 1634,  536, 1662,  566, 1632,  536, 590,  566, 1632,  566, 560,  566, 1634,  566, 1634,  566};  // HAIER_AC_YRW02


#include <Arduino.h>
#include <assert.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>


#ifdef ARDUINO_ESP32C3_DEV
const uint16_t kRecvPin = 4;  // 14 on a ESP32-C3 causes a boot loop.
#else  // ARDUINO_ESP32C3_DEV
const uint16_t kRecvPin = 4;
#endif  // ARDUINO_ESP32C3_DEV


const uint32_t kBaudRate = 115200;

const uint16_t kCaptureBufferSize = 1024;

#if DECODE_AC

const uint8_t kTimeout = 50;
#else   // DECODE_AC
const uint8_t kTimeout = 15;
#endif  // DECODE_AC

const uint16_t kMinUnknownSize = 12;


const uint8_t kTolerancePercentage = kTolerance;  // kTolerance is normally 25%


#define LEGACY_TIMING_INFO false

IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
decode_results results;  // Somewhere to store the results


void setup() {
#if defined(ESP8266)
  Serial.begin(kBaudRate, SERIAL_8N1, SERIAL_TX_ONLY);
#else  // ESP8266
  Serial.begin(kBaudRate, SERIAL_8N1);
#endif  // ESP8266
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);
 
  assert(irutils::lowLevelSanityCheck() == 0);

  Serial.printf("\n" D_STR_IRRECVDUMP_STARTUP "\n", kRecvPin);
#if DECODE_HASH
  // Ignore messages with less than minimum on or off pulses.
  irrecv.setUnknownThreshold(kMinUnknownSize);
#endif  // DECODE_HASH
  irrecv.setTolerance(kTolerancePercentage);  // Override the default tolerance.
  irrecv.enableIRIn();  // Start the receiver
}

// The repeating section of the code
void loop() {
  // Check if the IR code has been received.
  if (irrecv.decode(&results)) {
    // Display a crude timestamp.
    uint32_t now = millis();
    Serial.printf(D_STR_TIMESTAMP " : %06u.%03u\n", now / 1000, now % 1000);
    // Check if we got an IR message that was to big for our capture buffer.
    if (results.overflow)
      Serial.printf(D_WARN_BUFFERFULL "\n", kCaptureBufferSize);
    // Display the library version the message was captured with.
    Serial.println(D_STR_LIBRARY "   : v" _IRREMOTEESP8266_VERSION_STR "\n");
    // Display the tolerance percentage if it has been change from the default.
    if (kTolerancePercentage != kTolerance)
      Serial.printf(D_STR_TOLERANCE " : %d%%\n", kTolerancePercentage);
    // Display the basic output of what we found.
    Serial.print(resultToHumanReadableBasic(&results));
    // Display any extra A/C info if we have it.
    String description = IRAcUtils::resultAcToString(&results);
    if (description.length()) Serial.println(D_STR_MESGDESC ": " + description);
    yield();  // Feed the WDT as the text output can take a while to print.
#if LEGACY_TIMING_INFO
    // Output legacy RAW timing info of the result.
    Serial.println(resultToTimingInfo(&results));
    yield();  // Feed the WDT (again)
#endif  // LEGACY_TIMING_INFO
    // Output the results as source code
    Serial.println(resultToSourceCode(&results));
    Serial.println();    // Blank line between entries
    yield();             // Feed the WDT (again)
  }
}
