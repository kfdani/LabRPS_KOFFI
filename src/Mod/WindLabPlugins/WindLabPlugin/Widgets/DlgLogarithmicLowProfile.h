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

#ifndef WINDLABGUI_DlgLogarithmicLowProfile_H
#define WINDLABGUI_DlgLogarithmicLowProfile_H


#include <Gui/TaskView/TaskDialog.h>
#include <Gui/TaskView/TaskView.h>
#include <App/PropertyUnits.h>
#include <QString>

class QSignalMapper;

namespace WindLabGui {

class Ui_DlgLogarithmicLowProfile;
class DlgLogarithmicLowProfile : public QWidget
{
    Q_OBJECT

public:
    DlgLogarithmicLowProfile(const App::PropertyLength& TerrainRoughness, const App::PropertySpeed& ShearVelocity, const App::PropertyLength& ZeroPlanDisplacement, const App::PropertyString& featureName, QWidget* parent = nullptr);
    ~DlgLogarithmicLowProfile();
    void accept();
    void reject();

     

private:
    std::unique_ptr<Ui_DlgLogarithmicLowProfile> ui;
    std::string _featureName;
};

class DlgLogarithmicLowProfileEdit : public Gui::TaskView::TaskDialog
{
    Q_OBJECT

public:
    DlgLogarithmicLowProfileEdit(const App::PropertyLength& TerrainRoughness, const App::PropertySpeed& ShearVelocity, const App::PropertyLength& ZeroPlanDisplacement, const App::PropertyString& featureName);
    ~DlgLogarithmicLowProfileEdit();
    
public:
    bool accept();
    bool reject();
    QDialogButtonBox::StandardButtons getStandardButtons() const;

public:
    DlgLogarithmicLowProfile* widget;
    Gui::TaskView::TaskBox* taskbox;

};

} // namespace WindLabGui

#endif // WINDLABGUI_DlgLogarithmicLowProfile_H
