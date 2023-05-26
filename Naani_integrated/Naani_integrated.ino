#define echof A5
#define echor A2
#define echol A4
#define echob 12
#define trigf 8
#define trigr 2
#define trigl A0
#define trigb 13

#include <RedBot.h>
#include <Servo.h>
Servo servobase;
Servo servodrop;

RedBotSensor left_sen = RedBotSensor(A1);   
RedBotSensor center_sen = RedBotSensor(A3); 
int pwm = 70;
int lineStandard = 750;

//motor one
int enA = 3;
int in1 = 4;
int in2 = 5;
// motor two
int enB = 11;
int in3 = 6;
int in4 = 7;
int drops = 0;
unsigned long dur1, dur2, dur3, dur4;
unsigned long dists[4] = {0, 0, 0, 0}, maxIn = 0;;
bool setDir = 0, pot1 = 0, ini = 0, drop = 0;
int dir = -1;
bool first_dropped = false;
bool bridge_dropped = false;
void setup ()
{
  //servo connections and settings
  servobase.attach(10);
  servodrop.attach(9);
  servobase.write(90); //90 centre, 0 right, 180 left
  servodrop.write(0);
  
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  Serial.begin(9600);
  pinMode(echof, INPUT);
  pinMode(echor, INPUT);
  pinMode(echol, INPUT);
  pinMode(echob, INPUT);
  pinMode(trigf, OUTPUT);
  pinMode(trigr, OUTPUT);
  pinMode(trigl, OUTPUT);
  pinMode(trigb, OUTPUT);
}


