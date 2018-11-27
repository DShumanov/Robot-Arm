#include <Servo.h>

#define STATUS_LED LED_BUILTIN

#define JOINT_COUNT 6

#define JOINT_MAX_DEG 180
#define JOINT_MIN_DEG 0
#define JOINT_INIT_POS 90

#define DELTA_STEP 2

// constants won't change:
#define TASK_RATE 50

#define JOYSTICK_MAX 0
#define JOYSTICK_MIN 1023

int JoystickPing_g[JOINT_COUNT] = {A0, A1, A2, A3, A4, A5};
int ServoPing_g[JOINT_COUNT] = {3, 5, 6, 9, 10, 11};


int JoystickPosition_g[JOINT_COUNT] = { 0, 0, 0, 0, 0, 0 };
int ServoPosition_g[JOINT_COUNT] = { JOINT_INIT_POS, JOINT_INIT_POS, JOINT_INIT_POS, JOINT_INIT_POS, JOINT_INIT_POS, JOINT_INIT_POS };
int ServoPositionDeadMax_g[JOINT_COUNT] = { 480, 480, 480, 480, 480, 480 };
int ServoPositionDeadMin_g[JOINT_COUNT] = { 650, 650, 650, 650, 650, 650 };

// Variables will change:
int LedState_g = LOW;

Servo ServoJoint_g[JOINT_COUNT];

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store

void setup() {
  // set the digital pin as output:
  pinMode(STATUS_LED, OUTPUT);

  Serial.begin(115200);

  for(int index = 0; index < JOINT_COUNT; index++)
  {   
    ServoJoint_g[index].attach(ServoPing_g[index]);
    ServoJoint_g[index].write (JOINT_INIT_POS);
  }
}

unsigned long PreviousMillisL = 0;
unsigned long CurrentMillisL = 0;

void loop() {
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.
  
  CurrentMillisL = millis();
  
  if (CurrentMillisL - PreviousMillisL >= TASK_RATE)
  {
    // save the last time you blinked the LED
    PreviousMillisL = CurrentMillisL;

    task();
  }
}

void task()
{
  status_led();

  for(int index = 0; index < JOINT_COUNT; index++)
  {   
    JoystickPosition_g[index] = analogRead(JoystickPing_g[index]);
    
    if(JoystickPosition_g[index] > ServoPositionDeadMax_g[index] &&
       JoystickPosition_g[index] < ServoPositionDeadMin_g[index])
    {
      JoystickPosition_g[index] = 511;
    }
  }
  
  for(int index = 0; index < JOINT_COUNT; index++)
  {
    JoystickPosition_g[index] = (int)map(JoystickPosition_g[index], JOYSTICK_MIN, JOYSTICK_MAX, -DELTA_STEP, DELTA_STEP);    
  }
  
  for(int index = 0; index < JOINT_COUNT; index++)
  {
    ServoPosition_g[index] += JoystickPosition_g[index];

    if(ServoPosition_g[index] > JOINT_MAX_DEG)
    {
      ServoPosition_g[index] = JOINT_MAX_DEG;
    }
    if(ServoPosition_g[index] < JOINT_MIN_DEG)
    {
      ServoPosition_g[index] = JOINT_MIN_DEG;
    }
    
    ServoJoint_g[index].write(ServoPosition_g[index]);
  }

      
  for(int index = 0; index < JOINT_COUNT; index++)
  {   
    Serial.print(index, DEC);
    Serial.print(":");
    Serial.print(ServoPosition_g[index], DEC);
    Serial.print(", ");
  }

  Serial.println();

  return;
}

void status_led()
{
  // if the LED is off turn it on and vice-versa:
  if (LedState_g == LOW) {
    LedState_g = HIGH;
  } else {
    LedState_g = LOW;
  }

  // set the LED with the LedState_g of the variable:
  digitalWrite(STATUS_LED, LedState_g);
}
