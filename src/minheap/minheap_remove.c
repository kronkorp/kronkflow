/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Remove a task from the minheap
*/
#include "../scheduler/scheduler.h"
#include "minheap.h"
#include <stddef.h>

void prMinHeap_remove(
    kfScheduler *sch,
    size_t idx
)
{
    size_t left, right, smallest;

    sch->tasks[idx] = sch->tasks[sch->count - 1];
    sch->count--;
    while (1) {
        left = (idx << 1) + 1;
        right = (idx << 1) + 2;
        smallest = idx;
        if (left < sch->count && sch->tasks[left].target < sch->tasks[smallest].target)
            smallest = left;
        if (right < sch->count && sch->tasks[right].target < sch->tasks[smallest].target)
            smallest = right;
        if (smallest != idx) {
            prMinHeap_swap(&sch->tasks[idx], &sch->tasks[smallest]);
            idx = smallest;
        } else {
            break;
        }
    }
}
