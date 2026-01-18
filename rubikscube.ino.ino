#include <Servo.h>

#define DELAY_BETWEEN_STEPS (150)
#define DELAY_BETWEEN_STEPS2 (500)

// ========================================
// PIN DEFINITIONS
// ========================================

// Claw motors (Rotaters) - Pins 2, 3, 4, 5
#define PIN_CLAW_RIGHT  2
#define PIN_CLAW_BACK   3
#define PIN_CLAW_LEFT   4
#define PIN_CLAW_FRONT  5

// Arm motors (Pushers) - Pins 6, 7, 8, 9
#define PIN_ARM_RIGHT   6
#define PIN_ARM_BACK    7
#define PIN_ARM_LEFT    8
#define PIN_ARM_FRONT   9

// ========================================
// SERVO ANGLE CONSTANTS
// ========================================

// Claw angles
#define CLAW_HOME      0
#define CLAW_ROTATED   105

// Arm angles
#define ARM_HOLDING    0
#define ARM_RELEASED   100

// ========================================
// SERVO INSTANCES
// ========================================

Servo clawRight;
Servo clawBack;
Servo clawLeft;
Servo clawFront;

Servo armRight;
Servo armBack;
Servo armLeft;
Servo armFront;

// ========================================
// SERVO CONTROL FUNCTIONS - CLOCKWISE
// ========================================

void ServosFace_RightCW() {
  clawRight.write(CLAW_ROTATED);        // Rotate 105° CW
  delay(DELAY_BETWEEN_STEPS2);
  armRight.write(ARM_RELEASED);         // Release arm
  delay(DELAY_BETWEEN_STEPS);
  clawRight.write(CLAW_HOME);           // Reset to 0°
  delay(DELAY_BETWEEN_STEPS);
  armRight.write(ARM_HOLDING);          // Grab cube
  delay(DELAY_BETWEEN_STEPS);
}

void ServosFace_LeftCW() {
  clawLeft.write(CLAW_ROTATED);
  delay(DELAY_BETWEEN_STEPS2);
  armLeft.write(ARM_RELEASED);
  delay(DELAY_BETWEEN_STEPS);
  clawLeft.write(CLAW_HOME);
  delay(DELAY_BETWEEN_STEPS);
  armLeft.write(ARM_HOLDING);
  delay(DELAY_BETWEEN_STEPS);
}

void ServosFace_FrontCW() {
  clawFront.write(CLAW_ROTATED);
  delay(DELAY_BETWEEN_STEPS2);
  armFront.write(ARM_RELEASED);
  delay(DELAY_BETWEEN_STEPS);
  clawFront.write(CLAW_HOME);
  delay(DELAY_BETWEEN_STEPS);
  armFront.write(ARM_HOLDING);
  delay(DELAY_BETWEEN_STEPS);
}

void ServosFace_BackCW() {
  clawBack.write(CLAW_ROTATED);
  delay(DELAY_BETWEEN_STEPS2);
  armBack.write(ARM_RELEASED);
  delay(DELAY_BETWEEN_STEPS);
  clawBack.write(CLAW_HOME);
  delay(DELAY_BETWEEN_STEPS);
  armBack.write(ARM_HOLDING);
  delay(DELAY_BETWEEN_STEPS);
}

// ========================================
// SERVO CONTROL FUNCTIONS - COUNTER-CLOCKWISE
// ========================================

void ServosFace_RightCCW() {
  // For CCW: First go to rotated position (loads the mechanism)
  clawRight.write(CLAW_ROTATED);        // Go to 105°
  delay(DELAY_BETWEEN_STEPS2);
  armRight.write(ARM_RELEASED);         // Release arm
  delay(DELAY_BETWEEN_STEPS);
  // Now return creates CCW motion
  clawRight.write(CLAW_HOME);           // Return to 0° (CCW motion)
  delay(DELAY_BETWEEN_STEPS);
  // Reset by going forward again
  clawRight.write(CLAW_ROTATED);        // Go to 105° again
  delay(DELAY_BETWEEN_STEPS);
  clawRight.write(CLAW_HOME);           // Back to home
  delay(DELAY_BETWEEN_STEPS);
  armRight.write(ARM_HOLDING);          // Grab cube
  delay(DELAY_BETWEEN_STEPS);
}

