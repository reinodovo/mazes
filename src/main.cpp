#include <Arduino.h>
#include <Mazes.h>

#include "maze.h"
#include "button.h"

Maze maze;
Cell target, current;

const int BUTTONS = 4;
int BUTTON_PINS[BUTTONS] = {36, 35, 39, 34};
Button btns[BUTTONS];

int int_abs(int x) {
    return x < 0 ? -x : x;
}

Cell generate_random_cell() {
    int x = int_abs(esp_random()) % MAZE_SIZE;
    int y = int_abs(esp_random()) % MAZE_SIZE;
    return {x, y};
}

void left(ButtonState state) {
    if (state != ButtonState::Pressed) return;
    current.x = (current.x + MAZE_SIZE - 1) % MAZE_SIZE;
}

void right(ButtonState state) {
    if (state != ButtonState::Pressed) return;
    current.x = (current.x + 1) % MAZE_SIZE;
}

void up(ButtonState state) {
    if (state != ButtonState::Pressed) return;
    current.y = (current.y + MAZE_SIZE - 1) % MAZE_SIZE;
}

void down(ButtonState state) {
    if (state != ButtonState::Pressed) return;
    current.y = (current.y + 1) % MAZE_SIZE;
}

void setup() {
    pinMode(CLOCK, OUTPUT);
    pinMode(STORE, OUTPUT);
    pinMode(SER, OUTPUT);
    for (int i = 0; i < BUTTONS; i++) {
        pinMode(BUTTON_PINS[i], INPUT);
    }
    for (int i = 0; i < CHANNELS; i++) {
        pinMode(RED_PINS[i], OUTPUT);
        pinMode(GREEN_PINS[i], OUTPUT);
        pinMode(BLUE_PINS[i], OUTPUT);
        digitalWrite(RED_PINS[i], HIGH);
        digitalWrite(GREEN_PINS[i], HIGH);
        digitalWrite(BLUE_PINS[i], HIGH);
    }
    Maze* mazes = generate_mazes(0);
    maze = mazes[esp_random() % NUMBER_OF_MAZES];
    target = generate_random_cell();
    current = generate_random_cell();
    MazeDisplay::init(maze, target);
    MazeDisplay::set_current(current);
    btns[0].init(BUTTON_PINS[0], left);
    btns[1].init(BUTTON_PINS[1], right);
    btns[2].init(BUTTON_PINS[2], up);
    btns[3].init(BUTTON_PINS[3], down);
}

void loop() {
    MazeDisplay::update();
    for (int i = 0; i < BUTTONS; i++) {
        btns[i].update();
    }
    MazeDisplay::set_current(current);
}