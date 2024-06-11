/***************************************************************************
 *   Copyright (c) 2004 Jürgen Riegel <juergen.riegel@web.de>              *
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


#ifndef WINDLABGUI_VIEWPROVIDERWINDLABSIMULATIONCOMPARISON_H
#define WINDLABGUI_VIEWPROVIDERWINDLABSIMULATIONCOMPARISON_H

#include <Gui/ViewProviderDocumentObject.h>
#include <Libraries/Alphaplot/Table.h>
#include <Mod/WindLab/WindLabGlobal.h>
#include <Base/SmartPtrPy.h>
#include <Mod/WindLab/Gui/WindLabComparisonStarter.h>
namespace WindLabGui {


class WindLabGuiExport ViewProviderWindLabSimulationComparison : public Gui::ViewProviderDocumentObject
{
    PROPERTY_HEADER_WITH_OVERRIDE(WindLabGui::ViewProviderWindLabSimulationComparison);

public:
    /// constructor
    ViewProviderWindLabSimulationComparison();
    /// destructor
    virtual ~ViewProviderWindLabSimulationComparison();

    virtual bool doubleClicked(void);
    void setupContextMenu(QMenu*, QObject*, const char*);

    bool start();
    bool stop();

protected:
    virtual bool setEdit(int ModNum);
    virtual void unsetEdit(int ModNum);

public:
    virtual PyObject* getPyObject() override;

    /// python object of this class
protected:// attributes
    Py::SmartPtr PythonObject;

public:
    WindLabComparisonStarter* windLabComparisonStarter = nullptr;
};

} // namespace PartGui


#endif // WINDLABGUI_VIEWPROVIDERWINDLABFEATUREZSPECTRUM_H

