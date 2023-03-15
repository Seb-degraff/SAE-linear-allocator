clang linear_allocator_demo.cpp sokol_time.cpp           ^
    -o program.exe                                 ^
    -g                                             ^
    -O0                                            ^
    -std=c++20                                     ^
    -I./include                                    ^
    -Wall -Wextra -Werror -Wno-unused-variable     ^
    -fdiagnostics-color=always                     ^
    lib/windows-msvc16/raylib.lib                  ^
    -lmsvcrt -lOpenGL32 -lGdi32 -lWinMM -lkernel32 -lshell32 -lUser32 -Xlinker /NODEFAULTLIB:libcmt
REM Wondering how I knew what libraries to link with? Here you go -> https://stackoverflow.com/questions/72208683/what-standard-libraries-do-i-need-to-link-to-use-raylib-with-windows-and-clang
REM I wish raylib had better documentation for this

REM Stop on error
if %errorlevel% neq 0 exit /b %errorlevel%

REM Launch the program.exe
program.exe
