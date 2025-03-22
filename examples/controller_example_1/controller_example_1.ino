/*
 * Author: Joseph Neal
 * Initializes the library and provides a basic
 * controller example for 2 stepper motors.
 */

#include <TMC2209LEGACY.h>

// Pinouts for motor 1
#define EN_PIN           44       // Enable 
#define MS1              46       // Microstep
#define MS2              48       //     

#define STEP_PIN         50       // Step 
#define DIR_PIN          52       // Direction 

// Pinouts for motor 2
#define EN_PIN_B         45       // Enable 
#define MS1_B            47       // Microstep 
#define MS2_B            49       //          

#define STEP_PIN_B       51       // Step 
#define DIR_PIN_B        53       // Direction 


// initialize driver objects and an array to store them
TMC2209LEGACY driver1(STEP_PIN, DIR_PIN, EN_PIN, MS1, MS2);  
TMC2209LEGACY driver2(STEP_PIN_B, DIR_PIN_B, EN_PIN_B, MS1_B, MS2_B);  

TMC2209LEGACY* motors[2] = {&driver1, &driver2};  

static int numMotors = 2;   

// variables
float userAngle;
float userAngle2;
float degreesPerStep;
long stepsPerRev;
int sign = 1;
bool running;
float ang;

// ======== SETUP ========

void setup() {

  Serial.begin(115200);                    
  driver1.initializeDriver();              // initializes driver pins, zeros current position, sets microstepping to default value of 8
  driver2.initializeDriver();
  driver1.setMicrosteps(8);                // microstepping can be set to 8, 16, 32, and 64 (values other than this will result in default of 8)
  driver2.setMicrosteps(8);
  driver1.setGearRatio(20);                // if set to n > 1, an angle change of theta degrees will be physically represented by theta*n degrees
  driver2.setGearRatio(10);
  driver1.setAngle(45);                    // takes in n degrees, manually zeros the stepper so that its current position is treated as n degrees.
  driver2.setAngle(25);                    // to zero the stepper at current position, you would set n = 0
  running = true;
}

// ======== LOOP ========

void loop() {
  if (running) {
    if (Serial.available() > 0) {
      userAngle = Serial.parseFloat();
      driver1.setTargetAngle(userAngle);
      userAngle2 = Serial.parseFloat();
      driver2.setTargetAngle(userAngle2);
      Serial.read();      // ensures serial buffer is cleared, I believe you can also use Serial.flush()
    }
    if(abs(driver1.getAngleDif()) <= driver1.getDegreesPerStep()/2) {
      Serial.println("At target");
    }  else {
      Serial.println("Not at target");
    }
    actuate(motors, numMotors);  // actuate an array of motor objects towards their corresponding target angle.  numMotors should be exactly equal to the size of the array
  }
}

// ======== CONTROL FUNCTIONS ========

// takes in an array of motor objects and the arrays length 
// -> steps each motor i towards their respective target angle by j steps where j is determined by a motor's relative distance to its target
void actuate(TMC2209LEGACY* motors[], int numMotors) {      
  long stepDif;
  long minStepDif = 2147483647;
  for (int  i = 0; i < numMotors; i++) {
    stepDif = abs(motors[i]->getStepDif());
    if ((stepDif > 0) && (stepDif < minStepDif)) {
      minStepDif = stepDif;
    }
  }
  for (int i = 0; i < numMotors; i++) {
    int numSteps = (int)ceil(abs(motors[i]->getStepDif()) / minStepDif);
    for (int j = 0; j < numSteps; j++) {
      motors[i]->stepToAngle();
    }
  }
}





