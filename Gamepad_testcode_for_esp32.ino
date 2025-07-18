// This code won't compile on the lastest version of "esp32 by Espressif Systems", to run click the dropdown arrow next to "Installed" on searching esp32 by Espressif Systems in IDE, select version 2.0.7 hit download, then run this code
#include <DabbleESP32.h>

void setup() {
  Serial.begin(9600);
  Dabble.begin("ESP32Test"); // Your Bluetooth name
}

void loop() {
  Dabble.processInput(); // Must call this to read latest data

  if (GamePad.isUpPressed()) {
    Serial.println("Up pressed");
  }
  if (GamePad.isDownPressed()) {
    Serial.println("Down pressed");
  }
  if (GamePad.isLeftPressed()) {
    Serial.println("Left pressed");
  }
  if (GamePad.isRightPressed()) {
    Serial.println("Right pressed");
  }

  delay(100); // Prevent flooding Serial
}
