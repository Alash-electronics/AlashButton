# AlashButton: руководство / нұсқаулық / guide

[Русский](#русский) | [Қазақша](#қазақша) | [English](#english)

---

## Русский

Простая библиотека для кнопок: убирает дребезг и сама различает **клик**, **двойной клик**, **долгое нажатие** и **удержание с повтором**. Без `delay()`, без лишних зависимостей. Сделана для уроков и наборов Alash Electronics.

### Установка
Arduino IDE: Скетч, Подключить библиотеку, Управлять библиотеками, найти **AlashButton** и нажать Установить. Или скачайте ZIP с GitHub: Скетч, Подключить библиотеку, Добавить .ZIP библиотеку.

### Платы
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 и другие, где работает Arduino-ядро.

### Подключение
| Кнопка | Uno / Nano | ESP32 |
|---|---|---|
| ножка 1 | пин 2 | GPIO4 |
| ножка 2 | GND | GND |

Резистор не нужен: подтяжка включена внутри платы.

### Быстрый старт
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

### Справочник по методам
| Метод | Что делает |
|---|---|
| `begin()` | Настроить пин (можно не вызывать) |
| `clicked()` | Короткий клик (один раз) |
| `doubleClicked()` | Двойной клик (нужен setDoubleClickMs) |
| `longPressed()` | Долгое нажатие (один раз) |
| `pressed() / released()` | Момент нажатия и отпускания |
| `repeat()` | Повтор при удержании (setRepeat) |
| `isDown() / heldMs()` | Нажата ли и сколько держат |
| `setDebounceMs / setLongPressMs / setDoubleClickMs` | Времена: дребезг, долгое нажатие, двойной клик |
| `onClick / onLongPress / onDoubleClick` | Обработчики событий (по желанию) |
| `diagnose(Serial)` | Самопроверка |

### Если что-то не работает
**Кнопка не реагирует**

Кнопка между пином и GND, режим `BTN_PULLUP` (по умолчанию). Если кнопка на питание, нужен `BTN_PULLDOWN` и резистор на GND. Запустите пример Diagnose: он покажет уровень на пине.

**ESP32: пины 34-39 не работают с кнопкой**

У GPIO34-39 нет внутренней подтяжки: поставьте внешний резистор 10 кОм к питанию и режим `BTN_EXTERNAL`.

**Клик приходит с задержкой**

Задержка есть только при включённом двойном клике (`setDoubleClickMs`): библиотека ждёт, не будет ли второго. Не включайте его, если двойной клик не нужен.

### Чем отличается от других библиотек
Мы посмотрели OneButton (Matthias Hertel), Bounce2 (Thomas Fredericks), AceButton (Brian Park) и GyverButton:
- В большинстве нужно прописать функции-обработчики и передать их в библиотеку; новичку проще `if (button.clicked())`. У нас можно и так, и так.
- Bounce2 только убирает дребезг: клик, двойной клик и долгое нажатие надо считать самому.
- Если включён двойной клик, одиночный приходит с задержкой. У нас задержка только когда двойной клик включён явно, по умолчанию клик приходит сразу.
- Ни в одной нет подсказок, что делать, если кнопка не реагирует: у нас `diagnose()`.
- Если зажать кнопку при включении платы, у нас не будет ложного нажатия и долгого нажатия.

### Самопроверка
В библиотеке есть `diagnose(Serial)`: она печатает, что видит плата, и подсказывает, что проверить. Язык подсказок: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Примеры
`Basic`, `ClickLongDouble`, `Callbacks`, `Counter`, `TwoButtons`, `Diagnose`.

---

## Қазақша

Түймелерге арналған қарапайым кітапхана: дірілді (дребезг) жояды және **клик**, **қос клик**, **ұзақ басу**, **ұстап тұрып қайталауды** өзі ажыратады. `delay()` жоқ, артық тәуелділіксіз. Alash Electronics сабақтары мен жинақтарына арналған.

### Орнату
Arduino IDE: Sketch, Include Library, Manage Libraries, **AlashButton** табып, Install басыңыз. Немесе GitHub-тан ZIP жүктеп, Sketch, Include Library, Add .ZIP Library.

### Тақталар
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 және Arduino ядросы бар басқалар.

### Қосу
| Түйме | Uno / Nano | ESP32 |
|---|---|---|
| аяқ 1 | 2-пин | GPIO4 |
| аяқ 2 | GND | GND |

### Жылдам бастау
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

### Әдістер анықтамасы
| Әдіс | Не істейді |
|---|---|
| `begin()` | Пинді баптау (шақырмауға болады) |
| `clicked()` | Қысқа клик (бір рет) |
| `doubleClicked()` | Қос клик (setDoubleClickMs керек) |
| `longPressed()` | Ұзақ басу (бір рет) |
| `pressed() / released()` | Басу және жіберу сәті |
| `repeat()` | Ұстап тұрғанда қайталау (setRepeat) |
| `isDown() / heldMs()` | Басулы ма және қанша ұстайды |
| `setDebounceMs / setLongPressMs / setDoubleClickMs` | Уақыттар: діріл, ұзақ басу, қос клик |
| `onClick / onLongPress / onDoubleClick` | Оқиға өңдеушілері (қалауынша) |
| `diagnose(Serial)` | Өзін тексеру |

### Бірдеңе жұмыс істемесе
**Түйме жауап бермейді**

Түйме пин мен GND арасында, `BTN_PULLUP` режимі. Түйме қорекке қосылса, `BTN_PULLDOWN` және GND-ге резистор керек. Diagnose мысалын іске қосыңыз.

**ESP32: 34-39 пиндер түймемен жұмыс істемейді**

GPIO34-39 ішкі тартуы жоқ: қорекке 10 кОм сыртқы резистор қойып, `BTN_EXTERNAL` қолданыңыз.

**Клик кешігіп келеді**

Кідіріс тек қос клик қосылғанда (`setDoubleClickMs`) болады. Қос клик керек болмаса, қоспаңыз.

### Өзін-өзі тексеру
Кітапханада `diagnose(Serial)` бар: тақта не көргенін басып шығарады және нені тексеру керегін айтады. Кеңес тілі: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Мысалдар
`Basic`, `ClickLongDouble`, `Callbacks`, `Counter`, `TwoButtons`, `Diagnose`.

---

## English

A simple button library: removes contact bounce and detects **click**, **double click**, **long press** and **hold-repeat** by itself. No `delay()`, no dependencies. Made for classes and Alash Electronics kits.

### Installation
Arduino IDE: Sketch, Include Library, Manage Libraries, search **AlashButton** and click Install. Or download the ZIP from GitHub: Sketch, Include Library, Add .ZIP Library.

### Boards
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 and others with an Arduino core.

### Wiring
| Button | Uno / Nano | ESP32 |
|---|---|---|
| leg 1 | pin 2 | GPIO4 |
| leg 2 | GND | GND |

### Quick start
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

### Method reference
| Method | What it does |
|---|---|
| `begin()` | Set up the pin (optional) |
| `clicked()` | Short click (once) |
| `doubleClicked()` | Double click (needs setDoubleClickMs) |
| `longPressed()` | Long press (once) |
| `pressed() / released()` | The moment of press and release |
| `repeat()` | Repeat while held (setRepeat) |
| `isDown() / heldMs()` | Is it down and for how long |
| `setDebounceMs / setLongPressMs / setDoubleClickMs` | Times: debounce, long press, double click |
| `onClick / onLongPress / onDoubleClick` | Event handlers (optional) |
| `diagnose(Serial)` | Self-check |

### If something does not work
**The button does not react**

The button goes between the pin and GND, mode `BTN_PULLUP` (default). For a button to power use `BTN_PULLDOWN` with a resistor to GND. Run the Diagnose example: it shows the pin level.

**ESP32: pins 34-39 do not work with a button**

GPIO34-39 have no internal pull-up: add a 10 kOhm resistor to power and use `BTN_EXTERNAL`.

**The click comes late**

There is a delay only when double click is on (`setDoubleClickMs`): the library waits for a second click. Leave it off if you do not need it.

### Self-check
The library has `diagnose(Serial)`: it prints what the board sees and tells what to check. Tips language: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Examples
`Basic`, `ClickLongDouble`, `Callbacks`, `Counter`, `TwoButtons`, `Diagnose`.

---

MIT License, Alash Electronics. https://github.com/Alash-electronics/AlashButton
