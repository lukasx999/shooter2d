run: build
    LD_LIBRARY_PATH=/usr/local/lib ./build/game

configure:
    cmake -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang

build: configure
    cmake --build build
