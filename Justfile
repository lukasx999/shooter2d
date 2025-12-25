run: build
    ./build/game

spred: build
    ./build/spred

configure:
    cmake -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang

build: configure
    cmake --build build
