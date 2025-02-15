#include <mazes.h>

#include <vector>

std::mt19937 rng;

long random(long a, long b) { return rng() % (b - a + 1) + a; }

int cell(int x, int y) { return y * MAZE_SIZE + x; }

Cell randomCell() {
  Cell c;
  c.x = random(0, MAZE_SIZE - 1);
  c.y = random(0, MAZE_SIZE - 1);
  return c;
}

bool Cell::operator==(Cell &o) const { return o.x == x && o.y == y; }

bool compare(Reference r1, Reference r2) {
  if (r1.first == r2.first && r1.second == r2.second) return true;
  if (r1.second == r2.first && r1.first == r2.second) return true;
  return false;
}

Reference randomReference() {
  Reference r;
  r.first = randomCell();
  do {
    r.second = randomCell();
  } while (r.first == r.second);
  return r;
}

Reference uniqueReference(std::vector<Reference> &refs) {
  Reference r = randomReference();
  for (auto rr : refs)
    if (compare(r, rr)) return uniqueReference(refs);
  return r;
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
}

Maze *generateMazes(int code) {
  rng = std::mt19937(code);
  Maze *mazes = new Maze[NUMBER_OF_MAZES];
  std::vector<Reference> references;
  for (int i = 0; i < NUMBER_OF_MAZES; i++) {
    mazes[i].generate();
    mazes[i].reference = uniqueReference(references);
    references.push_back(mazes[i].reference);
  }
  return mazes;
}