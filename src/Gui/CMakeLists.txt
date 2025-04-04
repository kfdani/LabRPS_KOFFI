# Aspect framework legacy code
add_compile_definitions(LEGACY_CODE_0_2_x)

# Enable OpenGL for QCustomPlot
add_compile_definitions(QCUSTOMPLOT_USE_OPENGL)

add_compile_definitions(RPS_BUILDING_CORE)

#add_subdirectory(Icons)
add_subdirectory(Stylesheets)
add_subdirectory(PreferencePacks)
add_subdirectory(PreferencePackTemplates)

if(WIN32)
add_definitions(-DRPSGui -DQSINT_MAKEDLL -DOVR_OS_WIN32 -DQUARTER_INTERNAL -DQUARTER_MAKE_DLL -DCOIN_DLL)
endif(WIN32)

IF(CMAKE_BUILD_TYPE)
    add_definitions(-DCMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}")
ENDIF(CMAKE_BUILD_TYPE)

if (BUILD_VR)
    add_definitions(-DBUILD_VR )
endif(BUILD_VR)

if (BUILD_ADDONMGR)
    add_definitions(-DBUILD_ADDONMGR )
endif(BUILD_ADDONMGR)

include_directories(
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_BINARY_DIR}/..
    ${CMAKE_CURRENT_BINARY_DIR}/Language
    ${CMAKE_CURRENT_BINARY_DIR}/propertyeditor
    ${CMAKE_CURRENT_BINARY_DIR}/TaskView
    ${Boost_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
    ${XercesC_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIR}
)

if(MSVC)
    set(LabRPSGui_LIBS
         LabRPSApp
         QtPropertyBrowser
         Alphaplot
    )

else(MSVC)
    set(LabRPSGui_LIBS
        LabRPSApp
        ${Boost_LIBRARIES}
        QtPropertyBrowser
        Alphaplot
    )
endif(MSVC)

if (BUILD_QT5)

    if (Qt5WebEngineWidgets_FOUND AND BUILD_WITH_CONDA)
        add_definitions(-DQTWEBENGINE)
    endif()

    include_directories(
        ${Qt5Core_INCLUDE_DIRS}
        ${Qt5Widgets_INCLUDE_DIRS}
        ${Qt5PrintSupport_INCLUDE_DIRS}
        ${Qt5Svg_INCLUDE_DIRS}
        ${Qt5Network_INCLUDE_DIRS}
        ${Qt5UiTools_INCLUDE_DIRS}
        ${Qt5DataVisualization_INCLUDE_DIRS}

    )
    list(APPEND LabRPSGui_LIBS
        ${Qt5Core_LIBRARIES}
        ${Qt5Widgets_LIBRARIES}
        ${Qt5PrintSupport_LIBRARIES}
        ${Qt5Svg_LIBRARIES}
        ${Qt5Network_LIBRARIES}
        ${Qt5UiTools_LIBRARIES}
        ${Qt5DataVisualization_LIBRARIES}

    )
    if(${Qt5WinExtras_FOUND})
      include_directories(
        ${Qt5WinExtras_INCLUDE_DIRS}
      )
      list(APPEND LabRPSGui_LIBS
        ${Qt5WinExtras_LIBRARIES}
      )
    endif()
else()
    include_directories(
        ${QT_INCLUDE_DIR}
    )
    list(APPEND LabRPSGui_LIBS
        ${QT_LIBRARIES}
        ${QT_QTUITOOLS_LIBRARY}
    )
endif()

IF(SPNAV_FOUND)
    if(SPNAV_USE_X11)
      add_definitions(-DSPNAV_USE_X11)
      if (BUILD_QT5 AND UNIX AND NOT APPLE)
        find_package(Qt5X11Extras REQUIRED)
        include_directories(${Qt5X11Extras_INCLUDE_DIRS})
        list(APPEND LabRPSGui_LIBS ${Qt5X11Extras_LIBRARIES})
      endif()
      find_package(X11 QUIET)
      if (X11_FOUND)
        list(APPEND LabRPSGui_LIBS
            ${X11_X11_LIB}
        )
      endif(X11_FOUND)
    endif(SPNAV_USE_X11)

    add_definitions(-DSPNAV_FOUND)
    include_directories(
        ${SPNAV_INCLUDE_DIR}
    )
    list(APPEND LabRPSGui_LIBS
        ${SPNAV_LIBRARIES}
    )
ENDIF(SPNAV_FOUND)

IF(OCULUS_FOUND)
    add_definitions(-DOCULUS_FOUND)
    include_directories(
        ${OCULUS_INCLUDE_DIRS}
    )
    list(APPEND LabRPSGui_LIBS
        ${OCULUS_LIBRARIES}
    )
