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

#include "ViewProviderWindLabFeatureZSpectrum.h"
#include "ViewProviderWindLabFeatureZSpectrumPy.h"

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
#include <Mod/WindLabAPI/App/IrpsWLZSpectrum.h>

using namespace WindLabGui;

//**************************************************************************
// Construction/Destruction

PROPERTY_SOURCE(WindLabGui::ViewProviderWindLabFeatureZSpectrum, Gui::ViewProviderRPSFeature)


ViewProviderWindLabFeatureZSpectrum::ViewProviderWindLabFeatureZSpectrum()
{
    static const char* complexDisplayGroup = "Complex Number Display";
    static const char* complexRealImagEnum[] = {"Real", "Imaginary", nullptr};

    ADD_PROPERTY_TYPE(ComplexNumberDisplay, ((long int)0), complexDisplayGroup, App::Prop_None,"This specifies how to display the complex numbers.");

    ComplexNumberDisplay.setEnums(complexRealImagEnum);

  sPixmap = ":/icons/WindLabFeatures/WindLab_Feature_SpectrumZObj.svg";
}

ViewProviderWindLabFeatureZSpectrum::~ViewProviderWindLabFeatureZSpectrum()
{

}

bool ViewProviderWindLabFeatureZSpectrum::doubleClicked(void)
{
    ActivateFeature();
    Gui::Application::Instance->activeDocument()->setEdit(this);
    OnInitialSetting();
    return true;
}

bool ViewProviderWindLabFeatureZSpectrum::ActivateFeature()
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

    parentSim->VerticalWindSpectrumModel.setValue(obj->getNameInDocument());
    return true;
}

bool ViewProviderWindLabFeatureZSpectrum::setEdit(int ModNum)
{
    Q_UNUSED(ModNum);

    if (ModNum == ViewProvider::Default) {

        return true;
    }
    else {
        return Gui::ViewProviderDocumentObject::setEdit(ModNum);
    }
}

void ViewProviderWindLabFeatureZSpectrum::unsetEdit(int ModNum)
{
    if (ModNum == ViewProvider::Default) {
        // when pressing ESC make sure to close the dialog
        Gui::Control().closeDialog();
    }
    else {
        Gui::ViewProviderDocumentObject::unsetEdit(ModNum);
    }
}

void ViewProviderWindLabFeatureZSpectrum::setupContextMenu(QMenu* menu, QObject*, const char*)
{
    // toggle command to display components
    Gui::ActionFunction* func = new Gui::ActionFunction(menu);
    QAction* cvalue = menu->addAction(QObject::tr("ComputeZCrossSpectrumValue"));
    func->trigger(cvalue, boost::bind(&ViewProviderWindLabFeatureZSpectrum::computeZCrossSpectrumValue, this));

    QAction* cvectorf = menu->addAction(QObject::tr("ComputeZCrossSpectrumVectorF"));
    func->trigger(cvectorf, boost::bind(&ViewProviderWindLabFeatureZSpectrum::computeZCrossSpectrumVectorF, this));

    QAction* cvectort = menu->addAction(QObject::tr("ComputeZCrossSpectrumVectorT"));
    func->trigger(cvectort, boost::bind(&ViewProviderWindLabFeatureZSpectrum::computeZCrossSpectrumVectorT, this));

    QAction* cmatrix = menu->addAction(QObject::tr("ComputeZCrossSpectrumMatrixPP"));
    func->trigger(cmatrix, boost::bind(&ViewProviderWindLabFeatureZSpectrum::computeZCrossSpectrumMatrixPP, this));

    QAction* autoValue = menu->addAction(QObject::tr("ComputeZAutoSpectrumValue"));
    func->trigger(autoValue, boost::bind(&ViewProviderWindLabFeatureZSpectrum::computeZAutoSpectrumValue, this));

    QAction* autoF = menu->addAction(QObject::tr("ComputeZAutoSpectrumVectorF"));
    func->trigger(autoF, boost::bind(&ViewProviderWindLabFeatureZSpectrum::computeZAutoSpectrumVectorF, this));

    QAction* autoT = menu->addAction(QObject::tr("ComputeZAutoSpectrumVectorT"));
    func->trigger(autoT, boost::bind(&ViewProviderWindLabFeatureZSpectrum::computeZAutoSpectrumVectorT, this));

    QAction* init = menu->addAction(QObject::tr("Setup Feature"));
    func->trigger(init, boost::bind(&ViewProviderWindLabFeatureZSpectrum::OnInitialSetting, this));

    QAction* act = menu->addAction(QObject::tr("Activate Feature"));
    func->trigger(act, boost::bind(&ViewProviderWindLabFeatureZSpectrum::ActivateFeature, this));

    QAction* gtp = menu->addAction(QObject::tr("Go to Publication"));
    func->trigger(gtp, boost::bind(&ViewProviderWindLabFeatureZSpectrum::goToPublication, this));
}

