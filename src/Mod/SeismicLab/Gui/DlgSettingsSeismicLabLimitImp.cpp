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

#include "PreCompiled.h"

#include "DlgSettingsSeismicLabLimitImp.h"
#include "ui_DlgSettingsSeismicLabLimit.h"
#include <Gui/Application.h>


using namespace SeismicLabGui;

DlgSettingsSeismicLabLimitImp::DlgSettingsSeismicLabLimitImp(QWidget* parent)
    : PreferencePage(parent)
    , ui(new Ui_DlgSettingsSeismicLabLimitImp)
{
    ui->setupUi(this);
}

DlgSettingsSeismicLabLimitImp::~DlgSettingsSeismicLabLimitImp()
{
    // no need to delete child widgets, Qt does it all for us
}

void DlgSettingsSeismicLabLimitImp::saveSettings()
{
     ParameterGrp::handle hGrp = App::GetApplication().GetParameterGroupByPath(
        "User parameter:BaseApp/Preferences/Mod/SeismicLab/Limit");

    hGrp->SetInt("MaxSN", ui->sb_lm_SampleNumber->value());
    ui->sb_lm_SampleNumber->onSave();
    hGrp->SetInt("MaxSPN", ui->sb_lm_SimulationPointsNumber->value());
    ui->sb_lm_SimulationPointsNumber->onSave();
    hGrp->SetInt("MaxFIN", ui->sb_lm_FrequencyNumber->value());
    ui->sb_lm_FrequencyNumber->onSave();
    hGrp->SetInt("MaxTIN", ui->sb_lm_TimeNumber->value());
    ui->sb_lm_TimeNumber->onSave();
    hGrp->SetInt("MaxDIN", ui->sb_lm_DirectionNumber->value());
    ui->sb_lm_DirectionNumber->onSave();
    hGrp->SetInt("MaxXIN", ui->sb_lm_XValueNumber->value());
    ui->sb_lm_XValueNumber->onSave();
}

void DlgSettingsSeismicLabLimitImp::loadSettings()
{
    ParameterGrp::handle hGrp = App::GetApplication().GetParameterGroupByPath(
        "User parameter:BaseApp/Preferences/Mod/SeismicLab/Limit");

    int MaxSN = hGrp->GetInt("MaxSN", 300);
    if (MaxSN > -1)
        ui->sb_lm_SampleNumber->setValue(MaxSN);

    int MaxSPN = hGrp->GetInt("MaxSPN", 300);
    if (MaxSPN > -1)
        ui->sb_lm_SimulationPointsNumber->setValue(MaxSPN);

    int MaxFIN = hGrp->GetInt("MaxFIN", 102400);
    if (MaxFIN > -1)
        ui->sb_lm_FrequencyNumber->setValue(MaxFIN);

    int MaxTIN = hGrp->GetInt("MaxTIN", 61400);
    if (MaxTIN > -1)
        ui->sb_lm_TimeNumber->setValue(MaxTIN);

    int MaxDIN = hGrp->GetInt("MaxDIN", 100000);
    if (MaxDIN > -1)
        ui->sb_lm_DirectionNumber->setValue(MaxDIN);

    int MaxXIN = hGrp->GetInt("MaxXIN", 100000);
    if (MaxXIN > -1)
        ui->sb_lm_XValueNumber->setValue(MaxXIN);
}

/**
 * Sets the strings of the subwidgets using the current language.
 */
void DlgSettingsSeismicLabLimitImp::changeEvent(QEvent* e)
{
    if (e->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    else {
        QWidget::changeEvent(e);
    }
}

#include "moc_DlgSettingsSeismicLabLimitImp.cpp"
