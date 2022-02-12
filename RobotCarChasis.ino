#include <ComponentObject.h>
#include <RangeSensor.h>
#include <SparkFun_VL53L1X.h>
#include <vl53l1x_class.h>
#include <vl53l1_error_codes.h>

/*
  Reading distance from the laser based VL53L1X
  By: Nathan Seidle
  SparkFun Electronics
  Date: April 4th, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  SparkFun labored with love to create this code. Feel like supporting open source hardware?
  Buy a board from SparkFun! https://www.sparkfun.com/products/14667

  This example prints the distance to an object.

  Are you getting weird readings? Be sure the vacuum tape has been removed from the sensor.
*/

#include <Wire.h>
#include "SparkFun_VL53L1X.h" //Click here to get the library: http://librarymanager/All#SparkFun_VL53L1X

int E1 = 4;     //M1 Direction Control
int E2 = 7;     //M2 Direction Control
int M1 = 5;     //M1 Speed Control
int M2 = 6;     //M2 Speed Control

char LastTurn = 'l';
char PenultimateTurn = 'l';
char AntepenultimateTurn = 'l';

int fullSpeed = 0xff;
int halfSpeed = 0xdf;
int Stop = 0x00;

//when m1p/m2p is 255, it gives the maximum speed for one direction 0xFF
//When E1/E2 is HIGH, tracks move forward, When LOW, they go backward


void MoveForward(byte m1p, byte m2p)//Drive Motor Power Mode
{
  digitalWrite(E1, HIGH);
  analogWrite(M1, (m1p));

  digitalWrite(E2, HIGH);
  analogWrite(M2, (m2p));

  Serial.println("Move Forward");
}

void TurnRight(byte m1p, byte m2p)
{
  digitalWrite(E1, HIGH);
  analogWrite(M1, (m1p));

  digitalWrite(E2, LOW);
  analogWrite(M2, (m2p));

  Serial.println("Turn Right");
}

void TurnLeft(byte m1p, byte m2p)
{
  digitalWrite(E1, LOW);
  analogWrite(M1, (m1p));

  digitalWrite(E2, HIGH);
  analogWrite(M2, (m2p));
  
  Serial.println("Turn Left");
}

void BackUp(byte m1p, byte m2p)
{
  digitalWrite(E1, LOW);
  analogWrite(M1, (m1p));

  digitalWrite(E2, LOW);
  analogWrite(M2, (m2p));

  Serial.println("Back up");
}

//Optional interrupt and shutdown pins.
#define SHUTDOWN_PIN 2
#define INTERRUPT_PIN 3

SFEVL53L1X distanceSensor;
//Uncomment the following line to use the optional shutdown and interrupt pins.
//SFEVL53L1X distanceSensor(Wire, SHUTDOWN_PIN, INTERRUPT_PIN);

void setup(void)
{
  Wire.begin();

  Serial.begin(9600);
  Serial.println("VL53L1X Qwiic Test");

  Serial.println("Sensor online!");

  int i;
  for(i=4;i<=7;i++)
    pinMode(i, OUTPUT);
  delay(1000);
}

void loop(void)
{
  // Serial.println("Start of loop");
  distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
  while (!distanceSensor.checkForDataReady())
  {
    Serial.println("In Delay");
    delay(1);
  }
  int distance = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
  distanceSensor.clearInterrupt();
  distanceSensor.stopRanging();

  float distanceInches = distance * 0.0393701;
  float distanceFeet = distanceInches / 12.0;
  
  MoveForward(fullSpeed,fullSpeed);


  Serial.print("\tDistance(ft): ");
  Serial.print(distanceFeet, 2);

  Serial.println();

  // if(distanceFeet < 1.00)
  // {
  //   Serial.println(distanceFeet,2);
  //   //MoveForward(0x00,0x00);     // Stop
  //   Serial.println("Too Close");
  //   BackUp(fullSpeed, fullSpeed);  // Backup
  //   delay(1000);

  //   Serial.print("LastTurn = ");
  //   Serial.println(LastTurn);
  //   Serial.print("PenultimateTurn = ");
  //   Serial.println(PenultimateTurn);
  //   Serial.print("AntepenultimateTurn = ");
  //   Serial.println(AntepenultimateTurn);
  //   if((LastTurn == 'l' && PenultimateTurn == 'l' && AntepenultimateTurn == 'l') || 
  //      (LastTurn == 'r' && PenultimateTurn == 'l' && AntepenultimateTurn == 'l') || 
  //      (LastTurn == 'r' && PenultimateTurn == 'r' && AntepenultimateTurn == 'l'))
  //   {
  //     TurnRight(fullSpeed, fullSpeed);
  //     AntepenultimateTurn = PenultimateTurn;
  //     PenultimateTurn = LastTurn;
  //     LastTurn = 'r';
  //   }
  //   else
  //   {
  //     TurnLeft(fullSpeed, fullSpeed);
  //     PenultimateTurn = LastTurn;
  //     LastTurn = 'l';
  //   }
  //   delay(800);
  // }
}
