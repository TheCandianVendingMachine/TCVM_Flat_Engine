cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(FlatEngine VERSION 1.0.0 LANGUAGES CXX)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/extlib/headers)
include_directories(${PROJECT_SOURCE_DIR}/extlib/headers)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/;${CMAKE_MODULE_PATH}")

# Requires SFML to build
find_package(SFML 2 COMPONENTS audio graphics window system REQUIRED)
if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
else()
	set(SFML_ROOT "" CACHE PATH "SFML top-level directory")
	message("\n-> SFML directory not found. Set SFML_ROOT to SFML's top-level path (containing \"include\" and \"lib\" directories).")
	message("-> Make sure the SFML libraries with the same configuration (Release/Debug, Static/Dynamic) exist.\n")
endif()

find_package(Lua REQUIRED)

macro(fe_link_sfml FE_TARGET)
	target_link_libraries(${FE_TARGET} ${SFML_LIBRARIES})
endmacro()
	
macro(fe_link_lua FE_TARGET)
	target_link_libraries(${FE_TARGET} ${LUA_LIBRARIES})
endmacro()
	
macro(fe_link_sfml_dependencies FE_TARGET)
	target_link_libraries(${FE_TARGET} ${SFML_DEPENDENCIES})
endmacro()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

add_subdirectory(src)

set_target_properties(${PROJECT_NAME} PROPERTIES
            CXX_STANDARD 17)

					
install(DIRECTORY include
				DESTINATION .)

install(FILES License.txt
				DESTINATION .)
				
install(DIRECTORY extlib/headers
				DESTINATION .)
