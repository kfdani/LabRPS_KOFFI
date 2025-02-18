if(WIN32)
    add_definitions(-DFCBase)
    add_definitions(-DPYCXX_DLL)
    add_definitions(-DBOOST_DYN_LINK)
    add_definitions(-DZIPIOS_UTF8)
endif(WIN32)

include_directories(
    ${CMAKE_BINARY_DIR}/src
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
    ${XercesC_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIR}
    ${PYCXX_INCLUDE_DIR}
)

if(MSVC)
    set(LabRPSBase_LIBS
        ${Boost_LIBRARIES}
        ${XercesC_LIBRARIES}
        ${ZLIB_LIBRARIES}
        debug MSVCRTD.LIB
        debug MSVCPRTD.LIB
        optimized MSVCRT.LIB
        optimized MSVCPRT.LIB
        Rpcrt4.lib
        version.lib
    )

    #Universal C runtime introduced in VS 2015 (cl version 19)
    if (NOT (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19"))
        list(APPEND LabRPSBase_LIBS
            debug vcruntimed.lib
            debug ucrtd.lib
            debug concrtd.lib
            optimized vcruntime.lib
            optimized ucrt.lib
            optimized concrt.lib
        )
    endif()
elseif(MINGW)
    set(LabRPSBase_LIBS
        ${XercesC_LIBRARIES}
        ${Boost_LIBRARIES}
        ${ZLIB_LIBRARIES}
        Rpcrt4.lib
    )
else(MSVC)
    set(LabRPSBase_LIBS
        ${XercesC_LIBRARIES}
        ${Boost_LIBRARIES}
        ${ZLIB_LIBRARIES}
    )
endif(MSVC)

if (BUILD_QT5)
    include_directories(
        ${Qt5Core_INCLUDE_DIRS}
    )
    list(APPEND LabRPSBase_LIBS ${Qt5Core_LIBRARIES})
else()
    include_directories(
        ${QT_QTCORE_INCLUDE_DIR}
    )
    list(APPEND LabRPSBase_LIBS ${QT_QTCORE_LIBRARY})
endif()

if (BUILD_DYNAMIC_LINK_PYTHON)
    list(APPEND LabRPSBase_LIBS ${PYTHON_LIBRARIES})
endif (BUILD_DYNAMIC_LINK_PYTHON)

# needed for OpenSUSE
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    list(APPEND LabRPSBase_LIBS -lutil -ldl)
endif()

generate_from_xml(TypePy)
generate_from_xml(BaseClassPy)
generate_from_xml(PersistencePy)
generate_from_xml(VectorPy)
generate_from_xml(UnitPy)
generate_from_xml(QuantityPy)
generate_from_any(Parameter.xsd Parameter.inl xmlSchemeString)

if(SWIG_FOUND)
    # Create the file swigpyrun.hh and then compare with the file swigpyrun.h.
    # If the files are different or if swigpyrun.h doesn't exist then copy swigpyrun.hh.
    # This is to avoid to having to build the target each time cmake configure is executed.
    execute_process(COMMAND ${SWIG_EXECUTABLE} -python -external-runtime ${CMAKE_CURRENT_BINARY_DIR}/swigpyrun.hh)

    rps_copy_file_if_different(
        "${CMAKE_CURRENT_BINARY_DIR}/swigpyrun.hh"
        "${CMAKE_CURRENT_BINARY_DIR}/swigpyrun.h"
    )
    add_definitions(-DHAVE_SWIG=1)
endif(SWIG_FOUND)

if (EXISTS ${CMAKE_SOURCE_DIR}/src/zipios++ AND NOT LABRPS_USE_EXTERNAL_ZIPIOS)
SET(zipios_SRCS
    ../zipios++/backbuffer.h
    ../zipios++/basicentry.cpp
    ../zipios++/basicentry.h
    ../zipios++/collcoll.cpp
    ../zipios++/collcoll.h
    ../zipios++/deflateoutputstreambuf.cpp
    ../zipios++/deflateoutputstreambuf.h
    ../zipios++/dircoll.cpp
    ../zipios++/dircoll.h
    ../zipios++/directory.cpp
    ../zipios++/directory.h
    ../zipios++/fcoll.cpp
    ../zipios++/fcoll.h
    ../zipios++/fcollexceptions.cpp
    ../zipios++/fcollexceptions.h
    ../zipios++/fileentry.cpp
    ../zipios++/fileentry.h
    ../zipios++/filepath.cpp
    ../zipios++/filepath.h
    ../zipios++/filterinputstreambuf.cpp
    ../zipios++/filterinputstreambuf.h
    ../zipios++/filteroutputstreambuf.cpp
    ../zipios++/filteroutputstreambuf.h
    ../zipios++/gzipoutputstream.cpp
    ../zipios++/gzipoutputstream.h
    ../zipios++/gzipoutputstreambuf.cpp
    ../zipios++/gzipoutputstreambuf.h
    ../zipios++/inflateinputstreambuf.cpp
    ../zipios++/inflateinputstreambuf.h
    ../zipios++/meta-iostreams.h
    ../zipios++/outputstringstream.h
    ../zipios++/simplesmartptr.h
    ../zipios++/virtualseeker.h
    ../zipios++/zipfile.cpp
    ../zipios++/zipfile.h
    ../zipios++/ziphead.cpp
    ../zipios++/ziphead.h
    ../zipios++/zipheadio.cpp
    ../zipios++/zipheadio.h
    ../zipios++/zipinputstream.cpp
    ../zipios++/zipinputstream.h
    ../zipios++/zipinputstreambuf.cpp
    ../zipios++/zipinputstreambuf.h
    ../zipios++/zipios_common.h
    ../zipios++/zipios-config.h
    ../zipios++/zipios_defs.h
    ../zipios++/zipoutputstreambuf.cpp
    ../zipios++/zipoutputstreambuf.h
    ../zipios++/zipoutputstream.cpp
    ../zipios++/zipoutputstream.h
)
SOURCE_GROUP("zipios" FILES ${zipios_SRCS})
endif ()

SOURCE_GROUP("pycxx" FILES ${PYCXX_SOURCES})

SET(LabRPSBase_XML_SRCS
    BaseClassPy.xml
    PersistencePy.xml
    VectorPy.xml
    QuantityPy.xml
    TypePy.xml
    UnitPy.xml
)
SOURCE_GROUP("XML" FILES ${LabRPSBase_XML_SRCS})

SET(LabRPSBase_UNITAPI_SRCS
    UnitsApi.cpp
    UnitsApiPy.cpp
    UnitsApi.h
    UnitsSchema.h
    UnitsSchema.cpp
    UnitsSchemaInternal.h
    UnitsSchemaInternal.cpp
    UnitsSchemaMKS.h
    UnitsSchemaMKS.cpp
    UnitsSchemaImperial1.h
    UnitsSchemaImperial1.cpp
    UnitsSchemaCentimeters.h
    UnitsSchemaCentimeters.cpp
    UnitsSchemaMmMin.h
    UnitsSchemaMmMin.cpp
    UnitsSchemaFemMilliMeterNewton.h
    UnitsSchemaFemMilliMeterNewton.cpp
    Quantity.h
    Quantity.cpp
    QuantityPyImp.cpp
    QuantityParser.l
    QuantityParser.y
    Unit.h
    Unit.cpp
    UnitPyImp.cpp
    
)
SOURCE_GROUP("Units" FILES ${LabRPSBase_UNITAPI_SRCS})

SET(SWIG_SRCS
    swigpyrun.cpp
)

SET(LabRPSBase_CPP_SRCS
    Base64.cpp
    BaseClass.cpp
    BaseClassPyImp.cpp
    BindingManager.cpp
    Console.cpp
    ConsoleObserver.cpp
    Debugger.cpp
    DualQuaternion.cpp
    Exception.cpp
    ExceptionFactory.cpp
    Factory.cpp
    FileInfo.cpp
    FileTemplate.cpp
    FutureWatcherProgress.cpp
    gzstream.cpp
    gzstream.h
    Handle.cpp
    InputSource.cpp
    Interpreter.cpp
    MemDebug.cpp
    Parameter.xsd
    Parameter.cpp
    ParameterPy.cpp
    Persistence.cpp
    PersistencePyImp.cpp
    PyExport.cpp
    PyObjectBase.cpp
    Reader.cpp
    Sequencer.cpp
    SmartPtrPy.cpp
    Stream.cpp
    Swap.cpp
    ${SWIG_SRCS}
    TimeInfo.cpp
    Tools.cpp
    Translate.cpp
    Type.cpp
    TypePyImp.cpp
    Uuid.cpp
    Vector3D.cpp
    VectorPyImp.cpp
    Writer.cpp
    XMLTools.cpp
)

SET(SWIG_HEADERS
    swigpyrun.inl
)

SET(LabRPSBase_HPP_SRCS
    Base64.h
    BaseClass.h
    BindingManager.h
    Bitmask.h
    Console.h
    ConsoleObserver.h
    Converter.h
    Debugger.h
    DualNumber.h
    DualQuaternion.h
    Exception.h
    ExceptionFactory.h
    Factory.h
    FileInfo.h
    FileTemplate.h
    FutureWatcherProgress.h
    fdstream.hpp
    gzstream.h
    Handle.h
    InputSource.h
    Interpreter.h
    MemDebug.h
    Observer.h
    Parameter.h
    Persistence.h
    PyExport.h
    PyObjectBase.h
    Reader.h
    Sequencer.h
    SmartPtrPy.h
    Stream.h
    Swap.h
    ${SWIG_HEADERS}
    TimeInfo.h
    Tools.h
    Translate.h
    Type.h
    Uuid.h
    Vector3D.h
    Writer.h
    XMLTools.h
)

SET(LabRPSBase_SRCS
    ${PYCXX_SOURCES}
    ${LabRPSBase_CPP_SRCS}
    ${LabRPSBase_HPP_SRCS}
    ${LabRPSBase_XML_SRCS}
    ${LabRPSBase_UNITAPI_SRCS}
    PyTools.c
    PyTools.h
    PreCompiled.cpp
    PreCompiled.h
)

IF (MSVC)
    SET(LabRPSBase_SRCS
        ${LabRPSBase_SRCS}
        StackWalker.cpp
        StackWalker.h
)
ENDIF(MSVC)

# Use external zipios++ if specified.
if(LABRPS_USE_EXTERNAL_ZIPIOS)
    find_library(ZIPIOS_LIBRARY zipios)
    find_path(ZIPIOS_INCLUDES zipios++/zipios-config.h)
    if(ZIPIOS_LIBRARY)
        message(STATUS "Found zipios++: ${ZIPIOS}")
    endif()
    if(ZIPIOS_INCLUDES)
        message(STATUS "Found zipios++ headers.")
    endif()
    if(ZIPIOS_LIBRARY AND ZIPIOS_INCLUDES)
        list(APPEND LabRPSBase_LIBS ${ZIPIOS_LIBRARY})
        include_directories(${ZIPIOS_INCLUDES})
    else()
        message(FATAL_ERROR "Using external zipios++ was specified but was not found.")
    endif()
else(LABRPS_USE_EXTERNAL_ZIPIOS)
    list(APPEND LabRPSBase_SRCS ${zipios_SRCS})
endif(LABRPS_USE_EXTERNAL_ZIPIOS)


if(LABRPS_USE_PCH)
    add_definitions(-D_PreComp_)
    ADD_MSVC_PRECOMPILED_HEADER(LabRPSBase PreCompiled.h PreCompiled.cpp LabRPSBase_CPP_SRCS)
endif(LABRPS_USE_PCH)

add_library(LabRPSBase SHARED ${LabRPSBase_SRCS})

target_link_libraries(LabRPSBase ${LabRPSBase_LIBS})
set_target_properties(LabRPSBase PROPERTIES FOLDER "Base")

SET_BIN_DIR(LabRPSBase LabRPSBase)

if(WIN32)
    INSTALL(TARGETS LabRPSBase
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
else(WIN32)
    INSTALL(TARGETS LabRPSBase
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif(WIN32)
