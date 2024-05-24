int flagMenu = 0;
int nilaiAngka = 0;
// int flagInmenu=0;
unsigned long intervalButton = 250; //nilai

void resetNilai() {
  flagMenu = 0;
  nilaiAngka = 0;
}

void menu() {
  unsigned long currentMillis = millis();              //mendapatkan waktu saat ini
  if (currentMillis - prevMillis >= intervalButton) {  //jika interval waktu sudah terlewati
    prevMillis = currentMillis;
    if (digitalRead(pinDown) == LOW) {
      Serial.println("masuk");
      lcd.clear();
      flagMenu++;
      Serial.println(flagMenu);
      if (flagMenu > 2) {
        flagMenu = 0;
      }
    } else if (digitalRead(pinUp) == LOW) {
      lcd.clear();
      flagMenu--;
      Serial.println(flagMenu);
      if (flagMenu < 0) {
        flagMenu = 2;
      }
    }
    else if (digitalRead(pinOk) == LOW && digitalRead (pinLeft) == LOW)
    {
      resetNilai();
      resetAll();
      lcd.clear();
      resetFunc();
    }
    else if (digitalRead(pinOk) == LOW) {
      if (flagMenu == 0) {
        menuSetTimeout();
      } else if (flagMenu == 1) {
        menuSetDelay();
      } else if (flagMenu == 2) {
        menuSetArah();
      }
    }
  }
  lcd.setCursor(3, 0);
  lcd.print("Menu Setting");
  if (flagMenu == 0) {
    lcd.setCursor(0, 1);
    lcd.print(">Set TimeOut<");
    lcd.setCursor(0, 2);
    lcd.print("Set Delay Open");
    lcd.setCursor(0, 3);
    lcd.print("Set Arah");
  } else if (flagMenu == 1) {
    lcd.setCursor(0, 1);
    lcd.print("Set TimeOut");
    lcd.setCursor(0, 2);
    lcd.print(">Set Delay Open<");
    lcd.setCursor(0, 3);
    lcd.print("Set Arah");
  }
  if (flagMenu == 2) {
    lcd.setCursor(0, 1);
    lcd.print("Set TimeOut");
    lcd.setCursor(0, 2);
    lcd.print("Set Delay Open");
    lcd.setCursor(0, 3);
    lcd.print(">Set Arah<");
  }
}
void menuSetTimeout() {
  lcd.clear();
  while (flagMenu == 0) {
    lcd.setCursor(3, 0);
    lcd.print("Set Timeout");
    lcd.setCursor(2, 1);
    lcd.print("masukan timeout ");
    unsigned long currentMillis = millis();  //mendapatkan waktu saat ini
    if (currentMillis - prevMillis >= intervalButton) {
      prevMillis = currentMillis;
      if (digitalRead(pinRight) == LOW) {
        nilaiAngka++;
        lcd.setCursor(7, 2);
        lcd.print(nilaiAngka);
        if (nilaiAngka > 20) {
          nilaiAngka = 0;
          lcd.setCursor(8, 2);
          lcd.print(" ");
          lcd.setCursor(7, 2);
          lcd.print(nilaiAngka);
        }
      } else if (digitalRead(pinLeft) == LOW) {
        nilaiAngka--;
        if (nilaiAngka == 9) {
          lcd.setCursor(8, 2);
          lcd.print(" ");
        } else if (nilaiAngka <= 0) {
          nilaiAngka = 0;
        }
        lcd.setCursor(7, 2);
        lcd.print(nilaiAngka);
      }
      else if (digitalRead(pinOk) == LOW) {
        simpanEEPROM(nilaiDelay, nilaiAngka, EEPROMDelay, EEPROMTimeout);
        delay(10);
        resetNilai();
        resetAll();
        lcd.clear();
        resetFunc();
        break;
      }
      else if (digitalRead(pinOk) == LOW && digitalRead (pinLeft) == LOW)
      {
        resetNilai();
        resetAll();
        lcd.clear();
        resetFunc();
        break;
      }
    }
  }
}
void menuSetDelay() {
  lcd.clear();
  while (flagMenu == 1) {
    lcd.setCursor(3, 0);
    lcd.print("Set Delay");
    lcd.setCursor(2, 1);
    lcd.print("Masukan Delay Open");
    unsigned long currentMillis = millis();  //mendapatkan waktu saat ini
    if (currentMillis - prevMillis >= intervalButton) {
      prevMillis = currentMillis;
      if (digitalRead(pinRight) == LOW) {
        nilaiAngka++;
        lcd.setCursor(7, 2);
        lcd.print(nilaiAngka);
        if (nilaiAngka > 20) {
          nilaiAngka = 0;
          lcd.setCursor(8, 2);
          lcd.print(" ");
          lcd.setCursor(7, 2);
          lcd.print(nilaiAngka);
        }
      } else if (digitalRead(pinLeft) == LOW) {
        nilaiAngka--;
        if (nilaiAngka == 9) {
          lcd.setCursor(8, 2);
          lcd.print(" ");
        } else if (nilaiAngka <= 0) {
          nilaiAngka = 0;
        }
        lcd.setCursor(7, 2);
        lcd.print(nilaiAngka);
      } else if (digitalRead(pinOk) == LOW) {
        simpanEEPROM(nilaiAngka, timeout, EEPROMDelay, EEPROMTimeout);
        resetNilai();
        resetAll();
        lcd.clear();
        resetFunc();
        break;
      }
      else if (digitalRead(pinOk) == LOW && digitalRead (pinLeft) == LOW)
      {
        resetNilai();
        resetAll();
        lcd.clear();
        resetFunc();
        break;
      }
    }
  }
}
void menuSetArah() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Set Arah");
  lcd.setCursor(2, 1);
  lcd.print("Masukan Arah");
  lcd.setCursor(7, 2);
  lcd.print(nilaiAngka);
  while (flagMenu == 2) {
    unsigned long currentMillis = millis();  //mendapatkan waktu saat ini
    if (currentMillis - prevMillis >= intervalButton) {
      prevMillis = currentMillis;
      if (digitalRead(pinRight) == LOW) {
        nilaiAngka++;
        lcd.setCursor(7, 2);
        lcd.print(nilaiAngka);
        if (nilaiAngka > 3) {
          nilaiAngka = 0;
          lcd.setCursor(8, 2);
          lcd.print(" ");
          lcd.setCursor(7, 2);
          lcd.print(nilaiAngka);
        }
      } else if (digitalRead(pinLeft) == LOW) {
        nilaiAngka--;
        if (nilaiAngka == 9) {
          lcd.setCursor(8, 2);
          lcd.print(" ");
        } else if (nilaiAngka < 0) {
          nilaiAngka = 3;
        }
        lcd.setCursor(7, 2);
        lcd.print(nilaiAngka);
      } else if (digitalRead(pinOk) == LOW) {
        simpanArah(nilaiAngka, EEPROMArah);
        resetNilai();
        resetAll();
        lcd.clear();
        resetFunc();
        break;
      }
      else if (digitalRead(pinOk) == LOW && digitalRead (pinLeft) == LOW)
      {
        resetNilai();
        resetAll();
        lcd.clear();
        resetFunc();
        break;
      }
    }
  }
}