void printAll() {
  Serial.print("S1"); //forward
  Serial.print(" ");
  Serial.print(dists[0]);
  Serial.print(" S2"); //right
  Serial.print(" ");
  Serial.print(dists[1]);
  Serial.print(" S3"); //left
  Serial.print(" ");
  Serial.print(dists[2]);
  Serial.print(" S4"); //backward
  Serial.print(" ");
  Serial.print(dists[3]);
  Serial.println(" ");
}
void loop ()
{
  for (int i =0;i<20;i++)
  {
    setDistances();
    delay(150);
  }
  if (!drop) //when first drop has not been set
  {
    printAll();
    if (!setDir)
    {
      if (wiggly(dists[1], dists[2], 4)) // if right and left in range SPECIAL CASE
      {
        if (dists[0] > dists[3]) //if forward greater than back
          dir = 1; //move forward
        else
          dir = 4; //move backward
        setDir = 1; //direction has been set
      }
      else if (wiggly(dists[0], dists[3], 4)) //if front and back in range SPECIAL CASE
      {
        if (dists[1] > dists[2]) //if right greater than  left
          dir = 2; //move left
        else
          dir = 3; //move right
        setDir = 1; //direction has been set
      }
      else //GENERIC CASE
      {
        maxIn = getMaxi(dists, 4); //set the maximum distance
        Serial.println(maxIn);
        while (!wiggly(dists[0],maxIn,15)) //while front doesn't get maximum reading
        {
          setDistances(); //set all distances again
          turnRight(70); //do it slowly
          delay(135);
          halt(100);
          delay(150);
          Serial.println("dash");
          printAll();
        } 
        setDir = 1; //direction has been set
        dir = 1; //move forward
      }
    }
    if (!ini) //if initial = 0 
    {
      switch (dir)
      {
        case 2: //if drection right
          while (!dists[0] < (maxIn)) // while front sensor less than max
          {
            setDistances();
            turnRight(70);
            delay(135);
            halt(100);
            delay(150);
          }
          break;
        case 3: //if direction is left
          while (!dists[0] < (maxIn)) // while front sensor less than max
          {
            setDistances();
            turnLeft(70);
            delay(135);
            halt(100);
            delay(150);
          }
          break;
        case 4: //if direction is back
          while (!dists[0] < (maxIn)) // while front sensor less than max
          {
            setDistances();
            turnRight(60);
            delay(135);
            halt(100);
            delay(150);
          }
          break;
      }
      ini = 1;
    }
    setDistances();
    if (!(wiggly(dists[1], 17, 10) || wiggly(dists[2], 17, 10))) //right left approximation, if close to box at 2,3
    {
      moveForward(50);
    }
    else
    {
      halt(100);
      drop = 1;
    }
    //drop condition
    if (drop) 
    {
      if (!(wiggly(dists[1], 17, 2))) //on right sensor
      {
        dropIt(1);
      }
      if (!(wiggly(dists[2], 17, 2))) //on left sensor
      {
        dropIt(2);
        //check orientation
        if (dists[3] > dists[0]) //back greater than front
        {
          moveForward(60);
          delay(135); //can change
          for (int i =0; i <10; i++)
          {
            turnLeft(70);
          }
          delay(150);
          for (int i =0; i <10; i++)
          {
            moveForward(60);
          }
          
        }
      }
    }
  }
  //MOVE TO BRIDGE NOW 
  /***LFR CODE****/
  if (drop)
  {
    pwm = 100;
    Serial.print(left_sen.read());
    Serial.print(" "); 
    Serial.println(center_sen.read()); 
    
    //get readings from front ultrasonic sensor
    digitalWrite(trigf, LOW);
    delayMicroseconds(2);
    digitalWrite(trigf, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigf, LOW);
    dur1 = pulseIn(echof, HIGH);
    dur1 = (dur1 * (0.033)) / 2;
  
    Serial.print("front sensor reading:");
    Serial.println(dur1);
    //get readings from IR
    while (dur1 < 5 && (dur1 >= 4 || dur1 >= 3))
    {
      dropIt(0);
      stopMoving();
      Serial.println("LAST DROPPED AND STOPPED MOVING");
    }
    if ((left_sen.read() > 500) && (center_sen.read() < 500)) { turnLeft(pwm); }
    else if ((left_sen.read() < 500) && (center_sen.read() > 500)) { turnRight(pwm); }
    else if ((left_sen.read() > 500) && (center_sen.read() > 500)) { Serial.println("HIGH DROPPED"); halt(pwm); dropIt(1); bridge_dropped = true; for(int i = 0; i < 30; i++) {moveForward(pwm);} halt(100); pwm = 60; } //if both high on bridge
    else {moveForward(pwm); } 
  }  
}
bool wiggly(unsigned int val1, unsigned int val2, unsigned int range)
{
  return (val1 < (val2 + range) && val1 > (val2 - range)); //true if in the range
}
int getMaxi(unsigned long* ar, int len)
{
  int maxi = ar[0];
  for (int i = 0; i < len; i++)
  {
    if (ar[i] > maxi)
    {
      maxi = ar[i];
    }
  }
  return maxi;
}
void setDistances()
{
  digitalWrite(trigf, LOW);
  delayMicroseconds(2);
  digitalWrite(trigf, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigf, LOW);
  dur1 = pulseIn(echof, HIGH);

  digitalWrite(trigr, LOW);
  delayMicroseconds(2);
  digitalWrite(trigr, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigr, LOW);
  dur2 = pulseIn(echor, HIGH);
  
  digitalWrite(trigl, LOW);
  delayMicroseconds(2);
  digitalWrite(trigl, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigl, LOW);
  dur3 = pulseIn(echol, HIGH);
  
  digitalWrite(trigb, LOW);
  delayMicroseconds(2);
  digitalWrite(trigb, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigb, LOW);
  dur4 = pulseIn(echob, HIGH);
    
  unsigned int df,dr,dl,db;
  df = (dur1 * (0.033)) / 2;
  dr = (dur2 * (0.033)) / 2;
  dl = (dur3 * (0.033)) / 2;
  db = (dur4 * (0.033)) / 2;
  if (df < 400)
    dists[0] = df;
  if (dr <400)
    dists[1] = dr;
  if (dl <400)
    dists[2] = dl;
  if (db <400)
    dists[3] = db;
}

void dropIt(int dir)
{
  // 0 forward, 1 right, 2 left
  if (dir == 0)
  {
    servodrop.write(180);
  }
  if (dir == 1) //right
  {
    servobase.write(15); //or zero depending upon direction.
    delay(2000);
    if (!first_dropped)
    {
      servodrop.write(180);
      first_dropped = true;
    }
    else
    {
      servodrop.write(-180);
    }
    delay(2000);
    servobase.write(90); 
    delay(2000);
  }
  if (dir == 2) //left
  {
    servobase.write(175); //or zero depending upon direction.
    delay(2000);
    if (!first_dropped)
    {
      servodrop.write(180);
      first_dropped = true;
    }
    else
    {
      servodrop.write(-180);
    }
    delay(2000);
    servobase.write(90); 
    delay(2000);
  }
  
}

void moveForward(int pwm)
{
  analogWrite(enA, pwm-20);
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
  digitalWrite(in4, HIGH);
}

void turnRight(int pwm)
{
  analogWrite(enA, pwm);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enB, pwm);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void halt(int pwm)
{
  analogWrite(enA, pwm);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, HIGH);
  analogWrite(enB, pwm);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, HIGH);
}