ENDIF(OCULUS_FOUND)

if(LABRPS_USE_SHIBOKEN)
    if (BUILD_QT5)
        add_definitions(-DHAVE_SHIBOKEN2)
    else()
        add_definitions(-DHAVE_SHIBOKEN)
    endif()
    include_directories(
        ${SHIBOKEN_INCLUDE_DIR}
    )
    if (SHIBOKEN_LIBRARY)
        list(APPEND LabRPSGui_LIBS
            ${SHIBOKEN_LIBRARY}
        )
    else (SHIBOKEN_LIBRARY)
        if (TARGET Shiboken2::libshiboken)
            list(APPEND LabRPSGui_LIBS
                Shiboken2::libshiboken
            )
        endif (TARGET Shiboken2::libshiboken)
    endif (SHIBOKEN_LIBRARY)
endif(LABRPS_USE_SHIBOKEN)

if(LABRPS_USE_PYSIDE)
    include_directories(
        ${PYSIDE_INCLUDE_DIR}
        ${PYSIDE_INCLUDE_DIR}/QtCore
        ${PYSIDE_INCLUDE_DIR}/QtGui
    )
    if (PYSIDE_LIBRARY)
        list(APPEND LabRPSGui_LIBS
            ${PYSIDE_LIBRARY}
        )
    else (PYSIDE_LIBRARY)
        if (TARGET PySide2::pyside2)
            list(APPEND LabRPSGui_LIBS
                PySide2::pyside2
            )
        endif (TARGET PySide2::pyside2)
    endif (PYSIDE_LIBRARY)
    if (BUILD_QT5)
        include_directories(
            ${PYSIDE_INCLUDE_DIR}/QtWidgets
        )
        add_definitions(-DHAVE_PYSIDE2)
    else()
        add_definitions(-DHAVE_PYSIDE)
    endif()
endif(LABRPS_USE_PYSIDE)

generate_from_xml(DocumentPy)
generate_from_xml(PythonWorkbenchPy)
generate_from_xml(ViewProviderPy)
generate_from_xml(ViewProviderDocumentObjectPy)
generate_from_xml(ViewProviderExtensionPy)
generate_from_xml(WorkbenchPy)
generate_from_xml(SelectionObjectPy)
generate_from_xml(LinkViewPy)
generate_from_xml(ViewProviderLinkPy)
generate_from_xml(CommandPy)
generate_from_xml(ViewProviderRPSFeaturePy)

generate_from_py(LabRPSGuiInit GuiInitScript.h)

# The XML files
SET(LabRPSGui_XML_SRCS
    ViewProviderDocumentObjectPy.xml
    ViewProviderPy.xml
    ViewProviderExtensionPy.xml
    PythonWorkbenchPy.xml
    WorkbenchPy.xml
    SelectionObjectPy.xml
    DocumentPy.xml
    LinkViewPy.xml
    ViewProviderLinkPy.xml
    CommandPy.xml
    ViewProviderRPSFeaturePy.xml
)
SOURCE_GROUP("XML" FILES ${LabRPSApp_XML_SRCS})

if (BUILD_QT5)
    set_property(SOURCE GraphvizView.h GraphvizView.cpp PROPERTY SKIP_AUTOMOC ON)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/moc_GraphvizView-internal.cpp
        COMMAND ${Qt5Core_MOC_EXECUTABLE} -o ${CMAKE_CURRENT_BINARY_DIR}/moc_GraphvizView-internal.cpp ${CMAKE_CURRENT_SOURCE_DIR}/GraphvizView.cpp
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/GraphvizView.cpp)

    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/moc_GraphvizView.cpp
        COMMAND ${Qt5Core_MOC_EXECUTABLE} -o ${CMAKE_CURRENT_BINARY_DIR}/moc_GraphvizView.cpp ${CMAKE_CURRENT_SOURCE_DIR}/GraphvizView.h
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/GraphvizView.h)
else()
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/moc_GraphvizView-internal.cpp
        COMMAND ${QT_MOC_EXECUTABLE} -o ${CMAKE_CURRENT_BINARY_DIR}/moc_GraphvizView-internal.cpp ${CMAKE_CURRENT_SOURCE_DIR}/GraphvizView.cpp
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/GraphvizView.cpp)
endif()

set_property(SOURCE GraphvizView.cpp APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/moc_GraphvizView-internal.cpp)
set_property(SOURCE GraphvizView.h APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/moc_GraphvizView.cpp)

