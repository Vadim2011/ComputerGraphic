
#define GLEW_DLL
#define GLFW_DLL


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Mesh.h"
#include "Model.h"

#include <iostream>
#include <cmath>
#include <array>
#include <fstream>
#include <sstream>


const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// const float aspectRatio = (float)SCR_HEIGHT / (float)SCR_WIDTH; // camera
glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 6.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    // move came delta
const float cameraSpeed = 0.015f;
    // move mouse
bool firstMouse = true;
float pitch = 0.0;
float yaw = -90.0;
float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);


int main()
{

    float points[] = { -0.5f, -0.5f, 0.0f,
                   0.5f, -0.5f, 0.0f,
                   -0.5f, 0.5f, 0.0f,
                   0.5f, 0.5f, 0.0f };

    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Mainwindow", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return 1;
    } 

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    glEnable(GL_DEPTH_TEST); // Z - buffer depth

    // bound cursore to window and height + hanler mouse call
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    GLenum ret = glewInit();
    if (GLEW_OK != ret) {
        fprintf(stderr, "ERROR: %s \n", glewGetErrorString(ret));
        return 1;
    }

    // load models   
    // Model loadedModel("./RTC_NikolaevVA.obj");
    Model baseModel("./model/RTC_NikolaevVA_base.obj");
    Model bottomModel("./model/RTC_NikolaevVA_bottom.obj");
    Model middleModel("./model/RTC_NikolaevVA_middle.obj");
    Model topModel("./model/RTC_NikolaevVA_top.obj");

    // Model loadedModel("./Cube.obj");
    // glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 model_base = glm::mat4(1.0f); 
    glm::mat4 model_bottom = glm::mat4(1.0f);
    glm::mat4 model_middle = glm::mat4(1.0f);
    glm::mat4 model_top = glm::mat4(1.0f);

    // матрица проекции вида
    glm::mat4 proj = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 80.0f);

    // create shader
    // Shader ourShader("shaders/shader.vert", "shaders/shader.frag");    
    Shader shader_base("shaders/shader.vert", "shaders/shader.frag");
    Shader shader_bottom("shaders/shader.vert", "shaders/shader.frag");
    Shader shader_middle("shaders/shader.vert", "shaders/shader.frag");
    Shader shader_top("shaders/shader.vert", "shaders/shader.frag");
    
    float rotate_bottom = 0.0f;
    float rotate_middle = 0.0f;
    float rotate_top = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        shader_base.setLightMaterial(proj);
        shader_bottom.setLightMaterial(proj);
        shader_middle.setLightMaterial(proj);
        shader_top.setLightMaterial(proj);

        shader_base.use();
        shader_bottom.use();
        shader_middle.use();
        shader_top.use();

        // render background color
        glClearColor(0.1f, 0.1f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        shader_base.setMatrix4fv("model", model_base);
        shader_base.setMatrix4fv("view", view);
        shader_base.setVec3("viewPos", cameraPos);
        baseModel.Draw(shader_base);
        // ourShader.setFloat("u_aspect_ratio", aspectRatio);

        // move Object =========================== MODEL matrix ================
        model_bottom = glm::mat4(1.0f);
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
            rotate_bottom -= 1.0f;  // forward
        }
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
            rotate_bottom += 1.0f;  // back
        }
        model_bottom = glm::rotate(model_bottom, glm::radians(rotate_bottom), glm::vec3(0.0f, 1.0f, 0.0f));
        model_middle = glm::rotate(glm::mat4(1.0f), glm::radians(rotate_bottom), glm::vec3(0.0f, 1.0f, 0.0f));
        model_top = glm::rotate(glm::mat4(1.0f), glm::radians(rotate_bottom), glm::vec3(0.0f, 1.0f, 0.0f));


        // ===============================================================================

        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS && rotate_middle > -134.0f) {
            rotate_middle -= 1.0f;  
        }
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && rotate_middle < 27.0f) {
            rotate_middle += 1.0f;  
        }

        model_middle = glm::translate(model_middle, glm::vec3(0.89f, 1.84f, 0.0f));
        model_top = glm::translate(model_top, glm::vec3(0.89f, 1.84f, 0.0f));

        model_middle = glm::rotate(model_middle, glm::radians(rotate_middle), glm::vec3(0.0f, 0.0f, 1.0f));
        model_top = glm::rotate(model_top, glm::radians(rotate_middle), glm::vec3(0.0f, 0.0f, 1.0f));

        model_middle = glm::translate(model_middle, glm::vec3(-0.89f, -1.84f, 0.0f));
        model_top = glm::translate(model_top, glm::vec3(-0.89f, -1.84f, 0.0f));

        // ===============================================================================

        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && rotate_top > -13.0f) {
            rotate_top -= 1.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && rotate_top < 210.f) {
            rotate_top += 1.0f;
        }
        model_top = glm::translate(model_top, glm::vec3(-1.0f, 3.14f, 0.0f));
        model_top = glm::rotate(model_top, glm::radians(rotate_top), glm::vec3(0.0f, 0.0f, 1.0f));
        model_top = glm::translate(model_top, glm::vec3(1.0f, -3.14f, 0.0f));

        shader_bottom.setMatrix4fv("model", model_bottom);
        shader_bottom.setMatrix4fv("view", view);
        shader_bottom.setVec3("viewPos", cameraPos);
        bottomModel.Draw(shader_bottom);

        shader_middle.setMatrix4fv("model", model_middle);
        shader_middle.setMatrix4fv("view", view);
        shader_middle.setVec3("viewPos", cameraPos);
        middleModel.Draw(shader_middle);

        shader_top.setMatrix4fv("model", model_top);
        shader_top.setMatrix4fv("view", view);
        shader_top.setVec3("viewPos", cameraPos);
        topModel.Draw(shader_top);
        
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cameraPos += cameraSpeed * cameraFront;  // forward
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cameraPos -= cameraSpeed * cameraFront;  // back
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;  // left
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;  // reight
        }

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            cameraPos += cameraUp * cameraSpeed;  // up
        }
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
            cameraPos -= cameraUp * cameraSpeed;  // down
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();

    return 0;
}


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = (float)xposIn;
    float ypos = (float)yposIn;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
};