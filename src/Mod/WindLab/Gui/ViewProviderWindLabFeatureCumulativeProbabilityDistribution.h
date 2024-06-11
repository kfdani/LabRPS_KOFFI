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


#ifndef WINDLABGUI_VIEWPROVIDERWINDLABFEATURECPD_H
#define WINDLABGUI_VIEWPROVIDERWINDLABFEATURECPD_H

#include <Gui/ViewProviderDocumentObject.h>
#include <Libraries/Alphaplot/Table.h>
#include <Mod/WindLab/WindLabGlobal.h>
#include <Base/SmartPtrPy.h>
#include <Mod/WindLab/Gui/WindLabAllFeaturesComputation.h>
namespace WindLabGui {


class WindLabGuiExport ViewProviderWindLabFeatureCumulativeProbabilityDistribution : public Gui::ViewProviderDocumentObject
{
    PROPERTY_HEADER_WITH_OVERRIDE(WindLabGui::ViewProviderWindLabFeatureCumulativeProbabilityDistribution);

public:
    /// constructor
    ViewProviderWindLabFeatureCumulativeProbabilityDistribution();
    /// destructor
    virtual ~ViewProviderWindLabFeatureCumulativeProbabilityDistribution();

    virtual bool doubleClicked(void);
    void setupContextMenu(QMenu*, QObject*, const char*);

    bool computeCPDValue();
    bool computeCPDVectorX();
    bool OnInitialSetting();
    bool ActivateFeature();

protected:
    virtual bool setEdit(int ModNum);
    virtual void unsetEdit(int ModNum);

private:
    QPointer<Table> tableWidget;

public:
    virtual PyObject* getPyObject() override;

    /// python object of this class
protected:// attributes
    Py::SmartPtr PythonObject;

public:
    WindLabAllFeaturesComputation* windLabAllFeaturesComputation = nullptr;

public:
    bool runFeatureMethod(const QString function, const char* complexNumberDisplay = "Real");
};

} // namespace PartGui


#endif // WINDLABGUI_VIEWPROVIDERWINDLABFEATURECPD_H

