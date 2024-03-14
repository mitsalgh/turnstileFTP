void simpanEEPROM (int setDelay,int setTimeout,int addressDelay,int addressTimeout)
{
  EEPROM.write(addressDelay,setDelay);
  EEPROM.write(addressTimeout,setTimeout);
}

void bacaEEPROM(int addressDelay,int addressTimeout)
{
 nilaiDelay = EEPROM.read(addressDelay);
 timeout = EEPROM.read(addressTimeout);
}