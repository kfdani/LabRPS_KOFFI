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

#ifndef SEALABGUI_DlgGaussianSwellSpectrum_H
#define SEALABGUI_DlgGaussianSwellSpectrum_H


#include <Gui/TaskView/TaskDialog.h>
#include <Gui/TaskView/TaskView.h>
#include <App/PropertyUnits.h>
#include <QString>

class QSignalMapper;

namespace SeaLabGui {

class Ui_DlgGaussianSwellSpectrum;
class DlgGaussianSwellSpectrum : public QWidget
{
    Q_OBJECT

public:
    DlgGaussianSwellSpectrum(const App::PropertyLength&  SignificantWaveHeight, const App::PropertyTime&  PeakPeriod, const App::PropertyFloat&  Sigma, const App::PropertyString& featureName, QWidget* parent = nullptr);
    
    ~DlgGaussianSwellSpectrum();
    void accept();
    void reject();

       

private:
    std::unique_ptr<Ui_DlgGaussianSwellSpectrum> ui;
    std::string _featureName;
};

class DlgGaussianSwellSpectrumEdit : public Gui::TaskView::TaskDialog
{
    Q_OBJECT

public:
    DlgGaussianSwellSpectrumEdit(const App::PropertyLength&  SignificantWaveHeight, const App::PropertyTime&  PeakPeriod, const App::PropertyFloat&  Sigma, const App::PropertyString& featureName);
    ~DlgGaussianSwellSpectrumEdit();
    
public:
    bool accept();
    bool reject();
    QDialogButtonBox::StandardButtons getStandardButtons() const;

public:
    DlgGaussianSwellSpectrum* widget;
    Gui::TaskView::TaskBox* taskbox;

};

} // namespace SeaLabGui

#endif // SEALABGUI_DlgGaussianSwellSpectrum_H
