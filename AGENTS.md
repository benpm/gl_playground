# GL Playground - Project Overview

## Project Description

GL Playground is an OpenGL-based 3D graphics application developed for Interactive Computer Graphics, Physics-Based Animation, and Computational Geometry courses. It serves as a learning platform and demonstration of various computer graphics techniques and physics simulations.

## Current Architecture

### Technology Stack
- **Graphics API**: OpenGL (native desktop)
- **Language**: C++20
- **Build System**: CMake
- **Window Management**: GLFW 3.3.8
- **Math Library**: Eigen
- **UI**: ImGui
- **Logging**: spdlog
- **Entity Component System**: EnTT

### Key Features
1. **Lighting System**
   - Directional lights
   - Spot lights
   - Point lights
   - Basic shadow-mapping support

2. **Physics Simulation**
   - Interactive rigid body physics
   - Soft-body physics solver

3. **Rendering**
   - Multi-material objects
   - Basic shading models
   - Surface nets generated in compute shaders
   - Wireframe rendering
   - Shadow mapping

4. **Model Support**
   - OBJ file loading (using cyTriMesh)
   - Multiple pre-loaded models (teapot, suzanne, cube, sphere, quad)

## Project Structure

```
gl_playground/
├── src/                    # Source files
│   ├── main.cpp           # Entry point, initialization, main loop
│   ├── app.cpp            # Main application class and render loop
│   ├── camera.cpp         # Camera system
│   ├── gfx.cpp            # Graphics utilities (texture generation)
│   ├── light.cpp          # Light entity management
│   ├── mesh.cpp           # Mesh rendering and geometry
│   ├── model.cpp          # Model loading utilities
│   ├── physics.cpp        # Physics simulation
│   ├── texture.cpp        # Texture loading from PNG files
│   └── extmath.cpp        # Extended math utilities
│
├── include/               # Header files (mirrors src/ structure)
│   ├── app.hpp
│   ├── camera.hpp
│   ├── gfx.hpp
│   ├── light.hpp
│   ├── mesh.hpp
│   ├── model.hpp
│   ├── physics.hpp
│   ├── texture.hpp
│   └── extmath.hpp
│
├── lib/                   # Third-party libraries (included in repo)
│   ├── include/
│   │   ├── glad/          # OpenGL loader
│   │   ├── KHR/           # Khronos headers
│   │   ├── cyCore.h       # cyCodeBase core
│   │   ├── cyVector.h     # cyCodeBase vectors
│   │   ├── cyTriMesh.h    # cyCodeBase mesh utilities
│   │   ├── cyGL.h         # cyCodeBase OpenGL utilities
│   │   └── gleq.h         # GLFW event queue
│   └── src/
│       └── glad.c         # OpenGL loader implementation
│
├── cmake/                 # CMake dependency configurations
│   ├── eigen.cmake.in     # Eigen linear algebra library
│   ├── glfw.cmake.in      # GLFW windowing library
│   ├── spdlog.cmake.in    # Logging library
│   ├── imgui.cmake.in     # ImGui UI library
│   ├── entt.cmake.in      # Entity component system
│   ├── libspng.cmake.in   # PNG image loading
│   └── cxxopts.cmake.in   # Command-line option parsing
│
├── resources/             # Runtime assets
│   ├── models/            # 3D model files (.obj, .mtl)
│   │   ├── teapot.obj
│   │   ├── suzanne.obj
│   │   ├── cube.obj
│   │   ├── sphere.obj
│   │   └── quad.obj
│   └── shaders/           # GLSL shader files
│       ├── basic.vert     # Basic vertex shader
│       ├── basic.frag     # Basic fragment shader
│       ├── wireframe.*    # Wireframe rendering shaders
│       ├── wires.*        # Wire rendering shaders
│       ├── sky.*          # Skybox shaders
│       └── shadow.frag    # Shadow mapping shader
│
├── .vscode/               # VS Code configuration
│   ├── settings.json
│   ├── tasks.json
│   └── launch.json
│
├── CMakeLists.txt         # Main CMake build configuration
├── README.md              # User documentation
├── .gitignore            # Git ignore rules
├── download_assets.sh    # Script to download external assets
├── package_assets.sh     # Script to package assets
└── upload_assets.sh      # Script to upload assets
```

## Build Process (Current)

### Dependencies
The project uses CMake's FetchContent to automatically download and build:
- GLFW (window management)
- Eigen (linear algebra)
- spdlog (logging)
- ImGui (UI)
- EnTT (entity component system)
- libspng (PNG loading)
- cxxopts (CLI parsing)

### Platform Support (Current)
- **Linux**: Primary development platform
  - Requires: libxinerama-dev, libglu1-mesa-dev, libxi-dev, ninja-build
- **Windows**: CMake project support
  - Recommended: Ninja build system

### Build Commands
```bash
cmake -B build
cmake --build build
./build/app
```

## WebGPU/WebASM Migration Plan

To make this project work on the web, the following changes are needed:

### 1. Graphics API Migration
- Replace OpenGL calls with WebGPU API
- Port shaders from GLSL to WGSL (WebGPU Shading Language)
- Adapt rendering pipeline to WebGPU's explicit state management

### 2. Build System
- Add Emscripten toolchain support
- Configure CMake for WebAssembly compilation
- Set up asset embedding for web deployment

### 3. Platform Abstraction
- Abstract windowing (GLFW works with Emscripten but needs configuration)
- Handle input differences between native and web
- Manage resource loading (async for web)

### 4. Dependencies
All major dependencies have WebAssembly support:
- GLFW: Has Emscripten backend
- Eigen: Header-only, works with Emscripten
- ImGui: Has WebGPU backend available
- EnTT: Header-only, works with Emscripten

### 5. Deployment
- Generate HTML5 application
- Host assets appropriately
- Set up GitHub Pages or similar for hosting

## Development Notes

### Code Style
- C++20 features used throughout
- Smart pointers for resource management
- Entity-Component-System architecture via EnTT
- GLFW for cross-platform windowing

### Key Classes
- `App`: Main application class, manages render loop and state
- `Camera`: Camera system with view/projection matrices
- `Mesh`: Geometry and rendering data
- `Model`: Model loading and management
- Various physics-related structures in physics.hpp

### Logging
Uses spdlog with custom error handling that raises SIGINT on error messages.

### Asset Management
External assets are hosted separately and downloaded via script (from cs.utah.edu/~benpm/assets.zip).
