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

#include "ViewProviderSeaLabFeatureFrequencySpectrum.h"
#include "ViewProviderSeaLabFeatureFrequencySpectrumPy.h"

#include <App/Application.h>
#include <Mod/SeaLab/App/SeaLabSimulation.h>
#include <Mod/SeaLab/Gui/DlgSeaLabFeaturesCreation.h>
#include <Gui/Control.h>
#include <Mod/SeaLab/Gui/SeaLabSimulationObserver.h>
#include <App/Document.h>
#include <Gui/ActionFunction.h>
#include "ViewProviderSeaLabSimulation.h"
#include <Gui/Application.h>
#include <Gui/Document.h>
#include <Mod/SeaLab/App/SeaLabUtils.h>
#include <Gui/AlphaPlot.h>
#include <Gui/MainWindow.h>
#include <Mod/SeaLabAPI/App/IrpsSeLFrequencySpectrum.h>

using namespace SeaLabGui;

//**************************************************************************
// Construction/Destruction

PROPERTY_SOURCE(SeaLabGui::ViewProviderSeaLabFeatureFrequencySpectrum, Gui::ViewProviderRPSFeature)


ViewProviderSeaLabFeatureFrequencySpectrum::ViewProviderSeaLabFeatureFrequencySpectrum()
{
    static const char* complexDisplayGroup = "Complex Number Display";
    static const char* complexRealImagEnum[] = {"Real", "Imaginary", nullptr};
    
    ADD_PROPERTY_TYPE(ComplexNumberDisplay, ((long int)0), complexDisplayGroup, App::Prop_None, "This specifies how to display the complex numbers.");

    ComplexNumberDisplay.setEnums(complexRealImagEnum);

  sPixmap = ":/icons/SeaLabFeatures/SeaLab_Feature_FrequencySpectrumObj.svg";
}

ViewProviderSeaLabFeatureFrequencySpectrum::~ViewProviderSeaLabFeatureFrequencySpectrum()
{

}

bool ViewProviderSeaLabFeatureFrequencySpectrum::doubleClicked(void)
{
    ActivateFeature();
    Gui::Application::Instance->activeDocument()->setEdit(this);
    OnInitialSetting();
    return true;
}

bool ViewProviderSeaLabFeatureFrequencySpectrum::ActivateFeature()
{
auto doc = App::GetApplication().getActiveDocument();
    if (!doc)
        return false;
    auto obj = doc->getObject(this->getObject()->getNameInDocument());
    if (!obj)
        return false;
    std::string parentSimName = this->getObject()->getGroup()->getNameInDocument();
    
    //get the corresponding view provider
    SeaLabGui::ViewProviderSeaLabSimulation* vp = dynamic_cast<SeaLabGui::ViewProviderSeaLabSimulation*>(Gui::Application::Instance->getViewProvider(this->getObject()->getGroup()));

    if (!vp) {
        return false;
    }

    //activate the paraent simulation
    vp->doubleClicked();

    SeaLab::SeaLabSimulation* parentSim = static_cast<SeaLab::SeaLabSimulation*>(SeaLabGui::SeaLabSimulationObserver::instance()->getSimulation(parentSimName));
    if (!parentSim) {Base::Console().Error("The parent simulation of the selected feature couldn't be actived.\n");return false;}

    bool isfeatureStationary = static_cast<SeaLabAPI::SeaLabFeature*>(obj)->IsStationary.getValue();
    if (true == parentSim->Stationarity.getValue() && false == isfeatureStationary)
    {
        Base::Console().Error("Simulation stationarity and that of the feature does not match. The feature couldn't be activated.\n");
        return false;
    }

    SeaLabGui::SeaLabSimulationObserver::instance()->setActiveSimulation(parentSim);

    parentSim->FrequencySpectrum.setValue(obj->getNameInDocument());
    return true;
}

