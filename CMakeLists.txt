cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(LINK_WHAT_YOU_USE ON)

project(
  PathFinding
  LANGUAGES C CXX
  VERSION 0.0.1)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "Please create a build directory and run CMake from there")
endif ()

add_definitions(-D_USE_MATH_DEFINES)

string(COMPARE EQUAL ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR} IS_MAIN_PROJECT)
option(PATH_FINDING_BUILD_TEST "Build tests" ${IS_MAIN_PROJECT})

file(GLOB_RECURSE CODE_FILES CONFIGURE_DEPENDS src/*.cpp src/*.c src/*.cc)
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS include/*.h include/*.hpp)

add_library(${PROJECT_NAME} STATIC ${CODE_FILES} ${HEADER_FILES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                                  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE ON POSITION_INDEPENDENT_CODE ON)

if (MSVC)
  target_compile_options(
    ${PROJECT_NAME}
    INTERFACE /utf-8
              /W4
              /EHsc
              /Zc:__cplusplus
              /Zc:preprocessor
              /Gy
              $<$<CONFIG:Debug>:/Od>
              $<$<CONFIG:Release>:/O2
              /GL>)
  target_link_options(
    ${PROJECT_NAME}
    INTERFACE
    $<$<CONFIG:Release>:
    /LTCG
    /OPT:REF
    /OPT:ICF
    >)
else ()
  target_compile_options(
    ${PROJECT_NAME}
    INTERFACE -fPIC
              -Wall
              -Wextra
              -Wconversion
              -Wsign-compare
              -Werror=uninitialized
              -Werror=return-type
              -Werror=unused-result
              -Werror=suggest-override
              -Wzero-as-null-pointer-constant
              -Wmissing-declarations
              -Wold-style-cast
              -Wnon-virtual-dtor
              $<$<CONFIG:Debug>:-g>
              $<$<CONFIG:Release>:-O3
              -flto>)
  target_link_options(${PROJECT_NAME} INTERFACE $<$<CONFIG:Release>:-flto>)
endif ()

if (PATH_FINDING_BUILD_TEST)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd/Catch2)

  add_executable(${PROJECT_NAME}_test test/test_main.cpp)
  target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME} Catch2::Catch2WithMain)

  enable_testing()
  add_test(NAME PathFindingTest COMMAND ${PROJECT_NAME}_test)
endif ()

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
  EXPORT ${PROJECT_NAME}Targets
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
