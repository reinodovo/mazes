#ifndef MAZES_H
#define MAZES_H

#include <random>

const int NUMBER_OF_MAZES = 9;
const int MAZE_SIZE = 6;

struct Cell {
    int x, y;
};

struct Maze {
    int p[MAZE_SIZE * MAZE_SIZE];
    int w[MAZE_SIZE * MAZE_SIZE];
    bool walls_right[MAZE_SIZE][MAZE_SIZE];
    bool walls_down[MAZE_SIZE][MAZE_SIZE];
    Cell references[2];
    int find(int);
    bool join(int, int);
    void generate();
};

Maze* generate_mazes(int);

#endif // MAZES_H