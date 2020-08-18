#include <Arduino.h>
#include <Wire.h>
#include "LCD.h"
//#include "PresMotor.h"

#define STOPNA 7 //-----------------концевик начала
#define STOPKO 8 //-----------------концевик конца

bool stopna = false;
bool stopko = false;

//------------------создание символов--------------
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
  lcd.print("V.0.1");

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

  Serial.begin(115200);

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
}

void loop()
{

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
  if (STOPNA == 1)
  {
    stopna = true;
    stoppress();
    digitalWrite(ENA0, HIGH);
  }

  if (STOPKO == 1)
  {
    stopko = true;
    stoppress();
    forwardpress(200);
  }
}