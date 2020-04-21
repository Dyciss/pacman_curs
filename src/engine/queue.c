#include "queue.h"
#include <stdlib.h>

queue_int_t *new_qi() {
    queue_int_t *qi = (queue_int_t *)malloc(sizeof(queue_int_t));
    qi->first = (struct queue_int *)malloc(sizeof(struct queue_int));
    qi->first->next = NULL;
    qi->last = qi->first;
    return qi;
}

queue_int_t *push_qi(queue_int_t *qi, int data) {
    // check empty queue
    struct queue_int *new =
        (struct queue_int *)malloc(sizeof(struct queue_int));
    new->data = data;
    // no elements, new is first
    if (qi->last->next == NULL) {
        new->next = qi->last;
        qi->last = new;
        qi->first = new;
    } else {
        new->next = qi->last->next;
        qi->last->next = new;
        qi->last = new;
    }

    return qi;
}

int pop_qi(queue_int_t **qi) {
    int data = (*qi)->first->data;
    struct queue_int *first = (*qi)->first;
    (*qi)->first = first->next;
    free(first);
    if ((*qi)->first->next == NULL) {
        (*qi)->last = (*qi)->first;
    }
    return data;
}

void free_qi(queue_int_t *qi) {
    struct queue_int *first = NULL;
    while (qi->first) {
        first = qi->first->next;
        free(qi->first);
        qi->first = first;
    }
    free(qi);
}