void ServosFace_LeftCCW() {
  clawLeft.write(CLAW_ROTATED);
  delay(DELAY_BETWEEN_STEPS2);
  armLeft.write(ARM_RELEASED);
  delay(DELAY_BETWEEN_STEPS);
  clawLeft.write(CLAW_HOME);
  delay(DELAY_BETWEEN_STEPS);
  clawLeft.write(CLAW_ROTATED);
  delay(DELAY_BETWEEN_STEPS);
  clawLeft.write(CLAW_HOME);
  delay(DELAY_BETWEEN_STEPS);
  armLeft.write(ARM_HOLDING);
  delay(DELAY_BETWEEN_STEPS);
}

void ServosFace_FrontCCW() {
  clawFront.write(CLAW_ROTATED);
  delay(DELAY_BETWEEN_STEPS2);
  armFront.write(ARM_RELEASED);
  delay(DELAY_BETWEEN_STEPS);
  clawFront.write(CLAW_HOME);
  delay(DELAY_BETWEEN_STEPS);
  clawFront.write(CLAW_ROTATED);
  delay(DELAY_BETWEEN_STEPS);
  clawFront.write(CLAW_HOME);
  delay(DELAY_BETWEEN_STEPS);
  armFront.write(ARM_HOLDING);
  delay(DELAY_BETWEEN_STEPS);
}

void ServosFace_BackCCW() {
  clawBack.write(CLAW_ROTATED);
  delay(DELAY_BETWEEN_STEPS2);
  armBack.write(ARM_RELEASED);
  delay(DELAY_BETWEEN_STEPS);
  clawBack.write(CLAW_HOME);
  delay(DELAY_BETWEEN_STEPS);
  clawBack.write(CLAW_ROTATED);
  delay(DELAY_BETWEEN_STEPS);
  clawBack.write(CLAW_HOME);
  delay(DELAY_BETWEEN_STEPS);
  armBack.write(ARM_HOLDING);
  delay(DELAY_BETWEEN_STEPS);
}

// ========================================
// CUBE ROTATION FUNCTIONS
// ========================================

void ServosCube_MoveX() {
  // Tilt cube forward: Up becomes Front
  armFront.write(ARM_RELEASED);
  armBack.write(ARM_RELEASED);
  delay(DELAY_BETWEEN_STEPS);
  
  clawRight.write(CLAW_ROTATED);
  clawLeft.write(CLAW_HOME);
  delay(DELAY_BETWEEN_STEPS2);
  
  armFront.write(ARM_HOLDING);
  armBack.write(ARM_HOLDING);
  delay(DELAY_BETWEEN_STEPS);
  
  armRight.write(ARM_RELEASED);
  armLeft.write(ARM_RELEASED);
  delay(DELAY_BETWEEN_STEPS);
  
  clawRight.write(CLAW_HOME);
  clawLeft.write(CLAW_ROTATED);
  delay(DELAY_BETWEEN_STEPS);
  
  clawLeft.write(CLAW_HOME);
  delay(DELAY_BETWEEN_STEPS);
  
  armRight.write(ARM_HOLDING);
  armLeft.write(ARM_HOLDING);
  delay(DELAY_BETWEEN_STEPS);
}

