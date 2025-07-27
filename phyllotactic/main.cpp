#include <cmath>
#include <raylib.h>
#include <raymath.h>
#include <vector>

using namespace std;

const int WIDTH = 1000;
const int HEIGHT = 1000;

int main() {
    InitWindow(WIDTH, HEIGHT, "Phyllotactic");

    SetTargetFPS(60);

    int c = 4;
    /// int n = 0;
    vector<pair<int, pair<int, int>>> points;

    for (int n = 0; n < 1000; n++) {
        double phi = n * 137.5;
        double rad = c * sqrt(n);

        double x = (WIDTH / 2.0) + rad * cos(phi * PI / 180);
        double y = (HEIGHT / 2.0) + rad * sin(phi * PI / 180);
        points.push_back({n, {x, y}});
    }

    Camera2D camera = { 0 };
    camera.target = (Vector2){ WIDTH / 2., HEIGHT / 2. };
    camera.offset = (Vector2){ WIDTH / 2., HEIGHT / 2. };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(0x403d3d00));
        BeginMode2D(camera);

        DrawText("Planar Model", (WIDTH / 2.0) - (MeasureText("Planar Model", 30) / 2.0), 100, 30, WHITE);

        camera.zoom = expf(logf(camera.zoom) + ((float)GetMouseWheelMove()*0.1f));
        // camera.target = GetMousePosition();

        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        else if (camera.zoom < 1.f) camera.zoom = 1.f;


        for (const auto [k, p] : points) {
            DrawCircle(p.first, p.second, 2, WHITE);
        }

        EndMode2D();
        EndDrawing();
    }

    CloseWindow();
}
