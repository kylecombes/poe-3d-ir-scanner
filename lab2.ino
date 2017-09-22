#include <Servo.h>

Servo servoPan;  // create servo object to control a servo

/*

I intentionally use the const byte construct here
instead of #define. It's less dangerous (no name collision possible)
and safer since variables have scope.
*/
const byte PUSH_BUTTON = 8;
const byte IR_SENSOR = A0;
const byte CMD_READ_POT = 1;
const byte CMD_READ_BTN = 2;
const byte XY_SERVO = 3;

int THETA_MIN = 70;
int THETA_MAX = 110;

long prev_t = 0;
int radius = 100;
int pot_value = 100;
int theta = THETA_MIN;
int thetaStep = 1;
byte cmd_id = 0;

byte btn_state = LOW;

String result="";

void setup() {
  //Setup input and outputs: LEDs out, pushbutton in.
  pinMode(PUSH_BUTTON, INPUT);
  servoPan.attach(XY_SERVO);
  Serial.begin(9600);
}

// Move the panning servo to a particular angle
void setTheta(int theta) {
  if (theta > THETA_MAX) {
    theta = THETA_MAX;
  } else if (theta < THETA_MIN) {
    theta = THETA_MIN;
  }
  servoPan.write(theta);
}

// Take the average distance reading over 10 readings
float READINGS_PER_ANGLE = 20.0;
float distanceSum = 0.0;
float readCount = 0.0;

void loop() {

  // Take a reading, add it to our running sum, and increment our read counter
  distanceSum += readDistFromSensor();
  readCount++;

  if (readCount == READINGS_PER_ANGLE) {

    // Calculate the average distance reading
    float distance = distanceSum / READINGS_PER_ANGLE;
    
    // Send our position and distance reading to the computer
    transmitData(distance, theta);

    // Pan to the next position
    theta += thetaStep;
    setTheta(theta);
    
    // Change the direction for the next theta change, if we've reached the max or min
    if (theta >= THETA_MAX || theta <= THETA_MIN) {
      thetaStep = -thetaStep;
    }

    // Reset counter and sum
    distanceSum = 0.0;
    readCount = 0.0;

    // Wait 50ms for Python program to receive and process
    delay(200);
  } else {
    // Wait 10ms between readings
    delay(10);
  }
}

// Get the distance (in inches) measured by the IR sensor
float readDistFromSensor() {
  // Slope and intercept determined by calibration experiment
//  [0.349182602325546,-27.900101889476854,6.870208215211669e+02]
  float d = (float)analogRead(IR_SENSOR);
  return 0.0002654*d*d - 0.2693*d  + 74.25;
}

void transmitData(float radius, float theta) {
  Serial.println(result + radius + "\t" + theta);
}

