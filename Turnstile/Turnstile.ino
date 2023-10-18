//==================Pin Setup=================
#define ena 7 // pin ENA
#define dir 8 // pin Direction TB6600
#define pul 9 // Pin pulse di TB6600
#define sensor1 A4 // Sensor IR IN
#define sensor2 A1 // Sensor IR EXIT

//============ data setup======================

int flagIn = 0;
String dataIn = "";
int flagExit = 0;

//============ Speed Setup NEMA ===============
#define speedStepper 200 //step untuk buka dan tutup setting TB6600 di 400 step
//=============================================

void steadyFlip() // fungsi untuk steady kan flip untuk posisi closed
{
  digitalWrite(ena, LOW); // aktifator ENA
  digitalWrite(dir, HIGH);
  for (int i = 0; i <= 200; i++)
  {
    digitalWrite(pul, HIGH);
    delayMicroseconds(speedStepper);
    digitalWrite(pul, LOW);
    delayMicroseconds(speedStepper);
  }
  digitalWrite(ena, HIGH); //nonaktifkan ENA
}

void openSwingIn() // fungsi untuk stepper untuk proses memnula gate
{
  digitalWrite(ena, LOW);//Aktifator ENA
  digitalWrite(dir, HIGH); // mengatur direction untuk open set di HIGH
  for (int i = 0; i <= 200; i++)//perulangan untuk per step buka
  {
    digitalWrite(pul, HIGH);
    delayMicroseconds(speedStepper);
    digitalWrite(pul, LOW);
    delayMicroseconds(speedStepper);
  }
  while (digitalRead(sensor2) == 1)// perulangan dan check untuk mode open orang masuk lalu menutup pintu
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
      digitalWrite(ena, HIGH); //nonakatifkan ENA
      delay(100);
      break;
    }
  }
}
void openSwingOut()
{
  digitalWrite(ena, LOW);//aktifator ENA
  digitalWrite(dir, LOW);// mengatur direction untuk gate ke arah exit set up LOW
  for (int i = 0; i <= 200; i++)// perulangan stepper untuk membuka gate ke arah Exit
  {
    digitalWrite(pul, HIGH);
    delayMicroseconds(speedStepper);
    digitalWrite(pul, LOW);
    delayMicroseconds(speedStepper);
  }
  while (digitalRead(sensor1) == 1)// perulangan untuk deteksi user lewat arah exit dan menutup gate kembali
  {
    if (digitalRead(sensor1) == 0)
    {
      digitalWrite(ena, LOW);
      digitalWrite(dir, HIGH);
      for (int i = 0; i <= 200; i++)// fungsi stepper
      {
        digitalWrite(pul, HIGH);
        delayMicroseconds(speedStepper);
        digitalWrite(pul, LOW);
        delayMicroseconds(speedStepper);

      }
      digitalWrite(ena, HIGH);
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
  //  while(1)
  //  {
  //    Serial.println(digitalRead(sensor2));
  //    if(digitalRead(sensor2)==0)
  //    {
  //      Serial.println("masuk");
  //      delay(3000);
  //    }
  //  }
  if (digitalRead(sensor1) == 0)// deteksi untuk arah masuk
  {
    Serial.println("in");
    flagIn = 1;
    while (flagIn == 1)
    {
      if (Serial.available() > 0)
      {
        delay(10);
        dataIn = Serial.readString();
        if (dataIn == "o" || dataIn == "O" || dataIn == "o\n" || dataIn == "O\n")
        {
          Serial.println("masuk");
          openSwingIn(); // proses open arah Enter
        }
      }
    }
  }
  else if (digitalRead(sensor2) == 0)//deteksi user untuk arah keluar
  {
    Serial.println("exit");
    flagExit = 1;
    while (flagExit == 1)
    {
      if (Serial.available() > 0)
      {
        delay(10);
        dataIn = Serial.readString();
        if (dataIn == "o" || dataIn == "O" || dataIn == "o\n" || dataIn == "O\n")
        {
          Serial.println("oke");
          flagExit = 0;
          openSwingOut();// proses open arah exit
        }
      }
    }
  }
}
