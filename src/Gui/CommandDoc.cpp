/***************************************************************************
 *   Copyright (c) 2002 Jürgen Riegel <juergen.riegel@web.de>              *
 *                                                                         *
 *   This file is part of the LabRPS CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/


#include "PreCompiled.h"
#ifndef _PreComp_
# include <QApplication>
# include <QClipboard>
# include <QDateTime>
# include <QTextStream>
# include <QTreeWidgetItem> 
#endif

#include <boost/regex.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <App/AutoTransaction.h>
#include <App/Document.h>
#include <App/DocumentObject.h>
#include <App/Expression.h>
#include <App/RPSFeature.h>
#include <Base/Exception.h>
#include <Base/FileInfo.h>
#include <Base/Stream.h>
#include <Base/Tools.h>

#include "Action.h"
#include "Application.h"
#include "BitmapFactory.h"
#include "Command.h"
#include "Control.h"
#include "FileDialog.h"
#include "MainWindow.h"
#include "Selection.h"
#include "DlgObjectSelection.h"
#include "DlgProjectInformationImp.h"
#include "DlgProjectUtility.h"
#include "GraphvizView.h"
#include "MergeDocuments.h"
#include "ViewProvider.h"
#include "WaitCursor.h"
#include "Document.h"
#include "ViewProviderDocumentObject.h"

RPS_LOG_LEVEL_INIT("Command", false)

using namespace Gui;


//===========================================================================
// Std_Open
//===========================================================================

DEF_STD_CMD(StdCmdOpen)

StdCmdOpen::StdCmdOpen()
  : Command("Std_Open")
{
    // setting the
    sGroup        = "File";
    sMenuText     = QT_TR_NOOP("&Open...");
    sToolTipText  = QT_TR_NOOP("Open a document or import files");
    sWhatsThis    = "Std_Open";
    sStatusTip    = QT_TR_NOOP("Open a document or import files");
    sPixmap       = "document-open";
    sAccel        = keySequenceToAccel(QKeySequence::Open);
    eType         = NoTransaction;
}

void StdCmdOpen::activated(int iMsg)
{
    Q_UNUSED(iMsg);

    // fill the list of registered endings
    QString formatList;
    const char* supported = QT_TR_NOOP("Supported formats");
    const char* allFiles = QT_TR_NOOP("All files (*.*)");
    formatList = QObject::tr(supported);
    formatList += QLatin1String(" (");

    std::vector<std::string> filetypes = App::GetApplication().getImportTypes();
    std::vector<std::string>::iterator it;
    // Make sure RPSStd is the very first fileformat
    it = std::find(filetypes.begin(), filetypes.end(), "RPSStd");
    if (it != filetypes.end()) {
        filetypes.erase(it);
        filetypes.insert(filetypes.begin(), "RPSStd");
    }
    for (it=filetypes.begin();it != filetypes.end();++it) {
        formatList += QLatin1String(" *.");
        formatList += QLatin1String(it->c_str());
    }

    formatList += QLatin1String(");;");

    std::map<std::string, std::string> FilterList = App::GetApplication().getImportFilters();
    std::map<std::string, std::string>::iterator jt;
    // Make sure the format name for RPSStd is the very first in the list
    for (jt=FilterList.begin();jt != FilterList.end();++jt) {
        if (jt->first.find("*.RPSStd") != std::string::npos) {
            formatList += QLatin1String(jt->first.c_str());
            formatList += QLatin1String(";;");
            FilterList.erase(jt);
            break;
        }
    }
    for (jt=FilterList.begin();jt != FilterList.end();++jt) {
        formatList += QLatin1String(jt->first.c_str());
        formatList += QLatin1String(";;");
    }
    formatList += QObject::tr(allFiles);

    QString selectedFilter;
    QStringList fileList = FileDialog::getOpenFileNames(getMainWindow(),
        QObject::tr("Open document"), QString(), formatList, &selectedFilter);
    if (fileList.isEmpty())
        return;

    // load the files with the associated modules
    SelectModule::Dict dict = SelectModule::importHandler(fileList, selectedFilter);
    if (dict.isEmpty()) {
        QMessageBox::critical(getMainWindow(),
            qApp->translate("StdCmdOpen", "Cannot open file"),
            qApp->translate("StdCmdOpen", "Loading the file %1 is not supported").arg(fileList.front()));
    }
    else {
        for (SelectModule::Dict::iterator it = dict.begin(); it != dict.end(); ++it) {
            getGuiApplication()->open(it.key().toUtf8(), it.value().toLatin1());

            App::Document *doc = App::GetApplication().getActiveDocument();

            if(doc && doc->testStatus(App::Document::PartialRestore))
                QMessageBox::critical(getMainWindow(), QObject::tr("Error"), QObject::tr("There were errors while loading the file. Some data might have been modified or not recovered at all. Look in the report view for more specific information about the objects involved."));

            if(doc && doc->testStatus(App::Document::RestoreError))
                QMessageBox::critical(getMainWindow(), QObject::tr("Error"), QObject::tr("There were serious errors while loading the file. Some data might have been modified or not recovered at all. Saving the project will most likely result in loss of data."));
        }
    }
}

//===========================================================================
// Std_Import
//===========================================================================

DEF_STD_CMD_A(StdCmdImport)

StdCmdImport::StdCmdImport()
  : Command("Std_Import")
{
    // setting the
    sGroup        = "File";
    sMenuText     = QT_TR_NOOP("&Import...");
    sToolTipText  = QT_TR_NOOP("Import a file in the active document");
    sWhatsThis    = "Std_Import";
    sStatusTip    = QT_TR_NOOP("Import a file in the active document");
    sPixmap       = "Std_Import";
    sAccel        = "Ctrl+I";
}

void StdCmdImport::activated(int iMsg)
{
    Q_UNUSED(iMsg);

    // fill the list of registered endings
    QString formatList;
    const char* supported = QT_TR_NOOP("Supported formats");
    const char* allFiles = QT_TR_NOOP("All files (*.*)");
    formatList = QObject::tr(supported);
    formatList += QLatin1String(" (");

    std::vector<std::string> filetypes = App::GetApplication().getImportTypes();
    std::vector<std::string>::const_iterator it;
    for (it=filetypes.begin();it != filetypes.end();++it) {
        if (*it != "RPSStd") {
            // ignore the project file format
            formatList += QLatin1String(" *.");
            formatList += QLatin1String(it->c_str());
        }
    }

    formatList += QLatin1String(");;");

    std::map<std::string, std::string> FilterList = App::GetApplication().getImportFilters();
    std::map<std::string, std::string>::const_iterator jt;
    for (jt=FilterList.begin();jt != FilterList.end();++jt) {
        // ignore the project file format
        if (jt->first.find("(*.RPSStd)") == std::string::npos) {
            formatList += QLatin1String(jt->first.c_str());
            formatList += QLatin1String(";;");
        }
    }
    formatList += QObject::tr(allFiles);

    Base::Reference<ParameterGrp> hPath = App::GetApplication().GetUserParameter().GetGroup("BaseApp")
                               ->GetGroup("Preferences")->GetGroup("General");
    QString selectedFilter = QString::fromStdString(hPath->GetASCII("FileImportFilter"));
    QStringList fileList = FileDialog::getOpenFileNames(getMainWindow(),
        QObject::tr("Import file"), QString(), formatList, &selectedFilter);
    if (!fileList.isEmpty()) {
        hPath->SetASCII("FileImportFilter", selectedFilter.toLatin1().constData());
        SelectModule::Dict dict = SelectModule::importHandler(fileList, selectedFilter);

        bool emptyDoc = (getActiveGuiDocument()->getDocument()->countObjects() == 0);
        // load the files with the associated modules
        for (SelectModule::Dict::iterator it = dict.begin(); it != dict.end(); ++it) {
            getGuiApplication()->importFrom(it.key().toUtf8(),
                getActiveGuiDocument()->getDocument()->getName(),
                it.value().toLatin1());
        }
    }
}

bool StdCmdImport::isActive()
{
    return (getActiveGuiDocument() ? true : false);
}


//===========================================================================
// Std_Export
//===========================================================================

DEF_STD_CMD_A(StdCmdExport)

StdCmdExport::StdCmdExport()
  : Command("Std_Export")
{
    // setting the
    sGroup        = "File";
    sMenuText     = QT_TR_NOOP("&Export...");
    sToolTipText  = QT_TR_NOOP("Export an object in the active document");
    sWhatsThis    = "Std_Export";
    sStatusTip    = QT_TR_NOOP("Export an object in the active document");
    //sPixmap       = "Open";
    sAccel        = "Ctrl+E";
    sPixmap       = "Std_Export";
    eType         = 0;
}

/**
Create a default filename from a user-specified format string
 
Format options are:
%F - the basename of the .RPSStd file (or the label, if it is not saved yet)
%Lx - the label of the selected object(s), separated by character 'x'
%Px - the label of the selected object(s) and their first parent, separated by character 'x'
%U - the date and time, in UTC, ISO 8601
%D - the date and time, in local timezone, ISO 8601
Any other characters are treated literally, though if the filename is illegal
it will be changed on saving.

The format string is stored in two user preferences (not currently exposed in the GUI):
* BaseApp/Preferences/General/ExportDefaultFilenameSingle
* BaseApp/Preferences/General/ExportDefaultFilenameMultiple
*/
QString createDefaultExportBasename()
{
    QString defaultFilename;

    auto selection = Gui::Selection().getObjectsOfType(App::DocumentObject::getClassTypeId());
    QString exportFormatString;
    if (selection.size() == 1) {
        exportFormatString = QString::fromStdString (App::GetApplication().GetParameterGroupByPath("User parameter:BaseApp/Preferences/General")->
            GetASCII("ExportDefaultFilenameSingle", "%F-%P-"));
    }
    else {
        exportFormatString = QString::fromStdString (App::GetApplication().GetParameterGroupByPath("User parameter:BaseApp/Preferences/General")->
            GetASCII("ExportDefaultFilenameMultiple", "%F"));
    }

    // For code simplicity, pull all values we might need

    // %F - the basename of the.RPSStd file(or the label, if it is not saved yet)
    QString docFilename = QString::fromUtf8(App::GetApplication().getActiveDocument()->getFileName());
    QFileInfo fi(docFilename);
    QString rpsstdBasename = fi.completeBaseName();
    if (rpsstdBasename.isEmpty()) 
        rpsstdBasename = QString::fromStdString(App::GetApplication().getActiveDocument()->Label.getStrValue());

    // %L - the label of the selected object(s)
    QStringList objectLabels;
    for (const auto& object : selection)
        objectLabels.push_back(QString::fromStdString(object->Label.getStrValue()));

    // %P - the label of the selected objects and their first parent
    QStringList parentLabels;
    for (const auto& object : selection) {
        auto parents = object->getParents();
        QString firstParent;
        if (!parents.empty())
            firstParent = QString::fromStdString(parents.front().first->Label.getStrValue());
        parentLabels.append(firstParent + QString::fromStdString(object->Label.getStrValue()));
    }

    // %U - the date and time, in UTC, ISO 8601
    QDateTime utc = QDateTime(QDateTime::currentDateTimeUtc());
    QString utcISO8601 = utc.toString(Qt::ISODate);

    // %D - the date and time, in local timezone, ISO 8601
    QDateTime local = utc.toLocalTime();
    QString localISO8601 = local.toString(Qt::ISODate);

    // Parse the format string one character at a time:
    for (int i = 0; i < exportFormatString.size(); ++i) {
        auto c = exportFormatString.at(i);
        if (c != QLatin1Char('%')) {
            // Anything that's not a format start character is just a literal
            defaultFilename.append(c);
        }
        else {
            // The format start character now requires us to look at at least the next single
            // character (if there isn't another character, the % just gets eaten)
            if (i < exportFormatString.size() - 1) {
                ++i;
                auto formatChar = exportFormatString.at(i);
                QChar separatorChar = QLatin1Char('-');
                // If this format type requires an additional char, read that now (or default to
                // '-' if the format string ends) 
                if (formatChar == QLatin1Char('L') ||
                    formatChar == QLatin1Char('P')) {
                    if (i < exportFormatString.size() - 1) {
                        ++i;
                        separatorChar = exportFormatString.at(i);
                    }
                }

                // Handle our format characters:
                if (formatChar == QLatin1Char('F')) {
                    defaultFilename.append(rpsstdBasename);
                }
                else if (formatChar == QLatin1Char('L')) {
                    defaultFilename.append(objectLabels.join(separatorChar));
                }
                else if (formatChar == QLatin1Char('P')) {
                    defaultFilename.append(parentLabels.join(separatorChar));
                }
                else if (formatChar == QLatin1Char('U')) {
                    defaultFilename.append(utcISO8601);
                }
                else if (formatChar == QLatin1Char('D')) {
                    defaultFilename.append(localISO8601);
                }
                else {
                    RPS_WARN("When parsing default export filename format string, %" 
                        << QString(formatChar).toStdString() 
                        << " is not a known format string.");
                }
            }
        }
    }

    // Finally, clean the string so it's valid for all operating systems:
    QString invalidCharacters = QLatin1String("/\\?%*:|\"<>");
    for (const auto &c : invalidCharacters)
        defaultFilename.replace(c,QLatin1String("_"));

    return defaultFilename;
}

