#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

//====================== Pin Setup =================
#define EEPROMDelay 0
#define EEPROMTimeout 1
#define EEPROMArah 3
#define ena 7       // pin ENA
#define dir 8       // pin Direction TB6600
#define pul 9       // Pin pulse di TB6600
#define sensor1 A0  // Sensor IR IN
#define sensor2 A1  // Sensor IR EXIT
#define sensorSteady A2
#define pinLed1 3
#define pinLed2 4
#define timeCount 9
#define pinLeft 6
#define pinUp 11
#define pinRight 10
#define pinDown 7
#define pinOk 12

//============================ data setup ======================================

int flagIn = 0;
String dataIn = "";
int flagExit = 0;
int flagMasukMenu = 0;

int timeout = 0;
int nilaiDelay = 0;
int nilaiArah = 0;

int count = timeout;
int flagTimeout = timeout;

const int stepOpen = 750;   //step untuk stepper motor untuk membuka gate
const int stepClose = 750;  //step untuk Stepper motor untuk menutup kembali

unsigned long lastDebounceTime = 0;  //variabel untuk debounce
unsigned long debounceDelay = 50;    //waktu debounce dalam milidetik
unsigned long prevMillis = 0;        //variabel untuk menyimpan waktu sebelumnya
unsigned long interval = 500;        //interval waktu untuk debounce sensor

unsigned long previousMillisTimeOut = 0;
unsigned long intervalTimeOut = timeout * 100;

//============ Speed Setup NEMA ===============
#define speedStepper 200  //step untuk buka dan tutup setting TB6600 di 400 step
//=============================================
void (*resetFunc)(void) = 0; //to restart arduino

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
    // Serial.println("waiting " + String (count));
    // delay(2);
    if (digitalRead(sensor2) == LOW || flagTimeout == 1) {
      digitalWrite(ena, LOW);
      digitalWrite(dir, LOW);
      for (int i = 0; i <= stepClose; i++) {
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
  delay(5);                             //delay untuk holding
  while (digitalRead(sensor1) == HIGH)  // perulangan untuk deteksi user lewat arah exit dan menutup gate kembali
  {
    checkTimeout();
    Serial.println("waiting " + String(count));
    // delay(2); //   Serial.println("Waiting");elay untuk trigeer proses
    if (digitalRead(sensor1) == LOW || flagTimeout == 1) {
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
      delay(200);  // delay diganti jadi 10ms
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
  flagTimeout = 0;
  count = timeout;
  flagMasukMenu = 0;
}

void checkTimeout() {
  unsigned long currentMillis = millis();

  if ((unsigned long)(currentMillis - previousMillisTimeOut) >= intervalTimeOut) {
    count--;
    Serial.println("Timeout : " + String(count));
    if (count == 0) {
      flagTimeout = 1;
    } else {
      // Serial.print(count);
    }
    previousMillisTimeOut = millis();
  }
}

void setup() {
  Serial.begin(9600);
  bacaEEPROM(EEPROMDelay, EEPROMTimeout);
  Serial.println("Timeout : " + String(timeout) + "Delay(ms) : " + String(nilaiDelay));
  pinMode(pinUp, INPUT_PULLUP);
  pinMode(pinDown, INPUT_PULLUP);
  pinMode(pinRight, INPUT_PULLUP);
  pinMode(pinLeft, INPUT_PULLUP);
  pinMode(pinOk, INPUT_PULLUP);
  lcd.init();  // initialize the lcd
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  pinMode(ena, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(pul, OUTPUT);
  pinMode(sensor1, INPUT_PULLUP);
  pinMode(sensor2, INPUT_PULLUP);
  pinMode(pinLed1, OUTPUT);
  pinMode(pinLed2, OUTPUT);
  digitalWrite(ena, HIGH);
  lcd.setCursor(0, 1);
  lcd.print("System Ready");
  delay(100);
  // simpanEEPROM (150,100,EEPROMDelay,EEPROMTimeout);
  bacaArah(EEPROMArah);
  bacaEEPROM(EEPROMDelay, EEPROMTimeout);
  Serial.println("Delay : " + String(nilaiDelay) + " Timeout : " + String(timeout) + " Mode = " + String(nilaiArah));
  delay(10);
  intervalTimeOut = timeout * 100;
  count = timeout;
  flagTimeout = timeout;
  if(nilaiArah==3)
  {
    lcd.setCursor(0, 2);
     lcd.print("System Not Activated");
  }

  Serial.println(intervalTimeOut);
}

void loop() {
  if (nilaiArah==3)
  {
    digitalWrite(pinLed1, HIGH);
    digitalWrite(pinLed2, LOW);
  }
  if (nilaiArah !=3)
  {
    digitalWrite(pinLed1, LOW);
    digitalWrite(pinLed2, HIGH);
  }
  unsigned long currentMillis = millis();        //mendapatkan waktu saat ini
  if (currentMillis - prevMillis >= interval) {  //jika interval waktu sudah terlewati
    prevMillis = currentMillis;                  //memperbarui waktu sebelumnya

    if (digitalRead(sensor1) == LOW)  // deteksi untuk arah masuk
    {
      if (nilaiArah == 0 || nilaiArah == 1) {
        Serial.println("in");
        flagIn = 1;
        while (flagIn == 1) {
          if (Serial.available() > 0) {
            delay(10);
            dataIn = Serial.readString();
            if (dataIn == "o" || dataIn == "O" || dataIn == "o\n" || dataIn == "O\n") {
              digitalWrite(pinLed1, HIGH);
              digitalWrite(pinLed2, LOW);
              // Serial.println("masuk");
              openSwingIn();  // proses open arah Enter
            }
          }
        }
      } else {
        Serial.println("Salah Arah Masuk");
        for (int x = 0; x <= 10; x++) {
          
          digitalWrite(pinLed1, HIGH);
          digitalWrite(pinLed2, LOW);
          delay(100);
          digitalWrite(pinLed1, LOW);
          digitalWrite(pinLed2, LOW);
          delay(100);
        }
      }
    } else if (digitalRead(sensor2) == LOW)  //deteksi user untuk arah keluar
    {
      if (nilaiArah == 0 || nilaiArah == 2) {
        Serial.println("exit");
        flagExit = 1;
        while (flagExit == 1) {
          if (Serial.available() > 0) {
            delay(10);
            dataIn = Serial.readString();
            if (dataIn == "o" || dataIn == "O" || dataIn == "o\n" || dataIn == "O\n") {
              digitalWrite(pinLed1, HIGH);
              digitalWrite(pinLed2, LOW);
              openSwingOut();  // proses open arah exit
            }
          }
        }
      } else {
        Serial.println("Salah Arah keluar");
        for (int x = 0; x <= 5; x++) {
          
          digitalWrite(pinLed1, HIGH);
          digitalWrite(pinLed2, LOW);
          delay(100);
          digitalWrite(pinLed1, LOW);
          digitalWrite(pinLed2, LOW);
          delay(100);
          
        }
      }
    } else if (digitalRead(pinOk) == LOW && digitalRead(pinLeft) == LOW) {
      // flagMasukMenu=1;
      Serial.print("masuk");
      while (1) {
        menu();
      }
    }
  }
}
