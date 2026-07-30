/*
** EPITECH PROJECT, 2026
** PANORAMIX
** File description:
** Queue push
*/
#include "queue.h"
#include <stdbool.h>
#include <stdlib.h>

bool queue_push(queue_t *q, void *data)
{
    queue_node_t *new_node;

    if (!q)
        return false;
    new_node = malloc(sizeof(queue_node_t));
    if (!new_node)
        return false;
    new_node->data = data;
    new_node->next = NULL;
    if (queue_empty(q)) {
        q->head = new_node;
        q->tail = new_node;
    } else {
        q->tail->next = new_node;
        q->tail = new_node;
    }
    q->size++;
    return true;
}
