#include <list>
#include <raylib.h>
#include <raymath.h>
#include <vector>

using namespace std;

const int WIDTH = 1000;
const int HEIGHT = 1000;

const int ITERATIONS = 150;

int main() {
    InitWindow(WIDTH, HEIGHT, "Fourier Series");

    SetTargetFPS(60);

    double wave_translate = 300.0;

    double centerx = WIDTH / 4.25;
    double centery = HEIGHT / 2.0;

    double angle = 0.01;

    list<double> coords;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        double x = centerx;
        double y = centery;

        for (int i = 0; i < 100; i++) {
            const int n = i * 2 + 1;
            double prevx = x;
            double prevy = y;

            double rad = 100.0 * (4 / (n * PI));
            x += rad * cos(n * angle);
            y += rad * sin(n * angle);

            DrawCircleLines(prevx, prevy, rad, GetColor(0x80808080));
            DrawLine(prevx, prevy, x, y, BLUE);
        }

        DrawLine(x, y, centerx + wave_translate, coords.front(), YELLOW);

        coords.push_front(y);

        EndDrawing();

        angle += 0.01;
        if (angle > 360.0) {
            angle = 0.01;
        }

        int xx = 0;

        vector<Vector2> points;
        for (const auto yy : coords) {
            float nx = xx + centerx + wave_translate;
            points.push_back(Vector2{.x = nx, .y = (float)yy});
            // DrawPixel(xx + centerx + wave_translate, yy, RED);
            xx++;
        }

        DrawLineStrip(points.data(), points.size(), RED);

        if (coords.size() > 500) {
            coords.pop_back();
        }
    }

    CloseWindow();
}
