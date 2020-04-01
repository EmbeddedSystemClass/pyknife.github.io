#include "hopper.h"

 uint8_t lastButtonState;
 uint8_t currentButtonState;
  uint8_t frameRate;
  uint16_t frameCount;
  uint8_t eachFrameMillis;
  long lastFrameStart;
  long nextFrameStart;
  bool post_render;
  uint8_t lastFrameDurationMs;
  
/* Frame management */
void SetFrameRate(uint8_t rate)
{
  frameRate = rate;
  eachFrameMillis = 1000/rate;
}

bool EveryXFrames(uint8_t frames)
{
  return frameCount % frames == 0;
}

static bool nextFrame(void)
{
  long now = HAL_GetTick();//��ǰ������
  //uint8_t remaining;

  // post render
  if (post_render) {
    lastFrameDurationMs = now - lastFrameStart;
    frameCount++;
    post_render = false;
  }

  // if it's not time for the next frame yet
  if (now < nextFrameStart) {
    //remaining = nextFrameStart - now;
    // if we have more than 1ms to spare, lets sleep
    // we should be woken up by timer0 every 1ms, so this should be ok
    //if (remaining > 1)
    return false;
  }
   // pre-render
  // technically next frame should be last frame + each frame but if we're
  // running a slow render we would constnatly be behind the clock
  // keep an eye on this and see how it works.  If it works well the
  // lastFrameStart variable could be eliminated completely
  nextFrameStart = now + eachFrameMillis;
  lastFrameStart = now;
  post_render = true;
  return post_render;
}

bool NextFrame(void)
{
    bool ret = nextFrame();
    if (ret) {
        lastButtonState = currentButtonState;
        currentButtonState = ButtonsState();
    }
    return ret;
}


bool ButtonDown(uint8_t buttons)
{
    return currentButtonState & ~lastButtonState & buttons;
}

bool ButtonPressed(uint8_t buttons)
{
    return currentButtonState & buttons;
}

bool ButtonUp(uint8_t buttons)
{
    return ~currentButtonState & lastButtonState & buttons;
}

// ��ʼ��α�����������
void randomSeed(unsigned long seed)
{
    if (seed != 0) {
        srand(seed);
    }
}

//����α�����
// howbig�����ֵ���Ͻ�
long random(long howbig)
{
    if (howbig == 0) {
        return 0;
    }
    return rand() % howbig;
}

//����α�����
// howsmall�����ֵ���½�
//howbig�����ֵ���Ͻ�
long random2(long howsmall, long howbig)
{
    if (howsmall >= howbig) {
        return howsmall;
    }
    long diff = howbig - howsmall;
    return random(diff) + howsmall;
}
// ��ȡ��Ƭ�����ϵ�����������΢����
uint32_t micros(void)
{
	uint32_t System_ms ;
	System_ms=HAL_GetTick();
    return (System_ms * 1000 + (SysTick_LoadValue - SysTick->VAL) / CYCLES_PER_MICROSECOND);
}


