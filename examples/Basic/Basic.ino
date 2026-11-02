// [ru] AlashButton: самый простой пример. Один клик по кнопке включает или выключает светодиод.
//
//      Подключение: одна ножка кнопки на пин 2, другая на GND. Резистор не нужен:
//      внутри платы включается подтяжка (режим по умолчанию PULLUP).
// [kk] AlashButton: ең қарапайым мысал. Батырманы бір басу жарықдиодты қосады немесе өшіреді.
//
//      Қосу: батырманың бір аяғы 2 пинге, екіншісі GND-ға. Резистор керек емес:
//      тақта ішінде тартқыш қосылады (әдепкі PULLUP режимі).
// [en] AlashButton: the simplest example. One click on the button switches an LED on or off.
//
//      Wiring: one leg of the button to pin 2, the other to GND. No resistor needed:
//      the board's pull-up is enabled inside (the default PULLUP mode).
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