void ServosCube_Movex() {
  // Tilt cube backward: Down becomes Front
  armFront.write(ARM_RELEASED);
  armBack.write(ARM_RELEASED);
  delay(DELAY_BETWEEN_STEPS);
  
  clawRight.write(CLAW_HOME);
  clawLeft.write(CLAW_ROTATED);
  delay(DELAY_BETWEEN_STEPS2);
  
  armFront.write(ARM_HOLDING);
  armBack.write(ARM_HOLDING);
  delay(DELAY_BETWEEN_STEPS);
  
  armRight.write(ARM_RELEASED);
  armLeft.write(ARM_RELEASED);
  delay(DELAY_BETWEEN_STEPS);
  
  clawRight.write(CLAW_ROTATED);
  clawLeft.write(CLAW_HOME);
  delay(DELAY_BETWEEN_STEPS);
  
  clawRight.write(CLAW_HOME);
  delay(DELAY_BETWEEN_STEPS);
  
  armRight.write(ARM_HOLDING);
  armLeft.write(ARM_HOLDING);
  delay(DELAY_BETWEEN_STEPS);
}

void ServosCube_MoveZ() {
  // Rotate cube sideways
  armRight.write(ARM_RELEASED);
  armLeft.write(ARM_RELEASED);
  delay(DELAY_BETWEEN_STEPS);
  
  clawFront.write(CLAW_HOME);
  clawBack.write(CLAW_ROTATED);
  delay(DELAY_BETWEEN_STEPS2);
  
  armRight.write(ARM_HOLDING);
  armLeft.write(ARM_HOLDING);
  delay(DELAY_BETWEEN_STEPS);
  
  armFront.write(ARM_RELEASED);
  armBack.write(ARM_RELEASED);
  delay(DELAY_BETWEEN_STEPS);
  
  clawFront.write(CLAW_ROTATED);
  clawBack.write(CLAW_HOME);
  delay(DELAY_BETWEEN_STEPS);
  
  clawFront.write(CLAW_HOME);
  delay(DELAY_BETWEEN_STEPS);
  
  armFront.write(ARM_HOLDING);
  armBack.write(ARM_HOLDING);
  delay(DELAY_BETWEEN_STEPS);
}

void ServosCube_Movez() {
  // Rotate cube sideways opposite direction
  armRight.write(ARM_RELEASED);
  armLeft.write(ARM_RELEASED);
  delay(DELAY_BETWEEN_STEPS);
  
  clawFront.write(CLAW_ROTATED);
  clawBack.write(CLAW_HOME);
  delay(DELAY_BETWEEN_STEPS2);
  
  armRight.write(ARM_HOLDING);
  armLeft.write(ARM_HOLDING);
  delay(DELAY_BETWEEN_STEPS);
  
  armFront.write(ARM_RELEASED);
  armBack.write(ARM_RELEASED);
  delay(DELAY_BETWEEN_STEPS);
  
  clawFront.write(CLAW_HOME);
  clawBack.write(CLAW_ROTATED);
  delay(DELAY_BETWEEN_STEPS);
  
  clawBack.write(CLAW_HOME);
  delay(DELAY_BETWEEN_STEPS);
  
  armFront.write(ARM_HOLDING);
  armBack.write(ARM_HOLDING);
  delay(DELAY_BETWEEN_STEPS);
}

// ========================================
// CUBE ORIENTATION TRACKING
// ========================================

#define PHYS_FRONT  0
#define PHYS_RIGHT  1
#define PHYS_BACK   2
#define PHYS_LEFT   3
#define PHYS_UP     4
#define PHYS_DOWN   5

#define FACE_GREEN   0
#define FACE_RED     1
#define FACE_BLUE    2
#define FACE_ORANGE  3
#define FACE_WHITE   4
#define FACE_YELLOW  5

byte facePosition[6];

void initOrientation() {
  facePosition[FACE_GREEN]  = PHYS_FRONT;
  facePosition[FACE_RED]    = PHYS_RIGHT;
  facePosition[FACE_BLUE]   = PHYS_BACK;
  facePosition[FACE_ORANGE] = PHYS_LEFT;
  facePosition[FACE_WHITE]  = PHYS_UP;
  facePosition[FACE_YELLOW] = PHYS_DOWN;
}

