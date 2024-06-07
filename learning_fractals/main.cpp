#include <random>
#include <raylib.h>
#include <vector>

using std::vector;

const int WIDTH = 800;
const int HEIGHT = 800;
const int ITERATIONS = 50000;

/*
Define the affine transformations in the form: Ax + b
[x'] = [a b][x] + [e]
[y'] = [c d][y] + [f]

The above is flatten to: [a, b, c, d, e, f] (row wise flattening)
*/
vector<vector<float>> transformations = {{0.0, 0.0, 0.0, 0.16, 0.0, 0.0},
                                         {0.2, -0.26, 0.23, 0.22, 0.0, 1.6},
                                         {-0.15, 0.28, 0.26, 0.24, 0.0, 0.44},
                                         {0.85, 0.04, -0.04, 0.85, 0.0, 1.6}};

// Probabilities for each transformation
vector<float> probabilities = {0.01, 0.07, 0.07, 0.85};

Vector2 applyTransformation(const vector<float> &transformation,
                            const Vector2 &point) {
    float x = transformation[0] * point.x + transformation[1] * point.y +
              transformation[4]; // ax + by + e
    float y = transformation[2] * point.x + transformation[3] * point.y +
              transformation[5]; // cx + dy + f

    return Vector2{x, y};
}

float gen_random_float(float min, float max) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<float> dist(min, max);

    return dist(rng);
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

    for (int _ = 0; _ < ITERATIONS; ++_) {
        // chose a random transformation using Inverse Transform Sampling
        const float random_number = gen_random_float(.0, 1.);
        float probabilitySum = 0.f;

        for (size_t index = 0; index < transformations.size(); index++) {
            probabilitySum += probabilities[index];

            // if the random number is the probability distribution range
            if (random_number <= probabilitySum) {
                point = applyTransformation(transformations[index], point);
                break;
            }
        }
        /* Other method of picking the transformation according to the given
         * probability:
         * We can form a cumulative probability out of the given
         * probability and then find the first equal or the next greater value
         * to the random value, thus taking it's index.
         */

        // map the point into our WIDTH x HEIGHT coordinate space
        Vector2 mappedPoint = point;

        mappedPoint.x = mapRange(point.x, -2.1820, 2.6558, 0, WIDTH);
        mappedPoint.y =
            mapRange(point.y, 0, 9.9983, HEIGHT,
                     0); // here we are mapping from HEIGHT-0 because we are
                         // converting from mathematical(y inc up) to computer
                         // coordinates system(y inc down).

        points.push_back(mappedPoint);
    }

    // main loop
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
