
IF (BUILD_GUI)
    PYSIDE_WRAP_RC(_TEMPLATEPY__QRC_SRCS Resources/_TEMPLATEPY_.qrc)
ENDIF (BUILD_GUI)

SET(_TEMPLATEPY__SRCS
    Init.py
    InitGui.py
    _TEMPLATEPY_.py
    _TEMPLATEPY_Gui.py
)
SOURCE_GROUP("" FILES ${_TEMPLATEPY__SRCS})

SET(_TEMPLATEPY_GuiIcon_SVG
    Resources/icons/_TEMPLATEPY_Workbench.svg
)

ADD_CUSTOM_TARGET(_TEMPLATEPY_ ALL
    SOURCES ${_TEMPLATEPY__SRCS} ${_TEMPLATEPY__QRC_SRCS} ${_TEMPLATEPY_GuiIcon_SVG}
)

rps_copy_sources(_TEMPLATEPY_ "${CMAKE_BINARY_DIR}/Mod/_TEMPLATEPY_" ${_TEMPLATEPY__SRCS})
rps_copy_sources(_TEMPLATEPY_ "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_DATADIR}/Mod/_TEMPLATEPY_" ${_TEMPLATEPY_GuiIcon_SVG})

IF (BUILD_GUI)
    rps_target_copy_resource(_TEMPLATEPY_
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_BINARY_DIR}/Mod/_TEMPLATEPY_
        _TEMPLATEPY__rc.py)
ENDIF (BUILD_GUI)

INSTALL(
    FILES
        ${_TEMPLATEPY__SRCS}
        ${_TEMPLATEPY__QRC_SRCS}
    DESTINATION
        Mod/_TEMPLATEPY_
)

INSTALL(FILES ${_TEMPLATEPY_GuiIcon_SVG} DESTINATION "Mod/_TEMPLATEPY_/Resources/icons")
