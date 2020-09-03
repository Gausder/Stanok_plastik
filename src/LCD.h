#include <LiquidCrystal_I2C.h>
#include "Move.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);

uint16_t period = 350;

int8_t strelkiVN = 0;
int8_t strelkiLP = 0;
int8_t glubina = 0;

boolean ok = false;        // Флаг ввода
boolean exfrwrd = false;   // Флаг ввода количества шагов вперед для экструдера
boolean exback = false;    // Флаг ввода количества шагов назад для экструдера
boolean excycle = false;   // Флаг ввода количества циклов дыижения экструдера
boolean prntcycle = false; // Флаг ввода количества печати в штуках
boolean prntpause = false; // Флаг ввода количества выдержки

uint8_t sf = 1;    // переменная шага вперед для экструдера
uint8_t sb = 1;    // переменная шага назад для экструдера
uint8_t ciex = 0;  // переменная циклов дыижения экструдера
uint16_t cipr = 1; // переменная количества печати в штуках
uint8_t pause = 1; // переменная количества выдержки

float vremya = 0;
int stp = sf * 800;

void startprint(int cipr, int ciex, int pause)
{
  DEBUG("startprint печатаем ", cipr);

  short schob = 0; //счётчик оборотов экструдера
  int sch = 0;     // счётчик напечатанного
  while (sch < cipr)
  {

    DEBUG("Погнали", sch);

    stopko = true;
    stopna = true;
    DEBUG("Функция назад", "ПРЕСС БЫСТРО");
    while (digitalRead(STOPKO) != 0)
    {
      reversepressfast(); // поехали пресом до концевика
    }

    if (digitalRead(STOPKO) == 0)
    {
      stopko = false;
      DEBUG("", "STOPKO");
      reversepress(200);
      DEBUG("Доехали", " и довели");
      stoppress();
    }

    for (uint32_t start = millis(); millis() - start < 1000;)
  {
  }

    DEBUG("Давим", "Экструдер один оборот");
    while (schob <= (ciex * 800))
    {
      forwardextruderfast();
      schob++;
    }
    DEBUG("Давим", "Экструдер 2 секунды");
    for (uint32_t start = millis(); millis() - start < 2000;) // 2 секунды давим пластик быстро
    {
      forwardextruderfast();
    }
    DEBUG("Давим", "Экструдер дожим 2 оборота");
    forwardextruder(1600); // дожим 2 оборота
    DEBUG("Ретракт", "Экструдер 1 оборот");
    reverseextruder(800); // ретракт 1 оборот
    DEBUG("Выдержка сек ", pause);

    for (uint32_t start = millis(); (millis() - start) < (pause * 1000);) // выдержка в секундах
    {
    }
    DEBUG("Откат", "прес до концевика");
    while (digitalRead(STOPNA) != 0)
    {
      forwardpressfast(); // откат преса до концевика
    }

    if (digitalRead(STOPNA) == 0)
    {
      stopna = false;
      stoppress();
      digitalWrite(ENA0, HIGH);
    }
    sch++;

    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("OTPECHATANO");
    lcd.setCursor(7, 2);
    lcd.print(sch);
  }
  reversepress(600);
  DEBUG("Доехали", " и довели");
  stoppress();
  vremya = (millis() / 60000.0);
  lcd.setCursor(9, 3);
  lcd.print(vremya);
  lcd.setCursor(4, 3);
  lcd.print("min:");
}

void Menu() // первый уровень
{
  glubina = 0;
  lcd.clear();
  lcd.setCursor(8, 0);
  lcd.print("MENU");
  lcd.setCursor(7, 1);
  lcd.print("Extruder");
  lcd.setCursor(7, 2);
  lcd.print("Press");
  lcd.setCursor(7, 3);
  lcd.print("Print");
}

void Extruder() //уровень 1.1
{
  glubina = 1;
  lcd.clear();
  lcd.setCursor(7, 0);
  lcd.print("EXTRUDER");
  lcd.setCursor(7, 1);
  lcd.print("Forward:");
  lcd.setCursor(7, 2);
  lcd.print("Backward:");
  lcd.setCursor(7, 3);
  lcd.print("Cycle:");
}

