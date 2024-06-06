#include <raylib.h>
#include <raymath.h>
#include <vector>
using std::vector;

const int WIDTH = 1200;
const int HEIGHT = WIDTH;
const int ITERATIONS = 10;

bool operator==(const Vector2 &v1, const Vector2 &v2) {
    return v1.x == v2.x && v1.y == v2.y;
}

Vector2 Vector2MidPoint(Vector2 v1, Vector2 v2) {
    Vector2 vec = Vector2Add(v1, v2);
    vec = Vector2Scale(vec, .5f);
    return vec;
}

int main() {
    InitWindow(WIDTH, HEIGHT, "IFS");

    SetTargetFPS(60);

    vector<Vector2> initialPoints = {
        Vector2{WIDTH / 2.0, 50.0},
        Vector2{50, HEIGHT - 50.0},
        Vector2{WIDTH - 50, HEIGHT - 50.0},
    };

    vector<Vector2> next;
    next.assign(initialPoints.begin(), initialPoints.end());

    vector<Vector2> points;
    points.assign(initialPoints.begin(), initialPoints.end());

    for (int i(0); i < ITERATIONS; i++) {
        vector<Vector2> next_points;

        for (const Vector2 p : next) {
            points.push_back(p);

            for (const Vector2 initPoints : initialPoints) {
                Vector2 vec = Vector2MidPoint(p, initPoints);

                next_points.push_back(vec);
            }
        }

        next.assign(next_points.begin(), next_points.end());
    }

    next.clear();
    initialPoints.clear();

    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(BLACK);

            for (const Vector2 p : points) {
                DrawCircleV(p, 1., RED);
            }
        }
        EndDrawing();
    }

    CloseWindow();
}
