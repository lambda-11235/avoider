
#include "object.h"


#ifndef AI_H
#define AI_H


struct CollisionAvoid {
    void *data;

    void (*free)(struct CollisionAvoid *collAvoid);

    /**
     * Avoidance congestion algorithm.
     *
     * @return The new speed for the bot.
     */
    Speed (*avoid)(const struct Object *bot, struct Circle goal,
        struct ObjectList *obstacles, float framesPassed, void *data);
};


void doNothingFree(struct CollisionAvoid *collAvoid);


/**
 * A really simple collision avoidance algorithm.
 *
 * psuedo-code
 *
 * if no obstacles detected
 *     go straight to goal
 * else
 *     obs = closest obstacle
 *
 *     if obs is moving
 *         move perpendicular to obs's movement
 *     else if obs is within a cushion
 *         move directly away from obs
 *     else if bot is closer to goal than obs
 *         go straight to goal
 *     else
 *         move around obs's circumference
 */
void initSimpleAvoid(struct CollisionAvoid *collAvoid);

void initSimple2Avoid(struct CollisionAvoid *collAvoid);


#endif /* end of include guard: AI_H */
