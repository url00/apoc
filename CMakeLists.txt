cmake_minimum_required (VERSION 3.11)


project(apoc)



set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

add_subdirectory(src)

# If directory-local project is equal to the global project:
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Tell generators to make use of folder hierachies.
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(PROJECT_BINARY_DIR "../bin/linux")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(PROJECT_BINARY_DIR "../bin/win")
endif()
