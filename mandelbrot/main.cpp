#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <raylib.h>

const int WIDTH = 600;
const int HEIGHT = 600;

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
    float sliderA = -2.f; // should be negative
    float sliderB = 1.45f; // should be positive

    while (!WindowShouldClose()) {
        //H => decrease sliderA
        if (IsKeyPressed(KEY_H)) {
            sliderA -= .05f;
        }

        //K => decrease sliderB
        if (IsKeyPressed(KEY_K)) {
            sliderB -= .05f;
        }

        //J => increase sliderA
        if (IsKeyPressed(KEY_J)) {
            sliderA += .05f;
        }
        //L => increase sliderB
        if (IsKeyPressed(KEY_L)) {
            sliderB -= .05f;
        }

        BeginDrawing();
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
                            color = Color{(unsigned char)c, 255, (unsigned char)c, 255};
                        } else {
                            // Far away it changes from black to green
                            // color = GetColor(floatToHex(0, c, 0));
                            color = Color{0,(unsigned char)c, 0, 255};
                        }
                    }

                    DrawPixel(x, y, color);
                }
            }
        }

        EndDrawing();

        if (IsKeyPressed(KEY_R)) {
            TakeScreenshot("mandelbrot.png");
        }
    }

    CloseWindow();
}
