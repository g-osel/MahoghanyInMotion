/*---------------Libraries-----------------------------*/
#include <Arduino.h>
#include <SPI.h>
#include <AccelStepper.h>
/*---------------Module Defines-----------------------------*/
#define REDLED 23
#define BLUELED 22
#define WAITINGLED 21
#define IN3_R 19 //controls right wheel dir
#define IN1_L 18 //controls left wheel dir
#define IN4_R 17 //controls right wheel dir
#define IN2_L 16 //controls left wheel dir
#define ENA_L 3 //left wheel PWM
#define ENB_R 4 //right wheel PWM
#define STEP 14 // for stepper motor
#define DIR 15 // for stepper motor
#define TEAMPOTPIN A6
#define speed_value 200
//#define GGTIME 250000000
#define GGTIME 20000000

#define Line_Threshold 2.5 //FILL THIS AND BELOW IN!
#define Left_Line 1
#define LeftCenter_Line 2
#define Center_Line 3
#define RightCenter_Line 4
#define Right_Line 5

/*---------------Module Function Prototypes-----------------*/
//handle state functions
void handleWaiting(void);
void handleGoToBlueSheep(void);
void handleGoToRedSheep(void);
void handleGG(void);

//move robot functions
void driveForward(int speed);
void driveBackward(void);
void stopDriving(void);
void stopAllMotors(void);
void turnRight(void);
void turnLeft(void);
void lineFollow(void);
void openDoor(void);
void closeDoor(void);

//additional useful functions
int potRead(int pin);
bool OnLine(int line);




/*---------------State Definitions--------------------------*/
typedef enum {WAITING, GO_TO_SHEEP_RED, GO_TO_SHEEP_BLUE,GG} States_t;


/*---------------Module Variables---------------------------*/
IntervalTimer GGtimer;
States_t state;
int potval;
int teamcolorval;
AccelStepper doorStepper(1, STEP, DIR);

/*---------------Main Functions----------------*/

void setup() {
  // pinMode(ENA_L, OUTPUT);
  // pinMode(ENB_R, OUTPUT);
  pinMode(IN1_L, OUTPUT);
  pinMode(IN2_L, OUTPUT);
  pinMode(IN3_R, OUTPUT);
  pinMode(IN4_R, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(BLUELED, OUTPUT);
  pinMode(REDLED, OUTPUT);
  pinMode(WAITINGLED, OUTPUT);
  pinMode(TEAMPOTPIN, INPUT);
  state = WAITING;
  GGtimer.begin(handleGG,GGTIME);
  doorStepper.setMaxSpeed(800);
  doorStepper.setSpeed(600);
  Serial.begin(9600);

}
void loop(){
  switch (state) {
    case WAITING:
      handleWaiting();
      break;
    case GO_TO_SHEEP_BLUE:
      handleGoToBlueSheep();
      break;
    case GO_TO_SHEEP_RED:
      handleGoToRedSheep();
      break;
    case GG:
      handleGG();
      break;
    default:    // Should never get into an unhandled state
      Serial.println("What is this I do not even...");
  }
}



/*----------------Module Functions--------------------------*/
int potRead(int pin){ // reads a potentiometer value and returns a value from 0 to 100
  potval = analogRead(pin);            
  return map(potval,0,1023,0,100);
}

void handleWaiting(void){
    digitalWrite(REDLED,LOW);
    digitalWrite(BLUELED,LOW);
    digitalWrite(WAITINGLED,HIGH);
    if (potRead(TEAMPOTPIN)>=75){
       state = GO_TO_SHEEP_BLUE;
     }
    if (potRead(TEAMPOTPIN)<=25){
       state = GO_TO_SHEEP_RED;
     }

}

void handleGoToBlueSheep(void){

    digitalWrite(REDLED,LOW);
    digitalWrite(BLUELED,HIGH);
    digitalWrite(WAITINGLED,LOW);
    //driveForward(speed_value);
    openDoor();
    //INSERT TIMED STOP

}

void handleGoToRedSheep(void){
    digitalWrite(REDLED,HIGH);
    digitalWrite(BLUELED,LOW);
    digitalWrite(WAITINGLED,LOW);
    //driveForward(speed_value);
    openDoor();
    //INSERT TIMED STOP
}

void handleGG(void){ // all motors off, all LEDs off
    digitalWrite(REDLED,LOW);
    digitalWrite(BLUELED,LOW);
    digitalWrite(WAITINGLED,LOW);
    stopAllMotors();
    state = GG;
}

void driveForward(int speed){//moves robot forward
  digitalWrite(IN1_L, HIGH);
  digitalWrite(IN2_L, LOW);
  digitalWrite(IN3_R, HIGH);
  digitalWrite(IN4_R, LOW);
  analogWrite(ENA_L, speed);
  analogWrite(ENB_R, speed);


}

void driveBackward(int speed){ // moves robot backward
  digitalWrite(IN2_L, HIGH);
  digitalWrite(IN1_L, LOW);
  digitalWrite(IN4_R, HIGH);
  digitalWrite(IN3_R, LOW);
  analogWrite(ENA_L, speed);
  analogWrite(ENB_R, speed);
}

void stopDriving(void){ // stops driving motors
  analogWrite(ENA_L, 0);
  analogWrite(ENB_R, 0);
}

void stopAllMotors(void){ // stops all motors
//I don't know what this is For?
}

void turnRight(int speed){ // turns robot right
  digitalWrite(IN4_R, HIGH);
  digitalWrite(IN3_R, LOW);
  digitalWrite(IN1_L, HIGH);
  digitalWrite(IN2_L, LOW);
  analogWrite(ENA_L, speed);
  analogWrite(ENB_R, speed);
}

void turnLeft(int speed){ // turns robot left Right wheel forward; left wheel back
  digitalWrite(IN3_R, HIGH);
  digitalWrite(IN4_R, LOW);
  digitalWrite(IN2_L, HIGH);
  digitalWrite(IN1_L, LOW);
  analogWrite(ENA_L, speed);
  analogWrite(ENB_R, speed);
}

void lineFollow(void){ // line following
//if center black move forward and check if other sensor black then at crossroads
if(OnLine(Center_Line))
{
  driveForward(speed_value);
  if(OnLine(Right_Line))
    { //make state turning to goal
    }
  if(OnLine(Left_Line))
    { //make state turning to goal
    }
}
//if center white
else
{
//if left black turn right
  if(OnLine(LeftCenter_Line))
    turnRight(speed_value);

//if right black turn left
  if(OnLine(RightCenter_Line))
    turnLeft(speed_value);
}
}

void openDoor(void){
   doorStepper.moveTo(5000);
   doorStepper.setSpeed(600);
   doorStepper.run();

}

void closeDoor(void){
  doorStepper.moveTo(-5000);
  doorStepper.setSpeed(600);
  doorStepper.run();

}

bool OnLine(int line){
  if (digitalRead(line) < Line_Threshold)
    return true;
  else
    return false;
}




