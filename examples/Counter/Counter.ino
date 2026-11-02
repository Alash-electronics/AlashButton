// [ru] AlashButton: счётчик. Клик добавляет 1, удержание быстро добавляет ещё (повтор),
//      долгое нажатие сбрасывает счёт.
// [kk] AlashButton: санауыш. Басу 1 қосады, ұстап тұру тез қоса береді (қайталау),
//      ұзақ басу санауды нөлге түсіреді.
// [en] AlashButton: a counter. A click adds 1, holding quickly adds more (repeat),
//      a long press resets the count.
#include <AlashButton.h>

AlashButton button(2);
int count = 0;

void setup() {
  Serial.begin(115200);
  button.begin();
  button.setLongPressMs(2000);            // сброс только после 2 секунд
  button.setRepeat(500, 100);             // через 0.5 с повтор каждые 0.1 с
}

void loop() {
  if (button.clicked() || button.repeat()) {
    count++;
    Serial.println(count);
  }
  if (button.longPressed()) {
    count = 0;
    Serial.println("Сброс");
  }
}