SET(Gui_UIC_SRCS
    AboutApplication.ui
    DlgActions.ui
    DlgActivateWindow.ui
    DlgUnitsCalculator.ui
    DlgAuthorization.ui
    DlgChooseIcon.ui
    DlgCommands.ui
    DlgCreateNewPreferencePack.ui
    DlgDisplayProperties.ui
    DlgEditor.ui
    DlgGeneral.ui
    DlgInputDialog.ui
    DlgKeyboard.ui
    DlgMacroExecute.ui
    DlgRunExternal.ui
    DlgMacroRecord.ui
    DlgOnlineHelp.ui
    DlgParameter.ui
    DlgParameterFind.ui
    DlgPreferencePackManagement.ui
    DlgPreferences.ui
    DlgProjectInformation.ui
    DlgProjectUtility.ui
    DlgPropertyLink.ui
    DlgReportView.ui
    DlgRevertToBackupConfig.ui
    DlgSettingsCacheDirectory.ui
    DlgSettingsSelection.ui
    DlgSettingsUnits.ui
    DlgSettingsDocument.ui
    DlgSettingsImage.ui
    DlgSettingsLazyLoaded.ui
    DlgSettingsMacro.ui
    DlgSettingsPythonConsole.ui
    DlgCheckableMessageBox.ui
    DlgToolbars.ui
    DlgWorkbenches.ui
    DlgTreeWidget.ui
    DlgLocationAngle.ui
    DlgLocationPos.ui
    DocumentRecovery.ui
    DownloadManager.ui
    DownloadItem.ui
    DlgExpressionInput.ui
    InputVector.ui
    TaskView/TaskAppearance.ui
    TaskView/TaskSelectLinkProperty.ui
    TaskElementColors.ui
    DlgObjectSelection.ui
    DlgAddProperty.ui
    VectorListEditor.ui
    DlgPlugin.ui
    plugininstallerbrowser.ui


)

SET(Gui_RES_SRCS
    Icons/resource.qrc
    Language/translation.qrc
    QSint/actionpanel/schemes.qrc
)

if (BUILD_QT5)
    QT5_ADD_RESOURCES(Gui_QRC_SRCS ${Gui_RES_SRCS})
    QT5_WRAP_UI(Gui_UIC_HDRS ${Gui_UIC_SRCS})
else()
    QT4_ADD_RESOURCES(Gui_QRC_SRCS ${Gui_RES_SRCS})
    QT4_WRAP_UI(Gui_UIC_HDRS ${Gui_UIC_SRCS})
endif()
SOURCE_GROUP("Uic" FILES ${Gui_UIC_HDRS})

# The command sources
SET(Command_CPP_SRCS
    Action.cpp
    ActionFunction.cpp
    Command.cpp
    CommandDoc.cpp
    CommandFeat.cpp
    CommandStructure.cpp
    CommandMacro.cpp
    CommandStd.cpp
    CommandWindow.cpp
    CommandTest.cpp
    CommandView.cpp
    CommandLink.cpp
    CommandPyImp.cpp
)
SET(Command_SRCS
    ${Command_CPP_SRCS}
    Action.h
    ActionFunction.h
    Command.h
    CommandT.h
)
SOURCE_GROUP("Command" FILES ${Command_SRCS})

# The dialog sources
SET(Dialog_CPP_SRCS
    DlgActivateWindowImp.cpp
    DlgCreateNewPreferencePackImp.cpp
    DlgUnitsCalculatorImp.cpp
    DlgDisplayPropertiesImp.cpp
    DlgInputDialogImp.cpp
    DlgMacroExecuteImp.cpp
    DlgRunExternal.cpp
    DlgEditFileIncludePropertyExternal.cpp
    DlgMacroRecordImp.cpp
    DlgParameterImp.cpp
    DlgParameterFind.cpp
    DlgPreferencePackManagementImp.cpp
    DlgProjectInformationImp.cpp
    DlgProjectUtility.cpp
    DlgPropertyLink.cpp
    DlgRevertToBackupConfigImp.cpp
    DlgExpressionInput.cpp
    TaskDlgRelocation.cpp
    DlgCheckableMessageBox.cpp
    TaskCSysDragger.cpp
    DlgUndoRedo.cpp
    InputVector.cpp
    PropertyPage.cpp
    DownloadItem.cpp
    DownloadManager.cpp
    DocumentRecovery.cpp
    TaskElementColors.cpp
    DlgObjectSelection.cpp
    DlgAddProperty.cpp
    VectorListEditor.cpp
    plugininstallerbrowser.cpp

)

