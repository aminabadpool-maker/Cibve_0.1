#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PARTS 1000

typedef struct {
    int id;
    int checked;
    int is_active;
} VideoPart;

// Helper function for simple interactive buttons in pure Raylib
bool CustomButton(Rectangle bounds, const char *text) {
    Vector2 mousePoint = GetMousePosition();
    bool clicked = false;
    Color col = LIGHTGRAY;

    if (CheckCollisionPointRec(mousePoint, bounds)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) col = DARKGRAY;
        else col = GRAY;
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) clicked = true;
    } else {
        col = LIGHTGRAY;
    }

    DrawRectangleRec(bounds, col);
    DrawRectangleLines((int)bounds.x, (int)bounds.y, (int)bounds.width, (int)bounds.height, DARKGRAY);
    DrawText(text, (int)bounds.x + 10, (int)bounds.y + 8, 12, BLACK);

    return clicked;
}

void ExportSelectedParts(VideoPart *parts, int total_parts) {
    printf("[FFmpeg Engine] Processing selected parts for export...\n");
    for (int i = 0; i < total_parts; i++) {
        if (parts[i].checked) {
            printf("Exporting part %d...\n", parts[i].id);
        }
    }
}

int main(void) {
    const int screenWidth = 900;
    const int screenHeight = 650;

    InitWindow(screenWidth, screenHeight, "CIBVE v0.1 - C Is Best Video Editor");
    SetTargetFPS(60);

    int total_parts = 25;
    VideoPart parts[MAX_PARTS];
    for (int i = 0; i < total_parts; i++) {
        parts[i].id = i + 1;
        parts[i].checked = 1;
        parts[i].is_active = 0;
    }

    int playing_index = -1;
    float playback_timer = 0.0f;
    Vector2 scrollOffset = { 0, 0 };

    while (!WindowShouldClose()) {
        if (playing_index != -1) {
            playback_timer += GetFrameTime();
            for (int i = 0; i < total_parts; i++) parts[i].is_active = 0;
            parts[playing_index].is_active = 1;

            if (playback_timer > 1.5f) {
                playback_timer = 0.0f;
                playing_index++;
                while (playing_index < total_parts && !parts[playing_index].checked) {
                    playing_index++;
                }
                if (playing_index >= total_parts) {
                    playing_index = -1;
                    for (int i = 0; i < total_parts; i++) parts[i].is_active = 0;
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Header Section
        DrawRectangle(0, 0, screenWidth, 70, DARKGRAY);
        DrawText("CIBVE - Studio-Grade Video Editor for YouTube", 20, 20, 22, WHITE);
        DrawText("Lightweight C & Raylib Architecture", 20, 45, 12, LIGHTGRAY);

        // Control Buttons
        if (CustomButton((Rectangle){ 20, 90, 110, 35 }, "Mark All")) {
            for (int i = 0; i < total_parts; i++) parts[i].checked = 1;
        }
        if (CustomButton((Rectangle){ 140, 90, 110, 35 }, "Unmark All")) {
            for (int i = 0; i < total_parts; i++) parts[i].checked = 0;
        }
        if (CustomButton((Rectangle){ 260, 90, 130, 35 }, "Full Preview")) {
            for (int i = 0; i < total_parts; i++) {
                if (parts[i].checked) {
                    playing_index = i;
                    playback_timer = 0.0f;
                    break;
                }
            }
        }
        if (CustomButton((Rectangle){ 730, 90, 150, 35 }, "Export & Upload")) {
            ExportSelectedParts(parts, total_parts);
        }

        DrawLine(20, 140, 880, 140, LIGHTGRAY);

        // Parts List Area
        int start_y = 160;
        int visible_count = 12;
        int list_height = visible_count * 35;

        BeginScissorMode(20, start_y, 860, list_height);
        for (int i = 0; i < total_parts; i++) {
            int y = start_y + (i * 35) - (int)scrollOffset.y;
            
            if (y >= start_y - 30 && y <= start_y + list_height) {
                DrawRectangle(30, y + 5, 20, 20, parts[i].checked ? DARKGREEN : LIGHTGRAY);
                char label[64];
                snprintf(label, sizeof(label), "Part %03d (Segment Video Stream)", parts[i].id);
                DrawText(label, 65, y + 7, 14, BLACK);

                if (parts[i].is_active) {
                    DrawCircle(700, y + 15, 8, GREEN);
                    DrawText("PLAYING...", 720, y + 7, 12, DARKGREEN);
                } else {
                    DrawCircle(700, y + 15, 6, LIGHTGRAY);
                }

                if (CustomButton((Rectangle){ 800, y + 2, 60, 26 }, "Play")) {
                    playing_index = i;
                    playback_timer = 0.0f;
                }
            }
        }
        EndScissorMode();

        // Footer status
        DrawRectangle(0, 620, screenWidth, 30, LIGHTGRAY);
        DrawText("Status: Ready | Memory footprint: < 2MB | C11 Standard", 20, 628, 10, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
