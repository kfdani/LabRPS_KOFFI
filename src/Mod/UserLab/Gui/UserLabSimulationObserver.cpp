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

#include <Base/Console.h>
#include "UserLabSimulationObserver.h"
#include <App/Simulation.h>
#include <App/Application.h>
#include <App/Document.h>
#include <Gui/Application.h>
#include <Gui/Document.h>
#include <Gui/ViewProviderDocumentObject.h>
#include <Mod/UserLab/App/UserLabSimulation.h>


using namespace UserLabGui;

UserLabSimulationObserver* UserLabSimulationObserver::_instance = nullptr;

UserLabSimulationObserver* UserLabSimulationObserver::instance()
{
    if (_instance == nullptr)
        _instance = new UserLabSimulationObserver;
    return _instance;
}

void UserLabSimulationObserver::destruct()
{
    delete _instance;
    _instance = nullptr;
}

UserLabSimulationObserver::UserLabSimulationObserver() 
   : _activeSimulation(nullptr), activeView(nullptr), activeDocument(nullptr) 
{
}

UserLabSimulationObserver::~UserLabSimulationObserver()
{
  
}

App::Simulation* UserLabSimulationObserver::createSimulation (const std::string& name, const std::string& className)
{
    App::Simulation* sim = nullptr;
    sim = static_cast<App::Simulation*>(App::GetApplication().getActiveDocument()->addObject(className.c_str(), name.c_str()));

    if (!sim) {
        std::stringstream str;
        str << "'" << className << "' not a simulation type" << std::ends;
        throw Base::TypeError(str.str());
    }

    sim->setName(name);
    _activeSimulation = sim;

    return sim;
}



App::Simulation* UserLabSimulationObserver::getSimulation(const std::string& name) const
{
    App::Document* doc = App::GetApplication().getActiveDocument();
    App::DocumentObject* obj = doc->getObject(name.c_str());
    if (!obj || !obj->getTypeId().isDerivedFrom(UserLab::UserLabSimulation::getClassTypeId())) {
        throw Base::RuntimeError("Active Simulation object have to be of type UserLab::UserLabSimulation!");
    }

    UserLab::UserLabSimulation* sim = static_cast<UserLab::UserLabSimulation*>(obj);

    return sim;
}

bool UserLabSimulationObserver::run(const std::string& name)
{
    App::Simulation* wb = getSimulation(name);
    if (wb) {
        _activeSimulation = wb;
        wb->run();
        return true;
    }

    return false;
}

bool UserLabSimulationObserver::stop(const std::string& name)
{
    App::Simulation* sim = getSimulation(name);
    if (sim) {
        _activeSimulation = sim;
        sim->stop();
        return true;
    }

    return false;
}

App::Simulation* UserLabSimulationObserver::active() const {
    return _activeSimulation;
}

bool UserLabSimulationObserver::hasActiveObject() const
{
    return _activeSimulation != nullptr;
}

void UserLabSimulationObserver::highlightActiveObject(const Gui::HighlightMode& mode, bool on)
{
    if (activeDocument && activeView)
        activeDocument->signalHighlightObject(*activeView, mode, on, 0, 0);
}

void UserLabSimulationObserver::setActiveSimulation(App::Simulation* sim)
{
      if (sim) {
        _activeSimulation = sim;
        App::Document* doc = sim->getDocument();
        activeDocument = Gui::Application::Instance->getDocument(doc);
        activeView = static_cast<Gui::ViewProviderDocumentObject*>(activeDocument->getViewProvider(_activeSimulation));
        attachDocument(doc);
    }
    else {
        _activeSimulation = nullptr;
        activeView = nullptr;
    }
}

std::list<std::string> UserLabSimulationObserver::simulations() const
{
    std::list<std::string> sims;
    App::Document* doc = App::GetApplication().getActiveDocument();
    std::vector<App::DocumentObject*> docObjects = doc->getObjects();
    for (std::vector<App::DocumentObject*>::iterator j = docObjects.begin(); j != docObjects.end();
         ++j) {
        if ((*j) && (*j)->getTypeId().isDerivedFrom(UserLab::UserLabSimulation::getClassTypeId())) {
            sims.push_back((*j)->getNameInDocument());
        }
    }

    return sims;
}

std::vector<App::Simulation*> UserLabSimulationObserver::getAllSimulations() const
{
    std::vector<App::Simulation*> sims;
    App::Document* doc = App::GetApplication().getActiveDocument();
    std::vector<App::DocumentObject*> docObjects = doc->getObjects();
    for (std::vector<App::DocumentObject*>::iterator j = docObjects.begin(); j != docObjects.end();
         ++j) {
        if ((*j) && (*j)->getTypeId().isDerivedFrom(UserLab::UserLabSimulation::getClassTypeId())) {
            sims.push_back(static_cast<UserLab::UserLabSimulation*>(*j));
        }
    }

    return sims;
}

void UserLabSimulationObserver::slotDeletedDocument(const App::Document& Doc)
{
    App::Document* d = getDocument();
    if (d == &Doc) {
        _activeSimulation = nullptr;
        activeDocument = nullptr;
        activeView = nullptr;
        detachDocument();
    }
}

void UserLabSimulationObserver::slotDeletedObject(const App::DocumentObject& Obj)
{
    if (_activeSimulation == &Obj) {
        _activeSimulation = nullptr;
        activeView = nullptr;
    }
}
