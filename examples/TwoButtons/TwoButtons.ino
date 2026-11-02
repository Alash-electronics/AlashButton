// [ru] AlashButton: две кнопки. Каждая кнопка это отдельный объект, работают независимо.
//      Кнопка "плюс" на пин 2, кнопка "минус" на пин 3 (вторые ножки на GND).
// [kk] AlashButton: екі батырма. Әр батырма бөлек нысан, олар тәуелсіз жұмыс істейді.
//      «Плюс» батырмасы 2 пинге, «минус» батырмасы 3 пинге (екінші аяқтары GND-ға).
// [en] AlashButton: two buttons. Each button is a separate object, they work independently.
//      The "plus" button on pin 2, the "minus" button on pin 3 (the other legs to GND).
#include <AlashButton.h>

AlashButton plus(2);
AlashButton minus(3);
int value = 0;

void setup() {
  Serial.begin(115200);
  plus.begin();
  minus.begin();
  plus.setRepeat(500, 100);               // удержание быстро меняет значение
  minus.setRepeat(500, 100);
}

void loop() {
  if (plus.clicked() || plus.repeat()) {  // клик или удержание
    value++;
    Serial.println(value);
  }
  if (minus.clicked() || minus.repeat()) {
    value--;
    Serial.println(value);
  }
}
