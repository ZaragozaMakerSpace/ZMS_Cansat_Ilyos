#include <Arduino.h>
#include <EEPROM.h>

#ifndef _CANSAT_EEPROM_H
#define _CANSAT_EEPROM_H

uint8_t eeAddress = 0;
template <typename T> void EEPROMWrite(int startAddr, const T &value) {
  const byte *ptr = (const byte *)(const void *)&value;
  for (unsigned i = 0; i < sizeof(value); i++) {
    
    #if defined(ESP32)
    EEPROM.write(startAddr++, *ptr++);
    #endif
    
  }
}

template <typename T> void EEPROMRead(int startAddr, T &value) {
  byte *ptr = (byte *)(void *)&value;
  for (unsigned i = 0; i < sizeof(value); i++) {
    *ptr++ = EEPROM.read(startAddr++);
  }
}
#endif
