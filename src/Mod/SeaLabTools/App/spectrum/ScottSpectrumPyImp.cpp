/***************************************************************************
 *   Copyright (c) 2010 Jürgen Riegel <juergen.riegel@web.de>              *
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

#include "PreCompiled.h"

#include <CXX/Objects.hxx>
#include "ScottSpectrum.h"
#include <Base/VectorPy.h>


// inclusion of the generated files (generated out of ScottSpectrumPy.xml)
#include "Mod/SeaLabTools/App/spectrum/ScottSpectrumPy.h"
#include "Mod/SeaLabTools/App/spectrum/ScottSpectrumPy.cpp"

//#include <API/SeaLabAPI/RPSSeaLabsimuDataPy.h>

using namespace SeaLabTools;

// returns a string which represents the object e.g. when printed in python
std::string ScottSpectrumPy::representation(void) const
{
    return std::string("<ScottSpectrum object>");
}

PyObject* ScottSpectrumPy::PyMake(struct _typeobject*, PyObject*, PyObject*)// Python wrapper
{
    // create a new instance of WindPy and the Twin object
    return new ScottSpectrumPy(new ScottSpectrum());
}

// constructor method
int ScottSpectrumPy::PyInit(PyObject* /*args*/, PyObject* /*kwd*/)
{
    return 0;
}

// +++ methods implementer ++++++++++++++++++++++++++++++++++++++++++++++++

PyObject* ScottSpectrumPy::computeSpectrum(PyObject *args)
{
    double frequency;
    double significantWaveHeight;
    double peakFrequency;

    if (!PyArg_ParseTuple(args, "ddd", &frequency, &significantWaveHeight, &peakFrequency))
    return nullptr;
    return Py::new_reference_to(Py::Float(getScottSpectrumPtr()->computeSpectrum(frequency, significantWaveHeight, peakFrequency)));

}

// +++ custom attributes implementer ++++++++++++++++++++++++++++++++++++++++


PyObject *ScottSpectrumPy::getCustomAttributes(const char* /*attr*/) const
{
    return nullptr;
}

int ScottSpectrumPy::setCustomAttributes(const char* /*attr*/, PyObject* /*obj*/)
{
    return 0;
}
