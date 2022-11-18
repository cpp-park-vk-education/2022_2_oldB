run TYPE="Debug": (build TYPE)
  ./build/app

build TYPE: (cmake TYPE)
  ninja -Cbuild

test: (build "Debug")
  ./build/test/test

cmake TYPE:
  cmake -Bbuild -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE={{TYPE}}

clean:
  rm -r build
