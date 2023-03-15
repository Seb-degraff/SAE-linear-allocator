clang $1 sokol_time.cpp           \
    -o program                                     \
    -g                                             \
    -O0                                            \
    -std=c++20                                     \
    -I./include                                    \
    -Wall -Wextra -Werror -Wno-unused-variable     \
    -fdiagnostics-color=always                     \
    lib/macos/libraylib.a                          \
    -ObjC++                                        \
    -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL \
|| exit 1 # if clang failed, exit early. 

echo "--- Program start ---"

./program
