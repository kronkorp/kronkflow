/*
** EPITECH PROJECT, 2026
** PANORAMIX
** File description:
** Destroy queue
*/
#include "queue.h"
#include <stdlib.h>

void queue_destroy(queue_t *q, void (*free_func)(void *))
{
    if (!q)
        return;
    queue_clear(q, free_func);
    free(q);
}

void queue_clear(
    queue_t *q,
    void (*free_func)(void *)
)
{
    void *data;

    if (!q)
        return;
    while (!queue_empty(q)) {
        data = queue_pop(q);
        if (free_func && data) {
            free_func(data);
        }
    }
}
