
project(QtPropertyBrowser)

find_package(Qt5Widgets REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(src)

macro(AddFileDirectory dir)
    file(GLOB SRC ${dir}/*.*)

    set(LOCAL_SRC_ALL ${LOCAL_SRC_ALL} ${SRC})
endmacro()

AddFileDirectory(src)

add_library(QtPropertyBrowser STATIC ${LOCAL_SRC_ALL})
target_link_libraries(QtPropertyBrowser Qt5::Widgets)
set_target_properties(QtPropertyBrowser PROPERTIES FOLDER "Libraries")

SET_BIN_DIR(QtPropertyBrowser QtPropertyBrowser)

if(WIN32)
    INSTALL(TARGETS QtPropertyBrowser
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
else(WIN32)
    INSTALL(TARGETS QtPropertyBrowser
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif(WIN32)