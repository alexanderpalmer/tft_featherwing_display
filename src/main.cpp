#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>

void drawButton();
void checkButtonPress(int x, int y);

#define STMPE_CS 16
#define TFT_CS 0
#define TFT_DC 15
#define SD_CS 2

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 3800
#define TS_MAXX 100
#define TS_MINY 100
#define TS_MAXY 3750

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 3800
#define TS_MAXX 100
#define TS_MINY 100
#define TS_MAXY 3750

int x = 0;
int y = 0;
int z = 0;

void setup()
{
  Serial.begin(9600);
  delay(10);

  if (!ts.begin())
  {
    Serial.println("Couldn't start touchscreen controller");
    while (1)
      ;
  }

  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);

  drawButton();
}

void loop()
{

  if (ts.touched())
  {
    TS_Point p = ts.getPoint();
    // Scale from ~0->4000 to tft.width using the calibration #'s
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

    if (p.x != x || p.y != y)
    {
      x = p.x;
      y = p.y;
      z = p.z;
      checkButtonPress(x, y);
      Serial.println("Last touch-Coordinates");
      Serial.print("X = ");
      Serial.print(x);
      Serial.print("\tY = ");
      Serial.println(y);
      Serial.print("\tPressure = ");
      Serial.println(z);
    }
  }
}

void drawButton()
{
  tft.setCursor(110, 80);
  // tft.setTextColor(ILI9341_WHITE);
  // tft.setTextSize(1);
  // tft.println("Hello World!");

  tft.fillRect(110, 80, 100, 80, ILI9341_RED);
}

void checkButtonPress(int x, int y)
{
  if (x >= 110 && x <= 210 && y >= 80 && y <= 160)
  {
    tft.fillRect(110, 80, 100, 80, ILI9341_BLUE);
    delay(100);
    tft.fillRect(110, 80, 100, 80, ILI9341_RED);
  }
}
