
#include "../ai.h"


struct Simple2Info {
};


Speed simple2Avoid(const struct Object *bot, struct Circle goal,
    struct ObjectList *obstacles, float framesPassed, void *data);

void simple2Free(struct CollisionAvoid *collAvoid);


void initSimple2Avoid(struct CollisionAvoid *collAvoid) {
    struct Simple2Info *info = malloc(sizeof(struct Simple2Info));

    collAvoid->data = info;
    collAvoid->free = simple2Free;
    collAvoid->avoid = simple2Avoid;
}


void simple2Free(struct CollisionAvoid *collAvoid) {
    free(collAvoid->data);
}


Speed simple2Avoid(const struct Object *bot, struct Circle goal,
    struct ObjectList *obstacles, float framesPassed, void *data) {
    struct Simple2Info *info = data;
    Point pos = bot->geom.center;
    Speed newSpeed;
    float goalDist = distance(pos, goal.center);

    newSpeed.x = (goal.center.x - pos.x);
    newSpeed.y = (goal.center.y - pos.y);

    struct ObjectNode *node = obstacles->head;
    while (node != NULL) {
        Point obsPos = node->object.geom.center;
        float dist = distance(pos, obsPos) - node->object.geom.radius - bot->geom.radius;
        float speed = magnitude(node->object.speed);

        float common_mult = 1/(dist*obstacles->length/BOT_REACT_RADIUS);

        bool approachingBot = projectMultiplier(node->object.speed, subVec(pos, obsPos)) > 0;
        bool betweenBotGoal = distance(obsPos, goal.center) <= goalDist;

        if (speed > 0 && (approachingBot || (betweenBotGoal && magnitude(bot->speed) > speed))) {
            // Clockwise movement relative to speed
            newSpeed.x += -(pos.y - obsPos.y)*common_mult*sqr(speed);
            newSpeed.y += (pos.x - obsPos.x)*common_mult*sqr(speed);

            // Go the other way if this will take us across the obstacle's path.
            float angle = normAngle(angleBetween(obsPos, pos)
                - angleVec(node->object.speed));
            if (angle > M_PI)
                newSpeed = multScalar(-1, newSpeed);
        } else if (betweenBotGoal) {
            newSpeed.x += (pos.x - obsPos.x)*common_mult;
            newSpeed.y += (pos.y - obsPos.y)*common_mult;
        }

        node = node->next;
    }

    return setMagnitude(newSpeed, BOT_MAX_SPEED);
}

