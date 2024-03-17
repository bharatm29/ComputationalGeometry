/*
 * Explanation of the entire process:
 * https://www.youtube.com/watch?v=eYMAR5bW8eo&list=PLvv0ScY6vfd9zlZkIIqGDeG5TUWswkMox&index=13
 * */

#include "./include/glad/glad.h" // this is a handy header file which provides us opengl functions
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <fstream>
#include <iostream>
#include <vector>

const int WIDTH  = 640;
const int HEIGHT = 480;

SDL_Window *gGraphicsAppWindow = nullptr;
SDL_GLContext gOpenGContext    = nullptr;

bool gQuit = false;

// Objects are usually represented by an GLuint in OpenGL
GLuint gVertexArrayObject = 0; // Represents our VAO

GLuint gVertexBufferObject      = 0; // Represents our VBO for vertices position
GLuint gVertexBufferColorObject = 0; // Represents our VBO for colors

// Program Object (for our shaders) -> Our graphic pipeline. Something that has
// a pipeline to compile our Vertex and Fragment shader
// Program == Graphics pipeline
GLuint gGraphicsPipelineShaderProgram = 0;

void error(const std::string message) {
    std::cerr << "[ERROR] " + message << std::endl;
    exit(1);
}

void InitializeProgram() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) { // init returns -1 if there was an error
        error("Could not init SDL");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE); // Disables old functions

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Allow smooth transitions
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);  // More detailed(?)

    gGraphicsAppWindow = SDL_CreateWindow("First Program", 0, 0, WIDTH, HEIGHT,
                                          SDL_WINDOW_OPENGL);

    if (!gGraphicsAppWindow) {
        error("Could not create SDL_Window");
    }

    gOpenGContext = SDL_GL_CreateContext(gGraphicsAppWindow);

    if (!gOpenGContext) {
        error("Could not create OpenGL context");
    }

    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        error("Could not init glad");
    }

    // std::cout << glGetString(GL_VERSION) << std::endl;
}

void VertexSpecification() {
    // Lives on our CPU. We have to make it live on the GPU
    const std::vector<GLfloat> vertexPosition{
        // x y z
        -0.8f, -0.8f, 0.0f, // vertex 1
        1.0f,  0.0f,  0.0f, // vertex color 1

        0.8f,  -0.8f, 0.0f, // vertex 2
        0.0f,  0.0f,  1.0f, // vertex color 2

        0.0f,  0.8f,  0.0f, // vertex 3
        0.0f,  1.0f,  0.0f, // vertex color 3
    };

    // To set this up on our GPU we have to setup Vertex Array and Buffer Object
    // (VAO and VBO). VBO will actually contain all this data for us and VAO
    // will determine how the vertex is processed

    glGenVertexArrays(
        1,
        &gVertexArrayObject); // the gVertexArrayObject will represent out VAO
    glBindVertexArray(gVertexArrayObject);

    // Now we will generate our VBOs
    glGenBuffers(2, &gVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertexPosition.size() * sizeof(GLfloat),
                 vertexPosition.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // which attribute we wanna access
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        sizeof(GLfloat) * 6, // The stride is how many we have to jump to reach
                             // the next starting index, in this case it is 6
        (void *)0);
    // After previous two commands, out VAO knows how to work with VBO

    // linking and working with color VBO
    glEnableVertexAttribArray(
        1); // which attribute we wanna access. 1 Represents our color VBO
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6,
                          (GLvoid *)(sizeof(GLfloat) * 3));

    // Following is clean up code
    glBindVertexArray(0); // previously we bind to gVertexArrayObject now we
                          // don't want to bind to anything
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

std::string loadShaderAsString(const std::string &filename) {
    std::string result{""}, line{""};

    std::ifstream file(filename.c_str());

    if (file.is_open()) {
        while (getline(file, line)) {
            result += line + '\n';
        }

        file.close();
    }

    return result;
}

GLuint compileShader(GLuint type, const std::string &shaderSrc) {
    GLuint shaderObject = 0;

    if (type == GL_VERTEX_SHADER) {
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    } else if (type == GL_FRAGMENT_SHADER) {
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }

    const char *src_c_str = shaderSrc.c_str();
    glShaderSource(shaderObject, 1, &src_c_str, nullptr);
    glCompileShader(shaderObject);

    return shaderObject;
}

GLuint createShaderProgram(const std::string &vertexShaderSrc,
                           const std::string &fragmentShaderSrc) {
    // Program == Graphics pipeline
    GLuint programObject = glCreateProgram();

    // compileShader() compiles the shader from src string and then we will
    // attack the shader and link the program
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSrc);
    GLuint fragmentShader =
        compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    glLinkProgram(programObject);

    // validate program for any potential errors
    glValidateProgram(programObject);

    return programObject;
}

void CreateGraphicsPipeline() {
    const std::string vertexShaderSource =
        loadShaderAsString("./shaders/vert.glsl");
    const std::string fragmentShaderSource =
        loadShaderAsString("./shaders/frag.glsl");

    gGraphicsPipelineShaderProgram =
        createShaderProgram(vertexShaderSource, fragmentShaderSource);
}

void Input() {
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            std::cout << "Goodbyte" << std::endl;
            gQuit = true;
        }
    }
}

// responsible for settings OpenGl state
void PreDraw() {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(1.f, 1.f, 0.f, 1.f);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(gGraphicsPipelineShaderProgram); // activate the graphic
                                                  // pipeline, now we will draw
}

void Draw() {
    // first set what VAO and VBO we will be using
    glBindVertexArray(gVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);

    glDrawArrays(GL_TRIANGLES, 0, 3); // draw triangles. We start from 0 and
                                      // draw 3 triangles (we gave 3 vertices)
}

void MainLoop() {
    while (!gQuit) {
        Input();

        PreDraw();

        Draw();

        // Since we are using Double buffers, this will make the window we drawn
        // to the main window
        // In short, this will update the window
        SDL_GL_SwapWindow(gGraphicsAppWindow);
    }
}

void CleanUp() {
    SDL_DestroyWindow(gGraphicsAppWindow);

    SDL_Quit();
}

int main() {
    InitializeProgram();

    VertexSpecification(); // Loads the vertex info into the gpu or setup the
                           // geometry

    CreateGraphicsPipeline(); // compiles shader source code to our gpu for us
                              // to use

    MainLoop();

    CleanUp();
}
