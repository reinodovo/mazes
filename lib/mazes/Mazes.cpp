#include <Mazes.h>

std::mt19937 rng;

long random(long a, long b) {
    return rng() % (b - a + 1) + a;
}

int cell(int x, int y) {
    return y * MAZE_SIZE + x;
}

Cell random_cell() {
    Cell cell;
    cell.x = random(0, MAZE_SIZE - 1);
    cell.y = random(0, MAZE_SIZE - 1);
    return cell;
}

int Maze::find(int cell) {
    return (cell == p[cell]) ? cell : (p[cell] = find(p[cell]));
}

bool Maze::join(int cell1, int cell2) {
    if ((cell1 = find(cell1)) == (cell2 = find(cell2))) return false;
    if (w[cell1] < w[cell2]) std::swap(cell1, cell2);
    p[cell2] = cell1;
    w[cell1] += w[cell2];
    return true;
}

void Maze::generate() {
    for (int i = 0; i < MAZE_SIZE * MAZE_SIZE; i++) w[p[i] = i] = 1;
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            walls_right[i][j] = true;
            walls_down[i][j] = true;
        }
    }
    while (w[find(0)] != MAZE_SIZE * MAZE_SIZE) {
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
        if (!joined) continue;
        if (dir == 0) {
            walls_right[cell1x][cell1y] = false;
        } else {
            walls_down[cell1x][cell1y] = false;
        }
    }
    // TODO: make sure this is not the same pair of cells as another maze
    references[0] = random_cell();
    references[1] = random_cell();
}

Maze* generate_mazes(int seed) {
    rng = std::mt19937(seed);
    Maze* mazes = new Maze[NUMBER_OF_MAZES];
    for (int i = 0; i < NUMBER_OF_MAZES; i++) {
        mazes[i].generate();
    }
    return mazes;
}