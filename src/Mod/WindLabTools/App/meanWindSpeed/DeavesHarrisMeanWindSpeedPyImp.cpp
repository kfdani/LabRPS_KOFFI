/***************************************************************************
 *   Copyright (c) 2010 Jürgen Riegel <juergen.riegel@web.de>              *
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

#include <CXX/Objects.hxx>
#include "DeavesHarrisMeanWindSpeed.h"
#include <Base/VectorPy.h>


// inclusion of the generated files (generated out of DeavesHarrisMeanWindSpeedPy.xml)
#include "Mod/WindLabTools/App/meanWindSpeed/DeavesHarrisMeanWindSpeedPy.h"
#include "Mod/WindLabTools/App/meanWindSpeed/DeavesHarrisMeanWindSpeedPy.cpp"

//#include <API/WindLabAPI/RPSWindLabSimulationDataPy.h>

using namespace WindLabTools;

// returns a string which represents the object e.g. when printed in python
std::string DeavesHarrisMeanWindSpeedPy::representation(void) const
{
    return std::string("<DeavesHarrisMeanWindSpeed object>");
}

PyObject* DeavesHarrisMeanWindSpeedPy::PyMake(struct _typeobject*, PyObject*, PyObject*)// Python wrapper
{
    // create a new instance of WindPy and the Twin object
    return new DeavesHarrisMeanWindSpeedPy(new DeavesHarrisMeanWindSpeed());
}

// constructor method
int DeavesHarrisMeanWindSpeedPy::PyInit(PyObject* /*args*/, PyObject* /*kwd*/)
{
    return 0;
}

// +++ methods implementer ++++++++++++++++++++++++++++++++++++++++++++++++

PyObject* DeavesHarrisMeanWindSpeedPy::computeMeanWindSpeed(PyObject *args)
{
        double height;
        double terrainRoughness;
        double shearVelocity;
        double zeroPlanDisplacement;
        double latitude;
        double earthAngularVelocity = 0.0000729;
        double beta = 0;

    if (!PyArg_ParseTuple(args, "ddddd|dd", &height, &terrainRoughness, &shearVelocity,
                                                  &zeroPlanDisplacement, &latitude, &earthAngularVelocity, &beta))
    return nullptr;
    return Py::new_reference_to(Py::Float(getDeavesHarrisMeanWindSpeedPtr()->computeMeanWindSpeed(height, terrainRoughness, shearVelocity,
                                                  zeroPlanDisplacement, latitude, earthAngularVelocity, beta)));
}


// +++ custom attributes implementer ++++++++++++++++++++++++++++++++++++++++


PyObject *DeavesHarrisMeanWindSpeedPy::getCustomAttributes(const char* /*attr*/) const
{
    return nullptr;
}

int DeavesHarrisMeanWindSpeedPy::setCustomAttributes(const char* /*attr*/, PyObject* /*obj*/)
{
    return 0;
}
