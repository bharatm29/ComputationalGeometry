#include "./transformation.cpp"
#include <raylib.h>
#include <vector>

using std::vector;

const int WIDTH = 800;
const int HEIGHT = 800;

const Fractals curFractal = TREE;

int main() {
    InitWindow(WIDTH, HEIGHT, "IFSs");

    SetTargetFPS(60);

    vector<Vector2> points;

    Vector2 point = {0, 0};
    points.push_back(point);

    float minX = 100000.0f, minY = 100000.0f;
    float maxX = -100000.0f, maxY = -100000.0f;

    for (int _ = 0; _ < getIterations(curFractal); ++_) {
        point = applyTransformation(curFractal, point);
        // std::cout<<point.x<<" "<<point.y<<std::endl;

        points.push_back(point);

        minX = std::min(minX, point.x);
        minY = std::min(minY, point.y);

        maxX = std::max(maxX, point.x);
        maxY = std::max(maxY, point.y);
    }

    // main loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(BLACK);

            // Draw the points
            for (Vector2 point : points) {
                // range of x and y
                vector<Vector2> range;

                if (std::optional<vector<Vector2>> rangeOpt =
                        getRange(curFractal);
                    rangeOpt.has_value()) { // if we already know the range
                    range = rangeOpt.value();
                } else {
                    // if we don't know the range, make it using min and max
                    range = {
                        Vector2{minX, maxX},
                        Vector2{minY, maxY},
                    };
                }

                /*
                 * map the point: x => 0 - WIDTH and y => HEIGHT - 0
                 * HEIGHT - 0 since we are converting from mathematical to
                   computer corrdinate system.
                 */
                point.x = mapRange(point.x, range[0].x, range[0].y, 0, WIDTH);
                point.y = mapRange(point.y, range[1].x, range[1].y, HEIGHT, 0);

                // rainbow gradient
                float hue = mapRange(point.x, 0, WIDTH, 0, 360);
                float value = mapRange(point.y, 0, HEIGHT, 1, 0);

                if (IsKeyDown(KEY_SPACE)) {
                    value = 1.f;
                }

                Color color = ColorFromHSV(hue, 1.0f, value);

                DrawPixelV(point, color);
            }
        }
        EndDrawing();
    }

    CloseWindow();
}
