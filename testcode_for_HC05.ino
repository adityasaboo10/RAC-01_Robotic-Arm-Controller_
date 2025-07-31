//This will work only on android
//connect the module's RXD TO 11, TXD TO 10, VCC to 5V, Gnd to Gnd
//donwload serial bluetooth terminal from playstore, then connect the HC-05 from device settings (pair code will be either 1234 or 0000).This will pair the HC-05 to the phone but it is still not connected
//now open the terminal app, go to the paired device option, and select HC-05
//try sending some message, it should appear on serial monitor

#include <SoftwareSerial.h>
SoftwareSerial Bluetooth(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);
  Bluetooth.begin(9600);
  Serial.println("Echo test started");
}

void loop() {
  if (Bluetooth.available()) {
    char c = Bluetooth.read();
    Serial.write(c);
  }

  if (Serial.available()) {
    Bluetooth.write(Serial.read());
  }
}
