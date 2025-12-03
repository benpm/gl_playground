# WebGPU/WebAssembly Build Guide

This document explains how to build and deploy GL Playground for the web using WebGPU and WebAssembly.

## Overview

The GL Playground project has been configured to support WebAssembly builds using Emscripten. This allows the application to run in modern web browsers that support WebGPU.

## Prerequisites

### 1. Install Emscripten SDK

```bash
# Clone the Emscripten SDK
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

# Install and activate the latest version
./emsdk install latest
./emsdk activate latest

# Set up environment variables (add to your .bashrc or .zshrc)
source ./emsdk_env.sh
```

### 2. Verify Installation

```bash
emcc --version
```

You should see the Emscripten compiler version information.

## Building for WebAssembly

### Quick Build (Recommended)

Use the provided build script:

```bash
./build_wasm.sh
```

This will:
- Configure the project for WebAssembly
- Build the application
- Create a `dist/` directory with all necessary files

### Manual Build

If you prefer to build manually:

```bash
# Create build directory
mkdir -p build-wasm
cd build-wasm

# Configure with Emscripten
emcmake cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
emmake make -j$(nproc)

# The output will be in build-wasm/app.html, app.js, app.wasm
```

## Testing Locally

After building, you need to serve the files with a local web server (due to CORS restrictions):

### Using Python (Recommended)

```bash
cd dist
python3 -m http.server 8000
```

Then open http://localhost:8000 in your browser.

### Using Node.js

```bash
npx http-server dist -p 8000
```

### Using PHP

```bash
cd dist
php -S localhost:8000
```

## Browser Requirements

The WebGPU build requires a modern browser with WebGPU support:

- **Chrome/Edge**: Version 113+ (WebGPU enabled by default)
- **Firefox**: Version 113+ (may need to enable in about:config)
- **Safari**: Technology Preview with WebGPU flag enabled

To check WebGPU support, visit: https://webgpureport.org/

## GitHub Actions CI/CD

The project includes automated workflows:

### build-wasm.yml
- Automatically builds WebAssembly on push/PR
- Uploads build artifacts
- Deploys to GitHub Pages on main branch

### build-native.yml
- Builds native versions for Linux and Windows
- Ensures cross-platform compatibility

## Deployment

### GitHub Pages (Automatic)

When you push to the `main` branch, the build-wasm workflow will automatically:
1. Build the WebAssembly version
2. Deploy to the `gh-pages` branch
3. Make it available at: `https://<username>.github.io/gl_playground/`

### Manual Deployment

To deploy to your own server:

1. Build the project: `./build_wasm.sh`
2. Copy everything from `dist/` to your web server
3. Ensure your server serves .wasm files with the correct MIME type:
   ```
   application/wasm
   ```

## Configuration

### Emscripten Settings

The build is configured with these Emscripten flags (in `cmake/emscripten.cmake`):

- `USE_GLFW=3`: Use Emscripten's GLFW implementation
- `USE_WEBGPU=1`: Enable WebGPU support
- `ALLOW_MEMORY_GROWTH=1`: Allow dynamic memory allocation
- `FULL_ES3=1`: Enable full OpenGL ES 3.0 support
- `WASM=1`: Generate WebAssembly (not asm.js)

### Custom Shell

The `shell.html` file provides the HTML wrapper for the application with:
- Responsive canvas sizing
- Loading progress indicator
- Controls information
- Custom styling

## Troubleshooting

### Build Issues

**Problem**: "Emscripten not found"
**Solution**: Make sure you've sourced the Emscripten environment:
```bash
source /path/to/emsdk/emsdk_env.sh
```

**Problem**: "resources directory not found"
**Solution**: Download the assets first:
```bash
./download_assets.sh
```

### Runtime Issues

**Problem**: "WebGPU not supported"
**Solution**: Use a browser with WebGPU support or enable it in browser settings.

**Problem**: "Failed to load app.data"
**Solution**: Make sure you're serving the files through a web server, not opening the HTML file directly.

**Problem**: "SharedArrayBuffer is not defined"
**Solution**: Your server needs to send these headers:
```
Cross-Origin-Opener-Policy: same-origin
Cross-Origin-Embedder-Policy: require-corp
```

## Performance Considerations

### Optimization Levels

The build uses `-O3` optimization by default. For debugging, you can change to `-O0` or `-g` in the CMake configuration.

### File Size

The WebAssembly build will be larger than native builds due to:
- Included runtime libraries
- Embedded resource files
- Debug symbols (if enabled)

To reduce file size:
- Remove unused assets from resources/
- Use Release build type
- Enable compression on your web server (gzip/brotli)

## Migration Status

### Current Status
- ✅ Build system configured for Emscripten
- ✅ GitHub Actions workflows created
- ✅ Web shell template created
- ✅ Documentation written
- ⚠️ Graphics API still uses OpenGL (needs WebGPU port)
- ⚠️ Shaders need to be ported to WGSL
- ⚠️ Some OpenGL-specific features may not work

### Next Steps

To complete the WebGPU migration:

1. **Port Graphics API**: Replace OpenGL calls with WebGPU API
2. **Convert Shaders**: Port GLSL shaders to WGSL format
3. **Update Rendering Pipeline**: Adapt to WebGPU's explicit state management
4. **Test and Optimize**: Ensure performance is acceptable

## Resources

- [Emscripten Documentation](https://emscripten.org/docs/index.html)
- [WebGPU Specification](https://www.w3.org/TR/webgpu/)
- [WGSL Specification](https://www.w3.org/TR/WGSL/)
- [WebGPU Samples](https://webgpu.github.io/webgpu-samples/)

## Contributing

When contributing WebAssembly-related changes:

1. Test both native and WebAssembly builds
2. Ensure the workflows pass
3. Update documentation if needed
4. Consider browser compatibility
