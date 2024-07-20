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

#ifndef IRPSSELMODULATION_H
#define IRPSSELMODULATION_H

#include "SeaLabSimulationData.h"
#include <Mod/SeaLabAPI/App/SeaLabFeatureModulation.h>
#include <Base/Vector3D.h>

namespace SeaLabAPI {

class IrpsSeLModulation : public SeaLabAPI::SeaLabFeatureModulation
{
public:
    virtual ~IrpsSeLModulation() {};

    virtual bool ComputeModulationValue(const SeaLabSimulationData &Data, Base::Vector3d location, const double &dTime, double &dValue) = 0;

    virtual bool ComputeModulationVectorP(const SeaLabSimulationData &Data, const double &dTime, vec &dVarVector, vec &dValVector) = 0;

    virtual bool ComputeModulationVectorT(const SeaLabSimulationData &Data, Base::Vector3d location, vec &dVarVector, vec &dValVector) = 0;

	virtual bool OnInitialSetting(const SeaLabSimulationData &Data) = 0;

};

} //namespace SeaLabAPI


#endif  // IRPSSELMODULATION_H
