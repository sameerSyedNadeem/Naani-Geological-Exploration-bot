/*created by Mostafa Zaghloul with a helper library called RedBot.h you can find 
 * it on this link https://github.com/sparkfun/RedBot 
 *--------------------------- thanks a million --------------------------------- 
 *you can find me on 
 *FaceBook::https://www.facebook.com/mostafa.zaghlol3
 *Twitter::https://twitter.com/Mostafazaghlul
 *to Contact for hardware or software questions 
 */
#include <RedBot.h>
#include <Servo.h>
Servo servobase;
Servo servodrop;
/*------ Coding Line Follower Robot using Arduino----- */
#define S0 4
#define S1 8
#define S2 12
#define S3 13
#define sensorOut 0
int trigPin = 2;
int echoPin = A2;
long duration = 0;
int distance = 0;

RedBotSensor left_sen = RedBotSensor(A1);   

RedBotSensor center_sen = RedBotSensor(A3); 
RedBotSensor right_sen = RedBotSensor(A0);  
int pwm = 100;
int lineStandard = 750;
/*-------defining Outputs------*/
//motor one
int enA = 3;
int in1 = 4;
int in2 = 5;
// motor two
int enB = 11;
int in3 = 6;
int in4 = 7;
int drops = 0;

void setup()
{

//color sensor

  //pinMode(S0, OUTPUT); Direct Vcc
  //pinMode(S1, OUTPUT); Direct GND
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  //pinMode(sensorOut1, INPUT);
  
  // Setting frequency-scaling to 20%
  //digitalWrite(S0,HIGH);
  //digitalWrite(S1,LOW);
  
  servobase.attach(10);
  servodrop.attach(9);
  servobase.write(90);
  servodrop.write(0);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  Serial.begin(9600);
  
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  digitalWrite(0, HIGH);
  digitalWrite(1, HIGH);

  //Ultrasonic
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}
/*
int color(){
    // Setting red filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  
  // Reading the output frequency
  //frequency = pulseIn(sensorOut, LOW);
  //frequency1 = 3;//pulseIn(sensorOut1, LOW);
  // Printing the value on the serial monitor
  Serial.print("R= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.print("  ");
  delay(100);

  Serial.print("R1= ");//printing name
  Serial.print(frequency1);//printing RED color frequency
  Serial.print("  ");
  delay(100);
  
  // Setting Green filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  frequency1 = pulseIn(sensorOut1, LOW);
  // Printing the value on the serial monitor
  Serial.print("G= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.print("  ");
  delay(100);

  Serial.print("G1= ");//printing name
  Serial.print(frequency1);//printing RED color frequency
  Serial.print("  ");
  delay(100);
  
  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  frequency1 = 4;//pulseIn(sensorOut1, LOW);
  // Printing the value on the serial monitor
  Serial.print("B= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.println("  ");
  delay(100);

  Serial.print("B1= ");//printing name
  Serial.print(frequency1);//printing RED color frequency
  Serial.println("  ");
  delay(100);
}*/

/*void getPosition(int front, int back, int right, int left, int color)
{
  
}*/

void drop()
{
  servobase.write(180); //or zero depending upon direction.
  delay(2000);
  if (drops != 1) { servodrop.write(180); }
  else { servodrop.write(-180); }
  drops++;
  delay(2000);
  servobase.write(90); 
  delay(2000);
}

void moveForward(int pwm)
{
  analogWrite(enA, pwm);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enB, pwm);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW); 
  //delay(100); 
}

void stopMoving()
{
  analogWrite(enA, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enB, 0);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  //delay(100);
}
void turnLeft(int pwm)
{
  analogWrite(enA, pwm);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enB, pwm);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void turnRight(int pwm)
{
  analogWrite(enA, pwm);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enB, pwm);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void halt(int pwm)
{
  analogWrite(enA,pwm);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, HIGH);
  analogWrite(enB, pwm);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, HIGH);
}


void loop()
{
  //moveForward(pwm);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  duration = pulseIn(echoPin, HIGH);
  distance = duration/2 * 0.033;
  Serial.println(distance);
 /* Serial.print(left_sen.read());
  Serial.print("\n");  
  Serial.print(center_sen.read());
  Serial.print("\t"); */
//  Serial.print(right_sen.read());
//  Serial.println();
//  moveForward(pwm);
//  front();

/*
 for(int motorValue = 0 ; motorValue <= 255; motorValue +=5){
 moveForward(motorValue);
 Serial.println(motorValue);
 delay(300);      
 }
 for(int motorValue = 255 ; motorValue >= 0; motorValue -=5){
 moveForward(motorValue);
 Serial.println(motorValue);
 delay(300);         
 }*/
 // moveForward(pwm);
  //turnLeft(pwm);
  //turnRight(pwm);
  
  
  /*delay(2000);
  servobase.write(0);
  delay(2000);
  servobase.write(180);*/
  //drop();

  //moveForward(pwm);
/*  if ((left_sen.read() > 500) && (center_sen.read() < 500)) { turnLeft(pwm); }
  else if ((left_sen.read() < 500) && (center_sen.read() > 500)) { turnRight(pwm); }
 // else if ((left_sen.read() > 500) && (center_sen.read() > 500)) { drop(); }
  else { moveForward(pwm); } */
  
  if ((distance > 41)) { turnRight(pwm);Serial.println("Right"); }
  else if (distance < 38) { turnLeft(pwm); Serial.println("Left"); }
 // else if ((left_sen.read() > 500) && (center_sen.read() > 500)) { drop(); }
  else { moveForward(pwm); Serial.println("Forwad");} 
 /*if ((left_sen.read() > 500) && (center_sen.read() < 500)) { turnLeft(pwm); }
  if ((left_sen.read() < 500) && (center_sen.read() > 500)) { moveForward(pwm); }
  if ((left_sen.read() > 500) && (center_sen.read() > 500)) { turnLeft(pwm); }
  if ((left_sen.read() > 500) && (center_sen.read() > 500)) { turnRight(pwm); }
/*  while ((left_sen.read() > 800) && (center_sen.read() > 800)) { turnLeft(pwm); }
  halt(pwm);
  while ((left_sen.read() < 800) && (center_sen.read() > 800)) { turnRight(pwm); }
  halt(pwm);
  while ((left_sen.read() > 800) && (center_sen.read() < 800)) { turnLeft(pwm); }
  halt(pwm);*/
  
  /*turnRight(pwm);
  delay(500);
  halt(pwm);
  delay(1000);
  turnLeft(pwm);
  delay(500);
  halt(pwm);
  delay(1000);*/
  /*
  //data_sheet
  // if on the line drive left and right at the same speed (left is CCW / right is CW)
  if(center_sen.read() > lineStandard)
  {//right
   turnRight(pwm);
  }
  //data_sheet
  // if the line is under the right sensor, adjust relative speeds to turn to the right
  else if(right_sen.read() > lineStandard)
  {//left
  turnLeft(pwm);
  }
  //data_sheet
  // if the line is under the left sensor, adjust relative speeds to turn to the left
  else if(left_sen.read() > lineStandard)
  {//right
  turnRight(pwm);
  }
  //data_sheet
  // if all sensors are on black or up in the air, stop the motors.
  // otherwise, run motors given the control speeds above.
  if((left_sen.read() > lineStandard) && (center_sen.read() > lineStandard) && (right_sen.read() > lineStandard) )
  {
    //stop
    stopMoving();
  }
  else
  {
  //any dir
    moveForward(pwm);
  }*/
  delay(50);  // add a delay to decrease sensitivity.
}

