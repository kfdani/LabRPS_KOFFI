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


#include "PreCompiled.h"

#ifndef _PreComp_
#endif

#include "ViewProviderWindLabFeatureYSpectrum.h"
#include "ViewProviderWindLabFeatureYSpectrumPy.h"

#include <App/Application.h>
#include <Mod/WindLab/App/WindLabSimulation.h>
#include <Mod/WindLab/Gui/DlgWindLabFeaturesCreation.h>
#include <Gui/Control.h>
#include <Mod/WindLab/Gui/WindLabSimulationObserver.h>
#include <App/Document.h>
#include <Gui/ActionFunction.h>
#include "ViewProviderWindLabSimulation.h"
#include <Gui/Application.h>
#include <Gui/Document.h>
#include <Mod/WindLab/App/WindLabUtils.h>
#include <Gui/AlphaPlot.h>
#include <Gui/MainWindow.h>
#include <Mod/WindLabAPI/App/IrpsWLYSpectrum.h>

using namespace WindLabGui;

//**************************************************************************
// Construction/Destruction

PROPERTY_SOURCE(WindLabGui::ViewProviderWindLabFeatureYSpectrum, Gui::ViewProviderDocumentObject)


ViewProviderWindLabFeatureYSpectrum::ViewProviderWindLabFeatureYSpectrum()
{
    static const char* complexDisplayGroup = "Complex Number Display";
    static const char* complexRealImagEnum[] = {"Real", "Imaginary", nullptr};

    ADD_PROPERTY_TYPE(ComplexNumberDisplay, ((long int)0), complexDisplayGroup, App::Prop_None,"This specifies how to display the complex numbers.");

    ComplexNumberDisplay.setEnums(complexRealImagEnum);

  sPixmap = ":/icons/WindLabFeatures/WindLab_Feature_SpectrumYObj.svg";
}

ViewProviderWindLabFeatureYSpectrum::~ViewProviderWindLabFeatureYSpectrum()
{

}

bool ViewProviderWindLabFeatureYSpectrum::doubleClicked(void)
{
    ActivateFeature();
    Gui::Application::Instance->activeDocument()->setEdit(this);
    OnInitialSetting();
    return true;
}

bool ViewProviderWindLabFeatureYSpectrum::ActivateFeature()
{
auto doc = App::GetApplication().getActiveDocument();
    if (!doc)
        return false;
    auto obj = doc->getObject(this->getObject()->getNameInDocument());
    if (!obj)
        return false;
    std::string parentSimName = this->getObject()->getGroup()->getNameInDocument();
    
    //get the corresponding view provider
    WindLabGui::ViewProviderWindLabSimulation* vp = dynamic_cast<WindLabGui::ViewProviderWindLabSimulation*>(Gui::Application::Instance->getViewProvider(this->getObject()->getGroup()));

    if (!vp) {
        return false;
    }

    //activate the paraent simulation
    vp->doubleClicked();

    WindLab::WindLabSimulation* parentSim = static_cast<WindLab::WindLabSimulation*>(WindLabGui::WindLabSimulationObserver::instance()->getSimulation(parentSimName));
    if (!parentSim) {Base::Console().Warning("The parent simulation of the selected feature couldn't be actived.\n");return false;}

    bool isfeatureStationary = static_cast<WindLabAPI::WindLabFeature*>(obj)->IsStationary.getValue();
    if (true == parentSim->Stationarity.getValue() && false == isfeatureStationary)
    {
        Base::Console().Warning("Simulation stationarity and that of the feature does not match. The feature couldn't be activated.\n");
        return false;
    }

    WindLabGui::WindLabSimulationObserver::instance()->setActiveSimulation(parentSim);

    parentSim->AcrossWindSpectrumModel.setValue(obj->getNameInDocument());
    return true;
}

bool ViewProviderWindLabFeatureYSpectrum::setEdit(int ModNum)
{
    Q_UNUSED(ModNum);

    if (ModNum == ViewProvider::Default) {

        return true;
    }
    else {
        return Gui::ViewProviderDocumentObject::setEdit(ModNum);
    }
}

void ViewProviderWindLabFeatureYSpectrum::unsetEdit(int ModNum)
{
    if (ModNum == ViewProvider::Default) {
        // when pressing ESC make sure to close the dialog
        Gui::Control().closeDialog();
    }
    else {
        Gui::ViewProviderDocumentObject::unsetEdit(ModNum);
    }
}

