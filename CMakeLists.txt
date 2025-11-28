cmake_minimum_required(VERSION 3.11)
project(Calculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

add_executable(Calculator
    main.cpp 
    Calculator.cpp
    loader.cpp
)

# ---- plugins ----
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)

function(make_plugin name src)
  add_library(${name} SHARED ${src})
  set_target_properties(${name} PROPERTIES
      OUTPUT_NAME ${name}
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins")
  if (MSVC)
    target_compile_options(${name} PRIVATE /EHsc /W3)
  endif()
endfunction()

make_plugin(dynamic_sin  plugins/sin_deg.cpp)
make_plugin(dynamic_cos  plugins/cos_deg.cpp)
make_plugin(dynamic_ln  plugins/ln.cpp)
make_plugin(dynamic_pow  plugins/pow.cpp)

add_custom_command(TARGET Calculator POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:Calculator>/plugins"
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_BINARY_DIR}/plugins" "$<TARGET_FILE_DIR:Calculator>/plugins"
)
