#include <Arduino.h>
#include <Wire.h>
#include "LCD.h"

//-----------------------Отладка-------------------
#define DEBUG_ENABLE

#ifdef DEBUG_ENABLE
#define DEBUG(x, y) \
  Serial.print(x);  \
  Serial.println(y)
#else
#define DEBUG(x, y)
#endif

//------------------создание символов---------------
byte strelka1[8] = {
    0b00100,
    0b00100,
    0b01110,
    0b11111,
    0b11111,
    0b00100,
    0b00100,
    0b00100};
byte strelka2[8] = {
    0b00000,
    0b00100,
    0b01100,
    0b11111,
    0b11111,
    0b01100,
    0b00100,
    0b00000};
byte strelka3[8] = {
    0b00100,
    0b00100,
    0b00100,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00100};
byte strelka4[8] = {
    0b00000,
    0b00100,
    0b00110,
    0b11111,
    0b11111,
    0b00110,
    0b00100,
    0b00000};

byte b1[8] = {
    0b00000,
    0b11111,
    0b10000,
    0b10111,
    0b10100,
    0b10111,
    0b10101,
    0b10111};
byte o1[8] = {
    0b00000,
    0b11111,
    0b00001,
    0b11101,
    0b10101,
    0b10101,
    0b10101,
    0b11101};
byte o2[8] = {
    0b11101,
    0b10101,
    0b10101,
    0b10101,
    0b11101,
    0b00001,
    0b11111,
    0b00000};
byte p1[8] = {
    0b10111,
    0b10101,
    0b10111,
    0b10100,
    0b10100,
    0b10000,
    0b11111,
    0b00000};

void setup()
{

  lcd.init();
  lcd.backlight();

  lcd.createChar(4, b1);
  lcd.createChar(5, o1);
  lcd.createChar(6, o2);
  lcd.createChar(7, p1);
  lcd.setCursor(15, 3);
  lcd.print("V.2.1");

  lcd.setCursor(9, 1);
  lcd.write(byte(4));
  lcd.setCursor(10, 1);
  lcd.write(byte(5));
  lcd.setCursor(9, 2);
  lcd.write(byte(7));
  lcd.setCursor(10, 2);
  lcd.write(byte(6));

  pinMode(STEP0, OUTPUT);
  pinMode(DIR0, OUTPUT);
  pinMode(ENA0, OUTPUT);

  pinMode(STEP1, OUTPUT);
  pinMode(DIR1, OUTPUT);
  pinMode(ENA1, OUTPUT);

  pinMode(STOPNA, INPUT);
  pinMode(STOPKO, INPUT);

#ifdef DEBUG_ENABLE
  Serial.begin(115200);
#endif

  for (uint32_t start = millis(); millis() - start < 500;)
  {
  }

  lcd.createChar(0, strelka1);
  lcd.createChar(1, strelka2);
  lcd.createChar(2, strelka3);
  lcd.createChar(3, strelka4);

  lcd.clear();

  Menu();
  lcd.setCursor(5, 1);
  lcd.print("-");

  lcd.setCursor(1, 1);
  lcd.write(byte(0));
  lcd.setCursor(0, 2);
  lcd.write(byte(1));
  lcd.setCursor(2, 2);
  lcd.write(byte(3));
  lcd.setCursor(1, 3);
  lcd.write(byte(2));

  DEBUG("Версия от 03.09.20", " ");
}

