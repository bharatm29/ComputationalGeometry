#include <cmath>
#include <cstdlib>
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

    float sliderA = -2.f;
    float sliderB = 2.f;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_MINUS)) {
            sliderA -= .25f;
            sliderB += .25f;
        }

        if (IsKeyDown(KEY_LEFT_SHIFT) &&
            IsKeyPressed(KEY_EQUAL)) { // SHIFT = is +
            sliderA += .25f;
            sliderB -= .25f;
        }

        BeginDrawing();

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

                    float brightness =
                        mapRange(counter, 0.f, ITERATIONS, 0.f, 255);

                    if (counter == ITERATIONS) {
                        brightness = 0;
                    }

                    const Color color = {(unsigned char)brightness, (unsigned char)brightness,
                                         (unsigned char)brightness, 255};

                    DrawPixel(x, y, color);
                }
            }
        }

        EndDrawing();
    }

    CloseWindow();
}
