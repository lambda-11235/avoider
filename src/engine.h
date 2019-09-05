
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "ai.h"
#include "config.h"
#include "object.h"


#ifndef ENGINE_H
#define ENGINE_H


struct MouseDrag {
    bool active;
    Point start;
    Point end;
};

struct EngineInfo {
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running;
    bool paused;
    bool collided;

    unsigned int lastTicks;

    struct MouseDrag leftMouseDrag;
    struct MouseDrag rightMouseDrag;

    struct Object newObstacle;
    struct Object newStationaryObstacle;

    struct Object bot;
    struct ObjectList obstacles;
    struct ObjectList trackedObstacles;
    struct Circle goal;

    struct CollisionAvoid collAvoid;
};


int initEngine(struct EngineInfo *info);
void quitEngine(struct EngineInfo *info);

/**
 * Executes processEvents, runPhysics, and drawScene while managing framerate.
 */
void stepFrame(struct EngineInfo *info);
void processEvents(struct EngineInfo *info, float framesPassed);
void runPhysics(struct EngineInfo *info, float framesPassed);
void drawScene(struct EngineInfo *info, float framesPassed);


#endif /* end of include guard: ENGINE_H */
