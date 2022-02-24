/*---------------Libraries-----------------------------*/
#include <Arduino.h>
#include <SPI.h>

/*---------------Module Defines-----------------------------*/
#define REDLED 23
#define BLUELED 22
#define WAITINGLED 21
#define IN3 19
#define IN1  18
#define IN4 17
#define IN2 16
#define STEP 15
#define DIR 14
#define TEAMPOTPIN A6
//#define GGTIME 250000000
#define GGTIME 20000000
/*---------------Module Function Prototypes-----------------*/
//handle state functions
void handleWaiting(void);
void handleGoToBlueSheep(void);
void handleGoToRedSheep(void);
void handleGG(void);

//move robot functions
void driveForward(void);
void driveBackward(void);
void stopDriving(void);
void stopAllMotors(void);
void turnRight(void);
void turnLeft(void);
void lineFollow(void);

//additional useful functions
int potRead(int pin);




/*---------------State Definitions--------------------------*/
typedef enum {WAITING, GO_TO_SHEEP_RED, GO_TO_SHEEP_BLUE,GG} States_t;


/*---------------Module Variables---------------------------*/
IntervalTimer GGtimer;
States_t state;
int potval;
int teamcolorval;

/*---------------Main Functions----------------*/

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(BLUELED, OUTPUT);
  pinMode(REDLED, OUTPUT);
  pinMode(WAITINGLED, OUTPUT);
  pinMode(TEAMPOTPIN, INPUT);
  state = WAITING;
  GGtimer.begin(handleGG,GGTIME);

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
    driveForward();
    //INSERT TIMED STOP

}

void handleGoToRedSheep(void){
    digitalWrite(REDLED,HIGH);
    digitalWrite(BLUELED,LOW);
    digitalWrite(WAITINGLED,LOW);
    driveForward();
    //INSERT TIMED STOP
}

void handleGG(void){ // all motors off, all LEDs off
    digitalWrite(REDLED,LOW);
    digitalWrite(BLUELED,LOW);
    digitalWrite(WAITINGLED,LOW);
    stopAllMotors();
    state = GG;
}

void driveForward(void){//moves robot forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void driveBackward(void){ // moves robot backward

}

void stopDriving(void){ // stops driving motors

}

void stopAllMotors(void){ // stops all motors

}

void turnRight(void){ // turns robot right

}

void turnLeft(void){ // turns robot left

}

void lineFollow(void){ // line following

}


