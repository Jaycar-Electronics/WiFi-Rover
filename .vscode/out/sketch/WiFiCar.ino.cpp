#include <Arduino.h>
#line 1 "c:\\Users\\dw06\\Documents\\Arduino\\WiFiCar\\WiFiCar.ino"
#line 1 "c:\\Users\\dw06\\Documents\\Arduino\\WiFiCar\\WiFiCar.ino"
#include <AFMotor.h>

AF_DCMotor frontLeft(1);
AF_DCMotor rearLeft(2);
AF_DCMotor rearRight(3);
AF_DCMotor frontRight(4);

#define CONTROL_SERIAL Serial

#line 10 "c:\\Users\\dw06\\Documents\\Arduino\\WiFiCar\\WiFiCar.ino"
void setLeft(int speed);
#line 22 "c:\\Users\\dw06\\Documents\\Arduino\\WiFiCar\\WiFiCar.ino"
void setRight(int speed);
#line 38 "c:\\Users\\dw06\\Documents\\Arduino\\WiFiCar\\WiFiCar.ino"
void processSerialData();
#line 68 "c:\\Users\\dw06\\Documents\\Arduino\\WiFiCar\\WiFiCar.ino"
void serialEvent();
#line 89 "c:\\Users\\dw06\\Documents\\Arduino\\WiFiCar\\WiFiCar.ino"
void setup();
#line 102 "c:\\Users\\dw06\\Documents\\Arduino\\WiFiCar\\WiFiCar.ino"
void loop();
#line 10 "c:\\Users\\dw06\\Documents\\Arduino\\WiFiCar\\WiFiCar.ino"
void setLeft(int speed){
  bool forward = true; 
  if(speed < 0){
    speed = - speed;
    forward = false; //we're going reverse
  }
  frontLeft.setSpeed(speed);
  rearLeft.setSpeed(speed);
  frontLeft.run(forward? FORWARD: BACKWARD);
  rearLeft.run(forward? FORWARD: BACKWARD);
}

void setRight(int speed){
  bool forward = true; 
  if(speed < 0){
    speed = - speed;
    forward = false; //we're going reverse
  }
  frontRight.setSpeed(speed);
  rearRight.setSpeed(speed);
  frontRight.run(forward? FORWARD: BACKWARD);
  rearRight.run(forward? FORWARD: BACKWARD);
}

char serialBuffer[256] = {0}; 

short idx;

void processSerialData(){

  Serial.println("processing:");
  Serial.println(serialBuffer);

  char* sep = strchr(serialBuffer, ':');
  if (sep != 0){
    *sep = 0; 
    short leftMotor = atoi(serialBuffer);

    short rightMotor = atoi(++sep);

    Serial.print("setting left to ");
    Serial.println(leftMotor,DEC);

    Serial.print("setting right to ");
    Serial.println(rightMotor,DEC);

    setLeft(leftMotor);
    setRight(rightMotor);
  }
  else {
    Serial.println("Sep is:");
    Serial.println((int)sep, HEX);
    Serial.println((int)*sep, HEX);
    Serial.println(*sep);
  }
   
}

void serialEvent(){
  while( CONTROL_SERIAL.available() ){

    char data = CONTROL_SERIAL.read(); 

    if (data == '\n'){ //begin processing data

      serialBuffer[idx] = '\0'; //finalise String

      processSerialData();

      idx = 0; //restart the buffer

    } else {

      serialBuffer[idx] = data;
      idx++; 

    }
  }
}
void setup() {
  // put your setup code here, to run once:
  //this sets the run MODE 
  CONTROL_SERIAL.begin(115200);
  Serial.println("OK");

  frontLeft.run(RELEASE);
  frontRight.run(RELEASE);
  rearRight.run(RELEASE);
  rearLeft.run(RELEASE);

}

void loop() {
  // put your main code here, to run repeatedly:

  delay(100);
}

