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

#ifndef IRPSSELUSERDEFINEDRPSOBJECT_H
#define IRPSSELUSERDEFINEDRPSOBJECT_H

#include "SeaLabSimulationData.h"
#include <Mod/SeaLabAPI/App/SeaLabFeatureUserDefinedRPSObject.h>

namespace SeaLabAPI {

class IrpsSeLUserDefinedRPSObject : public SeaLabAPI::SeaLabFeatureUserDefinedRPSObject
{
public:

    virtual ~IrpsSeLUserDefinedRPSObject() {};

    virtual bool UserDefinedRPSObjectCompute(const SeaLabSimulationData &Data, mat &dresult) = 0;

	virtual bool OnInitialSetting(const SeaLabSimulationData &Data) = 0;
};

} //namespace SeaLabAPI


#endif  // IRPSSELUSERDEFINEDRPSOBJECT_H
