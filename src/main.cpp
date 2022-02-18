#include <Arduino.h>
#include <AccelStepper.h>
// Create an IntervalTimer object 


#define MotorPin 0
#define AnalogPin 16

#define left LOW
#define MAXSPEED 400
#define pinDirection 7
#define StepPin 3
#define Potentiometer 23
uint32_t period = 2040;
IntervalTimer OnTimer;
IntervalTimer OffTimer;


uint8_t ledState;
uint16_t duty; //amount which it is on (1-100)
volatile uint32_t count;
uint32_t time;
int16_t val;
uint32_t timePassed;
uint8_t direction = HIGH;



void OnFxn(void);
void OffFxn(void);
void Stop(void);
void ChangeDIR(void);
 AccelStepper stepper(1,StepPin,9);

void setup() {
  pinMode(MotorPin, OUTPUT);
  pinMode(Potentiometer, INPUT);
  pinMode(AnalogPin, OUTPUT);
 // digitalWrite(MotorPin, HIGH);
  pinMode(pinDirection, OUTPUT);
 // pinMode(pinStep, OUTPUT);
 stepper.setMaxSpeed(MAXSPEED);
  OnTimer.begin(OnFxn, period); //put in frequency
  Serial.begin(9600);
}



void loop() {
//  Serial.println("hi");

  val = analogRead(Potentiometer);
  val = map(val, 0, 1023,1, 99 ); //part 1
  //digitalWrite(pinDirection, direction);
  val = map(val, 0, 1023, 0, MAXSPEED ); //part 4
  stepper.setSpeed(val);
  
  //analogWrite(AnalogPin, val);
  if(Serial.available()>0){
    Serial.read();
    Stop(); //part 1
    ChangeDIR(); //part 4
  }
  stepper.runSpeed();
}

void Stop(){
  digitalWrite(MotorPin, LOW);
  OffTimer.end();
  OnTimer.end();
}

void OnFxn(){
  digitalWrite(MotorPin, HIGH);
  OffTimer.begin(OffFxn,val*period/100); // part 1
  OffTimer.begin(OffFxn, 50*period/100); // part 4
 // Serial.println("yo");
}

void OffFxn(){
  digitalWrite(MotorPin, LOW);
  OffTimer.end();
}

void ChangeDIR(){
  if(direction == HIGH){
    direction = LOW;
     digitalWrite(pinDirection, LOW);
  }
  else{
    //(direction == LOW){
    direction = HIGH;
    digitalWrite(pinDirection, HIGH);
  }
 
  Serial.println(direction);
}

// cant get 0 or 100 becuarse cant have 0 timer or have timers go off at same time
//error due to setting both timers at different instances and time to switch transistors
//want high f bc more stable (average V less wavering)