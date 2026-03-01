cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(LINK_WHAT_YOU_USE ON)

project(
  PathFinding
  LANGUAGES C CXX
  VERSION 0.0.1)

if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "Please create a build directory and run CMake from there")
endif ()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(CompilerSettings)

add_definitions(-D_USE_MATH_DEFINES)

string(COMPARE EQUAL ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR} IS_MAIN_PROJECT)
option(PATH_FINDING_BUILD_SHARED "Build shared library" OFF)
option(PATH_FINDING_BUILD_TEST "Build tests" ${IS_MAIN_PROJECT})
option(PATH_FINDING_BUILD_VISUAL "Build visualization application" ${IS_MAIN_PROJECT})

file(GLOB_RECURSE CODE_FILES CONFIGURE_DEPENDS src/*.cpp src/*.c src/*.cc)
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS include/*.h include/*.hpp)

if (PATH_FINDING_BUILD_SHARED)
  add_library(${PROJECT_NAME} SHARED ${CODE_FILES} ${HEADER_FILES})
  target_compile_definitions(${PROJECT_NAME} PRIVATE PATHFINDING_BUILDING_DLL)
else ()
  add_library(${PROJECT_NAME} STATIC ${CODE_FILES} ${HEADER_FILES})
endif ()
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                                  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_setting(${PROJECT_NAME})

if (PATH_FINDING_BUILD_TEST)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd/Catch2)

  add_executable(${PROJECT_NAME}_test test/test_main.cpp)
  target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME} Catch2::Catch2WithMain)

  enable_testing()
  add_test(NAME PathFindingTest COMMAND ${PROJECT_NAME}_test)
endif ()

if (PATH_FINDING_BUILD_VISUAL)
  find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Widgets)

  set(CMAKE_AUTOMOC ON)
  set(CMAKE_AUTORCC ON)

  set(GUI_SOURCES
      visual/src/main.cpp
      visual/src/GridModel.cpp
      visual/src/GridModel.h
      visual/src/PathFinderController.cpp
      visual/src/PathFinderController.h
      visual/src/LanguageManager.cpp
      visual/src/LanguageManager.h
      visual/resources.qrc)

  add_executable(${PROJECT_NAME}_visual ${GUI_SOURCES})
  target_link_libraries(${PROJECT_NAME}_visual PRIVATE ${PROJECT_NAME} Qt6::Core Qt6::Gui Qt6::Quick Qt6::Widgets)
  target_include_directories(${PROJECT_NAME}_visual PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
                                                            ${CMAKE_CURRENT_SOURCE_DIR}/visual/src)
  target_compile_definitions(${PROJECT_NAME}_visual PRIVATE QT_NO_KEYWORDS)
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