void Press() //уровень 1.2
{
  glubina = 2;
  lcd.clear();
  lcd.setCursor(8, 0);
  lcd.print("PRESS");
  lcd.setCursor(7, 1);
  lcd.print("Forward");
  lcd.setCursor(7, 2);
  lcd.print("Backward");
  lcd.setCursor(7, 3);
  lcd.print("Motor OFF");
}

void Prnt() //уровень 1.3
{
  glubina = 3;
  lcd.clear();
  lcd.setCursor(8, 0);
  lcd.print("PRINT");
  lcd.setCursor(7, 1);
  lcd.print("Kol-vo:");
  lcd.setCursor(7, 2);
  lcd.print("Pause:");
  lcd.setCursor(7, 3);
  lcd.print("Start");
}

void Forward() //уровень 1.1.1
{
  glubina = 1;
  lcd.clear();
  lcd.setCursor(7, 1);
  lcd.print("Forward");
  lcd.setCursor(9, 2);
  lcd.print(sf);
}

void Backward() //уровень 1.1.2
{
  glubina = 1;
  lcd.clear();
  lcd.setCursor(7, 1);
  lcd.print("Backward");
  lcd.setCursor(9, 2);
  lcd.print(sb);
}

void Cycle() //уровень 1.1.2
{
  glubina = 1;
  lcd.clear();
  lcd.setCursor(7, 1);
  lcd.print("Cycle");
  lcd.setCursor(9, 2);
  lcd.print(ciex);
}

void Kolichestvo() //уровень 1.3.1
{
  glubina = 3;
  lcd.clear();
  lcd.setCursor(7, 1);
  lcd.print("Kolichestvo");
  lcd.setCursor(9, 2);
  lcd.print(cipr);
}

void Viderjka() //уровень 1.3.2
{
  glubina = 3;
  lcd.clear();
  lcd.setCursor(7, 1);
  lcd.print("Pause");
  lcd.setCursor(9, 2);
  lcd.print(pause);
}