bool ViewProviderSeaLabFeatureFrequencySpectrum::setEdit(int ModNum)
{
    Q_UNUSED(ModNum);

    if (ModNum == ViewProvider::Default) {

        return true;
    }
    else {
        return Gui::ViewProviderDocumentObject::setEdit(ModNum);
    }
}

void ViewProviderSeaLabFeatureFrequencySpectrum::unsetEdit(int ModNum)
{
    if (ModNum == ViewProvider::Default) {
        // when pressing ESC make sure to close the dialog
        Gui::Control().closeDialog();
    }
    else {
        Gui::ViewProviderDocumentObject::unsetEdit(ModNum);
    }
}

void ViewProviderSeaLabFeatureFrequencySpectrum::setupContextMenu(QMenu* menu, QObject*, const char*)
{
    // toggle command to display components
    Gui::ActionFunction* func = new Gui::ActionFunction(menu);
    QAction* cvalue = menu->addAction(QObject::tr("ComputeCrossFrequencySpectrumValue"));
    func->trigger(cvalue, boost::bind(&ViewProviderSeaLabFeatureFrequencySpectrum::ComputeCrossFrequencySpectrumValue, this));

    QAction* crossF = menu->addAction(QObject::tr("ComputeCrossFrequencySpectrumVectorF"));
    func->trigger(crossF, boost::bind(&ViewProviderSeaLabFeatureFrequencySpectrum::ComputeCrossFrequencySpectrumVectorF, this));

    QAction* crossT = menu->addAction(QObject::tr("ComputeCrossFrequencySpectrumVectorT"));
    func->trigger(crossT, boost::bind(&ViewProviderSeaLabFeatureFrequencySpectrum::ComputeCrossFrequencySpectrumVectorT, this));

    QAction* cmatrix = menu->addAction(QObject::tr("ComputeCrossFrequencySpectrumMatrixPP"));
    func->trigger(cmatrix, boost::bind(&ViewProviderSeaLabFeatureFrequencySpectrum::ComputeCrossFrequencySpectrumMatrixPP, this));
    
    QAction* autoValue = menu->addAction(QObject::tr("ComputeAutoFrequencySpectrumValue"));
    func->trigger(autoValue, boost::bind(&ViewProviderSeaLabFeatureFrequencySpectrum::ComputeAutoFrequencySpectrumValue, this));

    QAction* autoF = menu->addAction(QObject::tr("ComputeAutoFrequencySpectrumVectorF"));
    func->trigger(autoF, boost::bind(&ViewProviderSeaLabFeatureFrequencySpectrum::ComputeAutoFrequencySpectrumVectorF, this));

    QAction* autoT = menu->addAction(QObject::tr("ComputeAutoFrequencySpectrumVectorT"));
    func->trigger(autoT, boost::bind(&ViewProviderSeaLabFeatureFrequencySpectrum::ComputeAutoFrequencySpectrumVectorT, this));

    QAction* init = menu->addAction(QObject::tr("Setup Feature"));
    func->trigger(init, boost::bind(&ViewProviderSeaLabFeatureFrequencySpectrum::OnInitialSetting, this));

    QAction* act = menu->addAction(QObject::tr("Activate Feature"));
    func->trigger(act, boost::bind(&ViewProviderSeaLabFeatureFrequencySpectrum::ActivateFeature, this));

    QAction* gtp = menu->addAction(QObject::tr("Go to Publication"));
    func->trigger(gtp, boost::bind(&ViewProviderSeaLabFeatureFrequencySpectrum::goToPublication, this));

    QAction* gtw = menu->addAction(QObject::tr("Go to Wiki"));
    func->trigger(gtw, boost::bind(&ViewProviderSeaLabFeatureFrequencySpectrum::goToWiki, this));
}

