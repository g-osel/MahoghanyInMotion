/*---------------Libraries-----------------------------*/
#include <Arduino.h>
#include <A4988.h>
#include <SPI.h>
#include <AccelStepper.h>
/*---------------Module Defines-----------------------------*/
#define REDLED 23
#define BLUELED 22
#define WAITINGLED 21
#define IN3_R 19 //controls right wheel dir blue
#define IN1_L 18 //controls left wheel dir green
#define IN4_R 17 //controls right wheel dir yellow 
#define IN2_L 16 //controls left wheel dir orange
#define ENA_L 3 //left wheel PWM black
#define ENB_R 4 //right wheel PWM white
#define STEP 14 // for stepper motor orange
#define DIR 15 // for stepper motor yellow
#define TEAMPOTPIN A6
#define speed_value 300
#define GGTIME 250000000
//#define GGTIME 20000000

#define Line_Threshold 4.0 //FILL THIS AND BELOW IN! 3.5-4.4
#define Left_Line 10
#define LeftCenter_Line 9
#define Center_Line 8
#define RightCenter_Line 7
#define Right_Line 6

#define MS1 10
#define MS2 11
#define MS3 12


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
void turnRight(int speed);
void turnLeft(int speed);
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
//A4988 stepper(200, DIR, STEP);

/*---------------Main Functions----------------*/

void setup() {
   pinMode(ENA_L, OUTPUT);
   pinMode(ENB_R, OUTPUT);
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
  pinMode(Left_Line,INPUT);
  pinMode(LeftCenter_Line, INPUT); 
  pinMode(Center_Line,INPUT) ;
  pinMode(RightCenter_Line,INPUT) ;
  pinMode(Right_Line,INPUT) ;
  state = GO_TO_SHEEP_BLUE;
  GGtimer.begin(handleGG,GGTIME);
  doorStepper.setMaxSpeed(1000);
  doorStepper.setSpeed(1000);
  Serial.begin(9600);
 // stepper.begin(1, 1);

}
void loop(){
  closeDoor();
  //stepper.rotate(360);
  //Serial.println(analogRead(Left_Line));
  //lineFollow();
  // doorStepper.runSpeed();
  // Serial.println(doorStepper.currentPosition());
  // switch (state) {
  //   case WAITING:
  //     handleWaiting();
  //     break;
  //   case GO_TO_SHEEP_BLUE:
  //     handleGoToBlueSheep();
  //     break;
  //   case GO_TO_SHEEP_RED:
  //     handleGoToRedSheep();

  //     break;
  //   case GG:
  //     handleGG();
  //     break;
  //   default:    // Should never get into an unhandled state
  //     Serial.println("What is this I do not even...");
  // }
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
    closeDoor();
    //INSERT TIMED STOP

}

void handleGoToRedSheep(void){
    digitalWrite(REDLED,HIGH);
    digitalWrite(BLUELED,LOW);
    digitalWrite(WAITINGLED,LOW);
    driveForward(speed_value);
    //openDoor();
    ////closeDoor();
    
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
  Serial.println("fwd");
  digitalWrite(IN1_L, LOW);
  digitalWrite(IN2_L, HIGH);
  digitalWrite(IN3_R, HIGH);
  digitalWrite(IN4_R, LOW);
  analogWrite(ENA_L, 600);
  analogWrite(ENB_R, 600);
  //delay(3000);
  //stopDriving();
  //delay(5000);  


}

void driveBackward(int speed){ // moves robot backward
  Serial.println("backwards");
  digitalWrite(IN2_L, HIGH);
  digitalWrite(IN1_L, LOW);
  digitalWrite(IN4_R, HIGH);
  digitalWrite(IN3_R, LOW);
  analogWrite(ENA_L, speed);
  analogWrite(ENB_R, speed);
}

void stopDriving(void){ // stops driving motors
  Serial.println("stop");
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
  digitalWrite(IN1_L, HIGH);
  digitalWrite(IN2_L, LOW);
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
  //Serial.println("open door");
   //doorStepper.moveTo(-100);
   //doorStepper.setSpeed(-600); 
   //doorStepper.run();
   
  doorStepper.moveTo(-800); 
  doorStepper.setSpeed(-600); 
  //doorStepper.run();
  
  //while (doorStepper.currentPosition() != doorStepper.targetPosition()) { 
    doorStepper.runSpeedToPosition();
Serial.println("yo");
  //  }

}

void closeDoor(void){

  doorStepper.moveTo(800); 
  doorStepper.setSpeed(600); 
  
 // while (doorStepper.currentPosition() != doorStepper.targetPosition()) { 
  doorStepper.runSpeedToPosition();
      Serial.println(doorStepper.currentPosition());
   // }

}

bool OnLine(int line){
  if (analogRead(line) == 1)
    return true;
  else
    return false;
}




