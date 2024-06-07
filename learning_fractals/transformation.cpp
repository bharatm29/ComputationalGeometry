#include <optional>
#include <random>
#include <raylib.h>
#include <vector>

using std::vector;

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

enum Fractals {
    FERN,
    SPIRAL,
    SIEPENKSI,
};

typedef struct {
    /*
    Define the affine transformations in the form: Ax + b
    [x'] = [a b][x] + [e]
    [y'] = [c d][y] + [f]

    The above is flatten to: [a, b, c, d, e, f] (row wise flattening)
    */
    vector<vector<float>> transformations;

    // Probabilities for each transformation
    vector<float> probabilities;

    const int iterations;

    // whether we already know the range of the values of transformations
    const bool isRangeDefined;
    const vector<Vector2> range;
} Transformation;

const vector<Transformation> transformations = {
    Transformation{.transformations = {{0.0, 0.0, 0.0, 0.16, 0.0, 0.0},
                                       {0.2, -0.26, 0.23, 0.22, 0.0, 1.6},
                                       {-0.15, 0.28, 0.26, 0.24, 0.0, 0.44},
                                       {0.85, 0.04, -0.04, 0.85, 0.0, 1.6}},

                   .probabilities = {0.01, 0.07, 0.07, 0.85},

                   .iterations = 50000,

                   .isRangeDefined = true,

                   .range =
                       {
                           Vector2{-2.1820, 2.6558},
                           Vector2{0, 9.9983},
                       }},
    Transformation{
        .transformations = {{0.25, 0.0, 0.0, 0.25, 0.0, 0.5},
                            {0.823, -0.475, 0.475, 0.823, 0.301, -0.172}},

        .probabilities = {0.073, 0.927},

        .iterations = 50000,

        .isRangeDefined = false,
    },

    Transformation {
        .transformations = {{.5, .0, .0, .5, .0, .0,},
            {.5, .0, .0, .5, .5, .5,},
            {.5, .0, .0, .5, 1, .0,}
        },

        .probabilities = {.3333333333333333, .3333333333333333, .3333333333333333},

        .iterations = 50000,
        .isRangeDefined = true,

        .range =
            {
                Vector2{0, 2},
                Vector2{0, 1},
            }
    }
};

int getIterations(Fractals fractal) {
    return transformations[fractal].iterations;
}

std::optional<vector<Vector2>> getRange(Fractals fractal) {
    if (transformations[fractal].isRangeDefined) {
        return std::make_optional<vector<Vector2>>(
            transformations[fractal].range);
    }

    return std::nullopt;
}

Vector2 applyTransformation(const vector<float> &transformation,
                            const Vector2 &point) {
    float x = transformation[0] * point.x + transformation[1] * point.y +
              transformation[4]; // ax + by + e
    float y = transformation[2] * point.x + transformation[3] * point.y +
              transformation[5]; // cx + dy + f

    return Vector2{x, y};
}

Vector2 applyTransformation(Fractals fractal, const Vector2 &point) {
    Vector2 newPoint = point;
    const Transformation &curFractalTran = transformations[fractal];

    // chose a random transformation using Inverse Transform Sampling
    const float random_number = gen_random_float(.0, 1.);
    float probabilitySum = 0.f;

    for (size_t index = 0; index < curFractalTran.transformations.size();
         index++) {
        probabilitySum += curFractalTran.probabilities[index];

        // if the random number is the probability distribution range
        if (random_number <= probabilitySum) {
            newPoint = applyTransformation(
                curFractalTran.transformations[index], point);
            break;
        }
    }

    /*TODO: Other method of picking the transformation according to the given
     * probability:
     * We can form a cumulative probability out of the given
     * probability and then find the first equal or the next greater value
     * to the random value, thus taking it's index.
     */

    return newPoint;
}
