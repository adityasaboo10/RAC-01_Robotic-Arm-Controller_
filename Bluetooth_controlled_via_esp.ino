// ----------------------------
// Robotic Arm with Joystick Control and Mode Switching
// Modes: 
//   0 → Elbow Control    (Single click-toggle to get elbow control from arm)
//   1 → Arm Control     (Vice-versa as above)
//   2 → Claw Control (Double Click)
//   3 → Base Control (Triple Click)
//   4 → Reset Position (Long Press)
// ----------------------------

#include <ESP32Servo.h>
#include <DabbleESP32.h>

// variables for input from gamepad
int up, down, right, left, X;

// Servo motor pins
int servoarmpin = 14, servoelbowpin = 27, servobasepin = 26, servoclawpin = 25;

// Servo position variables
int servoarmpos = 0, servoelbowpos = 0, servobasepos = 0, servoclawpos = 0;
int servoarm_meanpos = 180, servoelbow_meanpos = 50, servobase_meanpos = 90, servoclaw_meanpos = 0;

// LED indicator pins
int led1 = 32, led2 = 33, led3 = 15, led4 = 4;


// Speed settings
int speed = 15;        // Delay between movements
int anglespeed = 5;    // Step size for angle increment/decrement

// State variable
int state = 0;         // 0:elbow, 1:arm, 2:claw, 3:base, 4:reset

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
  up = GamePad.isUpPressed();
  down = GamePad.isDownPressed();
  right = GamePad.isRightPressed(); 
  left = GamePad.isLeftPressed();
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
  X = GamePad.isCrossPressed();
  Serial.print(", X=");
  Serial.println(X);


  // Update press state (active low)
  if (X == 1) {   //active reading of this
    isPressed = true;
  } else if (X == 0) {
    isPressed = false;
  }

  unsigned long currentTime = millis();
  unsigned long pressDuration = 0;

  // Detect press start
  if (isPressed && !wasPressed) {       //only works for the first time when X=1(isPressed=true) as then waspressed will also become true, so intial time would only change after the reset
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

  if ( left == 1 && servobasepos < 180) {    //base
      servobasepos += anglespeed;
      servobase.write(servobasepos);
      delay(speed);
      moved = true;
    } else if (right == 1 && servobasepos > 0) { 
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
  
    if (down == 1 && servoelbowpos > 0) {
      servoelbowpos -= anglespeed;
      servoelbow.write(servoelbowpos);
      delay(speed);
      moved = true;
    } else if (up == 1 && servoelbowpos < 180) {
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
  
    if (down == 1&& servoarmpos < 180) {
      servoarmpos += anglespeed;
      servoarm.write(servoarmpos);
      delay(speed);
      moved = true;
    } else if (up == 1 && servoarmpos > 0) {
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
    if (up == 1 && servoclawpos < 180) {
      servoclawpos += anglespeed;
      servoclaw.write(servoclawpos);
      delay(speed);
      moved = true;
    } else if (down == 1 && servoclawpos > 0) {
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
  Dabble.begin("RAC-BV"); // Bluetooth name

  // Pin setup
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
  Dabble.processInput();
  read();        // Read joystick and button
  switcher();    // Handle click logic
  move();    
  //delay(100) or delay(50) 

  }    // Move based on current state
