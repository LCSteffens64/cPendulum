// a program for smooth motion of the cart between two limit switches along a track

// include the relevant libraries
#include <ezButton.h>
#include <AccelStepper.h>

// initialize the rotation directions, counterclockwise and clockwise respectively
#define DIRECTION_CCW -1
#define DIRECTION_CW   1

// define the different possible states the cart may be in at any given time
#define STATE_CHANGE_DIR 1
#define STATE_MOVE       2
#define STATE_MOVING     3

// define the maximum of position we can set
#define MAX_POSITION 0x7FFFFFFF 

// define the limit switches as buttons which attach to pins 6 and 7 respectively
ezButton leftLimitSwitch(6);  
ezButton rightLimitSwitch(7);

// define the stepper motor used and the pins it is attached to
AccelStepper stepper(AccelStepper::FULL4WIRE, 8, 9, 10, 11 );


int stepperState = STATE_MOVE;
int direction    = DIRECTION_CW;
long rightswitchpos = 0;
long leftswitchpos = 0;
long targetpos = direction * MAX_POSITION;
bool leftp = false;
bool rightp = false;

void setup() {
  Serial.begin(9600);

  // set the debounce time of each limit switch to 50 milliseconds
  leftLimitSwitch.setDebounceTime(50); 
  rightLimitSwitch.setDebounceTime(50); 

  // set the maximum speed
  stepper.setMaxSpeed(5000.0);   // set the maximum speed
  // set the acceleration
  stepper.setAcceleration(100000000.0); 
  // set initial speed
  stepper.setSpeed(2000);  
  stepper.setCurrentPosition(0);
  
}

void loop() {
  leftLimitSwitch.loop(); 
  rightLimitSwitch.loop(); 
  
  if (leftLimitSwitch.isPressed()) {
    stepperState = STATE_CHANGE_DIR;
    leftswitchpos = stepper.currentPosition(); //record the position of each switch, similar below
    leftp = true;
  }

  if (rightLimitSwitch.isPressed()) {
    stepperState = STATE_CHANGE_DIR;
    rightswitchpos = stepper.currentPosition();
    rightp = true;
  }

  switch (stepperState) {
    case STATE_CHANGE_DIR:
      targetpos = -targetpos;
      // after changing direction, the state changes again so we actually move the motor
      stepperState = STATE_MOVE;
      break;

    case STATE_MOVE:
      if (leftp && rightp){
        targetpos = (leftswitchpos + rightswitchpos)/2; //compute and set target to the average position of both switches, i.e. the middle.
      }
      stepper.move(targetpos);
      // after moving, go to the next state to allow the motor to keep moving indefinitely
      stepperState = STATE_MOVING; 
      break;
      

    case STATE_MOVING:
    if (leftp && rightp) {
      stepper.moveTo(targetpos); //moves the motor TO the target, which is now the middle.
      //for some strange, stupid reason, when I use moveTo() below for when the motor is just searching for the switches,
      //the button presses do not change direction of the motor, despite the fact that when I checked what the target was,
      //it did flip sign. When I use moveTo() in the else loop of this logic, the motor is TOLD to change direction, but it just
      //doesn't do it. WTF....
      if(stepper.distanceToGo()==0){
        Serial.println(stepper.currentPosition());
        stepper.setCurrentPosition(0);
        delay(100);
        leftp = false;
        rightp = false;
        targetpos = direction * MAX_POSITION;
        stepperState = STATE_MOVE;
        //stepper stops in the middle it found, resets its position, and repeats the process by forgetting where the switches were.
        //this is for graphing purposes, as we can then see how accurate the position indicator of the stepper is. I am hoping to find a consistent
        //middle to within 500 steps.
      }
    }
    else {
      stepper.move(targetpos);
    }

    break;
  }
  stepper.run();
}
