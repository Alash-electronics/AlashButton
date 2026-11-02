// [ru] AlashButton: обработчики. Вы пишете функции, библиотека вызывает их сама.
//      Нужно лишь вызывать button.update() в loop().
// [kk] AlashButton: өңдеушілер. Сіз функциялар жазасыз, кітапхана оларды өзі шақырады.
//      loop() ішінде тек button.update() шақыру жеткілікті.
// [en] AlashButton: handlers. You write functions, the library calls them itself.
//      You only need to call button.update() in loop().
#include <AlashButton.h>

AlashButton button(2);

void onClick() { Serial.println("Клик"); }
void onLong() { Serial.println("Долгое нажатие"); }
void onPress() { Serial.println("Нажали"); }
void onRelease() { Serial.println("Отпустили"); }

void setup() {
  Serial.begin(115200);
  button.begin();
  button.onClick(onClick);
  button.onLongPress(onLong);
  button.onPress(onPress);
  button.onRelease(onRelease);
}

void loop() {
  button.update();
}
