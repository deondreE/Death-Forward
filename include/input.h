#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_KEYS 512
#define WORD_COUNT (MAX_KEYS / 64)

typedef struct {
  uint64_t keys[WORD_COUNT];
} InputState;

void input_init(InputState* state);
void input_set_key(InputState* state, int keycode, bool pressed);
bool input_is_down(const InputState* state, int keycode);
void input_was_just_pressed(const InputState* curr, const InputState* prev, int keycode);

#endif
