#include <Servo.h>

#define IR_SENSOR A0
#define PAN_SERVO 3
#define TILT_SERVO 5
#define THETA_MIN 75
#define THETA_MAX 115
#define PHI_MIN 75
#define PHI_MAX 115

Servo servoPan;  // create servo object to control the pan servo
Servo servoTilt;  // create servo object to control the tilt servo

int radius = 100;
int theta = THETA_MIN;
int phi = PHI_MIN;
int thetaStep = 1;
int phiStep = 1;

String result="";

void setup() {
  // Setup the servos and being serial communication with a computer
  servoPan.attach(PAN_SERVO);
  servoTilt.attach(TILT_SERVO);
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

// Move the tilt servo to a particular angle
void setPhi(int phi) {
  if (phi > PHI_MAX) {
    phi = PHI_MAX;
  } else if (phi < PHI_MIN) {
    phi = PHI_MIN;
  }
  servoTilt.write(phi);
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
    transmitData(distance, theta, phi);

    // Pan the sensor in the correct direction
    pan();

    // Reset counter and sum
    distanceSum = 0.0;
    readCount = 0.0;

    // Wait 100ms for Python program to receive and process
    delay(100);
  } else {
    // Wait 10ms between readings
    delay(5);
  }
}

// Pan the scanner in the correct direction
void pan() {
  // Update theta
  theta += thetaStep;

  // If we've gone past the max or min, change directions and go back the other way
  if (theta > THETA_MAX || theta < THETA_MIN) {
    thetaStep = -thetaStep;
    theta += 2*thetaStep;
    tilt();
  }

  // Move the servo
  setTheta(theta);
}

// Tilts the scanner in the correct direction
void tilt() {
  // Update phi
  phi += phiStep;
  
  // If we've gone past the max or min, change directions and go back the other way
  if (phi > PHI_MAX || phi < PHI_MIN) {
    Serial.println(result + "Phi too big/small! " + phi + " vs " + PHI_MAX);
    phiStep = -phiStep;
    phi += 2*phiStep;
  }
  
  // Move the servo
  setPhi(phi);
}

// Get the distance (in inches) measured by the IR sensor
float readDistFromSensor() {
  // Slope and intercept determined by calibration experiment
  float d = (float)analogRead(IR_SENSOR);
  return 0.0002654*d*d - 0.2693*d  + 74.25;
}

// Send data to the computer over serial
void transmitData(float radius, float theta, float phi) {
  Serial.println(result + radius + "\t" + theta + "\t" + phi);
}

