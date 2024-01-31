const int latchPin = 11;
const int dataPin = 10;
const int clockPin = 12;

#define PIN4 4
#define PIN5 5
#define PIN6 6
#define PIN7 7
#define PIN8 8


#define REV_LCD_POS 8

uint8_t digitToSegMap[10] = {
  0b01110111,//0
  0b01000001,//1
  0b00111011,//2
  0b01101011,//3
  0b01001101,//4
  0b01101110,//5
  0b01111110,//6
  0b01000011,//7
  0b01111111,//8
  0b01101111//9
};
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(PIN4, OUTPUT);
  pinMode(PIN5, OUTPUT);
  pinMode(PIN6, OUTPUT);
  pinMode(PIN7, OUTPUT);
  pinMode(PIN8, OUTPUT);

  pinMode(latchPin, OUTPUT); //latch
  pinMode(dataPin, OUTPUT); //data
  pinMode(clockPin, OUTPUT); //clock
  digitalWrite(PIN4, LOW);
  digitalWrite(PIN5, LOW);

}


void showNum(int pos, int dataBits)
{
  uint32_t starttime = micros();
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, dataBits);
  digitalWrite(latchPin, HIGH);
  digitalWrite(pos, HIGH);
  //Serial.println(micros() - starttime);
}
const int GENERAL_DLY = 1;

const int msbPos = PIN4;
const int lsbPos = PIN5;

uint8_t segmentCorrRevLCD(uint8_t input)
{
  uint8_t bitPos4 = (input & 0xF) >> 3;
  uint8_t bitPos8 = (input & 0x80) >> 7;
  Serial.println(bitPos4);
  Serial.println(bitPos8);
  uint8_t out = (uint8_t)((uint8_t)bitPos4 << (uint8_t)7 | (uint8_t)bitPos8 << (uint8_t)3) | (uint8_t)((uint8_t)input & (uint8_t)0x77);
  return out;
}

void printNumLCD5Seg(uint32_t numToPrint, uint32_t duration)
{
  uint8_t databuf[5] = {0, 0, 0, 0, 0};
  //numToPrint = 99999;
  if (numToPrint >= 0 && numToPrint < 100)
  {
    databuf[0] = numToPrint % 10;
    databuf[1] = numToPrint / 10;
  }
  else if (numToPrint >= 100 && numToPrint < 1000)
  {
    uint32_t rem = numToPrint % 100;
    databuf[2] = numToPrint / 100;
    databuf[1] = rem / 10;
    databuf[0] = rem % 10;
  }
  else if (numToPrint >= 1000 && numToPrint < 10000)
  {

    uint32_t rem1 = numToPrint % 1000;
    databuf[3] = numToPrint / 1000;
    databuf[2] = rem1 / 100;
    uint16_t rem2 = rem1 % 100;
    databuf[1] = rem2 / 10;
    databuf[0] = rem2 % 10;
  }
  else
  {
    uint32_t rem1 = numToPrint % 10000;
    databuf[4] = numToPrint / 10000;
    databuf[3] = rem1 / 1000;
    uint16_t rem2 = rem1 % 1000;
    databuf[2] = rem2 / 100;
    uint8_t rem3 = rem2 % 100;
    databuf[1] = rem3 / 10;
    databuf[0] = rem3 % 10;
  }
  for (uint8_t i = 0; i < 5; i++)
  {
    Serial.print(databuf[i]) ;
  }
  Serial.println("");
  uint64_t startime = millis();
  while (millis() - startime < duration)
  {
    uint8_t j = 0;
    for ( uint8_t i = PIN4; i < PIN8 + 1; i++)
    {
      if (i == PIN6)
      {
        showNum(i, segmentCorrRevLCD(digitToSegMap[databuf[j]]) );
      }
      else
      {
        showNum(i, digitToSegMap[databuf[j]] );
      }
      delay(GENERAL_DLY);
      digitalWrite(i, LOW);
      j++;
    }


  }

}

void numberShow(uint32_t showTime)
{
  uint32_t i = 0;
  for (i = 0; i < 10; i++)
  {
    uint64_t startime = millis();
    uint8_t idx1 = i;
    uint8_t idx2 = i < 9 ? i + 1 : i;
    while (millis() - startime < showTime)
    {

      for (uint8_t i = PIN4; i < PIN8 + 1; i++) {
        if (i == PIN6) {
          showNum(i, segmentCorrRevLCD(digitToSegMap[idx1]));
        }
        else {
          showNum(i, digitToSegMap[idx1]);
        }

        delay(GENERAL_DLY);
        digitalWrite(i, LOW);
        delay(GENERAL_DLY);

      }
    }
  }
}

void loop() {

  //numberShow(500);
  delay(1000);

  for (uint32_t i = 0; i < 10000; i++)
  {

    printNumLCD5Seg(i, 1);
  }

}
