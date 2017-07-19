#include <stdio.h>

//#define DEBUG


#define TICK 1
#define MAX_TASKS 5
#define SENSOR_READ_TASK 0
#define TONE_OFF_TASK 1
#define LED_OFF_TASK 2
#define BATTERY_STATUS_TASK 3
#define NORMAL_OP_TASK 4
#define MAX_READINGS 2000
#define BATTERY_READINGS 3




int taskTimer [MAX_TASKS] = {-1}; // by default all are off
int counter = 0;
int taskCount = 0;

//RECORD readings [MAX_READINGS];
int readingCount = 0;
int battery [3000];

const int LEDPIN = 13;
const int BUZZER = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  pinMode (LEDPIN, OUTPUT);
  pinMode (BUZZER, OUTPUT);
  digitalWrite (LEDPIN, LOW); // default led ON
  taskTimer[SENSOR_READ_TASK] = 1;  // initialize frequency of sensor readings to 1/sec
  //taskTimer[NORMAL_OP_TASK] = 5;    // 
  Serial.println("Done Setup");
}

void loop() {
  // put your main code here, to run repeatedly

   battery[readingCount] = analogRead(A5);
   Serial.println(analogRead(A5));
   readingCount++;

   
   if ((Serial.available() > 0) && (Serial.read() == (int) "p"))
   {
    for(int i = 0; i < readingCount; i++)
    {
      Serial.print(i);
      Serial.print(":  ");
      Serial.println(battery[readingCount]);
      
    }
   }
  delay(1000);
}
