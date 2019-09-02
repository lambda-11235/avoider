
#include "engine.h"

#include <SDL2_gfxPrimitives.h>


int initEngine(struct EngineInfo *info) {
    info->running = true;
    info->paused = true;
    info->collided = false;

    info->leftMouseDrag.active = false;
    info->rightMouseDrag.active = false;

    info->bot.geom = (struct Circle) {{BOT_RADIUS, BOT_RADIUS}, BOT_RADIUS};
    info->bot.speed = (struct Speed) {0, 0};

    initObjectList(&info->obstacles);

    info->goal = (struct Circle) {{SCREEN_WIDTH - GOAL_RADIUS, SCREEN_HEIGHT - GOAL_RADIUS},
        GOAL_RADIUS};

    initSimpleAvoid(&info->collAvoid);


    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return 1;
    }


    info->window = SDL_CreateWindow("avoider",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (info->window == NULL) {
        return 1;
    }

    info->renderer = SDL_CreateRenderer(info->window, -1, SDL_RENDERER_ACCELERATED);

    return 0;
}


void quitEngine(struct EngineInfo *info) {
    info->collAvoid.free(&info->collAvoid);

    freeObjectList(&info->obstacles);

    SDL_DestroyRenderer(info->renderer);
    SDL_DestroyWindow(info->window);
    SDL_Quit();
}


void stepFrame(struct EngineInfo *info) {
    int ticks = SDL_GetTicks();
    // A frame is 1/60 of a second, used to establish the simulation tempo
    float framesPassed = (ticks - info->lastTicks)/MILLIS_PER_FRAME;
    info->lastTicks = ticks;

    processEvents(info, framesPassed);
    runPhysics(info, framesPassed);
    drawScene(info, framesPassed);

    SDL_Delay(1000/FPS - (SDL_GetTicks() - info->lastTicks));
}


void processEvents(struct EngineInfo *info, float framesPassed) {
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            info->running = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_q: case SDLK_ESCAPE:
                    info->running = false;
                    break;
                case SDLK_p: case SDLK_SPACE:
                    info->paused = !info->paused;
                    break;
            }
        } else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
            }
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                info->leftMouseDrag.active = true;
                info->leftMouseDrag.start.x = event.button.x;
                info->leftMouseDrag.start.y = event.button.y;
                info->leftMouseDrag.end.x = event.button.x;
                info->leftMouseDrag.end.y = event.button.y;

                // Start new obstacle creation
                info->newObstacle.geom = (struct Circle)
                    {{event.button.x, event.button.y}, OBSTACLE_DEFAULT_RADIUS};
            } else if (event.button.button == SDL_BUTTON_RIGHT) {
                info->rightMouseDrag.active = true;
                info->rightMouseDrag.start.x = event.button.x;
                info->rightMouseDrag.start.y = event.button.y;
                info->rightMouseDrag.end.x = event.button.x;
                info->rightMouseDrag.end.y = event.button.y;

                // Start new obstacle creation
                info->newStationaryObstacle.geom = (struct Circle)
                    {{event.button.x, event.button.y}, OBSTACLE_DEFAULT_RADIUS};
            }
        } else if (event.type == SDL_MOUSEBUTTONUP) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                info->leftMouseDrag.active = false;

                // Add a new obstacle to the scene.
                info->newObstacle.speed.x = NEW_OBSTACLE_SPEED_SENSITIVITY
                    * (info->leftMouseDrag.end.x - info->leftMouseDrag.start.x);

                info->newObstacle.speed.y = NEW_OBSTACLE_SPEED_SENSITIVITY
                    * (info->leftMouseDrag.end.y - info->leftMouseDrag.start.y);

                limitObjectSpeed(&info->newObstacle, OBSTACLE_MAX_SPEED);

                addObject(&info->obstacles, info->newObstacle);
            } else if (event.button.button == SDL_BUTTON_RIGHT) {
                info->rightMouseDrag.active = false;

                // Add a new obstacle to the scene.
                info->newStationaryObstacle.speed.x = 0;
                info->newStationaryObstacle.speed.y = 0;
                addObject(&info->obstacles, info->newStationaryObstacle);
            } else if (event.button.button == SDL_BUTTON_MIDDLE) {
                info->goal.center.x = event.button.x;
                info->goal.center.y = event.button.y;
            }
        } else if (event.type == SDL_MOUSEMOTION) {
            if (info->leftMouseDrag.active) {
                info->leftMouseDrag.end.x = event.motion.x;
                info->leftMouseDrag.end.y = event.motion.y;
            }

            if (info->rightMouseDrag.active) {
                info->rightMouseDrag.end.x = event.motion.x;
                info->rightMouseDrag.end.y = event.motion.y;
            }
        } else if (event.type == SDL_MOUSEWHEEL) {
            // Change new obstacle radius.
            info->newObstacle.geom.radius += MOUSE_WHEEL_SENSITIVITY*event.wheel.y;
            info->newStationaryObstacle.geom.radius += MOUSE_WHEEL_SENSITIVITY*event.wheel.y;
        }
    }
}


