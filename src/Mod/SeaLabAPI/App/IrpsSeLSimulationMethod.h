/***************************************************************************
 *   Copyright (c) 2024 Koffi Daniel <kfdani@labrps.com>         *
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

#ifndef IRPSSELSIMUMETHOD_H
#define IRPSSELSIMUMETHOD_H

#include "SeaLabSimulationData.h"
#include <Mod/SeaLabAPI/App/SeaLabFeatureSimulationMethod.h>

namespace SeaLabAPI {

class IrpsSeLSimulationMethod : public SeaLabAPI::SeaLabFeatureSimulationMethod
{
public:
    virtual ~IrpsSeLSimulationMethod() {};
	
    virtual bool Simulate(const SeaLabSimulationData &Data, mat &dVelocityArray) = 0;

    virtual bool SimulateInLargeScaleMode(const SeaLabSimulationData &Data, QString &strFileName) = 0;

	virtual bool OnInitialSetting(const SeaLabSimulationData &Data) = 0;

};


} //namespace SeaLabAPI


#endif  // IRPSSELSIMUMETHOD_H
