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
        ClearBackground(GetColor(0x403d3d00));

        DrawText("Fourier Series", (WIDTH / 2.0) - (MeasureText("Fourier Series", 30) / 2.0), 100, 30, WHITE);

        double rad = 100.0 * (4 / (1 * PI)); // radius of the big circle

        { // drawing coordinate plane
            #if 1
            float cartx = centerx + wave_translate;
            float carty = centery + rad;

            DrawLine(cartx, carty, cartx, centery - rad, WHITE);
            DrawLine(cartx, carty, WIDTH, carty, WHITE);

            DrawTriangle({.x = cartx, .y = (float)(centery - rad)}, {.x = cartx - 5, .y = (float)(centery - rad) + 10}, {.x = cartx + 5, .y = (float)(centery - rad) + 10}, WHITE);
            DrawTriangle({.x = WIDTH, .y = carty}, {.x = WIDTH - 10, .y = carty - 5}, {.x = WIDTH - 10, .y = carty + 5}, WHITE);
            #endif
        }

        double x = centerx;
        double y = centery;

        for (int i = 0; i < 5; i++) {
            const int n = i * 2 + 1;
            double prevx = x;
            double prevy = y;

            rad = 100.0 * (4 / (n * PI));
            x += rad * cos(n * angle);
            y += rad * sin(n * angle);

            DrawCircleLines(prevx, prevy, rad, GetColor(0x80808080));
            DrawLine(prevx, prevy, x, y, BLUE);
            DrawCircle(x, y, 2, WHITE);
        }

        float pointx = centerx + wave_translate;
        float pointy = coords.front();
        DrawLine(x, y, pointx, pointy, ORANGE);
        // DrawCircle(centerx + wave_translate, coords.front(), 2, WHITE);
        DrawTriangle({.x = pointx, .y = pointy}, {.x = pointx - 10, .y = pointy - 5}, {.x = pointx - 10, .y = pointy + 5}, WHITE);

        coords.push_front(y);

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

        if (coords.size() > 500) { // just remove out of window points
            coords.pop_back();
        }

        EndDrawing();
    }

    CloseWindow();
}
