/***************************************************************************
 *   Copyright (c) YEAR YOUR NAME         <Your e-mail address>            *
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
#include <Base/Interpreter.h>
#include <Base/PyObjectBase.h>
#include <Gui/Application.h>
#include "RPSGeneralToolsPyTool.h"


namespace GeneralToolsGui {
class Module : public Py::ExtensionModule<Module>
{
public:
    Module() : Py::ExtensionModule<Module>("GeneralToolsGui")
    {
        initialize("This module is the GeneralToolsGui module."); // register with Python
    }

    virtual ~Module() {}

private:
};

PyObject* initModule()
{
    return Base::Interpreter().addModule(new Module);
}

} // namespace GeneralToolsGui


/* Python entry */
PyMOD_INIT_FUNC(GeneralToolsGui)
{
    if (!Gui::Application::Instance) {
        PyErr_SetString(PyExc_ImportError, "Cannot load Gui module in console application.");
        PyMOD_Return(nullptr);
    }

    PyObject* mod = GeneralToolsGui::initModule();

    Base::Console().Log("Loading GUI of GeneralToolsGui module... done\n");

    static struct PyModuleDef pGeneralToolsPyToolPyModuleDef = {
        PyModuleDef_HEAD_INIT,
        "GeneralToolsPyTool",
        "GeneralToolsPyTool", -1,
        GeneralToolsGui::RPSGeneralToolsPyTool::Methods,
        nullptr, nullptr, nullptr, nullptr};

    PyObject* pGeneralToolsPyToolPyModule = PyModule_Create(&pGeneralToolsPyToolPyModuleDef);

    Py_INCREF(pGeneralToolsPyToolPyModule);
    PyModule_AddObject(mod, "GeneralToolsPyTool", pGeneralToolsPyToolPyModule);
    PyMOD_Return(mod);
}
