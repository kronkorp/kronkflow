<a id="readme-top"></a>

<!-- PROJECT SHIELDS -->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]



<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/kronkorp">
    <img src="images/kronkcorp-k.svg" alt="Kronk Corp logo" width="200" align="middle">
  </a>
  &emsp;
  <img src="images/separator.svg" alt="" width="1" height="170" align="middle">
  &emsp;
  <a href="https://github.com/kronkorp/kronkflow">
    <img src="images/logo.png" alt="Kronkflow logo" width="200" align="middle">
  </a>

<h3 align="center">Kronkflow</h3>

  <p align="center">
    A lightweight and efficient task scheduler library for C
    <br />
    <a href="https://github.com/kronkorp/kronkflow#usage"><strong>Explore the usage »</strong></a>
    <br />
    <br />
    <a href="https://github.com/kronkorp/kronkflow/issues/new?labels=bug&template=bug-report---.md">Report Bug</a>
    &middot;
    <a href="https://github.com/kronkorp/kronkflow/issues/new?labels=enhancement&template=feature-request---.md">Request Feature</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li>
      <a href="#usage">Usage</a>
      <ul>
        <li><a href="#api-overview">API Overview</a></li>
      </ul>
    </li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

Kronkflow is a lightweight, efficient task scheduler library for C applications. It implements a **min-heap binary tree** to manage task priorities, ensuring optimal resource usage and fast task lookups.

Key Features:

- **Efficient Scheduling**: Uses a min-heap to keep the next upcoming task at the top.

- **One-time & Periodic Tasks**: Support for both punctual and recurring tasks.

- **Staged Execution**: Each tick, ready tasks are executed stage by stage, whatever the insertion order.

- **Lightweight**: Zero-dependency library, easy to integrate.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Built With

* [![C][C_]][C-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

To get a local copy up and running, follow these steps.

### Prerequisites

CMake and a C compiler:
```sh
sudo apt install cmake build-essential
```

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/kronkorp/kronkflow.git
   cd kronkflow/
   ```
2. Build
   ```sh
   mkdir build
   cd build/
   cmake ..
   make
   ```
   This generates `libkronkflow.a` and `libkronkflow.so`.
3. Install the library
   ```sh
   sudo make install
   ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage


Here is a simple example of how to use Kronkflow:

```c
#include <kronkflow/scheduler.h>
#include <kronkflow/task.h>
#include <stdio.h>
#include <unistd.h>

// Your task handler
static kfBool my_handler(void *context, void *data)
{
    printf("Task executed with data: %ld\n", (long int)data);
    return kfTrue;
}

int main(void)
{
    // Create a scheduler with a capacity of 512 tasks
    kfScheduler *sch = kfScheduler_create(512);

    // Add a periodic task on stage 0 (runs in 2 ticks, then every 5 ticks)
    kfScheduler_addTask(sch,
        kfTask_opt(&my_handler, (void *)1, NULL, 0, (kfRWMasks){ 0, 0 }), 2, 5);

    // Add a punctual task on stage 1 (runs once in 10 ticks).
    // Each tick, tasks are sorted and executed stage by stage, so this
    // always runs after every stage 0 task, no matter the insertion order.
    kfScheduler_addTask(sch,
        kfTask_opt(&my_handler, (void *)3, NULL, 1, (kfRWMasks){ 0, 0 }), 10, 0);

    // Main loop
    while (1) {
        sleep(1); // Simulate time passing
        kfScheduler_tick(sch, NULL);
    }

    kfScheduler_destroy(sch);
    return 0;
}
```

### API Overview

#### Scheduler Management
- `kfScheduler_create(size_t size)`: Allocate and initialize a new scheduler.
- `kfScheduler_destroy(kfScheduler *sch)`: Free the scheduler and its resources.
- `kfScheduler_tick(kfScheduler *sch, void *context)`: Advance the scheduler by one tick and execute ready tasks, stage by stage.

#### Task Management
- `kfTask_opt(kfHandler handler, void *data, kfClearer clearer, kfStageId stage, kfRWMasks masks)`: Build a task opt structure.
- `kfScheduler_addTask(kfScheduler *sch, kfTaskOpt opt, kfTick delay, kfTick interval)`: Register a task in the scheduler.

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feat/amazing-feature dev`)
3. Commit your Changes (`git commit -m '[ADD] Add some amazing feature'`)
4. Push to the Branch (`git push origin feat/amazing-feature`)
5. Open a Pull Request against `dev`

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Top contributors:

<a href="https://github.com/kronkorp/kronkflow/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=kronkorp/kronkflow" alt="contrib.rocks image" />
</a>



<!-- LICENSE -->
## License

Distributed under the MIT license. See `LICENSE` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Gauthier Malfilatre - [@GauthierMalfilatre](https://www.linkedin.com/in/gauthier-malfilatre-648001339/) - gauthier.malfilatre@epitech.eu

Project Link: [https://github.com/kronkorp/kronkflow](https://github.com/kronkorp/kronkflow)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- MARKDOWN LINKS & IMAGES -->
[contributors-shield]: https://img.shields.io/github/contributors/kronkorp/kronkflow.svg?style=for-the-badge
[contributors-url]: https://github.com/kronkorp/kronkflow/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/kronkorp/kronkflow.svg?style=for-the-badge
[forks-url]: https://github.com/kronkorp/kronkflow/network/members
[stars-shield]: https://img.shields.io/github/stars/kronkorp/kronkflow.svg?style=for-the-badge
[stars-url]: https://github.com/kronkorp/kronkflow/stargazers
[issues-shield]: https://img.shields.io/github/issues/kronkorp/kronkflow.svg?style=for-the-badge
[issues-url]: https://github.com/kronkorp/kronkflow/issues
[license-shield]: https://img.shields.io/github/license/kronkorp/kronkflow.svg?style=for-the-badge
[license-url]: https://github.com/kronkorp/kronkflow/blob/main/LICENSE
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://www.linkedin.com/in/gauthier-malfilatre-648001339/
[C_]: https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white
[C-url]: https://www.c-language.org
