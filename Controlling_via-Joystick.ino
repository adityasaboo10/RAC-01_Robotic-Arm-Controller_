// ----------------------------
// Robotic Arm with Joystick Control and Mode Switching
// Modes: 
//   0 → Elbow Control    (Single click-toggle to get elbow control from arm)
//   1 → Arm Control     (Vice-versa as above)
//   2 → Base Control (Double Click)
//   3 → Reset Position (Long Press)
// ----------------------------

#include <Servo.h>

// Analog joystick input
int Vrx, Vry, Sw = 0;
int Vrxpin = A0, Vrypin = A1, Swpin = 2;

// Servo motor pins
int servoarmpin = 7, servoelbowpin = 6, servobasepin = 5, servoclawpin = 8;

// Servo position variables
int servoarmpos = 0, servoelbowpos = 0, servobasepos = 0, servoclawpos = 0;
int servoarm_meanpos = 180, servoelbow_meanpos = 50, servobase_meanpos = 90, servoclaw_meanpos = 0;

// LED indicator pins
int led1=9, led2=10, led3=11, led4=12;   

// Speed settings
int speed = 15;        // Delay between movements
int anglespeed = 5;    // Step size for angle increment/decrement

// State variable
int state = 0;         // 0:elbow, 1:arm, 2:base, 3:reset

// Button click tracking
unsigned long lastClickTime = 0;
unsigned long pressStartTime = 0;
int clickCount = 0;
bool isPressed = false;
bool wasPressed = false;

// Servo objects
Servo servobase, servoelbow, servoarm, servoclaw;

// ----------------------------
// Read joystick values
// ----------------------------
void read() {
  Vrx = analogRead(Vrxpin);
  Vry = analogRead(Vrypin);
  Serial.print("Vrx=");
  Serial.print(Vrx);
  Serial.print(", Vry=");
  Serial.print(Vry);
  delay(100);
}

// ----------------------------
// Reset servo to mean position
// ----------------------------
void reset(Servo &servo, int &currentPos, int targetPos, int stepDelay) {
 blink();
  if (currentPos == targetPos) return;

  int step = (targetPos > currentPos) ? 1 : -1;
  for (int pos = currentPos; pos != targetPos; pos += step) {
    servo.write(pos);
    delay(stepDelay);
  }

  servo.write(targetPos);
  currentPos = targetPos;
}

// ----------------------------
// Handle button click logic for mode switching
// ----------------------------
void switcher() {
  Sw=!digitalRead(Swpin);
  Serial.print(", Sw=");
  Serial.println(Sw);


  // Update press state (active low)
  if (Sw == 1) {   //active reading of this
    isPressed = true;
  } else if (Sw == 0) {
    isPressed = false;
  }

  unsigned long currentTime = millis();
  unsigned long pressDuration = 0;

  // Detect press start
  if (isPressed && !wasPressed) {       //only works for the first time when Sw=1(isPressed=true) as then waspressed will also become true, so intial time would only change after the reset
    pressStartTime = currentTime;
  }

  // Detect release
  if (!isPressed && wasPressed) {    //this is the condition of just release
    pressDuration = currentTime - pressStartTime;          //duration of the click stores at the moment of just release

    if (pressDuration > 800) {                    
      state = 4;  // Long press → reset
    } else {            
      clickCount++;
      lastClickTime = currentTime;        //the time at which button was just released ie one click is recorded
    }
  }

  // Determine click pattern (single/double)
  if (!isPressed && clickCount > 0 && (currentTime - lastClickTime > 300)) {
    if (clickCount == 1) {
  if (state == 0) {
    state = 1;
  } else {
    state = 0;  // Force it back to 0 from any other state
  }
}
 else if (clickCount == 2) {
      state = 2;  // claw control
    }
    else if (clickCount == 3 ){
      state=3; //base control
    }
    clickCount = 0;
  }

  wasPressed = isPressed;
  Serial.print("State=");
  Serial.println(state);
}