SET(Dialog_HPP_SRCS
    DlgActivateWindowImp.h
    DlgCreateNewPreferencePackImp.h
    DlgUnitsCalculatorImp.h
    DlgDisplayPropertiesImp.h
    DlgInputDialogImp.h
    DlgMacroExecuteImp.h
    DlgRunExternal.h
    DlgEditFileIncludePropertyExternal.h
    DlgMacroRecordImp.h
    DlgParameterImp.h
    DlgParameterFind.h
    DlgPreferencePackManagementImp.h
    DlgProjectInformationImp.h
    DlgProjectUtility.h
    DlgPropertyLink.h
    DlgRevertToBackupConfigImp.h
    DlgCheckableMessageBox.h
    DlgExpressionInput.h
    TaskDlgRelocation.h
    TaskCSysDragger.h
    DlgUndoRedo.h
    InputVector.h
    PropertyPage.h
    DownloadItem.h
    DownloadManager.h
    DocumentRecovery.h
    TaskElementColors.h
    DlgObjectSelection.h
    DlgAddProperty.h
    VectorListEditor.h
    plugininstallerbrowser.h

)

SET(Dialog_SRCS
    ${Dialog_CPP_SRCS}
    ${Dialog_HPP_SRCS}
    AboutApplication.ui
    DlgActivateWindow.ui
    DlgUnitsCalculator.ui
    DlgAuthorization.ui
    DlgDisplayProperties.ui
    DlgInputDialog.ui
    DlgAddProperty.ui
    DlgLocationAngle.ui
    DlgLocationPos.ui
    DlgMacroExecute.ui
    DlgRunExternal.ui
    DlgMacroRecord.ui
    DlgParameter.ui
    DlgParameterFind.ui
    DlgPreferencePackManagement.ui
    DlgProjectInformation.ui
    DlgProjectUtility.ui
    DlgPropertyLink.ui
    DlgRevertToBackupConfig.ui
    DlgCheckableMessageBox.ui
    DlgTreeWidget.ui
    DlgExpressionInput.ui
    DlgCreateNewPreferencePack.ui
    DownloadManager.ui
    DownloadItem.ui
    DocumentRecovery.ui
    InputVector.ui
    TaskElementColors.ui
    DlgObjectSelection.ui
    VectorListEditor.ui
    plugininstallerbrowser.ui

)
SOURCE_GROUP("Dialog" FILES ${Dialog_SRCS})

# The customize dialog sources
SET(Dialog_Customize_CPP_SRCS
    DlgActionsImp.cpp
    DlgCommandsImp.cpp
    DlgCustomizeImp.cpp
    DlgKeyboardImp.cpp
    DlgToolbarsImp.cpp
    DlgWorkbenchesImp.cpp
    QListWidgetCustom.cpp
)
SET(Dialog_Customize_HPP_SRCS
    DlgActionsImp.h
    DlgCommandsImp.h
    DlgCustomizeImp.h
    DlgKeyboardImp.h
    DlgToolbarsImp.h
    DlgWorkbenchesImp.h
    QListWidgetCustom.h
)
SET(Dialog_Customize_SRCS
    ${Dialog_Customize_CPP_SRCS}
    ${Dialog_Customize_HPP_SRCS}
    DlgActions.ui
    DlgChooseIcon.ui
    DlgCommands.ui
    DlgKeyboard.ui
    DlgToolbars.ui
    DlgWorkbenches.ui
)
SOURCE_GROUP("Dialog\\Customize" FILES ${Dialog_Customize_SRCS})

# The settings dialog sources
SET(Dialog_Settings_CPP_SRCS
    DlgEditorImp.cpp
    DlgGeneralImp.cpp
    DlgOnlineHelpImp.cpp
    DlgPreferencesImp.cpp
    DlgReportViewImp.cpp
    DlgSettingsCacheDirectory.cpp
    DlgSettingsSelection.cpp
    DlgSettingsUnitsImp.cpp
    DlgSettingsDocumentImp.cpp
    DlgSettingsImageImp.cpp
    DlgSettingsLazyLoadedImp.cpp
    DlgSettingsMacroImp.cpp
    DlgSettingsPythonConsole.cpp
    DlgPluginImp.cpp

)
SET(Dialog_Settings_HPP_SRCS
    DlgEditorImp.h
    DlgGeneralImp.h
    DlgOnlineHelpImp.h
    DlgPreferencesImp.h
    DlgReportViewImp.h
    DlgSettingsCacheDirectory.h
    DlgSettingsSelection.h
    DlgSettingsUnitsImp.h
    DlgSettingsDocumentImp.h
    DlgSettingsImageImp.h
    DlgSettingsLazyLoadedImp.h
    DlgSettingsMacroImp.h
    DlgSettingsPythonConsole.h
    DlgPluginImp.h

)
SET(Dialog_Settings_SRCS
    ${Dialog_Settings_CPP_SRCS}
    ${Dialog_Settings_HPP_SRCS}
    DlgEditor.ui
    DlgGeneral.ui
    DlgOnlineHelp.ui
    DlgPreferences.ui
    DlgReportView.ui
    DlgSettingsCacheDirectory.ui
    DlgSettingsSelection.ui
    DlgSettingsUnits.ui
    DlgSettingsDocument.ui
    DlgSettingsImage.ui
    DlgSettingsLazyLoaded.ui
    DlgSettingsMacro.ui
    DlgSettingsPythonConsole.ui
    DlgPlugin.ui

)
SOURCE_GROUP("Dialog\\Settings" FILES ${Dialog_Settings_SRCS})

