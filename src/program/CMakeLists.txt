add_executable (ray-tracer "ray-tracer.cpp" "ray-tracer.h")

target_link_libraries(ray-tracer PRIVATE ray-tracer-lib)

set_property(TARGET ray-tracer PROPERTY CXX_STANDARD 23)
