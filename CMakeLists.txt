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
add_executable (CMakeTarget "ParticleSimulator.cpp" "ParticleSimulator.h" "SimulatorGUI.cpp" "SimulatorGUI.h" "Objects/Particle.cpp" "Objects/Particle.h" "Objects/Wall.h" "Objects/Wall.cpp"    "CollisionManager.h" "CollisionManager.cpp" "Objects/ObjectManager.h" "Objects/ObjectManager.cpp"   "Structures.h" "Objects/DebugCircle.cpp" "Objects/DebugCircle.h"   "Globals.h"  "Objects/Player.cpp" "Objects/Player.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeTarget PROPERTY CXX_STANDARD 20)
endif()

# Copy DLLs to the output directory
add_custom_command(TARGET CMakeTarget POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/vcpkg/blds/sdl2/x64-windows-dbg/SDL2d.dll"
		$<TARGET_FILE_DIR:CMakeTarget>)

# Add installation prefix of vcpkg to CMAKE_PREFIX_PATH
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows)

# Packages
find_package(glad CONFIG REQUIRED)
target_link_libraries(CMakeTarget PRIVATE glad::glad )

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(CMakeTarget PRIVATE
	$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
	$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(CMakeTarget PRIVATE imgui::imgui)

find_package(SDL2_image CONFIG REQUIRED)
target_link_libraries(CMakeTarget PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)

# TODO: Add tests and install targets if needed.
