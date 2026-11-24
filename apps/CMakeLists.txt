# Executables. Each one is a thin main() over the library, like a
# [project.scripts] entry point in pyproject.toml.

add_executable(detect_cli main.cpp)
target_link_libraries(detect_cli PRIVATE detect::detect detect::options)
