
#include <stdio.h>

#define DEBUG
/*
 * A3, A4, A5 are for FSR
*/

#define TICK 1
#define MAX_TASKS 5
#define SENSOR_READ_TASK 0
#define TONE_OFF_TASK 1
#define LED_OFF_TASK 2
#define BATTERY_STATUS_TASK 3
#define NORMAL_OP_TASK 4
#define MAX_READINGS 1500
#define BATTERY_READINGS 3
#define VOLTAGE_THRESHOLD 1024  // out of 1024
#define INNER_FACTOR 0.3
#define OUTER_FACTOR 0.2
#define HEEL_FACTOR 0.5



// function protocol
void storeReading (byte FSR, int sensorValue);
void pwm(int frequency, int dutyCycle);

// define thresholds
int INNER_THRESHOLD = 0;
int OUTER_THRESHOLD = 0;
int HEEL_THRESHOLD = 0;
int WEIGHT_LB = 0;

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

RECORD readings [MAX_READINGS];
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
  taskTimer[BATTERY_STATUS_TASK] = 2;
  // initialize threshold in setup
  WEIGHT_LB = 100;
  INNER_THRESHOLD = INNER_FACTOR*WEIGHT_LB;
  OUTER_THRESHOLD = OUTER_FACTOR*WEIGHT_LB;
  HEEL_THRESHOLD = HEEL_FACTOR*WEIGHT_LB;
  Serial.print("Inner:  ");
  Serial.print(INNER_THRESHOLD);
  Serial.print(", Outer:  ");
  Serial.print(OUTER_THRESHOLD);
  Serial.print(", Heel: ");
  Serial.println(HEEL_THRESHOLD);
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
      ledOffTask(); // call task
    }

    if (taskTimer[BATTERY_STATUS_TASK] == 0)
    {
      batteryStatusTask();  // call task
    }

    if (taskTimer[NORMAL_OP_TASK] == 0)
    {
      normalOperationTask();  // call task
    }
    delay(1000);
}


void sensorReadTask()
{
  counter++;

  int inner = 0;  // B01
  int outer = 0;  // B10
  int heel = 0;   // B11

  bool alarm = false;
  
  int numFastRead = 3;
  // take 3 reads and average it
  
  for(int i = 0; i < numFastRead;i++)
  {
    inner += analogRead(A3);
    outer += analogRead(A4);
    heel += analogRead(A5);
  }

  inner /= numFastRead;
  outer /= numFastRead;
  heel /= numFastRead;
#ifdef DEBUG
   Serial.print("Inner:  ");
  Serial.print(inner);
  Serial.print(", Outer:  ");
  Serial.print(outer);
  Serial.print(", Heel: ");
  Serial.println(heel);
#endif
// store inner FSR if over threshold
  if (inner >= INNER_THRESHOLD)
  {
      // store reading and timestamp into memory if there's enough space
      storeReading (B01, inner);
      alarm = true;
  }

// store outer FSR if over threshold
if (outer >= OUTER_THRESHOLD)
  {
      // store reading and timestamp into memory if there's enough space
      storeReading (B10, outer);
      alarm = true;
  }

  // store heel if over threshold
if (heel >= HEEL_THRESHOLD)
  {
      // store reading and timestamp into memory if there's enough space
      storeReading (B11, heel);
      alarm = true;
  }

 if (alarm == true)
 {
   toneEnable(1);
 }

 // reinitialize the task timer
  taskTimer[SENSOR_READ_TASK] = 1;
}

void toneOffTask()
{
  // turn PWM off
    //analogWrite (A3, 0);
}

void ledOffTask()
{
  // turn LED (pin 13) off
    digitalWrite (LEDPIN, HIGH);
}

void batteryStatusTask()
{
  // take 3 read of analog input, compare to threshold(ex. 20% of 3.7V)
    int analogLevel = 0;
    int numFastRead = 3;
    
    // take 3 reads and average it
    for(int i = 0; i < numFastRead;i++)
    {
      analogLevel += analogRead(A4);
    }
  
    analogLevel /= numFastRead;

// if battery voltage is less than voltage threshold, sound buzzer and turn on LED
    if (analogLevel <= VOLTAGE_THRESHOLD)
    {
        //ledEnable(5);
        //toneEnable(5);
    }
    taskTimer[BATTERY_STATUS_TASK] = 2;  // can set this to another
    int voltageLevel = analogToVoltage(analogLevel);
    
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
    pwm (2000, 50);
    taskTimer[TONE_OFF_TASK] = duration;
}

// turn on LED (pin D13) for a certain duration
void ledEnable (int duration)
{
    digitalWrite (LEDPIN, LOW);
    taskTimer[LED_OFF_TASK] = duration;
}

int analogToVoltage(int analogValue)
{
    return analogValue*5/1024;
}

// store reading and timestamp into memory if there's enough space
void storeReading (byte FSR, int sensorValue)
{
  // print out timestamp and save it in memory
    RECORD temp;
    temp.timestamp = counter;
    temp.sensorValue = sensorValue;
    temp.sensor = FSR;

    if (readingCount < MAX_READINGS)
    {
      readings[readingCount] = temp;
      readingCount++;
    } 
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
  Serial.println("Finished playing");
}
