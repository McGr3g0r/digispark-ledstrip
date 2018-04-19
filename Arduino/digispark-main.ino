#include <Adafruit_NeoPixel.h>
#include <DigiUSB.h>

// decrease to speed up, increase to slow down (it's not a delay actually)
// #define DELAY       5000
#define PIN 0
#define NUMPIXELS 10
#define BUF_SIZE ((2 * 7) + 1)
#define BRIGHTNESS  50
#define BLINK_MILLIS 1000

typedef struct
{
  char r;
  char g;
  char b;
} rgb_s;


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
char LEDS_ATTRS[NUMPIXELS];
rgb_s  COLORS[NUMPIXELS];
char blink_flag;
long previousMillis;

// 01|FF FF FF|00
int  brightness = 255;//Brightness initial value
char buf[BUF_SIZE];
int  charsRead = 0;
int  LED_initialized = 0;
int  i;
// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  DigiUSB.begin();

  for (i = 0; i < NUMPIXELS; i++)
    LEDS_ATTRS[i] = 0;
  blink_flag = 0;
  strip.begin();
  strip.setBrightness(brightness);
  strip.show();
  previousMillis = millis();
}

int hex_to_int(const char* src)
{
  if (src[0] >= '0' &&  src[0] <= '9')
    return src[0] - '0';
  else if (src[0] >= 'A' &&  src[0] <= 'F')
    return 10 + src[0] - 'A';
  else if (src[0] >= 'a' &&  src[0] <= 'f')
    return 10 + src[0] - 'a';
}


void do_blink() {
  if (millis() - previousMillis > BLINK_MILLIS) {
    if (blink_flag)
    {
      for (i = 0 ; i < NUMPIXELS; i++)
        if (LEDS_ATTRS[i] & 0x01) {
          strip.setPixelColor(i, 0, 0, 0);
        }
      blink_flag = 0;
    }
    else if (!blink_flag)
    {
      for (i = 0 ; i < NUMPIXELS; i++) {
        if (LEDS_ATTRS[i] & 0x01) {
          strip.setPixelColor(i, COLORS[i].g, COLORS[i].r, COLORS[i].b);
        }
      }
      blink_flag = 1;
    }
    strip.show();
    previousMillis = millis();
  }

}
// the loop routine runs over and over again forever:
void loop() {

  while (DigiUSB.available() && charsRead < BUF_SIZE) {
    char c = DigiUSB.read();
    buf[charsRead++] = c;
  }

  if (charsRead >= BUF_SIZE - 1)
  {
    int header = (hex_to_int(&buf[0]) << 4) | hex_to_int(&buf[1]);    
    int idx = (hex_to_int(&buf[2]) << 4) | hex_to_int(&buf[3]);

    char r = (hex_to_int(&buf[4]) << 4) | hex_to_int(&buf[5]);
    char g = (hex_to_int(&buf[6]) << 4) | hex_to_int(&buf[7]);
    char b = (hex_to_int(&buf[8]) << 4) | hex_to_int(&buf[9]);
    char attr = (hex_to_int(&buf[10]) << 4) | hex_to_int(&buf[11]);

    int terminator = (hex_to_int(&buf[12]) << 4) | hex_to_int(&buf[13]);

    if (header == 0xfd && terminator == 0xdf) {
      if (idx < NUMPIXELS) {
        LEDS_ATTRS[idx] = attr;
          COLORS[idx].r = r;
          COLORS[idx].g = g;
          COLORS[idx].b = b;
        strip.setPixelColor(idx, g, r, b);
        strip.show();
      } else if (idx == 0xff) {
        for (idx = 0; idx < NUMPIXELS; idx++) {
          COLORS[idx].r = r;
          COLORS[idx].g = g;
          COLORS[idx].b = b;
          LEDS_ATTRS[idx] = attr;
          strip.setPixelColor(idx, g, r, b);
          strip.show();
        }
      } else if (idx == 0xfe) {
          brightness = r;
          strip.setBrightness(brightness);
          strip.show();
      }
    }
    charsRead = 0;
  }

  do_blink();

  DigiUSB.delay(10);
}
