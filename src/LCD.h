#include <LiquidCrystal_I2C.h>
#include "Move.h"
//#include "PRINTING.h"

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
uint16_t cipr = 0; // переменная количества печати в штуках
uint8_t pause = 0; // переменная количества выдержки

int stp = sf * 800;

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
    Forward();
  }
  if (glubina == 1 && strelkiLP == 2 && strelkiVN == 1)
  {
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
      startprint(cipr,ciex,pause);
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