# An INTERFACE library has no sources of its own. It is a bundle of usage
# requirements (flags, definitions) that any target linking it inherits.
# We link it PRIVATE to our own targets only, so third-party code such as
# Catch2 is never compiled with our -Werror.

add_library(detect_options INTERFACE)
add_library(detect::options ALIAS detect_options)

# ---- Warnings ----------------------------------------------------------------
# -Wconversion / -Wsign-conversion matter a lot for numeric and later
# fixed-point code: they flag silent narrowing (double -> float, size_t -> int).
set(_gcc_clang_warnings
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wconversion
    -Wsign-conversion
    -Wdouble-promotion
    -Wold-style-cast
    -Wnon-virtual-dtor
    -Woverloaded-virtual
    -Wnull-dereference
    -Wimplicit-fallthrough
    -Wformat=2)

target_compile_options(
  detect_options
  INTERFACE $<$<CXX_COMPILER_ID:GNU,Clang>:${_gcc_clang_warnings}>
            $<$<AND:$<BOOL:${DETECT_WARNINGS_AS_ERRORS}>,$<CXX_COMPILER_ID:GNU,Clang>>:-Werror>)

# ---- Debug-only standard library checks --------------------------------------
# Cheap bounds checks on std::vector::operator[], std::span, etc. in libstdc++.
target_compile_definitions(detect_options INTERFACE $<$<CONFIG:Debug>:_GLIBCXX_ASSERTIONS>)

# ---- Sanitizers ----------------------------------------------------------------
# ASan: out-of-bounds, use-after-free, leaks. UBSan: signed overflow, bad
# shifts, misaligned access. -fno-sanitize-recover makes UB fail the test run
# instead of just printing a warning.
if(DETECT_ENABLE_SANITIZERS)
  set(_sanitizer_flags -fsanitize=address,undefined -fno-sanitize-recover=undefined
                       -fno-omit-frame-pointer)
  target_compile_options(detect_options INTERFACE ${_sanitizer_flags})
  target_link_options(detect_options INTERFACE ${_sanitizer_flags})
endif()
