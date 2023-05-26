
#define trig 8
#define echo1 9
#define echo2 10
#define echo3 11
#define echo4 12
long duration;
int distance;
#define PLMF 7 //left enable
#define LMFF 6   // front left motor, HIGH to move forward
#define LMFB 5   // front left motor, HIGH to move backward
#define PRMF 4  //right enable
#define RMFB 3    // front right motor, HIGH to move forward
#define RMFF 2    // front right motor, HIGH to move backward
void setup() 
{
  Serial.begin(9600);
  pinMode(trig,OUTPUT);
  pinMode(echo1,INPUT);
  pinMode(PLMF, OUTPUT); //enable
  pinMode(PRMF, OUTPUT); //enable
  pinMode(LMFF, OUTPUT);
  pinMode(LMFB, OUTPUT);
  pinMode(RMFF, OUTPUT);
  pinMode(RMFB, OUTPUT);
  
}

void loop() 
{
  Serial.println(getDistance());
  while(getDistance() < 12)
  {
    Serial.println(getDistance());
    //stopMoving();
    //delay(1000);
    turnRight(100);
  }
  moveForward(200);
  delay(100);
}

void moveForward(int pwm)
{
  analogWrite(PLMF, pwm);
  digitalWrite(LMFF, HIGH);
  digitalWrite(LMFB, LOW);
  analogWrite(PRMF, pwm);
  digitalWrite(RMFF, HIGH);
  digitalWrite(RMFB, LOW); 
  delay(100); 
}

void stopMoving()
{
  analogWrite(PLMF, 0);
  digitalWrite(LMFF, LOW);
  digitalWrite(LMFB, LOW);
  analogWrite(PRMF, 0);
  digitalWrite(RMFF, LOW);
  digitalWrite(RMFB, LOW);
  delay(100);
}
void turnRight(int pwm)
{
  analogWrite(PLMF, pwm);
  digitalWrite(LMFF, HIGH);
  digitalWrite(LMFB, LOW);
  analogWrite(PRMF, pwm);
  digitalWrite(RMFF, LOW);
  digitalWrite(RMFB, HIGH);
}

void turnLeft(int pwm)
{
  analogWrite(PLMF, pwm);
  digitalWrite(LMFF, LOW);
  digitalWrite(LMFB, HIGH);
  analogWrite(PRMF, pwm);
  digitalWrite(RMFF, HIGH);
  digitalWrite(RMFB, LOW);
}
void reverse(int pwm)
{
  analogWrite(PLMF, pwm);
  digitalWrite(LMFF, LOW);
  digitalWrite(LMFB, HIGH);
  analogWrite(PRMF, pwm);
  digitalWrite(RMFF, LOW);
  digitalWrite(RMFB, HIGH);
}
float getDistance()
{
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  duration = pulseIn(echo1,HIGH);
  distance = (duration*(0.033))/2;
  return distance;
}

