get_filename_component(PSB_JSON_DETECTOR_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

list(APPEND CMAKE_MODULE_PATH ${PSB_JSON_DETECTOR_CMAKE_DIR})

include(CMakeFindDependencyMacro)
find_dependency(wwa-coro)

if(NOT TARGET psb-json-detector)
    include("${PSB_JSON_DETECTOR_CMAKE_DIR}/psb-json-detector-target.cmake")
    add_library(psb::json-detector ALIAS psb-json-detector)
endif()