# The dock windows sources
SET(Dock_Windows_CPP_SRCS
    ComboView.cpp
    DockWindow.cpp
    PropertyView.cpp
    ReportView.cpp
    SelectionView.cpp
    ToolBox.cpp
    Tree.cpp
    TreeView.cpp
    ProjectView.cpp
    PlotExplorer.cpp
    PlotPropertyEditor.cpp
    MyPlotTreeWidget.cpp
    ResultView.cpp
)
SET(Dock_Windows_HPP_SRCS
    ComboView.h
    DockWindow.h
    PropertyView.h
    ReportView.h
    SelectionView.h
    ToolBox.h
    Tree.h
    TreeView.h
    ProjectView.h
    PlotExplorer.h
    PlotPropertyEditor.h
    MyPlotTreeWidget.h
    ResultView.h
)
SET(Dock_Windows_SRCS
    ${Dock_Windows_CPP_SRCS}
    ${Dock_Windows_HPP_SRCS}
)
SOURCE_GROUP("Dock Windows" FILES ${Dock_Windows_SRCS})

# The editor sources
SET(Editor_CPP_SRCS
    CallTips.cpp
    EditorView.cpp
    TextDocumentEditorView.cpp
    PythonConsole.cpp
    PythonConsolePy.cpp
    PythonDebugger.cpp
    PythonEditor.cpp
    SyntaxHighlighter.cpp
    TextEdit.cpp
)
SET(Editor_HPP_SRCS
    CallTips.h
    EditorView.h
    TextDocumentEditorView.h
    PythonConsole.h
    PythonConsolePy.h
    PythonDebugger.h
    PythonEditor.h
    SyntaxHighlighter.h
    TextEdit.h
)
SET(Editor_SRCS
    ${Editor_CPP_SRCS}
    ${Editor_HPP_SRCS}
)
SOURCE_GROUP("Editor" FILES ${Editor_SRCS})

# The help system
SET(Help_CPP_SRCS
    Assistant.cpp
    NetworkRetriever.cpp
    OnlineDocumentation.cpp
    WhatsThis.cpp
)
SET(Help_SRCS
    ${Help_CPP_SRCS}
    Assistant.h
    NetworkRetriever.h
    OnlineDocumentation.h
    WhatsThis.h
)
SOURCE_GROUP("Help" FILES ${Help_SRCS})


# The i18n sources
SET(Language_SRCS
    Language/Translator.cpp
    Language/Translator.h
)
SOURCE_GROUP("Language" FILES ${Language_SRCS})

# The property editor
SET(Propertyeditor_SRCS
    propertyeditor/PropertyEditor.cpp
    propertyeditor/PropertyEditor.h
    propertyeditor/PropertyItem.cpp
    propertyeditor/PropertyItem.h
    propertyeditor/PropertyItemDelegate.cpp
    propertyeditor/PropertyItemDelegate.h
    propertyeditor/PropertyModel.cpp
    propertyeditor/PropertyModel.h
)
SOURCE_GROUP("Propertyeditor" FILES ${Propertyeditor_SRCS})

# The task view
SET(Task_View_SRCS
    TaskView/TaskAppearance.cpp
    TaskView/TaskAppearance.h
    TaskView/TaskAppearance.ui
    TaskView/TaskSelectLinkProperty.cpp
    TaskView/TaskSelectLinkProperty.h
    TaskView/TaskSelectLinkProperty.ui
    TaskView/TaskEditControl.cpp
    TaskView/TaskEditControl.h
    TaskView/TaskEditControl.ui
    TaskView/TaskView.cpp
    TaskView/TaskView.h
    TaskView/TaskDialog.cpp
    TaskView/TaskDialog.h
    TaskView/TaskDialogPython.cpp
    TaskView/TaskDialogPython.h
    TaskView/TaskWatcher.cpp
    TaskView/TaskWatcher.h
)
SOURCE_GROUP("Task View" FILES ${Task_View_SRCS})