void StdCmdExport::activated(int iMsg)
{
    Q_UNUSED(iMsg);

    static QString lastExportFullPath = QString();
    static bool lastExportUsedGeneratedFilename = true;
    static QString lastExportFilterUsed = QString();

    auto selection = Gui::Selection().getObjectsOfType(App::DocumentObject::getClassTypeId());
    if (selection.size() == 0) {
        QMessageBox::warning(Gui::getMainWindow(),
            QString::fromUtf8(QT_TR_NOOP("No selection")),
            QString::fromUtf8(QT_TR_NOOP("Select the objects to export before choosing Export.")));
        return;
    }

    // fill the list of registered suffixes
    QStringList filterList;
    std::map<std::string, std::string> filterMap = App::GetApplication().getExportFilters();
    for (const auto &filter : filterMap) {
        // ignore the project file format
        if (filter.first.find("(*.RPSStd)") == std::string::npos)
            filterList << QString::fromStdString(filter.first);
    }
    QString formatList = filterList.join(QLatin1String(";;"));
    Base::Reference<ParameterGrp> hPath = 
        App::GetApplication().GetUserParameter().GetGroup("BaseApp")->GetGroup("Preferences")->GetGroup("General");
    QString selectedFilter = QString::fromStdString(hPath->GetASCII("FileExportFilter"));
    if (!lastExportFilterUsed.isEmpty())
        selectedFilter = lastExportFilterUsed;

    // Create a default filename for the export
    // * If this is the first export this session default, generate a new default.
    // * If this is a repeated export during the same session:
    //     * If the user accepted the default filename last time, regenerate a new
    //       default, potentially updating the object label.
    //     * If not, default to their previously-set export filename.
    QString defaultFilename = lastExportFullPath;

    bool filenameWasGenerated = false;
    // We want to generate a new default name in two cases:
    if (defaultFilename.isEmpty() || lastExportUsedGeneratedFilename) {
        // First, get the name and path of the current .RPSStd file, if there is one:
        QString docFilename = QString::fromUtf8(
            App::GetApplication().getActiveDocument()->getFileName());

        // Find the default location for our exported file. Three possibilities:
        QString defaultExportPath;
        if (!lastExportFullPath.isEmpty()) {
            QFileInfo fi(lastExportFullPath);
            defaultExportPath = fi.path();
        }
        else if (!docFilename.isEmpty()) {
            QFileInfo fi(docFilename);
            defaultExportPath = fi.path();
        }
        else {
            defaultExportPath = Gui::FileDialog::getWorkingDirectory();
        }

        if (lastExportUsedGeneratedFilename /*<- static, true on first call*/ ) {
            defaultFilename = defaultExportPath + QLatin1Char('/') + createDefaultExportBasename();

            // Append the last extension used, if there is one.
            if (!lastExportFullPath.isEmpty()) {
                QFileInfo lastExportFile(lastExportFullPath);
                if (!lastExportFile.suffix().isEmpty())
                    defaultFilename += QLatin1String(".") + lastExportFile.suffix();
            }
            filenameWasGenerated = true;
        }
    }

    // Launch the file selection modal dialog
    QString fileName = FileDialog::getSaveFileName(getMainWindow(),
        QObject::tr("Export file"), defaultFilename, formatList, &selectedFilter);
    if (!fileName.isEmpty()) {
        hPath->SetASCII("FileExportFilter", selectedFilter.toLatin1().constData());
        lastExportFilterUsed = selectedFilter; // So we can select the same one next time
        SelectModule::Dict dict = SelectModule::exportHandler(fileName, selectedFilter);
        // export the files with the associated modules
        for (SelectModule::Dict::iterator it = dict.begin(); it != dict.end(); ++it) {
            getGuiApplication()->exportTo(it.key().toUtf8(),
                getActiveGuiDocument()->getDocument()->getName(),
                it.value().toLatin1());
        }

        // Keep a record of if the user used our suggested generated filename. If they
        // did, next time we can recreate it, which will update the object label if
        // there is one.
        QFileInfo defaultExportFI(defaultFilename);
        QFileInfo thisExportFI(fileName);
        if (filenameWasGenerated && 
            thisExportFI.completeBaseName() == defaultExportFI.completeBaseName())
            lastExportUsedGeneratedFilename = true;
        else
            lastExportUsedGeneratedFilename = false;
        lastExportFullPath = fileName;
    }
}

