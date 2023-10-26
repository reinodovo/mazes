#include <utility>
#include <Mazes.h>

#define COLORS 3
enum Color {
    RED = 0,
    GREEN = 1,
    BLUE = 2,
};

#define RELEVANT_CELLS 4
#define TARGET 2
#define CURRENT 3

#define COLOR_PERIOD_MS 500
#define CELL_PERIOD_MS 5

const int CHANNELS = 3;

const int RED_PINS[CHANNELS] = {25, 14, 2};
const int GREEN_PINS[CHANNELS] = {33, 27, 15};
const int BLUE_PINS[CHANNELS] = {32, 26, 13};

const int CLOCK = 19, STORE = 18, SER = 5;

const int ROW_GROUPS = 12;
const uint16_t ROW_GROUPS_SERIAL[ROW_GROUPS] = {
    0b0100000000000000,
    0b0010000000000000,
    0b0001000000000000,
    0b0000100000000000,
    0b0000010000000000,
    0b0000001000000000,
    0b0000000001000000,
    0b0000000000100000,
    0b0000000000010000,
    0b0000000000001000,
    0b0000000000000100,
    0b0000000000000010
};

void send(uint16_t data) {
    shiftOut(SER, CLOCK, LSBFIRST, data);
    shiftOut(SER, CLOCK, LSBFIRST, data >> 8);
    digitalWrite(STORE, HIGH);
    digitalWrite(STORE, LOW);
}

void set(int col, int row, int color) {
    int row_group = (row * 2) + (col / 3);
    int row_group_serial = ROW_GROUPS_SERIAL[row_group];
    send(row_group_serial);
    int channel = col % 3;
    digitalWrite(RED_PINS[channel], color == RED ? LOW : HIGH);
    digitalWrite(GREEN_PINS[channel], color == GREEN ? LOW : HIGH);
    digitalWrite(BLUE_PINS[channel], color == BLUE ? LOW : HIGH);
}

namespace MazeDisplay {
    bool color[COLORS][MAZE_SIZE][MAZE_SIZE];
    Cell relevant_cells[RELEVANT_CELLS];
    int last_cell_index = 0;

    void init(Maze m, Cell t) {
        for (int i = 0; i < COLORS; i++) {
            for (int x = 0; x < MAZE_SIZE; x++) {
                for (int y = 0; y < MAZE_SIZE; y++) {
                    color[i][x][y] = false;
                }
            }
        }
        relevant_cells[0] = m.references[0];
        relevant_cells[1] = m.references[1];
        relevant_cells[2] = t;
        relevant_cells[3] = {0, 0};
        color[RED][t.x][t.y] = true;
        color[GREEN][m.references[0].x][m.references[0].y] = true;
        color[GREEN][m.references[1].x][m.references[1].y] = true;
    }

    void set_current(Cell c) {
        Cell current = relevant_cells[CURRENT];
        Cell target = relevant_cells[TARGET];
        color[BLUE][current.x][current.y] = false;
        current = c;
        relevant_cells[CURRENT] = current;
        color[BLUE][current.x][current.y] = true;
        if (current.x == target.x && current.y == target.y)
            color[RED][target.x][target.y] = false;
    }

    void update() {
        unsigned long now = millis();
        int index = (now / CELL_PERIOD_MS) % RELEVANT_CELLS;
        if (index == last_cell_index) return;
        set(relevant_cells[last_cell_index].x, relevant_cells[last_cell_index].y, -1);
        last_cell_index = index;
        Cell cell = relevant_cells[index];
        int color_index = (now / COLOR_PERIOD_MS) % 2;
        int found_colors = 0;
        int colors[COLORS];
        for (int i = 0; i < COLORS; i++) {
            if (color[i][cell.x][cell.y]) {
                colors[found_colors++] = i;
            }
        }
        if (found_colors == 1) {
            set(cell.x, cell.y, colors[0]);
        } else if (found_colors == 2) {
            set(cell.x, cell.y, colors[color_index]);
        } else {
            set(cell.x, cell.y, -1);
        }
    }
};