add_executable(apoc "apoc.cpp")
target_include_directories(apoc PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
set_target_properties(apoc PROPERTIES COMPILE_FLAGS "/Zc:wchar_t-")
target_link_libraries(apoc PUBLIC kernel32  user32  gdi32  winspool  comdlg32  advapi32  shell32  ole32  oleaut32  uuid  odbc32  odbccp32)