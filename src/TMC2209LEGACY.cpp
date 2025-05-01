/*
 * Filename: TMC2209LEGACY.cpp
 * Version: 1.0.0
 * Author: Joseph Neal
 * Date: 2025-03-21
 * Description: Basic Arduino library for driving the TMC2209 operated in Legacy Mode. 
 */

#include "TMC2209LEGACY.h"
#include "Arduino.h"


// ======== CONSTRUCTOR ========

TMC2209LEGACY::TMC2209LEGACY(int step_pin, int dir_pin, int en_pin, int ms1_pin, int ms2_pin) {
  stepPin = step_pin;
  dirPin = dir_pin;
  enPin = en_pin;
  ms1 = ms1_pin;
  ms2 = ms2_pin;
}


// ======== METHODS ========

void TMC2209LEGACY::initializeDriver() {
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(enPin, OUTPUT);           // Set pinmodes
  pinMode(ms1, OUTPUT);
  pinMode(ms1, OUTPUT);
  direction = true;
  netSteps = 0;
  zeroAngle = 0;
  angle = zeroAngle;
  targetAngle = 0;
  setMicrosteps(8);                 // DEFAULT MICROSTEPPING
  setGearRatio(1);                  // DEFAULT GEAR RATIO
  digitalWrite(enPin, LOW);         // Enable TMC2209 board  
  digitalWrite(dirPin, direction);
}

void TMC2209LEGACY::step() {
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(40);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(40);
  if (direction == true) {
    netSteps += 1;
  } else {
    netSteps -= 1;
  }
  if (netSteps < 0) {
    netSteps += 200*microsteps*gearRatio;
  } else if (netSteps == 200*microsteps*gearRatio) {
    netSteps = 0;
  } 
  angle = (float) (360 * netSteps )/ ((float) (200 * microsteps * gearRatio));
}

void TMC2209LEGACY::step(int delay) {                    
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(delay);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(delay);
  if (direction == true) {
    netSteps += 1;
  } else {
    netSteps -= 1;
  }
  if (netSteps < 0) {
    netSteps += 200*microsteps*gearRatio;
  } else if (netSteps == 200*microsteps*gearRatio) {
    netSteps = 0;
  } 
  angle = (float) (360 * netSteps )/ ((float) (200 * microsteps * gearRatio));
}

void TMC2209LEGACY::changeDirection() {
  direction = !direction;
  digitalWrite(dirPin, direction);
}

void TMC2209LEGACY::stepToAngle() {
  if(!(getStepDif() == 0)) {
    atTarget = false;
    angleDif = targetAngle - angle;
    if (angleDif <= -180) {
      angleDif += 360;
    } else if (angleDif > 180 ) {
      angleDif -= 360;
    }
    if (angleDif < 0) {
      setDirection(false);
    } else {
      setDirection(true);
    }
    step();
  } else {
    atTarget = true;
  }
}

void TMC2209LEGACY::stepToAngle(int delay) {             // MANUALLY SET STEP SPEED IN MICROSECONDS (recommended above 20)
  if(!(getStepDif() == 0)) {
    atTarget = false;
    angleDif = targetAngle - angle;
    if (angleDif <= -180) {
      angleDif += 360;
    } else if (angleDif > 180 ) {
      angleDif -= 360;
    }
    if (angleDif < 0) {
      setDirection(false);
    } else {
      setDirection(true);
    }
    step(delay);
  } else {
    atTarget = true;
  }
}

// ======== GETTERS ========

int TMC2209LEGACY::getMicrosteps() {
  return microsteps;
}

int TMC2209LEGACY::getGearRatio() {
  return gearRatio;
}

bool TMC2209LEGACY::getDirection() {
  return direction;
}

float TMC2209LEGACY::getAngle() {
  return angle;
}

float TMC2209LEGACY::getDegreesPerStep() {
  return degreesPerStep;
}

float TMC2209LEGACY::getTargetAngle() {
  return targetAngle;
}

float TMC2209LEGACY::getAngleDif() {
  angleDif = targetAngle - angle;
  return angleDif;
}

long TMC2209LEGACY::getStepDif() {
  angleDif = targetAngle - angle;
  return (long)((float)angleDif/(float)degreesPerStep);
}

long TMC2209LEGACY::getNetSteps() {
  return netSteps;
}

// ======== SETTERS ========

void TMC2209LEGACY::setDirection(bool dir) {
  direction = dir;
  digitalWrite(dirPin, direction);
}

void TMC2209LEGACY::setMicrosteps(int ms) {
  if (ms == 16) {
    microsteps = 16;
    digitalWrite(ms1, HIGH);
    digitalWrite(ms2, HIGH);
  } else if (ms == 32) {
    microsteps = 32;
    digitalWrite(ms1, HIGH);
    digitalWrite(ms2, LOW);
  } else if (ms == 64) {
    microsteps = 64;
    digitalWrite(ms1, LOW);
    digitalWrite(ms2, HIGH);
  } else { // DEFAULT VALUE is 8
    microsteps = 8;
    digitalWrite(ms1, LOW);
    digitalWrite(ms2, LOW);
  }
  degreesPerStep = (float) (360)/ ((float) (200 * microsteps * gearRatio));
}

void TMC2209LEGACY::setGearRatio(int gr) {
  if (gr > 0) {
    gearRatio = gr;  
  } else {
    gearRatio = 1;
  }
  degreesPerStep = (float) (360)/ ((float) (200 * microsteps * gearRatio));
}

void TMC2209LEGACY::setAngle(float newAngle) {
  netSteps = 0;
  while (angle < newAngle) {
    netSteps += 1;
    angle = (float) (360 * netSteps )/ ((float) (200 * microsteps * gearRatio));
  }
  zeroAngle = newAngle;    // NOTE TO SELF: create getter for zeroAngle
  angle = zeroAngle;
  setTargetAngle(zeroAngle);
}

void TMC2209LEGACY::setTargetAngle(float toAngle) {
  targetAngle = (int)toAngle % 360;
  targetAngle += toAngle - (int)toAngle;
  if (targetAngle >= 360) {
    targetAngle -= 360;
  } else if (targetAngle < 0){
    targetAngle += 360;
  }
}
