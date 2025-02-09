cmake_minimum_required(VERSION 3.25)

message("${CMAKE_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake")

project(test-json-detector VERSION 0.0.1 LANGUAGES CXX)

find_package(psb-json-detector CONFIG REQUIRED)

add_executable("${PROJECT_NAME}" test-json-detector.cpp)
target_link_libraries("${PROJECT_NAME}" PRIVATE psb::json-detector)

add_custom_target(
    smoke-test
    COMMAND echo "[]" | "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}"
    DEPENDS "${PROJECT_NAME}"
    COMMENT "Running smoke test"
)
