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

#define _BV(bit) (1 << (bit))





int taskTimer [MAX_TASKS] = {-1}; // by default all are off
int counter = 0;
int taskCount = 0;

//RECORD readings [MAX_READINGS];
int readingCount = 0;
int battery [3000];

const int LEDPIN = 13;
const int BUZZER = 5;
bool isIncrease = true;

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
  int freq = 0;
  char incomingByte;
  Serial.write("Frequency: ");
  while(Serial.available() <= 0)
  {
    //do nothing
  }

  freq = 0;
  while(1)
  {
  incomingByte = Serial.read();
  if (incomingByte == '\n')
      break;
  if (incomingByte == -1)
      continue;
  freq *= 10;    //shift to the left
  freq = ((incomingByte - 48) + freq);   //convert to int, 
  Serial.println(freq);
  }

  int adc = map(freq, 0, 1023, 0, 255); 
  Serial.print(" adc: ");
  Serial.println(adc);
  analogWrite(BUZZER, adc);
  //https://www.allaboutcircuits.com/projects/using-the-arduinos-analog-io/
}

void pwm(int frequency, int dutyCycle)
{
  int duty = dutyCycle/100;
  int sec = (1000000/frequency)*duty;
  for (int i = 0; i < frequency; i++)
  {
  digitalWrite(BUZZER, HIGH);
  delayMicroseconds(sec);
  digitalWrite(BUZZER, LOW);
  delayMicroseconds((1000000/frequency) - sec);
  }
}

