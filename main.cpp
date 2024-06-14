#include "./simulator.cpp"
#include "./verletobj.cpp"
#include <random>
#include <raylib.h>

const int WIDTH = 1000;
const int HEIGHT = 1000;

float gen_random_float(float min, float max) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<float> dist(min, max);

    return dist(rng);
}

float map(const float oldValue, const float oldMin, const float oldMax,
          const float newMin, const float newMax) {
    const float oldRange = (oldMax - oldMin);
    const float newRange = (newMax - newMin);

    const float newValue =
        (((oldValue - oldMin) * newRange) / oldRange) + newMin;

    return newValue;
}

int main() {
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "Verlet Physics Engine");

    SetTargetFPS(60);

    Simulator simulator;
    simulator.objs = {};

    // simulator.objs.push_back(Verlet({WIDTH / 2.f, HEIGHT / 2.f}));

    float dt = 0.01f;
    int delay = 0;
    int totalObj = 0;
    float angle = 0.0;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GRAY);

        if (totalObj < 500) {
            if (delay == 0) {
                const float mouseX = (WIDTH / 2.f - sin(angle) * 100.f);
                const float mouseY = (HEIGHT / 2.f - 200.f);

                // const float mouseX = GetMouseX();
                // const float mouseY = GetMouseY();

                const float rand = gen_random_float(5.f, 12.f);

                const float hue = map(rand, 5.f, 15.f, 0.f, 360.f);
                const Color color = ColorFromHSV(hue, 0.4f, 1.f);

                simulator.objs.push_back(Verlet({mouseX, mouseY}, rand, color));

                delay = 1;
                totalObj++;
                angle += 0.75;
            } else {
                delay--;
            }
        }

        {
            DrawCircle(WIDTH / 2, HEIGHT / 2, 300.f, BLACK);

            for (const Verlet &obj : simulator.objs) {
                DrawCircleV(obj.curPosition, obj.radius, obj.color);
            }
        }

        EndDrawing();

        simulator.update(dt);
    }

    CloseWindow();
}