void runPhysics(struct EngineInfo *info, float framesPassed) {
    int w, h;

    SDL_GetRendererOutputSize(info->renderer, &w, &h);
    gcObjectList(&info->obstacles, (struct Rectangle) {{0, 0}, {w, h}});

    if (!info->paused) {
        // Determine what objects are in sensory range, call avoidance
        // algorithm, and move objects.
        struct ObjectList senseObjects;
        initObjectList(&senseObjects);

        struct ObjectNode *node = info->obstacles.head;
        while (node != NULL) {
            moveObject(&node->object, framesPassed);

            if (overlapping(node->object.geom, info->bot.geom)) {
                info->collided = true;
            } else if (overlapping(node->object.geom,
                (struct Circle) {info->bot.geom.center, BOT_SENSE_RADIUS})) {
                addObject(&senseObjects, node->object);
            }

            node = node->next;
        }

        info->bot.speed = info->collAvoid.avoid(&info->bot, info->goal,
            &senseObjects, framesPassed, info->collAvoid.data);
        limitObjectSpeed(&info->bot, BOT_MAX_SPEED);

        freeObjectList(&senseObjects);

        moveObject(&info->bot, framesPassed);
    }
}


void drawCircle(struct EngineInfo *info, struct Circle circle,
    struct Color color) {
    filledCircleRGBA(info->renderer, circle.center.x, circle.center.y, circle.radius,
        color.red, color.green, color.blue, color.alpha);
}

void drawScene(struct EngineInfo *info, float framesPassed) {
    // Draw background
    if (info->collided) {
        SDL_SetRenderDrawColor(info->renderer,
            COLLIDED_BACKGROUND_COLOR.red, COLLIDED_BACKGROUND_COLOR.green, COLLIDED_BACKGROUND_COLOR.blue,
            COLLIDED_BACKGROUND_COLOR.alpha);
    } else if (overlapping(info->bot.geom, info->goal)) {
        SDL_SetRenderDrawColor(info->renderer,
            GOAL_BACKGROUND_COLOR.red, GOAL_BACKGROUND_COLOR.green, GOAL_BACKGROUND_COLOR.blue,
            GOAL_BACKGROUND_COLOR.alpha);
    } else {
        SDL_SetRenderDrawColor(info->renderer,
            BACKGROUND_COLOR.red, BACKGROUND_COLOR.green, BACKGROUND_COLOR.blue,
            BACKGROUND_COLOR.alpha);
    }

    SDL_RenderClear(info->renderer);


    // Draw new obstacle graphic
    if (info->leftMouseDrag.active) {
        thickLineRGBA(info->renderer,
            info->leftMouseDrag.start.x,
            info->leftMouseDrag.start.y,
            info->leftMouseDrag.end.x,
            info->leftMouseDrag.end.y,
            OBSTACLE_PATH_WIDTH,
            OBSTACLE_PATH_COLOR.red, OBSTACLE_PATH_COLOR.green, OBSTACLE_PATH_COLOR.blue,
            OBSTACLE_PATH_COLOR.alpha);

        drawCircle(info, info->newObstacle.geom, NEW_OBSTACLE_COLOR);
    }

    if (info->rightMouseDrag.active) {
        drawCircle(info, info->newStationaryObstacle.geom, NEW_OBSTACLE_COLOR);
    }

    // Draw bot
    drawCircle(info, info->bot.geom, BOT_COLOR);
    drawCircle(info, (struct Circle) {info->bot.geom.center, BOT_SENSE_RADIUS}, BOT_SENSE_COLOR);

    // Draw obstacles
    struct ObjectNode *node = info->obstacles.head;
    while (node != NULL) {
        drawCircle(info, node->object.geom, OBSTACLE_COLOR);
        node = node->next;
    }

    // Draw goal
    drawCircle(info, info->goal, GOAL_COLOR);

    SDL_RenderPresent(info->renderer);
}
