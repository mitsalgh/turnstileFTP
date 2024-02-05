//==================Pin Setup=================
#define ena 8
#define dir 10
#define pul 11
#define sensor1 A0
#define sensor2 A1
#define pinLed1 3
#define pinLed2 4

//============ data setup======================

int flagIn = 0;  // 
String dataIn = "";

int testOpen =0;
int testExit= 1;

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
    Serial.println("step open :" + String(i));
    delay(100);
  }
  while (digitalRead(sensor2) == 1)
  {
    if (digitalRead(sensor2) == testOpen)
    {
      Serial.println();
      digitalWrite(ena, LOW);
      digitalWrite(dir, LOW);
      for (int i = 0; i <= 200; i++)
      {
        digitalWrite(pul, HIGH);
        delayMicroseconds(speedStepper);
        digitalWrite(pul, LOW);
        delayMicroseconds(speedStepper);
        Serial.println("step :" + String(i));
        delay(100);
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
    Serial.println("step :" + String(i));
    delay(100);
  }
  while (digitalRead(sensor1) == 1)
  {
    if (digitalRead(sensor1) == 0)
    {
      Serial.println();
      digitalWrite(ena, LOW);
      digitalWrite(dir, HIGH);
      for (int i = 0; i <= 200; i++)
      {
        digitalWrite(pul, HIGH);
        delayMicroseconds(speedStepper);
        digitalWrite(pul, LOW);
        delayMicroseconds(speedStepper);
        Serial.println("step :" + String(i));
        delay(100);
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
  pinMode(pinLed1, OUTPUT);
  pinMode(pinLed2, OUTPUT);
  delay(100);
}

void loop()
{ 
//  while(1)
//  {
//   while(1)
//   {
//     Serial.print(digitalRead(sensor1)); Serial.print(digitalRead(sensor2));
//     Serial.println();
//   }
//   //  Serial.println("oke");
//    digitalWrite(pinLed1,HIGH);
//    digitalWrite(pinLed2,LOW);
//    if (Serial.available() > 0)
//     {
//       delay(10);
//       dataIn = Serial.readString();
//       if (dataIn == "o" || dataIn == "O" || dataIn == "o\n" || dataIn == "O\n")
//       {
//         digitalWrite(pinLed1,LOW);
//         digitalWrite(pinLed2,HIGH);
//         delay(2000);
//         // openSwingIn();
//       }
//     }
//  }
  digitalWrite(pinLed1,HIGH);
  digitalWrite(pinLed2,LOW);
  if (digitalRead(sensor1) == 0 && flagIn==0)
  {
    Serial.println("masuk");
    flagIn=1;
    while(flagIn==1)
    {
      if (Serial.available() > 0)
      {
        delay(10);
        dataIn = Serial.readString();
        if (dataIn == "o" || dataIn == "O" || dataIn == "o\n" || dataIn == "O\n")
        {
          Serial.println("masuk oke");
          digitalWrite(pinLed1,LOW);
          digitalWrite(pinLed2,HIGH);
          openSwingIn();
          flagIn=0;
        }
      }
    }
  }
  else if (digitalRead(sensor2) == 0 && flagIn==0)
  {
    Serial.print("Exit");
    flagIn=1;
    while(flagIn==1)
    {
      if (Serial.available() > 0)
      {
        delay(10);
        dataIn = Serial.readString();
        if (dataIn == "o" || dataIn == "O" || dataIn == "o\n" || dataIn == "O\n")
        {
          Serial.println("exit");
          digitalWrite(pinLed1,LOW);
          digitalWrite(pinLed2,HIGH);
          openSwingOut();
          flagIn=0;
        }
      }
    }
  }
}
