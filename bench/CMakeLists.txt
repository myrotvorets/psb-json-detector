if(ENABLE_MAINTAINER_MODE)
    string(REPLACE " " ";" COMPILE_OPTIONS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_MM} -Wno-global-constructors -Wno-exit-time-destructors -Wno-weak-vtables")
    set_directory_properties(PROPERTIES COMPILE_OPTIONS "$<$<COMPILE_LANGUAGE:CXX>:${COMPILE_OPTIONS}>")
    unset(COMPILE_OPTIONS)
endif()

set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/src")

set(BENCH_TARGET bench)

enable_language(ASM)
add_executable("${BENCH_TARGET}")
set_target_properties(
    "${BENCH_TARGET}"
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)
target_sources("${BENCH_TARGET}" PRIVATE data.s json_detector.cpp throughput.cpp)
target_link_libraries("${BENCH_TARGET}" PRIVATE benchmark::benchmark benchmark::benchmark_main "${PROJECT_NAME}")
set_source_files_properties(
    data.s
    PROPERTIES
        INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}"
        LANGUAGE ASM
)

add_custom_target(
    benchmark
    COMMAND "${CMAKE_CURRENT_BINARY_DIR}/${BENCH_TARGET}"
    DEPENDS "${PROJECT_NAME}"
    COMMENT "Running benchmarks"
)
