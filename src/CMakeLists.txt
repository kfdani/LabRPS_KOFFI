add_subdirectory(Build)
add_subdirectory(3rdParty)
add_subdirectory(Base)
add_subdirectory(App)
add_subdirectory(Main)
add_subdirectory(Mod)
add_subdirectory(Ext)
add_subdirectory(Doc)
add_subdirectory(Libraries)

if(BUILD_GUI)
    add_subdirectory(Gui)
    if(UNIX AND NOT APPLE)
        add_subdirectory(XDGData)
    endif()
endif(BUILD_GUI)

if(BUILD_TEMPLATE)
    add_subdirectory(Tools/_TEMPLATE_)
endif(BUILD_TEMPLATE)


if(LABRPS_CREATE_MAC_APP)
    add_subdirectory(MacAppBundle)
endif(LABRPS_CREATE_MAC_APP)
