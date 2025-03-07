/***************************************************************************
 *   Copyright (c) 2024 Koffi Daniel <kfdani@labrps.com>                   *
 *                                                                         *
 *   This file is part of the LabRPS development system.               *
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
# include <QFileInfo>
#endif

#include <App/DocumentObjectPy.h>
#include <Gui/BitmapFactory.h>
#include <Gui/Document.h>
#include <Gui/EditorView.h>
#include <Gui/MainWindow.h>
#include <Gui/TextEdit.h>
#include <Mod/SeismicLab/App/SeismicLabSimulation.h>
#include <Mod/SeismicLab/Gui/SeismicLabSimulationObserver.h>


namespace SeismicLabGui {
class Module : public Py::ExtensionModule<Module>
{
public:
    Module() : Py::ExtensionModule<Module>("SeismicLabGui")
    {
        add_varargs_method("setActiveSimulation", &Module::setActiveSimulation,
            "setActiveSimulation(SimulationObject) -- Set the simulation as active."
        );
        add_varargs_method("getActiveSimulation", &Module::getActiveSimulation,
            "getActiveSimulation() -- Returns the active simulation."
        );
        initialize("This module is the SeismicLabGui module."); // register with Python
    }

    ~Module() override {}

private:
    Py::Object invoke_method_varargs(void* method_def, const Py::Tuple& args) override
    {
        try {
            return Py::ExtensionModule<Module>::invoke_method_varargs(method_def, args);
        }
        catch (const Base::Exception& e) {
            throw Py::RuntimeError(e.what());
        }
        catch (const std::exception& e) {
            throw Py::RuntimeError(e.what());
        }
    }
    Py::Object setActiveSimulation(const Py::Tuple& args)
    {
        if (SeismicLabGui::SeismicLabSimulationObserver::instance()->hasActiveObject()) {
            SeismicLabGui::SeismicLabSimulationObserver::instance()->highlightActiveObject(Gui::HighlightMode::Blue, false);
            SeismicLabGui::SeismicLabSimulationObserver::instance()->setActiveSimulation(nullptr);
        }

        PyObject* object = nullptr;
        if (PyArg_ParseTuple(args.ptr(), "|O!", &(App::DocumentObjectPy::Type), &object) && object) {
            App::DocumentObject* obj = static_cast<App::DocumentObjectPy*>(object)->getDocumentObjectPtr();
            if (!obj || !obj->getTypeId().isDerivedFrom(SeismicLab::SeismicLabSimulation::getClassTypeId())) {
                throw Py::Exception(Base::PyExc_RPS_GeneralError, "Active Simulation object have to be of type SeismicLab::SeismicLabSimulation!");
            }

            // get the gui document of the Simulation Item
            SeismicLabGui::SeismicLabSimulationObserver::instance()->setActiveSimulation(static_cast<SeismicLab::SeismicLabSimulation*>(obj));
            SeismicLabGui::SeismicLabSimulationObserver::instance()->highlightActiveObject(Gui::HighlightMode::Blue, true);
        }

        return Py::None();
    }
    Py::Object getActiveSimulation(const Py::Tuple& args)
    {
        if (!PyArg_ParseTuple(args.ptr(), ""))
            throw Py::Exception();
        if (SeismicLabGui::SeismicLabSimulationObserver::instance()->hasActiveObject()) {
            return Py::asObject(SeismicLabGui::SeismicLabSimulationObserver::instance()->active()->getPyObject());
        }
        return Py::None();
    }

};

PyObject* initModule()
{
    return Base::Interpreter().addModule(new Module);
}

} // namespace SeismicLabGui
