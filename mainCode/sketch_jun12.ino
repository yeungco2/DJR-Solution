#include <stdio.h>



#define DEBUG


#define TICK 1
#define MAX_TASKS 5
#define SENSOR_READ_TASK 0
#define TONE_OFF_TASK 1
#define LED_OFF_TASK 2
#define BATTERY_STATUS_TASK 3
#define NORMAL_OP_TASK 4


// declare global variables
int taskTimer [MAX_TASKS] = {-1}; // by default all are off
int counter = 0;
int taskCount = 0;

const int LEDPIN = 13;



void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  pinMode (LEDPIN, OUTPUT);
  digitalWrite (LEDPIN, LOW); // default led OFF
  taskTimer[SENSOR_READ_TASK] = 1;  // initialize frequency of sensor readings to 1/sec
  taskTimer[NORMAL_OP_TASK] = 5;    // 
  Serial.println("DOne Setup");
}

// pin A3 and A4 and A7 are used for FSR
// OneSecondTimer
void loop () {

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
  Serial.println(taskCount);

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
  Serial.println();
  delay(1000);
  
  
}

void sensorReadTask ()
{
#ifdef DEBUG
  counter++;
  Serial.print(counter);
#endif
  Serial.print("       A3: ");
  Serial.print(analogRead(A3));
  Serial.print("       A4: ");
  Serial.println(analogRead(A4));
  Serial.println();
  //Serial.print("A7:");
  //Serial.println(analogRead (FSR3));

  // reinitialize the task timer
  taskTimer[SENSOR_READ_TASK] = 1;
}

void toneOffTask()
{
  // turn PWM off
}

void ledOffTask()
{
  // turn LED (pin 13) off
    digitalWrite (LEDPIN, LOW);
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
}

// turn on LED (pin D13) for a certain duration
void ledEnable (int duration)
{
    digitalWrite (LEDPIN, HIGH);
    taskTimer[LED_OFF_TASK] = duration;
}

int analogToDigitalVoltage(int analogValue)
{
    
}

