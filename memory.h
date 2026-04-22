#ifndef ZMS_MEMORY_H
#define ZMS_MEMORY_H

#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>
#include <Preferences.h>
#include <string.h>
#include <time.h>

namespace {
Preferences g_memPrefs;
char g_logPath[72] = "";
bool timeLooksValid(time_t t) {
  return t > 1577836800; /* ~2020-01-01 UTC */
}
void makeLogPath() {
  time_t now = time(nullptr);
  struct tm ti;
  if (timeLooksValid(now) && localtime_r(&now, &ti) != nullptr) {
    strftime(g_logPath, sizeof(g_logPath), "/log_%Y%m%d_%H%M%S.bin", &ti);
    return;
  }
  if (!g_memPrefs.begin("cansat_mem", false)) {
    strncpy(g_logPath, "/log_fallback.bin", sizeof(g_logPath) - 1);
    g_logPath[sizeof(g_logPath) - 1] = '\0';
    return;
  }
  uint32_t seq = g_memPrefs.getUInt("logseq", 0) + 1;
  g_memPrefs.putUInt("logseq", seq);
  g_memPrefs.end();
  snprintf(g_logPath, sizeof(g_logPath), "/log_%08lu.bin", (unsigned long)seq);
}
void normalizeFsPath(const char *in, char *out, size_t outSz) {
  if (in == nullptr || outSz == 0) {
    if (outSz) out[0] = '\0';
    return;
  }
  if (in[0] == '/')
    snprintf(out, outSz, "%s", in);
  else
    snprintf(out, outSz, "/%s", in);
}
bool isLogFilename(const char *name) {
  if (name == nullptr) return false;
  const char *base = strrchr(name, '/');
  base = base ? base + 1 : name;
  return strncmp(base, "log_", 4) == 0;
}
bool findLatestLogPath(char *out, size_t outSz) {
  if (outSz == 0) return false;
  out[0] = '\0';
  File root = LittleFS.open("/");
  if (!root || !root.isDirectory()) return false;
  char best[72] = "";
  File f = root.openNextFile();
  while (f) {
    if (!f.isDirectory()) {
      char norm[72];
      normalizeFsPath(f.name(), norm, sizeof(norm));
      if (isLogFilename(norm)) {
        if (best[0] == '\0' || strcmp(norm, best) > 0) {
          strncpy(best, norm, sizeof(best) - 1);
          best[sizeof(best) - 1] = '\0';
        }
      }
    }
    f = root.openNextFile();
  }
  root.close();
  if (best[0] == '\0') return false;
  strncpy(out, best, outSz - 1);
  out[outSz - 1] = '\0';
  return true;
}
void printOneRecord(const SensorData &d) {
  SERIALDEBUG.printf(
      "id=%u can=%u T=%.2fC P=%.2fhPa alt=%.2fm acc=%.3f,%.3f,%.3f m/s2 "
      "ypr=%.2f,%.2f,%.2f deg | "
      "ch0=%.3fV %.4fA ch1=%.3fV %.4fA ch2=%.3fV %.4fA\n",
      (unsigned)d.idPacket, (unsigned)d.cansatId, d.temperatureC, d.pressureHpa,
      d.altitudeM, d.accelXmss, d.accelYmss, d.accelZmss, d.yawDeg, d.pitchDeg,
      d.rollDeg, d.ch0Voltage, d.ch0Amps, d.ch1Voltage, d.ch1Amps,
      d.ch2Voltage, d.ch2Amps);
}
} // namespace
inline bool memoryInit() {
  if (!LittleFS.begin(true)) {
    SERIALDEBUG.println(F("LittleFS: mount/format fallo (revisa esquema de particion)."));
    g_logPath[0] = '\0';
    return false;
  }
  makeLogPath();
  SERIALDEBUG.print(F("Log activo: "));
  SERIALDEBUG.println(g_logPath);
  return true;
}
inline void memoryAppend(const SensorData &pkt) {
  if (g_logPath[0] == '\0') return;
  File f = LittleFS.open(g_logPath, FILE_APPEND, true);
  if (!f) {
    f = LittleFS.open(g_logPath, "w", true);
  }
  if (!f) {
    SERIALDEBUG.println(F("memoryAppend: no se pudo abrir el log."));
    return;
  }
  f.write(reinterpret_cast<const uint8_t *>(&pkt), sizeof(SensorData));
  f.close();
}
inline void memoryDumpLatest() {
  char path[72];
  if (!findLatestLogPath(path, sizeof(path))) {
    SERIALDEBUG.println(F("No hay ficheros log_* en LittleFS."));
    return;
  }
  SERIALDEBUG.print(F("Volcando "));
  SERIALDEBUG.println(path);
  File f = LittleFS.open(path, FILE_READ);
  if (!f) {
    SERIALDEBUG.println(F("No se pudo abrir el fichero."));
    return;
  }
  SensorData rec;
  size_t n = 0;
  while (f.read(reinterpret_cast<uint8_t *>(&rec), sizeof(SensorData)) ==
         sizeof(SensorData)) {
    printOneRecord(rec);
    n++;
  }
  f.close();
  SERIALDEBUG.print(F("Total registros: "));
  SERIALDEBUG.println(n);
}
inline void memoryPollSerial() {
  if (!SERIALDEBUG.available()) return;
  int c = SERIALDEBUG.read();
  if (c == 'M' || c == 'm') memoryDumpLatest();
}
#endif