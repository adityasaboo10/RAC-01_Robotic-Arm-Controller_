// Robotic arm
#include <Servo.h>

int Vrx, Vry, Sw = 0;
int Vrxpin = A0, Vrypin = A1, Swpin = 2;
int servoarmpin = 5, servoelbowpin = 6, servobasepin = 7, servoclawpin = 8;      // You must define pin numbers
int servoarmpos, servoelbowpos, servobasepos, servoclawpos;  // Start in neutral
int servoarm_meanpos = 90, servoelbow_meanpos = 0, servobase_meanpos = 90, servoclaw_meanpos = 90;
int rstpin = 3, rstvalue = 0;
int yellowpin = 10, greenpin = 11;
int speed = 15;  //speed ie delay
int anglespeed=5; //speed for angle increment or decrement 

Servo servobase, servoelbow, servoarm, servoclaw;

int toggle = 0;  // 0: elbowservo, 1: armservo

void read() {
  Vrx = analogRead(Vrxpin);
  Vry = analogRead(Vrypin);
  Sw = !digitalRead(Swpin);  // active low
  Serial.print("Vrx=");
  Serial.print(Vrx);
  Serial.print(", Vry=");
  Serial.print(Vry);
  Serial.print(", Sw=");
  Serial.println(Sw);
  delay(100);
}

void reset(Servo &servo, int &currentPos, int targetPos, int stepDelay) {
  digitalWrite(yellowpin, HIGH);
  digitalWrite(greenpin, LOW);
  if (currentPos == targetPos) return;

  int step = (targetPos > currentPos) ? 1 : -1;
  for (int pos = currentPos; pos != targetPos; pos += step) {
    servo.write(pos);
    delay(stepDelay);
  }

  servo.write(targetPos);
  currentPos = targetPos;
}

void move() {
  digitalWrite(greenpin, HIGH);
  digitalWrite(yellowpin, LOW);
  // Toggle control (button switch)
  if (Sw == 1) {
    toggle = 1 - toggle;
    delay(250);  // Debounce delay
  }

  bool moved = false;  // Track if any servo moved

  if (Vrx > 600 && servobasepos < 180) {
    servobasepos+=anglespeed;
    servobase.write(servobasepos);
    delay(speed);
    moved = true;
  } else if (Vrx < 400 && servobasepos > 0) {
    servobasepos-=anglespeed;
    servobase.write(servobasepos);
    delay(speed);
    moved = true;
  }

  if (toggle == 0) {
    if (Vry > 600 && servoelbowpos >0) {
      servoelbowpos-=anglespeed;
      servoelbow.write(servoelbowpos);
      delay(speed);
      moved = true;
    } else if (Vry < 400 && servoelbowpos <180) {
      servoelbowpos+=anglespeed;
      servoelbow.write(servoelbowpos);
      delay(speed);
      moved = true;
    }
  } else {
    if (Vry > 600 && servoarmpos < 180) {
      servoarmpos+=anglespeed;
      servoarm.write(servoarmpos);
      delay(speed);
      moved = true;
    } else if (Vry < 400 && servoarmpos > 0) {
      servoarmpos-=anglespeed;
      servoarm.write(servoarmpos);
      delay(speed);
      moved = true;
    }
  }

  // If no movement input, hold all current positions
  if (!moved) {
    servobase.write(servobasepos);
    servoelbow.write(servoelbowpos);
    servoarm.write(servoarmpos);
    servoclaw.write(servoclawpos);
  }
}


void setup() {
  Serial.begin(9600);
  pinMode(Vrxpin, INPUT);
  pinMode(Vrypin, INPUT);
  pinMode(Swpin, INPUT_PULLUP);
  pinMode(rstpin, INPUT);
  pinMode(yellowpin, OUTPUT);
  pinMode(greenpin, OUTPUT);

  servobase.attach(servobasepin);
  servoelbow.attach(servoelbowpin);
  servoarm.attach(servoarmpin);
  servoclaw.attach(servoclawpin);

  Serial.print("Initializing RAC");
  digitalWrite(greenpin, HIGH);
  digitalWrite(yellowpin, LOW);
  servobase.write(servobase_meanpos);
  servoelbow.write(servoelbow_meanpos);
  servoarm.write(servoarm_meanpos);
  servoclaw.write(servoclaw_meanpos);
  delay(1000);

  Serial.println("Initialization Complete");

servobasepos = servobase_meanpos;
servoelbowpos = servoelbow_meanpos;
servoarmpos = servoarm_meanpos;
servoclawpos = servoclaw_meanpos;
}

void loop() {
  rstvalue = digitalRead(rstpin);
  if (rstvalue == HIGH) {
    reset(servobase, servobasepos, servobase_meanpos, speed);
    reset(servoelbow, servoelbowpos, servoelbow_meanpos, speed);
    reset(servoarm, servoarmpos, servoarm_meanpos, speed);
    reset(servoclaw, servoclawpos, servoclaw_meanpos, speed);
  }
else{
  read();
  move();
}
}
