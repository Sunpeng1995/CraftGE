# CraftGE

An experimental computer graphics rendering engine based on OpenGL.

## Prerequisites

The things you need to install before building this project.

- CMake 3.0 or later

## Dependencies

- [GLFW](https://github.com/glfw/glfw) - a multi-platform library for OpenGL application development
- [Assimp](https://github.com/assimp/assimp) - a library to import and export various 3d-model-formats

Befor building the project, you need to get dependencies first:

### Windows (MSVC)

You can get dependencies in Windows by following ways:

Use [Microsoft VC++ Packaging Tool](https://github.com/Microsoft/vcpkg) (Recommand)

```PowerShell
PS > vcpkg install glfw3 assimp
PS > vcpkg integrate install
```

After that, the libraries will be handled automatically.

**Or**, build dependencies manually, then copy .lib and .dll to the lib directory.

### macOS & Linux

Manually build dependencies, then install them or copy .so files to lib directory.

## Build

Use CMake to generate project and build:

```bash
cd CraftGE
mkdir build && cd build
cmake ..
cmake --build .
```

## Roadmap

### Basic Modules

- Lights
  - [x] Point lights
  - [x] Directional lights
  - [x] Spot lights
- Transforms
  - [x] Basic object transforms
  - [x] Bones system
  - [ ] Animation Parsing
- Camera
  - [x] Free observing Camera
  - [ ] FPS Camera
  - [ ] Racing Camera
- Texture mappings
  - [x] Diffuse texture
  - [x] Normal texture
  - [x] Skybox via cube texture
  - [ ] LightMap baking
  - [ ] Texture resource management
- Shadow
  - [x] Basic shadow mapping (directional & point)
  - [x] Percentage closer soft shadow
  - [ ] Expotional soft shadow mapping
  - [ ] Screen-space shadow mapping
- Physics
  - [ ] Collision detection
  - [ ] Force
- Shader
  - [x] Shader resource management
  - [ ] Build-in default shaders
- [x] Model importer using third party library
- [x] Deferred rendering

### Advanced Modules

- [ ] BRDF distribution via StandardMaterial
- [ ] Global illumination (offline & simplified in real-time)
- [ ] Water simulation
- [ ] Rain, snow and fog simulation

## Acknowledge

OpenGL context and window management powered by [GLFW](https://github.com/glfw/glfw), Model importer powered by [Assimp](https://github.com/assimp/assimp).

Some modules and architecture design enlightened by [CanvasToy](https://github.com/Danielhu229/CanvasToy).