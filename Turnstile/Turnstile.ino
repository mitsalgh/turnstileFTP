//==================Pin Setup=================
#define ena 7       // pin ENA
#define dir 8       // pin Direction TB6600
#define pul 9       // Pin pulse di TB6600
#define sensor1 A0  // Sensor IR IN
#define sensor2 A1  // Sensor IR EXIT

//============ data setup======================

int flagIn = 0;
String dataIn = "";
int flagExit = 0;

const int stepOpen = 200;
const int stepClose = 200;

unsigned long lastDebounceTime = 0;  //variabel untuk debounce
unsigned long debounceDelay = 50;    //waktu debounce dalam milidetik
unsigned long prevMillis = 0;        //variabel untuk menyimpan waktu sebelumnya
unsigned long interval = 350;        //interval waktu untuk debounce sensor

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
    digitalWrite(pul, HIGH);
    delayMicroseconds(speedStepper);
    digitalWrite(pul, LOW);
    delayMicroseconds(speedStepper);
  }
  Serial.println("opened");
  delay(5);
  while (digitalRead(sensor2) == HIGH)  // perulangan dan check untuk mode open orang masuk lalu menutup pintu
  {
    delay(5);
    if (digitalRead(sensor2) == LOW) {
      digitalWrite(ena, LOW);
      digitalWrite(dir, LOW);
      for (int i = 0; i <= stepClose; i++) {
        digitalWrite(pul, HIGH);
        delayMicroseconds(speedStepper);
        digitalWrite(pul, LOW);
        delayMicroseconds(speedStepper);
      }
      Serial.println("closed");
      digitalWrite(ena, HIGH);  //nonakatifkan ENA
      delay(2000);
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
    digitalWrite(pul, HIGH);
    delayMicroseconds(speedStepper);
    digitalWrite(pul, LOW);
    delayMicroseconds(speedStepper);
    // Serial.print(".");
  }
  Serial.println("opened");
  delay(5); //delay untuk holding
  while (digitalRead(sensor1) == HIGH)  // perulangan untuk deteksi user lewat arah exit dan menutup gate kembali
  {
    // Serial.println("Waiting");
    delay(5); //delay untuk trigeer proses
    if (digitalRead(sensor1) == LOW) {
      digitalWrite(ena, LOW);
      digitalWrite(dir, HIGH);
      for (int i = 0; i <= stepClose; i++)  // fungsi stepper untuk menutup turnstile
      {
        digitalWrite(pul, HIGH);
        delayMicroseconds(speedStepper);
        digitalWrite(pul, LOW);
        delayMicroseconds(speedStepper);
      }
      digitalWrite(ena, HIGH);
      Serial.println("closed");
      delay(2000); // delay diganti jadi 10ms
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

void loop() {

  unsigned long currentMillis = millis();        //mendapatkan waktu saat ini
  if (currentMillis - prevMillis >= interval) {  //jika interval waktu sudah terlewati
    prevMillis = currentMillis;                  //memperbarui waktu sebelumnya

    if (digitalRead(sensor1) == LOW)  // deteksi untuk arah masuk
    {
      Serial.println("in");
      flagIn = 1;
      while (flagIn == 1) {
        if (Serial.available() > 0) {
          delay(10);
          dataIn = Serial.readString();
          if (dataIn == "o" || dataIn == "O" || dataIn == "o\n" || dataIn == "O\n") {
            // Serial.println("masuk");
            openSwingIn();  // proses open arah Enter
          }
        }
      }
    } else if (digitalRead(sensor2) == LOW)  //deteksi user untuk arah keluar
    {
      Serial.println("exit");
      flagExit = 1;
      while (flagExit == 1) {
        if (Serial.available() > 0) {
          delay(10);
          dataIn = Serial.readString();
          if (dataIn == "o" || dataIn == "O" || dataIn == "o\n" || dataIn == "O\n") {
            openSwingOut();  // proses open arah exit
          }
        }
      }
    }
  }
}
