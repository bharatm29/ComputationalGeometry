#include "./include/glad/glad.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <iostream>

const int WIDTH  = 640;
const int HEIGHT = 480;

SDL_Window *gGraphicsAppWindow = nullptr;
SDL_GLContext gOpenGContext    = nullptr;

bool gQuit = false;

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

    std::cout << glGetString(GL_VERSION) << std::endl;
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

void PreDraw() {}
void Draw() {}

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
    MainLoop();
    CleanUp();
    return 0;
}
