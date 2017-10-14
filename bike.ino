#include <FastLED.h>

#define PIXEL_COUNT 5
#define SPRITE_COUNT 3
#define SEGMENT_COUNT 2

struct SegmentMap {
  const uint8_t start;
  const uint8_t end;
  uint8_t length() {return end - start;}
};

struct Sprite {
  uint8_t m_stripIdx;
  uint8_t m_pos;
  CHSV m_color;
  Sprite()
    : m_stripIdx(0),
      m_pos(0) {}
      
  void step(SegmentMap* segments) {
    m_pos += 1;
    if (m_pos >= segments[m_stripIdx].length()) {
      m_stripIdx += 1;
      if (m_stripIdx >= SEGMENT_COUNT) {
        m_stripIdx = 0;
      }
    }
  }
  
  void render(CRGB* leds, SegmentMap* segments) {
     SegmentMap segment = segments[m_stripIdx];
     leds[segment.start + m_pos] = m_color;
  }
};

CRGB leds[PIXEL_COUNT];
Sprite sprites[SPRITE_COUNT];
SegmentMap segments[SEGMENT_COUNT] = {
  {.start = 0, .end = 5},
  {.start = 6, .end = 11}
};

void setup()
{
    FastLED.addLeds<WS2812, 11, GBR>(leds, PIXEL_COUNT);
    for(int i = 0; i < SPRITE_COUNT;i++) {
      sprites[i].m_pos = i;
      sprites[i].m_stripIdx = i % SPRITE_COUNT;
    }
}

void loop()
{
    for(int i = 0; i < SPRITE_COUNT;i++) {
      sprites[i].step(segments);
      sprites[i].render(leds, segments);
    }
    FastLED.show();
}
