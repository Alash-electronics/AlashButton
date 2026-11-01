// AlashButton: одна кнопка, три действия: клик, двойной клик и долгое нажатие.
// Для двойного клика включите setDoubleClickMs(): тогда одиночный клик придёт
// с небольшой задержкой (библиотека ждёт, не будет ли второго).
#include <AlashButton.h>

AlashButton button(2);

void setup() {
  Serial.begin(115200);
  button.begin();
  button.setDoubleClickMs(300);           // два клика не дольше 300 мс друг от друга
  button.setLongPressMs(800);             // долгое нажатие от 0.8 с
}

void loop() {
  if (button.clicked()) Serial.println("Клик");
  if (button.doubleClicked()) Serial.println("Двойной клик");
  if (button.longPressed()) Serial.println("Долгое нажатие");
}
