#include <manual.h>
#include <mazes.h>

using namespace std;

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
  cout << "<line stroke-width=\"" << WALL_WIDTH
       << "\" stroke=\"rgb(0%,0%,0%)\" x1=\"" << x1
       << "\" "
          "x2=\""
       << x2 << "\" y1=\"" << y1 << "\" y2=\"" << y2 << "\" />";
}

void generateMazeSvg(Maze maze, int offx, int offy) {
  cout << "<rect fill=\"black\" height=\"" << SIZE << "\" width=\"" << SIZE
       << "\" x=\"" << offx << "\" y=\"" << offy
       << "\" "
          "style=\"fill:white;stroke-width:"
       << BORDER_WIDTH << ";stroke:black\" />";
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
      cout << "<circle cx=\"" << offx + i * WALL_SIZE + WALL_SIZE / 2
           << "\" cy=\"" << offy + j * WALL_SIZE + WALL_SIZE / 2 << "\" r=\""
           << CELL_DOT_RADIUS
           << "\" fill=\"gray\" stroke=\"black\" "
              "/>";
    }
  }
  Cell references[2] = {maze.reference.first, maze.reference.second};
  for (int i = 0; i < 2; i++) {
    int x = references[i].x;
    int y = references[i].y;
    cout << "<circle cx=\"" << offx + x * WALL_SIZE + WALL_SIZE / 2
         << "\" cy=\"" << offy + y * WALL_SIZE + WALL_SIZE / 2 << "\" r=\""
         << REFERENCE_RADIUS
         << "\" fill=\"none\" stroke=\"black\" "
            "stroke-width=\"1\"/>";
  }
}

void generateSvg(const Maze *mazes) {
  int sz = (SIZE + SVG_GAP) * SVG_GRID_SIZE;
  cout << "<svg baseProfile=\"tiny\" height=\"" << sz << "\" width=\"" << sz
       << "\" version=\"1.2\" "
          "xmlns=\"http://www.w3.org/2000/svg\" "
          "xmlns:ev=\"http://www.w3.org/2001/xml-events\" "
          "xmlns:xlink=\"http://www.w3.org/1999/xlink\">";
  for (int i = 0; i < NUMBER_OF_MAZES; i++) {
    int offx = (SIZE + SVG_GAP) * (i % SVG_GRID_SIZE) + MAZE_OFFSET;
    int offy = (SIZE + SVG_GAP) * (i / SVG_GRID_SIZE) + MAZE_OFFSET;
    generateMazeSvg(mazes[i], offx, offy);
  }
  cout << "</svg>" << endl;
}

int main(int argc, char **argv) {
  int code = atoi(argv[1]);
  const Maze *mazes = generateMazes(code);

  ostringstream oss;
  std::streambuf *oldCoutBuf = std::cout.rdbuf(); // Save old buf
  cout.rdbuf(oss.rdbuf());

  generateSvg(mazes);

  cout.rdbuf(oldCoutBuf); // Restore old buf

  manual::json data =
      manual::init("Mazes", "Mazes",
                   "This seems to be some kind of maze, probably stolen off of "
                   "a restaurant placemat.",
                   APP_VERSION);

  cout << oss.str() << endl;
  data["mazes"] = base64Encode(oss.str());

  manual::save(data, code);
}