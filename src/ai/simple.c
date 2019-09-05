
#include "../ai.h"


struct SimpleInfo {
    bool rotateCounter;
    float framesLeft;
};


Speed simpleAvoid(const struct Object *bot, struct Circle goal,
    struct ObjectList *obstacles, float framesPassed, void *data);

void simpleFree(struct CollisionAvoid *collAvoid);


void initSimpleAvoid(struct CollisionAvoid *collAvoid) {
    struct SimpleInfo *info = malloc(sizeof(struct SimpleInfo));
    info->framesLeft = 0;

    collAvoid->data = info;
    collAvoid->free = simpleFree;
    collAvoid->avoid = simpleAvoid;
}


void simpleFree(struct CollisionAvoid *collAvoid) {
    free(collAvoid->data);
}


Speed simpleAvoid(const struct Object *bot, struct Circle goal,
    struct ObjectList *obstacles, float framesPassed, void *data) {
    struct SimpleInfo *info = data;
    Point pos = bot->geom.center;
    Speed newSpeed;


    // Grab closest object
    struct ObjectNode *node = obstacles->head;
    struct Object *obs = NULL;

    // Distance from bots surface to closest obstacle's surface.
    float dist = BOT_REACT_RADIUS - bot->geom.radius;

    while (node != NULL) {
        float nextDist = distance(pos, node->object.geom.center)
            - node->object.geom.radius - bot->geom.radius;

        if (nextDist < dist) {
            obs = &node->object;
            dist = nextDist;
        }

        node = node->next;
    }


    if (obs == NULL) {
        newSpeed.x = goal.center.x - pos.x;
        newSpeed.y = goal.center.y - pos.y;

        info->framesLeft -= framesPassed;
    } else {
        if (magnitude(obs->speed) > 0) {
            // Clockwise movement relative to speed
            newSpeed.x = -obs->speed.y;
            newSpeed.y = obs->speed.x;

            // Go the other way if this will take us across the obstacle's path.
            float angle = normAngle(angleBetween(obs->geom.center, pos)
                - angleVec(obs->speed));
            if (angle > M_PI)
                newSpeed = multScalar(-1, newSpeed);
        } else if (dist < BOT_CUSHION) {
            newSpeed.x = pos.x - obs->geom.center.x;
            newSpeed.y = pos.y - obs->geom.center.y;
        } else if (distance(pos, goal.center) < distance(obs->geom.center, goal.center)
            || distance(pos, goal.center) < distance(pos, obs->geom.center)) {
            newSpeed.x = (goal.center.x - pos.x);
            newSpeed.y = (goal.center.y - pos.y);
        } else {
            if (info->framesLeft <= 0) {
                float angle = normAngle(angleBetween(pos, obs->geom.center)
                    - angleBetween(pos, goal.center));
                info->rotateCounter = angle > M_PI;
            }

            // Clockwise rotation about object
            newSpeed.x = -(pos.y - obs->geom.center.y);
            newSpeed.y = pos.x - obs->geom.center.x;

            // Flip if we're taking the long way round.
            if (info->rotateCounter)
                newSpeed = multScalar(-1, newSpeed);

            info->framesLeft = BOT_ROTATE_FRAMES;
        }
    }

    return setMagnitude(newSpeed, BOT_MAX_SPEED);
}
