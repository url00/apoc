add_executable(apoc "apoc.cpp")
find_package(X11 REQUIRED)
target_link_libraries(apoc PUBLIC ${X11_LIBRARIES})
include_directories(${X11_INCLUDE_DIR})