//==================Pin Setup=================
#define ena 7       // pin ENA
#define dir 8       // pin Direction TB6600
#define pul 9       // Pin pulse di TB6600
#define sensor1 A0  // Sensor IR IN
#define sensor2 A1  // Sensor IR EXIT
#define sensorSteady A2 
#define pinLed1 3
#define pinLed2 4
#define timeCount 5

//============ data setup======================

int flagIn = 0;
String dataIn = "";
int flagExit = 0;
int count=timeCount;
int flagTimeout=timeCount;

const int stepOpen = 750; //step untuk stepper motor untuk membuka gate 
const int stepClose = 750; //step untuk Stepper motor untuk menutup kembali
const int timeoutDelay=10;

unsigned long lastDebounceTime = 0;  //variabel untuk debounce
unsigned long debounceDelay = 50;    //waktu debounce dalam milidetik
unsigned long prevMillis = 0;        //variabel untuk menyimpan waktu sebelumnya
unsigned long interval = 500;        //interval waktu untuk debounce sensor

unsigned long previousMillisTimeOut=0;
unsigned long intervalTimeOut=1000;


//============ Speed Setup NEMA ===============
#define speedStepper 200  //step untuk buka dan tutup setting TB6600 di 400 step
//=============================================

void steadyFlip()  // fungsi untuk steady kan flip untuk posisi closed
{
  digitalWrite(ena, LOW);  // aktifator ENA
  digitalWrite(dir, HIGH);
  for (int i = 0; i <= 200; i++) {
    digitalWrite(pul, HIGH);
    delayMicroseconds(speedStepper);
    digitalWrite(pul, LOW);
    delayMicroseconds(speedStepper);
  }
  digitalWrite(ena, HIGH);  //nonaktifkan ENA
}

void openSwingIn()  // fungsi untuk stepper untuk proses memnula gate
{
  
  digitalWrite(ena, LOW);              //Aktifator ENA
  digitalWrite(dir, HIGH);             // mengatur direction untuk open set di HIGH
  for (int i = 0; i <= stepOpen; i++)  //perulangan untuk per step buka (open proses)
  {
    // Serial.println("step open :" + String (i));
    digitalWrite(pul, HIGH);
    delayMicroseconds(speedStepper);
    digitalWrite(pul, LOW);
    delayMicroseconds(speedStepper);
    // delay(10);
  }
  Serial.println("opened");
  delay(5);
  while (digitalRead(sensor2) == HIGH)  // perulangan dan check untuk mode open orang masuk lalu menutup pintu
  {
    checkTimeout();
    Serial.println("waiting " + String (count));
    delay(2);
    if (digitalRead(sensor2) == LOW || flagTimeout==1) 
    {
      digitalWrite(ena, LOW);
      digitalWrite(dir, LOW);
      for (int i = 0; i <= stepClose; i++) 
      {
        // Serial.println("step close :" + String (i));
        digitalWrite(pul, HIGH);
        delayMicroseconds(speedStepper);
        digitalWrite(pul, LOW);
        delayMicroseconds(speedStepper);
        // delay(10);
      }
      Serial.println("closed");
      digitalWrite(ena, HIGH);  //nonakatifkan ENA
      delay(400);
      resetAll();
      loop();
      break;
    }
  }
}
void openSwingOut() {
  digitalWrite(ena, LOW);              //aktifator ENA
  digitalWrite(dir, LOW);              // mengatur direction untuk gate ke arah exit set up LOW
  for (int i = 0; i <= stepOpen; i++)  // perulangan stepper untuk membuka gate ke arah Exit
  {
    // Serial.println("step open :" + String (i));
    digitalWrite(pul, HIGH);
    delayMicroseconds(speedStepper);
    digitalWrite(pul, LOW);
    delayMicroseconds(speedStepper);
    // delay(10);
    // Serial.print(".");
  }
  Serial.println("opened");
  delay(5); //delay untuk holding
  while (digitalRead(sensor1) == HIGH)  // perulangan untuk deteksi user lewat arah exit dan menutup gate kembali
  {
    checkTimeout();
    Serial.println("waiting " + String (count));
    delay(2); //   Serial.println("Waiting");elay untuk trigeer proses
    if (digitalRead(sensor1) == LOW || flagTimeout==1) 
    {
      digitalWrite(ena, LOW);
      digitalWrite(dir, HIGH);
      for (int i = 0; i <= stepClose; i++)  // fungsi stepper untuk menutup turnstile
      {
        // Serial.println("step close :" + String (i));
        digitalWrite(pul, HIGH);
        delayMicroseconds(speedStepper);
        digitalWrite(pul, LOW);
        delayMicroseconds(speedStepper);
        // delay(10);
      }
      digitalWrite(ena, HIGH);
      Serial.println("closed");
      delay(200); // delay diganti jadi 10ms
      resetAll();
      loop();
      break;
    }
  }
}
void resetAll() {
  flagIn = 0;
  dataIn = "";
  flagExit = 0;
  flagTimeout=0;
  count=timeCount;
}

void checkTimeout()
{
  unsigned long currentMillis = millis();

  if ((unsigned long)(currentMillis - previousMillisTimeOut) >= intervalTimeOut) 
  {
    count--;
    if(count==0)
    {
      flagTimeout=1;
    }
    else
    {
      // Serial.print(count);
    }
    previousMillisTimeOut= millis();
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(ena, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(pul, OUTPUT);
  pinMode(sensor1, INPUT_PULLUP);
  pinMode(sensor2, INPUT_PULLUP);
  pinMode(pinLed1, OUTPUT);
  pinMode(pinLed2,OUTPUT);
  digitalWrite(ena, HIGH);
  delay(100);
}


void loop() {
  
  digitalWrite(pinLed1,HIGH);
  digitalWrite(pinLed2,LOW);
  unsigned long currentMillis = millis();        //mendapatkan waktu saat ini
  if (currentMillis - prevMillis >= interval) {  //jika interval waktu sudah terlewati
    prevMillis = currentMillis;                  //memperbarui waktu sebelumnya

    if (digitalRead(sensor1) == LOW)  // deteksi untuk arah masuk
    {
      Serial.println("in");
      flagIn = 1;
      while (flagIn == 1) 
      {
        if (Serial.available() > 0) {
          delay(10);
          dataIn = Serial.readString();
          if (dataIn == "o" || dataIn == "O" || dataIn == "o\n" || dataIn == "O\n") 
          {
            digitalWrite(pinLed1,LOW);
            digitalWrite(pinLed2,HIGH);
            // Serial.println("masuk");
            openSwingIn();  // proses open arah Enter
          }
        }
      }
    } 
    else if (digitalRead(sensor2) == LOW)  //deteksi user untuk arah keluar
    {
      Serial.println("exit");

      flagExit = 1;
      while (flagExit == 1) {
        if (Serial.available() > 0) 
        {
          delay(10);
          dataIn = Serial.readString();
          if (dataIn == "o" || dataIn == "O" || dataIn == "o\n" || dataIn == "O\n") 
          {
            digitalWrite(pinLed1,LOW);
            digitalWrite(pinLed2,HIGH);
            openSwingOut();  // proses open arah exit
          }
        }
      }
    }
  }
}
