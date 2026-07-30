/*
** EPITECH PROJECT, 2026
** PANORAMIX
** File description:
** Queue front
*/
#include "queue.h"
#include <stdlib.h>

void *queue_front(const queue_t *q)
{
    if (!q || q->size == 0 || !q->head)
        return NULL;
    return q->head->data;
}
