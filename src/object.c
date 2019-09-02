
#include "object.h"


void limitObjectSpeed(struct Object *obj, float speed) {
    float curSpeed = sqrt(sqr(obj->speed.x) + sqr(obj->speed.y));

    if (curSpeed > speed) {
        obj->speed.x *= speed/curSpeed;
        obj->speed.y *= speed/curSpeed;
    }
}


void moveObject(struct Object *obj, float framesPassed) {
    obj->geom.center.x += obj->speed.x*framesPassed;
    obj->geom.center.y += obj->speed.y*framesPassed;
}


int initObjectList(struct ObjectList *list) {
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
        } else {
            last = node;
        }

        node = next;
    }

    list->tail = last;
}