// ----------------------------
// Move servos based on current state
// ----------------------------
void move() {
 
  bool moved = false;
if (state == 3){
  digitalWrite(led1,HIGH);
  digitalWrite(led2,HIGH);
  digitalWrite(led3,HIGH);
  digitalWrite(led4,HIGH);

  if (Vrx < 400 && servobasepos < 180) {    //base
      servobasepos += anglespeed;
      servobase.write(servobasepos);
      delay(speed);
      moved = true;
    } else if (Vrx > 600 && servobasepos > 0) { 
      servobasepos -= anglespeed;
      servobase.write(servobasepos);
      delay(speed);
      moved = true;
    }
}
  if (state == 0) {  // Elbow
  digitalWrite(led1,HIGH);
  digitalWrite(led2,LOW);
  digitalWrite(led3,LOW);
  digitalWrite(led4,LOW);
  
    if (Vry > 600 && servoelbowpos > 0) {
      servoelbowpos -= anglespeed;
      servoelbow.write(servoelbowpos);
      delay(speed);
      moved = true;
    } else if (Vry < 400 && servoelbowpos < 180) {
      servoelbowpos += anglespeed;
      servoelbow.write(servoelbowpos);
      delay(speed);
      moved = true;
    }
  } 
  else if (state == 1) {  // Arm
  digitalWrite(led1,HIGH);
  digitalWrite(led2,HIGH);
  digitalWrite(led3,LOW);
  digitalWrite(led4,LOW);
  
    if (Vry > 600 && servoarmpos < 180) {
      servoarmpos += anglespeed;
      servoarm.write(servoarmpos);
      delay(speed);
      moved = true;
    } else if (Vry < 400 && servoarmpos > 0) {
      servoarmpos -= anglespeed;
      servoarm.write(servoarmpos);
      delay(speed);
      moved = true;
    }
  } 
  else if (state == 2) {  // Claw
  digitalWrite(led1,HIGH);
  digitalWrite(led2,HIGH);
  digitalWrite(led3,HIGH);
  digitalWrite(led4,LOW);
    if (Vry <400 && servoclawpos < 180) {
      servoclawpos += anglespeed;
      servoclaw.write(servoclawpos);
      delay(speed);
      moved = true;
    } else if (Vry > 600 && servoclawpos > 0) {
      servoclawpos -= anglespeed;
      servoclaw.write(servoclawpos);
      delay(speed);
      moved = true;
    }
  } 
  else if (state == 4) {  // Reset
    reset(servobase, servobasepos, servobase_meanpos, speed);
    reset(servoarm, servoarmpos, servoarm_meanpos, speed);
    reset(servoelbow, servoelbowpos, servoelbow_meanpos, speed);
    reset(servoclaw, servoclawpos, servoclaw_meanpos, speed);
    moved=true;
  }

  // Hold position if no movement
  if (!moved) {
    servobase.write(servobasepos);
    servoelbow.write(servoelbowpos);
    servoarm.write(servoarmpos);
    servoclaw.write(servoclawpos);
  }
}
void blink(){
  for(int l=0; l<2; l++){
  digitalWrite(led1,HIGH);
  digitalWrite(led2,HIGH);
  digitalWrite(led3,HIGH);
  digitalWrite(led4,HIGH);
  delay(10);     
   digitalWrite(led1,LOW);
  digitalWrite(led2,LOW);
  digitalWrite(led3,LOW);
  digitalWrite(led4,LOW);           
  }
}

// ----------------------------
// Arduino setup
// ----------------------------
void setup() {
  Serial.begin(9600);

  // Pin setup
  pinMode(Vrxpin, INPUT);
  pinMode(Vrypin, INPUT);
  pinMode(Swpin, INPUT_PULLUP);
  pinMode(rstpin, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  // Attach servos
  servobase.attach(servobasepin);
  servoelbow.attach(servoelbowpin);
  servoarm.attach(servoarmpin);
  servoclaw.attach(servoclawpin);

  // Startup status
  Serial.print("Initializing RAC");
  //blink to indicate
 blink();
  // Move to neutral positions
  servobase.write(servobase_meanpos);
  servoelbow.write(servoelbow_meanpos);
  servoarm.write(servoarm_meanpos);
  servoclaw.write(servoclaw_meanpos);
  delay(1000);

  Serial.println("Initialization Complete");



  // Initialize current positions
  servobasepos = servobase_meanpos;
  servoelbowpos = servoelbow_meanpos;
  servoarmpos = servoarm_meanpos;
  servoclawpos = servoclaw_meanpos;
}

// ----------------------------
// Arduino main loop
// ----------------------------
void loop() {
  read();        // Read joystick and button
  switcher();    // Handle click logic
  move();    
  }    // Move based on current state
