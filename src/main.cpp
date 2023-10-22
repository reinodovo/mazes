#include <Arduino.h>

const int CHANNELS = 3;

const int RED_PINS[CHANNELS] = {25, 14, 2};
const int GREEN_PINS[CHANNELS] = {33, 27, 15};
const int BLUE_PINS[CHANNELS] = {32, 26, 13};

const int CLOCK = 19, STORE = 18, SER = 5;

const int GRID_SIZE = 6;

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

void set(int row, int col, int red, int green, int blue) {
    int row_group = (row * 2) + (col / 3);
    int row_group_serial = ROW_GROUPS_SERIAL[row_group];
    for (int i = 0; i < CHANNELS; i++) {
        digitalWrite(RED_PINS[i], HIGH);
        digitalWrite(GREEN_PINS[i], HIGH);
        digitalWrite(BLUE_PINS[i], HIGH);
    }
    send(row_group_serial);
    int channel = col % 3;
    digitalWrite(RED_PINS[channel], red ? LOW : HIGH);
    digitalWrite(GREEN_PINS[channel], green ? LOW : HIGH);
    digitalWrite(BLUE_PINS[channel], blue ? LOW : HIGH);
}

void setup() {
    pinMode(CLOCK, OUTPUT);
    pinMode(STORE, OUTPUT);
    pinMode(SER, OUTPUT);

    for (int i = 0; i < CHANNELS; i++) {
        pinMode(RED_PINS[i], OUTPUT);
        pinMode(GREEN_PINS[i], OUTPUT);
        pinMode(BLUE_PINS[i], OUTPUT);
        digitalWrite(RED_PINS[i], LOW);
        digitalWrite(GREEN_PINS[i], LOW);
        digitalWrite(BLUE_PINS[i], LOW);
    }
}

void loop() {
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            set(row, col, 1, 0, 0);
            delay(20);
        }
    }
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            set(row, col, 0, 1, 0);
            delay(20);
        }
    }
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            set(row, col, 0, 0, 1);
            delay(20);
        }
    }
}