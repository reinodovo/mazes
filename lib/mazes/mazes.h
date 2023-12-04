#ifndef MAZES_H
#define MAZES_H

#include <random>
#include <utility>

const int NUMBER_OF_MAZES = 9;
const int MAZE_SIZE = 6;

struct Cell {
  int x, y;
  Cell() {}
  Cell(int x, int y) : x(x), y(y) {}
  bool operator==(Cell&) const;
};

using Reference = std::pair<Cell, Cell>;

struct Maze {
 public:
  bool walls_right[MAZE_SIZE][MAZE_SIZE];
  bool walls_down[MAZE_SIZE][MAZE_SIZE];
  Reference reference;
  void generate();
  Maze();

 private:
  int _p[MAZE_SIZE * MAZE_SIZE];
  int _w[MAZE_SIZE * MAZE_SIZE];
  int find(int cell_id);
  bool join(int cell_id_1, int cell_id_2);
};

Maze* generateMazes(int code);

#endif  // MAZES_H