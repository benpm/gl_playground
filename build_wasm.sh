#!/bin/bash

# Build script for WebAssembly using Emscripten
# This script helps build the GL Playground project for the web

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}GL Playground WebAssembly Build Script${NC}"
echo "========================================"

# Check if Emscripten is installed
if ! command -v emcc &> /dev/null; then
    echo -e "${RED}Error: Emscripten not found!${NC}"
    echo "Please install Emscripten SDK:"
    echo "  git clone https://github.com/emscripten-core/emsdk.git"
    echo "  cd emsdk"
    echo "  ./emsdk install latest"
    echo "  ./emsdk activate latest"
    echo "  source ./emsdk_env.sh"
    exit 1
fi

echo -e "${GREEN}✓ Emscripten found${NC}"
emcc --version | head -1

# Check if resources directory exists
if [ ! -d "resources" ]; then
    echo -e "${YELLOW}Warning: resources directory not found${NC}"
    echo "You may need to run ./download_assets.sh first"
fi

# Clean previous build
if [ -d "build-wasm" ]; then
    echo -e "${YELLOW}Cleaning previous build...${NC}"
    rm -rf build-wasm
fi

# Create build directory
mkdir -p build-wasm
cd build-wasm

echo -e "${GREEN}Configuring CMake for WebAssembly...${NC}"
emcmake cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=${EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake

echo -e "${GREEN}Building WebAssembly...${NC}"
emmake make -j$(nproc)

# Create distribution directory
cd ..
mkdir -p dist

# Copy build artifacts
echo -e "${GREEN}Preparing distribution...${NC}"
if [ -f "build-wasm/app.html" ]; then
    cp build-wasm/app.html dist/index.html
    cp build-wasm/app.js dist/
    cp build-wasm/app.wasm dist/
    cp build-wasm/app.data dist/ 2>/dev/null || true
    echo -e "${GREEN}✓ Build successful!${NC}"
    echo ""
    echo "Output files in ./dist/"
    echo "To test locally, run a web server in the dist directory:"
    echo "  cd dist && python3 -m http.server 8000"
    echo "Then open http://localhost:8000 in your browser"
else
    echo -e "${RED}Error: Build failed - app.html not found${NC}"
    exit 1
fi
