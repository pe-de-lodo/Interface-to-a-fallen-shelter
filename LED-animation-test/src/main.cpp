#include <FastLED.h>
#include <Math.h>
#include <Tween.h>

#define NUM_LEDS 300
#define DATA_PIN A3

CRGB leds[NUM_LEDS];

enum WAVE {Up, Down, Tri, Sin};
enum WRAP {Clamp, Wrap, Fold, None};
class Synth
{
  public:
  WAVE wave = Tri;
  WRAP wrap = Clamp;

  float phas = 0;
  float offs = 0;
  float freq = 1;
  float ampl = 1;
  float res = 0;

  Synth(){}

  Synth(WAVE wa, WRAP wr, float p, float o, float f, float a, float r)
  {
    wave = wa;
    wrap = wr;
    phas = p;
    offs = o;
    freq = f;
    ampl = a;
    res = r;
  }

  float Evaluate(float x)
  {
    float y = 0;

    x *= freq;
    x -= phas;

    //LOCK MODE

    switch(wave)
    {
      case Up:
        y = x - floor(x);
        break;

      case Down:
        y = 1 - (x - floor(x));
        break;

      case Tri:
        y = abs(x - floor(x + .5)) *2;
        break;

      case Sin:
        y = pow(sin(x*PI), 2);
        break;
    }


    y *= ampl;
    y += offs;
    y = pow(y, exp(res));

    switch(wrap)
    {
      case Clamp:
        y = constrain(y, 0, 1);
        break;

      case Wrap:
        y = fmod(y, 1);
        break;

      case Fold:
        y = fmod(y, 2.0);  // Ensure t is within the interval [0, 2]
        if (y > 1.0)
            y = 2.0 - y;
        break;

      case None:
        break;
    }

    return y;
  }
};

Tween::Timeline timeline;

#define NUM_WAVES 2
Synth synths[NUM_WAVES];

void setup() {
  delay(3000);
  Serial.begin(9600);

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(64);
  FastLED.clear();
  FastLED.show();

  // for(int i = 0; i < 10; i++)
  //   synths[i] = Synth();

  // Serial.println("A");

  synths[0].wave = Tri;
  synths[0].freq = 1;
  synths[0].offs = -0.1;

  synths[1].wave = Up;
  

  timeline.mode(Tween::Mode::REPEAT_SQ);
  timeline.add(synths[0].phas) // target tweens
        .init(0)         // init target value (optional)
        .offset(1000)    // delay start 1000[ms] (optional)
        .then(1, random(3000, 8000))  // to 10 in 5000[ms]     // then stops 1000[ms]
        .then(0, 0);

  timeline.add(synths[1].phas) // target tweens
        .init(0)         // init target value (optional)
        .offset(1000)    // delay start 1000[ms] (optional)
        .then(1, random(10000, 50000))  // to 10 in 5000[ms]
        .hold(1000)      // then stops 1000[ms]
        .then(0, random(10000, 50000))   // then to 5 in 5000[ms]
        .hold(1000);      // then stops 1000[ms]


// for(int i = 2; i < 10; i++)
// {
//     timeline.add(synths[i].phas) // target tweens
//         .init(0)         // init target value (optional)
//         .offset(1000)    // delay start 1000[ms] (optional)
//         .then(1, random(1000, 10000))  // to 10 in 5000[ms]
//         .hold(1000)      // then stops 1000[ms]
//         .then(0, random(1000, 10000))   // then to 5 in 5000[ms]
//         .hold(1000);      // then stops 1000[ms]
//     timeline.add(synths[i].offs) // target tweens
//         .init(0)         // init target value (optional)
//         .offset(1000)    // delay start 1000[ms] (optional)
//         .then(1, random(1000, 10000))  // to 10 in 5000[ms]
//         .hold(1000)      // then stops 1000[ms]
//         .then(0, random(1000, 10000))   // then to 5 in 5000[ms]
//         .hold(1000);      // then stops 1000[ms]
// }
  

    timeline.start();

    // Serial.println("B");

  // s.wave = Sin;

  /*leds[0] = CRGBW(255, 0, 0, 0);
  leds[1] = CRGBW(0, 255, 0, 0);
  leds[2] = CRGBW(0, 0, 255, 0);
  leds[3] = CRGBW(0, 0, 0, 255);*/

  // leds[0] = (CRGB)CHSV(0, 255, 255);
  // leds[1] = (CRGB)CHSV(60, 255, 255);
  // leds[2] = (CRGB)CHSV(120, 255, 255);
  // leds[3] = (CRGB)CHSV(180, 255, 255);

  

  // for(uint8_t i = 0; i < NUM_LEDS; i++)
  // {
  //   float x = (float)i / (float)NUM_LEDS;
  //   float y = s.Evaluate(x);
  //   Serial.print(x);
  //   Serial.print(" - ");
  //   Serial.print(y);
  //   Serial.println();

  //   leds[i] = (CRGB)CHSV(y*255, 255, 255);
  // }
  // FastLED.show();
}

void loop() {
  
  // Serial.println("C");
  timeline.update();
    for(uint16_t i = 0; i < NUM_LEDS; i++)
    {
      float x = (float)i / (float)NUM_LEDS;
      float v = synths[0].Evaluate(x);
      float h = synths[1].Evaluate(x);

      for(int j = NUM_WAVES-1; j >= 2; j--)
      {
        float a = synths[j].Evaluate(x);
      }

        leds[i] = CHSV(255 * h, 255, 255 * v);
  }
  FastLED.show();

  // Serial.println("D");

  // delay(10);
  //s.offs += 0.001;

}

