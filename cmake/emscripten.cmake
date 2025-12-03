# Emscripten-specific CMake configuration for WebAssembly build

if(EMSCRIPTEN)
    message(STATUS "Configuring for Emscripten/WebAssembly build")
    
    # Set Emscripten-specific flags
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    
    # Emscripten compiler flags
    set(EMSCRIPTEN_COMPILE_FLAGS
        -sUSE_GLFW=3
        -sUSE_WEBGPU=1
        -sALLOW_MEMORY_GROWTH=1
        -sFULL_ES3=1
    )
    
    # Emscripten linker flags
    set(EMSCRIPTEN_LINK_FLAGS
        -sUSE_GLFW=3
        -sUSE_WEBGPU=1
        -sALLOW_MEMORY_GROWTH=1
        -sFULL_ES3=1
        -sWASM=1
        -sEXPORT_ALL=1
        -sEXPORTED_RUNTIME_METHODS=['ccall','cwrap']
        --preload-file ${CMAKE_SOURCE_DIR}/resources@/resources
        --shell-file ${CMAKE_SOURCE_DIR}/shell.html
    )
    
    # Add flags to target
    target_compile_options(app PRIVATE ${EMSCRIPTEN_COMPILE_FLAGS})
    target_link_options(app PRIVATE ${EMSCRIPTEN_LINK_FLAGS})
    
    # Override GLFW to use Emscripten's built-in version
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    
    message(STATUS "WebAssembly configuration complete")
endif()
