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

__title__ = "SEISMICLAB command definitions"
__author__ = "Koffi Daniel"
__url__ = "https://www.labrps.com"

## @package commands
#  \ingroup SEISMICLAB
#  \brief SEISMICLAB command definitions

import LabRPS
import LabRPSGui
from LabRPS import Qt

from .manager import CommandManager

# Python command definitions:
# for C++ command definitions see src/Mod/SeismicLab/Command.cpp
# TODO, may be even more generic class creation
# with type() and identifier instead of class for
# the commands which add new document objects.
# see https://www.python-course.eu/python3_classes_and_type.php
# Translation:
# some information in the regard of translation can be found in forum post
# https://forum.labrps.com/viewtopic.php?f=18&t=62449&p=543845#p543593


class _Simulation(CommandManager):
    "The SeismicLab_Simulation command definition"

    def __init__(self):
        super(_Simulation, self).__init__()
        self.menutext = Qt.QT_TRANSLATE_NOOP("SeismicLab_Simulation", "Simulation")
        self.pixmap = "SeismicLab_NewSimulation"
        self.accel = "W, S"
        self.tooltip = Qt.QT_TRANSLATE_NOOP(
            "SeismicLab_Simulation",
            "Creates a simulation with default simulation data"
        )
        self.is_active = "with_document"

    def Activated(self):
        LabRPS.ActiveDocument.openTransaction("Create Simulation")
        LabRPSGui.addModule("SeismicLabGui")
        LabRPSGui.addModule("SeismicLabObjects")
        LabRPSGui.doCommand("SeismicLabObjects.makeSimulation(LabRPS.ActiveDocument, 'Simulation')")
        LabRPSGui.doCommand("SeismicLabGui.setActiveSimulation(LabRPS.ActiveDocument.ActiveObject)")
        LabRPS.ActiveDocument.commitTransaction()

class _Examples(CommandManager):
    "The SeismicLab_Examples command definition"

    def __init__(self):
        super(_Examples, self).__init__()
        self.pixmap = "SeismicLab_Examples"
        self.menutext = Qt.QT_TRANSLATE_NOOP(
            "SeismicLab_Examples",
            "Open SeismicLab examples"
        )
        self.tooltip = Qt.QT_TRANSLATE_NOOP(
            "SeismicLab_Examples",
            "Open SeismicLab examples"
        )
        self.is_active = "always"

    def Activated(self):
        LabRPSGui.addModule("seismiclabexamples.examplesgui")
        LabRPSGui.doCommand("seismiclabexamples.examplesgui.show_examplegui()")


# the string in add command will be the page name on LabRPS wiki
LabRPSGui.addCommand(
    "SeismicLab_Simulation",
    _Simulation()
)

LabRPSGui.addCommand(
    "SeismicLab_Examples",
    _Examples()
)
