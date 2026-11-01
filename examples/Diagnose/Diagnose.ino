// AlashButton: кнопка не реагирует? Скетч каждую секунду показывает, что видит плата на пине.
// Нажимайте кнопку: уровень должен меняться с HIGH на LOW (или наоборот для PULLDOWN).
#include <AlashButton.h>

AlashButton button(2);                    // пин и режим, как в вашем проекте

void setup() {
  Serial.begin(115200);
  delay(500);
  button.setLanguage(AlashButton::LANG_RU);   // LANG_KK: қазақша, LANG_EN: English
  button.begin();
}

void loop() {
  button.diagnose(Serial);
  Serial.println();
  delay(1000);
}
