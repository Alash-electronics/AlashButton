// AlashButton: самый простой пример. Один клик по кнопке включает или выключает светодиод.
//
// Подключение: одна ножка кнопки на пин 2, другая на GND. Резистор не нужен:
// внутри платы включается подтяжка (режим по умолчанию PULLUP).
#include <AlashButton.h>

AlashButton button(2);                    // пин кнопки
#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif
const uint8_t LED = LED_BUILTIN;
bool ledOn = false;

void setup() {
  pinMode(LED, OUTPUT);
  button.begin();
}

void loop() {
  if (button.clicked()) {                 // true один раз после каждого клика, дребезг убирает библиотека
    ledOn = !ledOn;
    digitalWrite(LED, ledOn);
  }
}
