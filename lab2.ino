#include <Servo.h>

#define IR_SENSOR A0
#define PAN_SERVO 3
#define TILT_SERVO 5
#define THETA_MIN 80
#define THETA_MAX 100
#define PHI_MIN 75
#define PHI_MAX 120

Servo servoPan;  // Create servo object to control the pan servo
Servo servoTilt;  // Create servo object to control the tilt servo

int theta = THETA_MIN; // Pan
int phi = PHI_MIN; // Tilt
int thetaStep = 1; // Measure every 1 degree
int phiStep = 1; // Measure every 1 degree
bool justTilted = false;

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

// Take the average distance reading over 20 readings
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
//    delay(100);
  } else {
    // Wait 10ms between readings for the sensor to measure again
    delay(50);
  }
}

// Pan the scanner in the correct direction
void pan() {
  // Update theta
  theta += thetaStep;

  // If we've gone past the max or min, change directions and go back the other way
  if (theta > THETA_MAX || theta < THETA_MIN) {
    if (justTilted) {
      // If we just tilted and took a scan, change pan directions
      thetaStep = -thetaStep;
      theta += 2*thetaStep;
      justTilted = false;
    } else { // Tilt and take a scan point before panning
      tilt();
      justTilted = true;
    }
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
  // Convert reading to inches
  return 0.0000000052465*d*d*d*d - 0.0000083255*d*d*d + 0.00483*d*d - 1.2578*d + 141;
}

// Send data to the computer over serial
void transmitData(float radius, float theta, float phi) {
  Serial.println(result + radius + "\t" + theta + "\t" + phi);
}

