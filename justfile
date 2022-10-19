run: build
  ./build/app

format:
  clang-format --dry-run --Werror *.cpp

tidy: cmake
  clang-tidy -p build *.cpp

build: cmake
  ninja -Cbuild

cmake:
  cmake -Bbuild -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

clean:
  rm -r build
