/*
Adapted from MotorKnob from the Stepper.h library examples.

This script was written to force a belt-driven cart-pendulum to execute oscillations
at sequentially increasing frequencies. The serial output is formatted to .csv for eventual
reading and analysis by sweepgrapher.py.

Leonard Steffens
August 2022
Cavendish Laboratories
University of Cambridge
lcs64@cam.ac.uk
 */

#include "Stepper.h"
#include "AS5600.h"
#include "Wire.h"
#include "math.h"

AS5600 as5600;

// Steps per rotation for this motor are 200.
#define STEPS 200

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepper(STEPS, 8, 9, 10, 11);

// the previous reading from the analog input
int previous = 0;
// current move value
long val;
// the differential by which the motor moves in the current step
int move;
// independent variable of time
float t;
float telapsed = 0;
float kilo = 1000.0;

//set the amplitude
int amp = 200;
//initialise frequency variable as well as time normalisation
//frequency increment per 25 osc is also set here
float w=2.0;
float wincr = 0.5;
float maxw = 20;
//oscillations per cycle:
int oscl = 30;
//initial motor speed
int s_0 = w*60;
//with this line the motor speed to frequency ratio remains constant in all steps
int sincr = s_0 * wincr/w;


void setup() {
  Serial.begin(9600);
  //I think the motor speed is in RPM..? Not that it matters since it isn't
  //running constant RPM anyways. The motor-speed-to-frequency ratio was found
  //to optimally be 60.
  stepper.setSpeed(s_0);
  Wire.begin();
  as5600.begin();
  //print the header of what will eventually be a .csv file once the serial
  //output is logged in PuTTY or otherwise
  Serial.println("Freq,time,motorpath,angle");
}

void loop() {
  //The loop performs 'oscl' oscillations, and then moves on to the next frequency & motor speed.

  for (float z = 0; z < oscl*M_PI/w; z = t-oscl*M_PI/w) {
    //initialise time variable
    t = millis()/kilo - telapsed;
    //plot sinusoidal motor path at correct ampl and frequency
    val = floor(amp*sin(w*t));
    //differential movement of the motor
    move = (val - previous);
    stepper.step(move);
    //save the last motor move for the next differential
    previous = val;
    //print the current readings to the serial/.csv file.
    Serial.print(w);
    Serial.print(", \t");
    Serial.print(t);
    Serial.print(", \t");
    Serial.print(val);
    Serial.print(", \t");
    Serial.println(as5600.rawAngle() * AS5600_RAW_TO_RADIANS);
  }
  //after for loop excecutes for 25 oscillations, the frequency increases
  //this records the current time and resets the t variable.
  telapsed = millis()/kilo;
  //frequency is incremented
  w += wincr;
  //motor speed is incremented. this is to maintain optimal forcing speed
  s_0 += sincr;
  stepper.setSpeed(s_0);
  //reset dynamic variables to prevent jumping
  val = 0;
  move = 0;
  previous = 0;

  if (w>maxw) {
    while(1) {
      //once the highest frequency of interest is reached, the program 
      //enters this loop and does nothing for infinite time.
    }
  }
}







