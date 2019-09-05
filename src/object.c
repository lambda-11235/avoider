
#include "object.h"


void limitObjectSpeed(struct Object *obj, float speed) {
    float curSpeed = magnitude(obj->speed);

    if (curSpeed > speed)
        obj->speed = setMagnitude(obj->speed, speed);
}


void moveObject(struct Object *obj, float framesPassed) {
    struct Vector delta = multScalar(framesPassed, obj->speed);
    obj->geom.center = addVec(obj->geom.center, delta);
}


int initObjectList(struct ObjectList *list) {
    list->length = 0;
    list->head = NULL;
    list->tail = NULL;
    return 0;
}


void freeObjectList(struct ObjectList *list) {
    struct ObjectNode *node = list->head;

    while (node != NULL) {
        struct ObjectNode *tmp = node;
        node = node->next;
        free(tmp);
    }

    list->length = 0;
}


void addObject(struct ObjectList *list, struct Object obj) {
    struct ObjectNode *newNode = malloc(sizeof(struct ObjectNode));
    newNode->object = obj;
    newNode->next = NULL;

    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        list->tail->next = newNode;
        list->tail = newNode;
    }

    list->length++;
}


bool hasObject(struct ObjectList *list, struct Object obj, float error) {
    struct ObjectNode *node = list->head;

    while (node != NULL) {
        if (fabs(node->object.geom.center.x - obj.geom.center.x) < error
            && fabs(node->object.geom.center.y - obj.geom.center.y) < error
            && fabs(node->object.geom.radius - obj.geom.radius) < error
            && fabs(node->object.speed.x - obj.speed.x) < error
            && fabs(node->object.speed.y - obj.speed.y) < error) {
            return true;
        }

        node = node->next;
    }

    return false;
}


void gcObjectList(struct ObjectList *list, struct Rectangle rect) {
    struct ObjectNode *last = NULL;
    struct ObjectNode *next = NULL;
    struct ObjectNode *node = list->head;

    while (node != NULL) {
        next = node->next;

        if (node->object.geom.center.x < rect.pos1.x - node->object.geom.radius ||
            node->object.geom.center.y < rect.pos1.y - node->object.geom.radius ||
            node->object.geom.center.x > rect.pos2.x + node->object.geom.radius ||
            node->object.geom.center.y > rect.pos2.y + node->object.geom.radius) {
            if (last != NULL) {
                last->next = next;
            } else {
                list->head = next;

                if (list->head == NULL)
                    list->tail = NULL;
            }

            free(node);
            list->length--;
        } else {
            last = node;
        }

        node = next;
    }

    list->tail = last;
}

