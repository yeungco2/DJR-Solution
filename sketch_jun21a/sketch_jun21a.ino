
#include <stdio.h>

//#define DEBUG


#define TICK 1
#define MAX_TASKS 5
#define SENSOR_READ_TASK 0
#define TONE_OFF_TASK 1
#define LED_OFF_TASK 2
#define BATTERY_STATUS_TASK 3
#define NORMAL_OP_TASK 4


// declare global variables
typedef struct recording
{
  int timestamp;
  int sensorValue;
  byte sensor;
} RECORD;


int taskTimer [MAX_TASKS] = {-1}; // by default all are off
int counter = 0;
int taskCount = 0;

RECORD readings [1000];
int readingCount = 0;


const int LEDPIN = 13;
const int BUZZER = 5;


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

// pin A3 and A4 and A7 are used for FSR
// OneSecondTimer
void loop () {
  Serial.println("printing readings");
  for (int j = 0; j < readingCount; j++)
  {
     Serial.print("timestamp: ");
     Serial.print(readings[j].timestamp);
     Serial.print("     sensorValue: ");
     Serial.print(readings[j].sensorValue);
     Serial.print("     sensor: ");
     Serial.println(readings[j].sensor);
  }
  Serial.println();

  
  // decrement all taskTimer by TICK(1)
  for (int i = 0; i < MAX_TASKS; i++)
  {
    // decrement task by TICK(1) first
    if(taskTimer[i] >= 0)
        taskTimer[i] -= TICK;
    else{
        // do nothing, don't decrement
    }
  }

  // all tasks are the same priority
  // tasks can overlap
  
  taskCount++;
 // Serial.println(taskCount);

    // Execute all tasks that are complete/timeout (0)
    if(taskTimer[SENSOR_READ_TASK] == 0)
    {
      sensorReadTask(); // call sensorReadTask
    }

    if (taskTimer[TONE_OFF_TASK] == 0)
    {
      toneOffTask();  // call task
    }

    if (taskTimer[LED_OFF_TASK] == 0)
    {
#ifdef DEBUG
      Serial.println("calling LED OFF");
#endif
      ledOffTask(); // call task
    }

    if (taskTimer[BATTERY_STATUS_TASK] == 0)
    {
      batteryStatusTask();  // call task
    }

    if (taskTimer[NORMAL_OP_TASK] == 0)
    {
#ifdef DEBUG
      Serial.println("calling NORMAL");
#endif
      normalOperationTask();  // call task
    }


for (int i = 0; i < MAX_TASKS; i++)
{
#ifdef DEBUG
      Serial.print("taskTimer ");
      Serial.print(i);
      Serial.print(" : ");
      Serial.println(taskTimer[i]);
#endif
}
  //Serial.println();
  //delay(1000);
  
  
}

void sensorReadTask ()
{
  counter++;
#ifdef DEBUG
  //counter++;
  //Serial.print(counter);
#endif
  int avgFSR1 = 0;
  int avgFSR2 = 0;
  int numFastRead = 3;
  // take 3 reads and average it
  
  for(int i = 0; i < numFastRead;i++)
  {
    avgFSR1 += analogRead(A3);
    avgFSR2 += analogRead(A4);
  }

  avgFSR1 /= numFastRead;
  avgFSR2 /= numFastRead;

// store FSR1 if over threshold
  if (avgFSR1 >= 50)
  {
    // print out timestamp and save it in memory
    RECORD temp;
    temp.timestamp = counter;
    temp.sensorValue = avgFSR1;
    temp.sensor = B01;

    readings[readingCount] = temp;
    readingCount++;
    toneEnable (1);
  }

// store FSR2 if over threshold
if (avgFSR2 >= 50)
  {
    // print out timestamp and save it in memory
    RECORD temp;
    temp.timestamp = counter;
    temp.sensorValue = avgFSR2;
    temp.sensor = B10;

    readings[readingCount] = temp;
    readingCount++;
    toneEnable (1);
  }
  

  
//  time_t t = now();
//  int hour = hour(t);
//  int minutes = minute(t);
//  int second = second(t);
//  int day = day(t);
//  int weekday = weekday(t);
//  int month = month(t);
//  int year = year(t);
//
//  printf("%d/%d/%d, %d:%d:%d", year, month, day, hour, minutes, second);
//  

  
  Serial.print("       A3: ");
  Serial.print(avgFSR1);
  Serial.print("       A4: ");
  Serial.println(avgFSR2);
  Serial.println();
  //Serial.print("A7:");
  //Serial.println(analogRead (FSR3));

  // reinitialize the task timer
  taskTimer[SENSOR_READ_TASK] = 1;
}

void toneOffTask()
{
  // turn PWM off
    digitalWrite (BUZZER, LOW);
}

void ledOffTask()
{
  // turn LED (pin 13) off
    digitalWrite (LEDPIN, HIGH);
}

void batteryStatusTask()
{
    
}

//turns on LED for 1sec every 5 Ticks
void normalOperationTask()
{
    // turn on LED and turn off in 5 ticks
    ledEnable(1);
    taskTimer[NORMAL_OP_TASK] = 5;  //
}

void toneEnable (int duration)
{
    // activate buzzer with PWM pin 
    digitalWrite (BUZZER, HIGH);
    taskTimer[TONE_OFF_TASK] = duration;
}

// turn on LED (pin D13) for a certain duration
void ledEnable (int duration)
{
    digitalWrite (LEDPIN, LOW);
    taskTimer[LED_OFF_TASK] = duration;
}

int analogToDigitalVoltage(int analogValue)
{
    
}

