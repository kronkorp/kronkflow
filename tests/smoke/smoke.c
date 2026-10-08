/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Smoke test, with no test framework: runs on every platform.
*/
#include "kronkflow/scheduler.h"
#include "kronkflow/task.h"
#include <stdint.h>
#include <stdio.h>

#define ONE_SHOTS 100
#define STAGES    5
#define TICKS     10

typedef struct {
    int       runs[ONE_SHOTS];
    int       periodic;
    kfStageId lastStage;
    kfBool    inOrder;
} Record;

static kfBool oneShot(void *context, void *data)
{
    Record *rec = context;
    int i = (int)(intptr_t)data;
    kfStageId stage = (kfStageId)(i % STAGES);

    ++rec->runs[i];
    if (stage < rec->lastStage) {
        rec->inOrder = kfFalse;
    }
    rec->lastStage = stage;
    return kfFalse;
}

static kfBool periodic(void *context, void *data)
{
    (void)data;
    ++((Record *)context)->periodic;
    return kfTrue;
}

static int check(int ok, const char *what)
{
    printf("%s: %s\n", ok ? "ok" : "FAILED", what);
    return ok ? 0 : 1;
}

// NOTE: Room for one task at first, and five stages: the task array and the
//       stages both have to grow
int main(void)
{
    kfScheduler *sch = kfScheduler_create(1);
    Record rec = { .inOrder = kfTrue };
    size_t done = 0;
    int once = 1;
    int failed = 0;

    failed += check(sch != NULL, "a scheduler");
    if (!sch) {
        return 1;
    }
    for (int i = 0; i < ONE_SHOTS; ++i) {
        kfTaskOpt opt = kfTask_opt(&oneShot, (void *)(intptr_t)i, NULL, (kfStageId)(i % STAGES), (kfRWMasks){ 0, 0 });

        failed += kfScheduler_addTask(sch, opt, 1 + i / (ONE_SHOTS / TICKS), 0) == 0;
    }
    failed += kfScheduler_addTask(sch, kfTask_opt(&periodic, NULL, NULL, 0, (kfRWMasks){ 0, 0 }), 2, 2) == 0;
    for (int tick = 0; tick < TICKS; ++tick) {
        rec.lastStage = 0;
        done += kfScheduler_tick(sch, &rec);
    }
    for (int i = 0; i < ONE_SHOTS; ++i) {
        once = once && rec.runs[i] == 1;
    }
    failed += check(once, "every one-shot task ran once");
    failed += check(rec.inOrder, "stage by stage");
    failed += check(rec.periodic == TICKS / 2, "the periodic one ran every other tick");
    failed += check(done == ONE_SHOTS + TICKS / 2, "and the ticks counted all of them");
    failed += check(kfScheduler_currentTick(sch) == TICKS, "the tick count");
    kfScheduler_destroy(sch);

    printf("%s\n", failed ? "smoke test FAILED" : "smoke test passed");
    return failed ? 1 : 0;
}
