cmake_minimum_required(VERSION 3.1.0)

set(PRO_NAME city)

project(${PRO_NAME} VERSION 1.0 LANGUAGES CXX)

find_package(Qt5Widgets REQUIRED)

set(FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS} -std::c++11")
#include_directories(${Qt5Widgets_INCLUDE_DIRS})
#add_definitions(${Qt5Widgets_DEFINITIONS})

set(SOURCE
	citizen.cpp
	lift.cpp
	loader.cpp
	main.cpp
	npc.cpp
	settings_form.cpp
	sprite.cpp
	town.cpp
	worker.cpp
)
set(HEADER
	citizen.h
	lift.h
	loader.h
	npc.h
	settings_form.h
	sprite.h
	town.h
	worker.h
)
set(UI
	settings_form.ui
)
set(QRC
	res.qrc
)

qt5_wrap_cpp(
	MOC_WRAPPERS ${HEADER})
	
qt5_add_resources(
	QRC_WRAPPERS ${QRC}
	OPTIONS -no-compress)
	
qt5_wrap_ui(
	UI_WRAPPERS ${UI})

add_executable(
	${PRO_NAME} WIN32
	${SOURCE}
	${HEADER}
	${UI}
	${SRC}
	#${MOC_WRAPPERS}
	#${QRC_WRAPPERS}
	#${UI_WRAPPERS}
		
	
)

target_link_libraries(${PRO_NAME} Qt5::Widgets)
