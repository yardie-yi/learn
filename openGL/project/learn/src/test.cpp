// #include <GL/glew.h>
//#include <GL/gl.h>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 640, 480);
    // GLenum err = glewInit();
    // if (GLEW_OK != err)
    // {
    //     printf("glewInit err %s\n", glewGetErrorString(err));
    // }

    //std::cout << glGetString(GL_VERSION) << std::endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // glBegin(GL_TRIANGLES);

        // glVertex2d(-0.5f, -0.5f);
        // glVertex2d(0.0f, 0.5f);
        // glVertex2d(0.5f, -0.5f);

        // glEnd();

        processInput(window);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
