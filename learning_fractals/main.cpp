#include <random>
#include <raylib.h>
#include <vector>

using std::vector;

const int WIDTH = 800;
const int HEIGHT = 800;
const int ITERATIONS = 50000;

// Define the affine transformations for the Barnsley Fern
vector<vector<float>> transformations = {{0.0, 0.0, 0.0, 0.16, 0.0, 0.0},
                                         {0.2, -0.26, 0.23, 0.22, 0.0, 1.6},
                                         {-0.15, 0.28, 0.26, 0.24, 0.0, 0.44},
                                         {0.85, 0.04, -0.04, 0.85, 0.0, 1.6}};

// Probabilities for each transformation
vector<float> probabilities = {0.01, 0.07, 0.07, 0.85};

// Apply the affine transformation to a point
Vector2 applyTransformation(const vector<float> &transformation,
                            const Vector2 &point) {
    float x = transformation[0] * point.x + transformation[1] * point.y +
              transformation[4];
    float y = transformation[2] * point.x + transformation[3] * point.y +
              transformation[5];

    return Vector2{x, y};
}

float gen_random_float(float min, float max) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<float> dist6(.0, 1.);

    return dist6(rng);
}

float mapRange(const float oldValue, const float oldMin, const float oldMax,
               const float newMin, const float newMax) {
    const float oldRange = (oldMax - oldMin);
    const float newRange = (newMax - newMin);
    const float newValue =
        (((oldValue - oldMin) * newRange) / oldRange) + newMin;

    return newValue;
}

int main() {
    InitWindow(WIDTH, HEIGHT, "IFSs");

    SetTargetFPS(60);

    vector<Vector2> points;

    Vector2 point = {0, 0};
    points.push_back(point);

    for (int i = 0; i < ITERATIONS; ++i) {
        const float rnd = gen_random_float(.0, 1.);

        if (rnd < 0.01) {
            point = applyTransformation(transformations[0], point);
        } else if (rnd < 0.08) {
            point = applyTransformation(transformations[1], point);
        } else if (rnd < 0.15) {
            point = applyTransformation(transformations[2], point);
        } else {
            point = applyTransformation(transformations[3], point);
        }

        Vector2 mappedPoint = point;

        mappedPoint.x = mapRange(point.x, -2.1820, 2.6558, 0, WIDTH);
        mappedPoint.y =
            mapRange(point.y, 0, 9.9983, HEIGHT,
                     0); // here we are mapping from HEIGHT-0 because we are
                         // converting from mathematical(y inc up) to computer
                         // coordinates system(y inc down).

        points.push_back(mappedPoint);
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(BLACK);

            // Draw the points
            for (size_t i = 0; i < points.size(); ++i) {
                // rainbow gradient
                float hue = mapRange(points[i].x, 0, WIDTH, 0, 360);
                float value = mapRange(points[i].y, 0, HEIGHT, 1, 0);
                Color color = ColorFromHSV(hue, 1.0f, value);

                DrawPixelV(points[i], color);
            }
        }
        EndDrawing();
    }

    CloseWindow();
}
