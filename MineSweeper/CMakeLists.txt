cmake_minimum_required (VERSION 3.8)

if(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

project ("MineSweeper")

set(SRCROOT ${PROJECT_SOURCE_DIR}/src)
set(LIBROOT ${PROJECT_SOURCE_DIR}/extlibs)

# SFML
set(SFML_DIR ${LIBROOT}/SFML-2.5.1/lib/cmake/SFML)
find_package(SFML 2.5 COMPONENTS graphics main REQUIRED)

# TGUI
set(TGUI_DIR ${LIBROOT}/TGUI-0.8/lib/cmake/TGUI)
find_package(TGUI 0.8 REQUIRED)

# RapidXML
set(RAPIDXML 
	${LIBROOT}/rapidxml-1.13/rapidxml.hpp
	${LIBROOT}/rapidxml-1.13/rapidxml_print.hpp
)

set(
	SOURCEFILES 
	${SRCROOT}/Main.cpp
	${SRCROOT}/MineSweeper.cpp ${SRCROOT}/MineSweeper.h
	${SRCROOT}/Gui.cpp ${SRCROOT}/Gui.h
	${SRCROOT}/Tiles.cpp ${SRCROOT}/Tiles.h
	${SRCROOT}/Tilemap.cpp ${SRCROOT}/Tilemap.h
	${SRCROOT}/Utils.cpp ${SRCROOT}/Utils.h
)

set(
	RESOURCEFILES
	${PROJECT_SOURCE_DIR}/properties.rc
)

add_executable (
	MineSweeper 
	WIN32
	${SOURCEFILES} 
	${RESOURCEFILES}
	${RAPIDXML}
)

# Link SFML, TGUI
target_link_libraries(MineSweeper sfml-graphics sfml-main tgui)
