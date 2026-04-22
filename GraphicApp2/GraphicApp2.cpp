
#define GLEW_DLL
#define GLFW_DLL


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

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


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const float aspectRatio = (float)SCR_HEIGHT / (float)SCR_WIDTH;

// ============================ glm
// vectom glog coord  // this  global center coord
// glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    // move came delta
// const float cameraSpeed = 0.05f;
float cuttrentFrame, deltaTime, lastFrame;
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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Mainwindow", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;


    GLenum ret = glewInit();
    if (GLEW_OK != ret) {
        fprintf(stderr, "ERROR: %s \n", glewGetErrorString(ret));
        return 1;
    }

    GLuint VBO, VAO, EBO;
    
	// vertex array object объект массива вершин
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
	// init VBO vertex buffer object
    glGenBuffers(1, &VBO);    
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //  vbo
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// configure vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    GLuint indicess[] = { 0, 1, 2,    1, 2, 3 };

    // init EBO element buffer object
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // 
    // configure element attributes
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicess), indicess, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
       
    // create shader
    Shader ourShader("shaders/shader.vert", "shaders/shader.frag");

    // bound cursore to window and height + hanler mouse call
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);


    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        // move Object =========================== MODEL matrix ================
        // move back
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.2f));
        // rotate X
        model *= glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));

        // move camera (not move) ================ VIEW matrix ==================
        // glm::mat4 view = glm::mat4(1.f);
          cuttrentFrame = (float)glfwGetTime();
          deltaTime = cuttrentFrame - lastFrame;
          lastFrame = cuttrentFrame;
          const float cameraSpeed = 3 * deltaTime;

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

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        
        // proj init perspective ================== PROJECTION on CAMERA ==========
        // матрица проекции вида
        glm::mat4 proj = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);


		// render background color
        glClearColor(0.2f, 0.2f, 0.6f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        ourShader.setFloat("u_aspect_ratio", aspectRatio);

        GLfloat timeValue = static_cast<GLfloat>(glfwGetTime());
        GLfloat greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        GLfloat redValue = (cos(timeValue) / 2.0f) + 0.5f;

        ourShader.setFloat4("frag_colour", redValue, greenValue, 0.2f, 1.0f);    

        ourShader.setMatrix4fv("model", model);
        ourShader.setMatrix4fv("view", view);
        ourShader.setMatrix4fv("proj", proj);


        glBindVertexArray(VAO); 

        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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




