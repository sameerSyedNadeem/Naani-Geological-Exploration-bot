#define echof A5
#define echor A2
#define echol A4
#define echob 12
#define trigf 8
#define trigr 2
#define trigl A0
#define trigb 13
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
unsigned int dists[4] = {0.0, 0.0, 0.0, 0.0}, maxIn = 0;;
bool setDir = 0, pot1 = 0, ini = 0, drop = 0;
int dir = -1;

void setup ()
{
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
  Serial.print("S1");
  Serial.print(" ");
  Serial.print(dists[0]);
  Serial.print(" S2");
  Serial.print(" ");
  Serial.print(dists[1]);
  Serial.print(" S3");
  Serial.print(" ");
  Serial.print(dists[2]);
  Serial.print(" S4");
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
  if (!drop)
  {
    printAll();
    if (!setDir)
    {
      if (wiggly(dists[1], dists[2], 4))
      {
        if (dists[0] > dists[3])
          dir = 1;
        else
          dir = 4;
        setDir = 1;
      }
      else if (wiggly(dists[0], dists[3], 4))
      {
        if (dists[1] > dists[2])
          dir = 2;
        else
          dir = 3;
        setDir = 1;
      }
      else
      {
        maxIn = getMaxi(dists, 4);
        Serial.println(maxIn);
        while (!(wiggly(dists[0],maxIn,10)))
        {
          setDistances();
          turnRight(70);
          delay(135);
          halt(100);
          delay(150);
          Serial.println("dash");
          printAll();
        } 
        setDir = 1;
        dir = 1;
      }
    }
    if (!ini)
    {
      switch (dir)
      {
        case 2:
          while (!dists[0] < (maxIn))
          {
            setDistances();
            turnRight(70);
            delay(100);
            halt(100);
            delay(150);
          }
          break;
        case 3:
          while (!dists[0] < (maxIn))
          {
            setDistances();
            turnLeft(70);
            delay(150);
            halt(100);
            delay(150);
          }
          break;
        case 4:
          while (!dists[0] < (maxIn))
          {
            setDistances();
            turnRight(60);
            delay(150);
            halt(100);
            delay(150);
          }
          break;
      }
      ini = 1;
    }
    if (!(wiggly(dists[1], 17, 2) || wiggly(dists[2], 17, 2)))
    {
      moveForward(80);
//      delay(150);
//      halt(100);
//      delay(150);
    }
    else
    {
      halt(100);
      drop = 1;
    }
  }

}
bool wiggly(unsigned int val1,unsigned int val2,unsigned int range)
{
  return (val1 < (val2 + range) && val1 > (val2 - range));
}
unsigned int getMaxi(unsigned int* ar,unsigned int len)
{
  unsigned int maxi = ar[0];
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
void moveForward(int pwm)
{
  analogWrite(enA-25, pwm);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enB+25, pwm);
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
