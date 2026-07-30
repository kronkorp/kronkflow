/*
** EPITECH PROJECT, 2026
** KRONKFLOW
** File description:
** Generic linked list as a queue
*/
#ifndef KRONKFLOW_QUEUE_H
    #define KRONKFLOW_QUEUE_H
    #include <stdbool.h>
    #include <stdlib.h>

typedef struct queue_node_s {

    void *data;
    struct queue_node_s *next;

} queue_node_t;

typedef struct queue_s {

    queue_node_t *head;
    queue_node_t *tail;
    size_t size;

} queue_t;

typedef struct queue_s kfQueue;

queue_t *queue_create(void);
int queue_init(kfQueue *q);
bool queue_push(queue_t *q, void *data);
void *queue_pop(queue_t *q);
void *queue_front(const queue_t *q);
bool queue_empty(const queue_t *q);
size_t queue_size(const queue_t *q);
void queue_clear(queue_t *q, void (*free_func)(void *));
void queue_destroy(queue_t *q, void (*free_func)(void *));

#endif /* GUL_QUEUE_H */
