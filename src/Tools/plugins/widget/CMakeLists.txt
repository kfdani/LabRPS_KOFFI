if (PROJECT_NAME)
    set(IS_SUB_PROJECT TRUE)
endif()

if (NOT IS_SUB_PROJECT)
    project(LabRPS_widgets)
    cmake_minimum_required(VERSION 3.2.0)

    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    set(CMAKE_AUTOMOC ON)

    find_package(Qt5Widgets REQUIRED)
    find_package(Qt5Designer REQUIRED)
endif()

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Designer_INCLUDE_DIRS}
)

add_library(LabRPS_widgets SHARED
    customwidgets.cpp
    customwidgets.h
    plugin.cpp
    plugin.h
)

set(LabRPS_widgets_LIBS
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Designer_LIBRARIES}
)

if(MSVC)
    list(APPEND LabRPS_widgets_LIBS
        debug msvcrtd.lib
        debug msvcprtd.lib
        optimized msvcrt.lib
        optimized msvcprt.lib
    )

    #Universal C runtime introduced in VS 2015 (cl version 19)
    if (NOT (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19"))
        list(APPEND LabRPS_widgets_LIBS
            debug vcruntimed.lib
            debug ucrtd.lib
            debug concrtd.lib
            optimized vcruntime.lib
            optimized ucrt.lib
            optimized concrt.lib
        )
    endif()
endif()

target_link_libraries(LabRPS_widgets PRIVATE ${LabRPS_widgets_LIBS})
target_compile_options(LabRPS_widgets PRIVATE ${COMPILE_OPTIONS})


# Get the install location of a plugin to determine the path to designer plguins
get_target_property(QMAKE_EXECUTABLE Qt5::qmake LOCATION)
exec_program(${QMAKE_EXECUTABLE} ARGS "-query QT_INSTALL_PLUGINS" RETURN_VALUE return_code OUTPUT_VARIABLE DEFAULT_QT_PLUGINS_DIR )
set(DESIGNER_PLUGIN_LOCATION ${DEFAULT_QT_PLUGINS_DIR}/designer CACHE PATH "Path where the plugin will be installed to")

if (NOT IS_SUB_PROJECT)
    message(STATUS "Plugin will be installed to: ${DESIGNER_PLUGIN_LOCATION}")
else()
    set (libLabRPS_widgets "${CMAKE_SHARED_LIBRARY_PREFIX}LabRPS_widgets${CMAKE_SHARED_LIBRARY_SUFFIX}" PARENT_SCOPE)
endif()


INSTALL(TARGETS LabRPS_widgets
    RUNTIME DESTINATION "$ENV{DESTDIR}${DESIGNER_PLUGIN_LOCATION}"
    LIBRARY DESTINATION "$ENV{DESTDIR}${DESIGNER_PLUGIN_LOCATION}"
)
