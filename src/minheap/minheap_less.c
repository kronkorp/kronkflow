/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Order of two tasks in the minheap
*/
#include "minheap.h"
#include "../scheduler/scheduler.h"

/*
** Earliest target first. Ids are handed out in registration order and never
** change, so tasks with the same target run in the order they were added.
*/
bool prMinHeap_less(
    const kfTask *a,
    const kfTask *b
)
{
    if (a->target != b->target) {
        return a->target < b->target;
    }
    return a->id < b->id;
}