void proverka()
{
  if (strelkiVN < 0)
  {
    strelkiVN = 0;
  }
  if (strelkiLP < 0)
  {
    strelkiLP = 0;
  }
  if (glubina < 0)
  {
    glubina = 0;
  }

  if (strelkiLP > 2)
  {
    strelkiLP = 2;
  }
  if (strelkiVN > 2)
  {
    strelkiVN = 2;
  }
  if (glubina > 3)
  {
    glubina = 3;
  }

  //-------------------------------------перелистывание меню-----------------
  if (strelkiLP == 0 && strelkiVN == 0)
  {
    Menu();
    lcd.setCursor(5, 1);
    lcd.print("-");
  }
  if (strelkiLP == 0 && strelkiVN == 1)
  {
    Menu();
    lcd.setCursor(5, 2);
    lcd.print("-");
  }
  if (strelkiLP == 0 && strelkiVN == 2)
  {
    Menu();
    lcd.setCursor(5, 3);
    lcd.print("-");
  }

  //----------------------------------------выбор в меню---------------------
  if (glubina == 0 && strelkiLP == 1 && strelkiVN == 0)
  {
    Extruder();
    lcd.setCursor(5, 1);
    lcd.print("-");
  }
  if (glubina == 0 && strelkiLP == 1 && strelkiVN == 1)
  {
    Press();
    lcd.setCursor(5, 1);
    lcd.print("-");
  }
  if (glubina == 0 && strelkiLP == 1 && strelkiVN == 2)
  {
    Prnt();
    lcd.setCursor(5, 1);
    lcd.print("-");
  }

  //--------------------------------------перелистывание первого пункта------
  if (glubina == 1 && strelkiLP == 1 && strelkiVN == 0)
  {
    Extruder();
    lcd.setCursor(5, 1);
    lcd.print("-");
  }
  if (glubina == 1 && strelkiLP == 1 && strelkiVN == 1)
  {
    Extruder();
    lcd.setCursor(5, 2);
    lcd.print("-");
  }
  if (glubina == 1 && strelkiLP == 1 && strelkiVN == 2)
  {
    Extruder();
    lcd.setCursor(5, 3);
    lcd.print("-");
  }

  //-------------------------------------выбор подпунктов 1.1; 1.2; 1.3.-----
  if (glubina == 1 && strelkiLP == 2 && strelkiVN == 0)
  {
    DEBUG("выбор ", "Forward");
    Forward();
  }
  if (glubina == 1 && strelkiLP == 2 && strelkiVN == 1)
  {
    DEBUG("выбор ", "Backward");
    Backward();
  }
  if (glubina == 1 && strelkiLP == 2 && strelkiVN == 2)
  {
    Cycle();
  }

  //----------------------------- ввод значениея первого пункта--------------
  if (glubina == 1 && strelkiLP == 2 && strelkiVN == 0 && ok == 1)
  {
    exfrwrd = !exfrwrd;
    Forward();
  }
  if (glubina == 1 && strelkiLP == 2 && strelkiVN == 1 && ok == 1)
  {
    exback = !exback;
    Backward();
  }
  if (glubina == 1 && strelkiLP == 2 && strelkiVN == 2 && ok == 1)
  {
    excycle = !excycle;
    Cycle();
  }

  //-------------------------------------перелистывание второго пункта-------
  if (glubina == 2 && strelkiLP == 1 && strelkiVN == 0)
  {
    Press();
    lcd.setCursor(5, 1);
    lcd.print("-");
  }
  if (glubina == 2 && strelkiLP == 1 && strelkiVN == 1)
  {
    Press();
    lcd.setCursor(5, 2);
    lcd.print("-");
  }
  if (glubina == 2 && strelkiLP == 1 && strelkiVN == 2)
  {
    Press();
    lcd.setCursor(5, 3);
    lcd.print("-");
  }

  //-------------------------------------выбор подпунктов 2.1; 2.2; 2.3.-----
  if (glubina == 2 && strelkiLP == 2 && strelkiVN == 0)
  {
    lcd.clear();
    lcd.setCursor(4, 1);
    lcd.print("ForwarD press");
    forwardpressfast();
  }
  if (glubina == 2 && strelkiLP == 2 && strelkiVN == 1)
  {
    lcd.clear();
    lcd.setCursor(4, 2);
    lcd.print("Backword press");
    reversepressfast();
  }
  if (glubina == 2 && strelkiLP == 2 && strelkiVN == 2)
  {
    lcd.clear();
    lcd.setCursor(4, 3);
    lcd.print("Motor OFF");
    stoppress();
  }

  //-----------------------------------перелистывание третьего пункта--------
  if (glubina == 3 && strelkiLP == 1 && strelkiVN == 0)
  {
    Prnt();
    lcd.setCursor(5, 1);
    lcd.print("-");
  }
  if (glubina == 3 && strelkiLP == 1 && strelkiVN == 1)
  {
    Prnt();
    lcd.setCursor(5, 2);
    lcd.print("-");
  }
  if (glubina == 3 && strelkiLP == 1 && strelkiVN == 2)
  {
    Prnt();
    lcd.setCursor(5, 3);
    lcd.print("-");
  }

  //-------------------------------------выбор подпунктов 3.1; 3.2; 3.3.-----
  if (glubina == 3 && strelkiLP == 2 && strelkiVN == 0)
  {
    Kolichestvo();
  }
  if (glubina == 3 && strelkiLP == 2 && strelkiVN == 1)
  {
    lcd.clear();
    lcd.setCursor(4, 2);
    lcd.print("Zaderjka");
  }
  if (glubina == 3 && strelkiLP == 2 && strelkiVN == 2)
  {
    lcd.clear();
    lcd.setCursor(4, 2);
    lcd.print("Start");
  }

  //----------------------------- ввод значениея третьего пункта-------------
  if (glubina == 3 && strelkiLP == 2 && strelkiVN == 0 && ok == 1)
  {
    prntcycle = !prntcycle;
    Kolichestvo();
  }
  if (glubina == 3 && strelkiLP == 2 && strelkiVN == 1 && ok == 1)
  {
    prntpause = !prntpause;
    Viderjka();
  }
  if (glubina == 3 && strelkiLP == 2 && strelkiVN == 2 && ok == 1)
  {

    lcd.clear();
    lcd.setCursor(4, 1);
    lcd.print("Konceviki ne");
    lcd.setCursor(4, 2);
    lcd.print("ustanovleni");

    if (stopna == false && stopko == false)
    {
      lcd.clear();
      lcd.setCursor(4, 2);
      lcd.print("Start");
      startprint(cipr, ciex, pause);
    }
  }
}

