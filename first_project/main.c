#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "../raygui/raygui.h"
#include <stdbool.h>

void renderArray(const int arr[], const int size) {
    for (int i = 0; i < size; i++) {

        // array times 25 px?
        const int height = arr[i] == 0 ? 5 : arr[i] * 25;

        // 25px padding from left, then 20px width of the rect and 15px gap
        // between the rect * i.
        const int posX = 35 * i + 25;

        // 25px gap from the top
        const int posY = 0 + 25;

        // each rect is of 20px width
        const int width = 20;

        DrawRectangleGradientV(posX, posY, width, height, BLUE, RED);
    }
}

int main() {
    int arr[] = {20, 1, 3, 8, 4, 16, 12, 9, 5, 2, 11, 0, 19, 12};
    const int size = sizeof(arr) / sizeof(arr[0]);

    // 25px padding from both sides. each rectangle is of 20px and there is a
    // 15px gap between them
    static const int WIDTH = (size * 20) + (25 + 25) + ((size - 1) * 15);
    static const int HEIGHT = 600;

    InitWindow(WIDTH, HEIGHT, "please work");

    SetTargetFPS(60);

    int i = 0;
    bool sorted = false;
    bool render = false;
    bool btnExportPressed = false;
    int framCounter = -1;
    int waitTime = 60;

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(WHITE);

        DrawText("Bubble Sort", 10, 5, 20, RED);

        if (IsKeyPressed(KEY_ENTER)) {
            render = true;
        }

        if (render) {
            if (!sorted && framCounter == waitTime) {
                framCounter = 0;
                if (i < size) {
                    for (int j = 0; j < size - i - 1; j++) {
                        if (arr[j] > arr[j + 1]) {
                            const int temp = arr[j];
                            arr[j] = arr[j + 1];
                            arr[j + 1] = temp;
                        }
                    }
                    renderArray(arr, size);

                    i++;
                } else {
                    sorted = true;
                }
            } else {
                renderArray(arr, size);

                framCounter++;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
