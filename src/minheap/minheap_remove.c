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
    if (idx >= sch->count) {
        return;
    }
    // NOTE: Removing in the middle of the heap, the task moved in can be
    // earlier than its new parent: it has to go up, not only down.
    if (idx > 0 && prMinHeap_less(&sch->tasks[idx], &sch->tasks[(idx - 1) >> 1])) {
        prMinHeap_add(sch, idx);
        return;
    }
    while (1) {
        left = (idx << 1) + 1;
        right = (idx << 1) + 2;
        smallest = idx;
        if (left < sch->count && prMinHeap_less(&sch->tasks[left], &sch->tasks[smallest]))
            smallest = left;
        if (right < sch->count && prMinHeap_less(&sch->tasks[right], &sch->tasks[smallest]))
            smallest = right;
        if (smallest != idx) {
            prMinHeap_swap(&sch->tasks[idx], &sch->tasks[smallest]);
            idx = smallest;
        } else {
            break;
        }
    }
}
