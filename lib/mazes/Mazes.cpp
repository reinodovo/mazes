#include <mazes.hpp>

std::mt19937 rng;

long random(long a, long b) { return rng() % (b - a + 1) + a; }

int cell(int x, int y) { return y * MAZE_SIZE + x; }

Cell randomCell() {
  Cell c;
  c.x = random(0, MAZE_SIZE - 1);
  c.y = random(0, MAZE_SIZE - 1);
  return c;
}

int Maze::find(int cell_id) {
  return (cell_id == _p[cell_id]) ? cell_id : (_p[cell_id] = find(_p[cell_id]));
}

bool Maze::join(int cell_id_1, int cell_id_2) {
  if ((cell_id_1 = find(cell_id_1)) == (cell_id_2 = find(cell_id_2))) {
    return false;
  }
  if (_w[cell_id_1] < _w[cell_id_2]) {
    std::swap(cell_id_1, cell_id_2);
  }
  _p[cell_id_2] = cell_id_1;
  _w[cell_id_1] += _w[cell_id_2];
  return true;
}

Maze::Maze() {
  for (int i = 0; i < MAZE_SIZE * MAZE_SIZE; i++) {
    _w[_p[i] = i] = 1;
  }
  for (int i = 0; i < MAZE_SIZE; i++) {
    for (int j = 0; j < MAZE_SIZE; j++) {
      walls_right[i][j] = true;
      walls_down[i][j] = true;
    }
  }
}

void Maze::generate() {
  while (_w[find(0)] != MAZE_SIZE * MAZE_SIZE) {
    int cell1x = random(0, MAZE_SIZE - 2);
    int cell1y = random(0, MAZE_SIZE - 1);
    int cell2x = cell1x + 1;
    int cell2y = cell1y;
    int dir = random(0, 1);
    if (dir == 1) {
      std::swap(cell1x, cell1y);
      std::swap(cell2x, cell2y);
    }
    int c1 = cell(cell1x, cell1y);
    int c2 = cell(cell2x, cell2y);
    bool joined = join(c1, c2);
    if (!joined) {
      continue;
    }
    if (dir == 0) {
      walls_right[cell1x][cell1y] = false;
    } else {
      walls_down[cell1x][cell1y] = false;
    }
  }
  // TODO: make sure this is not the same pair of cells as another maze
  references[0] = randomCell();
  references[1] = randomCell();
}

Maze *generateMazes(int code) {
  rng = std::mt19937(code);
  Maze *mazes = new Maze[NUMBER_OF_MAZES];
  for (int i = 0; i < NUMBER_OF_MAZES; i++) {
    mazes[i].generate();
  }
  return mazes;
}