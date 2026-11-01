// AlashButton: кнопка без дребезга: нажатие, клик, двойной клик, долгое нажатие, повтор при удержании.
// Без delay() и без обязательного update(): достаточно вызывать методы в loop().
// MIT License. Alash Electronics, https://github.com/Alash-electronics/AlashButton
#ifndef ALASH_BUTTON_H
#define ALASH_BUTTON_H

#include <Arduino.h>

class AlashButton {
 public:
  // Какая подтяжка у кнопки
  enum Mode : uint8_t {
    BTN_PULLUP,      // кнопка между пином и GND, подтяжка внутри платы (самый простой способ, по умолчанию)
    BTN_PULLDOWN,    // кнопка между пином и питанием, внешний резистор на GND
    BTN_EXTERNAL     // свой резистор: кнопка на GND (активный LOW) или на питание
  };

  enum Error : uint8_t {
    ERR_OK = 0,
    ERR_NO_PULLUP    // у ESP32 GPIO34-39 нет внутренней подтяжки: нужен внешний резистор
  };

  enum Language : uint8_t { LANG_RU = 0, LANG_KK = 1, LANG_EN = 2 };

  // pin: к какому пину подключена кнопка. mode: BTN_PULLUP (по умолчанию), BTN_PULLDOWN или BTN_EXTERNAL.
  // activeLow используется только для BTN_EXTERNAL: true = нажатая кнопка даёт LOW.
  explicit AlashButton(uint8_t pin, Mode mode = BTN_PULLUP, bool activeLow = true);

  bool begin();                           // настроить пин (можно не вызывать: настроится при первом опросе)

  // ---- События: каждое возвращает true один раз -------------------------------
  bool pressed();                         // кнопку только что нажали
  bool released();                        // кнопку только что отпустили
  bool clicked();                         // короткий клик (после отпускания)
  bool doubleClicked();                   // два коротких клика подряд (нужно setDoubleClickMs)
  bool longPressed();                     // кнопку держат дольше setLongPressMs (один раз за нажатие)
  bool repeat();                          // пока кнопку держат: true через равные промежутки (включается setRepeat)

  // ---- Состояние -----------------------------------------------------------------
  bool isDown();                          // кнопка нажата прямо сейчас (без дребезга)
  uint32_t heldMs();                      // сколько мс кнопку держат (0, если отпущена)
  uint32_t lastHeldMs() const { return _lastHeld; }   // сколько держали в прошлый раз

  // ---- Настройки -----------------------------------------------------------------
  void setDebounceMs(uint16_t ms) { _debounceMs = ms; }              // по умолчанию 30
  void setLongPressMs(uint16_t ms) { _longMs = ms; }                 // по умолчанию 800
  void setDoubleClickMs(uint16_t ms) { _doubleMs = ms; }             // 0 = выключен (по умолчанию). Тогда клик приходит сразу
  void setRepeat(uint16_t delayMs, uint16_t everyMs) { _repDelay = delayMs; _repEvery = everyMs; }   // повтор при удержании, по умолчанию выключен. Пример: setRepeat(500, 100). После повторов клик при отпускании не приходит

  // Обработчики (по желанию): вызываются из опроса, пока вы вызываете любой метод кнопки
  void onClick(void (*fn)()) { _onClick = fn; }
  void onDoubleClick(void (*fn)()) { _onDouble = fn; }
  void onLongPress(void (*fn)()) { _onLong = fn; }
  void onPress(void (*fn)()) { _onPress = fn; }
  void onRelease(void (*fn)()) { _onRelease = fn; }
  void update() { poll(); }               // опрос вручную (нужен только если вы не вызываете методы выше)

  // ---- Диагностика -----------------------------------------------------------------
  void setLanguage(Language lang) { _lang = lang; }
  Error error() const { return _err; }
  const __FlashStringHelper *errorText();
  void diagnose(Print &out);              // режим, текущий уровень на пине и подсказки

 private:
  uint8_t _pin;
  Mode _mode;
  bool _activeLow;
  bool _started = false;
  Error _err = ERR_OK;
  Language _lang = LANG_RU;

  uint16_t _debounceMs = 30, _longMs = 800, _doubleMs = 0, _repDelay = 500, _repEvery = 0;

  bool _raw = false;                      // последний сырой уровень (true = нажата)
  uint32_t _rawAt = 0;                    // когда он изменился
  bool _down = false;                     // подтверждённое состояние
  uint32_t _downAt = 0, _lastHeld = 0;
  bool _longFired = false;
  bool _repFired = false;
  uint32_t _repNext = 0;
  bool _waiting = false;                  // ждём второй клик
  uint32_t _firstRelAt = 0;
  bool _secondPress = false;

  bool _fPress = false, _fRelease = false, _fClick = false, _fDouble = false, _fLong = false, _fRepeat = false;
  void (*_onClick)() = nullptr;
  void (*_onDouble)() = nullptr;
  void (*_onLong)() = nullptr;
  void (*_onPress)() = nullptr;
  void (*_onRelease)() = nullptr;

  bool readPressed();
  void poll();
  bool take(bool &flag);
};

#endif  // ALASH_BUTTON_H
