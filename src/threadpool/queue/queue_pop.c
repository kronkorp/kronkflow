/*
** EPITECH PROJECT, 2026
** PANORAMIX
** File description:
** Queue pop
*/
#include "queue.h"
#include <stdbool.h>
#include <stdlib.h>

void *queue_pop(queue_t *q)
{
    queue_node_t *temp;
    void *data;

    if (queue_empty(q))
        return NULL;
    temp = q->head;
    data = temp->data;
    q->head = q->head->next;
    if (q->head == NULL)
        q->tail = NULL;
    free(temp);
    q->size--;
    return data;
}
