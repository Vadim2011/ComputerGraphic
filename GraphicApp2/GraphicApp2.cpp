
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

// GLSL
// VBO / VAO / EBO
// VBO (vertex buffer object)
// VAO (vertex array object)
// EBO (element buffer object)


const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;
// const float aspectRatio = (float)SCR_HEIGHT / (float)SCR_WIDTH;

// ============================ glm
// vectom glog coord  // this  global center coord
// glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 6.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    // move came delta
const float cameraSpeed = 0.02f;
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
    Model loadedModel("./RTC_NikolaevVA4.obj");
    // Model loadedModel("./RTC_NikolaevVA7.obj");
    // Model loadedModel("./Cube.obj");
    glm::mat4 model = glm::mat4(1.0f); 

    // proj init perspective ================== PROJECTION on CAMERA ==========
      // матрица проекции вида
    glm::mat4 proj = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);

    // create shader
    Shader ourShader("shaders/shader.vert", "shaders/shader.frag");

    while (!glfwWindowShouldClose(window)) {

        // render background color
        glClearColor(0.1f, 0.1f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();
        // ourShader.setFloat("u_aspect_ratio", aspectRatio);

        // move Object =========================== MODEL matrix ================
        // move back
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        // rotate X
        // model *= glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));

        // move camera (not move) ================ VIEW matrix ==================
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

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
      
     
        ourShader.setMatrix4fv("model", model);
        ourShader.setMatrix4fv("view", view);
        ourShader.setMatrix4fv("proj", proj);

        ourShader.setVec3("light.position", glm::vec3(1.0f, 5.0f, 5.0f));

        ourShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f)); // Низкая интенсивность
        ourShader.setVec3("light.diffuse", glm::vec3(0.8f, 0.8f, 0.8f)); // Средняя
        ourShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f)); // Полная
        // Передача данных в шейдер материал
        ourShader.setVec3("material.ambient", glm::vec3(0.8f, 0.5f, 0.5f));
        ourShader.setVec3("material.diffuse", glm::vec3(0.8f, 0.7f, 0.6f));
        ourShader.setVec3("material.specular", glm::vec3(0.7f, 0.7f, 0.7f));
        ourShader.setFloat("material.shininess", 64.0f);

        ourShader.setVec3("viewPos", cameraPos);

        //glm::vec3 lightColor = glm::vec3(0.5f, 0.5f, 0.5f);        
        // ourShader.setVec3("lightColor", lightColor);


        loadedModel.Draw(ourShader);

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


}








// include "glm/glm.hpp"
// include "glm/gtc/matrix_transform.hpp"
// include "glm/gtc/type_ptr.hpp"




