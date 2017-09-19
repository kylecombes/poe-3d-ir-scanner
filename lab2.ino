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

int THETA_MIN = 0;
int THETA_MAX = 180;

long prev_t = 0;
int radius = 100;
int pot_value = 100;
int theta = 90;
int thetaStep = 0;
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
int READINGS_PER_ANGLE = 20;
int distanceSum = 0;
int readCount = 0;

void loop() {

  // Take a reading, add it to our running sum, and increment our read counter
  distanceSum += readDistFromSensor();
  readCount++;

  if (readCount == READINGS_PER_ANGLE) {

    // Calculate the average distance reading
    int distance = distanceSum / READINGS_PER_ANGLE;
    
    // Send our position and distance reading to the computer
    transmitData(distance, theta);

    // Pan to the next position
    theta += thetaStep;
    setTheta(theta);
    
    // Change the direction for the next theta change, if we've reached the max or min
    if (theta > THETA_MAX || theta < THETA_MIN) {
      thetaStep = -thetaStep;
    }

    // Reset counter and sum
    distanceSum = 0;
    readCount = 0;
  }
  // Wait 10ms between readings
  delay(10);
  
}

// Get the distance (in inches) measured by the IR sensor
float readDistFromSensor() {
  // Slope and intercept determined by calibration experiment
  return -19.3 * ((float)analogRead(IR_SENSOR)) + 245;
}

void transmitData(int radius, int theta) {
  Serial.println(result + radius + "\t" + theta);
}

