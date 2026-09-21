/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Tests for task ids, kfScheduler_removeTask and same-tick ordering
*/
#include <kronklab/kronklab.h>
#include <kronkflow/macros/types.h>
#include <kronkflow/scheduler.h>
#include <kronkflow/task.h>
#include <stdint.h>

#define MAX_LOG 128

typedef struct {
    kfScheduler *sch;
    int          calls;
    int          cleared;
    int          order[MAX_LOG];
    int          len;
    kfTaskID     victim;    //!< Task removed by remove_victim
    kfTaskID     self;      //!< Task removed by remove_self
} Ctx;

typedef struct {
    int      idx;
    int      ran;
    kfTick   ran_at;
    Ctx     *ctx;
} Slot;

static kfBool count_call(void *context, void *data)
{
    (void)data;
    ((Ctx *)context)->calls++;
    return kfTrue;
}

// data = the Ctx, so that we can count how many times a clearer ran
static void count_clear(void *data)
{
    ((Ctx *)data)->cleared++;
}

static kfBool record_marker(void *context, void *data)
{
    Ctx *ctx = context;

    if (ctx->len < MAX_LOG) {
        ctx->order[ctx->len++] = (int)(intptr_t)data;
    }
    return kfTrue;
}

static kfBool remove_victim(void *context, void *data)
{
    Ctx *ctx = context;

    (void)data;
    kfScheduler_removeTask(ctx->sch, ctx->victim);
    return kfTrue;
}

static kfBool remove_self(void *context, void *data)
{
    Ctx *ctx = context;

    (void)data;
    ctx->calls++;
    kfScheduler_removeTask(ctx->sch, ctx->self);
    return kfTrue;
}

static kfBool record_slot(void *context, void *data)
{
    Slot *slot = data;

    (void)context;
    slot->ran++;
    slot->ran_at = kfScheduler_currentTick(slot->ctx->sch);
    return kfFalse;
}

static kfTaskOpt opt(kfHandler h, void *data, kfClearer c, kfStageId stage)
{
    return kfTask_opt(h, data, c, stage, (kfRWMasks){ 0, 0 });
}

Test(task_id, ids_are_per_scheduler)
{
    kfScheduler *a = kfScheduler_create(4);
    kfScheduler *b = kfScheduler_create(4);
    kfTaskOpt o = opt(&count_call, NULL, NULL, 0);

    AssertNotNull(a, "create a");
    AssertNotNull(b, "create b");
    AssertEq(kfScheduler_addTask(a, o, 1, 0), 1, "first id of a");
    AssertEq(kfScheduler_addTask(b, o, 1, 0), 1, "first id of b must not depend on a");
    AssertEq(kfScheduler_addTask(a, o, 1, 0), 2, "second id of a");
    AssertEq(kfScheduler_addTask(b, o, 1, 0), 2, "second id of b");
    kfScheduler_destroy(a);
    kfScheduler_destroy(b);
}

Test(task_id, periodic_task_keeps_its_id)
{
    kfScheduler *sch = kfScheduler_create(4);
    Ctx ctx = { .sch = sch };
    kfTaskID id = kfScheduler_addTask(sch, opt(&count_call, &ctx, &count_clear, 0), 1, 1);
    kfTaskID other;

    AssertNotNull(sch, "create");
    for (int i = 0; i < 5; ++i) {
        kfScheduler_tick(sch, &ctx);
    }
    AssertEq(ctx.calls, 5, "periodic task ran every tick");
    other = kfScheduler_addTask(sch, opt(&count_call, &ctx, NULL, 0), 1, 0);
    AssertEq(other, id + 1, "no id was consumed by the reprogramming (got %zu)", other);
    AssertEq(kfScheduler_removeTask(sch, id), kfTrue, "removable with its first id after 5 runs");
    AssertEq(ctx.cleared, 1, "clearer called once");
    kfScheduler_tick(sch, &ctx);
    AssertEq(ctx.calls, 5 + 1, "only the other task ran");
    kfScheduler_destroy(sch);
}

Test(task_id, remove_unknown_or_zero_id)
{
    kfScheduler *sch = kfScheduler_create(4);

    AssertNotNull(sch, "create");
    AssertEq(kfScheduler_removeTask(sch, 0), kfFalse, "id 0");
    AssertEq(kfScheduler_removeTask(sch, 42), kfFalse, "unknown id");
    AssertEq(kfScheduler_removeTask(NULL, 1), kfFalse, "NULL scheduler");
    kfScheduler_destroy(sch);
}

Test(task_id, remove_pending_task)
{
    kfScheduler *sch = kfScheduler_create(4);
    Ctx ctx = { .sch = sch };
    kfTaskID id = kfScheduler_addTask(sch, opt(&count_call, &ctx, &count_clear, 0), 5, 0);

    AssertNotNull(sch, "create");
    AssertEq(kfScheduler_removeTask(sch, id), kfTrue, "found in the heap");
    AssertEq(ctx.cleared, 1, "clearer called on removal");
    AssertEq(kfScheduler_removeTask(sch, id), kfFalse, "already removed");
    for (int i = 0; i < 10; ++i) {
        kfScheduler_tick(sch, &ctx);
    }
    AssertEq(ctx.calls, 0, "never ran");
    AssertEq(ctx.cleared, 1, "clearer not called twice");
    kfScheduler_destroy(sch);
}

