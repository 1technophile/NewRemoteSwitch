/*
* Demo for RF remote switch receiver.
* For details, see RemoteReceiver.h!
*
* This sketch shows the received signals on the serial port.
* Connect the receiver to digital pin 2.
*/

#include <NewRemoteReceiver.h>

void setup() {
  Serial.begin(9600);
  
  // Initialize receiver on interrupt 0 (= digital pin 2) for arduino uno, calls the callback "showCode"
  // after 1 identical codes have been received in a row. (thus, keep the button pressed
  // for a moment), on esp8266 use on interrupt 5 = digital pin 1
  //
  // See the interrupt-parameter of attachInterrupt for possible values (and pins)
  // to connect the receiver.
  NewRemoteReceiver::init(0, 1, showCode);
}

void loop() {
}

// Callback function is called only when a valid code is received.
void showCode(unsigned int period, unsigned long address) {
  // Note: interrupts are disabled. You can re-enable them if needed.
  
  // Print the received code.
  Serial.print("Code: ");
  Serial.print(address);
  Serial.print(" Period: ");
  Serial.println(period);
  /*Serial.print(", period duration: ");
  Serial.print(period);
  Serial.println("us.");*/
}


