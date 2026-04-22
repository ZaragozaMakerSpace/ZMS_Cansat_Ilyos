#include <Arduino.h>

#include <Ticker.h> //Sistema no bloqueante

#include "config.h"
#include "datamodel.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

#include "radio.h"
#include "bmp_utils.h"
#include "imu_utils.h"
#include "solar.h"

Ticker radioTicker;

void setup() {
  if (PRINTDEBUG)
    SERIALDEBUG.begin(SERIALBAUDS);
  DUMPSLN(MSG_BMP_TITLE);
  delay(100);
  Wire.begin(8,9);
  delay(100);
  setupRadio();

  // BMP Initialization
  setupBMP();

  // INA Initialization
  setupINA();
  
  // IMU Initialization
  setupIMU();
  
  radioTicker.attach_ms(1000,sendRadioPacket );
}

void loop() {
  readBMP();
  readIMU();
  readINA();
  
}
