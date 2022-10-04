#ifndef VARIABLES_H
#define VARIABLES_H

#include "olcPixelGameEngine.h"

const int MAP_WIDTH = 24;
const int MAP_HEIGHT = 24;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_SCALE_WIDTH = 1;
const int SCREEN_SCALE_HEIGHT = 1;

const double MOVE_SPEED = 5.0;
const double ROT_SPEED = 3.0;

const olc::Key MOVE_FWD_KEY = olc::Key::W;
const olc::Key MOVE_BACK_KEY = olc::Key::S;
const olc::Key STRAFE_LEFT_KEY = olc::Key::A;
const olc::Key STRAFE_RIGHT_KEY = olc::Key::D;
const olc::Key ROTATE_LEFT_KEY = olc::Key::LEFT;
const olc::Key ROTATE_RIGHT_KEY = olc::Key::RIGHT;

#endif