bool StdCmdExport::isActive()
{
    return (getActiveGuiDocument() ? true : false);
}

//===========================================================================
// Std_MergeProjects
//===========================================================================

DEF_STD_CMD_A(StdCmdMergeProjects)

StdCmdMergeProjects::StdCmdMergeProjects()
  : Command("Std_MergeProjects")
{
    sAppModule    = "File";
    sGroup        = "File";
    sMenuText     = QT_TR_NOOP("Merge project...");
    sToolTipText  = QT_TR_NOOP("Merge project");
    sWhatsThis    = "Std_MergeProjects";
    sStatusTip    = QT_TR_NOOP("Merge project");
    sPixmap       = "Std_MergeProjects";
}

void StdCmdMergeProjects::activated(int iMsg)
{
    Q_UNUSED(iMsg);

    QString exe = qApp->applicationName();
    QString project = FileDialog::getOpenFileName(Gui::getMainWindow(),
        QString::fromUtf8(QT_TR_NOOP("Merge project")), FileDialog::getWorkingDirectory(),
        QString::fromUtf8(QT_TR_NOOP("%1 document (*.RPSStd)")).arg(exe));
    if (!project.isEmpty()) {
        FileDialog::setWorkingDirectory(project);
        App::Document* doc = App::GetApplication().getActiveDocument();
        QFileInfo info(QString::fromUtf8(doc->FileName.getValue()));
        QFileInfo proj(project);
        if (proj == info) {
            QMessageBox::critical(Gui::getMainWindow(),
                QString::fromUtf8(QT_TR_NOOP("Merge project")),
                QString::fromUtf8(QT_TR_NOOP("Cannot merge project with itself.")));
            return;
        }

        doc->openTransaction("Merge project");
        Base::FileInfo fi((const char*)project.toUtf8());
        Base::ifstream str(fi, std::ios::in | std::ios::binary);
        MergeDocuments md(doc);
        md.importObjects(str);
        str.close();
        doc->commitTransaction();
    }
}

bool StdCmdMergeProjects::isActive()
{
    return this->hasActiveDocument();
}

//===========================================================================
// Std_DependencyGraph
//===========================================================================

DEF_STD_CMD_A(StdCmdDependencyGraph)

StdCmdDependencyGraph::StdCmdDependencyGraph()
  : Command("Std_ExportDependencyGraph")
{
    // setting the
    sGroup        = "Tools";
    sMenuText     = QT_TR_NOOP("Dependency graph...");
    sToolTipText  = QT_TR_NOOP("Creates a new view window and show the dependency graph of the objects in the active document");
    sStatusTip    = QT_TR_NOOP("Creates a new view window and show the dependency graph of the objects in the active document");
    sWhatsThis    = "Std_ExportDependencyGraph";
    eType         = 0;
    sPixmap       = "Std_DependencyGraph";
}

void StdCmdDependencyGraph::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    App::Document* doc = App::GetApplication().getActiveDocument();
    Gui::GraphvizView* view = new Gui::GraphvizView(*doc);
    view->setWindowTitle(qApp->translate("Std_ExportDependencyGraph","Dependency graph"));
    getMainWindow()->addWindow(view);
}

