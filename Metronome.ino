#include <TM1637Display.h>

#define CLK 3
#define DIO 4
#define BUZZ 1
#define POT A1
#define PB 0
int i = 0;
int beats = 1;
int measures = 4;

int highFreq = 5000;
int lowFreq = 4000;
int potVal;
float tempo = 120;

int state = false;

TM1637Display display(CLK, DIO);

void setup() {

  pinMode(BUZZ, OUTPUT); // Set buzzer - pin 9 as an output
  pinMode(POT, INPUT);
  pinMode(PB, INPUT);
  display.setBrightness(0x03);
}

void loop() {
  //  display.clear();
  if (!state)
    getTempo();
}

void getTempo()
{
  delay(500);
  while (1)
  {
    potVal = analogRead(POT);
    if (digitalRead(PB) == HIGH)
    {
      tempo = map(potVal, 0, 1023, 35, 260);
      break;
    }
    display.showNumberDec(map(potVal, 0, 1023, 35, 260), false);
    delay(200);
  }
  delay(500);
  display.clear();
  while (1)
  {
    potVal = analogRead(POT);
    if (digitalRead(PB) == HIGH)
    {
      beats = map(potVal, 0, 1023, 1, 10);
      break;
    }
    display.showNumberDecEx((map(potVal, 0, 1023, 1, 10)*10)+measures, 0b01000000, false, 3, 4);
    delay(200);
  }
  
  state = true;
  playMetronome();
}

void playMetronome()
{
  display.showNumberDec(tempo, false);
  while (1)
  {
    for (int i = 0; i < beats; i++)
    {
      if (i == 0)
      {
        tone(BUZZ, highFreq, 30.0); // Send 1KHz sound signal...
      }
      else
      {
        tone(BUZZ, lowFreq, 30);
      }
      delay(int((60 / tempo) * 1000));    // ...for 1 sec
      noTone(BUZZ);     // Stop sound...
      if (digitalRead(PB) == HIGH)
      {
        state = false;
        break;
      }
    }
    if (!state)
      getTempo();  
  }
}
