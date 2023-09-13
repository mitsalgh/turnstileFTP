//==================Pin Setup=================
#define ena 8
#define dir 10
#define pul 11
#define sensor1 A0
#define sensor2 A1

//============ data setup======================

int flagIn = 0;
String dataIn = "";

void openSwingIn()
{
  
}
void openSwingOut()
{
  
}

void setup() {
  pinMode(ena, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(pul, OUTPUT);
  pinMode(sensor1, INPUT_PULLUP);
  pinMode(sensor2, INPUT_PULLUP);
  digitalWrite(ena,HIGH);
  delay(100);
}

void loop()
{
  if (digitalRead(sensor1) == 1)
  {
    if (Serial.available() > 0)
    {
      delay(10);
      dataIn= Serial.readString();
      if(dataIn == "o" || dataIn== "O" || dataIn== "o\n" || dataIn== "O\n")
      {
        openSwingIn(); 
      }
    }
  }
  else if (digitalRead(sensor2) == 1)
  {
    if (Serial.available() > 0)
    {
      delay(10);
      dataIn= Serial.readString();
      if(dataIn == "o" || dataIn== "O" || dataIn== "o\n" || dataIn== "O\n")
      {
        openSwingOut(); 
      }
    }
  }


}
