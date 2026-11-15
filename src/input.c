#include "input.h"
#include <string.h>

void input_init(InputState* state) {
  memset(state->keys, 0, sizeof(state->keys));
}

void input_set_key(InputState* state, int keycode, bool pressed) {
  if (keycode < 0 || keycode >= MAX_KEYS) return;

  int word_idx = keycode / 64;
  int bit_idx = keycode % 64;

  if (pressed) {
    state->keys[word_idx] |= (1ULL << bit_idx);
  } else {
    state->keys[word_idx] &= ~(1ULL << bit_idx);
  }
}

bool input_is_down(const InputState* state, int keycode) {
  if (keycode < 0 || keycode >= MAX_KEYS) return false;
  return (state->keys[keycode / 64] & (1ULL << (keycode % 64))) != 0;
}

bool input_was_just_pressed(const InputState* curr, const InputState* prev, int keycode) {
  return input_is_down(curr, keycode) && !input_is_down(prev, keycode);
}
