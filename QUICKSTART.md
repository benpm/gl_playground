# WebGPU/WebAssembly Migration - Quick Start

## What Was Added

This PR adds the infrastructure needed to build GL Playground for the web using WebGPU and WebAssembly.

### New Files

1. **AGENTS.md** - Complete project documentation including:
   - Project overview and architecture
   - Technology stack details
   - Directory structure explanation
   - WebGPU migration plan

2. **WEBGPU_BUILD.md** - Comprehensive build guide for WebAssembly:
   - Prerequisites and setup instructions
   - Build commands and configuration
   - Deployment options
   - Troubleshooting guide

3. **Build Infrastructure**:
   - `.github/workflows/build-wasm.yml` - Automated WebAssembly builds and GitHub Pages deployment
   - `.github/workflows/build-native.yml` - CI for Linux and Windows native builds
   - `build_wasm.sh` - Local build script for WebAssembly
   - `cmake/emscripten.cmake` - Emscripten-specific CMake configuration
   - `shell.html` - Custom HTML template for web deployment

### Modified Files

- **README.md** - Added WebAssembly build instructions
- **.gitignore** - Added web build artifacts

## Quick Start Guide

### For Developers

```bash
# Install Emscripten (one-time setup)
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh

# Build for WebAssembly
cd /path/to/gl_playground
./build_wasm.sh

# Test locally
cd dist
python3 -m http.server 8000
# Open http://localhost:8000 in a WebGPU-compatible browser
```

### For CI/CD

The workflows will automatically:
- Build WebAssembly on every push/PR
- Deploy to GitHub Pages when pushing to main
- Build native versions for Linux/Windows
- Upload build artifacts

## Current Limitations

⚠️ **Important**: This PR sets up the build infrastructure but does NOT complete the full WebGPU migration.

### What Works
- ✅ Build system configured for Emscripten
- ✅ Automated CI/CD workflows
- ✅ Documentation and guides
- ✅ Local development workflow

### What Needs Work
- ⚠️ Graphics code still uses OpenGL (not WebGPU)
- ⚠️ Shaders are in GLSL (need to be ported to WGSL)
- ⚠️ Some OpenGL-specific features may not work in browser

## Next Steps for Full WebGPU Support

To complete the WebGPU migration, the following work is needed:

1. **Graphics API Port** (Major):
   - Replace OpenGL calls in `src/gfx.cpp`, `src/mesh.cpp`, `src/app.cpp`
   - Use WebGPU API instead of OpenGL
   - Update render pipeline creation

2. **Shader Conversion** (Major):
   - Convert all `.vert`/`.frag`/`.geom` shaders in `resources/shaders/` from GLSL to WGSL
   - Update shader compilation/loading code

3. **Rendering Pipeline** (Major):
   - Adapt to WebGPU's explicit state management
   - Update framebuffer and texture handling
   - Modify shadow mapping implementation

4. **Testing** (Medium):
   - Test all features in WebGPU browsers
   - Performance optimization for web
   - Cross-browser compatibility testing

## Testing the Workflows

The workflows will be tested automatically when this PR is merged or when changes are pushed. You can also manually trigger them from the Actions tab in GitHub.

## Resources

- [Emscripten Documentation](https://emscripten.org/)
- [WebGPU Specification](https://www.w3.org/TR/webgpu/)
- [Learn WebGPU](https://eliemichel.github.io/LearnWebGPU/)
- [WebGPU Samples](https://webgpu.github.io/webgpu-samples/)

## Browser Support

WebGPU is supported in:
- Chrome/Edge 113+
- Firefox 113+ (may require flag)
- Safari Technology Preview (with flag)

Check support: https://webgpureport.org/

## Questions?

See AGENTS.md for project architecture details or WEBGPU_BUILD.md for build instructions.
