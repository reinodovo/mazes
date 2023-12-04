#include <Arduino.h>
#include <maze_display.h>
#include <mazes.h>
#include <puzzle_module.h>
#include <utils/button.h>

Maze maze;
Cell current, target;

const int BUTTONS = 4;
const int BUTTON_PINS[BUTTONS] = {36, 35, 39, 34};
Button btns[BUTTONS];
enum Direction { Left, Right, Up, Down };

bool hitWall(Cell initial, Cell final) {
  if (initial.x == final.x)
    return maze.walls_down[initial.x][min(initial.y, final.y)];
  return maze.walls_right[min(initial.x, final.x)][initial.y];
}

struct ButtonFunction {
  Direction direction;
  void operator()(ButtonState state, ButtonState _) const {
    if (state != ButtonState::Pressed) return;
    int x = direction == Left ? -1 : direction == Right ? 1 : 0;
    int y = direction == Up ? -1 : direction == Down ? 1 : 0;
    x = min(max(0, current.x + x), MAZE_SIZE - 1);
    y = min(max(0, current.y + y), MAZE_SIZE - 1);

    if (hitWall(current, {x, y})) {
      PuzzleModule::strike();
      return;
    }

    current = Cell(x, y);
    MazeDisplay::setCurrent(current);

    if (target == current) PuzzleModule::solve();
  }
};

const int RED_PIN = 23, GREEN_PIN = 22;

Cell generateRandomCell() {
  int x = esp_random() % MAZE_SIZE;
  int y = esp_random() % MAZE_SIZE;
  return {x, y};
}

void onManualCode(int code) {
  const Maze *mazes = generateMazes(code);
  maze = mazes[esp_random() % NUMBER_OF_MAZES];
}

void start() {
  target = generateRandomCell();
  do {
    current = generateRandomCell();
  } while (current == target);
  MazeDisplay::setMaze(maze, target);
  MazeDisplay::setCurrent(current);
}

void restart() { MazeDisplay::clear(); }

void setup() {
  if (!PuzzleModule::setup(PuzzleModule::StatusLight(RED_PIN, GREEN_PIN)))
    ESP.restart();

  PuzzleModule::onManualCode = onManualCode;
  PuzzleModule::onStart = start;
  PuzzleModule::onRestart = restart;

  for (int i = 0; i < BUTTONS; i++) {
    btns[i] = Button(BUTTON_PINS[i]);
    btns[i].onStateChange = ButtonFunction{(Direction)i};
  }

  MazeDisplay::setup();
}

void loop() {
  PuzzleModule::update();
  MazeDisplay::update();
  for (int i = 0; i < BUTTONS; i++) btns[i].update();
}