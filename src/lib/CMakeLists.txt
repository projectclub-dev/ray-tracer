add_library(ray-tracer-lib INTERFACE)

target_include_directories(ray-tracer-lib
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(ray-tracer-lib INTERFACE cxx_std_23)