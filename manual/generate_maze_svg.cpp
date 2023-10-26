#include <stdio.h>
#include <stdlib.h>
#include <Mazes.h>

const int WALL_SIZE = 34;
const int BORDER_WIDTH = 3;
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
    printf("<line stroke-width=\"%d\" stroke=\"rgb(0%%,0%%,0%%)\" x1=\"%d\" x2=\"%d\" y1=\"%d\" y2=\"%d\" />", WALL_WIDTH, x1, x2, y1, y2);
}

void generate_maze_svg(Maze maze, int offx, int offy) {
    printf("<rect fill=\"black\" height=\"%d\" width=\"%d\" x=\"%d\" y=\"%d\" style=\"fill:white;stroke-width:%d;stroke:black\" />", SIZE, SIZE, offx, offy, BORDER_WIDTH);
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE - 1; j++) {
            if (maze.walls_right[j][i])
                wall(j + 1, j + 1, i, i + 1, offx, offy);
            if (maze.walls_down[i][j])
                wall(i, i + 1, j + 1, j + 1, offx, offy);
        }
    }
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            printf("<circle cx=\"%d\" cy=\"%d\" r=\"%d\" fill=\"gray\" stroke=\"black\" />", offx + i * WALL_SIZE + WALL_SIZE / 2, offy + j * WALL_SIZE + WALL_SIZE / 2, WALL_SIZE / 10);
        }
    }
    for (int i = 0; i < 2; i++) {
        int x = maze.references[i].x;
        int y = maze.references[i].y;
        printf("<circle cx=\"%d\" cy=\"%d\" r=\"%d\" fill=\"none\" stroke=\"black\" stroke-width=\"1\"/>", offx + x * WALL_SIZE + WALL_SIZE / 2, offy + y * WALL_SIZE + WALL_SIZE / 2, WALL_SIZE / 3);
    }
}

void generate_svg(Maze* mazes) {
    int sz = (SIZE + 20) * 3;
    printf("<svg baseProfile=\"tiny\" height=\"%d\" version=\"1.2\" width=\"%d\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:ev=\"http://www.w3.org/2001/xml-events\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">", sz, sz);
    for (int i = 0; i < NUMBER_OF_MAZES; i++) {
        int offx = (SIZE + 20) * (i % 3) + 5;
        int offy = (SIZE + 20) * (i / 3) + 5;
        generate_maze_svg(mazes[i], offx, offy);
    }
    printf("</svg>");
}

int main(int argc, char** argv) {
    freopen("./manual/mazes.svg", "w", stdout);
    int seed = atoi(argv[1]);
    Maze* mazes = generate_mazes(seed);
    generate_svg(mazes);
}