bool ViewProviderWindLabFeatureZSpectrum::computeZCrossSpectrumValue()
{


    return runFeatureMethod(WindLab::WindLabUtils::ComputeZCrossSpectrumValue, this->ComplexNumberDisplay.getValueAsString());
    
}

bool ViewProviderWindLabFeatureZSpectrum::computeZCrossSpectrumVectorT()
{
    
    return runFeatureMethod(WindLab::WindLabUtils::ComputeZCrossSpectrumVectorT, this->ComplexNumberDisplay.getValueAsString());
    
}

bool ViewProviderWindLabFeatureZSpectrum::computeZCrossSpectrumVectorF()
{
    
    return runFeatureMethod(WindLab::WindLabUtils::ComputeZCrossSpectrumVectorF, this->ComplexNumberDisplay.getValueAsString());
    
}

bool ViewProviderWindLabFeatureZSpectrum::computeZCrossSpectrumMatrixPP()
{
    
    return runFeatureMethod(WindLab::WindLabUtils::ComputeZCrossSpectrumMatrixPP, this->ComplexNumberDisplay.getValueAsString());
    
}

bool ViewProviderWindLabFeatureZSpectrum::computeZAutoSpectrumValue()
{
    
    return runFeatureMethod(WindLab::WindLabUtils::ComputeZAutoSpectrumValue);
    
}
bool ViewProviderWindLabFeatureZSpectrum::computeZAutoSpectrumVectorF()
{
    
    return runFeatureMethod(WindLab::WindLabUtils::ComputeZAutoSpectrumVectorF);
    
}
bool ViewProviderWindLabFeatureZSpectrum::computeZAutoSpectrumVectorT()
{
    
    return runFeatureMethod(WindLab::WindLabUtils::ComputeZAutoSpectrumVectorT);
    
}

bool ViewProviderWindLabFeatureZSpectrum::OnInitialSetting()
{
ActivateFeature();
    WindLab::WindLabSimulation* sim = static_cast<WindLab::WindLabSimulation*>(WindLabGui::WindLabSimulationObserver::instance()->active());
    if (!sim) {Base::Console().Warning("No valide active simulation found.\n");return false;}

    sim->windLabFeatureInitalSetting(WindLab::WindLabUtils::objGroupVerticalWindSpectrum, QString::fromUtf8(sim->getSimulationData()->verticalWindSpectrumModel.getValue()));
     
    return true;
}

PyObject* ViewProviderWindLabFeatureZSpectrum::getPyObject(void)
{
    if (PythonObject.is(Py::_None())) {
        // ref counter is set to 1
        PythonObject = Py::Object(new ViewProviderWindLabFeatureZSpectrumPy(this), true);
    }
    return Py::new_reference_to(PythonObject);
}

bool ViewProviderWindLabFeatureZSpectrum::runFeatureMethod(const QString function, const char* complexNumberDisplay)
{  
ActivateFeature();
    WindLab::WindLabSimulation* sim = static_cast<WindLab::WindLabSimulation*>(WindLabGui::WindLabSimulationObserver::instance()->active());
    if (!sim) {Base::Console().Warning("No valide active simulation found.\n");return false;}

        WindLabGui::ViewProviderWindLabSimulation* vp = dynamic_cast<WindLabGui::ViewProviderWindLabSimulation*>(Gui::Application::Instance->getViewProvider(sim));
    if (vp)
    {
        auto computation = vp->getAllComputation();
        if (computation)
        {
            auto worker = vp->getAllComputation()->GetWindLabSimulationWorker();
            if (worker) {
                if (!vp->getAllComputation()->GetWindLabSimulationWorker()->isStopped()) {
                    Base::Console().Error("A simulation is running, please stop it first.\n");
                    return false;
                }
            }
        }
    }
    vp->setAllComputation(new WindLabAllFeaturesComputation(sim));
    vp->getAllComputation()->startSimulationWorker(function, complexNumberDisplay);
    vp->getAllComputation()->getWindLabSimulationThread()->start();
    return true;
}