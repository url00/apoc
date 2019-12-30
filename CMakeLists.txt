cmake_minimum_required (VERSION 3.8)

add_executable (apoc "apoc.cpp" "apoc.h")

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	find_package(X11 REQUIRED)
	target_link_libraries(apoc ${X11_LIBRARIES})
	include_directories(${X11_INCLUDE_DIR})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	target_link_libraries(apoc kernel32  user32  gdi32  winspool  comdlg32  advapi32  shell32  ole32  oleaut32  uuid  odbc32  odbccp32 ;)
endif()
