#ifndef MAZE_H
#define MAZE_H

#include <Arduino.h>

#include <mazes.hpp>
#include <utility>

#define COLORS 3
enum Color {
  RED = 0,
  GREEN = 1,
  BLUE = 2,
};

#define RELEVANT_CELLS 4
#define TARGET 2
#define CURRENT 3

#define COLOR_PERIOD_MS 500
#define CELL_PERIOD_MS 5

const int CHANNELS = 3;

const int RED_PINS[CHANNELS] = {25, 14, 2};
const int GREEN_PINS[CHANNELS] = {33, 27, 15};
const int BLUE_PINS[CHANNELS] = {32, 26, 13};

const int CLOCK = 19, STORE = 18, SER = 5;

const int ROW_GROUPS = 12;
const uint16_t ROW_GROUPS_SERIAL[ROW_GROUPS] = {
    0b0100000000000000, 0b0010000000000000, 0b0001000000000000,
    0b0000100000000000, 0b0000010000000000, 0b0000001000000000,
    0b0000000001000000, 0b0000000000100000, 0b0000000000010000,
    0b0000000000001000, 0b0000000000000100, 0b0000000000000010};

void send(uint16_t data) {
  shiftOut(SER, CLOCK, LSBFIRST, data);
  shiftOut(SER, CLOCK, LSBFIRST, data >> 8);
  digitalWrite(STORE, HIGH);
  digitalWrite(STORE, LOW);
}

void set(int col, int row, int color) {
  int row_group = (row * 2) + (col / 3);
  int row_group_serial = ROW_GROUPS_SERIAL[row_group];
  send(row_group_serial);
  int channel = col % 3;
  digitalWrite(RED_PINS[channel], color == RED ? LOW : HIGH);
  digitalWrite(GREEN_PINS[channel], color == GREEN ? LOW : HIGH);
  digitalWrite(BLUE_PINS[channel], color == BLUE ? LOW : HIGH);
}

void off() {
  for (int i = 0; i < CHANNELS; i++) {
    digitalWrite(RED_PINS[i], HIGH);
    digitalWrite(GREEN_PINS[i], HIGH);
    digitalWrite(BLUE_PINS[i], HIGH);
  }
}

namespace MazeDisplay {
bool color[COLORS][MAZE_SIZE][MAZE_SIZE];
Cell relevant_cells[RELEVANT_CELLS];
int last_cell_index = 0;

void setup(Maze m, Cell t) {
  pinMode(CLOCK, OUTPUT);
  pinMode(STORE, OUTPUT);
  pinMode(SER, OUTPUT);
  for (int i = 0; i < CHANNELS; i++) {
    pinMode(RED_PINS[i], OUTPUT);
    pinMode(GREEN_PINS[i], OUTPUT);
    pinMode(BLUE_PINS[i], OUTPUT);
    digitalWrite(RED_PINS[i], HIGH);
    digitalWrite(GREEN_PINS[i], HIGH);
    digitalWrite(BLUE_PINS[i], HIGH);
  }
  for (int i = 0; i < COLORS; i++) {
    for (int x = 0; x < MAZE_SIZE; x++) {
      for (int y = 0; y < MAZE_SIZE; y++) {
        color[i][x][y] = false;
      }
    }
  }
  relevant_cells[0] = m.references[0];
  relevant_cells[1] = m.references[1];
  relevant_cells[2] = t;
  relevant_cells[3] = {0, 0};
  color[RED][t.x][t.y] = true;
  color[GREEN][m.references[0].x][m.references[0].y] = true;
  color[GREEN][m.references[1].x][m.references[1].y] = true;
}

void setCurrent(Cell c) {
  Cell old = relevant_cells[CURRENT];
  color[BLUE][old.x][old.y] = false;
  relevant_cells[CURRENT] = c;
  color[BLUE][c.x][c.y] = true;
}

void update() {
  unsigned long now = millis();
  int index = (now / CELL_PERIOD_MS) % RELEVANT_CELLS;
  if (index == last_cell_index) {
    return;
  }
  off();
  last_cell_index = index;
  Cell cell = relevant_cells[index];
  int found_colors = 0;
  int colors[COLORS];
  for (int i = 0; i < COLORS; i++) {
    if (color[i][cell.x][cell.y]) {
      colors[found_colors++] = i;
    }
  }
  if (found_colors != 0) {
    int color_index = (now / COLOR_PERIOD_MS) % found_colors;
    set(cell.x, cell.y, colors[color_index]);
  }
}
}  // namespace MazeDisplay

#endif  // MAZE_H