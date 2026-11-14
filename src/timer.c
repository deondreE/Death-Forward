#include "timer.h"
#include <windows.h>

static double frequency;
static LARGE_INTEGER last_time;
static float delta_time;

void timer_init(void) {
  LARGE_INTEGER freq;
  QueryPerformanceFrequency(&freq);
  frequency = (double)freq.QuadPart;
  QueryPerformanceCounter(&last_time);
  delta_time = 0.0f;
}

void timer_update(void) {
  LARGE_INTEGER current_time;
  QueryPerformanceCounter(&current_time);

  delta_time = (float)((current_time.QuadPart - last_time.QuadPart) / frequency);
  last_time = current_time;
}

float timer_delta(void) {
  return delta_time;
}
