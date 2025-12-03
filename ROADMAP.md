# WebGPU Migration Roadmap

This document outlines the complete roadmap for migrating GL Playground from OpenGL to WebGPU.

## Phase 1: Infrastructure Setup ✅ COMPLETE

**Goal**: Set up build system and workflows for WebAssembly

- [x] Create project documentation (AGENTS.md)
- [x] Set up Emscripten build configuration
- [x] Create GitHub Actions workflows for CI/CD
- [x] Write build scripts and documentation
- [x] Create web deployment HTML template
- [x] Update repository documentation

**Deliverables**: 
- Build system that can compile to WebAssembly (using Emscripten)
- Automated workflows for native and web builds
- Comprehensive documentation for developers

## Phase 2: Graphics API Port 🔄 IN PROGRESS

**Goal**: Replace OpenGL calls with WebGPU equivalents

### 2.1 Create WebGPU Abstraction Layer
- [ ] Design abstraction interface for graphics operations
- [ ] Implement WebGPU backend
- [ ] Implement OpenGL backend (for backward compatibility)
- [ ] Add compile-time selection between backends

### 2.2 Port Core Rendering
- [ ] Replace `glad` with WebGPU API
- [ ] Port framebuffer operations (`src/gfx.cpp`)
- [ ] Port texture creation and management
- [ ] Update render target system

### 2.3 Port Mesh Rendering
- [ ] Convert vertex buffer creation (`src/mesh.cpp`)
- [ ] Update index buffer handling
- [ ] Port draw calls to WebGPU render passes
- [ ] Implement vertex attribute layout in WebGPU

### 2.4 Port Application Rendering
- [ ] Update main render loop (`src/app.cpp`)
- [ ] Port shadow mapping to WebGPU
- [ ] Update camera/view/projection handling
- [ ] Implement multiple render passes

**Estimated Effort**: 4-6 weeks

## Phase 3: Shader Conversion 🔜 NEXT

**Goal**: Convert all GLSL shaders to WGSL

### 3.1 Basic Shaders
- [ ] Convert `basic.vert` to WGSL
- [ ] Convert `basic.frag` to WGSL
- [ ] Test basic rendering pipeline

### 3.2 Advanced Shaders
- [ ] Convert wireframe shaders (`.vert`, `.frag`, `.tesc`, `.geom`)
- [ ] Convert wire shaders
- [ ] Convert sky shaders
- [ ] Convert shadow shaders

### 3.3 Shader System
- [ ] Create WGSL shader loader
- [ ] Implement shader compilation/validation
- [ ] Add error handling and reporting
- [ ] Support shader hot-reloading (development)

**Estimated Effort**: 2-3 weeks

**Notes**: 
- Geometry shaders don't exist in WebGPU - need alternative approach
- Tessellation shaders have different implementation in WebGPU

## Phase 4: Platform Abstraction ⏳ PLANNED

**Goal**: Abstract platform-specific code for web compatibility

### 4.1 Window Management
- [ ] Test GLFW with Emscripten backend
- [ ] Handle canvas resize events
- [ ] Implement fullscreen toggle for web

### 4.2 Input Handling
- [ ] Verify mouse input works in browser
- [ ] Test keyboard input
- [ ] Handle touch input for mobile browsers
- [ ] Add gamepad support (optional)

### 4.3 Resource Loading
- [ ] Make asset loading asynchronous for web
- [ ] Implement loading progress indicators
- [ ] Handle preloaded resources (Emscripten file system)
- [ ] Optimize asset bundle size

### 4.4 Performance
- [ ] Profile WebAssembly performance
- [ ] Optimize hot paths
- [ ] Reduce memory usage
- [ ] Implement LOD system if needed

**Estimated Effort**: 2-3 weeks

## Phase 5: Feature Parity ⏳ PLANNED

**Goal**: Ensure all features work in WebGPU version

