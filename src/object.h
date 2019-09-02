
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "config.h"


#ifndef OBJECT_H
#define OBJECT_H


#define OBJ_LIST_INIT_CAPACITY 8


struct Object {
    struct Circle geom;
    struct Speed speed;
};

struct ObjectNode {
    struct Object object;
    struct ObjectNode *next;
};

struct ObjectList {
    struct ObjectNode *head;
    struct ObjectNode *tail;
};


void limitObjectSpeed(struct Object *obj, float speed);

/** Move and object a specific speed.
 *
 * @param framesPassed The number of frames (1/60s) that have passed.
 */
void moveObject(struct Object *obj, float framesPassed);


int initObjectList(struct ObjectList *list);
void freeObjectList(struct ObjectList *list);
void addObject(struct ObjectList *list, struct Object obj);

/**
 * Detects if an object is already in the list.
 *
 * @param error The margin of error in object dimensions for an object to be
 * considered the same.
 */
bool hasObject(struct ObjectList *list, struct Object obj, float error);

/**
 * Deletes all objects not in rect.
 */
void gcObjectList(struct ObjectList *list, struct Rectangle rect);


#endif /* end of include guard: OBJECT_H */
