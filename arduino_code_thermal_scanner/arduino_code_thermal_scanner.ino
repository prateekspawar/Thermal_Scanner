#include <SparkFunMLX90614.h>
#include <Servo.h>
#include <Wire.h> 
//define the servo motors,IR sensors and thermal sensor
Servo entryservo;
Servo exitservo;
IRTherm therm;


#define buzzer 2 // pin number for buzzer relay
#define IR1 3 //set pin for IR proximity 1
#define IR2 4 //set pin for IR proximity 1
#define IR3 5 //set pin for IR proximity 1
#define temp_limit 38.5 //limit for temperature
#define dir1 7 //direction 1 pin of motor driver
#define dir2 8  //direction 2 pin of motor driver
#define pwmpin 6 //pwm pin of motor driver
#define pwm 255 //pwm (Speed) of motor (0-255)


void setup() {
  Serial.begin(9600); 
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  therm.begin(); 
  therm.setUnit(TEMP_C); 
  entryservo.attach(9); //pin for entryservo
  exitservo.attach(11);// pin for exit servo
  entryservo.write(0); //initial closed position
  exitservo.write(0);//initial closed position
  
  digitalWrite(buzzer,0);//initialize buzzer as off
  digitalWrite(dir1,0);// initialize both directions as low to break the motor
  digitalWrite(dir2,0);

  analogWrite(pwmpin,pwm) ; // set speed of motor 
}

void loop() {
float  temperature; 
if(digitalRead(IR1)) //proximity to IR1
{
  entryservo.write(90); // open entry barrier
  
  if(digitalRead(IR2))// check proximity to IR2
  {
    entryservo.write(0); //close entry barrier
    long starttime=millis(); //start time counter
    //lets move the support plate down until it detects top of head
    while(digitalRead(IR3)==LOW)
    {
      digitalWrite(dir1,HIGH);
      digitalWrite(dir2,LOW);    
    }
    long endtime=millis();// end time counter
    long downtime=endtime-starttime; //time for which the plate was moving down
    digitalWrite(dir1,LOW);// stop the motor
    if (therm.read()) 
    {
      temperature = (therm.object(),2);
    }
    if(temperature > temp_limit)//Infected
    {
      digitalWrite(buzzer,1);// sound buzzer
      //move the plate upward
      digitalWrite(dir1,LOW);
      digitalWrite(dir2,HIGH);
      delay(downtime);// move the plate to initial position by rotating motor in opposite direction for same amount of time
      exitservo.write(90); //open exit barrier
      delay(1000);// wait for 1 second
      exitservo.write(0);//close the exit barrier
      digitalWrite(buzzer,0); //stop the buzzer       
     }
     else  // Not infected
     {
      //move the plate upward
      digitalWrite(dir1,LOW);
      digitalWrite(dir2,HIGH);
      delay(downtime);// move the plate to initial position by rotating motor in opposite direction for same amount of time
      exitservo.write(90); //open exit barrier
      delay(1000);// wait for 1 second
      exitservo.write(0);//close the exit barrier
     }    
  }
 }
}
