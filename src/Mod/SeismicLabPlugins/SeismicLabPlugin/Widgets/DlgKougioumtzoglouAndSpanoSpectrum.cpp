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




#include "DlgKougioumtzoglouAndSpanoSpectrum.h"
#include <Mod/SeismicLabPlugins/SeismicLabPlugin/ui_DlgKougioumtzoglouAndSpanoSpectrum.h>
#include <Mod/SeismicLabPlugins/SeismicLabPlugin/RPSKougioumtzoglouAndSpanoSpectrum.h>

#include <QSignalMapper>
#include <App/Application.h>
#include <App/Document.h>

//#include <QPushButton>
//#include <QApplication>


using namespace SeismicLabGui;

/* TRANSLATOR SeismicLabGui::DlgKougioumtzoglouAndSpanoSpectrum */

DlgKougioumtzoglouAndSpanoSpectrum::DlgKougioumtzoglouAndSpanoSpectrum(const App::PropertyFloat& ConstantS, const App::PropertyString& featureName, QWidget* parent)
	: QWidget(parent), ui(new Ui_DlgKougioumtzoglouAndSpanoSpectrum), _featureName(featureName.getStrValue())
{
	ui->setupUi(this);

    ui->doubleSpinBox_ConstantS->setValue(ConstantS.getValue());

    ui->label_Image->setPixmap(QPixmap(":icons/RPSKougioumtzoglouAndSpanoSpectrum.png"));


}

/*
 *  Destroys the object and frees any allocated resources
 */
DlgKougioumtzoglouAndSpanoSpectrum::~DlgKougioumtzoglouAndSpanoSpectrum()
{
}

void DlgKougioumtzoglouAndSpanoSpectrum::accept()
{
    auto doc = App::GetApplication().getActiveDocument();
    if(!doc)
	    return;

    SeismicLab::CRPSKougioumtzoglouAndSpanoSpectrum* activefeature = static_cast<SeismicLab::CRPSKougioumtzoglouAndSpanoSpectrum*>(doc->getObject(_featureName.c_str()));
    if (!activefeature)
        return;

    activefeature->ConstantS.setValue(ui->doubleSpinBox_ConstantS->value().getValue());
  }

void DlgKougioumtzoglouAndSpanoSpectrum::reject()
{

}


// ----------------------------------------------

/* TRANSLATOR PartGui::DlgKougioumtzoglouAndSpanoSpectrumEdit */

DlgKougioumtzoglouAndSpanoSpectrumEdit::DlgKougioumtzoglouAndSpanoSpectrumEdit(const App::PropertyFloat& ConstantS, const App::PropertyString& featureName)
{
	// create and show dialog for the SeismicLabFeatures
    widget = new DlgKougioumtzoglouAndSpanoSpectrum(ConstantS, featureName, nullptr);
	taskbox = new Gui::TaskView::TaskBox(QPixmap(), widget->windowTitle(), true, nullptr);
	taskbox->groupLayout()->addWidget(widget);
	Content.push_back(taskbox);

}

DlgKougioumtzoglouAndSpanoSpectrumEdit::~DlgKougioumtzoglouAndSpanoSpectrumEdit()
{
	// automatically deleted in the sub-class
}

QDialogButtonBox::StandardButtons DlgKougioumtzoglouAndSpanoSpectrumEdit::getStandardButtons() const
{
	return QDialogButtonBox::Cancel | QDialogButtonBox::Ok;
}

bool DlgKougioumtzoglouAndSpanoSpectrumEdit::accept()
{ 
	widget->accept();
	return true;
}

bool DlgKougioumtzoglouAndSpanoSpectrumEdit::reject()
{
	widget->reject();
	return true;
}

//#include "moc_DlgKougioumtzoglouAndSpanoSpectrum.cpp"