bool ViewProviderSeaLabFeatureFrequencySpectrum::ComputeCrossFrequencySpectrumValue()
{ 
    return runFeatureMethod(SeaLab::SeaLabUtils::ComputeCrossFrequencySpectrumValue, this->ComplexNumberDisplay.getValueAsString());   
}
bool ViewProviderSeaLabFeatureFrequencySpectrum::ComputeCrossFrequencySpectrumVectorF()
{ 
    return runFeatureMethod(SeaLab::SeaLabUtils::ComputeCrossFrequencySpectrumVectorF, this->ComplexNumberDisplay.getValueAsString());   
}
bool ViewProviderSeaLabFeatureFrequencySpectrum::ComputeCrossFrequencySpectrumVectorT()
{ 
    return runFeatureMethod(SeaLab::SeaLabUtils::ComputeCrossFrequencySpectrumVectorT, this->ComplexNumberDisplay.getValueAsString());   
}
bool ViewProviderSeaLabFeatureFrequencySpectrum::ComputeCrossFrequencySpectrumMatrixPP()
{ 
    return runFeatureMethod(SeaLab::SeaLabUtils::ComputeCrossFrequencySpectrumMatrixPP, this->ComplexNumberDisplay.getValueAsString());   
}
bool ViewProviderSeaLabFeatureFrequencySpectrum::ComputeAutoFrequencySpectrumValue()
{
    return runFeatureMethod(SeaLab::SeaLabUtils::ComputeAutoFrequencySpectrumValue);   
}
bool ViewProviderSeaLabFeatureFrequencySpectrum::ComputeAutoFrequencySpectrumVectorF()
{
    return runFeatureMethod(SeaLab::SeaLabUtils::ComputeAutoFrequencySpectrumVectorF);   
}
bool ViewProviderSeaLabFeatureFrequencySpectrum::ComputeAutoFrequencySpectrumVectorT()
{
    return runFeatureMethod(SeaLab::SeaLabUtils::ComputeAutoFrequencySpectrumVectorT);   
}



bool ViewProviderSeaLabFeatureFrequencySpectrum::OnInitialSetting()
{
ActivateFeature();
    SeaLab::SeaLabSimulation* sim = static_cast<SeaLab::SeaLabSimulation*>(SeaLabGui::SeaLabSimulationObserver::instance()->active());
    if (!sim) {Base::Console().Warning("No valide active simulation found.\n");return false;}

    sim->seaLabFeatureInitalSetting(SeaLab::SeaLabUtils::objGroupFrequencySpectrum, QString::fromUtf8(sim->getSimulationData()->frequencySpectrum.getValue()));
     
    return true;
}

PyObject* ViewProviderSeaLabFeatureFrequencySpectrum::getPyObject(void)
{
    if (PythonObject.is(Py::_None())) {
        // ref counter is set to 1
        PythonObject = Py::Object(new ViewProviderSeaLabFeatureFrequencySpectrumPy(this), true);
    }
    return Py::new_reference_to(PythonObject);
}

bool ViewProviderSeaLabFeatureFrequencySpectrum::runFeatureMethod(const QString function, const char* complexNumberDisplay)
{  
ActivateFeature();
    SeaLab::SeaLabSimulation* sim = static_cast<SeaLab::SeaLabSimulation*>(SeaLabGui::SeaLabSimulationObserver::instance()->active());
    if (!sim) {Base::Console().Warning("No valide active simulation found.\n");return false;}

        SeaLabGui::ViewProviderSeaLabSimulation* vp = dynamic_cast<SeaLabGui::ViewProviderSeaLabSimulation*>(Gui::Application::Instance->getViewProvider(sim));
    if (sim->isRuning())
    {
        Base::Console().Error("A simulation is running, please stop it first.\n");
        return false;
    }

    vp->setAllComputation(new SeaLabAllFeaturesComputation(sim));
    vp->getAllComputation()->startSimulationWorker(function, complexNumberDisplay);
    vp->getAllComputation()->getSeaLabSimulationThread()->start();

    return true;
}