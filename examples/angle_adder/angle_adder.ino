/*
 * Author: Joseph Neal
 * Initializes the library and allows user to increase
 * or decrease angle by a specified angle
 */

#include <TMC2209LEGACY.h>

// Pinouts for motor driver
#define EN_PIN           2       // Enable 
#define MS1              3       // Microstep
#define MS2              4       //     

#define STEP_PIN         6       // Step 
#define DIR_PIN          7       // Direction 

TMC2209LEGACY driver(STEP_PIN, DIR_PIN, EN_PIN, MS1, MS2);

float userAngle;

void setup() {
  Serial.begin(9600);
  driver.initializeDriver();

  driver.initializeDriver();               // initializes driver pins, zeros current position, sets microstepping to default value of 8
  driver.setMicrosteps(16);                // microstepping can be set to 8, 16, 32, and 64 (values other than this will result in default of 8)
  driver.setGearRatio(2);                  // takes in an integer -> if set to n > 1, anlges are automatically converted to accomodate a n:1 gearbox
  driver.setAngle(90);                     // takes in a float n in degrees degrees, manually zeros the stepper so that its current position is treated as n degrees.

}

void loop() {  
  if (Serial.available() > 0) {
    userAngle = Serial.parseFloat();
    if (userAngle < 0) {                                            // sets direction based on sign of angle
      driver.setDirection(false);
    } else {
      driver.setDirection(true);
    }
    long stepsToAngle = (long)(((abs(userAngle)*200.0)/360.0) * driver.getMicrosteps());   // converts user angle to motor steps (for a 200 step motor)
        
    Serial.println("Input: " + String(userAngle));
    Serial.println("New angle: " + String(driver.getAngle()));      // current angle relative to intial position
    Serial.print("Net Steps: " + String(driver.getNetSteps()));     // step offset from position at time of driver initialization

    for (double i = stepsToAngle; i>0; i--) {                       // steps motor until desired angle has been added to previous position
      driver.step();
      // driver.step(30);                                           // step delay can be specified in microseconds to adjust speed (default is 40)
    }
  }

}
