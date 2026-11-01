// Минимальная имитация Arduino.h для запуска тестов на компьютере (g++).
#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string.h>
using __FlashStringHelper = char;
#define F(x) ((const __FlashStringHelper *)(x))
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
// Как в настоящем Arduino.h (AVR, ESP32): эти слова уже заняты макросами, имена в библиотеке не должны с ними пересекаться
#define PULLUP 0x3
#define PULLDOWN 0x4
#define EXTERNAL 0x5
#define LOW 0
#define HIGH 1
inline uint32_t g_now = 0;
inline uint8_t g_level[64];
inline uint8_t g_mode[64];
inline uint32_t millis() { return g_now; }
inline void pinMode(uint8_t p, uint8_t m) { g_mode[p] = m; }
inline int digitalRead(uint8_t p) { return g_level[p]; }
class Print {
 public:
  void print(const char *s) { printf("%s", s); }
  void print(unsigned v) { printf("%u", v); }
  void println(const char *s = "") { printf("%s\n", s); }
  void println(unsigned v) { printf("%u\n", v); }
  void println(int v) { printf("%d\n", v); }
};
