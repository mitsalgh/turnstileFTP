void simpanEEPROM (int setDelay,int setTimeout,int addressDelay,int addressTimeout)
{
  EEPROM.write(addressDelay,setDelay);
  EEPROM.write(addressTimeout,setTimeout);
}
void simpanArah(int setArah,int addressArah)
{
  EEPROM.write(addressArah,setArah);
}
void bacaEEPROM(int addressDelay,int addressTimeout)
{
 nilaiDelay = EEPROM.read(addressDelay);
 timeout = EEPROM.read(addressTimeout);
}
void bacaArah(int addressArah)
{
 nilaiArah = EEPROM.read(addressArah);
}
 
