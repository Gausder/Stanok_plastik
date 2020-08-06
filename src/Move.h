//-----------------установка движков-------------
//-----------------Пресс
#define STEP0 11
#define DIR0 10
#define ENA0 12
//-----------------Экструдер
#define STEP1 6
#define DIR1 5
#define ENA1 4

uint32_t tme = 0;

//----------------------------медленно и без возможности остановить
void forwardpress(int stp)
{
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
  for (tme = 0; micros() - tme < 150;)
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
  digitalWrite(ENA0, LOW);
  analogWrite(DIR0, 0);
  for (tme = 0; micros() - tme < 50;)
  {
  }
  analogWrite(STEP0, 0);
  tme = micros();
}