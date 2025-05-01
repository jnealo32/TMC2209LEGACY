/*
 * Author: Joseph Neal
 * Initializes the library and provides a basic
 * method to simultaneously control 2 motors
 */

#include <TMC2209LEGACY.h>

// Pinouts for motor 1 
#define EN_PIN           40       // Enable 
#define MS1              42       // Microstep
#define MS2              44       //     

#define STEP_PIN         46       // Step 
#define DIR_PIN          48       // Direction 

// Pinouts for motor 2
#define EN_PIN_B         41       // Enable 
#define MS1_B            43       // Microstep
#define MS2_B            45       //     

#define STEP_PIN_B       47       // Step 
#define DIR_PIN_B        49       // Direction 


// initialize driver objects and an array to store them
TMC2209LEGACY driver1(STEP_PIN, DIR_PIN, EN_PIN, MS1, MS2);  
TMC2209LEGACY driver2(STEP_PIN_B, DIR_PIN_B, EN_PIN_B, MS1_B, MS2_B);  

// create an array of pointers for both drivers to be moved simultaneously
TMC2209LEGACY* motors[2] = {&driver1, &driver2};  
static int numMotors = 2;   

// variables
float userAngle;
float userAngle2;
bool running;


// ======== SETUP ========

void setup() {
  Serial.begin(9600);     

  driver1.initializeDriver();               // initializes driver pins, zeros current position, sets microstepping to default value of 8
  driver1.setMicrosteps(16);                // microstepping can be set to 8, 16, 32, and 64 (values other than this will result in default of 8)
  driver1.setGearRatio(2);                  // takes in an integer -> if set to n > 1, anlges are automatically converted to accomodate a n:1 gearbox
  driver1.setAngle(90);                      // takes in a float n in degrees degrees, manually zeros the stepper so that its current position is treated as n degrees.

  driver2.initializeDriver();              
  driver2.setMicrosteps(32);                

  running = true;
}


// ======== LOOP ========

void loop() {
  if (running) {
    if (Serial.available() > 0) {             // allows user to type two angles into the serial monitor to set the desired angle of each motor
      userAngle = Serial.parseFloat();
      driver1.setTargetAngle(userAngle);
      userAngle2 = Serial.parseFloat();
      driver2.setTargetAngle(userAngle2);
      
      Serial.read();
    }

    actuate(motors, 1);  // actuate an array of motor objects towards their corresponding target angle.  numMotors should be exactly equal to the size of the array
  }
}


// ======== HELPER FUNCTIONS ========

// takes in an array of motor objects and the arrays length 
// -> steps each motor i towards their respective target angle by j steps where j is determined by a motor's relative distance to its target, allowing both rotations to start and end at the same time
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
      motors[i]->stepToAngle(15);             // each motor steps j times towards the user-defined target angle with a 2*15 microsecond delay between steps
    }
  }
}





