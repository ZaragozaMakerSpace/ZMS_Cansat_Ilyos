#include "Adafruit_INA3221.h"

// Create an INA3221 object
Adafruit_INA3221 ina3221;

void setupINA() {

  Wire.begin(8,9);
  // Initialize the INA3221
  if (!ina3221.begin(0x40, &Wire)) { // can use other I2C addresses or buses
    DUMPSLN("Failed to find INA3221 chip");
    while (1)
      delay(10);
  }
  DUMPSLN("INA3221 Found!");

  ina3221.setAveragingMode(INA3221_AVG_16_SAMPLES);

  // Set shunt resistances for all channels to 0.05 ohms
  for (uint8_t i = 0; i < 3; i++) {
    ina3221.setShuntResistance(i, 0.05);
  }

  // Set a power valid alert to tell us if ALL channels are between the two
  // limits:
  ina3221.setPowerValidLimits(3.0 /* lower limit */, 15.0 /* upper limit */);
}

void readINA() {
  // Display voltage and current (in mA) for all three channels
  DUMPPRINTLN();
  for (uint8_t i = 0; i < 3; i++) {
    float voltage = ina3221.getBusVoltage(i);
    float current = ina3221.getCurrentAmps(i) * 1000; // Convert to mA

    switch (i) {
      case 0: data.ch0Voltage = voltage; data.ch0Amps = current; break;
      case 1: data.ch1Voltage = voltage; data.ch1Amps = current; break;
      case 2: data.ch2Voltage = voltage; data.ch2Amps = current; break;
    }

    DUMP("Channel ", i);
    DUMP(": Voltage = ", voltage);
    DUMP(" V, Current = ", current);
    DUMPSLN(" mA");
  }

  DUMPPRINTLN();
  // Delay for 250ms before the next reading
  delay(100);
}
