/*

I intentionally use the const byte construct here
instead of #define. It's less dangerous (no name collision possible)
and safer since variables have scope.
*/
const byte PUSH_BUTTON = 8;
const byte POT = A0;
const byte CMD_READ_POT = 1;
const byte CMD_READ_BTN = 2;

long prev_t = 0;
int radius = 100;
int pot_value = 100;
int theta = 0;
byte cmd_id = 0; 

byte btn_state = LOW;

String result="";

void setup() {
  //Setup input and outputs: LEDs out, pushbutton in.
  pinMode(PUSH_BUTTON, INPUT);
  Serial.begin(9600);
}

void loop() {
//  byte btn_state = digitalRead(PUSH_BUTTON);
  radius = analogRead(POT);

  transmitData(radius, theta);

//  if(Serial.available() > 0) {
//    cmd_id = Serial.read();
//  } else {
//    cmd_id = 0;
//  }

//  switch(cmd_id){
//    case CMD_READ_POT:
//      result = result + "Potentiometer reads: " + pot_value;
//      Serial.println(sensor_value);
//      result = "";
//      break;
//    case CMD_READ_BTN:
//      result = result + "Button state: " + btn_state;
//      Serial.println(result);
//      result = "";
//      break;
//    break;
//  }
}

void transmitData(int radius, int theta) {
  Serial.println(result + radius + "\t" + theta);
}

