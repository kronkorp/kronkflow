/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Tests for staged (per-stage bucketed) execution
*/
#include <kronklab/kronklab.h>
#include <kronkflow/macros/types.h>
#include <kronkflow/scheduler.h>
#include <kronkflow/task.h>
#include <stdint.h>

typedef struct {
    int order[64];
    int len;
} StageOrder;

static kfBool record_stage(
    void *context,
    void *data
)
{
    StageOrder *rec = context;
    long marker = (long)(intptr_t)data;

    if (rec->len < 64) {
        rec->order[rec->len++] = (int)marker;
    }
    return kfTrue;
}

static kfTaskOpt marked_task(
    kfStageId stage,
    long marker
)
{
    return kfTask_opt(&record_stage, (void *)(intptr_t)marker, NULL,
        stage, (kfRWMasks){ 0, 0 });
}

Test(staging, sorted_by_stage_not_insertion)
{
    kfScheduler *sch = kfScheduler_create(4);
    StageOrder rec = { { 0 }, 0 };

    kfScheduler_addTask(sch, marked_task(3, 3), 1, 0);
    kfScheduler_addTask(sch, marked_task(0, 0), 1, 0);
    kfScheduler_addTask(sch, marked_task(5, 5), 1, 0);
    kfScheduler_addTask(sch, marked_task(1, 1), 1, 0);

    kfScheduler_tick(sch, &rec);

    AssertEq(rec.len, 4, "all four tasks should have run");
    AssertEq(rec.order[0], 0, "stage 0 should run first");
    AssertEq(rec.order[1], 1, "stage 1 should run second");
    AssertEq(rec.order[2], 3, "stage 3 should run third");
    AssertEq(rec.order[3], 5, "stage 5 should run last");
    kfScheduler_destroy(sch);
}

Test(staging, same_stage_runs_together)
{
    kfScheduler *sch = kfScheduler_create(4);
    StageOrder rec = { { 0 }, 0 };
    int seen_11 = 0;
    int seen_22 = 0;
    int seen_33 = 0;
    int i;

    kfScheduler_addTask(sch, marked_task(2, 11), 1, 0);
    kfScheduler_addTask(sch, marked_task(2, 22), 1, 0);
    kfScheduler_addTask(sch, marked_task(2, 33), 1, 0);
    kfScheduler_addTask(sch, marked_task(5, 99), 1, 0);

    kfScheduler_tick(sch, &rec);

    AssertEq(rec.len, 4, "all four tasks should have run");
    AssertEq(rec.order[3], 99, "the later stage must run after every stage-2 task");
    /* The min-heap gives no ordering guarantee among tasks sharing the same
    ** target tick, so same-stage siblings may land in any relative order -
    ** only check that all three actually ran, as a set. */
    for (i = 0; i < 3; i++) {
        if (rec.order[i] == 11) {
            seen_11 = 1;
        }
        if (rec.order[i] == 22) {
            seen_22 = 1;
        }
        if (rec.order[i] == 33) {
            seen_33 = 1;
        }
    }
    AssertEq(seen_11, 1, "task 11 should have run");
    AssertEq(seen_22, 1, "task 22 should have run");
    AssertEq(seen_33, 1, "task 33 should have run");
    kfScheduler_destroy(sch);
}

Test(staging, sparse_stage_no_phantom_tasks)
{
    kfScheduler *sch = kfScheduler_create(4);
    StageOrder rec = { { 0 }, 0 };
    size_t done;

    /* Regression: the outer bucket array's freshly-grown capacity was
    ** never zeroed, so lower/unused stage slots held garbage pointers
    ** instead of NULL. */
    kfScheduler_addTask(sch, marked_task(9, 9), 1, 0);
    done = kfScheduler_tick(sch, &rec);

    AssertEq(done, (size_t)1, "only the single staged task should run");
    AssertEq(rec.len, 1, "no phantom task from unused lower stages should run");
    AssertEq(rec.order[0], 9, "the one task should still run at its own stage");
    kfScheduler_destroy(sch);
}

Test(staging, buckets_cleared_and_reused)
{
    kfScheduler *sch = kfScheduler_create(4);
    StageOrder rec = { { 0 }, 0 };

    kfScheduler_addTask(sch, marked_task(2, 1), 1, 0);
    kfScheduler_tick(sch, &rec);
    AssertEq(rec.len, 1, "the first tick should run the stage-2 task once");

    kfScheduler_tick(sch, &rec);
    AssertEq(rec.len, 1, "an empty tick must not replay the previous tick's bucket");

    kfScheduler_addTask(sch, marked_task(2, 2), 1, 0);
    kfScheduler_tick(sch, &rec);
    AssertEq(rec.len, 2, "a new task pushed into a reused bucket should run exactly once");
    kfScheduler_destroy(sch);
}

Test(staging, growing_stage_range_stays_ok)
{
    kfScheduler *sch = kfScheduler_create(4);
    StageOrder rec = { { 0 }, 0 };
    size_t stage;
    size_t total_done = 0;

    /* Forces sch->staged to grow its capacity on almost every iteration,
    ** exercising the resize -> zero-new-slots -> sync-load path. */
    for (stage = 0; stage < 40; stage++) {
        kfScheduler_addTask(sch, marked_task((kfStageId)stage, (long)stage), 1, 0);
        total_done += kfScheduler_tick(sch, &rec);
    }
    AssertEq(total_done, (size_t)40,
        "every one of the 40 growth ticks should execute exactly one task");
    AssertEq(rec.len, 40, "all 40 tasks across growing stage ids should have run");
    kfScheduler_destroy(sch);
}

Test(staging, stage_order_after_reschedule)
{
    kfScheduler *sch = kfScheduler_create(4);
    StageOrder rec = { { 0 }, 0 };

    kfScheduler_addTask(sch, marked_task(5, 5), 1, 2);
    kfScheduler_addTask(sch, marked_task(1, 1), 3, 0);

    kfScheduler_tick(sch, &rec);
    AssertEq(rec.len, 1, "only the stage-5 task is due on tick 1");
    AssertEq(rec.order[0], 5, "stage 5 ran alone on tick 1");

    kfScheduler_tick(sch, &rec);
    AssertEq(rec.len, 1, "nothing new is due on tick 2");

    kfScheduler_tick(sch, &rec);
    AssertEq(rec.len, 3, "tick 3 reruns stage 5 (rescheduled) and stage 1, in stage order");
    AssertEq(rec.order[1], 1, "stage 1 must run before the rescheduled stage 5");
    AssertEq(rec.order[2], 5, "the rescheduled stage-5 task runs after stage 1");
    kfScheduler_destroy(sch);
}
