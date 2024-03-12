#include "system.h"

const static int NUM_COLS = 80;
const static int NUM_ROWS = 25;

struct Char {
    unsigned char character;
    unsigned char color;
};

char bf[100];

struct Char* buffer = (struct Char*) 0xb8000;
int col = 0;
int row = 0;
unsigned char color = PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4;

void clear_row(int row) {
  struct Char empty = (struct Char) {
          character: ' ',
          color: color,
  };

  for (int col = 0; col < NUM_COLS; col++) {
    buffer[col + NUM_COLS * row] = empty;
  }
}

void print_clear() {
  for (int i = 0; i < NUM_ROWS; i++) {
    clear_row(i);
  }
}

void print_newline() {
  col = 0;

  if (row < NUM_ROWS - 1) {
    row++;
    return;
  }

  for (int row = 1; row < NUM_ROWS; row++) {
    for (int col = 0; col < NUM_COLS; col++) {
      struct Char character = buffer[col + NUM_COLS * row];
      buffer[col + NUM_COLS * (row - 1)] = character;
    }
  }

  clear_row(NUM_COLS - 1);
}

void print_char(char character) {
  if (character == '\n') {
    print_newline();
    return;
  }

  if (col > NUM_COLS) {
    print_newline();
  }

  buffer[col + NUM_COLS * row] = (struct Char) {
          character: (unsigned char) character,
          color: color,
  };

  col++;
}

void print_str(char* str) {
  for (int i = 0; 1; i++) {
    char character = (unsigned char) str[i];

    if (character == '\0') {
      return;
    }

    print_char(character);
  }
}

void print_set_color(unsigned char foreground, unsigned char background) {
  color = foreground + (background << 4);
}

void print_num(unsigned long long n) {
  int i = 0;
  for (;;) {
    bf[i] = n % 10;
    n /= 10;
    if (n == 0)
      break;
    i ++;
  }
  for (int j = 0; j < i; j ++) {
    bf[i + 1] = bf[j];
    bf[j] = bf[i - j];
    bf[i - j] = bf[i + 1];
  }
  bf[i + 1] = 0;
  for (int j = 0; j <= i; j ++)
    bf[j] += '0';
  print_str(bf);
  print_newline();
}