#include <stdio.h>

int counter = 0;

//#define MAX_TASKS 5
//#define SENSOR_READ_TASK 0
//#define TONE_OFF_TASK 1
//#define LED_OFF_TASK 2
//#define BATTERY_STATUS_TASK 3
//#define NORMAL_OP_TASK 4

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  counter++;
  Serial.print(counter);
  Serial.print("       A3: ");
  int var1 = analogRead(A3);
  Serial.print(var1);
  Serial.print("       A4: ");
  int var2 = analogRead(A4);
  Serial.println(var2);
  
  //Serial.print("A7:");
  //Serial.println(analogRead (FSR3));
  delay(500);
  
  
}

int analogToDigitalVoltage(int analogValue)
{
    
}

