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




#include "DlgGaussianSwellSpectrum.h"
#include <Mod/SeaLabPlugins/SeaLabPlugin/ui_DlgGaussianSwellSpectrum.h>
#include <Mod/SeaLabPlugins/SeaLabPlugin/RPSGaussianSwellSpectrum.h>

#include <QSignalMapper>
#include <App/Application.h>
#include <App/Document.h>

//#include <QPushButton>
//#include <QApplication>


using namespace SeaLabGui;

/* TRANSLATOR SeaLabGui::DlgGaussianSwellSpectrum */

DlgGaussianSwellSpectrum::DlgGaussianSwellSpectrum(const App::PropertyLength&  SignificantWaveHeight, const App::PropertyTime&  PeakPeriod, const App::PropertyFloat&  Sigma, const App::PropertyString& featureName, QWidget* parent)
	: QWidget(parent), ui(new Ui_DlgGaussianSwellSpectrum), _featureName(featureName.getStrValue())
{
	ui->setupUi(this);

    ui->doubleSpinBox_SignificantWaveHeight->setUnit(Base::Unit::Length);
    ui->doubleSpinBox_PeakPeriod->setUnit(Base::Unit::TimeSpan);

    ui->doubleSpinBox_SignificantWaveHeight->setValue(SignificantWaveHeight.getValue());
    ui->doubleSpinBox_PeakPeriod->setValue(PeakPeriod.getValue());
    ui->doubleSpinBox_Sigma->setValue(Sigma.getValue());

    ui->label_Image->setPixmap(QPixmap(":icons/RPSGaussianSwellSpectrum.png"));


}

/*
 *  Destroys the object and frees any allocated resources
 */
DlgGaussianSwellSpectrum::~DlgGaussianSwellSpectrum()
{
}

void DlgGaussianSwellSpectrum::accept()
{
    auto doc = App::GetApplication().getActiveDocument();
    if(!doc)
	    return;

    SeaLab::CRPSGaussianSwellSpectrum* activefeature = static_cast<SeaLab::CRPSGaussianSwellSpectrum*>(doc->getObject(_featureName.c_str()));
    if (!activefeature)
        return;

    activefeature->SignificantWaveHeight.setValue(ui->doubleSpinBox_SignificantWaveHeight->value().getValue());
    activefeature->PeakPeriod.setValue(ui->doubleSpinBox_PeakPeriod->value().getValue());
    activefeature->Sigma.setValue(ui->doubleSpinBox_Sigma->value().getValue());

  }

void DlgGaussianSwellSpectrum::reject()
{

}


// ----------------------------------------------

/* TRANSLATOR PartGui::DlgGaussianSwellSpectrumEdit */

DlgGaussianSwellSpectrumEdit::DlgGaussianSwellSpectrumEdit(const App::PropertyLength&  SignificantWaveHeight, const App::PropertyTime&  PeakPeriod, const App::PropertyFloat&  Sigma, const App::PropertyString& featureName)
{
	// create and show dialog for the SeaLabFeatures
    widget = new DlgGaussianSwellSpectrum(SignificantWaveHeight, PeakPeriod, Sigma, featureName, nullptr);
	taskbox = new Gui::TaskView::TaskBox(QPixmap(), widget->windowTitle(), true, nullptr);
	taskbox->groupLayout()->addWidget(widget);
	Content.push_back(taskbox);

}

DlgGaussianSwellSpectrumEdit::~DlgGaussianSwellSpectrumEdit()
{
	// automatically deleted in the sub-class
}

QDialogButtonBox::StandardButtons DlgGaussianSwellSpectrumEdit::getStandardButtons() const
{
	return QDialogButtonBox::Cancel | QDialogButtonBox::Ok;
}

bool DlgGaussianSwellSpectrumEdit::accept()
{ 
	widget->accept();
	return true;
}

bool DlgGaussianSwellSpectrumEdit::reject()
{
	widget->reject();
	return true;
}

//#include "moc_DlgGaussianSwellSpectrum.cpp"
