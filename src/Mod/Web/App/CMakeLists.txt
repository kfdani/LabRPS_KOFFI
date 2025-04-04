
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIR}
)

if(BUILD_QT5)
    include_directories(
        ${Qt5Network_INCLUDE_DIRS}
    )
    set(Web_LIBS
         LabRPSApp
         ${Qt5Network_LIBRARIES}
    )
else()
    set(Web_LIBS
         LabRPSApp
         ${QT_LIBRARIES}
    )
endif()

SET(Web_SRCS
    AppWeb.cpp
    PreCompiled.cpp
    PreCompiled.h
    Server.cpp
    Server.h
)

add_library(Web SHARED ${Web_SRCS})
target_link_libraries(Web ${Web_LIBS})
set_target_properties(Web PROPERTIES FOLDER "Workbenches/Web")


SET_BIN_DIR(Web Web /Mod/Web)
SET_PYTHON_PREFIX_SUFFIX(Web)

install(TARGETS Web DESTINATION ${CMAKE_INSTALL_LIBDIR})
