int Vrx, Vry, Sw;

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(2, INPUT_PULLUP);  // Use internal pull-up resistor
}

void loop() {
  Vrx = analogRead(A0);
  Vry = analogRead(A1);
  Sw = digitalRead(2); // 0 = pressed, 1 = not pressed

  Serial.print("Vrx=");
  Serial.print(Vrx);
  Serial.print(", Vry=");
  Serial.print(Vry);
  Serial.print(", Sw=");
  Serial.println(Sw == 0 ? "PRESSED" : "RELEASED");

  delay(200);
}
