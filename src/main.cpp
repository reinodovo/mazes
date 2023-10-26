#include <Arduino.h>
#include <PuzzleModule.h>
#include <Mazes.h>
#include <utils/Button.h>

#include "maze.h"

Maze maze;
Cell target, current;

bool hit_wall(Cell initial, Cell final) {
    if (initial.x == final.x) {
        return maze.walls_down[initial.x][min(initial.y, final.y)];
    } else {
        return maze.walls_right[min(initial.x, final.x)][initial.y];
    }
}

const int BUTTONS = 4;
int BUTTON_PINS[BUTTONS] = {36, 35, 39, 34};
Button btns[BUTTONS];
enum Direction {
    Left,
    Right,
    Up,
    Down
};

struct ButtonFunction {
    Direction direction;
    void operator() (State state) {
        if (state != State::PRESSED) return;
        int x = direction == Left ? -1 : direction == Right ? 1 : 0;
        int y = direction == Up ? -1 : direction == Down ? 1 : 0;
        x = min(max(0, current.x + x), MAZE_SIZE - 1);
        y = min(max(0, current.y + y), MAZE_SIZE - 1);

        if (hit_wall(current, {x, y})) {
            PuzzleModule::strike();
            return;
        }

        current = {x, y};
        MazeDisplay::set_current(current);

        if (target.x == current.x && target.y == current.y)
            PuzzleModule::solve();
    }
};

const int RED_PIN = 23, GREEN_PIN = 22;

Cell generate_random_cell() {
    int x = esp_random() % MAZE_SIZE;
    int y = esp_random() % MAZE_SIZE;
    return {x, y};
}

void onSeed(int seed) {
    Maze* mazes = generate_mazes(seed);
    maze = mazes[esp_random() % NUMBER_OF_MAZES];
    target = generate_random_cell();
    current = generate_random_cell();
    MazeDisplay::init(maze, target);
    MazeDisplay::set_current(current);
}

void setup() {
    pinMode(CLOCK, OUTPUT);
    pinMode(STORE, OUTPUT);
    pinMode(SER, OUTPUT);
    for (int i = 0; i < BUTTONS; i++) {
        pinMode(BUTTON_PINS[i], INPUT);
        btns[i] = Button(BUTTON_PINS[i]);
        btns[i].onStateChange = ButtonFunction{(Direction)i};
    }
    for (int i = 0; i < CHANNELS; i++) {
        pinMode(RED_PINS[i], OUTPUT);
        pinMode(GREEN_PINS[i], OUTPUT);
        pinMode(BLUE_PINS[i], OUTPUT);
        digitalWrite(RED_PINS[i], HIGH);
        digitalWrite(GREEN_PINS[i], HIGH);
        digitalWrite(BLUE_PINS[i], HIGH);
    }

    PuzzleModule::StatusLight statusLight;
    statusLight.init(RED_PIN, GREEN_PIN);

    if (!PuzzleModule::init(statusLight, onSeed))
        ESP.restart();
}

void loop() {
    PuzzleModule::update();
    MazeDisplay::update();
    for (int i = 0; i < BUTTONS; i++) {
        btns[i].update();
    }
}