/*
* Demo for RF remote switch receiver.
* For details, see RemoteReceiver.h!
*
* This sketch shows the received signals on the serial port.
* Connect the receiver to an attachable interruption pin. See below.
* Detected codes example:
 Code: 8233372 Period: 273
 unit: 1
 groupBit: 0
 switchType: 0
 dimLevel: 3
*/

#include <NewRemoteReceiver.h>

void setup() {
  Serial.begin(115200);

  // Initialize receiver on interrupt 0 (= digital pin 2) for Arduino Uno. 
  // On ESP8266 and ESP32 use on GPIO 5 = digital pin depends board. 
  // Review file "pins_arduino.h" of your variant:
  //   https://github.com/esp8266/Arduino/tree/master/variants
  //   https://github.com/espressif/arduino-esp32/tree/master/variants
  //
  // See the interrupt-parameter of attachInterrupt for possible values (and pins)
  // to connect the receiver.
  //
  // Calls the callback "showCode" after 2 identical codes have been received 
  // in a row. (thus, keep the button pressed for a moment).

  // if you don't see codes try to reset your board after upload
  
    #if defined ESP8266 || defined ESP32
      NewRemoteReceiver::init(5, 2, showCode);
    #else
      NewRemoteReceiver::init(0, 2, showCode);
    #endif
    Serial.println("Receiver initialized");    
}

void loop() {

}

// Callback function is called only when a valid code is received.
void showCode(unsigned int period, unsigned long address, unsigned long groupBit, unsigned long unit, unsigned long switchType, boolean dimLevelPresent, byte dimLevel) {

  // Print the received code.
  Serial.print("Code: ");
  Serial.print(address);
  Serial.print(" Period: ");
  Serial.println(period);
  Serial.print(" unit: ");
  Serial.println(unit);
  Serial.print(" groupBit: ");
  Serial.println(groupBit);
  Serial.print(" switchType: ");
  Serial.println(switchType);

  if (dimLevelPresent){
    Serial.print(" dimLevel: ");
    Serial.println(dimLevel);    
  }

}
