/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Add a task to the minheap
*/
#include "minheap.h"
#include "../scheduler/scheduler.h"
/*
** To know parent      : (i - 1) / 2 (>> 1)
** To know left child  : (2 * i) + 1 (<< 1)
** To know right child : (2 * i) + 2 (<< 1)
*/
// DEBUG: Bubble up
void prMinHeap_add(
    prScheduler *sch,
    size_t idx
)
{
    while (idx > 0 && sch->tasks[(idx - 1) >> 1].target > sch->tasks[idx].target) {
        prMinHeap_swap(&sch->tasks[idx],  &sch->tasks[(idx - 1) >> 1]);
        idx = (idx - 1) >> 1;
    }
}
