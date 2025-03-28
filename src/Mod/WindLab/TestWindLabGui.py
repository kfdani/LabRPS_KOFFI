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

#import LabRPS, LabRPSGui, os, sys, unittest, WindLab, WindLabGui


#---------------------------------------------------------------------------
# define the test cases to test the LabRPS WindLab module
#---------------------------------------------------------------------------

import unittest
import LabRPS

from PySide import QtCore, QtGui
import LabRPSGui

# ----------------------------------------------------------------------------------
# define the functions to test the LabRPS WindLab GUI
# ----------------------------------------------------------------------------------


class WindLabGuiCases(unittest.TestCase):
    def setUp(self):
        self.doc = LabRPS.newDocument()
        self.wind = self.doc.addObject("WindLab::WindLabSimulation", "Wind")
        self.view_provider = self.wind.ViewObject

    def tearDown(self):
        pass
        # LabRPS.closeDocument(self.doc.Name)

    def injectSimpleData(self):
        """A utility function to initialize a blank wind with some known data"""
        self.doc.recompute()

    def testSomething(self):
        self.injectSimpleData()
        self.view_provider.doubleClicked()
        self.doc.recompute()
