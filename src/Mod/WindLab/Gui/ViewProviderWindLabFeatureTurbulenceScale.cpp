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

#include "ViewProviderWindLabFeatureTurbulenceScale.h"
#include "ViewProviderWindLabFeatureTurbulenceScalePy.h"

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
#include <Mod/WindLabAPI/App/IrpsWLTurbulenceScale.h>

using namespace WindLabGui;

//**************************************************************************
// Construction/Destruction

PROPERTY_SOURCE(WindLabGui::ViewProviderWindLabFeatureTurbulenceScale, Gui::ViewProviderRPSFeature)


ViewProviderWindLabFeatureTurbulenceScale::ViewProviderWindLabFeatureTurbulenceScale()
{
  sPixmap = ":/icons/WindLabFeatures/WindLab_Feature_TurbulenceScaleObj.svg";
}

ViewProviderWindLabFeatureTurbulenceScale::~ViewProviderWindLabFeatureTurbulenceScale()
{

}

bool ViewProviderWindLabFeatureTurbulenceScale::doubleClicked(void)
{
    ActivateFeature();
    Gui::Application::Instance->activeDocument()->setEdit(this);
    OnInitialSetting();
    return true;
}

bool ViewProviderWindLabFeatureTurbulenceScale::ActivateFeature()
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

    parentSim->TurbulenceScale.setValue(obj->getNameInDocument());
    return true;
}
bool ViewProviderWindLabFeatureTurbulenceScale::setEdit(int ModNum)
{
    Q_UNUSED(ModNum);

    if (ModNum == ViewProvider::Default) {

        return true;
    }
    else {
        return Gui::ViewProviderDocumentObject::setEdit(ModNum);
    }
}

void ViewProviderWindLabFeatureTurbulenceScale::unsetEdit(int ModNum)
{
    if (ModNum == ViewProvider::Default) {
        // when pressing ESC make sure to close the dialog
        Gui::Control().closeDialog();
    }
    else {
        Gui::ViewProviderDocumentObject::unsetEdit(ModNum);
    }
}

void ViewProviderWindLabFeatureTurbulenceScale::setupContextMenu(QMenu* menu, QObject*, const char*)
{
    // toggle command to display components
    Gui::ActionFunction* func = new Gui::ActionFunction(menu);

    QAction* cvalue = menu->addAction(QObject::tr("ComputeTurbulenceScaleValue"));
    func->trigger(cvalue, boost::bind(&ViewProviderWindLabFeatureTurbulenceScale::computeTurbulenceScaleValue, this));

    QAction* cvectorP = menu->addAction(QObject::tr("ComputeTurbulenceScaleVectorP"));
    func->trigger(cvectorP, boost::bind(&ViewProviderWindLabFeatureTurbulenceScale::computeTurbulenceScaleVectorP, this));

    QAction* cvector = menu->addAction(QObject::tr("ComputeTurbulenceScaleVectorT"));
    func->trigger(cvector, boost::bind(&ViewProviderWindLabFeatureTurbulenceScale::computeTurbulenceScaleVectorT, this));

    QAction* init = menu->addAction(QObject::tr("Setup Feature"));
    func->trigger(init, boost::bind(&ViewProviderWindLabFeatureTurbulenceScale::OnInitialSetting, this));

    QAction* act = menu->addAction(QObject::tr("Activate Feature"));
    func->trigger(act, boost::bind(&ViewProviderWindLabFeatureTurbulenceScale::ActivateFeature, this));

    QAction* gtp = menu->addAction(QObject::tr("Go to Publication"));
    func->trigger(gtp, boost::bind(&ViewProviderWindLabFeatureTurbulenceScale::goToPublication, this));
}


bool ViewProviderWindLabFeatureTurbulenceScale::computeTurbulenceScaleValue()
{
    
    return runFeatureMethod(WindLab::WindLabUtils::ComputeTurbulenceScaleValue);
    
}
bool ViewProviderWindLabFeatureTurbulenceScale::computeTurbulenceScaleVectorP()
{
    
    return runFeatureMethod(WindLab::WindLabUtils::ComputeTurbulenceScaleVectorP);
    
}
bool ViewProviderWindLabFeatureTurbulenceScale::computeTurbulenceScaleVectorT()
{
    
    return runFeatureMethod(WindLab::WindLabUtils::ComputeTurbulenceScaleVectorT);
    
}

bool ViewProviderWindLabFeatureTurbulenceScale::OnInitialSetting()
{
ActivateFeature();
    WindLab::WindLabSimulation* sim = static_cast<WindLab::WindLabSimulation*>(WindLabGui::WindLabSimulationObserver::instance()->active());
    if (!sim) {Base::Console().Warning("No valide active simulation found.\n");return false;}

    sim->windLabFeatureInitalSetting(WindLab::WindLabUtils::objGroupTurbulenceScale, QString::fromUtf8(sim->getSimulationData()->turbulenceScale.getValue()));
     
    return true;
}

PyObject* ViewProviderWindLabFeatureTurbulenceScale::getPyObject(void)
{
    if (PythonObject.is(Py::_None())) {
        // ref counter is set to 1
        PythonObject = Py::Object(new ViewProviderWindLabFeatureTurbulenceScalePy(this), true);
    }
    return Py::new_reference_to(PythonObject);
}

bool ViewProviderWindLabFeatureTurbulenceScale::runFeatureMethod(const QString function, const char* complexNumberDisplay)
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