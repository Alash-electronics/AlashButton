# AlashButton

[Русский](#русский) | [Қазақша](#қазақша) | [English](#english)

---

## Русский

Простая библиотека для кнопок: убирает дребезг и сама различает **клик**, **двойной клик**, **долгое нажатие** и **удержание с повтором**. Без `delay()`, без лишних зависимостей. Сделана для уроков и наборов Alash Electronics.

```cpp
#include <AlashButton.h>
AlashButton button(2);             // кнопка между пином 2 и GND

void setup() { button.begin(); Serial.begin(115200); }
void loop() {
  if (button.clicked()) Serial.println("Клик");
}
```

### Что умеет
- **Не дребезжит:** контакт считается нажатым, когда уровень устойчив 30 мс (`setDebounceMs()`).
- **События приходят один раз:** `pressed()`, `released()`, `clicked()`, `doubleClicked()`, `longPressed()`, `repeat()`. Не надо хранить прошлое состояние кнопки в своих переменных.
- **Состояние:** `isDown()`, `heldMs()` (сколько держат), `lastHeldMs()`.
- **Одна кнопка, три действия:** клик, двойной клик, долгое нажатие (`setDoubleClickMs(300)`, `setLongPressMs(800)`).
- **Удержание с повтором** для счётчиков: `setRepeat(500, 100)`, потом `repeat()`.
- **Обработчики** по желанию: `onClick(fn)`, `onLongPress(fn)`, `onDoubleClick(fn)`, `onPress(fn)`, `onRelease(fn)` и `update()` в `loop()`.
- **Сколько угодно кнопок:** каждая это отдельный объект, не мешают друг другу.
- **Режимы подключения:** `BTN_PULLUP` (по умолчанию, кнопка на GND, резистор не нужен), `BTN_PULLDOWN` (кнопка на питание), `BTN_EXTERNAL` (свой резистор).
- **Понятные подсказки** на русском, казахском и английском: `diagnose(Serial)` и `errorText()`. Например, предупредит, что у пинов ESP32 GPIO34-39 нет внутренней подтяжки.
- Работает на **Arduino AVR, ESP32, ESP8266** и других платах.

### Подключение
| Кнопка | Uno / Nano | ESP32 |
|---|---|---|
| ножка 1 | пин 2 | GPIO4 |
| ножка 2 | GND | GND |

Резистор не нужен: подтяжка включена внутри платы.

### Чем отличается от других библиотек
Мы посмотрели OneButton (Matthias Hertel), Bounce2 (Thomas Fredericks), AceButton (Brian Park) и GyverButton:
- В большинстве нужно прописать функции-обработчики и передать их в библиотеку; новичку проще `if (button.clicked())`. У нас можно и так, и так.
- Bounce2 только убирает дребезг: клик, двойной клик и долгое нажатие надо считать самому.
- Если включён двойной клик, одиночный приходит с задержкой. У нас задержка только когда двойной клик включён явно, по умолчанию клик приходит сразу.
- Ни в одной нет подсказок, что делать, если кнопка не реагирует: у нас `diagnose()`.
- Если зажать кнопку при включении платы, у нас не будет ложного нажатия и долгого нажатия.

### Примеры
`Basic`, `ClickLongDouble`, `Callbacks`, `Counter`, `TwoButtons`, `Diagnose`.

### Тесты
Дребезг, клик, двойной клик, долгое нажатие, повтор, обработчики, переполнение `millis()` проверены без железа: `extras/test`.

---

## Қазақша

Түймелерге арналған қарапайым кітапхана: дірілді (дребезг) жояды және **клик**, **қос клик**, **ұзақ басу**, **ұстап тұрып қайталауды** өзі ажыратады. `delay()` жоқ, артық тәуелділіксіз. Alash Electronics сабақтары мен жинақтарына арналған.

```cpp
#include <AlashButton.h>
AlashButton button(2);             // түйме 2-пин мен GND арасында

void setup() { button.begin(); Serial.begin(115200); }
void loop() {
  if (button.clicked()) Serial.println("Клик");
}
```

### Мүмкіндіктері
- **Діріл жоқ:** деңгей 30 мс тұрақты болса, түйме басылған деп саналады (`setDebounceMs()`).
- **Оқиғалар бір рет келеді:** `pressed()`, `released()`, `clicked()`, `doubleClicked()`, `longPressed()`, `repeat()`.
- **Күй:** `isDown()`, `heldMs()`, `lastHeldMs()`.
- **Бір түйме, үш әрекет:** клик, қос клик, ұзақ басу (`setDoubleClickMs(300)`, `setLongPressMs(800)`).
- **Қайталау:** `setRepeat(500, 100)` және `repeat()`.
- **Өңдеушілер** (қалауыңызша): `onClick(fn)`, `onLongPress(fn)`, `onDoubleClick(fn)` және `loop()` ішінде `update()`.
- **Қосу режимдері:** `BTN_PULLUP` (әдепкі, түйме GND-ға, резистор керек емес), `BTN_PULLDOWN`, `BTN_EXTERNAL`.
- **Түсінікті кеңестер** орысша, қазақша, ағылшынша: `diagnose(Serial)`, `errorText()`.
- **Arduino AVR, ESP32, ESP8266** тақталарында жұмыс істейді.

### Қосу
| Түйме | Uno / Nano | ESP32 |
|---|---|---|
| аяқ 1 | 2-пин | GPIO4 |
| аяқ 2 | GND | GND |

### Мысалдар
`Basic`, `ClickLongDouble`, `Callbacks`, `Counter`, `TwoButtons`, `Diagnose`.

---

## English

A simple button library: removes contact bounce and detects **click**, **double click**, **long press** and **hold-repeat** by itself. No `delay()`, no dependencies. Made for classes and Alash Electronics kits.

```cpp
#include <AlashButton.h>
AlashButton button(2);             // button between pin 2 and GND

void setup() { button.begin(); Serial.begin(115200); }
void loop() {
  if (button.clicked()) Serial.println("Click");
}
```

### Features
- **No bounce:** the button counts as pressed when the level is stable for 30 ms (`setDebounceMs()`).
- **Each event arrives once:** `pressed()`, `released()`, `clicked()`, `doubleClicked()`, `longPressed()`, `repeat()`.
- **State:** `isDown()`, `heldMs()`, `lastHeldMs()`.
- **One button, three actions:** click, double click, long press (`setDoubleClickMs(300)`, `setLongPressMs(800)`).
- **Hold-repeat** for counters: `setRepeat(500, 100)`, then `repeat()`.
- **Callbacks** if you like: `onClick(fn)`, `onLongPress(fn)`, `onDoubleClick(fn)`, and `update()` in `loop()`.
- **Wiring modes:** `BTN_PULLUP` (default, button to GND, no resistor), `BTN_PULLDOWN`, `BTN_EXTERNAL`.
- **Clear tips** in Russian, Kazakh and English: `diagnose(Serial)`, `errorText()`.
- Works on **Arduino AVR, ESP32, ESP8266** and others.

### Wiring
| Button | Uno / Nano | ESP32 |
|---|---|---|
| leg 1 | pin 2 | GPIO4 |
| leg 2 | GND | GND |

### Examples
`Basic`, `ClickLongDouble`, `Callbacks`, `Counter`, `TwoButtons`, `Diagnose`.

### License
MIT, Alash Electronics.
