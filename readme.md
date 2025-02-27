# Ray Tracing Simulation 

A simple ray tracing simulation implemented in C using SDL3. This project demonstrates dynamic ray casting from a movable light source, featuring an improved shadow rendering with a smooth radial gradient and additional visual enhancements.

## Features

- **Dynamic Ray Casting:** Rays are emitted from a light source that follows mouse movement.
- **Improved Shadow Rendering:** A moving obstacle (shadow) is rendered with a soft, radial gradient.
- **Interactive Visualization:** The light source moves in response to mouse input, and the shadow dynamically changes position.
- **Visual Finishing Touches:** An outline is drawn around the light source for added visual definition.

## Dependencies

- **SDL3:** [Simple DirectMedia Layer 3](https://www.libsdl.org/) for window management, rendering, and event handling.
- **C Standard Library:** Including headers such as `stdio.h` and `math.h`.

## Installation
- **Clone the Repository:** `git clone
- **Build the Project:** `make`
- **Run the Executable:** `./main`
if make is not installed, you can compile the project using the following command:
```g++ -I src/include -L src/lib -o main raytracing.c -lmingw32 -lSDL3```

## Author
- **[Saugat Pokhrel](https://github.com/saugatpokhrel)** 

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

