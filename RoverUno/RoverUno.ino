#include <AFMotor.h>

AF_DCMotor frontLeft(1);
AF_DCMotor rearLeft(2);
AF_DCMotor rearRight(3);
AF_DCMotor frontRight(4);

char serialBuffer[256] = {0};
short idx; // serial index;

// ---------------------------------------------
// Traditional arduino functions
// ---------------------------------------------
void setup()
{
  // put your setup code here, to run once:

  Serial.begin(115200);  // to the computer ( if connected)
  Serial3.begin(115200); // to the ESP8266
  Serial3.println("OK"); // tell ESP to start up

  frontLeft.run(RELEASE);
  frontRight.run(RELEASE);
  rearRight.run(RELEASE);
  rearLeft.run(RELEASE);
}

void loop()
{
  // put your main code here, to run repeatedly:
  delay(100); // we can delay as the serial events cause the movemment
}

// -------------------------------------------
// This function will be called when there's serial data
// -------------------------------------------
void serialEvent3()
{
  while (Serial3.available())
  {

    char data = Serial3.read();

    if (data == '\n')
    { //begin processing data

      serialBuffer[idx] = '\0'; //finalise String

      processSerialData();

      idx = 0; //restart the buffer
    }
    else
    {

      //add it to the buffer and increment the index
      serialBuffer[idx] = data;
      idx++;
    }
  }
}

// ------------------------------------------------------
// Private functions
// -----------------

// it is generally better to make sure that both the left
// and right motors use the same algorithm for speed and power
// so we should put them into their own functions
// that when we change it once, it will change it for both. 
int getPower(int reading){
  //atoi can -1; or reading can be 100; for "center"
  if (reading < 0 || reading == 100)
    return 0;
  else
    return 255; // we like max power, you could also use some value of reading.
}

int getDirection(int reading){
  if (reading < 100)
    return BACKWARD; 
  else 
    return FORWARD; //adafruit constants, isn't nice but ok
}

// -------------------------------------------
// Set Left side motors, either direction
// -------------------------------------------
void setLeft(int reading)
{
  int power = getPower(reading);
  int direction = getDirection(reading);

  frontLeft.setSpeed(power);
  frontLeft.run(direction);

  rearLeft.setSpeed(power);
  rearLeft.run(direction);
}

// -------------------------------------------
// Set Right side motors, depending on position
// -------------------------------------------
void setRight(int reading)
{
  int power = getPower(reading);
  int direction = getDirection(reading);

  frontRight.setSpeed(power);
  frontRight.run(direction);

  rearRight.setSpeed(power);
  rearRight.run(direction);
}

//---------------------------------------------
// extract information from serial and run motors
//---------------------------------------------
void processSerialData()
{
  //send info to computer.
  Serial.println("processing:");
  Serial.println(serialBuffer);

  if (serialBuffer[0] == '%')
  {
    return; //we can receive comments and do nothing with them.
  }

  //this gets a "pointer" to where ':' is in the string
  char *sep = strchr(serialBuffer, ':');

  //if we have a pointer:
  if (sep != 0)
  {

    *sep = 0; //change ':' into NULL; terminating the string.

    // now, serialBuffer as a string will only be up until the
    // original ':' character, as we've nulled the string at this
    // point.
    short leftMotor = atoi(serialBuffer);

    // thankfully, we still have that pointer; we can
    // increment it THEN send to atoi ( precrement );
    // which will get the next segment of data:
    // the pointer up until the original NULL char; (see 10 lines up)
    // for the next segment, until the original NULL char.

    short rightMotor = atoi(++sep);

    Serial.print("setting left to ");
    Serial.println(leftMotor, DEC);

    Serial.print("setting right to ");
    Serial.println(rightMotor, DEC);

    setLeft(leftMotor);
    setRight(rightMotor);
  }
  else
  {
    //send information to the computer, because we have a bug
    Serial.println("Sep is:");
    Serial.println((int)sep, HEX);
    Serial.println((int)*sep, HEX);
    Serial.println(*sep);
  }
}