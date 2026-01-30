#include "MeMegaPi.h"
#include <Adafruit_NeoPixel.h>


//LIghts
#define NUM_LEDS 4
// Define your two pins
Adafruit_NeoPixel stripA(NUM_LEDS, A13, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripB(NUM_LEDS, A14, NEO_GRB + NEO_KHZ800);

// Motor setup - all 4 motors
MeMegaPiDCMotor motor1(PORT3A);  // Front left
MeMegaPiDCMotor motor2(PORT3B);  // Front right  
MeMegaPiDCMotor motor3(PORT2A);  // Rear eft
MeMegaPiDCMotor motor4(PORT2B);  // Rear right

// Motor speeds (adjust these as needed)
#define SPEED_FORWARD 100    // Normal forward speed
#define SPEED_TURN 80        // Turning speed
#define SPEED_SEARCH 60      // Search/slow turn speed
#define SPEED_BACKWARD 100   // Reverse speed

String inputString = "";
bool stringComplete = false;

void setup() {
    Serial.begin(9600);
    
    // Wait for serial connection
    while (!Serial) {
        delay(10);
    }
    
    // Initialize all motors to stopped
    stop_moving();
    
    Serial.println("READY");  // Signal to Raspberry Pi

  stripA.begin();
  stripB.begin();
  stripA.show(); // Initialize all pixels to 'off'
  stripB.show();
}

void loop() {

    //color_loop();

    // Read serial commands from Raspberry Pi
    while (Serial.available()) {
        char inChar = (char)Serial.read();
        
        if (inChar == '\n') {
            stringComplete = true;
        } else {
            inputString += inChar;
        }
    }
    
    // Execute command when complete
    if (stringComplete) {
        inputString.trim();
        executeCommand(inputString);
        inputString = "";
        stringComplete = false;
    }
    
    delay(5);  // Small delay
}

void executeCommand(String cmd) {
    // Execute command from Raspberry Pi
    
    if (cmd == "LEFT") {
        // Rotate left (all motors rotate same direction for spot turn)
        motor1.run(SPEED_TURN);
        motor2.run(SPEED_TURN);
        motor3.run(SPEED_TURN);
        motor4.run(SPEED_TURN);
    }
    else if (cmd == "RIGHT") {
        // Rotate right (spot turn)
        motor1.run(-SPEED_TURN);
        motor2.run(-SPEED_TURN);
        motor3.run(-SPEED_TURN);
        motor4.run(-SPEED_TURN);
    }
    else if (cmd == "FORWARD") {
        // Move forward 
        motor1.run(SPEED_FORWARD);
        motor2.run(SPEED_FORWARD);
        motor3.run(-SPEED_FORWARD);
        motor4.run(-SPEED_FORWARD);
    }
    else if (cmd == "BACKWARD") {
        // Move backward
        motor1.run(-SPEED_BACKWARD);
        motor2.run(-SPEED_BACKWARD);
        motor3.run(SPEED_BACKWARD);
        motor4.run(SPEED_BACKWARD);
    }
    else if (cmd == "STRAFE_LEFT") {
        // Strafe left (if mecanum wheels)
        motor1.run(-SPEED_TURN);
        motor2.run(SPEED_TURN);
        motor3.run(SPEED_TURN);
        motor4.run(-SPEED_TURN);
    }
    else if (cmd == "STRAFE_RIGHT") {
        // Strafe right (if mecanum wheels)
        motor1.run(SPEED_TURN);
        motor2.run(-SPEED_TURN);
        motor3.run(-SPEED_TURN);
        motor4.run(SPEED_TURN);
    }
    else if (cmd == "SEARCH") {
        // Search for object (slow rotation)
        motor1.run(SPEED_SEARCH);
        motor2.run(SPEED_SEARCH);
        motor3.run(-SPEED_SEARCH);
        motor4.run(-SPEED_SEARCH);
    }
    else if (cmd == "SEARCH_LEFT") {
        // Search left (slower rotation)
        motor1.run(60);
        motor2.run(60);
        motor3.run(60);
        motor4.run(-0);
    }
    else if (cmd == "SEARCH_RIGHT") {
        // Search right (slower rotation)
        motor1.run(-60);
        motor2.run(-60);
        motor3.run(-60);
        motor4.run(-60);
    }
    else if (cmd == "GRASP") {
        // Stop and grasp
        stop_moving();
        delay(1000);  // Simulate grasping time
    }
    else if (cmd == "STOP") {
        stop_moving();
    }
    // Ignore unknown commands
}

// Movement functions 
void forward() {
    motor1.run(SPEED_FORWARD);
    motor2.run(SPEED_FORWARD);
    motor3.run(-SPEED_FORWARD);
    motor4.run(-SPEED_FORWARD);
}

void reverse() {
    motor1.run(-SPEED_BACKWARD);
    motor2.run(-SPEED_BACKWARD);
    motor3.run(SPEED_BACKWARD);
    motor4.run(SPEED_BACKWARD);
}

void rotate_left() {
    motor1.run(SPEED_TURN);
    motor2.run(SPEED_TURN);
    motor3.run(SPEED_TURN);
    motor4.run(SPEED_TURN);
}

void rotate_right() {
    motor1.run(-SPEED_TURN);
    motor2.run(-SPEED_TURN);
    motor3.run(-SPEED_TURN);
    motor4.run(-SPEED_TURN);
}

void stop_moving() {
    motor1.stop();
    motor2.stop();
    motor3.stop();
    motor4.stop();
}

void color_loop()
{
  // Turn Pin A13 LEDs Red
  for(int i=0; i<NUM_LEDS; i++) {
    stripA.setPixelColor(i, stripA.Color(255, 0, 0));
  }
  stripA.show();

  // Turn Pin A14 LEDs Blue
  for(int i=0; i<NUM_LEDS; i++) {
    stripB.setPixelColor(i, stripB.Color(0, 0, 255));
  }
  stripB.show();

  delay(1000);
  
  // Clear all
  stripA.clear();
  stripB.clear();
  stripA.show();
  stripB.show();
  delay(1000);
}