bool StdCmdDependencyGraph::isActive()
{
    return (getActiveGuiDocument() ? true : false);
}

//===========================================================================
// Std_ExportDependencyGraph
//===========================================================================

DEF_STD_CMD_A(StdCmdExportDependencyGraph)

StdCmdExportDependencyGraph::StdCmdExportDependencyGraph()
  : Command("Std_ExportDependencyGraph")
{
    sGroup        = "Tools";
    sMenuText     = QT_TR_NOOP("Export dependency graph...");
    sToolTipText  = QT_TR_NOOP("Export the dependency graph to a file");
    sStatusTip    = QT_TR_NOOP("Export the dependency graph to a file");
    sWhatsThis    = "Std_ExportDependencyGraph";
    eType         = 0;
    sPixmap       = "Std_DependencyGraph";
}

void StdCmdExportDependencyGraph::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    App::Document* doc = App::GetApplication().getActiveDocument();
    QString format = QString::fromLatin1("%1 (*.gv)").arg(Gui::GraphvizView::tr("Graphviz format"));
    QString fn = Gui::FileDialog::getSaveFileName(Gui::getMainWindow(), Gui::GraphvizView::tr("Export graph"), QString(), format);
    if (!fn.isEmpty()) {
        QFile file(fn);
        if (file.open(QFile::WriteOnly)) {
            std::stringstream str;
            doc->exportGraphviz(str);
            QByteArray buffer = QByteArray::fromStdString(str.str());
            file.write(buffer);
            file.close();
        }
    }
}

bool StdCmdExportDependencyGraph::isActive()
{
    return (getActiveGuiDocument() ? true : false);
}


//===========================================================================
// Std_New
//===========================================================================

DEF_STD_CMD(StdCmdNew)

StdCmdNew::StdCmdNew()
  :Command("Std_New")
{
    sGroup        = "File";
    sMenuText     = QT_TR_NOOP("&New");
    sToolTipText  = QT_TR_NOOP("Create a new empty document");
    sWhatsThis    = "Std_New";
    sStatusTip    = QT_TR_NOOP("Create a new empty document");
    sPixmap       = "document-new";
    sAccel        = keySequenceToAccel(QKeySequence::New);
}

void StdCmdNew::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    QString cmd;
    cmd = QString::fromLatin1("App.newDocument(\"%1\")")
        .arg(qApp->translate("StdCmdNew","Unnamed"));
    runCommand(Command::Doc,cmd.toUtf8());
}

//===========================================================================
// Std_Save
//===========================================================================
DEF_STD_CMD_A(StdCmdSave)

StdCmdSave::StdCmdSave()
  :Command("Std_Save")
{
  sGroup        = "File";
  sMenuText     = QT_TR_NOOP("&Save");
  sToolTipText  = QT_TR_NOOP("Save the active document");
  sWhatsThis    = "Std_Save";
  sStatusTip    = QT_TR_NOOP("Save the active document");
  sPixmap       = "document-save";
  sAccel        = keySequenceToAccel(QKeySequence::Save);
  eType         = 0;
}

void StdCmdSave::activated(int iMsg)
{
    Q_UNUSED(iMsg);
#if 0
  Gui::Document* pActiveDoc = getActiveGuiDocument();
  if ( pActiveDoc )
    pActiveDoc->save();
  else
#endif
    doCommand(Command::Gui,"Gui.SendMsgToActiveView(\"Save\")");
}

bool StdCmdSave::isActive()
{
#if 0
  if( getActiveGuiDocument() )
    return true;
  else
#endif
    return getGuiApplication()->sendHasMsgToActiveView("Save");
}

//===========================================================================
// Std_SaveAs
//===========================================================================
DEF_STD_CMD_A(StdCmdSaveAs)

StdCmdSaveAs::StdCmdSaveAs()
  :Command("Std_SaveAs")
{
  sGroup        = "File";
  sMenuText     = QT_TR_NOOP("Save &As...");
  sToolTipText  = QT_TR_NOOP("Save the active document under a new file name");
  sWhatsThis    = "Std_SaveAs";
  sStatusTip    = QT_TR_NOOP("Save the active document under a new file name");
  sPixmap       = "document-save-as";
  sAccel        = keySequenceToAccel(QKeySequence::SaveAs);
  eType         = 0;
}

void StdCmdSaveAs::activated(int iMsg)
{
    Q_UNUSED(iMsg);
#if 0
  Gui::Document* pActiveDoc = getActiveGuiDocument();
  if ( pActiveDoc )
    pActiveDoc->saveAs();
  else
#endif
    doCommand(Command::Gui,"Gui.SendMsgToActiveView(\"SaveAs\")");
}

bool StdCmdSaveAs::isActive()
{
#if 0
  if( getActiveGuiDocument() )
    return true;
  else
#endif
    return getGuiApplication()->sendHasMsgToActiveView("SaveAs");
}

//===========================================================================
// Std_SaveCopy
//===========================================================================
DEF_STD_CMD_A(StdCmdSaveCopy)

StdCmdSaveCopy::StdCmdSaveCopy()
  :Command("Std_SaveCopy")
{
  sGroup        = "File";
  sMenuText     = QT_TR_NOOP("Save a &Copy...");
  sToolTipText  = QT_TR_NOOP("Save a copy of the active document under a new file name");
  sWhatsThis    = "Std_SaveCopy";
  sStatusTip    = QT_TR_NOOP("Save a copy of the active document under a new file name");
  sPixmap       = "Std_SaveCopy";
}

void StdCmdSaveCopy::activated(int iMsg)
{
    Q_UNUSED(iMsg);
#if 0
  Gui::Document* pActiveDoc = getActiveGuiDocument();
  if ( pActiveDoc )
    pActiveDoc->saveCopy();
  else
#endif
    doCommand(Command::Gui,"Gui.SendMsgToActiveView(\"SaveCopy\")");
}

bool StdCmdSaveCopy::isActive()
{
  return ( getActiveGuiDocument() ? true : false );
}

//===========================================================================
// Std_SaveAll
//===========================================================================
DEF_STD_CMD_A(StdCmdSaveAll)

StdCmdSaveAll::StdCmdSaveAll()
  :Command("Std_SaveAll")
{
  sGroup        = "File";
  sMenuText     = QT_TR_NOOP("Save All");
  sToolTipText  = QT_TR_NOOP("Save all opened document");
  sWhatsThis    = "Std_SaveAll";
  sStatusTip    = QT_TR_NOOP("Save all opened document");
  sPixmap       = "Std_SaveAll";
}

void StdCmdSaveAll::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    Gui::Document::saveAll();
}

bool StdCmdSaveAll::isActive()
{
  return ( getActiveGuiDocument() ? true : false );
}


//===========================================================================
// Std_Revert
//===========================================================================
DEF_STD_CMD_A(StdCmdRevert)

