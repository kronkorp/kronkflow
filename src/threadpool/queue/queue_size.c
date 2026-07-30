/*
** EPITECH PROJECT, 2026
** PANORAMIX
** File description:
** Queue is empty
*/
#include "queue.h"
#include <stdlib.h>

size_t queue_size(const queue_t *q)
{
    if (!q->size)
        return 0;
    return q->size;
}