SET(qsintActionPanel_SRCS
    QSint/actionpanel/actionbox.cpp
    QSint/actionpanel/actionbox.h
    QSint/actionpanel/actionlabel.cpp
    QSint/actionpanel/actionlabel.h
    QSint/actionpanel/actiongroup.cpp
    QSint/actionpanel/actiongroup.h
    QSint/actionpanel/actionpanel.cpp
    QSint/actionpanel/actionpanel.h
    QSint/actionpanel/actionpanelscheme.cpp
    QSint/actionpanel/actionpanelscheme.h
    QSint/actionpanel/androidpanelscheme.cpp
    QSint/actionpanel/androidpanelscheme.h
    QSint/actionpanel/macpanelscheme.cpp
    QSint/actionpanel/macpanelscheme.h
    QSint/actionpanel/taskgroup_p.cpp
    QSint/actionpanel/taskgroup_p.h
    QSint/actionpanel/taskheader_p.cpp
    QSint/actionpanel/taskheader_p.h
    QSint/actionpanel/winvistapanelscheme.cpp
    QSint/actionpanel/winvistapanelscheme.h
    QSint/actionpanel/winxppanelscheme.cpp
    QSint/actionpanel/winxppanelscheme.h
    QSint/actionpanel/labrpsscheme.cpp
    QSint/actionpanel/labrpsscheme.h
)
SOURCE_GROUP("Widget\\QSintActionPanel" FILES ${qsintActionPanel_SRCS})
set(qsint_MOC_HDRS
    QSint/actionpanel/actionbox.h
    QSint/actionpanel/actionlabel.h
    QSint/actionpanel/actiongroup.h
    QSint/actionpanel/actionpanel.h
    QSint/actionpanel/taskheader_p.h
)

if(BUILD_QT5)
    qt5_wrap_cpp(qsint_MOC_SRCS ${qsint_MOC_HDRS})
else()
    qt4_wrap_cpp(qsint_MOC_SRCS ${qsint_MOC_HDRS})
endif()
SOURCE_GROUP("Widget\\QSintActionPanel\\Mocs" FILES ${qsint_MOC_SRCS})

#SET(Resource_RCS
#   Icons/resource.qrc
#   Language/translation.qrc)
#qt4_add_resources(Resource_SRCS ${Resource_RCS})
#SET(Resource_SRCS
#   ${Resource_SRCS}
#   Icons/resource.qrc
#   Language/translation.qrc
#)
#SOURCE_GROUP("Resource" FILES ${Resource_SRCS})

# The 3d view
SET(View3D_CPP_SRCS
    View.cpp
)
SET(View3D_SRCS
    ${View3D_CPP_SRCS}
    View.h
)
SOURCE_GROUP("View3D" FILES ${View3D_SRCS})

# The view provider sources
SET(Viewprovider_CPP_SRCS
    ViewProvider.cpp
    ViewProviderExtension.cpp
    ViewProviderExtensionPyImp.cpp
    ViewProviderGroupExtension.cpp
    # ViewProviderGeoFeatureGroupExtension.cpp
    ViewProviderDocumentObject.cpp
    ViewProviderDocumentObjectGroup.cpp
    ViewProviderDocumentObjectPyImp.cpp
    ViewProviderRPSFeature.cpp
    ViewProviderRPSFeaturePyImp.cpp
    ViewProviderDragger.cpp
    ViewProviderExtern.cpp
    ViewProviderFeature.cpp
    ViewProviderPyImp.cpp
    ViewProviderPythonFeature.cpp
    ViewProviderBuilder.cpp
    # ViewProviderGeoFeatureGroup.cpp
    ViewProviderTextDocument.cpp
    ViewProviderLink.cpp
    LinkViewPyImp.cpp
    ViewProviderLinkPyImp.cpp
)
SET(Viewprovider_SRCS
    ${Viewprovider_CPP_SRCS}
    ViewProvider.h
    ViewProviderExtension.h
    ViewProviderGroupExtension.h
    # ViewProviderGeoFeatureGroupExtension.h
    ViewProviderDocumentObject.h
    ViewProviderDocumentObjectGroup.h
    ViewProviderRPSFeature.h
    ViewProviderDragger.h
    ViewProviderExtern.h
    ViewProviderFeature.h
    ViewProviderPythonFeature.h
    ViewProviderBuilder.h
    # ViewProviderGeoFeatureGroup.h
    ViewProviderTextDocument.h
    ViewProviderLink.h
)
SOURCE_GROUP("View3D\\Viewprovider" FILES ${Viewprovider_SRCS})

