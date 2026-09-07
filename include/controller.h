#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "raylib.h"

#define A_BIT_POS 0
#define B_BIT_POS 1
#define SELECT_BIT_POS 2
#define START_BIT_POS 3
#define UP_BIT_POS 4
#define DOWN_BIT_POS 5
#define LEFT_BIT_POS 6
#define RIGHT_BIT_POS 7

typedef struct controller
{
    bool a;
    bool b;
    bool select;
    bool start;
    bool up;
    bool down;
    bool left;
    bool right;
    uint8_t shift_register;
    bool strobe;
} Controller;

void controller_update(Controller *controller);


uint8_t controller_read(Controller *controller);



void controller_write(Controller *controller, uint8_t value);