/*
** EPITECH PROJECT, 2026
** PANORAMIX
** File description:
** Create queue
*/
#include "queue.h"
#include <stdlib.h>

queue_t *queue_create(void)
{
    queue_t *q = calloc(1, sizeof(queue_t));

    if (queue_init(q) == -1) {
        free(q);
        return NULL;
    }
    return q;
}

int queue_init(kfQueue *q)
{
    if (!q)
        return -1;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return 0;
}