StdCmdRevert::StdCmdRevert()
  :Command("Std_Revert")
{
    sGroup        = "File";
    sMenuText     = QT_TR_NOOP("Revert");
    sToolTipText  = QT_TR_NOOP("Reverts to the saved version of this file");
    sWhatsThis    = "Std_Revert";
    sStatusTip    = QT_TR_NOOP("Reverts to the saved version of this file");
    sPixmap       = "Std_Revert";
    eType         = NoTransaction;
}

void StdCmdRevert::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    QMessageBox msgBox(Gui::getMainWindow());
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(qApp->translate("Std_Revert","Revert document"));
    msgBox.setText(qApp->translate("Std_Revert","This will discard all the changes since last file save."));
    msgBox.setInformativeText(qApp->translate("Std_Revert","Do you want to continue?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Yes)
        doCommand(Command::App,"App.ActiveDocument.restore()");
}

bool StdCmdRevert::isActive()
{
  return ( getActiveGuiDocument() ? true : false );
}

//===========================================================================
// Std_ProjectInfo
//===========================================================================

DEF_STD_CMD_A(StdCmdProjectInfo)

StdCmdProjectInfo::StdCmdProjectInfo()
  :Command("Std_ProjectInfo")
{
  // setting the
  sGroup        = "File";
  sMenuText     = QT_TR_NOOP("Project i&nformation...");
  sToolTipText  = QT_TR_NOOP("Show details of the currently active project");
  sWhatsThis    = "Std_ProjectInfo";
  sStatusTip    = QT_TR_NOOP("Show details of the currently active project");
  sPixmap       = "document-properties";
}

void StdCmdProjectInfo::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    Gui::Dialog::DlgProjectInformationImp dlg(getActiveGuiDocument()->getDocument(), getMainWindow());
    dlg.exec();
}

bool StdCmdProjectInfo::isActive()
{
  return ( getActiveGuiDocument() ? true : false );
}

//===========================================================================
// Std_ProjectUtil
//===========================================================================

DEF_STD_CMD_A(StdCmdProjectUtil)

StdCmdProjectUtil::StdCmdProjectUtil()
  :Command("Std_ProjectUtil")
{
    // setting the
    sGroup        = "Tools";
    sWhatsThis    = "Std_ProjectUtil";
    sMenuText     = QT_TR_NOOP("Project utility...");
    sToolTipText  = QT_TR_NOOP("Utility to extract or create project files");
    sStatusTip    = QT_TR_NOOP("Utility to extract or create project files");
    sPixmap       = "Std_ProjectUtil";
}

void StdCmdProjectUtil::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    Gui::Dialog::DlgProjectUtility dlg(getMainWindow());
    dlg.exec();
}

bool StdCmdProjectUtil::isActive()
{
    return true;
}

//===========================================================================
// Std_Print
//===========================================================================
DEF_STD_CMD_A(StdCmdPrint)

StdCmdPrint::StdCmdPrint()
  :Command("Std_Print")
{
    sGroup        = "File";
    sMenuText     = QT_TR_NOOP("&Print...");
    sToolTipText  = QT_TR_NOOP("Print the document");
    sWhatsThis    = "Std_Print";
    sStatusTip    = QT_TR_NOOP("Print the document");
    sPixmap       = "document-print";
    sAccel        = keySequenceToAccel(QKeySequence::Print);
    eType         = 0;
}

void StdCmdPrint::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    if (getMainWindow()->activeWindow()) {
        getMainWindow()->showMessage(QObject::tr("Printing..."));
        getMainWindow()->activeWindow()->print();
    }
}

bool StdCmdPrint::isActive()
{
    return getGuiApplication()->sendHasMsgToActiveView("Print");
}

//===========================================================================
// Std_PrintPreview
//===========================================================================
DEF_STD_CMD_A(StdCmdPrintPreview)

StdCmdPrintPreview::StdCmdPrintPreview()
  :Command("Std_PrintPreview")
{
    sGroup        = "File";
    sMenuText     = QT_TR_NOOP("&Print preview...");
    sToolTipText  = QT_TR_NOOP("Print the document");
    sWhatsThis    = "Std_PrintPreview";
    sStatusTip    = QT_TR_NOOP("Print preview");
    sPixmap       = "document-print-preview";
    eType         = 0;
}

void StdCmdPrintPreview::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    if (getMainWindow()->activeWindow()) {
        getMainWindow()->activeWindow()->printPreview();
    }
}

bool StdCmdPrintPreview::isActive()
{
    return getGuiApplication()->sendHasMsgToActiveView("PrintPreview");
}

//===========================================================================
// Std_PrintPdf
//===========================================================================
DEF_STD_CMD_A(StdCmdPrintPdf)

StdCmdPrintPdf::StdCmdPrintPdf()
  :Command("Std_PrintPdf")
{
    sGroup        = "File";
    sMenuText     = QT_TR_NOOP("&Export PDF...");
    sToolTipText  = QT_TR_NOOP("Export the document as PDF");
    sWhatsThis    = "Std_PrintPdf";
    sStatusTip    = QT_TR_NOOP("Export the document as PDF");
    sPixmap       = "Std_PrintPdf";
    eType         = 0;
}

void StdCmdPrintPdf::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    if (getMainWindow()->activeWindow()) {
        getMainWindow()->showMessage(QObject::tr("Exporting PDF..."));
        getMainWindow()->activeWindow()->printPdf();
    }
}

bool StdCmdPrintPdf::isActive()
{
    return getGuiApplication()->sendHasMsgToActiveView("PrintPdf");
}

//===========================================================================
// Std_Quit
//===========================================================================

DEF_STD_CMD(StdCmdQuit)

StdCmdQuit::StdCmdQuit()
  :Command("Std_Quit")
{
  sGroup        = "File";
  sMenuText     = QT_TR_NOOP("E&xit");
  sToolTipText  = QT_TR_NOOP("Quits the application");
  sWhatsThis    = "Std_Quit";
  sStatusTip    = QT_TR_NOOP("Quits the application");
  sPixmap       = "application-exit";
  sAccel        = keySequenceToAccel(QKeySequence::Quit);
  eType         = NoTransaction;
}

void StdCmdQuit::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    // close the main window and exit the event loop
    getMainWindow()->close();
}

//===========================================================================
// Std_Undo
//===========================================================================

DEF_STD_CMD_AC(StdCmdUndo)

StdCmdUndo::StdCmdUndo()
  :Command("Std_Undo")
{
  sGroup        = "Edit";
  sMenuText     = QT_TR_NOOP("&Undo");
  sToolTipText  = QT_TR_NOOP("Undo exactly one action");
  sWhatsThis    = "Std_Undo";
  sStatusTip    = QT_TR_NOOP("Undo exactly one action");
  sPixmap       = "edit-undo";
  sAccel        = keySequenceToAccel(QKeySequence::Undo);
  eType         = ForEdit|NoTransaction;
}

