#include <Servo.h>

// Pin definitions
#define PIN_CLAW_RIGHT  2
#define PIN_CLAW_BACK   3
#define PIN_CLAW_LEFT   4
#define PIN_CLAW_FRONT  5

#define PIN_ARM_RIGHT   6
#define PIN_ARM_BACK    7
#define PIN_ARM_LEFT    8
#define PIN_ARM_FRONT   9

// Servo instances
Servo clawRight, clawBack, clawLeft, clawFront;
Servo armRight, armBack, armLeft, armFront;

// Calibration settings - ADJUST THESE
int CLAW_HOME = 0;
int CLAW_ROTATED = 100;
int ARM_HOLDING = 0;
int ARM_RELEASED = 100;

// Mode: 0=Claws, 1=Arms
int mode = 0;

// Which servo to test (1=Right, 2=Back, 3=Left, 4=Front, 5=All)
int selected = 5;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Attach all servos
  clawRight.attach(PIN_CLAW_RIGHT);
  clawBack.attach(PIN_CLAW_BACK);
  clawLeft.attach(PIN_CLAW_LEFT);
  clawFront.attach(PIN_CLAW_FRONT);
  
  armRight.attach(PIN_ARM_RIGHT);
  armBack.attach(PIN_ARM_BACK);
  armLeft.attach(PIN_ARM_LEFT);
  armFront.attach(PIN_ARM_FRONT);
  
  // Reset to safe positions
  clawRight.write(CLAW_HOME);
  clawBack.write(CLAW_HOME);
  clawLeft.write(CLAW_HOME);
  clawFront.write(CLAW_HOME);
  
  armRight.write(ARM_RELEASED);
  armBack.write(ARM_RELEASED);
  armLeft.write(ARM_RELEASED);
  armFront.write(ARM_RELEASED);
  
  delay(1000);
  
  printMenu();
}

void printMenu() {
  Serial.println(F("\n=============================="));
  Serial.println(F("  CALIBRATION TOOL"));
  Serial.println(F("=============================="));
  Serial.println();
  Serial.println(F("MODE SELECTION:"));
  Serial.println(F("  c = CLAW mode"));
  Serial.println(F("  m = ARM mode"));
  Serial.println();
  Serial.println(F("SELECT SERVO:"));
  Serial.println(F("  r = Right only"));
  Serial.println(F("  b = Back only"));
  Serial.println(F("  l = Left only"));
  Serial.println(F("  f = Front only"));
  Serial.println(F("  a = ALL together"));
  Serial.println();
  Serial.println(F("MOVEMENT:"));
  Serial.println(F("  1 = Position 1 (HOME/HOLDING)"));
  Serial.println(F("  2 = Position 2 (ROTATED/RELEASED)"));
  Serial.println();
  Serial.println(F("ADJUST:"));
  Serial.println(F("  3 = Increase Position 1 by 5"));
  Serial.println(F("  4 = Decrease Position 1 by 5"));
  Serial.println(F("  5 = Increase Position 2 by 5"));
  Serial.println(F("  6 = Decrease Position 2 by 5"));
  Serial.println();
  Serial.println(F("INFO:"));
  Serial.println(F("  s = Show settings"));
  Serial.println(F("  h = Show this menu"));
  Serial.println(F("=============================="));
  showSettings();
}

void showSettings() {
  Serial.println();
  Serial.println(F("=== CURRENT SETTINGS ==="));
  
  if (mode == 0) {
    Serial.println(F("MODE: CLAWS"));
    Serial.print(F("  HOME: "));
    Serial.println(CLAW_HOME);
    Serial.print(F("  ROTATED: "));
    Serial.println(CLAW_ROTATED);
  } else {
    Serial.println(F("MODE: ARMS"));
    Serial.print(F("  HOLDING: "));
    Serial.println(ARM_HOLDING);
    Serial.print(F("  RELEASED: "));
    Serial.println(ARM_RELEASED);
  }
  
  Serial.print(F("TESTING: "));
  switch(selected) {
    case 1: Serial.println(F("Right")); break;
    case 2: Serial.println(F("Back")); break;
    case 3: Serial.println(F("Left")); break;
    case 4: Serial.println(F("Front")); break;
    case 5: Serial.println(F("ALL")); break;
  }
  Serial.println(F("========================"));
  Serial.println();
}

void moveClaws(int angle) {
  Serial.print(F("CLAWS -> "));
  Serial.print(angle);
  Serial.print(F(" deg - "));
  
  switch(selected) {
    case 1:
      clawRight.write(angle);
      Serial.println(F("Right"));
      break;
    case 2:
      clawBack.write(angle);
      Serial.println(F("Back"));
      break;
    case 3:
      clawLeft.write(angle);
      Serial.println(F("Left"));
      break;
    case 4:
      clawFront.write(angle);
      Serial.println(F("Front"));
      break;
    case 5:
      clawRight.write(angle);
      clawBack.write(angle);
      clawLeft.write(angle);
      clawFront.write(angle);
      Serial.println(F("ALL"));
      break;
  }
}

