#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>

#include <imgui/imconfig.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <chrono>
#include <cstdlib>
#include <ctime>

#include "macros.h"
#include "global.h"
#include "window.h"
#include "vao.h"
#include "vbo.h"
#include "ebo.h"
#include "defaultshader.h"
#include "DefaultShader2D.h"
#include "texture.h"

#include "model.h"
#include "cube.h"
#include "mouse.h"
#include "camera.h"
#include "light.h"
#include "lamp_shader.h"
#include "sphere.h"
#include "bounds.h"
#include "model_array.h"
#include "bound_cube.h"
#include "environment.h"
#include "TextRenderer.h"
#include "TextShader.h"
#include "Rectangle.h"

PerspectiveCamera perspectiveCamera;
bool mouseCatched = false;
long lastTime;
long fps;
long lastFps;
float deltaTime = 0;
bool drawLine = false;
SphereArray *spheres; // must be pointer (or segment fault because opengl/glfw not properly initialized)
float colDistVal = 0.7f;
float fontScale = 1.0f;

uint64_t timeSinceEpochMillisec() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void printFPS(TextRenderer &textRenderer, TextShader &textShader) {
    long currentTime = timeSinceEpochMillisec();

    textRenderer.begin();
    textRenderer.render(textShader, "FPS: " + std::to_string(lastFps), 50.0f, 50.0f, fontScale, glm::vec3(1.0f));
    textRenderer.end();

    if ((currentTime - lastTime) >= 1000L) {
        std::cout << "FPS: " << fps << std::endl;

        lastTime = currentTime;
        lastFps = fps;
        fps = 0;
    }
}

void handleMouse(GLFWwindow &window) {
    if (mouseCatched) {
        perspectiveCamera.ProcessMouseMovement(Mouse::offsetX, Mouse::offsetY, deltaTime);

        Mouse::offsetX = 0;
        Mouse::offsetY = 0;
    }
}

void handleKeyboard(GLFWwindow &window) {
    if (glfwGetKey(&window, GLFW_KEY_W) == GLFW_PRESS) {
        perspectiveCamera.Move(Camera_Movement::FORWARD, deltaTime);
    }

    if (glfwGetKey(&window, GLFW_KEY_S) == GLFW_PRESS) {
        perspectiveCamera.Move(Camera_Movement::BACKWARD, deltaTime);
    }

    if (glfwGetKey(&window, GLFW_KEY_A) == GLFW_PRESS) {
        perspectiveCamera.Move(Camera_Movement::LEFT, deltaTime);
    }

    if (glfwGetKey(&window, GLFW_KEY_D) == GLFW_PRESS) {
        perspectiveCamera.Move(Camera_Movement::RIGHT, deltaTime);
    }

    if (glfwGetKey(&window, GLFW_KEY_Q) == GLFW_PRESS) {
        perspectiveCamera.Move(Camera_Movement::UP, deltaTime);
    }

    if (glfwGetKey(&window, GLFW_KEY_E) == GLFW_PRESS) {
        perspectiveCamera.Move(Camera_Movement::DOWN, deltaTime);
    }
}

void keyboardCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
        if (mouseCatched) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            mouseCatched = false;
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            mouseCatched = true;
        }
    } else if (key == GLFW_KEY_X && action == GLFW_PRESS) {
        if (!drawLine) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            drawLine = true;
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            drawLine = false;
        }
    } else if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        spotLightOn = spotLightOn ? false : true;
    } else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        RigidBody rigidBody;
        rigidBody.acceleration = Environment::gravitationalAcceleration;
        rigidBody.position = perspectiveCamera.Position;
        rigidBody.transferEnergy(1000.0f, perspectiveCamera.Direction);
        rigidBody.applyAcceleration(Environment::gravitationalAcceleration);
//        rigidBody.applyImpulse(camera.Direction, 1000.0f, deltaTime);
        spheres->addInstance(rigidBody);
    }
}

