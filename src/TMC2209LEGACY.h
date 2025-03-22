/*
 * Filename: TMC2209LEGACY.h
 * Version: 1.0.0
 * Author: Joseph Neal
 * Date: 2025-03-21
 * Description: Basic Arduino library for driving the TMC2209 operated in Legacy Mode.  (header file) 
 */

#ifndef TMC2209LEGACY_H
#define TMC2209LEGACY_H

#include <Arduino.h>

class TMC2209LEGACY {
private:
  int stepPin, dirPin, enPin, ms1, ms2;
  int microsteps;
  float gearRatio;
  bool direction;
  long netSteps;   
  float angle, zeroAngle, targetAngle, lastAngle, degreesPerStep, angleDif;   // zeroAngle is obsolete             
public:
  bool atTarget;                        // not properly implemented yet

  // Constructor
  TMC2209LEGACY(int step_pin, int dir_pin, int en_pin, int ms1_pin, int ms2_pin);

  // Methods
  void initializeDriver();              // Initializes driver pins, zeros current position, sets microstepping to default value of 8
  void step();                          // Complete 1 step.  Direction must be changed using setDirection();
  void step(int delay);                 // Used to manually set delay between steps in microseconds (recommended above 20 at default microstepping).
  void changeDirection();               // Flips direction of step().
  void stepToAngle();                   // Step once towards targetAngle (default is 0).  Automatically takes the shortest of the two paths.
  void stepToAngle(int delay);          // Same as step(int delay)

  // Getters
  int getMicrosteps();                  // Returns current microstepping setting.
  int getGearRatio();                   // Returns current gear ratio setting.
  bool getDirection();                  // Returns current direction of step().
  float getAngle();                     // Returns current angle.
  float getDegreesPerStep();            // Return the physical angle that corresponds to a single step.
  float getTargetAngle();               // Returns targetAngle.
  float getAngleDif();                  // (BUGGY, does not always return shortest distance) Returns distance from current angle to target angle.
  long getStepDif();                    // (BUGGY, does not always return exact step distance)
  long getNetSteps();                   // Returns the clockwise distance in steps from zero position.

  // Setters
  void setDirection(bool dir);          // Set direction of step().
  void setMicrosteps(int ms);           // Set desired microstepping.
  void setGearRatio(int gr);            // Set desired gear ratio.
  void setAngle(float newAngle);        // Set new zero position to (current stepper position - newAngle).
  void setTargetAngle(float toAngle);   // Sets the angle which stepToAngle() steps towards.

};

#endif