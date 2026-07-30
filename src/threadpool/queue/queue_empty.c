/*
** EPITECH PROJECT, 2026
** PANORAMIX
** File description:
** Queue is empty
*/
#include "queue.h"
#include <stdlib.h>

bool queue_empty(const queue_t *q)
{
    return (q == NULL || q->size == 0);
}