### 5.1 Lighting System
- [ ] Test directional lights
- [ ] Test spot lights
- [ ] Test point lights
- [ ] Verify shadow mapping works

### 5.2 Physics
- [ ] Test rigid body physics
- [ ] Test soft-body solver
- [ ] Optimize for web performance

### 5.3 UI
- [ ] Verify ImGui works with Emscripten
- [ ] Test all UI controls
- [ ] Ensure responsive layout

### 5.4 Materials & Textures
- [ ] Test multi-material objects
- [ ] Verify texture loading
- [ ] Test normal/displacement maps

**Estimated Effort**: 2-3 weeks

## Phase 6: Testing & Polish ⏳ PLANNED

**Goal**: Comprehensive testing and optimization

### 6.1 Cross-Browser Testing
- [ ] Test in Chrome/Edge (primary target)
- [ ] Test in Firefox
- [ ] Test in Safari (if WebGPU available)
- [ ] Document browser-specific issues

### 6.2 Performance Optimization
- [ ] Profile and optimize rendering
- [ ] Reduce WebAssembly bundle size
- [ ] Optimize shader compilation
- [ ] Implement progressive loading

### 6.3 User Experience
- [ ] Add loading screen
- [ ] Show progress during initialization
- [ ] Handle errors gracefully
- [ ] Add browser compatibility warnings

### 6.4 Documentation
- [ ] Update all documentation
- [ ] Create usage examples
- [ ] Add troubleshooting guide
- [ ] Create demo videos/screenshots

**Estimated Effort**: 1-2 weeks

## Phase 7: Deployment ⏳ PLANNED

**Goal**: Deploy and maintain web version

- [ ] Set up GitHub Pages hosting
- [ ] Configure custom domain (optional)
- [ ] Set up analytics (optional)
- [ ] Create landing page
- [ ] Announce release

**Estimated Effort**: 1 week

## Total Estimated Timeline

**12-18 weeks** for complete migration

## Known Challenges

### Technical Challenges

1. **Geometry Shaders**: WebGPU doesn't support geometry shaders
   - **Solution**: Implement wireframe using alternative technique (e.g., barycentric coordinates, compute shaders)

2. **Tessellation**: Different implementation in WebGPU
   - **Solution**: Rewrite tessellation shaders or use compute-based approach

3. **Compute Shaders**: Different API in WebGPU
   - **Solution**: Port existing compute shaders to WebGPU compute pipeline

4. **Dependencies**: Not all libraries work with WebAssembly
   - **Status**: Major dependencies (GLFW, Eigen, ImGui, EnTT) have WebAssembly support

5. **Performance**: WebAssembly may be slower than native
   - **Mitigation**: Optimize hot paths, use SIMD where possible

### Resource Challenges

1. **Asset Loading**: Must be asynchronous for web
2. **Bundle Size**: Need to optimize asset sizes
3. **Memory**: Limited by browser constraints

## Success Criteria

- ✅ Application compiles to WebAssembly
- ✅ Runs in modern browsers with WebGPU support
- ✅ All major features functional
- ✅ Acceptable performance (>30 FPS on target hardware)
- ✅ Good user experience (loading, errors, controls)
- ✅ Comprehensive documentation

## Resources & References

- [WebGPU API Specification](https://www.w3.org/TR/webgpu/)
- [WGSL Specification](https://www.w3.org/TR/WGSL/)
- [Learn WebGPU](https://eliemichel.github.io/LearnWebGPU/)
- [WebGPU Fundamentals](https://webgpufundamentals.org/)
- [Emscripten Documentation](https://emscripten.org/docs/)
- [WebGPU Samples](https://webgpu.github.io/webgpu-samples/)

## Contributing

If you'd like to help with the migration:

1. Check this roadmap for in-progress items
2. Discuss approach in an issue before major changes
3. Test thoroughly in multiple browsers
4. Update documentation as you go
5. Submit focused PRs for specific phases

## Questions?

Open an issue with the `webgpu-migration` label for discussion.
