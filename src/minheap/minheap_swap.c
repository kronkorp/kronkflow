/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Swap two minheap tasks.
*/
#include "minheap.h"
#include "../scheduler/scheduler.h"

void prMinHeap_swap(
    kfTask *task1,
    kfTask *task2
)
{
    kfTask tmp = *task1;

    *task1 = *task2;
    *task2 = tmp;
}