Test(task_id, remove_finished_one_shot_task)
{
    kfScheduler *sch = kfScheduler_create(4);
    Ctx ctx = { .sch = sch };
    kfTaskID id = kfScheduler_addTask(sch, opt(&count_call, &ctx, &count_clear, 0), 1, 0);

    kfScheduler_tick(sch, &ctx);
    AssertEq(ctx.calls, 1, "ran once");
    AssertEq(ctx.cleared, 1, "cleared at the end of its run");
    AssertEq(kfScheduler_removeTask(sch, id), kfFalse, "nothing left to remove");
    AssertEq(ctx.cleared, 1, "clearer not called again");
    kfScheduler_destroy(sch);
}

Test(task_id, handler_removes_a_staged_task)
{
    kfScheduler *sch = kfScheduler_create(4);
    Ctx ctx = { .sch = sch };
    kfTaskID victim;

    AssertNotNull(sch, "create");
    // Same tick, stage 0 runs before stage 1: the victim is already staged
    kfScheduler_addTask(sch, opt(&remove_victim, NULL, NULL, 0), 1, 0);
    victim = kfScheduler_addTask(sch, opt(&count_call, &ctx, &count_clear, 1), 1, 0);
    ctx.victim = victim;
    kfScheduler_tick(sch, &ctx);
    AssertEq(ctx.calls, 0, "the victim did not run");
    AssertEq(ctx.cleared, 1, "the victim was cleared once");
    kfScheduler_destroy(sch);
}

Test(task_id, handler_removes_itself)
{
    kfScheduler *sch = kfScheduler_create(4);
    Ctx ctx = { .sch = sch };

    AssertNotNull(sch, "create");
    ctx.self = kfScheduler_addTask(sch, opt(&remove_self, &ctx, &count_clear, 0), 1, 1);
    for (int i = 0; i < 5; ++i) {
        kfScheduler_tick(sch, &ctx);
    }
    AssertEq(ctx.calls, 1, "ran once, was not reprogrammed");
    AssertEq(ctx.cleared, 1, "cleared once");
    kfScheduler_destroy(sch);
}

Test(task_id, registration_order)
{
    kfScheduler *sch = kfScheduler_create(2);
    Ctx ctx = { .sch = sch };

    AssertNotNull(sch, "create");
    // Registered 1..8, all due on the same tick and periodic
    for (long m = 1; m <= 8; ++m) {
        kfScheduler_addTask(sch, opt(&record_marker, (void *)(intptr_t)m, NULL, 0), 1, 1);
    }
    for (int t = 0; t < 3; ++t) {
        kfScheduler_tick(sch, &ctx);
    }
    AssertEq(ctx.len, 24, "8 tasks x 3 ticks");
    for (int i = 0; i < 24; ++i) {
        AssertEq(ctx.order[i], (i % 8) + 1, "order at %d is %d", i, ctx.order[i]);
    }
    kfScheduler_destroy(sch);
}

// NOTE: Removing in the middle of the heap moves the last task to a hole that
// can be above a later parent: only some layouts expose a broken heap, so this
// replays many pseudo-random ones.
Test(task_id, heap_valid_after_removals)
{
    enum { N = 40, MAX_DELAY = 30, TICKS = MAX_DELAY + 2, SEEDS = 300 };

    for (unsigned seed = 1; seed <= SEEDS; ++seed) {
        unsigned rng = seed * 2654435761u;
        kfScheduler *sch = kfScheduler_create(4);
        Ctx ctx = { .sch = sch };
        Slot slots[N];
        kfTaskID ids[N];
        kfTick delays[N];
        int removed[N];

        AssertNotNull(sch, "create");
        for (int i = 0; i < N; ++i) {
            rng = rng * 1664525u + 1013904223u;
            delays[i] = 1 + (rng >> 16) % MAX_DELAY;
            slots[i] = (Slot){ .idx = i, .ctx = &ctx };
            ids[i] = kfScheduler_addTask(sch, opt(&record_slot, &slots[i], NULL, 0), delays[i], 0);
        }
        for (int i = 0; i < N; ++i) {
            rng = rng * 1664525u + 1013904223u;
            removed[i] = ((rng >> 16) % 3) == 0;
            if (removed[i]) {
                AssertEq(kfScheduler_removeTask(sch, ids[i]), kfTrue, "seed %u: remove %d", seed, i);
            }
        }
        for (int t = 0; t < TICKS; ++t) {
            kfScheduler_tick(sch, &ctx);
        }
        for (int i = 0; i < N; ++i) {
            if (removed[i]) {
                AssertEq(slots[i].ran, 0, "seed %u: removed task %d must not run", seed, i);
            } else {
                AssertEq(slots[i].ran, 1, "seed %u: task %d must run exactly once", seed, i);
                AssertEq(slots[i].ran_at, delays[i], "seed %u: task %d ran at tick %lu instead of %lu",
                    seed, i, (unsigned long)slots[i].ran_at, (unsigned long)delays[i]);
            }
        }
        kfScheduler_destroy(sch);
    }
}