void loop()
{

#ifdef DEBUG_ENABLE
  if (Serial.available() > 0)
  {
    char io = Serial.read();
    DEBUG("Прочитано ", io);
    if (io == 'a') //-----------------------Влево
    {
      if (!exfrwrd && !exback && !excycle && !prntcycle && !prntpause)
      {
        Vlevo();
      }
    }
    else if (io == 'w') //-----------------------Вверх
    {
      if (!exfrwrd && !exback && !excycle && !prntcycle && !prntpause)
      {
        Vverh();
      }
    }
    else if (io == 'e') //-----------------------Ввод
    {
      Vvod();
    }
    else if (io == 's') //-----------------------Вниз
    {
      if (!exfrwrd && !exback && !excycle && !prntcycle && !prntpause)
      {
        Vniz();
      }
    }
    else if (io == 'd') //-----------------------Вправо
    {
      if (!exfrwrd && !exback && !excycle && !prntcycle && !prntpause)
      {
        Vpravo();
      }
    }
  }
#endif

  uint16_t sV = analogRead(A0);

  if (sV > 680 && sV < 730) //-----------------------Влево
  {
    if (!exfrwrd && !exback && !excycle && !prntcycle && !prntpause)
    {
      Vlevo();
    }
  }
  else if (sV > 500 && sV < 550) //-----------------------Вверх
  {
    if (!exfrwrd && !exback && !excycle && !prntcycle && !prntpause)
    {
      Vverh();
    }
  }
  else if (sV > 800) //-----------------------Ввод
  {
    Vvod();
  }
  else if (sV > 331 && sV < 399) //-----------------------Вниз
  {
    if (!exfrwrd && !exback && !excycle && !prntcycle && !prntpause)
    {
      Vniz();
    }
  }
  else if (sV > 400 && sV < 450) //-----------------------Вправо
  {
    if (!exfrwrd && !exback && !excycle && !prntcycle && !prntpause)
    {
      Vpravo();
    }
  }

  if (exfrwrd)
  {
    Forward();
    for (uint32_t start = millis(); millis() - start < 80;)
    {
    }
    if (sV > 500 && sV < 550)
    {
      sf++;
    }
    else if (sV > 331 && sV < 399)
    {
      sf--;
    }
  }

  if (exback)
  {
    Backward();
    for (uint32_t start = millis(); millis() - start < 80;)
    {
    }
    if (sV > 500 && sV < 550)
    {
      sb++;
    }
    else if (sV > 331 && sV < 399)
    {
      sb--;
    }
  }

  if (excycle)
  {
    Cycle();
    for (uint32_t start = millis(); millis() - start < 80;)
    {
    }
    if (sV > 500 && sV < 550)
    {
      ciex++;
    }
    else if (sV > 331 && sV < 399)
    {
      ciex--;
    }
  }

  if (prntcycle)
  {
    Kolichestvo();
    for (uint32_t start = millis(); millis() - start < 80;)
    {
    }
    if (sV > 500 && sV < 550)
    {
      cipr++;
    }
    else if (sV > 331 && sV < 399)
    {
      cipr--;
    }
  }

  if (prntpause)
  {
    Viderjka();
    for (uint32_t start = millis(); millis() - start < 80;)
    {
    }
    if (sV > 500 && sV < 550)
    {
      pause++;
    }
    else if (sV > 331 && sV < 399)
    {
      pause--;
    }
  }

  //-----------Установка концевиков

  if (digitalRead(STOPNA) == 0)
  {
    stopna = false;
    if (!stpprs)
    {
      stoppress();
      digitalWrite(ENA0, HIGH);
    }

    DEBUG("Концевик ", "Начало");
  }

  if (digitalRead(STOPKO) == 0)
  {
    stopko = false;
    if (!stpprs)
    {
      stoppress();
    }
    DEBUG("Концевик ", "Конец");
  }

  if (stopko == false && digitalRead(STOPKO) == 0 && dovodpress == false)
  {
    dovodpress = true;
    reversepress(200);
    stoppress();
  }
  if (stopko == false && digitalRead(STOPKO) != 0 && dovodpress == true)
  {
    stpprs = false;
    dovodpress = false;
    DEBUG("СТОП ПРЕС", " активна");
  }
  if (stopna == false && digitalRead(STOPNA) == 0 && ostanovpress == false)
  {
    ostanovpress = true;
  }
  if (stopna == false && digitalRead(STOPNA) != 0 && ostanovpress == true)
  {
    stpprs = false;
    ostanovpress = false;
    DEBUG("СТОП ПРЕС", " активна");
  }
}