void Vverh()
{
  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.print("*");
  lcd.setCursor(0, 2);
  lcd.write(byte(1));
  lcd.setCursor(2, 2);
  lcd.write(byte(3));
  lcd.setCursor(1, 3);
  lcd.write(byte(2));

  for (uint32_t start = millis(); millis() - start < period;)
  {
  }

  strelkiVN--;
  proverka();
  lcd.setCursor(1, 1);
  lcd.write(byte(0));
  lcd.setCursor(0, 2);
  lcd.write(byte(1));
  lcd.setCursor(2, 2);
  lcd.write(byte(3));
  lcd.setCursor(1, 3);
  lcd.write(byte(2));
}

void Vniz()
{
  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.write(byte(0));
  lcd.setCursor(0, 2);
  lcd.write(byte(1));
  lcd.setCursor(2, 2);
  lcd.write(byte(3));
  lcd.setCursor(1, 3);
  lcd.print("*");

  for (uint32_t start = millis(); millis() - start < period;)
  {
  }

  strelkiVN++;
  proverka();
  lcd.setCursor(1, 1);
  lcd.write(byte(0));
  lcd.setCursor(0, 2);
  lcd.write(byte(1));
  lcd.setCursor(2, 2);
  lcd.write(byte(3));
  lcd.setCursor(1, 3);
  lcd.write(byte(2));
}

void Vlevo()
{

  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.write(byte(0));
  lcd.setCursor(0, 2);
  lcd.print("*");
  lcd.setCursor(2, 2);
  lcd.write(byte(3));
  lcd.setCursor(1, 3);
  lcd.write(byte(2));

  for (uint32_t start = millis(); millis() - start < period;)
  {
  }

  strelkiLP--;
  proverka();
  lcd.setCursor(1, 1);
  lcd.write(byte(0));
  lcd.setCursor(0, 2);
  lcd.write(byte(1));
  lcd.setCursor(2, 2);
  lcd.write(byte(3));
  lcd.setCursor(1, 3);
  lcd.write(byte(2));
}

void Vpravo()
{
  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.write(byte(0));
  lcd.setCursor(0, 2);
  lcd.write(byte(1));
  lcd.setCursor(2, 2);
  lcd.print("*");
  lcd.setCursor(1, 3);
  lcd.write(byte(2));

  for (uint32_t start = millis(); millis() - start < period;)
  {
  }

  strelkiLP++;
  proverka();
  lcd.setCursor(1, 1);
  lcd.write(byte(0));
  lcd.setCursor(0, 2);
  lcd.write(byte(1));
  lcd.setCursor(2, 2);
  lcd.write(byte(3));
  lcd.setCursor(1, 3);
  lcd.write(byte(2));
}

void Vvod()
{
  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.write(byte(0));
  lcd.setCursor(0, 2);
  lcd.write(byte(1));
  lcd.setCursor(2, 2);
  lcd.write(byte(3));
  lcd.setCursor(1, 3);
  lcd.write(byte(2));
  lcd.setCursor(1, 2);
  lcd.print("*");

  for (uint32_t start = millis(); millis() - start < period;)
  {
  }

  ok = true;
  proverka();
  lcd.setCursor(1, 1);
  lcd.write(byte(0));
  lcd.setCursor(0, 2);
  lcd.write(byte(1));
  lcd.setCursor(2, 2);
  lcd.write(byte(3));
  lcd.setCursor(1, 3);
  lcd.write(byte(2));
  ok = false;
}
