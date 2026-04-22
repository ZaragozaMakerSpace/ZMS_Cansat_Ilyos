#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>

Adafruit_BMP280 bmp;

bool setupBMP() {
  bool status = bmp.begin(0x76);
  if (status) {
    bmp.setSampling(
      Adafruit_BMP280::MODE_NORMAL,  /* Operating Mode. */
      Adafruit_BMP280::SAMPLING_X2,  /* Temp. oversampling */
      Adafruit_BMP280::SAMPLING_X16, /* Pressure oversampling */
      Adafruit_BMP280::FILTER_X16,   /* Filtering. */
      Adafruit_BMP280::STANDBY_MS_500);
  } else {
    DUMPSLN(MSG_BMP_FAIL);
  }
  return status;
}

void readBMP() {
  DUMP( TEMP_MSG , bmp.readTemperature());
  DUMPSLN( TEMP_UNIT );

  DUMP( PRESSURE_MSG , bmp.readPressure());
  DUMPSLN( PRESSURE_UNIT );

  DUMP(ALTITUDE_MSG, bmp.readAltitude(1013.25));
  DUMPSLN( ALTITUDE_UNIT );
}
