#include <ShiftRegister74HC595.h>
#include <EEPROMex.h>
const int motorVertical = 4;
const int motorHorizontal = 7;
const int motorVerticalEnable = 5;
const int motorHorizontalEnable = 6;
const int DATA_pin = 8;
const int LATCH_pin = 9;
const int SHIFTCLOCK_pin = 10;


long degreeH = 0;
unsigned long degreeTargetH = 0;
unsigned long degreeSourceH = 0;
unsigned long timeH = 0;
unsigned long timePerDegreeHorizontal = 640;
int sumOfTime = 0;

String readString = "";
int value = 0;

ShiftRegister74HC595 sr (2, DATA_pin, SHIFTCLOCK_pin, LATCH_pin);
void setup() {
  Serial.begin(9600);//defien a serial
  pinMode(motorVertical, OUTPUT); // Pin 4 as output and used for Direction1
  pinMode(motorHorizontal, OUTPUT); // Pin 7 as output and used for Direction2
  pinMode(motorVerticalEnable, OUTPUT); // Pin 5  as ouput and used for Enable1
  pinMode(motorHorizontalEnable, OUTPUT); // Pin 6 as output and used for Enable2
  //degreeSourceH = EEPROM.readInt(0);//Read eeprom address 0 where previous target value was saved
  //motorHorizontalFunction(0);//Motor initialize
  pinMode(2,OUTPUT);

}


/**
   control 74HC595 shift register
*/
void shiftRegisterControl(String inputData)
{
  int i;
  for (i = 0; i < inputData.length(); i++)
  {
    if (inputData.charAt(i) == 'A')
    {
      sr.set(1, HIGH);
    }
    else if (inputData.charAt(i) == 'B')
    {
      sr.set(2, HIGH);
    }
    else if (inputData.charAt(i) == 'C')
    {
      sr.set(3, HIGH);
    }
    else if (inputData.charAt(i) == 'D')
    {
      sr.set(4, HIGH);
    }
    else if (inputData.charAt(i) == 'E')
    {
      sr.set(5, HIGH);
    }
    else if (inputData.charAt(i) == 'F')
    {
      sr.set(6, HIGH);
    }
    else if (inputData.charAt(i) == 'G')
    {
      sr.set(7, HIGH);
    }
    else if (inputData.charAt(i) == 'H')
    {
      sr.set(9, HIGH);
    }
    else if (inputData.charAt(i) == 'I')
    {
      sr.set(10, HIGH);
    }
    else if (inputData.charAt(i) == 'J')
    {
      sr.set(11, HIGH);
    }
    else if (inputData.charAt(i) == 'K')
    {
      sr.set(12, HIGH);
    }
    else if (inputData.charAt(i) == 'L')
    {
      sr.set(13, HIGH);
    }

  }
  delay(5000);

  sr.setAllLow();
  delay(5000);
}

