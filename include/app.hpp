#pragma once

#ifdef PLATFORM_WINDOWS
    #define NOMINMAX
#endif
#include <cmath>
#include <iostream>
#include <unordered_set>
#include <memory>
#include <vector>
#include <camera.hpp>
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#undef near
#undef far
#include <GLFW/glfw3.h>
#include <cyGL.h>
#include <model.hpp>
#include <mesh.hpp>
#include <physics.hpp>
#include <light.hpp>
#include <entt/entt.hpp>
#include <texture.hpp>
#include <cxxopts.hpp>

// Index into element counts and offsets arrays
struct ObjRef {
    size_t objID;
};

// Represents a render pass with a target framebuffer
struct RenderPass {
    std::shared_ptr<Camera> camera;
    GLuint fbo;
    GLuint tex;
    GLenum texTarget = GL_TEXTURE_2D;
    GLuint rbo;
    std::vector<ObjRef> objMask = {};
};

class App
{
    private:
        ColliderInteriorBox box{Vector3f(-6.0f, 0.0f, -6.0f), vec3(6.0f)};
        // const size_t objectsToGen = this->box.volume() * 0.01f;
        const size_t objectsToGen = 5u;
        const float frameRate = 60.0f;
        const float framePeriod = 1.0f / this->frameRate;

        float t = 0.0f;
        float lastFrameTime = 0.0f;

        // Input
        Vector2f mousePos = {0.0f, 0.0f};
        Vector2f mouseClickStart = {0.0f, 0.0f};
        Vector2f mouseDeltaPos = {0.0f, 0.0f};
        bool mouseLeft = false;
        bool mouseRight = false;
        bool mouseMiddle = false;
        bool mouseMoved = false;
        std::unordered_set<int> pressedKeys;


        Vector2i windowSize = {1280, 720};
        std::shared_ptr<Camera> camera = std::make_shared<Camera>();
        std::shared_ptr<Camera> reflCamera = std::make_shared<Camera>();
        cyGLSLProgram meshProg;
        cyGLSLProgram wiresProg;
        cyGLSLProgram skyProg;
        std::shared_ptr<Light> sunlight;

        entt::registry reg;
        entt::entity ePlane;
        
        RNG rng{0u};

        // ID of model transform matrices SSBO
        GLuint ssboModels;
        // ID of model materials SSBO
        GLuint ssboMaterials;
        // ID of lights SSBO
        GLuint ssboLights;

        GLuint vaoMeshes;
        MeshCollection meshes;

        GLuint vaoSky;
        GLuint vboSky;

        GLuint vaoWires;
        GLuint vboWires;
        GLuint vboPath;
        GLuint vboBox;
        // ID of arrow model transform matrices SSBO
        GLuint ssboArrows;
        // ID of arrow colors SSBO
        GLuint ssboArrowColors;

        TextureCollection skyTextures;

        GLuint fboReflections;
        GLuint rboDepth;
        GLuint texReflections;

        std::vector<RenderPass> renderPasses;

        // Element counts
        std::vector<GLsizei> vCounts;
        // Element offsets
        std::vector<size_t> vOffsets;
    public:
        GLFWwindow* window;

        App(cxxopts::ParseResult& args);
        ~App();

        /**
         * @brief Handle key events from GLFW
         * 
         * @param key Some GLFW key code
         * @param pressed True if pressed, false if released
         */
        void onKey(int key, bool pressed);

        /**
         * @brief Handle mouse button events from GLFW
         * 
         * @param button 
         * @param pressed True if pressed, false if released
         */
        void onClick(int button, bool pressed);

        /**
         * @brief Called on window resize
         * 
         * @param width 
         * @param height 
         */
        void onResize(int width, int height);

        /**
         * @brief Called between frames if there is time to spare
         */
        void idle();

        /**
         * @brief Main entry
         */
        void run();

        /**
         * @brief Draw the scene
         */
        void draw(float dt);

        // Draws the sky shader program to the current framebuffer
        void drawSky(const Matrix4f& view, const Matrix4f& proj);

        // Draws the meshes shader program to the current framebuffer
        void drawMeshes(const Matrix4f& view, const Matrix4f& proj, const Vector3f& camPos);

        // Draws the debugging shader program to the current framebuffer
        void drawDebug(const Matrix4f& view, const Matrix4f& proj);

        // Updates SSBOs for all shader programs
        void updateBuffers();

        void simulate(float dt);

        void composeUI();

        void loadingScreen(const std::string& message);

        // Hides given object from rendering
        void hidden(entt::entity e, bool hidden);

        entt::entity makeParticle();
        entt::entity makeModel(const std::string& name);
        entt::entity makeReflectiveModel(const std::string& name);
        entt::entity makeLight(const Vector3f& pos, const Vector3f& color, float intensity, LightType type);
};