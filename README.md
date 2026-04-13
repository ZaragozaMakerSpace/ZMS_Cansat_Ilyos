# 🛰️ CanSat Ilyos

Sistema de adquisición de datos para CanSat basado en ESP32, orientado a monitorizar sensores y gestionar almacenamiento para su posterior transmisión con sistema de carga de batería con paneles solares.
El sistema recoge información de sensores como IMU (aceleración) y barómetro (presión, temperatura y altitud).
---

## ⚙️ ¿Qué hace actualmente?

* Lectura de sensor barométrico (BMP280):

  * Temperatura
  * Presión
  * Altitud

* Lectura de IMU (MPU6050):

  * Aceleración en ejes X, Y, Z

* Estructuración de datos en paquetes (`SensorData`)

* Sistema básico de almacenamiento en EEPROM (adaptado a ESP32)

* Salida de datos por puerto serie para depuración

---

## 📦 Estructura de datos

```cpp
struct SensorData {
  uint16_t idPacket;
  float accelX, accelY, accelZ;
  float pressure;
  float altitude;
};
```

---

## 🚧 Estado del proyecto

### ✅ Implementado

* [x] Lectura de BMP280
* [x] Lectura de IMU
* [x] Modelo de datos unificado
* [x] Sistema de debug por Serial
* [x] Base de almacenamiento en EEPROM

---

### 🛠️ Pendiente

* [ ] Monitorización de carga de batería con paneles solares 🌞🔋
* [ ] Sistema de envío de datos por radio (RFM69) 📡
* [ ] Integración completa de sensores en un único flujo de datos

---

### 🌐 Opcional / Futuro

* [ ] Punto de acceso WiFi para configuración ⚙️
* [ ] Interfaz para consulta de datos almacenados 📊
* [ ] Migración de EEPROM a `Preferences` o filesystem 💾
* [ ] Sistema de configuración remota

---

## 🎯 Objetivo

Construir un sistema modular, eficiente y robusto para telemetría en CanSat, preparado para operar en condiciones reales y optimizado para bajo consumo.

---
