
#include "math_util.h"

#ifndef CONFIG_H
#define CONFIG_H


static const unsigned int SCREEN_WIDTH = 960;
static const unsigned int SCREEN_HEIGHT = 540;
static const unsigned int FPS = 60;
static const float MILLIS_PER_FRAME = 1000.0/60.0;

static const float MOUSE_WHEEL_SENSITIVITY = 10.0;


static const struct Color BACKGROUND_COLOR = {0xFF, 0xFF, 0xFF, 0xFF};
static const struct Color COLLIDED_BACKGROUND_COLOR = {0xFF, 0x88, 0x88, 0xFF};
static const struct Color GOAL_BACKGROUND_COLOR = {0x88, 0xFF, 0x88, 0xFF};

static const struct Color OBSTACLE_COLOR = {0xFF, 0x00, 0x00, 0xFF};
static const struct Color TRACKED_OBSTACLE_COLOR = {0xFF, 0x00, 0xFF, 0x88};
static const float OBSTACLE_DEFAULT_RADIUS = 20;
static const float OBSTACLE_MAX_SPEED = 5;

static const struct Color NEW_OBSTACLE_COLOR = {0xFF, 0x00, 0x00, 0x88};
static const float NEW_OBSTACLE_SPEED_SENSITIVITY = 0.025;

static const struct Color OBSTACLE_PATH_COLOR = {0xFF, 0xFF, 0x00, 0xFF};
static const unsigned int OBSTACLE_PATH_WIDTH = 10;

static const float TRACKING_ERROR = 0.01;

static const struct Color BOT_COLOR = {0x00, 0x00, 0xFF, 0xFF};
static const float BOT_RADIUS = 20;
static const float BOT_MAX_SPEED = 2.5;
// Minimum safe distance between bot and obstacle
static const float BOT_CUSHION = 5;
static const struct Color BOT_SENSE_COLOR = {0x00, 0x00, 0x00, 0x22};
static const float BOT_REACT_RADIUS = 80;
static const float BOT_SENSE_RADIUS = 80;
// How many frames to wait before recalculating which way to rotate about
// stationary objects.
static const float BOT_ROTATE_FRAMES = 15;

static const struct Color GOAL_COLOR = {0x00, 0xFF, 0x00, 0x88};
static const float GOAL_RADIUS = 20;


#endif /* end of include guard: CONFIG_H */
