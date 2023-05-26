#define echof A0
#define echor A2
#define echol A4
#define echob A5
#define trig 2
//motor one
int enA = 3;
int in1 = 4;
int in2 = 5;
// motor two
int enB = 11;
int in3 = 6;
int in4 = 7;
int drops = 0;
long dur1, dur2, dur3, dur4;
int maxIn = 0;
float dists[4] = {0.0,0.0,0.0,0.0};
bool setDir = 0, pot1 = 0, ini = 0;
int dir = -1;

void setup () {
  Serial.begin(9600);
  pinMode(echof, INPUT);
  pinMode(echor, INPUT);
  pinMode(echol, INPUT);
  pinMode(echob, INPUT);  
  pinMode(trig, OUTPUT);
}


//This section of code is if you want to print the range readings to your computer too remove this from the code put /* before the code section and */ after the code
void printAll(){         
  Serial.print("S1");
  Serial.print(" ");
  Serial.print(dists[0]);
  Serial.print("S2");
  Serial.print(" ");
  Serial.print(dists[1]); 
  Serial.print("S3");
  Serial.print(" ");
  Serial.print(dists[2]);
  Serial.print("S4");
  Serial.print(" ");
  Serial.print(dists[3]);
  Serial.println(" ");
}
void loop () 
{
  printAll();
  setDistances();
  if (!setDir)
  {
    if (wiggly(dists[1],dists[2],4))
    {
      if (dists[0] > dists[3])
        dir = 1;
      else
        dir = 4;
      setDir = 1;
    }
    else if(wiggly(dists[0],dists[3],4))
    {
      if (dists[1] > dists[2])
        dir = 2;
      else
        dir = 3;
      setDir = 1;
    }
    else
    {  
      maxIn = getMaxInd(dists,4);
      do
      {
        turnRight(100);
        setDistances();
      }while(!((dists[0]<(dists[maxIn]))));
      setDir = 1;
      dir = 1;
    }
  }
  if (!ini)
  {
    switch (dir)
    {
      case 2:
      while (!dists[0]<(dists[maxIn]))
      {
        turnRight(100);
        setDistances();
      }
      break;
      case 3:
      while (!dists[0]<(dists[maxIn]))
      {
        turnLeft(100);
        setDistances();
      }
      break;
      case 4:
      while (!dists[0]<(dists[maxIn]))
      {
        turnRight(100);
        setDistances();
      }
      break;
    }
    ini = 1;
  }
  if(!(wiggly(dists[1],17,2)||wiggly(dists[2],17,2)))
  {
    moveForward(100);
  }
  else
  {
    halt(100);
  }
  
}
bool wiggly(float val1,float val2,float range)
{
  return(val1<(val2+range) || val1>(val2-range));
}
int getMaxInd(float* ar,int len)
{
  int maxi = 0;
  for (int i =0;i<len;i++)
  {
    if (ar[i]>ar[maxi])
    {
      maxi = i;
    }
  }
  return maxi;
}
float setDistances()
{
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  dur1 = pulseIn(echof,HIGH);
  dur2 = pulseIn(echor,HIGH);
  dur3 = pulseIn(echol,HIGH);
  dur4 = pulseIn(echob,HIGH);
  dists[0] = (dur1*(0.033))/2;
  dists[1] = (dur2*(0.033))/2;
  dists[2] = (dur3*(0.033))/2;
  dists[3] = (dur4*(0.033))/2;
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
