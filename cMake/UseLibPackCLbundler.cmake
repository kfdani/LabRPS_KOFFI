set(ENV{PATH} "${LABRPS_LIBPACK_DIR};$ENV{PATH}")
set(ENV{CMAKE_PREFIX_PATH} ${LABRPS_LIBPACK_DIR})

set(Boost_INCLUDE_DIR ${LABRPS_LIBPACK_DIR}/include CACHE PATH "" FORCE)

set(OCE_DIR ${LABRPS_LIBPACK_DIR}/lib/cmake CACHE PATH "" FORCE)

set(SWIG_EXECUTABLE ${LABRPS_LIBPACK_DIR}/bin/swig/swig.exe CACHE FILEPATH "Swig" FORCE)


# Set paths to cmake config files for each Qt module
set(Qt5_ROOT_DIR ${LABRPS_LIBPACK_DIR} CACHE PATH "")

set (Qt5_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5 CACHE PATH "")
set (Qt5AxBase_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5AxBase CACHE PATH "")
set (Qt5AxContainer_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5AxContainer CACHE PATH "")
set (Qt5AxServer_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5AxServer CACHE PATH "")
set (Qt5Concurrent_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5Concurrent CACHE PATH "")
set (Qt5Core_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5Core CACHE PATH "")
set (Qt5DBus_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5DBus CACHE PATH "")
set (Qt5Designer_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5Designer CACHE PATH "")
set (Qt5Gui_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5Gui CACHE PATH "")
set (Qt5Help_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5Help CACHE PATH "")
set (Qt5LinguistTools_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5LinguistTools CACHE PATH "")
set (Qt5Multimedia_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5Multimedia CACHE PATH "")
set (Qt5MultimediaWidgets_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5MultimediaWidgets CACHE PATH "")
set (Qt5Network_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5Network CACHE PATH "")
set (Qt5OpenGL_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5OpenGL CACHE PATH "")
set (Qt5OpenGLExtensions_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5OpenGLExtensions CACHE PATH "")
set (Qt5PrintSupport_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5PrintSupport CACHE PATH "")
set (Qt5Qml_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5Qml CACHE PATH "")
set (Qt5Quick_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5Quick CACHE PATH "")
set (Qt5QuickTest_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5QuickTest CACHE PATH "")
set (Qt5QuickWidgets_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5QuickWidgets CACHE PATH "")
set (Qt5Sql_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5Sql CACHE PATH "")
set (Qt5Svg_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5Svg CACHE PATH "")
set (Qt5Test_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5Test CACHE PATH "")
set (Qt5UiPlugin_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5UiPlugin CACHE PATH "")
set (Qt5UiTools_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5UiTools CACHE PATH "")
set (Qt5Widgets_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5Widgets CACHE PATH "")
set (Qt5Xml_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5Xml CACHE PATH "")
set (Qt5XmlPatterns_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5XmlPatterns CACHE PATH "")
set (Qt5DataVisualization_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5DataVisualization CACHE PATH "")
set (Qt5Script_DIR ${Qt5_ROOT_DIR}/lib/cmake/Qt5Script CACHE PATH "")

find_library(XercesC_LIBRARY_RELEASE xerces-c_3 "${LABRPS_LIBPACK_DIR}/lib")
find_library(XercesC_LIBRARY_DEBUG xerces-c_3D "${LABRPS_LIBPACK_DIR}/lib")
set (XercesC_LIBRARIES debug ${XercesC_LIBRARY_DEBUG} optimized ${XercesC_LIBRARY_RELEASE})
set(XercesC_FOUND TRUE) 

find_library(COIN3D_LIBRARY_RELEASE coin4 "${LABRPS_LIBPACK_DIR}/lib")
find_library(COIN3D_LIBRARY_DEBUG coin4d "${LABRPS_LIBPACK_DIR}/lib")
set(COIN3D_LIBRARIES optimized ${COIN3D_LIBRARY_RELEASE}
                     debug ${COIN3D_LIBRARY_DEBUG})
set(COIN3D_FOUND TRUE)

set(NETGENDATA ${LABRPS_LIBPACK_DIR}/include/netgen)

if(LABRPS_USE_FREETYPE)
    set(FREETYPE_INCLUDE_DIR_freetype2 ${LABRPS_LIBPACK_DIR}/include/freetype2)
endif(LABRPS_USE_FREETYPE)

link_directories(${LABRPS_LIBPACK_DIR}/lib)