void ViewProviderWindLabFeatureYSpectrum::setupContextMenu(QMenu* menu, QObject*, const char*)
{
    // toggle command to display components
    Gui::ActionFunction* func = new Gui::ActionFunction(menu);
    QAction* cvalue = menu->addAction(QObject::tr("ComputeYCrossSpectrumValue"));
    func->trigger(cvalue, boost::bind(&ViewProviderWindLabFeatureYSpectrum::computeYCrossSpectrumValue, this));

    QAction* cvectorf = menu->addAction(QObject::tr("ComputeYCrossSpectrumVectorF"));
    func->trigger(cvectorf, boost::bind(&ViewProviderWindLabFeatureYSpectrum::computeYCrossSpectrumVectorF, this));

    QAction* cvectort = menu->addAction(QObject::tr("ComputeYCrossSpectrumVectorT"));
    func->trigger(cvectort, boost::bind(&ViewProviderWindLabFeatureYSpectrum::computeYCrossSpectrumVectorT, this));

    QAction* cmatrix = menu->addAction(QObject::tr("ComputeYCrossSpectrumMatrixPP"));
    func->trigger(cmatrix, boost::bind(&ViewProviderWindLabFeatureYSpectrum::computeYCrossSpectrumMatrixPP, this));

     QAction* autoValue = menu->addAction(QObject::tr("ComputeYAutoSpectrumValue"));
    func->trigger(autoValue, boost::bind(&ViewProviderWindLabFeatureYSpectrum::computeYAutoSpectrumValue, this));

    QAction* autoF = menu->addAction(QObject::tr("ComputeYAutoSpectrumVectorF"));
    func->trigger(autoF, boost::bind(&ViewProviderWindLabFeatureYSpectrum::computeYAutoSpectrumVectorF, this));

    QAction* autoT = menu->addAction(QObject::tr("ComputeYAutoSpectrumVectorT"));
    func->trigger(autoT, boost::bind(&ViewProviderWindLabFeatureYSpectrum::computeYAutoSpectrumVectorT, this));

    QAction* init = menu->addAction(QObject::tr("Setup Feature"));
    func->trigger(init, boost::bind(&ViewProviderWindLabFeatureYSpectrum::OnInitialSetting, this));

    QAction* act = menu->addAction(QObject::tr("Activate Feature"));
    func->trigger(act, boost::bind(&ViewProviderWindLabFeatureYSpectrum::ActivateFeature, this));
}

bool ViewProviderWindLabFeatureYSpectrum::computeYCrossSpectrumValue()
{
    
    return runFeatureMethod(WindLab::WindLabUtils::ComputeYCrossSpectrumValue, this->ComplexNumberDisplay.getValueAsString());
    
}

bool ViewProviderWindLabFeatureYSpectrum::computeYCrossSpectrumVectorT()
{
    
    return runFeatureMethod(WindLab::WindLabUtils::ComputeYCrossSpectrumVectorT, this->ComplexNumberDisplay.getValueAsString());
    
}

bool ViewProviderWindLabFeatureYSpectrum::computeYCrossSpectrumVectorF()
{
    
    return runFeatureMethod(WindLab::WindLabUtils::ComputeYCrossSpectrumVectorF, this->ComplexNumberDisplay.getValueAsString());
    
}

bool ViewProviderWindLabFeatureYSpectrum::computeYCrossSpectrumMatrixPP()
{
    
    return runFeatureMethod(WindLab::WindLabUtils::ComputeYCrossSpectrumMatrixPP, this->ComplexNumberDisplay.getValueAsString());
    
}

bool ViewProviderWindLabFeatureYSpectrum::computeYAutoSpectrumValue()
{
    
    return runFeatureMethod(WindLab::WindLabUtils::ComputeYAutoSpectrumValue);
    
}
bool ViewProviderWindLabFeatureYSpectrum::computeYAutoSpectrumVectorF()
{
    
    return runFeatureMethod(WindLab::WindLabUtils::ComputeYAutoSpectrumVectorF);
    
}
bool ViewProviderWindLabFeatureYSpectrum::computeYAutoSpectrumVectorT()
{
    
    return runFeatureMethod(WindLab::WindLabUtils::ComputeYAutoSpectrumVectorT);
    
}

bool ViewProviderWindLabFeatureYSpectrum::OnInitialSetting()
{
ActivateFeature();
    WindLab::WindLabSimulation* sim = static_cast<WindLab::WindLabSimulation*>(WindLabGui::WindLabSimulationObserver::instance()->active());
    if (!sim) {Base::Console().Warning("No valide active simulation found.\n");return false;}

    sim->windLabFeatureInitalSetting(WindLab::WindLabUtils::objGroupAcrossWindSpectrum, QString::fromUtf8(sim->getSimulationData()->acrossWindSpectrumModel.getValue()));
     
    return true;
}

PyObject* ViewProviderWindLabFeatureYSpectrum::getPyObject(void)
{
    if (PythonObject.is(Py::_None())) {
        // ref counter is set to 1
        PythonObject = Py::Object(new ViewProviderWindLabFeatureYSpectrumPy(this), true);
    }
    return Py::new_reference_to(PythonObject);
}

bool ViewProviderWindLabFeatureYSpectrum::runFeatureMethod(const QString function, const char* complexNumberDisplay)
{  
ActivateFeature();
    WindLab::WindLabSimulation* sim = static_cast<WindLab::WindLabSimulation*>(WindLabGui::WindLabSimulationObserver::instance()->active());
    if (!sim) {Base::Console().Warning("No valide active simulation found.\n");return false;}

    windLabAllFeaturesComputation = new WindLabAllFeaturesComputation(sim);
    windLabAllFeaturesComputation->startSimulationWorker(function, complexNumberDisplay);
    windLabAllFeaturesComputation->getWindLabSimulationThread()->start();
    return true;
}