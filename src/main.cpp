#include <Arduino.h>
#include <maze_display.h>
#include <mazes.h>
#include <modules/puzzle_module.h>
#include <utils/button.h>

const int RED_PIN = 23, GREEN_PIN = 22;
PuzzleModule module(StatusLight(RED_PIN, GREEN_PIN));

std::vector<Maze> mazes;
Maze maze;
Cell current, target;

const int BUTTONS = 4;
const int BUTTON_PINS[BUTTONS] = {36, 35, 39, 34};
Button btns[BUTTONS];
enum Direction { Left, Right, Up, Down };

bool hitWall(Cell initial, Cell final) {
  if (initial.x == final.x) return maze.walls_down[initial.x][min(initial.y, final.y)];
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
      module.strike();
      return;
    }

    current = Cell(x, y);
    MazeDisplay::set_current(current);

    if (target == current) module.solve();
  }
};

Cell generate_random_cell() {
  int x = esp_random() % MAZE_SIZE;
  int y = esp_random() % MAZE_SIZE;
  return {x, y};
}

void on_manual_code(int code) { mazes = generate_mazes(code); }

void start() {
  maze = mazes[esp_random() % NUMBER_OF_MAZES];
  target = generate_random_cell();
  do {
    current = generate_random_cell();
  } while (current == target);
  MazeDisplay::set_maze(maze, target);
  MazeDisplay::set_current(current);
}

void restart() { MazeDisplay::clear(); }

void setup() {
  module.on_manual_code(on_manual_code);
  module.on_start(start);
  module.on_reset(restart);

  if (!module.setup()) ESP.restart();

  for (int i = 0; i < BUTTONS; i++) {
    btns[i] = Button(BUTTON_PINS[i]);
    btns[i].on_state_change(ButtonFunction{(Direction)i});
  }

  MazeDisplay::setup();
}

void loop() {
  module.update();
  MazeDisplay::update();
  if (module.get_state() != PuzzleModuleState::Started) return;
  for (int i = 0; i < BUTTONS; i++) btns[i].update();
}