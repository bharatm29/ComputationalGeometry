#include <algorithm>
#include <raylib.h>
#include <vector>

std::vector<double> pchipSlopes(const std::vector<double> &x,
                                const std::vector<double> &y) {
    int n = x.size();
    std::vector<double> delta(n - 1);
    for (int i = 0; i < n - 1; ++i) {
        delta[i] = (y[i + 1] - y[i]) / (x[i + 1] - x[i]);
    }

    std::vector<double> m(n);
    m[0] = delta[0];
    for (int i = 1; i < n - 1; ++i) {
        if (delta[i - 1] * delta[i] <= 0) {
            m[i] = 0;
        } else {
            m[i] = 2 / ((1 / delta[i - 1]) + (1 / delta[i]));
        }
    }
    m[n - 1] = delta[n - 1];
    return m;
}

double pchipInterpolate(double x, double x0, double x1, double y0, double y1,
                        double m0, double m1) {
    double h = x1 - x0;
    double t = (x - x0) / h;
    double t2 = t * t;
    double t3 = t2 * t;
    return (2 * t3 - 3 * t2 + 1) * y0 + (t3 - 2 * t2 + t) * m0 * h +
           (-2 * t3 + 3 * t2) * y1 + (t3 - t2) * m1 * h;
}

std::vector<double> linspace(double start, double end, int num) {
    std::vector<double> result(num);
    double step = (end - start) / (num - 1);
    for (int i = 0; i < num; ++i) {
        result[i] = start + i * step;
    }
    return result;
}

std::vector<double> pchip(const std::vector<double> &x,
                          const std::vector<double> &y,
                          const std::vector<double> &x_new) {
    int n = x.size();
    std::vector<double> slopes = pchipSlopes(x, y);
    std::vector<double> y_new(x_new.size());

    for (size_t i = 0; i < x_new.size(); ++i) {
        for (int j = 0; j < n - 1; ++j) {
            if (x_new[i] >= x[j] && x_new[i] <= x[j + 1]) {
                y_new[i] = pchipInterpolate(x_new[i], x[j], x[j + 1], y[j],
                                            y[j + 1], slopes[j], slopes[j + 1]);
                break;
            }
        }
    }
    return y_new;
}

std::vector<Color> generateUltraFractalColors() {
    std::vector<double> x_observed = {-0.1425, 0,      0.16, 0.42,
                                      0.6425,  0.8575, 1,    1.16};
    std::vector<double> yR_observed = {0, 0, 32, 237, 255, 0, 0, 32};
    std::vector<double> yG_observed = {2, 7, 107, 255, 170, 2, 7, 107};
    std::vector<double> yB_observed = {0, 100, 203, 255, 0, 0, 100, 203};

    std::vector<double> x =
        linspace(*std::min_element(x_observed.begin(), x_observed.end()),
                 *std::max_element(x_observed.begin(), x_observed.end()), 1000);

    std::vector<double> yR = pchip(x_observed, yR_observed, x);
    std::vector<double> yG = pchip(x_observed, yG_observed, x);
    std::vector<double> yB = pchip(x_observed, yB_observed, x);

    auto start =
        std::find_if(x.begin(), x.end(), [](double v) { return v >= 0; });
    auto end = std::find_if(x.begin(), x.end(), [](double v) { return v > 1; });

    int startIndex = std::distance(x.begin(), start);
    int endIndex = std::distance(x.begin(), end);

    std::vector<Color> gradientColors(endIndex - startIndex);
    for (int i = startIndex; i < endIndex; ++i) {
        gradientColors[i - startIndex] = {static_cast<unsigned char>(yR[i]),
                                          static_cast<unsigned char>(yG[i]),
                                          static_cast<unsigned char>(yB[i])};
    }

    return gradientColors;
}