void drawImGui() {
    ImGui::Begin("Light");
    ImGui::Checkbox("Directonal light", &directionalLightOn);
    ImGui::Checkbox("Spot light", &spotLightOn);
    ImGui::SliderFloat("Shininess", &shininess, 0, 5);
    ImGui::Text("PointLight");
    ImGui::Text("pointLightValue.position");
    ImGui::SliderFloat("X###plpx", &pointLightValue.position.x, -500, 500);
    ImGui::SliderFloat("Y###plpy", &pointLightValue.position.y, -500, 500);
    ImGui::SliderFloat("Z###plpz", &pointLightValue.position.z, -500, 500);
    ImGui::Text("pointLightValue.ambient");
    ImGui::SliderFloat("R###plar", &pointLightValue.ambient.x, 0, 1);
    ImGui::SliderFloat("G###plag", &pointLightValue.ambient.y, 0, 1);
    ImGui::SliderFloat("B###plab", &pointLightValue.ambient.z, 0, 1);
    ImGui::Text("pointLightValue.diffuse");
    ImGui::SliderFloat("R###pldr", &pointLightValue.diffuse.x, 0, 1);
    ImGui::SliderFloat("G###pldg", &pointLightValue.diffuse.y, 0, 1);
    ImGui::SliderFloat("B###pldb", &pointLightValue.diffuse.z, 0, 1);
    ImGui::Text("pointLightValue.k");
    ImGui::SliderFloat("K0###plk0", &pointLightValue.k0, 0, 1);
    ImGui::SliderFloat("K1###plk1", &pointLightValue.k1, 0, 1);
    ImGui::SliderFloat("K2###plk2", &pointLightValue.k2, 0, 1);

    ImGui::Text("SpotLight");
    ImGui::Text("spotLightValue.ambient");
    ImGui::SliderFloat("R###slar", &spotLightValue.ambient.x, 0, 1);
    ImGui::SliderFloat("G###slag", &spotLightValue.ambient.y, 0, 1);
    ImGui::SliderFloat("B###slab", &spotLightValue.ambient.z, 0, 1);
    ImGui::Text("spotLightValue.diffuse");
    ImGui::SliderFloat("R###sldr", &spotLightValue.diffuse.x, 0, 1);
    ImGui::SliderFloat("G###sldg", &spotLightValue.diffuse.y, 0, 1);
    ImGui::SliderFloat("B###sldb", &spotLightValue.diffuse.z, 0, 1);
    ImGui::Text("spotLightValue.k");
    ImGui::SliderFloat("K0###slk0", &spotLightValue.k0, 0, 1);
    ImGui::SliderFloat("K1###slk1", &spotLightValue.k1, 0, 1);
    ImGui::SliderFloat("K2###slk2", &spotLightValue.k2, 0, 1);
    ImGui::Text("spotLightValue.cutOff");
    ImGui::SliderFloat("cutOff###slco", &spotLightValue.cutOff, 0, 1);
    ImGui::Text("spotLightValue.outterCutOff");
    ImGui::SliderFloat("outterCutOff###sloco", &spotLightValue.outterCutOff, 0, 1);
    ImGui::End();

    ImGui::Begin("Other");
    ImGui::Text("Camera");
    ImGui::SliderFloat("Speed###cs", &perspectiveCamera.MovementSpeed, 0, 200);
    ImGui::SliderFloat("Mouse###cm", &perspectiveCamera.MouseSensitivity, 0, 20);
    ImGui::Text("Model");
    ImGui::SliderFloat("X###mx", &modelposition.x, 0, 1000);
    ImGui::SliderFloat("Y###my", &modelposition.y, 0, 1000);
    ImGui::SliderFloat("Z###mz", &modelposition.z, 0, 1000);
    ImGui::Text("Collision Dist");
    ImGui::SliderFloat("Distance###cd", &colDistVal, 0, 1);
    ImGui::Text("Font");
    ImGui::SliderFloat("Scale###fs", &fontScale, -2, 10);
    ImGui::End();

    ImGui::Begin("Draw");
    ImGui::Checkbox("Line", &drawLine);
    ImGui::End();

    ImGui::Begin("Apply");
    ImGui::Checkbox("Rigid Body", &applyRb);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main() {
    // initialize glfw
    if (!glfwInit()) {
        LOG("failed initialize glfw");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    Window window(WINDOW_WIDTH, WINDOW_HEIGHT);
    window.makeContextCurrent();

    glfwSetKeyCallback(window.window, keyboardCallBack);
    glfwSetCursorPosCallback(window.window, Mouse::cursorPosCallback);

    DefaultShader defaultShader("assets/shaders/default.vertex.glsl", "assets/shaders/default.fragment.glsl");
    LampShader lampShader("assets/shaders/default.vertex.glsl", "assets/shaders/lamp.fragment.glsl");

    std::vector<Cube> cubes;
    srand(time(0));

    int randomVal = 10;
    int offSetX = 10;
    int offSetY = 10;

    Texture texture("assets/textures/indo.jpeg", GL_TEXTURE0, aiTextureType_DIFFUSE);
//    Texture texture2("assets/textures/spec.png", GL_TEXTURE1, aiTextureType_SPECULAR);

    for (int i = 1; i <= 10; i++) {
        Cube cube(eMaterial::emerald);
        cube.addTexture(texture);
//        cube.addTexture(texture2);
        cube.translate((float) (rand() % (randomVal * 2) - randomVal) - offSetX, (float) (rand() % (randomVal * 2) - randomVal) + offSetY, (float) (rand() % (randomVal * 2) - randomVal));
        cube.scale(glm::vec3(2.0f));
        cubes.push_back(cube);
    }

    Environment environment;

    DirectionalLight directionalLight(glm::vec3(0.f, -2.0f, -3.f), glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), glm::vec4(1.4f, 1.4f, 1.4f, 1.0f), glm::vec4(.75f, .75f, .75f, 1.0f));
//    SpotLight spotLight(camera.Position, camera.Direction, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(20.f)), glm::vec3(0.f), glm::vec3(1.f), glm::vec3(1.f), 1.0, .07, .032);
    SpotLight spotLight(perspectiveCamera.Position, perspectiveCamera.Direction, glm::cos(glm::radians(15.5f)), glm::cos(glm::radians(20.f)), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, .005f, .0052f);

    for (int i = 1; i <= 3; i++) {
        Lamp lamp(glm::vec3(1.f));
        lamp.translate((float) (rand() % (randomVal * 2) - randomVal) - offSetX, (float) (rand() % (randomVal * 2) - randomVal) + offSetX, (float) (rand() % (randomVal * 2) - randomVal));
        PointLight pointLight(glm::vec3(0.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), 1.0f, 0.05f, 0.05f);
        pointLight.position = lamp.getPosition();
        environment.addLight(pointLight);
        environment.addLamp(lamp);
    }

    environment.addLight(spotLight);
    environment.addLight(directionalLight);

    perspectiveCamera.MovementSpeed = 6;
    perspectiveCamera.MouseSensitivity = 11;
//    camera.Position.x -= offSetX;
//    camera.Position.y += offSetY;
//    camera.UpdateView();

    spheres = new SphereArray();
    Model model("assets/models/wwc.obj", BoundTypes::AABB);
    Cube c(eMaterial::green_plastic);

//    glfwSwapInterval(0);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    TextShader textShader("assets/shaders/text.vertex.glsl", "assets/shaders/text.fragment.glsl");
    TextRenderer::init();
    TextRenderer textRenderer("assets/fonts/FreeMono.ttf");
    DefaultShader2D defaultShader2D("assets/shaders/default2d.vertex.glsl", "assets/shaders/default2d.fragment.glsl");

//    Rectangle rectangle(100, 100, glm::vec4(1.0f));
//    rectangle.scale(100.0f, 100.0f, 0.0f);
//    OrthographicCamera orthographicCamera;

    glEnable(GL_DEPTH_TEST);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    int currentTime = 0;
    lastTime = 0;
    long lastFrameTime = 0;
    int elapseFrameTime = 0;

    while (!glfwWindowShouldClose(window.window)) {
        currentTime = timeSinceEpochMillisec();
        elapseFrameTime = currentTime - lastFrameTime;
        deltaTime = elapseFrameTime / 1000.f;
        lastFrameTime = currentTime;

        glfwPollEvents(); // should call if want to handle event
        handleMouse(*window.window);
        handleKeyboard(*window.window);

        ////////////////////////////////////////////////////////////////////// DRAWING 3D

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!environment.spotLights.empty()) {
            environment.spotLights[0].position = perspectiveCamera.Position;
            environment.spotLights[0].direction = perspectiveCamera.Direction;
            environment.spotLights[0].cutOff = spotLightValue.cutOff;
            environment.spotLights[0].outterCutOff = spotLightValue.outterCutOff;
            environment.spotLights[0].k0 = spotLightValue.k0;
            environment.spotLights[0].k1 = spotLightValue.k1;
            environment.spotLights[0].k2 = spotLightValue.k2;
        }

        environment.render(lampShader, perspectiveCamera);

        c.transToIdentity();
        c.translate(perspectiveCamera.Position + (perspectiveCamera.Direction * 5.0f));
        glm::vec3 intersectsDistance = glm::vec3(0.0f);

        for (Cube &cube : cubes) {
            if (c.intersectsWith(*cube.getBoundingRegion(), intersectsDistance)) {
                LOG("-------intersects");
            }
        }


        for (Cube &cube : cubes) {
            cube.begin();
            cube.render(defaultShader, perspectiveCamera, environment);
            cube.end();
        }

        c.begin();
        c.render(defaultShader, perspectiveCamera, environment);
        c.begin();

        spheres->render(defaultShader, perspectiveCamera, environment, deltaTime);

        model.begin();
        model.render(defaultShader, perspectiveCamera, environment);
        model.end();



        //////////////////////////////////////////////////////////////////////// DRAWING 2D

//        glClear(GL_DEPTH_BUFFER_BIT); // must call before rendering 2D

//        rectangle.begin();
//        rectangle.render(defaultShader2D, orthographicCamera);
//        rectangle.end();

        ////////////////////////////////////////////////////////////////////////

        fps++;
        printFPS(textRenderer, textShader);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        drawImGui();

        glfwSwapBuffers(window.window);

    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    defaultShader.cleanUp();
    lampShader.cleanUp();

    for (Cube &cube : cubes) {
        cube.cleanUp();
    }

    for (Lamp &lamp : environment.lamps) {
        lamp.cleanUp();
    }

    spheres->cleanUp();

    // should call before exit
    glfwTerminate();
    return 0;
}
