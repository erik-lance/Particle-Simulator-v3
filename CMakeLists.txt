# CMakeList.txt : CMake project for Particle Simulator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Particle Simulator")

# Add source to this project's executable.
add_executable (CMakeTarget "Particle Simulator.cpp" "Particle Simulator.h" "SimulatorGUI.cpp" "SimulatorGUI.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeTarget PROPERTY CXX_STANDARD 20)
endif()

# Add installation prefix of vcpkg to CMAKE_PREFIX_PATH
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows)

# Packages
find_package(glad CONFIG REQUIRED)
target_link_libraries(CMakeTarget PRIVATE glad::glad )

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(CMakeTarget PRIVATE glfw)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(CMakeTarget PRIVATE imgui::imgui)

# TODO: Add tests and install targets if needed.
