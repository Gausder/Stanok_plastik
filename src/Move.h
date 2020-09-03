
#define DEBUG_ENABLE

#ifdef DEBUG_ENABLE
#define DEBUG(x, y) \
  Serial.print(x);  \
  Serial.println(y)
#else
#define DEBUG(x, y)
#endif

//-----------------установка движков-------------
//-----------------Пресс
#define STEP0 11
#define DIR0 10
#define ENA0 12
//-----------------Экструдер
#define STEP1 6
#define DIR1 5
#define ENA1 4

#define STOPNA 7 //-----------------концевик начала
#define STOPKO 8 //-----------------концевик конца

boolean stopna = true;
boolean stopko = true;
boolean dovodpress = false;
boolean ostanovpress = false;
boolean stpprs = false;

uint32_t tme = 0;

//----------------------------медленно и без возможности остановить
void forwardpress(int stp)
{
  DEBUG("Функция вперед", "ПРЕСС");
  digitalWrite(ENA0, LOW);
  digitalWrite(DIR0, HIGH);
  for (int i = 0; i < stp; i++)
  {
    digitalWrite(STEP0, !digitalRead(STEP0));
    delay(5);
  }
  digitalWrite(ENA0, HIGH);
}

void reversepress(int stp)
{
  DEBUG("Функция назад", "ПРЕСС");
  digitalWrite(ENA0, LOW);
  digitalWrite(DIR0, LOW);
  for (int i = 0; i < stp; i++)
  {
    digitalWrite(STEP0, !digitalRead(STEP0));
    delay(5);
  }
  digitalWrite(ENA0, HIGH);
}

void forwardextruder(int stp)
{
  DEBUG("Функция вперед", "ЭКСТРУДЕР");
  digitalWrite(ENA1, LOW);
  digitalWrite(DIR1, HIGH);
  for (int i = 0; i < stp; i++)
  {
    digitalWrite(STEP1, !digitalRead(STEP1));
    delay(5);
  }
  digitalWrite(ENA1, HIGH);
}

void reverseextruder(int stp)
{
  DEBUG("Функция назад", "Экструдер");
  digitalWrite(ENA1, LOW);
  digitalWrite(DIR1, LOW);
  for (int i = 0; i < stp; i++)
  {
    digitalWrite(STEP1, !digitalRead(STEP1));
    delay(5);
  }
  digitalWrite(ENA1, HIGH);
}

//----------------------------быстро и с возможностью остановить
void forwardextruderfast()
{
  digitalWrite(ENA1, LOW);
  analogWrite(DIR1, 255);
  for (tme = 0; micros() - tme < 150;)
  {
  }
  analogWrite(STEP1, 1);
  tme = micros();
}

void reversextruderfast()
{
  digitalWrite(ENA1, LOW);
  analogWrite(DIR1, 0);
  for (tme = 0; micros() - tme < 150;)
  {
  }
  analogWrite(STEP1, 1);
  tme = micros();
}

void forwardpressfast()
{
  digitalWrite(ENA0, LOW);
  analogWrite(DIR0, 255);
  for (tme = 0; micros() - tme < 50;)
  {
  }
  analogWrite(STEP0, 1);
  tme = micros();
}

void reversepressfast()
{
  digitalWrite(ENA0, LOW);
  analogWrite(DIR0, 0);
  for (tme = 0; micros() - tme < 50;)
  {
  }
  analogWrite(STEP0, 1);
  tme = micros();
}

void stoppress()
{
  DEBUG("Функция остановка", "ПРЕСС");
  digitalWrite(ENA0, LOW);
  analogWrite(DIR0, 0);
  for (tme = 0; micros() - tme < 50;)
  {
  }
  analogWrite(STEP0, 0);
  tme = micros();
  stpprs = true;
}