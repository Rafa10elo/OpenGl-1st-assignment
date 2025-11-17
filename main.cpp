#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shader.h"
#include "Cuboid.h"
#include "Disk.h"
#include "HollowDisk.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <chrono>

glm::vec3 camPos = glm::vec3(0.0f, 1.0f, 7.0f);
glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
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

    yaw += xoffset;
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
    fov -= (float)yoffset;
    if (fov < 1.0f) fov = 1.0f;
    if (fov > 45.0f) fov = 45.0f;
}
glm::vec3 clockCenter(0.0f, 2.1f, 0.3f);
int main() {
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(800, 600, "Pendulum Clock", nullptr, nullptr);
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

    Shader shader("./../shaders.glsl");

    Cuboid clockBase(
        glm::vec3(0.f, 0.5f, 0.f),
        glm::vec3(1.0f, 2.5f, 0.49f),
        {
                glm::vec3(0.45f, 0.25f, 0.18f),
                glm::vec3(0.45f, 0.25f, 0.18f),
                glm::vec3(0.45f, 0.25f, 0.18f),
                glm::vec3(0.45f, 0.25f, 0.18f),
                glm::vec3(0.45f, 0.25f, 0.18f),
                glm::vec3(0.45f, 0.25f, 0.18f)
        }
    );

    Disk clockFace(
        glm::vec3(0.f, 2.1f, 0.f),
        0.9f,
        80,
        0.5f,
        glm::vec3(0.99f, 0.98f, 0.94f)
    );

    HollowDisk clockRim(
        glm::vec3(0.f, 2.1f, 0.15f),
        0.95f,
        0.75f,
        80,
        glm::vec3(0.55f, 0.41f, 0.39f)
    );

    Cuboid hourHand(
        glm::vec3(0.0f, 2.26f, 0.3f),
        glm::vec3(0.11f, 0.45f, 0.03f),
        {
                glm::vec3(0.91f, 0.68f, 0.65f),
                glm::vec3(0.91f, 0.68f, 0.65f),
                glm::vec3(0.91f, 0.68f, 0.65f),
                glm::vec3(0.91f, 0.68f, 0.65f),
                glm::vec3(0.91f, 0.68f, 0.65f),
                glm::vec3(0.91f, 0.68f, 0.65f)
        }
    );
    Cuboid minuteHand(
        glm::vec3(0.0f, 2.32f, 0.32f),
        glm::vec3(0.08f, 0.65f, 0.03f),
        {
                glm::vec3(0.91f, 0.68f, 0.65f),
                glm::vec3(0.91f, 0.68f, 0.65f),
                glm::vec3(0.91f, 0.68f, 0.65f),
                glm::vec3(0.91f, 0.68f, 0.65f),
                glm::vec3(0.91f, 0.68f, 0.65f),
                glm::vec3(0.91f, 0.68f, 0.65f)
        }
    );
    Cuboid secondHand(
        glm::vec3(0.0f, 2.4f, 0.34f),
        glm::vec3(0.04f, 0.75f, 0.02f),
        {
                glm::vec3(0.45f, 0.25f, 0.18f),
                glm::vec3(0.45f, 0.25f, 0.18f),
                glm::vec3(0.45f, 0.25f, 0.18f),
                glm::vec3(0.45f, 0.25f, 0.18f),
                glm::vec3(0.45f, 0.25f, 0.18f),
                glm::vec3(0.45f, 0.25f, 0.18f)
        }
    );
    Cuboid rod(
        glm::vec3(0.0f, 0.45f, 0.29f),
        glm::vec3(0.25f, 1.55f, 0.12f),
        {
                glm::vec3(0.65f, 0.48f, 0.35f),
                glm::vec3(0.65f, 0.48f, 0.35f),
                glm::vec3(0.65f, 0.48f, 0.35f),
                glm::vec3(0.65f, 0.48f, 0.35f),
                glm::vec3(0.65f, 0.48f, 0.35f),
                glm::vec3(0.65f, 0.48f, 0.35f)
        }
    );
    Disk pendulum(
        glm::vec3(0.f, -0.28f, 0.29f),
        0.2f,
        80,
        0.1f,
        glm::vec3(0.65f, 0.48f, 0.35f)
    );
    Cuboid mid(
        glm::vec3(0.0f, 2.8f, 0.3f),
        glm::vec3(0.08f, 0.22f, 0.03f),
        {
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.1f, 0.1f, 0.1f)
        }
    );
    Cuboid six(
        glm::vec3(0.0f, 1.43f, 0.3f),
        glm::vec3(0.08f, 0.22f, 0.03f),
        {
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.1f, 0.1f, 0.1f)
        }
    );
    Cuboid three(
        glm::vec3(0.7f, 2.1f, 0.3f),
        glm::vec3(0.22f, 0.079f, 0.03f),
        {
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.1f, 0.1f, 0.1f)
        }
    );
    Cuboid nine(
        glm::vec3(-0.68f, 2.1f, 0.3f),
        glm::vec3(0.22f, 0.079f, 0.03f),
        {
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.1f, 0.1f, 0.1f)
        }
    );



    glm::vec3 pendulumPivot(0.0f, 1.1f, 0.25f);


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

        auto now = std::chrono::system_clock::now();
        auto time_t_now = std::chrono::system_clock::to_time_t(now);
        std::tm local_time;
        localtime_s(&local_time, &time_t_now);

        float hours = local_time.tm_hour % 12;
        float minutes = local_time.tm_min;
        float seconds = local_time.tm_sec;

        float hourAngle = glm::radians(-(hours * 30.0f + minutes * 0.5f)); 
        float minuteAngle = glm::radians(-(minutes * 6.0f));  
        float secondAngle = glm::radians(-(seconds * 6.0f));

        float pendulumAngle = glm::sin(glfwGetTime() * 2.6f) * glm::radians(15.0f);


        clockBase.draw(shader, view, proj);
        clockFace.draw(shader, view, proj);
        clockRim.draw(shader, view, proj);
        hourHand.drawRotated(shader, view, proj, hourAngle, glm::vec3(0.0f, 0.0f, 1.0f)  ,clockCenter);
        minuteHand.drawRotated(shader, view, proj, minuteAngle, glm::vec3(0.0f, 0.0f, 1.0f) , clockCenter);
        secondHand.drawRotated(shader, view, proj, secondAngle, glm::vec3(0.0f, 0.0f, 1.0f),clockCenter);
        //rod.draw(shader, view, proj);
        //pendulum.draw(shader, view, proj);
        rod.drawRotated(shader, view, proj, pendulumAngle, glm::vec3(0.0f, 0.0f, 1.0f), pendulumPivot);
        pendulum.drawRotated(shader, view, proj, pendulumAngle, glm::vec3(0.0f, 0.0f, 1.0f), pendulumPivot);
        mid.draw(shader, view, proj);
        six.draw(shader, view, proj);
        three.draw(shader, view, proj);
        nine.draw(shader, view, proj);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
