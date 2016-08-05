#include "Globals.hpp"
#include "UI.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <ctime>
#include <string>
#include <memory>
#include <iostream>

#include <glew.h>
#include <glfw3.h>

#include "glm.hpp"
#include "matrix_transform.hpp"
#include "type_ptr.hpp"

#include "Particles.hpp"
#include "Updaters.hpp"
#include "Generators.hpp"
#include "Renderer.hpp"

#include "Effect.hpp"

#define VERTEX_SHADER_PATH "vShader.glsl"
#define FRAGMENT_SHADER_PATH "fShader.glsl"
#define TEXTURE_PATH "pTexture.png"

using namespace std;

const char* ApplicationWindowName = "Particle Systems Demo";

const unsigned int WND_WIDTH { 1280 };
const unsigned int WND_HEIGHT { 720 };

Shader* gShader = new Shader();
GLFWwindow* gWindow;
std::shared_ptr<Effect> gEffect;
int gFBwidth, gFBheight;

int gNumParticles = 0;
int gNumAlive = 0;

unsigned int gParticleTexture = 0;

struct Camera {
    
    float camDist;
    glm::vec3 camDir;
    glm::mat4 modelViewMatrix;
    glm::mat4 projectionMatrix;
} camera;

bool gAnimationOn = true;

// Func. Prototypes
bool initApp(void);
void cleanUp(void);

void updateScene(double deltaTime);
void renderScene(void);

// Func. Callbacks
static GLvoid mouseButtonCallback(GLFWwindow* window, GLint button, GLint action, GLint mods){
    TwEventMouseButtonGLFW(button, action);
}

static GLvoid cursorCallback(GLFWwindow* window, GLdouble x, GLdouble y) {
    TwEventMousePosGLFW(x * (gFBwidth / WND_WIDTH), y * (gFBheight / WND_HEIGHT)); // ATB is not designed for Retina/HiDPI Displays, map screen space coordinates to GL pixel coordinates
}

/// ____Program Entry Point____
int main(int argc, char* argv[]) {
    
    if (initApp()) {
        
        while (!glfwWindowShouldClose(gWindow)) {
            
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            double timestep = 0.0100;
            
            updateScene(timestep);

            renderScene();
            
            TwDraw();
            
            glfwSwapBuffers(gWindow);
            glfwPollEvents();
        }
    } else {
        cerr << "Initialisation Failed, Exiting..." << endl;
    }
    
    cleanUp();
    
    TwTerminate();
    glfwTerminate();
    
    return 0;
}

bool initApp(void) {
    
    if (!glfwInit()) {
        return 1;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x SSAA
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    gWindow = glfwCreateWindow(WND_WIDTH, WND_HEIGHT, ApplicationWindowName, NULL, NULL);
    glfwGetFramebufferSize(gWindow, &gFBwidth, &gFBheight);

    if (gWindow == NULL) {
        
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return 1;
    }
    
    glfwMakeContextCurrent(gWindow);
    
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        
        std::cerr << "Failed to initialize GLEW." << std::endl;
        glfwTerminate();
        exit(-1);
    }
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.11f, 0.12f, 0.13f, 0.0f);
    
    // Set Callback Functions
    glfwSetCursorPosCallback(gWindow, cursorCallback);
    glfwSetMouseButtonCallback(gWindow, mouseButtonCallback);
    
    camera.projectionMatrix = glm::perspective(35.0f, (float)WND_WIDTH / (float)WND_HEIGHT, 0.1f, 1000.0f);
    camera.camDir = *new glm::vec3(-0.65f, 0.70f, 0.20f);
    camera.camDist = 2.5f;
    
    // Compile and link shaders
    if (!(gShader->ProgramID = gShader->LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH))) {
        return false;
    }
    
    // Generate texture
    if (!texLdr::loadTexture(TEXTURE_PATH, &gParticleTexture)) {
        return false;
    }
    
    glUseProgram(gShader->GetProgramID());
    glUniform1i(glGetUniformLocation(gShader->GetProgramID(), "tex"), 0);
    glUseProgram(0);
    
    gEffect = std::make_shared<Effect>();
    gEffect->initialise(Effect::DEFAULT_PARTICLE_NUM_FLAG);
    gEffect->initialiseRenderer();
    
    // Initialise AntTweakBar and Tweaker parameters
    
    if (!((WND_WIDTH == gFBwidth) && (WND_HEIGHT == gFBheight))) { // Retina/HiDPI check
        
        TwDefine(" GLOBAL fontscaling = 2.00 ");
        TwDefine(" GLOBAL fontsize = '2' ");
    }
    
    TwInit(TW_OPENGL_CORE, NULL);
    TwWindowSize(gFBwidth, gFBheight);
    
    TwDefine(" GLOBAL iconmargin = '36 32' ");
    TwDefine(" GLOBAL contained = 'true' ");
    
    Globals::mainBar = TwNewBar("Controls");
    
    TwDefine(" 'Controls' size = '550 650' ");
    TwDefine(" 'Controls' refresh = '0.0001f' ");
    
    UI::AddVariableRW("Pause/Play", &gAnimationOn, NULL);
    UI::AddVariableRO("Particles", &gNumParticles, NULL);
    UI::AddVariableRO("Particles (Active)", &gNumAlive, NULL);
    
    UI::AddDirection("Camera", &camera.camDir.x, NULL); // Dir3f = 12 bytes
    UI::AddVariableRW("Cam. Distance", &camera.camDist, "min = 0.05 max = 4.0 step = 0.01");
    UI::AddSeparator();
    
    gEffect->addUI();
    
    return true;
}

void updateScene(double deltaTime) {
    
    if (!gAnimationOn) {
        return;
    }
    
    gEffect->update(deltaTime);
    gEffect->systemUpdate(deltaTime);
    
    gNumAlive = 0;
    gEffect->renderUpdate(deltaTime);
    
    gNumParticles = gEffect->numAllParticles();
    gNumAlive = gEffect->numAliveParticles();
}

void renderScene(void) {
    
    glDisable(GL_DEPTH_TEST);
    
    camera.modelViewMatrix = glm::lookAt(camera.camDir * camera.camDist, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gParticleTexture);
    
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    glUseProgram(gShader->GetProgramID());
    glUniformMatrix4fv(glGetUniformLocation(gShader->GetProgramID(), "matProjection"), 1, GL_FALSE, glm::value_ptr(camera.projectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(gShader->GetProgramID(), "matModelView"), 1, GL_FALSE, glm::value_ptr(camera.modelViewMatrix));
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    gEffect->render();
    
    glDisable(GL_BLEND);
    
    glUseProgram(0);
}

void cleanUp(void) {
    
    gEffect->clean();
    glDeleteTextures(1, &gParticleTexture);
}
