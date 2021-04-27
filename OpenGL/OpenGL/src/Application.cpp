#include <iostream>
#include<GL/glew.h>
#include <GLFW/glfw3.h>

const char* vertexShaderSource1 = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fregrementShaderSource1 = "#version 330 core\n"
"layout (location = 0) out vec4 color;\n"
"void main()\n"
"{\n"
"   color = vec4(1.0, 0.2, 0.0, 0.0);\n"
"}\0";


static unsigned int compileShader(unsigned int type, const std::string &source) {
    
    unsigned int shader = glCreateShader(type);
    const char* p = source.c_str();
    glShaderSource(shader, 1, &p, NULL);
    glCompileShader(shader);
    GLint result;
    glGetProgramiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(shader, 1024, &log_length, message);
        std::cout << "compile shader:%s failed!! " << (type == GL_VERTEX_SHADER ? "vertexShader" : "fregrementShader") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;

};

static unsigned int createShader(const std::string &vertextShaderSource, const std::string &fregramentShaderScource) {

    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertextShaderSource);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fregramentShaderScource);
    glAttachShader(program, vs);
    glAttachShader(program, fs);    
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
};



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

    if (glewInit() != GLEW_OK) {
        return -1;
    }

    std::cout << "opengl version is " << glGetString(GL_VERSION) << std::endl;

    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3,0);
    
    unsigned int pragram = createShader(vertexShaderSource1, fregrementShaderSource1);
    glUseProgram(pragram);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteProgram(pragram);

    glfwTerminate();
    return 0;
}