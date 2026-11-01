// MIT License. Alash Electronics.
#include "AlashButton.h"

#define T(ru, kk, en) (_lang == LANG_KK ? F(kk) : _lang == LANG_EN ? F(en) : F(ru))

AlashButton::AlashButton(uint8_t pin, Mode mode, bool activeLow)
    : _pin(pin), _mode(mode), _activeLow(mode == BTN_PULLUP ? true : mode == BTN_PULLDOWN ? false : activeLow) {}

bool AlashButton::begin() {
  _err = ERR_OK;
#if defined(ESP32)
  if (_mode == BTN_PULLUP && _pin >= 34 && _pin <= 39) _err = ERR_NO_PULLUP;   // эти пины только на вход, без подтяжки
#endif
  if (_mode == BTN_PULLUP) {
    pinMode(_pin, INPUT_PULLUP);
  } else if (_mode == BTN_PULLDOWN) {
#if defined(INPUT_PULLDOWN)
    pinMode(_pin, INPUT_PULLDOWN);          // ESP32: подтяжка к GND внутри. На Uno и др. нужен внешний резистор
#else
    pinMode(_pin, INPUT);
#endif
  } else {
    pinMode(_pin, INPUT);
  }
  _started = true;
  _raw = readPressed();
  _rawAt = millis();
  _down = _raw;                            // если кнопка зажата при старте, считаем её нажатой без события
  _downAt = _rawAt;
  _longFired = _down;                      // и долгое нажатие для неё не срабатывает
  _repFired = false;
  return _err == ERR_OK;
}

bool AlashButton::readPressed() {
  bool level = digitalRead(_pin) != 0;
  return _activeLow ? !level : level;
}

// Один шаг опроса: дребезг отсекаем «устойчивым уровнем» (не меняется debounceMs подряд).
void AlashButton::poll() {
  if (!_started) begin();
  uint32_t now = millis();

  bool raw = readPressed();
  if (raw != _raw) {
    _raw = raw;
    _rawAt = now;
  }

  if (_raw != _down && (uint32_t)(now - _rawAt) >= _debounceMs) {
    _down = _raw;
    if (_down) {                           // нажали
      _downAt = now;
      _longFired = false;
      _repFired = false;
      _repNext = now + _repDelay;
      _fPress = true;
      if (_waiting && (uint32_t)(now - _firstRelAt) <= _doubleMs) _secondPress = true;
      _waiting = false;
      if (_onPress) _onPress();
    } else {                               // отпустили
      _lastHeld = (uint32_t)(now - _downAt);
      _fRelease = true;
      if (_onRelease) _onRelease();
      if (!_longFired && !_repFired) {         // после долгого нажатия или повторов клика нет
        if (_doubleMs == 0) {
          _fClick = true;
          if (_onClick) _onClick();
        } else if (_secondPress) {
          _fDouble = true;
          _secondPress = false;
          if (_onDouble) _onDouble();
        } else {
          _waiting = true;                 // подождём, не будет ли второго клика
          _firstRelAt = now;
        }
      }
      _secondPress = false;
    }
  }

  if (_down) {
    uint32_t held = (uint32_t)(now - _downAt);
    if (!_longFired && held >= _longMs) {
      _longFired = true;
      _waiting = false;
      _secondPress = false;
      _fLong = true;
      if (_onLong) _onLong();
    }
    if (_repEvery && (int32_t)(now - _repNext) >= 0) {
      _fRepeat = true;
      _repFired = true;
      _repNext = now + _repEvery;
    }
  } else if (_waiting && (uint32_t)(now - _firstRelAt) > _doubleMs) {
    _waiting = false;                      // второго клика не было: это обычный клик
    _fClick = true;
    if (_onClick) _onClick();
  }
}

bool AlashButton::take(bool &flag) {
  poll();
  bool v = flag;
  flag = false;
  return v;
}

bool AlashButton::pressed() { return take(_fPress); }
bool AlashButton::released() { return take(_fRelease); }
bool AlashButton::clicked() { return take(_fClick); }
bool AlashButton::doubleClicked() { return take(_fDouble); }
bool AlashButton::longPressed() { return take(_fLong); }
bool AlashButton::repeat() { return take(_fRepeat); }

bool AlashButton::isDown() {
  poll();
  return _down;
}

uint32_t AlashButton::heldMs() {
  poll();
  return _down ? (uint32_t)(millis() - _downAt) : 0;
}

const __FlashStringHelper *AlashButton::errorText() {
  switch (_err) {
    case ERR_OK: return F("OK");
    case ERR_NO_PULLUP:
      return T("У этого пина ESP32 (GPIO34-39) нет внутренней подтяжки: поставьте резистор 10 кОм к 3.3 В и используйте режим BTN_EXTERNAL, либо выберите другой пин",
               "ESP32-нің бұл пинінде (GPIO34-39) ішкі тартқыш жоқ: 3.3 В-қа 10 кОм резистор қойып, BTN_EXTERNAL режимін қолданыңыз немесе басқа пин таңдаңыз",
               "This ESP32 pin (GPIO34-39) has no internal pull-up: add a 10 kOhm resistor to 3.3 V and use BTN_EXTERNAL mode, or pick another pin");
  }
  return F("");
}

void AlashButton::diagnose(Print &out) {
  if (!_started) begin();
  out.println(T("AlashButton: диагностика", "AlashButton: диагностика", "AlashButton: diagnostics"));
  out.print(T("Пин: ", "Пин: ", "Pin: "));
  out.println(_pin);
  out.print(T("Режим: ", "Режим: ", "Mode: "));
  out.println(_mode == BTN_PULLUP ? F("PULLUP (кнопка на GND)") : _mode == BTN_PULLDOWN ? F("PULLDOWN (кнопка на питание)") : F("EXTERNAL"));
  bool level = digitalRead(_pin) != 0;
  out.print(T("Уровень на пине: ", "Пиндегі деңгей: ", "Pin level: "));
  out.println(level ? F("HIGH") : F("LOW"));
  out.print(T("Кнопка: ", "Түйме: ", "Button: "));
  out.println(readPressed() ? T("нажата", "басылған", "pressed") : T("отпущена", "жіберілген", "released"));
  if (_err != ERR_OK) out.println(errorText());
  out.println(T("Нажмите кнопку и запустите диагностику снова: уровень должен смениться. Если не меняется, проверьте провода и пин.",
                "Түймені басып, диагностиканы қайта іске қосыңыз: деңгей өзгеруі керек. Өзгермесе, сымдар мен пинді тексеріңіз.",
                "Press the button and run the diagnostics again: the level must change. If it does not, check the wires and the pin."));
}
