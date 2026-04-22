#include <Arduino.h>
#include <EEPROM.h>

#ifndef _CANSAT_EEPROM_H
#define _CANSAT_EEPROM_H

// Definir el tamaño de la EEPROM (ajusta según tus necesidades, max 4096)
#define EEPROM_SIZE 512

// Inicialización recomendada
/*
 * 
inline void initEEPROM() {
  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("Error al inicializar EEPROM");
  }
}*/

// Dirección global si la necesitas
uint16_t eeAddress = 0; 

/**
 * Escribe cualquier tipo de dato o estructura.
 * ESP32 usa 'put', que internamente ya verifica si el dato 
 * ha cambiado antes de escribir (equivalente a 'update').
 */
template <typename T> 
void EEPROMWrite(int startAddr, const T &value) {
  EEPROM.put(startAddr, value);
  EEPROM.commit(); // ¡Crucial en ESP32 para guardar los cambios!
}

/**
 * Lee cualquier tipo de dato o estructura.
 */
template <typename T> 
void EEPROMRead(int startAddr, T &value) {
  EEPROM.get(startAddr, value);
}

#endif
