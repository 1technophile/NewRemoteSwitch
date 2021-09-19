/*
 * Demo for RF remote switch receiver.
 * For details, see NewRemoteReceiver.h!
 *
 * Connect the receiver to an attachable interruption pin. See below.
 * Connect the transmitter to digital pin 11 on Arduino or pin 4 on ESP8266 and ESP32.
 * 
 * To prevent kernel panic for interrupt wdt timeout on ESP8266 and ESP32, 
 * the retransmission of the received code is done in loop() function.
 *
 * When run, this sketch waits for a valid code from a new-style the receiver,
 * decodes it, and retransmits it after 5 seconds.
 */

#include <NewRemoteReceiver.h>
#include <NewRemoteTransmitter.h>


NewRemoteCode receivedCode;   // Global var to store received code for retramitt it
boolean codeLearned = false;  // Flag to set a valid learned code has been received

void setup() {
  // See example ShowReceivedCode for info on this
  #if defined ESP8266 || defined ESP32
    NewRemoteReceiver::init(5, 2, retransmitter);
  #else
    NewRemoteReceiver::init(0, 2, retransmitter);
  #endif
}

void loop() {

  if (codeLearned) {

    // Clear flag
    codeLearned = false;

    // Wait 5 seconds before sending.
    delay(5000);

    // Create a new transmitter with the received address and period, use a digital pin as output pin
    #if defined ESP8266 || defined ESP32
    NewRemoteTransmitter transmitter(receivedCode.address, 4, receivedCode.period);
    #else
    NewRemoteTransmitter transmitter(receivedCode.address, 11, receivedCode.period);
    #endif

    if (receivedCode.switchType == NewRemoteCode::dim || 
      (receivedCode.switchType == NewRemoteCode::on && receivedCode.dimLevelPresent)) {
      // Dimmer signal received

      if (receivedCode.groupBit) {
        transmitter.sendGroupDim(receivedCode.dimLevel);
      } 
      else {
        transmitter.sendDim(receivedCode.unit, receivedCode.dimLevel);
      }
    } 
    else {
      // On/Off signal received
      bool isOn = receivedCode.switchType == NewRemoteCode::on;

      if (receivedCode.groupBit) {
        // Send to the group
        transmitter.sendGroup(isOn);
      } 
      else {
        // Send to a single unit
        transmitter.sendUnit(receivedCode.unit, isOn);
      }
    }

    NewRemoteReceiver::enable();
  }
}

void retransmitter(NewRemoteCode retransmitterCode) {  

  // Disable the receiver; otherwise it might pick up the retransmit as well.
  NewRemoteReceiver::disable();

  // Store received code for retramitt it in loop()
  receivedCode = retransmitterCode;
  // Set valid learned code has been received
  codeLearned = true;

}

