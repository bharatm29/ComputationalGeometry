#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "../raygui/raygui.h"
#include <stdbool.h>

typedef enum { TITLE, BUBBLE_SORT, SELECTION_SORT, INSERTION_SORT } State;

void renderArray(const int arr[], const int size) {
    for (int i = 0; i < size; i++) {

        // array times 25 px?
        const int height = arr[i] == 0 ? 5 : arr[i] * 25;

        // 25px padding from left, then 20px width of the rect and 15px gap
        // between the rect * i.
        const int posX = 35 * i + 25;

        // 25px gap from the top
        const int posY = 0 + 25 + 15;

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
    State currentState = TITLE;
    int framCounter = -1;
    int waitTime = 60;

    int selectedSortTech = 0;

    const char *sortingTechs[] = {"Bubble Sort", "Selection Sort",
                                  "Insertion Sort"};

    while (!WindowShouldClose()) {
        if (render) {
            switch (selectedSortTech) {
            case 0: {
                currentState = BUBBLE_SORT;
            } break;
            case 1: {
                currentState = SELECTION_SORT;
            } break;
            case 2: {
                currentState = INSERTION_SORT;
            } break;
            default: {
                currentState = TITLE;
            } break;
            }
        }

        BeginDrawing();
        GuiEnable();

        switch (currentState) {
        case TITLE: {
            ClearBackground(SKYBLUE);

            const char title[] = {"Sortualizer"};

            int fontSize = 20;
            int textWidth = MeasureText(title, fontSize);

            DrawText(title, (WIDTH - textWidth) / 2, 15, fontSize, BLUE);

            const char selectText[] = "Pick a sorting technique";

            fontSize = 20;
            textWidth = MeasureText(selectText, fontSize);

            DrawText(selectText, (WIDTH - textWidth) / 2, HEIGHT / 2 - 60,
                     fontSize, BLUE);

            // Extract Into Varible
            const int defaultFontSize = 15;
            GuiSetStyle(DEFAULT, TEXT_SIZE, defaultFontSize);

            const int comboBoxWidth =
                // FIXME: Make the longest word dynamic and figure out what's
                // missing in the calculation :(
                MeasureText("Selection Sort", defaultFontSize) +
                GuiGetStyle(COMBOBOX, COMBO_BUTTON_WIDTH) +
                GuiGetStyle(COMBOBOX, COMBO_BUTTON_SPACING) + defaultFontSize;

            GuiComboBox((Rectangle){(WIDTH - comboBoxWidth) / 2.0, HEIGHT / 2.0 - 15,
                                    comboBoxWidth, 30},
                        TextJoin(sortingTechs,
                                 sizeof(sortingTechs) / sizeof(sortingTechs[0]),
                                 ";"),
                        &selectedSortTech);

            const char btnTxt[] = "Sort!";
            const int sortButtonWidth = 10 + MeasureText(btnTxt, defaultFontSize) + 10;

            const bool isSortButtonPressed =
                GuiButton((Rectangle){(WIDTH - sortButtonWidth) / 2.0,
                                      HEIGHT / 2.0 + 25, sortButtonWidth, 30},
                          btnTxt);

            if (IsKeyPressed(KEY_ENTER) || isSortButtonPressed) {
                render = true;
            }
        } break;

        case BUBBLE_SORT: {

            ClearBackground(WHITE);

            DrawText("Bubble Sort", 10, 5, 20, RED);

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
        } break;
        case SELECTION_SORT: {
            ClearBackground(WHITE);
            DrawText("Selection Sort", 10, 5, 20, RED);
        } break;
        case INSERTION_SORT: {
            ClearBackground(WHITE);
            DrawText("Insertion Sort", 10, 5, 20, RED);
        } break;
        default: {
            ClearBackground(WHITE);
            DrawText("Illegal State pls fix quick", 10, 5, 20, RED);
        } break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
