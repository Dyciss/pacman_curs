#pragma once

struct queue_int {
    struct queue_int *next;
    int data;
};

typedef struct {
    struct queue_int *first;
    struct queue_int *last;
} queue_int_t;

queue_int_t *new_qi();
queue_int_t *push_qi(queue_int_t *qi, int data);
int pop_qi(queue_int_t **qi);
void free_qi(queue_int_t *qi);
