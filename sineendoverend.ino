/*
 * MotorKnob
 *
 * A stepper motor follows the turns of a potentiometer
 * (or other sensor) on analog input 0.
 *
 * http://www.arduino.cc/en/Reference/Stepper
 * This example code is in the public domain.
 */

#include <Stepper.h>

// change this to the number of steps on your motor
#define STEPS 200

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepper(STEPS, 8, 9, 10, 11);

// the previous reading from the analog input
int previous = 0;
long val;
int move;
float t;
float w;
float kilo = 1000.0;

void setup() {
  // set the speed of the motor to 30 RPMs
  Serial.begin(9600);
  stepper.setSpeed(5000);
}

void loop() {

  t = millis()/kilo;
  val = floor(70*sin(24.5*t));
  move = 2*(val - previous);
  Serial.println(t);
  stepper.step(move);

  // move a number of steps equal to the change in the
  // sensor reading


  // remember the previous value of the sensor
  previous = val;
}