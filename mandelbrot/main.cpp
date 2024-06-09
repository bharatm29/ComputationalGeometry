#include <raylib.h>
#include <raymath.h>

const int WIDTH = 800;
const int HEIGHT = 800;

const int ITERATIONS = 150;

int main() {
    InitWindow(WIDTH, HEIGHT, "MandelBrot Set");

    SetTargetFPS(60);

    // Zoomed in areas: [.4, .35], [-.55, -.5]
    float minRange = -2.f;  // should be negative
    float maxRange = 1.45f; // should be positive

    Shader shader = LoadShader(0, "shader.glsl");

    int resolutionLoc = GetShaderLocation(shader, "resolution");
    int minRangeLoc = GetShaderLocation(shader, "minRange");
    int maxRangeLoc = GetShaderLocation(shader, "maxRange");
    int iterationLoc = GetShaderLocation(shader, "iterations");

    float res[2] = {WIDTH, HEIGHT};

    SetShaderValue(shader, resolutionLoc, res, SHADER_UNIFORM_VEC2);
    SetShaderValue(shader, iterationLoc, (const void *)&ITERATIONS,
                   SHADER_UNIFORM_INT);
    SetShaderValue(shader, minRangeLoc, (const void *)&minRange,
                   SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, maxRangeLoc, (const void *)&maxRange,
                   SHADER_UNIFORM_FLOAT);

    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(BLACK);

        BeginShaderMode(shader);
        DrawRectangle(0, 0, WIDTH, HEIGHT, BLACK);
        EndShaderMode();

        EndDrawing();

        if (IsKeyPressed(KEY_R)) {
            TakeScreenshot("mandelbrot.png");
        }
    }

    UnloadShader(shader);
    CloseWindow();
}
