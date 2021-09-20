cmake_minimum_required(VERSION 3.5)

project(qt_task2_mvc)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED Widgets Linguist Tools)

include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        treeitem.h
        treemodel.h
        treeitem.cpp
        treemodel.cpp
        mainwindow.ui
)

set(
        FILES_TO_TRANSLATE
        main.cpp
        mainwindow.cpp
        mainwindow.ui
)

set(
        TS_FILES
        translations/qt_mvc_en_US.ts
        translations/qt_mvc_ru_RU.ts
)

option(
        UPDATE_TRANSLATIONS
        "Update source translation (files translations/*.ts)."
        ON
)

if(UPDATE_TRANSLATIONS)
        message(STATUS "UPDATE_TRANSLATIONS option is set.")
        qt_create_translation(QM_FILES ${FILES_TO_TRANSLATE} ${TS_FILES})
else()
        qt_add_translation(QM_FILES &{TS_FILES})
endif()

add_custom_target(
        translations
        DEPENDS ${QM_FILES}
)

set_property(
        DIRECTORY
        PROPERTY CLEAN_NO_CUSTOM TRUE
)

qt_wrap_cpp(
	MOC_WRAPPERS
	mainwindow.h
	treeitem.h
	treemodel.h
	)
	
qt_wrap_ui(
	UIC_WRAPPERS
	mainwindow.ui
)	
	
add_executable(
	qt_task2_mvc WIN32
	${PROJECT_SOURCES}
	${FILES_TO_TRANSLATE}
	${MOC_WRAPPERS}
	${UIC_WRAPPERS})	


add_dependencies(
        qt_task2_mvc translations)

target_link_libraries(qt_task2_mvc PRIVATE Qt6::Widgets)


#installation

install(
    TARGETS qt_task2_mvc
    RUNTIME DESTINATION bin)

install(
    FILES ${QM_FILES}
    DESTINATION translations)
	
get_property(
	LIB_QTCORE_PATH
	TARGET Qt6::Core
	PROPERTY IMPORTED_LOCATION_RELEASE
)	

get_filename_component(
	QT_LIBRARY_DIR "${LIB_QTCORE_PATH}" DIRECTORY
)

set(
	EXE_PATH bin/qt_task2_mvc${CMAKE_EXECUTABLE_SUFFIX})
	
include(DeployQt4)

install_qt4_executable(
	"${EXE_PATH}"
	""
	""
	"${QT_LIBRARY_DIR}"
)	

get_filename_component(
	QT_PATH "${LIB_QTCORE_PATH}" PATH)
	
get_filename_component(
	QT_PATH "${QT_PATH}" PATH)	
	
install(
	DIRECTORY "${QT_PATH}/plugins/platforms"
	DESTINATION bin)