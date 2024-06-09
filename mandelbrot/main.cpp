#include <cmath>
#include <iostream>
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

    RenderTexture2D screen = LoadRenderTexture(WIDTH, HEIGHT);

    Camera2D camera = {0};
    camera.zoom = 1.0f;

    int zoomMode = 0; // 0-Mouse Wheel, 1-Mouse Move

    Shader shader = LoadShader(0, "shader.glsl");

    int resolutionLoc = GetShaderLocation(shader, "resolution");
    int minRangeLoc = GetShaderLocation(shader, "minRange");
    int maxRangeLoc = GetShaderLocation(shader, "maxRange");
    int iterationLoc = GetShaderLocation(shader, "iterations");

    float res[2] = {WIDTH, HEIGHT};

    SetShaderValue(shader, resolutionLoc, res, SHADER_UNIFORM_VEC2);
    SetShaderValue(shader, iterationLoc, (const void *)&ITERATIONS,
                   SHADER_UNIFORM_INT);

    while (!WindowShouldClose()) {
        {
            if (IsKeyPressed(KEY_ONE))
                zoomMode = 0;
            else if (IsKeyPressed(KEY_TWO))
                zoomMode = 1;

            // Translate based on mouse right click
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                Vector2 delta = GetMouseDelta();
                delta = Vector2Scale(delta, -1.0f / camera.zoom);
                camera.target = Vector2Add(camera.target, delta);
            }

            if (zoomMode == 0) {
                // Zoom based on mouse wheel
                float wheel = GetMouseWheelMove();
                if (wheel != 0) {
                    // Get the world point that is under the mouse
                    Vector2 mouseWorldPos =
                        GetScreenToWorld2D(GetMousePosition(), camera);

                    // Set the offset to where the mouse is
                    camera.offset = GetMousePosition();

                    // Set the target to match, so that the camera maps the
                    // world space point under the cursor to the screen space
                    // point under the cursor at any zoom
                    camera.target = mouseWorldPos;

                    // Zoom increment
                    float scaleFactor = 1.0f + (0.25f * fabsf(wheel));
                    if (wheel < 0)
                        scaleFactor = 1.0f / scaleFactor;
                    camera.zoom =
                        Clamp(camera.zoom * scaleFactor, 0.125f, 64.0f);
                }
            } else {
                // Zoom based on mouse left click
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    // Get the world point that is under the mouse
                    Vector2 mouseWorldPos =
                        GetScreenToWorld2D(GetMousePosition(), camera);

                    // Set the offset to where the mouse is
                    camera.offset = GetMousePosition();

                    // Set the target to match, so that the camera maps the
                    // world space point under the cursor to the screen space
                    // point under the cursor at any zoom
                    camera.target = mouseWorldPos;
                }
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    // Zoom increment
                    float deltaX = GetMouseDelta().x;
                    float scaleFactor = 1.0f + (0.01f * fabsf(deltaX));
                    if (deltaX < 0)
                        scaleFactor = 1.0f / scaleFactor;
                    camera.zoom =
                        Clamp(camera.zoom * scaleFactor, 0.125f, 64.0f);
                }
            }
        }

        SetShaderValue(shader, minRangeLoc, (const void *)&minRange,
                       SHADER_UNIFORM_FLOAT);
        SetShaderValue(shader, maxRangeLoc, (const void *)&maxRange,
                       SHADER_UNIFORM_FLOAT);

        BeginDrawing();
        ClearBackground(BLACK);

        BeginTextureMode(screen);
        DrawRectangle(0, 0, WIDTH, HEIGHT, BLACK);
        EndTextureMode();

        BeginMode2D(camera);

        BeginShaderMode(shader);
        DrawTexture(screen.texture, 0, 0, WHITE);
        EndShaderMode();

        EndMode2D();
        EndDrawing();

        if (IsKeyPressed(KEY_R)) {
            TakeScreenshot("mandelbrot.png");
        }
    }

    UnloadShader(shader);
    UnloadRenderTexture(screen);
    CloseWindow();
}
