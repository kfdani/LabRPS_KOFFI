#***************************************************************************
#*   Copyright (c) 2024 Koffi Daniel <kfdani@labrps.com>                   *
#*                                                                         *
#*   This file is part of the LabRPS development system.                   *
#*                                                                         *
#*   This program is free software; you can redistribute it and/or modify  *
#*   it under the terms of the GNU Lesser General Public License (LGPL)    *
#*   as published by the Free Software Foundation; either version 2 of     *
#*   the License, or (at your option) any later version.                   *
#*   for detail see the LICENCE text file.                                 *
#*                                                                         *
#*   LabRPS is distributed in the hope that it will be useful,             *
#*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
#*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
#*   GNU Lesser General Public License for more details.                   *
#*                                                                         *
#*   You should have received a copy of the GNU Library General Public     *
#*   License along with LabRPS; if not, write to the Free Software         *
#*   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  *
#*   USA                                                                   *
#*                                                                         *
#***************************************************************************/

__title__ = "Objects SEISMICLAB"
__author__ = "Koffi Daniel"
__url__ = "https://www.labrps.com"

## \addtogroup SEISMICLAB
#  @{

import LabRPS
import SeismicLab

# PythonFeatures from package seismiclabobjects
# standard object name == class name == type without 'SeismicLab::'

# TODO
# There are objects which use a base object. It should be tested if the base object
# is in the same document as the doc in which the obj should be created.
# Could only be happen if the make is called from Python.
# What happens ATM? Error or the obj is moved to the other doc?


# ********* simulation objects *********************************************************************
def makeSimulation(doc, name="Simulation"):
    """makeSimulation(document, [name]):
    makes a SeismicLab Simulation object"""
    obj = doc.addObject("SeismicLab::SeismicLabSimulation", name)
    return obj

def newSimulation(docName = "Doc", name="Simulation"):
    """newSimulation(docName, name):
    makes a SeismicLab Simulation object"""
    doc =  LabRPS.newDocument(docName)
    obj = doc.addObject("SeismicLab::SeismicLabSimulation", name)
    return obj

# ********* SeismicLab Features *********************************************************************
def makeFeature(featureName, simulationName,  featureTypeName, featureGroup):
    """makeFeature(featureName, simulationName,  featureTypeName, featureGroup):
    makes a SeismicLab feature and add to the specified SeismicLab simulation"""
    feat = SeismicLab.makeFeature(featureName, simulationName,  featureTypeName, featureGroup)
    return feat

##  @}
