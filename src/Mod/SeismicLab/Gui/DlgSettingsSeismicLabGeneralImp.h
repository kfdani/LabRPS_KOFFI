/***************************************************************************
 *   Copyright (c) 2024 Koffi Daniel <kfdani@labrps.com>                   *                *
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

#ifndef SEISMICLABGUI_DLGSETTINGSFEMGENERALIMP_H
#define SEISMICLABGUI_DLGSETTINGSFEMGENERALIMP_H

#include <memory>
#include <Gui/PropertyPage.h>


namespace SeismicLabGui {
class Ui_DlgSettingsSeismicLabGeneralImp;
class DlgSettingsSeismicLabGeneralImp : public Gui::Dialog::PreferencePage
{
    Q_OBJECT

public:
    explicit DlgSettingsSeismicLabGeneralImp( QWidget* parent = nullptr );
    ~DlgSettingsSeismicLabGeneralImp() override;

protected Q_SLOTS:
    void onfileNameChanged(QString FileName);
    
protected:
    void saveSettings() override;
    void loadSettings() override;
    void changeEvent(QEvent *e) override;

private:
    std::unique_ptr<Ui_DlgSettingsSeismicLabGeneralImp> ui;
};

} // namespace SeismicLabGui

#endif // FEMGUI_DLGSETTINGSFEMGENERALIMP_H