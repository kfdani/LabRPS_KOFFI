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




#include "DlgvonKarmanAcrossWindSpectrum.h"
#include <Mod/WindLabPlugins/WindLabPlugin/ui_DlgvonKarmanAcrossWindSpectrum.h>
#include <Mod/WindLabPlugins/WindLabPlugin/RPSvonKarmanAcrossWindSpectrum.h>
#include <QSignalMapper>
#include <App/Application.h>
#include <App/Document.h>

//#include <QPushButton>
//#include <QApplication>


using namespace WindLabGui;

/* TRANSLATOR WindLabGui::DlgvonKarmanAcrossWindSpectrum */

DlgvonKarmanAcrossWindSpectrum::DlgvonKarmanAcrossWindSpectrum(const App::PropertyFloat& Constant1, const App::PropertyFloat& Constant2, const App::PropertyFloat& IntegralLengthScale, const App::PropertyFloat& StandardDeviation, const App::PropertyString& featureName, QWidget* parent)
	: QWidget(parent), ui(new Ui_DlgvonKarmanAcrossWindSpectrum), _featureName(featureName.getStrValue())
{
	ui->setupUi(this);

    ui->doubleSpinBox_Constant1->setRange(0.00, DBL_MAX);
    ui->doubleSpinBox_Constant2->setRange(0.00, DBL_MAX);
    ui->doubleSpinBox_IntegralLengthScale->setRange(0.00, DBL_MAX);
    ui->doubleSpinBox_StandardDeviation->setRange(0.00, DBL_MAX);

    ui->doubleSpinBox_Constant1->setValue(Constant1.getValue());
    ui->doubleSpinBox_Constant2->setValue(Constant2.getValue());
    ui->doubleSpinBox_IntegralLengthScale->setValue(IntegralLengthScale.getValue());
    ui->doubleSpinBox_StandardDeviation->setValue(StandardDeviation.getValue());

    ui->label_Image->setPixmap(QPixmap(":icons/RPSvonKarmanAlongWindSpectrum.png"));


}

/*
 *  Destroys the object and frees any allocated resources
 */
DlgvonKarmanAcrossWindSpectrum::~DlgvonKarmanAcrossWindSpectrum()
{
}

void DlgvonKarmanAcrossWindSpectrum::accept()
{
    auto doc = App::GetApplication().getActiveDocument();
    if(!doc)
	    return;
    //auto feature = doc->getObject(_featureName.c_str());
    WindLab::CRPSvonKarmanAcrossWindSpectrum* activefeature = static_cast<WindLab::CRPSvonKarmanAcrossWindSpectrum*>(doc->getObject(_featureName.c_str()));
    if (!activefeature)
        return;
    activefeature->Constant1.setValue(ui->doubleSpinBox_Constant1->value().getValue());
    activefeature->Constant2.setValue(ui->doubleSpinBox_Constant2->value().getValue());
    activefeature->IntegralLengthScale.setValue(ui->doubleSpinBox_IntegralLengthScale->value().getValue());
    activefeature->StandardDeviation.setValue(ui->doubleSpinBox_StandardDeviation->value().getValue());


  }

void DlgvonKarmanAcrossWindSpectrum::reject()
{

}


// ----------------------------------------------

/* TRANSLATOR PartGui::DlgvonKarmanAcrossWindSpectrumEdit */

DlgvonKarmanAcrossWindSpectrumEdit::DlgvonKarmanAcrossWindSpectrumEdit(const App::PropertyFloat& Constant1, const App::PropertyFloat& Constant2, const App::PropertyFloat& IntegralLengthScale, const App::PropertyFloat& StandardDeviation, const App::PropertyString& featureName)
{
	// create and show dialog for the WindLabFeatures
    widget = new DlgvonKarmanAcrossWindSpectrum(Constant1, Constant2, IntegralLengthScale, StandardDeviation, featureName, nullptr);
	taskbox = new Gui::TaskView::TaskBox(QPixmap(), widget->windowTitle(), true, nullptr);
	taskbox->groupLayout()->addWidget(widget);
	Content.push_back(taskbox);

}

DlgvonKarmanAcrossWindSpectrumEdit::~DlgvonKarmanAcrossWindSpectrumEdit()
{
	// automatically deleted in the sub-class
}

QDialogButtonBox::StandardButtons DlgvonKarmanAcrossWindSpectrumEdit::getStandardButtons() const
{
	return QDialogButtonBox::Cancel | QDialogButtonBox::Ok;
}

bool DlgvonKarmanAcrossWindSpectrumEdit::accept()
{ 
	widget->accept();
	return true;
}

bool DlgvonKarmanAcrossWindSpectrumEdit::reject()
{
	widget->reject();
	return true;
}

//#include "moc_DlgvonKarmanAcrossWindSpectrum.cpp"