void updateOrientationX() {
  byte temp = facePosition[FACE_WHITE];
  facePosition[FACE_WHITE]  = facePosition[FACE_BLUE];
  facePosition[FACE_BLUE]   = facePosition[FACE_YELLOW];
  facePosition[FACE_YELLOW] = facePosition[FACE_GREEN];
  facePosition[FACE_GREEN]  = temp;
}

void updateOrientationx() {
  byte temp = facePosition[FACE_WHITE];
  facePosition[FACE_WHITE]  = facePosition[FACE_GREEN];
  facePosition[FACE_GREEN]  = facePosition[FACE_YELLOW];
  facePosition[FACE_YELLOW] = facePosition[FACE_BLUE];
  facePosition[FACE_BLUE]   = temp;
}

void updateOrientationZ() {
  byte temp = facePosition[FACE_WHITE];
  facePosition[FACE_WHITE]  = facePosition[FACE_ORANGE];
  facePosition[FACE_ORANGE] = facePosition[FACE_YELLOW];
  facePosition[FACE_YELLOW] = facePosition[FACE_RED];
  facePosition[FACE_RED]    = temp;
}

void updateOrientationz() {
  byte temp = facePosition[FACE_WHITE];
  facePosition[FACE_WHITE]  = facePosition[FACE_RED];
  facePosition[FACE_RED]    = facePosition[FACE_YELLOW];
  facePosition[FACE_YELLOW] = facePosition[FACE_ORANGE];
  facePosition[FACE_ORANGE] = temp;
}

// ========================================
// MOVE EXECUTION
// ========================================

void executeFaceMove(byte face, bool clockwise) {
  byte physPos = facePosition[face];
  
  switch(physPos) {
    case PHYS_FRONT:
      if (clockwise) ServosFace_FrontCW();
      else ServosFace_FrontCCW();
      break;
      
    case PHYS_RIGHT:
      if (clockwise) ServosFace_RightCW();
      else ServosFace_RightCCW();
      break;
      
    case PHYS_BACK:
      if (clockwise) ServosFace_BackCW();
      else ServosFace_BackCCW();
      break;
      
    case PHYS_LEFT:
      if (clockwise) ServosFace_LeftCW();
      else ServosFace_LeftCCW();
      break;
      
    case PHYS_UP:
    case PHYS_DOWN:
      rotateCubeToAccessFace(face);
      executeFaceMove(face, clockwise);
      break;
  }
}

void rotateCubeToAccessFace(byte face) {
  byte physPos = facePosition[face];
  
  if (physPos == PHYS_UP) {
    ServosCube_MoveX();
    updateOrientationX();
    Serial.println(" [Cube rotated forward]");
  } 
  else if (physPos == PHYS_DOWN) {
    ServosCube_Movex();
    updateOrientationx();
    Serial.println(" [Cube rotated backward]");
  }
}

// ========================================
// SOLUTION PARSER
// ========================================

void executeMove(char move, bool prime, bool double_turn) {
  byte face;
  bool clockwise;
  
  switch(move) {
    case 'R':
      face = FACE_RED;
      clockwise = !prime;
      break;
    case 'L':
      face = FACE_ORANGE;
      clockwise = prime;
      break;
    case 'U':
      face = FACE_WHITE;
      clockwise = !prime;
      break;
    case 'D':
      face = FACE_YELLOW;
      clockwise = prime;
      break;
    case 'F':
      face = FACE_GREEN;
      clockwise = !prime;
      break;
    case 'B':
      face = FACE_BLUE;
      clockwise = prime;
      break;
    default:
      Serial.print("Unknown move: ");
      Serial.println(move);
      return;
  }
  
  Serial.print(move);
  if (prime) Serial.print("'");
  if (double_turn) Serial.print("2");
  Serial.print(" ");
  
  executeFaceMove(face, clockwise);
  
  if (double_turn) {
    executeFaceMove(face, clockwise);
  }
}

