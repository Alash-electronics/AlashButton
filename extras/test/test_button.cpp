// Тесты AlashButton без железа (время и уровень пина задаёт тест):
//   g++ -std=c++17 -Wall -I mock -I ../../src test_button.cpp ../../src/AlashButton.cpp && ./a.out
#include <stdio.h>
#include "Arduino.h"
#include "AlashButton.h"

static int fails = 0;
#define CHECK(cond) do { if (!(cond)) { printf("FAIL line %d: %s\n", __LINE__, #cond); fails++; } } while (0)

// Пройти время ms миллисекундами по 1 мс, опрашивая кнопки; собираем события в счётчики
struct Counts { int press = 0, rel = 0, click = 0, dbl = 0, lng = 0, rep = 0; };
static void run(AlashButton &b, Counts &c, uint32_t ms) {
  for (uint32_t i = 0; i < ms; i++) {
    g_now++;
    if (b.pressed()) c.press++;
    if (b.released()) c.rel++;
    if (b.clicked()) c.click++;
    if (b.doubleClicked()) c.dbl++;
    if (b.longPressed()) c.lng++;
    if (b.repeat()) c.rep++;
  }
}
static void setLevel(uint8_t pin, bool pressedActiveLow) { g_level[pin] = pressedActiveLow ? 0 : 1; }

static int cbClick = 0, cbLong = 0, cbDouble = 0;
static void onClick() { cbClick++; }
static void onLong() { cbLong++; }
static void onDouble() { cbDouble++; }

int main() {
  // 1) короткий клик: одно событие pressed/released/clicked, дребезг не считается
  g_now = 1000; g_level[4] = 1;
  AlashButton b(4);
  CHECK(b.begin());
  CHECK(g_mode[4] == INPUT_PULLUP);
  Counts c;
  run(b, c, 100);
  setLevel(4, true);  run(b, c, 3);                    // дребезг при нажатии
  setLevel(4, false); run(b, c, 2);
  setLevel(4, true);  run(b, c, 4);
  setLevel(4, false); run(b, c, 3);
  setLevel(4, true);  run(b, c, 150);                  // реально нажата
  CHECK(c.press == 1 && b.isDown());
  setLevel(4, false); run(b, c, 3);
  setLevel(4, true);  run(b, c, 2);                    // дребезг при отпускании
  setLevel(4, false); run(b, c, 100);
  CHECK(c.press == 1 && c.rel == 1 && c.click == 1 && c.lng == 0 && !b.isDown());
  CHECK(b.lastHeldMs() >= 150 && b.lastHeldMs() < 200);

  // 2) помеха короче времени подавления дребезга игнорируется
  Counts n;
  setLevel(4, true); run(b, n, 10); setLevel(4, false); run(b, n, 200);
  CHECK(n.press == 0 && n.click == 0);

  // 3) долгое нажатие: longPressed один раз, клика после отпускания нет
  Counts l;
  setLevel(4, true); run(b, l, 1500);
  CHECK(l.press == 1 && l.lng == 1 && l.click == 0);
  CHECK(b.heldMs() >= 1450);
  setLevel(4, false); run(b, l, 100);
  CHECK(l.rel == 1 && l.click == 0 && b.heldMs() == 0);

  // 4) повтор при удержании: через 500 мс, затем каждые 150 мс
  AlashButton r(5);
  g_level[5] = 1; r.begin(); r.setRepeat(500, 150);
  Counts rc;
  setLevel(5, true); run(r, rc, 1500);
  CHECK(rc.rep >= 6 && rc.rep <= 7);                  // (1500-30-500)/150 + 1
  setLevel(5, false); run(r, rc, 100);
  CHECK(rc.click == 0);                               // после повторов клика при отпускании нет
  AlashButton nr(11);                                 // повтор выключен по умолчанию
  g_level[11] = 1; nr.begin();
  Counts nrc;
  setLevel(11, true); run(nr, nrc, 650); setLevel(11, false); run(nr, nrc, 100);
  CHECK(nrc.rep == 0 && nrc.click == 1);

  // 5) двойной клик включён: клик приходит с задержкой, двойной клик даёт одно событие, а не два клика
  AlashButton d(6);
  g_level[6] = 1; d.begin(); d.setDoubleClickMs(300);
  Counts dc;
  setLevel(6, true); run(d, dc, 80); setLevel(6, false); run(d, dc, 100);
  setLevel(6, true); run(d, dc, 80); setLevel(6, false); run(d, dc, 500);
  CHECK(dc.dbl == 1 && dc.click == 0);
  Counts sc;
  setLevel(6, true); run(d, sc, 80); setLevel(6, false); run(d, sc, 200);
  CHECK(sc.click == 0);                                // ещё ждём возможный второй клик
  run(d, sc, 300);
  CHECK(sc.click == 1 && sc.dbl == 0);                 // дождались: одиночный клик
  Counts slow;
  setLevel(6, true); run(d, slow, 80); setLevel(6, false); run(d, slow, 400);
  setLevel(6, true); run(d, slow, 80); setLevel(6, false); run(d, slow, 500);
  CHECK(slow.click == 2 && slow.dbl == 0);             // слишком медленно: два одиночных клика

  // 6) обработчики
  AlashButton h(7);
  g_level[7] = 1; h.begin();
  h.onClick(onClick); h.onLongPress(onLong); h.onDoubleClick(onDouble);
  Counts hc;
  setLevel(7, true); run(h, hc, 100); setLevel(7, false); run(h, hc, 100);
  CHECK(cbClick == 1);
  setLevel(7, true); run(h, hc, 1000); setLevel(7, false); run(h, hc, 100);
  CHECK(cbLong == 1 && cbClick == 1);

  // 7) активный HIGH (кнопка на питание, BTN_PULLDOWN)
  AlashButton hi(8, AlashButton::BTN_PULLDOWN);
  g_level[8] = 0; hi.begin();
  Counts hic;
  g_level[8] = 1; run(hi, hic, 100); g_level[8] = 0; run(hi, hic, 100);
  CHECK(hic.press == 1 && hic.click == 1);

  // 8) нажата при старте: события нажатия нет, долгое не срабатывает
  g_level[9] = 0;
  AlashButton s(9);
  s.begin();
  Counts sc2;
  run(s, sc2, 2000);
  CHECK(s.isDown() && sc2.press == 0 && sc2.lng == 0);
  g_level[9] = 1; run(s, sc2, 100);
  CHECK(sc2.rel == 1 && sc2.click == 0);              // после долгого удержания с самого старта клик не засчитан

  // 9) переполнение millis() (через 49 суток) не ломает счёт времени
  g_now = 0xFFFFFFFFu - 200; g_level[10] = 1;
  AlashButton w(10); w.begin();
  Counts wc;
  setLevel(10, true); run(w, wc, 1200);
  CHECK(wc.press == 1 && wc.lng == 1);

  // 10) тексты ошибок и diagnose() работают
  w.setLanguage(AlashButton::LANG_KK);
  w.diagnose(*(new Print));

  if (fails) { printf("FAILED: %d\n", fails); return 1; }
  printf("ALL TESTS PASSED\n");
  return 0;
}
