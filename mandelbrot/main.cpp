#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <raylib.h>
#include <raymath.h>

const int WIDTH = 800;
const int HEIGHT = 800;

const int ITERATIONS = 150;

float mapRange(const float oldValue, const float oldMin, const float oldMax,
               const float newMin, const float newMax) {
    const float oldRange = (oldMax - oldMin);
    const float newRange = (newMax - newMin);

    const float newValue =
        (((oldValue - oldMin) * newRange) / oldRange) + newMin;

    return newValue;
}

int main() {
    InitWindow(WIDTH, HEIGHT, "MandelBrot Set");

    SetTargetFPS(60);

    // Zoomed in areas: [.4, .35], [-.55, -.5]
    float sliderA = -2.f;  // should be negative
    float sliderB = 1.45f; // should be positive

    RenderTexture2D screen = LoadRenderTexture(WIDTH, HEIGHT);

    Camera2D camera = { 0 };
    camera.zoom = 1.0f;

    int zoomMode = 0;   // 0-Mouse Wheel, 1-Mouse Move

        BeginTextureMode(screen);
        ClearBackground(BLACK);
        {
            for (int x = 0; x <= WIDTH; x++) {
                for (int y = 0; y <= HEIGHT; y++) {
                    float a = mapRange(x, 0, WIDTH, sliderA, sliderB);
                    float b = mapRange(y, 0, HEIGHT, sliderA, sliderB);

                    const float ca = a;
                    const float cb = b;

                    float counter{0};

                    while (counter < ITERATIONS) {
                        const float aa = a * a - b * b;
                        const float bb = 2 * a * b;

                        a = aa + ca;
                        b = bb + cb;

                        counter++;

                        if (std::abs(a + b) > 16) { // assuming 16 as infinity
                            break;
                        }
                    }

                    Color color = BLACK;

                    if (counter < ITERATIONS) {
                        double quotient = (double)counter / (double)ITERATIONS;
                        double c = std::clamp<double>(quotient, 0.f, 1.f);

                        c = mapRange(c, .0f, 1.f, 0, 255);

                        if (quotient > 0.5) {
                            // Close to the mandelbrot set the color changes
                            // from green to white
                            color = Color{(unsigned char)c, 255,
                                          (unsigned char)c, 255};
                        } else {
                            // Far away it changes from black to green
                            // color = GetColor(floatToHex(0, c, 0));
                            color = Color{0, (unsigned char)c, 0, 255};
                        }
                    }

                    DrawPixel(x, y, color);
                }
            }
        }
        EndTextureMode();

    while (!WindowShouldClose()) {
        {
        if (IsKeyPressed(KEY_ONE)) zoomMode = 0;
        else if (IsKeyPressed(KEY_TWO)) zoomMode = 1;

        // Translate based on mouse right click
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f/camera.zoom);
            camera.target = Vector2Add(camera.target, delta);
        }

        if (zoomMode == 0)
        {
            // Zoom based on mouse wheel
            float wheel = GetMouseWheelMove();
            if (wheel != 0)
            {
                // Get the world point that is under the mouse
                Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

                // Set the offset to where the mouse is
                camera.offset = GetMousePosition();

                // Set the target to match, so that the camera maps the world space point 
                // under the cursor to the screen space point under the cursor at any zoom
                camera.target = mouseWorldPos;

                // Zoom increment
                float scaleFactor = 1.0f + (0.25f*fabsf(wheel));
                if (wheel < 0) scaleFactor = 1.0f/scaleFactor;
                camera.zoom = Clamp(camera.zoom*scaleFactor, 0.125f, 64.0f);
            }
        }
        else
        {
            // Zoom based on mouse left click
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                // Get the world point that is under the mouse
                Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

                // Set the offset to where the mouse is
                camera.offset = GetMousePosition();

                // Set the target to match, so that the camera maps the world space point 
                // under the cursor to the screen space point under the cursor at any zoom
                camera.target = mouseWorldPos;
            }
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                // Zoom increment
                float deltaX = GetMouseDelta().x;
                float scaleFactor = 1.0f + (0.01f*fabsf(deltaX));
                if (deltaX < 0) scaleFactor = 1.0f/scaleFactor;
                camera.zoom = Clamp(camera.zoom*scaleFactor, 0.125f, 64.0f);
            }
        }
        }


        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);

        DrawTexture(screen.texture, 0, 0, WHITE);

        // Draw a reference circle
        // DrawCircle(GetScreenWidth()/2, GetScreenHeight()/2, 50, MAROON);

        EndMode2D();
        EndDrawing();

        if (IsKeyPressed(KEY_R)) {
            TakeScreenshot("mandelbrot.png");
        }
    }

    CloseWindow();
}