void moveArms(int angle) {
  Serial.print(F("ARMS -> "));
  Serial.print(angle);
  Serial.print(F(" deg - "));
  
  switch(selected) {
    case 1:
      armRight.write(angle);
      Serial.println(F("Right"));
      break;
    case 2:
      armBack.write(angle);
      Serial.println(F("Back"));
      break;
    case 3:
      armLeft.write(angle);
      Serial.println(F("Left"));
      break;
    case 4:
      armFront.write(angle);
      Serial.println(F("Front"));
      break;
    case 5:
      armRight.write(angle);
      armBack.write(angle);
      armLeft.write(angle);
      armFront.write(angle);
      Serial.println(F("ALL"));
      break;
  }
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    
    // Clear buffer
    while(Serial.available() > 0) {
      Serial.read();
    }
    
    switch(cmd) {
      // MODE SELECTION
      case 'c':
      case 'C':
        mode = 0;
        Serial.println(F("\n>>> MODE: CLAWS"));
        showSettings();
        break;
        
      case 'm':
      case 'M':
        mode = 1;
        Serial.println(F("\n>>> MODE: ARMS"));
        showSettings();
        break;
      
      // SERVO SELECTION
      case 'r':
      case 'R':
        selected = 1;
        Serial.println(F("\n>>> Selected: RIGHT"));
        break;
        
      case 'b':
      case 'B':
        selected = 2;
        Serial.println(F("\n>>> Selected: BACK"));
        break;
        
      case 'l':
      case 'L':
        selected = 3;
        Serial.println(F("\n>>> Selected: LEFT"));
        break;
        
      case 'f':
      case 'F':
        selected = 4;
        Serial.println(F("\n>>> Selected: FRONT"));
        break;
        
      case 'a':
      case 'A':
        selected = 5;
        Serial.println(F("\n>>> Selected: ALL"));
        break;
      
      // MOVEMENT
      case '1':
        Serial.println(F("\n>>> Position 1"));
        if (mode == 0) {
          moveClaws(CLAW_HOME);
        } else {
          moveArms(ARM_HOLDING);
        }
        break;
        
      case '2':
        Serial.println(F("\n>>> Position 2"));
        if (mode == 0) {
          moveClaws(CLAW_ROTATED);
        } else {
          moveArms(ARM_RELEASED);
        }
        break;
      
      // ADJUST POSITION 1
      case '3':
        if (mode == 0) {
          CLAW_HOME += 5;
          if (CLAW_HOME > 180) CLAW_HOME = 180;
          Serial.print(F("\n>>> CLAW_HOME = "));
          Serial.println(CLAW_HOME);
        } else {
          ARM_HOLDING += 5;
          if (ARM_HOLDING > 180) ARM_HOLDING = 180;
          Serial.print(F("\n>>> ARM_HOLDING = "));
          Serial.println(ARM_HOLDING);
        }
        Serial.println(F("Press 1 to test"));
        break;
        
      case '4':
        if (mode == 0) {
          CLAW_HOME -= 5;
          if (CLAW_HOME < 0) CLAW_HOME = 0;
          Serial.print(F("\n>>> CLAW_HOME = "));
          Serial.println(CLAW_HOME);
        } else {
          ARM_HOLDING -= 5;
          if (ARM_HOLDING < 0) ARM_HOLDING = 0;
          Serial.print(F("\n>>> ARM_HOLDING = "));
          Serial.println(ARM_HOLDING);
        }
        Serial.println(F("Press 1 to test"));
        break;
      
      // ADJUST POSITION 2
      case '5':
        if (mode == 0) {
          CLAW_ROTATED += 5;
          if (CLAW_ROTATED > 180) CLAW_ROTATED = 180;
          Serial.print(F("\n>>> CLAW_ROTATED = "));
          Serial.println(CLAW_ROTATED);
        } else {
          ARM_RELEASED += 5;
          if (ARM_RELEASED > 180) ARM_RELEASED = 180;
          Serial.print(F("\n>>> ARM_RELEASED = "));
          Serial.println(ARM_RELEASED);
        }
        Serial.println(F("Press 2 to test"));
        break;
        
      case '6':
        if (mode == 0) {
          CLAW_ROTATED -= 5;
          if (CLAW_ROTATED < 0) CLAW_ROTATED = 0;
          Serial.print(F("\n>>> CLAW_ROTATED = "));
          Serial.println(CLAW_ROTATED);
        } else {
          ARM_RELEASED -= 5;
          if (ARM_RELEASED < 0) ARM_RELEASED = 0;
          Serial.print(F("\n>>> ARM_RELEASED = "));
          Serial.println(ARM_RELEASED);
        }
        Serial.println(F("Press 2 to test"));
        break;
      
      // INFO
      case 's':
      case 'S':
        showSettings();
        break;
        
      case 'h':
      case 'H':
        printMenu();
        break;
        
      case '\n':
      case '\r':
        break;
        
      default:
        Serial.print(F("\n>>> Unknown: "));
        Serial.println(cmd);
        Serial.println(F("Press 'h' for help"));
        break;
    }
  }
}