void StdCmdUndo::activated(int iMsg)
{
    Q_UNUSED(iMsg);
//  Application::Instance->slotUndo();
    getGuiApplication()->sendMsgToActiveView("Undo");
}

bool StdCmdUndo::isActive()
{
  return getGuiApplication()->sendHasMsgToActiveView("Undo");
}

Action * StdCmdUndo::createAction()
{
    Action *pcAction;

    pcAction = new UndoAction(this,getMainWindow());
    pcAction->setShortcut(QString::fromLatin1(getAccel()));
    applyCommandData(this->className(), pcAction);
    if (getPixmap())
        pcAction->setIcon(Gui::BitmapFactory().iconFromTheme(getPixmap()));

    return pcAction;
}

//===========================================================================
// Std_Redo
//===========================================================================

DEF_STD_CMD_AC(StdCmdRedo)

StdCmdRedo::StdCmdRedo()
  :Command("Std_Redo")
{
  sGroup        = "Edit";
  sMenuText     = QT_TR_NOOP("&Redo");
  sToolTipText  = QT_TR_NOOP("Redoes a previously undone action");
  sWhatsThis    = "Std_Redo";
  sStatusTip    = QT_TR_NOOP("Redoes a previously undone action");
  sPixmap       = "edit-redo";
  sAccel        = keySequenceToAccel(QKeySequence::Redo);
  eType         = ForEdit|NoTransaction;
}

void StdCmdRedo::activated(int iMsg)
{
    Q_UNUSED(iMsg);
//  Application::Instance->slotRedo();
    getGuiApplication()->sendMsgToActiveView("Redo");
}

bool StdCmdRedo::isActive()
{
  return getGuiApplication()->sendHasMsgToActiveView("Redo");
}

Action * StdCmdRedo::createAction()
{
    Action *pcAction;

    pcAction = new RedoAction(this,getMainWindow());
    pcAction->setShortcut(QString::fromLatin1(getAccel()));
    applyCommandData(this->className(), pcAction);
    if (getPixmap())
        pcAction->setIcon(Gui::BitmapFactory().iconFromTheme(getPixmap()));

    return pcAction;
}

//===========================================================================
// Std_Cut
//===========================================================================
DEF_STD_CMD_A(StdCmdCut)

StdCmdCut::StdCmdCut()
  : Command("Std_Cut")
{
    sGroup        = "Edit";
    sMenuText     = QT_TR_NOOP("&Cut");
    sToolTipText  = QT_TR_NOOP("Cut out");
    sWhatsThis    = "Std_Cut";
    sStatusTip    = QT_TR_NOOP("Cut out");
    sPixmap       = "edit-cut";
    sAccel        = keySequenceToAccel(QKeySequence::Cut);
}

void StdCmdCut::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    getGuiApplication()->sendMsgToActiveView("Cut");
}

bool StdCmdCut::isActive()
{
    return getGuiApplication()->sendHasMsgToActiveView("Cut");
}

//===========================================================================
// Std_Copy
//===========================================================================
DEF_STD_CMD_A(StdCmdCopy)

StdCmdCopy::StdCmdCopy()
  : Command("Std_Copy")
{
    sGroup        = "Edit";
    sMenuText     = QT_TR_NOOP("C&opy");
    sToolTipText  = QT_TR_NOOP("Copy operation");
    sWhatsThis    = "Std_Copy";
    sStatusTip    = QT_TR_NOOP("Copy operation");
    sPixmap       = "edit-copy";
    sAccel        = keySequenceToAccel(QKeySequence::Copy);
}

void StdCmdCopy::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    bool done = getGuiApplication()->sendMsgToFocusView("Copy");
    if (!done) {
        QMimeData * mimeData = getMainWindow()->createMimeDataFromSelection();
        QClipboard* cb = QApplication::clipboard();
        cb->setMimeData(mimeData);
    }
}

bool StdCmdCopy::isActive()
{
    if (getGuiApplication()->sendHasMsgToFocusView("Copy"))
        return true;
    return Selection().hasSelection();
}

//===========================================================================
// Std_Paste
//===========================================================================
DEF_STD_CMD_A(StdCmdPaste)

StdCmdPaste::StdCmdPaste()
  : Command("Std_Paste")
{
    sGroup        = "Edit";
    sMenuText     = QT_TR_NOOP("&Paste");
    sToolTipText  = QT_TR_NOOP("Paste operation");
    sWhatsThis    = "Std_Paste";
    sStatusTip    = QT_TR_NOOP("Paste operation");
    sPixmap       = "edit-paste";
    sAccel        = keySequenceToAccel(QKeySequence::Paste);
}

void StdCmdPaste::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    bool done = getGuiApplication()->sendMsgToFocusView("Paste");
    if (!done) {
        QClipboard* cb = QApplication::clipboard();
        const QMimeData* mimeData = cb->mimeData();
        if (mimeData) {
            WaitCursor wc;
            getMainWindow()->insertFromMimeData(mimeData);
        }
    }
}

bool StdCmdPaste::isActive()
{
    if (getGuiApplication()->sendHasMsgToFocusView("Paste"))
        return true;
    QClipboard* cb = QApplication::clipboard();
    const QMimeData* mime = cb->mimeData();
    if (!mime)
        return false;
    return getMainWindow()->canInsertFromMimeData(mime);
}

DEF_STD_CMD_A(StdCmdDuplicateSelection)

StdCmdDuplicateSelection::StdCmdDuplicateSelection()
  :Command("Std_DuplicateSelection")
{
    sAppModule    = "Edit";
    sGroup        = "Edit";
    sMenuText     = QT_TR_NOOP("Duplicate selection");
    sToolTipText  = QT_TR_NOOP("Put duplicates of the selected objects to the active document");
    sWhatsThis    = "Std_DuplicateSelection";
    sStatusTip    = QT_TR_NOOP("Put duplicates of the selected objects to the active document");
    sPixmap       = "Std_DuplicateSelection";
}

