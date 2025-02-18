/***************************************************************************
 *   Copyright (c) 2007 Werner Mayer <wmayer[at]users.sourceforge.net>     *
 *                                                                         *
 *   This file is part of the LabRPS development system.              *
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

#ifndef SEISMICLABGUI_DlgWavePassageEffect_H
#define SEISMICLABGUI_DlgWavePassageEffect_H


#include <Gui/TaskView/TaskDialog.h>
#include <Gui/TaskView/TaskView.h>
#include <App/PropertyUnits.h>
#include <QString>

class QSignalMapper;

namespace SeismicLabGui {

class Ui_DlgWavePassageEffect;
class DlgWavePassageEffect : public QWidget
{
    Q_OBJECT

public:
    DlgWavePassageEffect(const App::PropertySpeed& ApparentWaveVelocity, const App::PropertyFloat& Coefficient, const App::PropertyString& featureName, QWidget* parent = nullptr);
    ~DlgWavePassageEffect();
    void accept();
    void reject();

       

private:
    std::unique_ptr<Ui_DlgWavePassageEffect> ui;
    std::string _featureName;
};

class DlgWavePassageEffectEdit : public Gui::TaskView::TaskDialog
{
    Q_OBJECT

public:
    DlgWavePassageEffectEdit(const App::PropertySpeed& ApparentWaveVelocity, const App::PropertyFloat& Coefficient, const App::PropertyString& featureName);
    ~DlgWavePassageEffectEdit();
    
public:
    bool accept();
    bool reject();
    QDialogButtonBox::StandardButtons getStandardButtons() const;

public:
    DlgWavePassageEffect* widget;
    Gui::TaskView::TaskBox* taskbox;

};

} // namespace SeismicLabGui

#endif // SEISMICLABGUI_DlgWavePassageEffect_H
