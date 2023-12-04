#include <mazes.h>
#include <stdio.h>
#include <stdlib.h>

const int SVG_GRID_SIZE = 3;
const int SVG_GAP = 20;
const int WALL_SIZE = 34;
const int REFERENCE_RADIUS = WALL_SIZE / 3;
const int CELL_DOT_RADIUS = WALL_SIZE / 10;
const int BORDER_WIDTH = 3;
const int MAZE_OFFSET = 5;
const int WALL_WIDTH = 2;
const int SIZE = MAZE_SIZE * WALL_SIZE;

void wall(int x1, int x2, int y1, int y2, int offx, int offy) {
  x1 = x1 * WALL_SIZE + offx;
  x2 = x2 * WALL_SIZE + offx;
  y1 = y1 * WALL_SIZE + offy;
  y2 = y2 * WALL_SIZE + offy;
  if (x1 == x2) {
    y1 -= WALL_WIDTH / 2;
    y2 += WALL_WIDTH / 2;
  } else {
    x1 -= WALL_WIDTH / 2;
    x2 += WALL_WIDTH / 2;
  }
  printf(
      "<line stroke-width=\"%d\" stroke=\"rgb(0%%,0%%,0%%)\" x1=\"%d\" "
      "x2=\"%d\" y1=\"%d\" y2=\"%d\" />",
      WALL_WIDTH, x1, x2, y1, y2);
}

void generateMazeSvg(Maze maze, int offx, int offy) {
  printf(
      "<rect fill=\"black\" height=\"%d\" width=\"%d\" x=\"%d\" y=\"%d\" "
      "style=\"fill:white;stroke-width:%d;stroke:black\" />",
      SIZE, SIZE, offx, offy, BORDER_WIDTH);
  for (int i = 0; i < MAZE_SIZE; i++) {
    for (int j = 0; j < MAZE_SIZE - 1; j++) {
      if (maze.walls_right[j][i]) {
        wall(j + 1, j + 1, i, i + 1, offx, offy);
      }
      if (maze.walls_down[i][j]) {
        wall(i, i + 1, j + 1, j + 1, offx, offy);
      }
    }
  }
  for (int i = 0; i < MAZE_SIZE; i++) {
    for (int j = 0; j < MAZE_SIZE; j++) {
      printf(
          "<circle cx=\"%d\" cy=\"%d\" r=\"%d\" fill=\"gray\" stroke=\"black\" "
          "/>",
          offx + i * WALL_SIZE + WALL_SIZE / 2,
          offy + j * WALL_SIZE + WALL_SIZE / 2, CELL_DOT_RADIUS);
    }
  }
  Cell references[2] = {maze.reference.first, maze.reference.second};
  for (int i = 0; i < 2; i++) {
    int x = references[i].x;
    int y = references[i].y;
    printf(
        "<circle cx=\"%d\" cy=\"%d\" r=\"%d\" fill=\"none\" stroke=\"black\" "
        "stroke-width=\"1\"/>",
        offx + x * WALL_SIZE + WALL_SIZE / 2,
        offy + y * WALL_SIZE + WALL_SIZE / 2, REFERENCE_RADIUS);
  }
}

void generateSvg(const Maze* mazes) {
  int sz = (SIZE + SVG_GAP) * SVG_GRID_SIZE;
  printf(
      "<svg baseProfile=\"tiny\" height=\"%d\" width=\"%d\" version=\"1.2\" "
      "xmlns=\"http://www.w3.org/2000/svg\" "
      "xmlns:ev=\"http://www.w3.org/2001/xml-events\" "
      "xmlns:xlink=\"http://www.w3.org/1999/xlink\">",
      sz, sz);
  for (int i = 0; i < NUMBER_OF_MAZES; i++) {
    int offx = (SIZE + SVG_GAP) * (i % SVG_GRID_SIZE) + MAZE_OFFSET;
    int offy = (SIZE + SVG_GAP) * (i / SVG_GRID_SIZE) + MAZE_OFFSET;
    generateMazeSvg(mazes[i], offx, offy);
  }
  printf("</svg>");
}

int main(int argc, char** argv) {
  freopen("./manual/mazes.svg", "w", stdout);
  int seed = atoi(argv[1]);
  const Maze* mazes = generateMazes(seed);
  generateSvg(mazes);
}