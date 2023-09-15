//==================Pin Setup=================
#define ena 8
#define dir 10
#define pul 11
#define sensor1 A0
#define sensor2 A1

//============ data setup======================

int flagIn = 0;
String dataIn = "";

//============ Speed Setup NEMA ===============
#define speedStepper 200
//=============================================

void steadyFlip()
{
  digitalWrite(ena, LOW);
  digitalWrite(dir, HIGH);
  for (int i = 0; i <= 200; i++)
  {
    digitalWrite(pul, HIGH);
    delayMicroseconds(speedStepper);
    digitalWrite(pul, LOW);
    delayMicroseconds(speedStepper);
  }
  digitalWrite(ena, HIGH);
}

void openSwingIn()
{
  digitalWrite(ena, LOW);
  digitalWrite(dir, HIGH);
  for (int i = 0; i <= 200; i++)
  {
    digitalWrite(pul, HIGH);
    delayMicroseconds(speedStepper);
    digitalWrite(pul, LOW);
    delayMicroseconds(speedStepper);
  }
  
  while (digitalRead(sensor2) == 1)
  {
    if (digitalRead(sensor2) == 0)
    {
      digitalWrite(ena, LOW);
      digitalWrite(dir, LOW);
      for (int i = 0; i <= 200; i++)
      {
        digitalWrite(pul, HIGH);
        delayMicroseconds(speedStepper);
        digitalWrite(pul, LOW);
        delayMicroseconds(speedStepper);
        
      }
      digitalWrite(ena,HIGH);
      delay(100);
      break;
    }
  }
}
void openSwingOut()
{
  digitalWrite(ena, LOW);
  digitalWrite(dir, LOW);
  for (int i = 0; i <= 200; i++)
  {
    digitalWrite(pul, HIGH);
    delayMicroseconds(speedStepper);
    digitalWrite(pul, LOW);
    delayMicroseconds(speedStepper);
    
  }
  while (digitalRead(sensor1) == 1)
  {
    if (digitalRead(sensor1) == 0)
    {
      digitalWrite(ena, LOW);
      digitalWrite(dir, HIGH);
      for (int i = 0; i <= 200; i++)
      {
        digitalWrite(pul, HIGH);
        delayMicroseconds(speedStepper);
        digitalWrite(pul, LOW);
        delayMicroseconds(speedStepper);
        
      }
      digitalWrite(ena,HIGH);
      delay(100);
      break;
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(ena, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(pul, OUTPUT);
  pinMode(sensor1, INPUT_PULLUP);
  pinMode(sensor2, INPUT_PULLUP);
  digitalWrite(ena, HIGH);
  delay(100);
}

void loop()
{
  if (digitalRead(sensor1) == 0)
  {
    if (Serial.available() > 0)
    {
      delay(10);
      dataIn = Serial.readString();
      if (dataIn == "o" || dataIn == "O" || dataIn == "o\n" || dataIn == "O\n")
      {
        Serial.println("masuk");
        openSwingIn();
      }
    }
  }
  else if (digitalRead(sensor2) == 1)
  {
    if (Serial.available() > 0)
    {
      delay(10);
      dataIn = Serial.readString();
      if (dataIn == "o" || dataIn == "O" || dataIn == "o\n" || dataIn == "O\n")
      {
        Serial.println("oke");
        openSwingOut();
      }
    }
  }
}
