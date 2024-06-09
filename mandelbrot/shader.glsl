#version 460 core

out vec4 fragColor;

uniform vec2 resolution;
uniform float minRange;
uniform float maxRange;
uniform int iterations;

// both functions stolen from somewhere idk where tho
vec3 hsv2rgb(vec3 hue) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(hue.xxx + K.xyz) * 6.0 - K.www);
    return hue.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), hue.y);
}

float map(float value, float inMin, float inMax, float outMin, float outMax) {
    return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
}

void main()
{
    float WIDTH = resolution.x;
    float HEIGHT = resolution.y;

    float a = map(gl_FragCoord.x, 0, WIDTH, minRange, maxRange);
    float b = map(gl_FragCoord.y, 0, HEIGHT, minRange, maxRange);

    // for mandelbrot
    // const float ca = a;
    // const float cb = b;

    // for julia sets
    // [−0.70176 − 0.3842i], [0.285 + 0.01i], [0.4 + 0.4i], [-0.8 + 0.156i], [−0.7269 + 0.1889i], [−0.835 − 0.2321i]
    const float ca = -0.835;
    const float cb = -0.2321;

    int counter = 0;

    while (counter < iterations) {
        const float aa = a * a - b * b;
        const float bb = 2 * a * b;

        a = aa + ca;
        b = bb + cb;

        counter++;

        if (abs(a + b) > 16) { // assuming 16 as infinity
            break;
        }
    }

    if (counter < iterations) {
        float quotient = float(counter) / float(iterations);
        float c = clamp(quotient, 0.0, 1.0);

        if (quotient > 0.5) {
            // Close to the mandelbrot set the color changes
            // from green to white
            fragColor = vec4(c, 255, c, 1.0);
        } else {
            // Far away it changes from black to green
            fragColor = vec4(0, c, 0, 1.0);
        }
    } else if (counter == iterations) {
        fragColor = vec4(0.0, 0, 0, 1.0);
    }
}
