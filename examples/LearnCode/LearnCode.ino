/*
 * Demo for RF remote switch receiver. 
 * This example is for the new KaKu / Home Easy type of remotes!
 *
 * For details, see NewRemoteReceiver.h!
 *
 * With this sketch you can control a LED connected to LED_BUILTIN pin,
 * after the sketch learned the code. After start, the LED starts to blink,
 * until a valid code has been received. The led stops blinking. Now you
 * can control the LED with the remote.
 *
 * Note: only unit-switches are supported in this sketch, no group or dim.
 *
 * Set-up: connect the receiver to an attachable interruption pin. See below.
 */

#include <NewRemoteReceiver.h>

boolean codeLearned = false;
unsigned long learnedAddress;
byte learnedUnit;

#if !defined LED_BUILTIN
  #if defined ESP8266 || defined ESP32
    #define LED_BUILTIN 2
  #else
    #define LED_BUILTIN 13
  #endif
  #define VALUE_TO_STRING(x) #x
  #define VALUE(x) VALUE_TO_STRING(x)
  #pragma message("SETTING LED_BUILTIN TO PIN " VALUE(LED_BUILTIN))
#endif

void setup() {
  // LED-pin as output
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize receiver on interrupt 0 (= digital pin 2) for Arduino Uno.
  // On ESP8266 and ESP32 use on GPIO 5 = digital pin depends board.
  // Review file "pins_arduino.h" of your variant:
  //   https://github.com/esp8266/Arduino/tree/master/variants
  //   https://github.com/espressif/arduino-esp32/tree/master/variants
  //
  // See the interrupt-parameter of attachInterrupt for possible values (and pins)
  // to connect the receiver.
  //
  // Calls the callback "processCode" after 2 identical codes have been received
  // in a row. (thus, keep the button pressed for a moment).

    #if defined ESP8266 || defined ESP32
      NewRemoteReceiver::init(5, 2, processCode);
    #else
      NewRemoteReceiver::init(0, 2, processCode);
    #endif
}

void loop() {
  // Blink led until a code has been learned
  if (!codeLearned) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }
}

// Callback function is called only when a valid code is received.
void processCode(NewRemoteCode receivedCode) {
  // A code has been received.
  // Do we already know the code?
  if (!codeLearned) {
    // No! Let's learn the received code.
    learnedAddress = receivedCode.address;
    learnedUnit = receivedCode.unit;
    codeLearned = true;    
  } else {
    // Yes!    
    // Is the received code identical to the learned code?
    if (receivedCode.address == learnedAddress && receivedCode.unit == learnedUnit) {
      // Yes!
      // Switch the LED off if the received code was "off".
      // Anything else (on, dim, on_with_dim) will switch the LED on.
      if (receivedCode.switchType == NewRemoteCode::off) {
        digitalWrite(LED_BUILTIN, LOW);
      } else {
        digitalWrite(LED_BUILTIN, HIGH);
      }
    }
  }
}
