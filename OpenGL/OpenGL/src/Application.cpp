#include <iostream>
#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>


struct ShaderScource
{
    const std::string vertexShader;
    const std::string fragmentShader;
};

enum class shaderType {
    defaultValue = -1,
    vertexShader,
    fragmentShader
};

static ShaderScource parseShaderFile(const std::string &filePath) {
    std::ifstream in(filePath);
    if (!in.is_open()) {
        std::cout << "ERROR!!!!! open file:%s, failed!!" << filePath << std::endl;
        return {"",""};
    }

    shaderType type = shaderType::defaultValue;
    std::string vertexSource;
    std::string fragmentSource;
    std::string line;
    while (getline(in, line))
    {
        if (line.find("vertexShader") != line.npos) {
            type = shaderType::vertexShader;
        }
        else if (line.find("fragmentShader") != line.npos) {
            type = shaderType::fragmentShader;
        }
        else {
            if (type == shaderType::vertexShader) {
                vertexSource.append(line + "\n");
            }
            if (type == shaderType::fragmentShader) {
                fragmentSource.append(line + "\n");
            }
        }
    }
    return { vertexSource , fragmentSource };
};


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
        std::cout << "compile shader:%s failed!! " << (type == GL_VERTEX_SHADER ? "vertexShader" : "fragmentShader") << std::endl;
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

    auto shaderSource = parseShaderFile("res/shaders/triangle.shader");

    std::cout << "opengl version is " << glGetString(GL_VERSION) << std::endl;

    float vertices[] = {
        //position              //color rgb
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,
         0.5f, 0.5f, 0.0f,      0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.0f,      0.0f, 1.0f, 1.0f
    };

    unsigned int indices[] = {
        0,1,2,
        0,2,4
    };
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indices, GL_STATIC_DRAW);
    
    unsigned int pragram = createShader(shaderSource.vertexShader, shaderSource.fragmentShader);
    glUseProgram(pragram);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        

        float timeValue = glfwGetTime();
        float value = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(pragram, "ourColor");
        glUniform4f(vertexColorLocation,value, value*value, -value*value, 1.0f);

        glBindVertexArray(vbo);
        /* Render here */
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteProgram(pragram);

    glfwTerminate();
    return 0;
}