
/***************************************************************************
 *   Copyright (c) 2024 Koffi Daniel <kfdani@labrps.com>                   *
 *                                                                         *
 *   This file is part of the LabRPS development system.                   *
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

#ifndef WINDLABGUI_DlgImportSimulationPointsFromFile_H
#define WINDLABGUI_DlgImportSimulationPointsFromFile_H


#include <Gui/TaskView/TaskDialog.h>
#include <Gui/TaskView/TaskView.h>
#include <App/PropertyUnits.h>
#include <QString>

class QSignalMapper;

namespace WindLabGui {

class Ui_DlgImportSimulationPointsFromFile;
class DlgImportSimulationPointsFromFile : public QWidget
{
    Q_OBJECT

public:
    DlgImportSimulationPointsFromFile(const App::PropertyString& FilePath, const App::PropertyString& featureName, QWidget* parent = nullptr);
    ~DlgImportSimulationPointsFromFile();
    void accept();
    void reject();

 public Q_SLOTS:
    void slotSetFilePath();   

private:
    std::unique_ptr<Ui_DlgImportSimulationPointsFromFile> ui;
    std::string _featureName;
    QString workingDirectory;
};

class DlgImportSimulationPointsFromFileEdit : public Gui::TaskView::TaskDialog
{
    Q_OBJECT

public:
    DlgImportSimulationPointsFromFileEdit(const App::PropertyString& FilePath, const App::PropertyString& featureName);
    ~DlgImportSimulationPointsFromFileEdit();
    
public:
    bool accept();
    bool reject();
    QDialogButtonBox::StandardButtons getStandardButtons() const;

public:
    DlgImportSimulationPointsFromFile* widget;
    Gui::TaskView::TaskBox* taskbox;
};

} // namespace WindLabGui

#endif // WINDLABGUI_DlgImportSimulationPointsFromFile_H