void StdCmdDuplicateSelection::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    std::vector<App::DocumentObject*> sel;
    std::set<App::DocumentObject*> objSet;
    for(auto &s : Selection().getCompleteSelection()) {
        if(s.pObject && s.pObject->getNameInDocument() && objSet.insert(s.pObject).second)
            sel.push_back(s.pObject);
    }
    if(sel.empty())
        return;

    bool hasXLink = false;
    Base::FileInfo fi(App::Application::getTempFileName());
    {
        auto all = App::Document::getDependencyList(sel);
        if (all.size() > sel.size()) {
            DlgObjectSelection dlg(sel,getMainWindow());
            if(dlg.exec()!=QDialog::Accepted)
                return;
            sel = dlg.getSelections();
            if(sel.empty())
                return;
        }
        std::vector<App::Document*> unsaved;
        hasXLink = App::PropertyXLink::hasXLink(sel,&unsaved);
        if(unsaved.size()) {
            QMessageBox::critical(getMainWindow(), QObject::tr("Unsaved document"),
                QObject::tr("The exported object contains external link. Please save the document"
                   "at least once before exporting."));
            return;
        }

        // save stuff to file
        Base::ofstream str(fi, std::ios::out | std::ios::binary);
        App::Document* doc = sel.front()->getDocument();
        MergeDocuments mimeView(doc);
        doc->exportObjects(sel, str);
        str.close();
    }
    App::Document* doc = App::GetApplication().getActiveDocument();
    if (doc) {
        bool proceed = true;
        if(hasXLink && !doc->isSaved()) {
            int ret = QMessageBox::question(getMainWindow(),
                qApp->translate("Std_DuplicateSelection","Object dependencies"),
                qApp->translate("Std_DuplicateSelection",
                "To link to external objects, the document must be saved at least once.\n"
                "Do you want to save the document now?"),
                QMessageBox::Yes,QMessageBox::No);
            if(ret == QMessageBox::Yes)
                proceed = Application::Instance->getDocument(doc)->saveAs();
        }
        if(proceed) {
            doc->openTransaction("Duplicate");
            // restore objects from file and add to active document
            Base::ifstream str(fi, std::ios::in | std::ios::binary);
            MergeDocuments mimeView(doc);
            mimeView.importObjects(str);
            str.close();
            doc->commitTransaction();
        }
    }
    fi.deleteFile();
}

bool StdCmdDuplicateSelection::isActive()
{
    return Gui::Selection().hasSelection();
}

//===========================================================================
// Std_SelectAll
//===========================================================================

DEF_STD_CMD_A(StdCmdSelectAll)

StdCmdSelectAll::StdCmdSelectAll()
  : Command("Std_SelectAll")
{
    sGroup        = "Edit";
    sMenuText     = QT_TR_NOOP("Select &All");
    sToolTipText  = QT_TR_NOOP("Select all");
    sWhatsThis    = "Std_SelectAll";
    sStatusTip    = QT_TR_NOOP("Select all");
    sPixmap       = "edit-select-all";
    //sAccel        = "Ctrl+A"; // superseeds shortcuts for text edits
}

void StdCmdSelectAll::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    SelectionSingleton& rSel = Selection();
    App::Document* doc = App::GetApplication().getActiveDocument();
    std::vector<App::DocumentObject*> objs = doc->getObjectsOfType(App::DocumentObject::getClassTypeId());
    rSel.setSelection(doc->getName(), objs);
}

bool StdCmdSelectAll::isActive()
{
    return App::GetApplication().getActiveDocument() != nullptr;
}

//===========================================================================
// Std_Delete
//===========================================================================
DEF_STD_CMD_A(StdCmdDelete)

StdCmdDelete::StdCmdDelete()
  :Command("Std_Delete")
{
  sGroup        = "Edit";
  sMenuText     = QT_TR_NOOP("&Delete");
  sToolTipText  = QT_TR_NOOP("Deletes the selected objects");
  sWhatsThis    = "Std_Delete";
  sStatusTip    = QT_TR_NOOP("Deletes the selected objects");
  sPixmap       = "edit-delete";
  sAccel        = keySequenceToAccel(QKeySequence::Delete);
  eType         = ForEdit;
}

void StdCmdDelete::activated(int iMsg)
{
    Q_UNUSED(iMsg);

    std::set<App::Document*> docs;
    try {
        openCommand(QT_TRANSLATE_NOOP("Command", "Delete"));
        if (getGuiApplication()->sendHasMsgToFocusView(getName())) {
            commitCommand();
            return;
        }

        App::TransactionLocker tlock;

        Gui::getMainWindow()->setUpdatesEnabled(false);
        auto editDoc = Application::Instance->editDocument();
        ViewProviderDocumentObject *vpedit = nullptr;
        if(editDoc)
            vpedit = dynamic_cast<ViewProviderDocumentObject*>(editDoc->getInEdit());
        if(vpedit) {
            for(auto &sel : Selection().getSelectionEx(editDoc->getDocument()->getName())) {
                if(sel.getObject() == vpedit->getObject()) {
                    if (!sel.getSubNames().empty()) {
                        vpedit->onDelete(sel.getSubNames());
                        docs.insert(editDoc->getDocument());
                    }
                    break;
                }
            }
        } else {
            std::set<QString> affectedLabels;
            bool more = false;
            auto sels = Selection().getSelectionEx();
            bool autoDeletion = true;
            for(auto &sel : sels) {
                auto obj = sel.getObject();
                for(auto parent : obj->getInList()) {
                    if(!Selection().isSelected(parent)) {
                        ViewProvider* vp = Application::Instance->getViewProvider(parent);
                        if (vp && !vp->canDelete(obj)) {
                            autoDeletion = false;
                            QString label;
                            if(parent->getDocument() != obj->getDocument())
                                label = QLatin1String(parent->getFullName().c_str());
                            else
                                label = QLatin1String(parent->getNameInDocument());
                            if(parent->Label.getStrValue() != parent->getNameInDocument())
                                label += QString::fromLatin1(" (%1)").arg(
                                        QString::fromUtf8(parent->Label.getValue()));
                            affectedLabels.insert(label);
                            if(affectedLabels.size()>=10) {
                                more = true;
                                break;
                            }
                        }
                    }
                }
                if(more)
                    break;
            }

            // The check below is not needed because we now only get selection
            // from the active document
#if 0
            //check for inactive objects in selection  Mantis #3477
            std::set<QString> inactiveLabels;
            App::Application& app = App::GetApplication();
            App::Document* actDoc = app.getActiveDocument();
            for (std::vector<Gui::SelectionObject>::iterator ft = sels.begin(); ft != sels.end(); ++ft) {
                App::DocumentObject* obj = ft->getObject();
                App::Document* objDoc = obj->getDocument();
                if (actDoc != objDoc) {
                    inactiveLabels.insert(QString::fromUtf8(obj->Label.getValue()));
                    autoDeletion = false;
                }
            }
#endif

            if (!autoDeletion) {
                QString bodyMessage;
                QTextStream bodyMessageStream(&bodyMessage);
                bodyMessageStream << qApp->translate("Std_Delete",
                                                     "The following referencing objects might break.\n\n"
                                                     "Are you sure you want to continue?\n");
                for (const auto &currentLabel : affectedLabels)
                    bodyMessageStream << '\n' << currentLabel;
                if(more)
                    bodyMessageStream << "\n...";
#if 0
                //message for inactive items
                if (!inactiveLabels.empty()) {
                    if (!affectedLabels.empty()) {
                        bodyMessageStream << "\n";
                    }
                    std::string thisDoc = pGuiDoc->getDocument()->getName();
                    bodyMessageStream << qApp->translate("Std_Delete",
                                            "These items are selected for deletion, but are not in the active document.");
                    for (const auto &currentLabel : inactiveLabels)
                        bodyMessageStream << currentLabel << " / " << Base::Tools::fromStdString(thisDoc) << '\n';
                }
#endif

                int ret = QMessageBox::warning(Gui::getMainWindow(),
                    qApp->translate("Std_Delete", "Object dependencies"), bodyMessage,
                    QMessageBox::Yes, QMessageBox::No);
                if (ret == QMessageBox::Yes)
                    autoDeletion = true;
            }
            if (autoDeletion) {
                for(auto &sel : sels) {
                    auto obj = sel.getObject();
                    Gui::ViewProvider* vp = Application::Instance->getViewProvider(obj);
                    if (vp) {
                        // ask the ViewProvider if it wants to do some clean up
                        if (vp->onDelete(sel.getSubNames())) {
                            docs.insert(obj->getDocument());
                            FCMD_OBJ_DOC_CMD(obj,"removeObject('" << obj->getNameInDocument() << "')");
                        }
                    }
                }
            }
        }
        if(docs.size()) {
            const auto &outList = App::PropertyXLink::getDocumentOutList();
            for(auto it=docs.begin();it!=docs.end();++it) {
                auto itd = outList.find(*it);
                if(itd!=outList.end()) {
                    for(auto doc : itd->second) {
                        if(doc != *it)
                            docs.erase(doc);
                    }
                }
            }
            for(auto doc : docs) {
                FCMD_DOC_CMD(doc,"recompute()");
            }
        }
    } catch (const Base::Exception& e) {
        QMessageBox::critical(getMainWindow(), QObject::tr("Delete failed"),
                QString::fromLatin1(e.what()));
        e.ReportException();
    } catch (...) {
        QMessageBox::critical(getMainWindow(), QObject::tr("Delete failed"),
                QString::fromLatin1("Unknown error"));
    }
    commitCommand();
    Gui::getMainWindow()->setUpdatesEnabled(true);
    Gui::getMainWindow()->update();
}

