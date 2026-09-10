/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Regression tests for the scheduler public API
*/
#include <kronklab/kronklab.h>
#include <kronkflow/macros/types.h>
#include <kronkflow/scheduler.h>
#include <kronkflow/task.h>

typedef struct {
    int calls;
    int cleared;
} Counters;

static kfBool count_call(
    void *context,
    void *data
)
{
    (void)data;
    ((Counters *)context)->calls++;
    return kfTrue;
}

static void count_clear(
    void *data
)
{
    ((Counters *)data)->cleared++;
}

Test(scheduler, create_and_destroy_empty)
{
    kfScheduler *sch = kfScheduler_create(4);

    AssertNotNull(sch, "create should succeed");
    kfScheduler_destroy(sch);
}

Test(scheduler, destroy_after_buckets_used)
{
    kfScheduler *sch = kfScheduler_create(4);
    Counters counters = { 0, 0 };
    kfTaskOpt opt = kfTask_opt(&count_call, NULL, NULL);

    AssertNotNull(sch, "create should succeed");
    opt.stage = 0;
    kfScheduler_addTask(sch, opt, 1, 0);
    opt.stage = 3;
    kfScheduler_addTask(sch, opt, 1, 0);
    opt.stage = 7;
    kfScheduler_addTask(sch, opt, 1, 0);
    kfScheduler_tick(sch, &counters);
    AssertEq(counters.calls, 3, "all three staged tasks should have run");
    /* Regression: freeing kuDynarray_at(sch->staged, i) instead of
    ** sch->staged[i] used to double-free the outer bucket array. */
    kfScheduler_destroy(sch);
}

Test(scheduler, tick_return_counts_executed)
{
    kfScheduler *sch = kfScheduler_create(4);
    Counters counters = { 0, 0 };
    kfTaskOpt opt = kfTask_opt(&count_call, NULL, NULL);
    size_t done;

    kfScheduler_addTask(sch, opt, 1, 0);
    kfScheduler_addTask(sch, opt, 1, 0);
    /* Regression: sch->staged's load was never advanced past 0, so the
    ** stage-processing loop used to iterate zero times and every tick
    ** silently ran nothing. */
    done = kfScheduler_tick(sch, &counters);
    AssertEq(done, (size_t)2, "tick should report the number of tasks it ran");
    AssertEq(counters.calls, 2, "handlers should have actually been invoked");
    kfScheduler_destroy(sch);
}

Test(scheduler, task_not_due_not_executed)
{
    kfScheduler *sch = kfScheduler_create(4);
    Counters counters = { 0, 0 };
    kfTaskOpt opt = kfTask_opt(&count_call, NULL, NULL);
    size_t i;

    kfScheduler_addTask(sch, opt, 5, 0);
    for (i = 0; i < 4; i++) {
        kfScheduler_tick(sch, &counters);
    }
    AssertEq(counters.calls, 0, "should not run before its target tick");
    kfScheduler_tick(sch, &counters);
    AssertEq(counters.calls, 1, "should run exactly on its target tick");
    kfScheduler_destroy(sch);
}

Test(scheduler, interval_zero_runs_once)
{
    kfScheduler *sch = kfScheduler_create(4);
    Counters counters = { 0, 0 };
    kfTaskOpt opt = kfTask_opt(&count_call, NULL, NULL);
    size_t i;

    kfScheduler_addTask(sch, opt, 1, 0);
    for (i = 0; i < 20; i++) {
        kfScheduler_tick(sch, &counters);
    }
    AssertEq(counters.calls, 1, "a task with interval 0 must not be rescheduled");
    kfScheduler_destroy(sch);
}

Test(scheduler, interval_task_reschedules)
{
    kfScheduler *sch = kfScheduler_create(4);
    Counters counters = { 0, 0 };
    kfTaskOpt opt = kfTask_opt(&count_call, NULL, NULL);
    size_t i;

    kfScheduler_addTask(sch, opt, 1, 3);
    for (i = 0; i < 30; i++) {
        kfScheduler_tick(sch, &counters);
    }
    AssertEq(counters.calls, 10,
        "a task rescheduled every 3 ticks starting at tick 1 should run "
        "at ticks 1,4,7,...,28 within 30 ticks");
    kfScheduler_destroy(sch);
}

Test(scheduler, non_reschedule_calls_clearer)
{
    kfScheduler *sch = kfScheduler_create(4);
    Counters counters = { 0, 0 };
    kfTaskOpt opt = kfTask_opt(&count_call, &counters, &count_clear);

    kfScheduler_addTask(sch, opt, 1, 0);
    kfScheduler_tick(sch, &counters);
    AssertEq(counters.cleared, 1, "clearer should run once the task is not rescheduled");
    kfScheduler_destroy(sch);
}

Test(scheduler, reschedule_skips_clearer)
{
    kfScheduler *sch = kfScheduler_create(4);
    Counters counters = { 0, 0 };
    kfTaskOpt opt = kfTask_opt(&count_call, &counters, &count_clear);

    kfScheduler_addTask(sch, opt, 1, 2);
    kfScheduler_tick(sch, &counters);
    AssertEq(counters.cleared, 0, "a rescheduled task's data is still owned, clearer must not fire");
    kfScheduler_destroy(sch);
}

Test(scheduler, addTask_null_sch_safe)
{
    kfTaskOpt opt = kfTask_opt(&count_call, NULL, NULL);

    AssertEq(kfScheduler_addTask(NULL, opt, 1, 0), (kfTaskID)0,
        "adding to a null scheduler should fail cleanly");
}

Test(scheduler, tick_null_sch_safe)
{
    AssertEq(kfScheduler_tick(NULL, NULL), (size_t)0,
        "ticking a null scheduler should return zero");
}

Test(scheduler, destroy_null_sch_safe)
{
    kfScheduler_destroy(NULL);
}

Test(scheduler, tick_count_advances)
{
    kfScheduler *sch = kfScheduler_create(4);
    size_t i;

    AssertEq(kfScheduler_currentTick(sch), (kfTick)0, "a fresh scheduler starts at tick 0");
    for (i = 0; i < 5; i++) {
        kfScheduler_tick(sch, NULL);
    }
    AssertEq(kfScheduler_currentTick(sch), (kfTick)5, "five ticks should advance the clock by five");
    kfScheduler_destroy(sch);
}
