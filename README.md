# Kronkflow

Kronkflow is a lightweight, efficient task scheduler library for C applications. It implements a **min-heap binary tree** to manage task priorities, ensuring optimal resource usage and fast task lookups.

## Features

- **Efficient Scheduling**: Uses a min-heap to keep the next upcoming task at the top.
- **One-time & Periodic Tasks**: Support for both punctual and recurring tasks.
- **Lightweight**: Zero-dependency library, easy to integrate.

## Installation

### Using CMake

```bash
mkdir build
cd build/
cmake ..
make
```

This will generate `libkronkflow.a` and `libkronkflow.so`. You can now install with

```bash
sudo make install
```

## Quick Start

Here is a simple example of how to use Kronkflow:

```c
#include <kronkflow.h>
#include <stdio.h>
#include <unistd.h>

// Your task handler
static bool my_handler(void *context, void *data)
{
    printf("Task executed with data: %ld\n", (long int)data);
}

int main(void)
{
    // Create a scheduler with a capacity of 512 tasks
    kfScheduler *sch = kfScheduler_create(512);

    // Add a periodic task (runs in 2 ticks, then every 5 ticks)
    kfScheduler_addTask(sch, kfTask_opt(&my_handler, (void *)1, NULL) 2, 5);

    // Add a punctual task (runs once in 10 ticks)
    kfScheduler_addTask(sch, kfTask_opt(&my_handler, (void *)3, NULL), 10, 0);

    // Main loop
    while (1) {
        sleep(1); // Simulate time passing
        kfScheduler_tick(sch, NULL);
    }

    kfScheduler_destroy(sch);
    return true;
}
```

## API Overview

### Scheduler Management
- `kfScheduler_create(size_t size)`: Allocate and initialize a new scheduler.
- `kfScheduler_destroy(kfScheduler *sch)`: Free the scheduler and its resources.
- `kfScheduler_tick(kfScheduler *sch, void *context)`: Advance the scheduler by one tick and execute ready tasks.

### Task Management
- `kfTask_opt(kfHandler handler, void *data, kfClearer clearer)`: Build a task opt structure.
- `kfScheduler_addTask(kfScheduler *sch, kfTask task, kfTick delay, kfTick interval)`: Register a task in the scheduler.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
