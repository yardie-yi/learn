//#include <GL/glew.h>
//#include <GL/gl.h>
//#include <GL/gl.h>
//#include <GL/glext.h>
//#include <GL/glext.h>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define IMG_PATH1 "/home/yardie/Desktop/learn/openGL/res/container.jpg"
#define IMG_PATH2 "/home/yardie/Desktop/learn/openGL/res/awesomeface.png"


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

  
    float vertices[] = {
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


  //texture1
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(IMG_PATH1, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //texture2
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(IMG_PATH2, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW);


    //vertexShader
    const char *vertexShaderSoure =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "out vec3 ourColor;\n"
    "out vec2 TexCoord;\n"
    "uniform mat4 transform;\n"
    "void main()\n"
    "{\n"
    //"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
    //"   ourColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
    "   ourColor = aColor;\n"
    "   TexCoord = aTexCoord;\n"
    //"   TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);\n"
    "}\n";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSoure, NULL);
    glCompileShader(vertexShader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "vertexShader error : " << infoLog << std::endl;
    }

    //fragmentShader
    const char *fragmentShaderSoure =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D ourTexture1;\n"
    "uniform sampler2D ourTexture2;\n"
    //"uniform vec4 ourColor;"
    "void main()\n"
    "{\n"
    //"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    //"   FragColor = vec4(ourColor, 1.0);\n"
    //"   FragColor = ourColor;\n"
    "   FragColor = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2);\n"
    "}\n";


    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSoure, NULL);
    glCompileShader(fragmentShader);
    
    memset(infoLog, '\0', sizeof(infoLog));
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "fragmentShader error : " << infoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    memset(infoLog, '\0', sizeof(infoLog));
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "shaderProgram error : " << infoLog << std::endl;
    }

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture2"), 1);



    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // glBegin(GL_TRIANGLES);

        // glVertex2d(-0.5f, -0.5f);
        // glVertex2d(0.0f, 0.5f);
        // glVertex2d(0.5f, -0.5f);

        // glEnd();

        // glUseProgram(shaderProgram);
        // float timeValue = glfwGetTime();
        // float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        // int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        //glm
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
        trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

        glUseProgram(shaderProgram);
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    

    glfwTerminate();
    return 0;
}
