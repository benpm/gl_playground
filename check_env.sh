#!/bin/bash

# Environment check script for WebAssembly development
# Verifies that all necessary tools are installed

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}======================================${NC}"
echo -e "${BLUE}GL Playground - Environment Check${NC}"
echo -e "${BLUE}======================================${NC}"
echo ""

# Function to check command existence
check_command() {
    if command -v $1 &> /dev/null; then
        echo -e "${GREEN}✓${NC} $1 is installed"
        if [ ! -z "$2" ]; then
            echo -e "  Version: $($2)"
        fi
        return 0
    else
        echo -e "${RED}✗${NC} $1 is not installed"
        return 1
    fi
}

# Track overall status
all_ok=true

echo -e "${YELLOW}Checking build tools...${NC}"
check_command "cmake" "cmake --version | head -1" || all_ok=false
check_command "make" "make --version | head -1" || all_ok=false
check_command "git" "git --version" || all_ok=false
echo ""

echo -e "${YELLOW}Checking Emscripten...${NC}"
if check_command "emcc" "emcc --version | head -1"; then
    check_command "em++" "em++ --version | head -1" || all_ok=false
    check_command "emcmake" || all_ok=false
    check_command "emmake" || all_ok=false
    
    if [ ! -z "$EMSDK" ]; then
        echo -e "${GREEN}✓${NC} EMSDK environment variable set: $EMSDK"
    else
        echo -e "${YELLOW}⚠${NC} EMSDK environment variable not set"
        echo -e "  Run: source /path/to/emsdk/emsdk_env.sh"
    fi
else
    all_ok=false
    echo -e "${RED}✗${NC} Emscripten not found!"
    echo ""
    echo -e "${YELLOW}To install Emscripten:${NC}"
    echo "  git clone https://github.com/emscripten-core/emsdk.git"
    echo "  cd emsdk"
    echo "  ./emsdk install latest"
    echo "  ./emsdk activate latest"
    echo "  source ./emsdk_env.sh"
fi
echo ""

echo -e "${YELLOW}Checking optional tools...${NC}"
check_command "python3" "python3 --version"
check_command "node" "node --version"
check_command "ninja" "ninja --version"
echo ""

echo -e "${YELLOW}Checking project structure...${NC}"
if [ -d "resources" ]; then
    echo -e "${GREEN}✓${NC} resources/ directory exists"
else
    echo -e "${YELLOW}⚠${NC} resources/ directory not found"
    echo -e "  You may need to run: ./download_assets.sh"
fi

if [ -f "CMakeLists.txt" ]; then
    echo -e "${GREEN}✓${NC} CMakeLists.txt found"
else
    echo -e "${RED}✗${NC} CMakeLists.txt not found - are you in the project root?"
fi

if [ -f "build_wasm.sh" ]; then
    echo -e "${GREEN}✓${NC} build_wasm.sh script found"
else
    echo -e "${RED}✗${NC} build_wasm.sh script not found"
fi
echo ""

# Summary
echo -e "${BLUE}======================================${NC}"
if [ "$all_ok" = true ]; then
    echo -e "${GREEN}✓ All essential tools are installed!${NC}"
    echo ""
    echo -e "You're ready to build! Run:"
    echo -e "  ${BLUE}./build_wasm.sh${NC}"
else
    echo -e "${RED}✗ Some tools are missing${NC}"
    echo ""
    echo -e "Please install missing tools before building."
    echo -e "See ${BLUE}WEBGPU_BUILD.md${NC} for detailed instructions."
fi
echo -e "${BLUE}======================================${NC}"

exit $([ "$all_ok" = true ] && echo 0 || echo 1)