/**
   Motor horizontal control
*/
void motorHorizontalFunction(unsigned long degreeInput)
{
  degreeTargetH = degreeInput;
  sr.setAllLow();
  // EEPROM.writeInt(0, (int)degreeTargetH);//write the target value in eeporm address 0 so that we can initialize the horizontal motor
  Serial.println(degreeTargetH);
  degreeH = degreeTargetH - degreeSourceH;

  if (degreeH == 0)
  {
    digitalWrite(motorHorizontalEnable, LOW) ;
    sr.setAllLow();

  }
  else if (degreeH > 180)
  {
    //Anti clockwise or backward
    degreeH -= 360 ;
    timeH = (-1) * degreeH * timePerDegreeHorizontal;
    //Serial.println(timeH);
    //delay(5000);
    digitalWrite(motorHorizontalEnable, HIGH) ;
    digitalWrite(motorHorizontal, LOW);
    //pinMode(11,HIGH);
    sr.setAllLow();
    delay(timeH);
    //pinMode(11,LOW);
    digitalWrite(motorHorizontalEnable, LOW) ;
    degreeSourceH = degreeTargetH;
    sr.setAllLow();
  }
  else if (degreeH < -180)
  {
    //clockwise or forward
    //Serial.println("paisi");
    //delay(5000);
    sr.setAllLow();
    degreeH += 360 ;
    timeH = degreeH * timePerDegreeHorizontal;
    //Serial.println(timeH);
    digitalWrite(motorHorizontalEnable, HIGH) ;
    digitalWrite(motorHorizontal, HIGH);
    // pinMode(11,HIGH);
    sr.setAllLow();
    delay(timeH);
    // pinMode(11,LOW);
    digitalWrite(motorHorizontalEnable, LOW) ;
    degreeSourceH = degreeTargetH;
    sr.setAllLow();
  }
  else
  {
    if (degreeH < 0)
    {
      //when target 90 and source 0
      timeH = -1 * degreeH * timePerDegreeHorizontal;
      Serial.println(timeH);
      // pinMode(11,HIGH);
      digitalWrite(motorHorizontalEnable, HIGH) ;
      digitalWrite(motorHorizontal, LOW);
      sr.setAllLow();
      delay(timeH);
      digitalWrite(motorHorizontalEnable, LOW) ;
      // pinMode(11,LOW);
      degreeSourceH = degreeTargetH;
      sr.setAllLow();

    }
    else
    {
      timeH = degreeH * timePerDegreeHorizontal;
      Serial.println(timeH);
      // pinMode(11,HIGH);
      digitalWrite(motorHorizontalEnable, HIGH) ;
      digitalWrite(motorHorizontal, HIGH);
      sr.setAllLow();
      delay(timeH);
      // pinMode(11,LOW);
      digitalWrite(motorHorizontalEnable, LOW) ;
      degreeSourceH = degreeTargetH;
      sr.setAllLow();
    }
  }
}


void motorVerticalFunction(int inputDegree)
{
  if (inputDegree == 0)
  {
    digitalWrite(motorVerticalEnable, HIGH);
    digitalWrite(motorVertical,LOW);
    sr.setAllLow();
    delay(22000);
    delay(sumOfTime);
    digitalWrite(motorVerticalEnable,LOW);
    
  }
  else
  {
    unsigned long perDegree = 538;
    digitalWrite(motorVerticalEnable, HIGH);
    digitalWrite(motorVertical, HIGH);
    sr.setAllLow();
    delay(22000);//Initial delay for the Vertical motor start up
    digitalWrite(motorVerticalEnable, LOW);
    sr.setAllLow();
    int ara[9] = {2880, 3000, 3050, 2500, 4500, 2800, 4000, 3200, 3000};
    int division = inputDegree / 5;
    int remainder = inputDegree % 5;
    sumOfTime = 0;
    for (int i = 0; i < division; i++)
    {
      sumOfTime += ara[i];
    }
    sumOfTime += (remainder * perDegree);
    digitalWrite(motorVerticalEnable, HIGH);
    digitalWrite(motorVertical, HIGH);
    delay(sumOfTime);
    digitalWrite(motorVerticalEnable, LOW);
    sr.setAllLow();
  }
}
void loop() {
  /*digitalWrite(2,HIGH);
  delay(500);
  digitalWrite(2,LOW);
  delay(500);*/
  while (Serial.available()) {

    delay(3);  //small delay to allow input buffer to fill

    char c = Serial.read();  //gets one byte from serial buffer
    if (c == '#') {
      break;
    }  //breaks out of capture loop to print readstring
    readString += c;
  } //makes the string readString


  if (readString.charAt(0) == 'h')
  {

    readString.replace("h", "0");
    value = readString.toInt();
    motorHorizontalFunction(value); //call horizontal motor function
    readString = "";
    sr.setAllLow();
  }
  else if (readString.charAt(0) == 'v')
  {
    digitalWrite(2,HIGH);
    readString.replace("v", "0");
    value = readString.toInt();
    motorVerticalFunction(value);
    digitalWrite(2,LOW);
    readString = "";
    sr.setAllLow();

  }
  else if (readString.charAt(0) >= 'A' && readString.charAt(0) <= 'L')
  {
    digitalWrite(2,HIGH);
    shiftRegisterControl(readString);
    digitalWrite(2,LOW);
    readString = "";
    sr.setAllLow();
  }
  else
  {
    readString = "";
    sr.setAllLow();
  }
}
