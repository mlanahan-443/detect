# detect

Detect-and-track pipeline: a NumPy prototype rebuilt in C++ (self-study project).

## Layout

```
detect/
├── CMakeLists.txt        top-level build: project, options, subdirectories
├── CMakePresets.json     named build configurations (debug, release, lint)
├── cmake/
│   └── ProjectOptions.cmake   warnings, sanitizers -> detect::options
├── include/detect/    PUBLIC headers  (#include <detect/...>)
├── src/                  library implementation (.cpp) + its CMakeLists
├── apps/                 executables built on the library
├── tests/                Catch2 unit tests
├── .clang-tidy           static analysis config
└── .clang-format         formatter config
```

## Everyday commands

```bash
# configure + build + test in one go (Debug, ASan/UBSan on)
cmake --workflow --preset debug

# or step by step
cmake --preset debug           # configure: generate build/debug/
cmake --build --preset debug   # compile
ctest --preset debug           # run tests
ctest --preset debug -R smoke  # run a subset (like pytest -k)

# optimized build for timing
cmake --preset release && cmake --build --preset release

# lint: clang-tidy runs on every translation unit as it compiles
cmake --preset lint && cmake --build --preset lint

# format
clang-format -i include/detect/*.hpp src/*.cpp apps/*.cpp tests/*.cpp
```
