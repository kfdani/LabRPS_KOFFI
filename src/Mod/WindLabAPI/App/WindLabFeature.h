/***************************************************************************
 *   Copyright (c) 2024 Koffi Daniel <kfdani@labrps.com>                   *
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

#ifndef WINDLAB_FEATURE_H
#define WINDLAB_FEATURE_H

#include <App/RPSFeature.h>
#include <Base/SmartPtrPy.h>
#include <Mod/WindLabAPI/WindLabAPIGlobal.h>

namespace WindLabAPI
{

/** Base class of all WindLab Features in LabRPS
 */
class WindLabAPIExport WindLabFeature : public App::RPSFeature
{
    PROPERTY_HEADER_WITH_OVERRIDE(WindLabAPI::WindLabFeature);

public:
    /// Constructor
    WindLabFeature(void);
    /// destructor
    virtual ~WindLabFeature();

    /** @name methods override feature */
    //@{
    virtual short mustExecute() const override;
    //@}

protected:
    /// recompute only this object
    virtual App::DocumentObjectExecReturn *recompute() override;
    virtual App::DocumentObjectExecReturn *execute() override;
    virtual void onChanged(const App::Property* prop) override;

    virtual PyObject* getPyObject() override;

protected:// attributes
    Py::SmartPtr PythonObject;
};

} //namespace App


#endif // WINDLAB_FEATURE_H