void parseSolution(String solution) {
  int i = 0;
  int len = solution.length();
  
  Serial.println("\n=== Executing Solution ===");
  Serial.print("Moves: ");
  
  while (i < len) {
    char c = solution.charAt(i);
    
    if (c == ' ') {
      i++;
      continue;
    }
    
    if (c == 'R' || c == 'L' || c == 'U' || c == 'D' || c == 'F' || c == 'B') {
      bool prime = false;
      bool double_turn = false;
      
      if (i + 1 < len && solution.charAt(i + 1) == '\'') {
        prime = true;
        i++;
      }
      
      if (i + 1 < len && solution.charAt(i + 1) == '2') {
        double_turn = true;
        i++;
      }
      
      executeMove(c, prime, double_turn);
    }
    
    i++;
  }
  
  Serial.println("\n=== Solution Complete! ===");
}

// ========================================
// INITIALIZATION AND READY STATE
// ========================================

void servos_init() {
  // Attach all servos
  clawRight.attach(PIN_CLAW_RIGHT);
  clawBack.attach(PIN_CLAW_BACK);
  clawLeft.attach(PIN_CLAW_LEFT);
  clawFront.attach(PIN_CLAW_FRONT);
  
  armRight.attach(PIN_ARM_RIGHT);
  armBack.attach(PIN_ARM_BACK);
  armLeft.attach(PIN_ARM_LEFT);
  armFront.attach(PIN_ARM_FRONT);
  
  // Initial position: claws at home, arms released (open)
  clawRight.write(CLAW_HOME);
  clawBack.write(CLAW_HOME);
  clawLeft.write(CLAW_HOME);
  clawFront.write(CLAW_HOME);
  
  armRight.write(ARM_RELEASED);
  armBack.write(ARM_RELEASED);
  armLeft.write(ARM_RELEASED);
  armFront.write(ARM_RELEASED);
  
  delay(1000);
}

void waitForUserAndGrabCube() {
  Serial.println("\n" + String("=") * 50);
  Serial.println("READY TO GRAB CUBE");
  Serial.println(String("=") * 50);
  Serial.println("Place cube in position with claws open");
  Serial.println("When ready, send 'START' or press Enter in Serial Monitor");
  Serial.println(String("=") * 50);
  
  // Wait for user input
  while (true) {
    if (Serial.available() > 0) {
      String input = Serial.readStringUntil('\n');
      input.trim();
      break;
    }
    delay(100);
  }
  
  Serial.println("\nClosing claws to grab cube...");
  
  // Close all arms to grab cube
  armRight.write(ARM_HOLDING);
  armBack.write(ARM_HOLDING);
  armLeft.write(ARM_HOLDING);
  armFront.write(ARM_HOLDING);
  
  delay(1500);
  
  Serial.println("✓ Cube secured!");
  Serial.println("Starting solution execution...\n");
  delay(500);
}

// ========================================
// MAIN SETUP AND LOOP
// ========================================

void setup() {
  Serial.begin(115200);
  
  delay(1000);
  
  Serial.println("=================================");
  Serial.println("Rubik's Cube Solver - Kociemba");
  Serial.println("=================================");
  
  Serial.println("Initializing servos...");
  servos_init();
  
  Serial.println("Initializing orientation...");
  initOrientation();
  
  Serial.println("\nREADY");
  Serial.println("Waiting for solution string...");
  Serial.println("Format: R U R' U' F2 D L' B");
  Serial.println("=================================\n");
}

void loop() {
  if (Serial.available() > 0) {
    String solution = Serial.readStringUntil('\n');
    solution.trim();
    
    if (solution.length() > 0) {
      Serial.print("Received: ");
      Serial.println(solution);
      
      // Wait for user to place cube and confirm
      waitForUserAndGrabCube();
      
      // Reset orientation
      initOrientation();
      
      // Parse and execute solution
      parseSolution(solution);
      
      Serial.println("\nReady for next solution...\n");
    }
  }
}