bool StdCmdDelete::isActive()
{
    return Selection().getCompleteSelection().size() > 0;
}

//===========================================================================
// Std_Refresh
//===========================================================================
DEF_STD_CMD_A(StdCmdRefresh)

StdCmdRefresh::StdCmdRefresh()
  : Command("Std_Refresh")
{
    sGroup        = "Edit";
    sMenuText     = QT_TR_NOOP("&Refresh");
    sToolTipText  = QT_TR_NOOP("Recomputes the current active document");
    sWhatsThis    = "Std_Refresh";
    sStatusTip    = QT_TR_NOOP("Recomputes the current active document");
    sPixmap       = "view-refresh";
    sAccel        = keySequenceToAccel(QKeySequence::Refresh);
    eType         = AlterDoc | Alter3DView | AlterSelection | ForEdit;
    bCanLog        = false;

    // Make it optional to create a transaction for a recompute.
    // The new default behaviour is quite cumbersome in some cases because when
    // undoing the last transaction the manual recompute will clear the redo stack.
    ParameterGrp::handle hGrp = App::GetApplication().GetParameterGroupByPath(
            "User parameter:BaseApp/Preferences/Document");
    bool create = hGrp->GetBool("TransactionOnRecompute", false);
    if (!create)
        eType = eType | NoTransaction;
}

void StdCmdRefresh::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    if (getActiveGuiDocument()) {
        App::AutoTransaction trans((eType & NoTransaction) ? nullptr : "Recompute");
        try {
            doCommand(Doc,"App.activeDocument().recompute(None,True,True)");
        }
        catch (Base::Exception& /*e*/) {
            int ret = QMessageBox::warning(getMainWindow(), QObject::tr("Dependency error"),
                qApp->translate("Std_Refresh", "The document contains dependency cycles.\n"
                            "Please check the Report View for more details.\n\n"
                            "Do you still want to proceed?"),
                    QMessageBox::Yes, QMessageBox::No);
            if(ret == QMessageBox::No)
                return;
            doCommand(Doc,"App.activeDocument().recompute(None,True)");
        }
    }
}

bool StdCmdRefresh::isActive()
{
    return this->getDocument() && this->getDocument()->mustExecute();
}

// //===========================================================================
// // Std_Edit
// //===========================================================================
// DEF_STD_CMD_A(StdCmdEdit)

// StdCmdEdit::StdCmdEdit()
//   : Command("Std_Edit")
// {
//     sGroup        = "Edit";
//     sMenuText     = QT_TR_NOOP("Toggle &Edit mode");
//     sToolTipText  = QT_TR_NOOP("Toggles the selected object's edit mode");
//     sWhatsThis    = "Std_Edit";
//     sStatusTip    = QT_TR_NOOP("Activates or Deactivates the selected object's edit mode");
//     sAccel        = "";
//     sPixmap       = "edit-edit";
//     eType         = ForEdit;
// }

// void StdCmdEdit::activated(int iMsg)
// {
//     Q_UNUSED(iMsg);
// }

// bool StdCmdEdit::isActive()
// {
//     return (Selection().getCompleteSelection().size() > 0) || (Gui::Control().activeDialog() != nullptr);
// }

namespace Gui {

void CreateDocCommands()
{
    CommandManager &rcCmdMgr = Application::Instance->commandManager();

    rcCmdMgr.addCommand(new StdCmdNew());
    rcCmdMgr.addCommand(new StdCmdOpen());
    rcCmdMgr.addCommand(new StdCmdImport());
    rcCmdMgr.addCommand(new StdCmdExport());
    rcCmdMgr.addCommand(new StdCmdMergeProjects());
    rcCmdMgr.addCommand(new StdCmdDependencyGraph());
    rcCmdMgr.addCommand(new StdCmdExportDependencyGraph());

    rcCmdMgr.addCommand(new StdCmdSave());
    rcCmdMgr.addCommand(new StdCmdSaveAs());
    rcCmdMgr.addCommand(new StdCmdSaveCopy());
    rcCmdMgr.addCommand(new StdCmdSaveAll());
    rcCmdMgr.addCommand(new StdCmdRevert());
    rcCmdMgr.addCommand(new StdCmdProjectInfo());
    rcCmdMgr.addCommand(new StdCmdProjectUtil());
    rcCmdMgr.addCommand(new StdCmdUndo());
    rcCmdMgr.addCommand(new StdCmdRedo());
    rcCmdMgr.addCommand(new StdCmdPrint());
    rcCmdMgr.addCommand(new StdCmdPrintPreview());
    rcCmdMgr.addCommand(new StdCmdPrintPdf());
    rcCmdMgr.addCommand(new StdCmdQuit());
    rcCmdMgr.addCommand(new StdCmdCut());
    rcCmdMgr.addCommand(new StdCmdCopy());
    rcCmdMgr.addCommand(new StdCmdPaste());
    rcCmdMgr.addCommand(new StdCmdDuplicateSelection());
    rcCmdMgr.addCommand(new StdCmdSelectAll());
    rcCmdMgr.addCommand(new StdCmdDelete());
    rcCmdMgr.addCommand(new StdCmdRefresh());
    // rcCmdMgr.addCommand(new StdCmdEdit());
}

} // namespace Gui
