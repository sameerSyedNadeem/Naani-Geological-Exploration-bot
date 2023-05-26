/*Feel free to use this code.
Author: Tom Bonar
Date: 12-11-2013
Analog pin 1 for reading in the analog voltage from the MaxSonar device.
This variable is a constant because the pin will not change throughout execution of this code.*/
#define anPin A0
#define anPin2 A1
#define trig 9
#define echo 8
long anVolt, anVolt2, cm,cm2, duration, distance;

void setup() {
  //This opens up a serial connection to shoot the results back to the PC console
  Serial.begin(9600);
  pinMode(anPin, INPUT);
  pinMode(anPin2,INPUT);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
}

void loop() {
  {
    //Used to read in the analog voltage output that is being sent by the XL-MaxSonar device.
    //Scale factor is (Vcc/1024) per centimeter. A 5V supply yields ~4.9mV/cm for standard range sensors
    anVolt = analogRead(anPin); // this is for the standard range sensors
    anVolt2 = analogRead(anPin2);
  }  
  cm = anVolt;
  cm2 = anVolt2;
  Serial.println(getDistance());
  Serial.print("Sensor 1: ");
  Serial.print(cm);
  Serial.println("cm");
  Serial.print("Sensor 2: ");
  Serial.print(cm2);
  Serial.println("cm");
  delay(500);
}
float getDistance()
{
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  duration = pulseIn(echo,HIGH);
  distance = (duration*(0.033))/2;
  return distance;
}
