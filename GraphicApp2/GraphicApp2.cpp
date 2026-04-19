
#define GLEW_DLL
#define GLFW_DLL


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


int main()
{
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(512, 512, "Mainwindow", NULL, NULL);

    if (!window) {
       glfwTerminate();
       return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;


    GLenum ret = glewInit();
    if (GLEW_OK != ret) {
        fprintf(stderr, "ERROR: %S \n", glewGetErrorString(ret));
        return 1;
    }

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.5f, 0.2f, 0.7f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(1.0f, 1.0f, 1.0f);

        glBegin(GL_POLYGON);

        glVertex2f(-0.5f, -0.5f);
        glVertex2f(-0.8f, 0.3f);
        glVertex2f(0.0f, 0.8f);
        glVertex2f(0.8f, 0.3f);
        glVertex2f(0.5f, -0.5f);

        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();

    return 0;
}

