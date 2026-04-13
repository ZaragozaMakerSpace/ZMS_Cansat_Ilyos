#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 imu;

bool setupIMU() { return imu.begin(); }

SensorData calcularIMU() {
  sensors_event_t a, g, temp;
  imu.getEvent(&a, &g, &temp);

  SensorData data;

  data.accelX = a.acceleration.x;
  data.accelY = a.acceleration.y;
  data.accelZ = a.acceleration.z;

  return data;
}
