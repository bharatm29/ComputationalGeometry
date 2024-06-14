#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "./verletobj.cpp"
#include <raylib.h>
#include <raymath.h>
#include <vector>

using std::vector;

struct Simulator {
    Vector2 gravity = {0.f, 1000.f};
    vector<Verlet> objs;

    void update(const float dt) {
        const unsigned int substep = 8;
        const float subDt = dt / (substep * 1.f);

        for (unsigned int i(substep); i--;) {
            applyGravity();

            applyConstraints();

            solveCollisions();

            updatePosition(dt);
        }
    }

    void applyGravity() {
        for (Verlet &obj : this->objs) {
            obj.accelerate(this->gravity);
        }
    }

    void updatePosition(const float dt) {
        for (Verlet &obj : this->objs) {
            obj.updatePos(dt);
        }
    }

    void applyConstraints() {
        const Vector2 center = {500.f, 500.f};
        const float radius = 300.f;

        const float objRadius = OBJ_RADIUS;

        for (Verlet &obj : this->objs) {
            const Vector2 posVecToCenter =
                Vector2Subtract(obj.curPosition, center);

            const float dist = Vector2Length(posVecToCenter);

            if (dist > (radius - objRadius)) {
                const Vector2 n = Vector2Scale(
                    posVecToCenter, 1.0 / dist); // no idea wtf is this

                obj.curPosition =
                    Vector2Add(center, Vector2Scale(n, radius - objRadius));
            }
        }
    }

    void solveCollisions() {
        for (size_t i = 0; i < objs.size(); i++) {
            Verlet &obj1 = objs.at(i);
            for (size_t j = 0; j < objs.size(); j++) {
                if (i == j)
                    continue;

                Verlet &obj2 = objs.at(j);

                const float collisionRadius = obj1.radius + obj2.radius;

                const Vector2 collision_axis =
                    Vector2Subtract(obj1.curPosition, obj2.curPosition);
                const float dist = Vector2Length(collision_axis);

                if (dist < collisionRadius) {
                    const Vector2 n = Vector2Scale(collision_axis, 1.f / dist);

                    const float delta = collisionRadius - dist;

                    Vector2 newVal = Vector2Scale(n, 0.5f * delta);
                    obj1.curPosition = Vector2Add(obj1.curPosition, newVal);
                    obj2.curPosition =
                        Vector2Subtract(obj2.curPosition, newVal);
                }
            }
        }
    }
};

#endif // !SIMULATOR_H
