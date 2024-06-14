#ifndef VERLET_H
#define VERLET_H

#include <raylib.h>
#include <raymath.h>

const float OBJ_RADIUS = 10.f;

class Verlet {
  public:
    Vector2 curPosition;
    Vector2 oldPosition;
    Vector2 acceleration;
    float radius;
    Color color;

    Verlet()
        : radius{OBJ_RADIUS}, color(WHITE), curPosition({0}), oldPosition({0}),
          acceleration({0}) {}

    Verlet(const Vector2 pos)
        : radius{OBJ_RADIUS}, color(WHITE), curPosition(pos), oldPosition({pos}),
          acceleration({0}) {}

    Verlet(const Vector2 pos, const float radius, const Color color)
        : radius{radius}, color(color), curPosition(pos), oldPosition({pos}),
          acceleration({0}) {}

    void updatePos(const float dt) {
        const Vector2 velocity =
            Vector2Subtract(this->curPosition, this->oldPosition);

        this->oldPosition = this->curPosition;

        Vector2 newPosition = Vector2Add(this->curPosition, velocity);

        this->curPosition =
            Vector2Add(newPosition, Vector2Scale(this->acceleration, dt * dt));

        this->acceleration = {0.f, 0.f};
    }

    void accelerate(Vector2 acc) {
        this->acceleration = Vector2Add(this->acceleration, acc);
    }
};

#endif // !VERLET_H
