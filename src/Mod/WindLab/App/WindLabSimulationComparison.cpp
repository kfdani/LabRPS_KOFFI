/***************************************************************************
 *   Copyright (c) 2013 Jürgen Riegel <LabRPS@juergen-riegel.net>         *
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
#include "WindLabSimulationComparison.h"
#include <Mod/WindLab/App/WindLabSimulationComparisonPy.h>

using namespace App;
using namespace WindLab;

PROPERTY_SOURCE(WindLab::WindLabSimulationComparison, App::DocumentObject)

WindLabSimulationComparison::WindLabSimulationComparison()
{
    static const char* group1 = "Basics";
    ADD_PROPERTY_TYPE(ComparisonGroup, (""), group1, Prop_ReadOnly, "The of feature to be compared.");
    ADD_PROPERTY_TYPE(ComparisonMethod, (""), group1, Prop_ReadOnly, "The method to be compared.");
    static const char* group2 = "Candidate 1";
    ADD_PROPERTY_TYPE(SimulationCandidate1, (""), group2, Prop_ReadOnly, "The name of the simulation that the feature candidate 1 belongs to.");
    ADD_PROPERTY_TYPE(FeatureTypeCandidate1, (""), group2, Prop_ReadOnly, "The type of feature candidate 1 (plugged Type).");
    ADD_PROPERTY_TYPE(FeatureCandidate1, (""), group2, Prop_ReadOnly, "The name of the feature candidate 1.");

    static const char* group3 = "Candidate 2";
    ADD_PROPERTY_TYPE(SimulationCandidate2, (""), group3, Prop_ReadOnly, "The name of the simulation that the feature candidate 2 belongs to.");
    ADD_PROPERTY_TYPE(FeatureTypeCandidate2, (""), group3, Prop_ReadOnly, "The type of feature candidate 2 (plugged Type).");
    ADD_PROPERTY_TYPE(FeatureCandidate2, (""), group3, Prop_ReadOnly, "The name of the feature candidate 2.");

    static const char* group4 = "Result";
    ADD_PROPERTY_TYPE(Accuracy, (0), group4, App::Prop_ReadOnly,"The difference between the computed valued in percentage");
    ADD_PROPERTY_TYPE(ComputationTime1, (0), group4, App::Prop_ReadOnly, "The computation time for candidate 1");
    ADD_PROPERTY_TYPE(ComputationTime2, (0), group4, App::Prop_ReadOnly, "The computation time for candidate 2");

    ADD_PROPERTY_TYPE(MemoryUsage, (0), group4, App::Prop_ReadOnly, "The memory usage");
}

WindLabSimulationComparison::~WindLabSimulationComparison() { }

void WindLabSimulationComparison::start()
{

}
void WindLabSimulationComparison::stop()
{

}

bool WindLabSimulationComparison::doubleClicked(void)
{
    /*return editSimulation();*/
    return true;
}

void WindLabSimulationComparison::onChanged(const App::Property* prop)
{
    if (prop == &ComparisonGroup)
    {

    }
    else if (prop == &SimulationCandidate1)
    {

    }
    else if (prop == &FeatureTypeCandidate1) 
    {

    }
    else if (prop == &SimulationCandidate2) 
    {

    }
    else if (prop == &FeatureTypeCandidate2) 
    {
    
    }

}

PyObject* WindLabSimulationComparison::getPyObject(void)
{
    if (PythonObject.is(Py::_None())) {
        // ref counter is set to 1
        PythonObject = Py::Object(new WindLabSimulationComparisonPy(this), true);
    }
    return Py::new_reference_to(PythonObject);
}