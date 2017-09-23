#include <Servo.h>

#define IR_SENSOR A0
#define PAN_SERVO 3
#define TILT_SERVO 5
#define THETA_MIN 80
#define THETA_MAX 100
#define PHI_MIN 80
#define PHI_MAX 100

Servo servoPan;  // create servo object to control a servo
Servo servoTilt;  // create servo object to control a servo

int radius = 100;
int theta = THETA_MIN;
int phi = PHI_MIN;
int thetaStep = 1;
int phiStep = 2;

String result="";

void setup() {
  //Setup input and outputs: LEDs out, pushbutton in.
//  pinMode(PUSH_BUTTON, INPUT);
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
    theta = PHI_MAX;
  } else if (phi < PHI_MIN) {
    theta = PHI_MIN;
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

    // Pan to the next position
    theta += thetaStep;
    setTheta(theta);
    
    // Change the direction for the next theta change, if we've reached the max or min
    if (theta >= THETA_MAX || theta <= THETA_MIN) {
      thetaStep = -thetaStep;
      tilt();
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

// Tilts the scanner in the correct direction
void tilt() {
  // Tilt the scanner
  phi += phiStep;
  setPhi(phi);

  // Check if we need to change directions
  if (phi >= PHI_MAX || phi <= PHI_MIN) {
    phiStep = -phiStep;
  }
}

// Get the distance (in inches) measured by the IR sensor
float readDistFromSensor() {
  // Slope and intercept determined by calibration experiment
//  [0.349182602325546,-27.900101889476854,6.870208215211669e+02]
  float d = (float)analogRead(IR_SENSOR);
  return 0.0002654*d*d - 0.2693*d  + 74.25;
}

// Send data to the computer over serial
void transmitData(float radius, float theta, float phi) {
  Serial.println(result + radius + "\t" + theta + "\t" + phi);
}