# The widget sources
SET(Widget_CPP_SRCS
    FileDialog.cpp
    MainWindow.cpp
    MainWindowPy.cpp
    PrefWidgets.cpp
    InputField.cpp
    ProgressBar.cpp
    ProgressDialog.cpp
    QuantitySpinBox.cpp
    SpinBox.cpp
    Splashscreen.cpp
    PythonWrapper.cpp
    UiLoader.cpp
    WidgetFactory.cpp
    Widgets.cpp
    Window.cpp
    ViewParams.cpp
)
SET(Widget_HPP_SRCS
    FileDialog.h
    MainWindow.h
    MainWindowPy.h
    PrefWidgets.h
    InputField.h
    ProgressBar.h
    ProgressDialog.h
    QuantitySpinBox.h
    QuantitySpinBox_p.h
    SpinBox.h
    Splashscreen.h
    PythonWrapper.h
    UiLoader.h
    WidgetFactory.h
    Widgets.h
    Window.h
    ViewParams.h
)
SET(Widget_SRCS
    ${Widget_CPP_SRCS}
    ${Widget_HPP_SRCS}
)
SOURCE_GROUP("Widget" FILES ${Widget_SRCS})

# The view sources
SET(View_CPP_SRCS
    MDIView.cpp
    MDIViewPy.cpp
    GraphvizView.cpp
    ActiveObjectList.cpp
)
SET(View_HPP_SRCS
    MDIView.h
    MDIViewPy.h
    GraphvizView.h
    ActiveObjectList.h
)
SET(View_SRCS
    ${View_CPP_SRCS}
    ${View_HPP_SRCS}
)
SOURCE_GROUP("View" FILES ${View_SRCS})

# The workbench sources
SET(Workbench_CPP_SRCS
    DockWindowManager.cpp
    MenuManager.cpp
    PythonWorkbenchPyImp.cpp
    ToolBarManager.cpp
    ToolBoxManager.cpp
    Workbench.cpp
    WorkbenchFactory.cpp
    WorkbenchManager.cpp
    WorkbenchPyImp.cpp
)
SET(Workbench_SRCS
    ${Workbench_CPP_SRCS}
    DockWindowManager.h
    MenuManager.h
    ToolBarManager.h
    ToolBoxManager.h
    Workbench.h
    WorkbenchFactory.h
    WorkbenchManager.h
)
SOURCE_GROUP("Workbench" FILES ${Workbench_SRCS})

SET(Selection_SRCS
    SelectionObjectPyImp.cpp
    SelectionObject.h
    SelectionObject.cpp
    Selection.h
    Selection.cpp
    SelectionFilter.h
    SelectionFilter.cpp
    SelectionFilter.y
    SelectionFilter.l
)
SOURCE_GROUP("Selection" FILES ${Selection_SRCS})

# The LabRPSGui sources
SET(LabRPSGui_CPP_SRCS
    Application.cpp
    ApplicationPy.cpp
    AutoSaver.cpp
    BitmapFactory.cpp
    Document.cpp
    DocumentModel.cpp
    DocumentPyImp.cpp
    DocumentObserver.cpp
    DocumentObserverPython.cpp
    ExpressionBinding.cpp
    ExpressionBindingPy.cpp
    GraphicsViewZoom.cpp
    ExpressionCompleter.cpp
    GuiApplication.cpp
    GuiApplicationNativeEventAware.cpp
    GuiConsole.cpp
    Macro.cpp
    MergeDocuments.cpp
    Namespace.h
    resource.cpp
    Control.cpp
    PreferencePackManager.cpp
    Thumbnail.cpp
    Utilities.cpp
    WaitCursor.cpp
    TransactionObject.cpp
    qcustomplot.cpp
    AlphaPlot.cpp
    AlphaPlotAprojHandler.cpp
    RPSPluginsInstallerWizard.cpp
    RPSPropItem.cpp

)
SET(LabRPSGui_SRCS 
    Application.h
    AutoSaver.h
    BitmapFactory.h
    Document.h
    DocumentModel.h
    DocumentObserver.h
    DocumentObserverPython.h
    ExpressionBinding.h
    ExpressionBindingPy.h
    GraphicsViewZoom.h
    ExpressionCompleter.h
    LabRPSGuiInit.py
    GuiApplication.h
    GuiApplicationNativeEventAware.h
    GuiConsole.h
    Macro.h
    MergeDocuments.h
    MetaTypes.h
    PreCompiled.cpp
    PreCompiled.h
    QtAll.h
    Control.h
    PreferencePackManager.h
    Thumbnail.h
    Tools.h
    Utilities.h
    WaitCursor.h
    TransactionObject.h
    ${LabRPSGui_SDK_MOC_HDRS}
    qcustomplot.h
    AlphaPlot.h
    AlphaPlotAprojHandler.h
    RPSGlobals.h
    RPSPluginsInstallerWizard.h
    RPSPropItem.h
)

