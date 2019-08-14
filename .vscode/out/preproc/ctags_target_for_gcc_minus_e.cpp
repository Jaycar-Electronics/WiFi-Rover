# 1 "c:\\Users\\dw06\\Documents\\Arduino\\WiFiCar\\WiFiCar.ino"
# 1 "c:\\Users\\dw06\\Documents\\Arduino\\WiFiCar\\WiFiCar.ino"
# 2 "c:\\Users\\dw06\\Documents\\Arduino\\WiFiCar\\WiFiCar.ino" 2

AF_DCMotor frontLeft(1);
AF_DCMotor rearLeft(2);
AF_DCMotor rearRight(3);
AF_DCMotor frontRight(4);



void setLeft(int speed){
  bool forward = true;
  if(speed < 0){
    speed = - speed;
    forward = false; //we're going reverse
  }
  frontLeft.setSpeed(speed);
  rearLeft.setSpeed(speed);
  frontLeft.run(forward? 1: 2);
  rearLeft.run(forward? 1: 2);
}

void setRight(int speed){
  bool forward = true;
  if(speed < 0){
    speed = - speed;
    forward = false; //we're going reverse
  }
  frontRight.setSpeed(speed);
  rearRight.setSpeed(speed);
  frontRight.run(forward? 1: 2);
  rearRight.run(forward? 1: 2);
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
    Serial.println(leftMotor,10);

    Serial.print("setting right to ");
    Serial.println(rightMotor,10);

    setLeft(leftMotor);
    setRight(rightMotor);
  }
  else {
    Serial.println("Sep is:");
    Serial.println((int)sep, 16);
    Serial.println((int)*sep, 16);
    Serial.println(*sep);
  }

}

void serialEvent(){
  while( Serial.available() ){

    char data = Serial.read();

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
  Serial.begin(115200);
  Serial.println("OK");

  frontLeft.run(4);
  frontRight.run(4);
  rearRight.run(4);
  rearLeft.run(4);

}

void loop() {
  // put your main code here, to run repeatedly:

  delay(100);
}
