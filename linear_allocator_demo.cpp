#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <stdarg.h> // for "varargs"

#include "raylib.h" // for window management, input and drawing
#include "sokol_time.h" // To measure performance

#define MIN(a,b) (((a)<(b))? (a):(b))
#define MAX(a,b) (((a)>(b))? (a):(b))

float lerp (float a, float b, float t)
{
    return a * t + b * (1-t);
}

// Forward declaration of both versions of the str_format function
char* str_format_malloc(const char* format, ...);
char* str_format_temp(const char* format, ...);

// A macro that whill switch between the two versions of str_format depending on the value of use_malloc for easy perf testing.
#define FORMAT(...) (use_malloc ? str_format_malloc(__VA_ARGS__) : str_format_temp(__VA_ARGS__))

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    // Setup stuff
    stm_setup(); // sokol_time initialization
    InitWindow(screenWidth, screenHeight, "linear allocator demo");
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    Font font = LoadFont("NotoSansMono-Regular.ttf");

    float average_frame_time = 1000000;
    uint64_t min_frame_time = UINT64_MAX;

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        bool use_malloc = !IsKeyDown(KEY_SPACE);

        BeginDrawing();

            ClearBackground(RAYWHITE);

            uint64_t frame_start_time = stm_now();

            DrawTextEx(font, "Some complicated stats updated every frame:", (Vector2){ 20, 20 }, 32, 1, DARKBLUE);

            char* fps_str = FORMAT("Current fps: %i (%.4fs)", GetFPS(), GetFrameTime());
            DrawTextEx(font, fps_str, (Vector2){ 60, 60 }, 32, 1, DARKBLUE);
            if (use_malloc) {
                free(fps_str);
            }

            DrawTextEx(font, use_malloc ? "using malloc" : "using temp", (Vector2){ 20, 400 }, 32, 1, DARKBLUE);

            double t = GetTime();
            for (int i = 0; i < 3000; i++) {
                char* str = FORMAT("sin: % 01.3f, cos: % 01.3f", sin(t*i), cos(t*i));
                if (i < 10) {
                    DrawTextEx(font, str, (Vector2){ 20, 120.f + i * 20 }, 32, 1, DARKBLUE);
                }
                if (use_malloc) {
                    free(str);
                }
            }

            uint64_t frame_duration = stm_since(frame_start_time);
            char* frame_duration_str = FORMAT("Our code: %.2fms. Average: %.2fms, Minimum: %.2fms", stm_ms(frame_duration), stm_ms(average_frame_time), stm_ms(min_frame_time));
            average_frame_time = lerp(average_frame_time, (float) frame_duration, 0.99);
            if (frame_duration < min_frame_time) {
                min_frame_time = frame_duration;
            }
            DrawTextEx(font, frame_duration_str, (Vector2){ 20, 120.f + 11 * 20 }, 32, 1, DARKBLUE);
            if (use_malloc) {
                free(frame_duration_str);
            }

        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context

    return EXIT_SUCCESS;
}

char* str_format_malloc(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    // Make a copy of the varargs, because we'll use them twice.
    va_list args_copy;
    va_copy(args_copy, args);

    // Call vsnprintf a first time without a buffer just to know the size of the required buffer.
    size_t size = vsnprintf(nullptr, 0, format, args_copy) + 1; // + 1 for the null character
    va_end(args_copy);

    // Allocate just the right amount of memory for our string.
    char* mem = (char*) malloc(size);
    
    // Call vsnprintf a second time to fill in the buffer.
    vsnprintf(mem, size, format, args);

    va_end(args);

	return mem;
}

char* str_format_temp(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    // Call vsnprintf a second time to fill in the buffer.
    vsnprintf(nullptr, 0, format, args);

    va_end(args);

    // TODO: implement this version with a linear allocator
    return (char*) "not implemented";
}
