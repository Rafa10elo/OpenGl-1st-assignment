#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shader.h"
#include "Cuboid.h"
#include "Disk.h"
#include "HollowDisk.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

glm::vec3 camPos   = glm::vec3(0.0f, 1.0f, 7.0f);
glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camUp    = glm::vec3(0.0f, 1.0f, 0.0f);
float yaw = -90.0f, pitch = 0.0f;
float lastX = 400, lastY = 300;
bool firstMouse = true;
float fov = 45.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= 0.1f;
    yoffset *= 0.1f;

    yaw   += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 dir;
    dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir.y = sin(glm::radians(pitch));
    dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    camFront = glm::normalize(dir);
}

void scroll_callback(GLFWwindow*, double, double yoffset) {
    fov -=(float)yoffset;
   if (fov < 1.0f) fov = 1.0f;
    if (fov > 45.0f) fov = 45.0f;
}

int main() {
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(800, 600, "rfrof", nullptr, nullptr);
    if (!window) return -1;
    glfwMakeContextCurrent(window);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW FAILED\n";
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    Shader shader("../shaders.glsl");

    Cuboid clockBase(
            glm::vec3(0.f, 0.5f, 0.f),
            glm::vec3(1.2f, 2.5f, 0.49f),
            {
                    glm::vec3(0.35f, 0.25f, 0.15f),
                    glm::vec3(0.35f, 0.25f, 0.15f),
                    glm::vec3(0.3f,  0.2f,  0.12f),
                    glm::vec3(0.3f,  0.2f,  0.12f),
                    glm::vec3(0.4f,  0.3f,  0.18f),
                    glm::vec3(0.2f,  0.15f, 0.1f)
            }
    );

    Disk clockFace(
            glm::vec3(0.f, 2.1f, 0.f),
         0.9f,
            80,
          0.5f,
          glm::vec3(0.9f, 0.9f, 0.95f)
    );

    HollowDisk clockRim(
            glm::vec3(0.f, 2.1f, 0.1f),
            0.95f,
            0.75f,
         80,
          glm::vec3(0.6f, 0.15f, 0.1f)
    );

    Cuboid hourHand(
            glm::vec3(0.f, 2.1f, 0.60f),
            glm::vec3(0.12f, 0.45f, 0.03f),
            {
                    glm::vec3(0.1f, 0.1f, 0.1f),
                    glm::vec3(0.1f, 0.1f, 0.1f),
                    glm::vec3(0.1f, 0.1f, 0.1f),
                    glm::vec3(0.1f, 0.1f, 0.1f),
                    glm::vec3(0.1f, 0.1f, 0.1f),
                    glm::vec3(0.1f, 0.1f, 0.1f)
            }
    );
    Cuboid minuteHand(
            glm::vec3(0.f, 2.1f, 0.61f),
            glm::vec3(0.08f, 0.65f, 0.03f),
            {
                    glm::vec3(0.2f, 0.2f, 0.2f),
                    glm::vec3(0.2f, 0.2f, 0.2f),
                    glm::vec3(0.2f, 0.2f, 0.2f),
                    glm::vec3(0.2f, 0.2f, 0.2f),
                    glm::vec3(0.2f, 0.2f, 0.2f),
                    glm::vec3(0.2f, 0.2f, 0.2f)
            }
    );
    Cuboid secondHand(
            glm::vec3(0.f, 2.1f, 0.62f),
            glm::vec3(0.04f, 0.75f, 0.02f),
            {
                    glm::vec3(0.8f, 0.1f, 0.1f),
                    glm::vec3(0.8f, 0.1f, 0.1f),
                    glm::vec3(0.8f, 0.1f, 0.1f),
                    glm::vec3(0.8f, 0.1f, 0.1f),
                    glm::vec3(0.8f, 0.1f, 0.1f),
                    glm::vec3(0.8f, 0.1f, 0.1f)
            }
    );



    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.08f, 0.08f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float spd = 0.005f;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camPos += camFront * spd;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camPos -= camFront * spd;
     if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camPos -= glm::normalize(glm::cross(camFront, camUp)) * spd;
     if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camPos += glm::normalize(glm::cross(camFront, camUp)) * spd;

        glm::mat4 view = glm::lookAt(camPos, camPos + camFront, camUp);
        glm::mat4 proj = glm::perspective(glm::radians(fov), 800.f / 600.f, 0.1f, 100.f);

        clockBase.draw(shader, view, proj);
        clockFace.draw(shader, view, proj);
        clockRim.draw(shader, view, proj);
        hourHand.draw(shader, view, proj);
        minuteHand.draw(shader, view, proj);
        secondHand.draw(shader, view, proj);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
