cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project("tlo-float-repr")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(global_compile_options -pedantic -Wall -Wextra -Werror)
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(global_compile_options ${global_compile_options} -Weverything
    -Wno-c++98-c++11-compat-binary-literal -Wno-c++98-compat)
endif()

if (MINGW)
  if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    # for some reason, strtod and strtof are marked unused
    set(global_compile_options ${global_compile_options}
      -Wno-used-but-marked-unused)
  endif()
endif()

option(TLOFR_COLORED_DIAGNOSTICS
  "Tell the compiler to use colors in diagnostics (GNU/Clang only)." ON)
if (TLOFR_COLORED_DIAGNOSTICS)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(global_compile_options ${global_compile_options} -fdiagnostics-color)
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(global_compile_options ${global_compile_options} -fcolor-diagnostics)
  endif()
endif()

option(TLOFR_USE_LIBCPP "Use libc++ (Clang only)." OFF)
if (TLOFR_USE_LIBCPP)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(global_compile_options ${global_compile_options} -stdlib=libc++)
    set(global_link_libraries ${global_link_libraries} c++)
  endif()
endif()

set(headers float-repr.hpp program.hpp)

add_executable(tlo-double-repr ${headers} tlo-double-repr.cpp)
set_target_properties(tlo-double-repr PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(tlo-double-repr PRIVATE cxx_std_17)
target_compile_options(tlo-double-repr PRIVATE ${global_compile_options})
target_link_libraries(tlo-double-repr PRIVATE ${global_link_libraries})

add_executable(tlo-float-repr ${headers} tlo-float-repr.cpp)
set_target_properties(tlo-float-repr PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(tlo-float-repr PRIVATE cxx_std_17)
target_compile_options(tlo-float-repr PRIVATE ${global_compile_options})
target_link_libraries(tlo-float-repr PRIVATE ${global_link_libraries})