SET(LabRPSGui_SRCS
    ${LabRPSGui_SDK_SRCS}
    ${LabRPSGui_CPP_SRCS}
    ${LabRPSGui_XML_SRCS}
    ${qsint_MOC_SRCS}
    ${Gui_QRC_SRCS}
    ${Gui_UIC_HDRS}
    ${Command_SRCS}
    ${Dialog_SRCS}
    ${Dialog_Customize_SRCS}
    ${Dialog_Settings_SRCS}
    ${Dock_Windows_SRCS}
    ${Editor_SRCS}
    ${Help_SRCS}
    ${Language_SRCS}
    ${Propertyeditor_SRCS}
    ${Task_View_SRCS}
    ${qsintActionPanel_SRCS}
    ${Resource_SRCS}
    ${View3D_SRCS}
    ${Viewprovider_SRCS}
    ${Widget_SRCS}
    ${View_SRCS}
    ${Workbench_SRCS}
    ${Selection_SRCS}
    ${LabRPSGui_SRCS}
)

if(MSVC)
    SET(LabRPSGui_CPP_SRCS
        Language/Translator.cpp
        propertyeditor/PropertyEditor.cpp
        propertyeditor/PropertyItem.cpp
        propertyeditor/PropertyItemDelegate.cpp
        propertyeditor/PropertyModel.cpp
        TaskView/TaskAppearance.cpp
        TaskView/TaskSelectLinkProperty.cpp
        TaskView/TaskEditControl.cpp
        TaskView/TaskView.cpp
        ${Command_CPP_SRCS}
        ${Dialog_CPP_SRCS}
        ${Dialog_Customize_CPP_SRCS}
        ${Dialog_Settings_CPP_SRCS}
        ${Dock_Windows_CPP_SRCS}
        ${Editor_CPP_SRCS}
        ${Help_CPP_SRCS}
        ${Viewprovider_CPP_SRCS}
        ${Widget_CPP_SRCS}
        ${Workbench_CPP_SRCS}
        ${LabRPSGui_CPP_SRCS}
    )
endif(MSVC)

if (LABRPS_USE_PCH)
    add_definitions(-D_PreComp_)
    ADD_MSVC_PRECOMPILED_HEADER(LabRPSGui PreCompiled.h PreCompiled.cpp LabRPSGui_CPP_SRCS)
endif(LABRPS_USE_PCH)

if (LABRPS_USE_QT_FILEDIALOG)
    set_source_files_properties(FileDialog.cpp PROPERTIES COMPILE_FLAGS -DUSE_QT_FILEDIALOG)
endif()

add_library(LabRPSGui SHARED ${LabRPSGui_SRCS})
target_link_libraries(LabRPSGui ${LabRPSGui_LIBS})
set_target_properties(LabRPSGui PROPERTIES FOLDER "Gui")

SET_BIN_DIR(LabRPSGui LabRPSGui)

if(WIN32)
    INSTALL(TARGETS LabRPSGui
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
else(WIN32)
    INSTALL(TARGETS LabRPSGui
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
    #INSTALL(FILES Icons/labrps.xpm
    #    Icons/labrps-icon-16.png
    #    Icons/labrps-icon-32.png
    #    Icons/labrps-icon-48.png
    #    Icons/labrps-icon-64.png
    #    Icons/labrps.svg
    #    Icons/labrps-doc.png
    #    DESTINATION ${CMAKE_INSTALL_DATADIR}
    #)
    INSTALL(FILES Icons/labrps-icon-16.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/16x16/apps RENAME labrps.png)
    INSTALL(FILES Icons/labrps-icon-32.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/32x32/apps RENAME labrps.png)
    INSTALL(FILES Icons/labrps-icon-48.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/48x48/apps RENAME labrps.png)
    INSTALL(FILES Icons/labrps-icon-64.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/64x64/apps RENAME labrps.png)
    INSTALL(FILES Icons/labrps.svg DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps)
    INSTALL(FILES Icons/labrps.xpm DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pixmaps)
    INSTALL(FILES Icons/labrps-doc.svg DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/mimetypes RENAME application-x-extension-rps.svg)
endif(WIN32)

set(LabRPSGui_Scripts
    RemoteDebugger.ui
    RemoteDebugger.py
)

add_custom_target(LabRPSGui_Resources ALL
    SOURCES ${LabRPSGui_Scripts}
)
set_target_properties(LabRPSGui_Resources PROPERTIES FOLDER "Gui")

rps_copy_sources(LabRPSGui_Resources
    ${CMAKE_BINARY_DIR}/Ext/labrps/gui
    ${LabRPSGui_Scripts}
)

INSTALL(
    FILES
        ${LabRPSGui_Scripts}
    DESTINATION
        Ext/labrps/gui
)
