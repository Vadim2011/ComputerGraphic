
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

	// vertex buffer object
    glGenBuffers(1, &VBO);    
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //  vbo
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// configure vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    GLuint indicess[] = { 0, 1, 2,    1, 2, 3 };

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicess), indicess, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
       

    Shader ourShader("shaders/shader.vert", "shaders/shader.frag");


    while (!glfwWindowShouldClose(window)) {

		// render background color
        glClearColor(0.2f, 0.2f, 0.6f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        ourShader.setFloat("u_aspect_ratio", aspectRatio);

        GLfloat timeValue = static_cast<GLfloat>(glfwGetTime());
        GLfloat greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        GLfloat redValue = (cos(timeValue) / 2.0f) + 0.5f;

        ourShader.setFloat4("frag_colour", redValue, greenValue, 0.2f, 1.0f);           

        glBindVertexArray(VAO); 

        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();

    return 0;
}

