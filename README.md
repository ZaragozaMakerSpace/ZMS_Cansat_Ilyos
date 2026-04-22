# CanSat Ilyos

Sistema de adquisición de datos para CanSat basado en **ESP32** (p. ej. ESP32-C3), orientado a telemetría (barómetro, IMU, monitorización de potencia) y envío por **RFM69** (RadioHead), con registro local en **LittleFS** y depuración por puerto serie.

---

## ¿Qué hace actualmente?

* **BMP280** (`bmp_utils.h`): temperatura (°C), presión (hPa) y altitud estimada (m).
* **MPU6050 con DMP** (`imu_utils.h`): aceleración lineal aproximada en X/Y/Z (m/s²) y ángulos yaw, pitch, roll (°).
* **INA3221** (`solar.h`): tensión y corriente en tres canales (almacenadas en el paquete de telemetría).
* **Modelo unificado** (`datamodel.h`): estructura `SensorData` compartida entre sensores, radio y registro en fichero.
* **Radio RFM69** (`radio.h`, RadioHead `RH_RF69`): envío binario del struct completo (`sizeof(SensorData)`).
* **Registro en flash** (`memory.h`):
  * **LittleFS**: ficheros binarios con prefijo `log_` y nombre con fecha/hora si el reloj del sistema es válido, o `log_NNNNNNNN.bin` usando un contador en **Preferences** (NVS) si no hay hora sincronizada.
  * Cada muestra es un registro consecutivo del mismo layout que `SensorData`.
* **Consulta por serie**: en el `loop`, si envías **`M`** o **`m`** por el puerto serie configurado (`SERIALDEBUG`), se vuelca por serial el contenido del **último** fichero `log_*` encontrado en LittleFS (registros interpretados campo a campo).

Salida de depuración habitual por serial durante la lectura de sensores (macros `DUMP*` en `config.h`).

---

## Estructura de datos (`SensorData`)

Definición actual en `datamodel.h` (el receptor / análisis offline debe usar el mismo layout y endianness, típicamente little-endian en ESP32):

```cpp
struct SensorData {
  uint16_t idPacket;
  uint8_t  cansatId;
  float temperatureC;
  float pressureHpa;
  float altitudeM;
  float accelXmss, accelYmss, accelZmss;
  float rollDeg, pitchDeg, yawDeg;
  float ch0Voltage, ch0Amps;
  float ch1Voltage, ch1Amps;
  float ch2Voltage, ch2Amps;
} data;
```

---

## Ficheros principales del sketch

| Fichero | Rol |
|---------|-----|
| `ZMS_Cansat_Ilyos.ino` | `setup` / `loop`, inicialización de memoria y encuesta serie |
| `config.h` | Pines radio, IDs, intervalos, macros de debug |
| `datamodel.h` | `SensorData` y variable global `data` |
| `bmp_utils.h` | BMP280 |
| `imu_utils.h` | MPU6050 + DMP |
| `solar.h` | INA3221 tres canales |
| `radio.h` | RFM69, `sendRadioPacket()` |
| `memory.h` | LittleFS, Preferences, `memoryAppend` / volcado con `M` |


---

## Requisitos de compilación (ESP32)

* Núcleo **esp32** en Arduino IDE o PlatformIO.
* Librerías habituales: **RadioHead**, **Adafruit BMP280**, **Adafruit BusIO**, **Adafruit INA3221**, **MPU6050** / **I2Cdev** (según tu instalación).
* **Esquema de partición** con espacio para **LittleFS**; si `memoryInit()` indica fallo de montaje, revisa *Tools → Partition Scheme* en el IDE.


---

## Estado del proyecto

### Implementado

* [x] Lectura BMP280 (temperatura, presión, altitud)
* [x] Lectura IMU MPU6050 con DMP (aceleración lineal + yaw/pitch/roll)
* [x] Lectura INA3221 (tensión y corriente en tres canales)
* [x] Modelo de datos unificado (`SensorData`) y variable `data`
* [x] Sistema de debug por serial
* [x] Envío de telemetría por radio RFM69 (payload binario)
* [x] Registro en flash con **LittleFS** + uso de **Preferences** para nombre de fichero cuando no hay reloj válido
* [x] Volcado del último log por serial (tecla `M` / `m`)
* [x] Flujo único en `loop`: lectura sensores → empaquetado → registro y envío radio


## 🎯 Objetivo

### Opcional / futuro

* [ ] Punto de acceso WiFi para configuración o descarga de logs
* [ ] Sistema de configuración remota (parámetros por serial o WiFi)

---

## Objetivo

Mantener un sistema modular y claro para telemetría CanSat: un solo tipo de paquete para radio, flash y depuración, con posibilidad de ampliar sensores o protocolo manteniendo versionado del layout si hace falta.
