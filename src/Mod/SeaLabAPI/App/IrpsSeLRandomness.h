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

#ifndef IRPSSELRANDOMNESS_H
#define IRPSSELRANDOMNESS_H

#include "SeaLabSimulationData.h"
#include <Mod/SeaLabAPI/App/SeaLabFeatureRandomness.h>

namespace SeaLabAPI {

class IrpsSeLRandomness : public SeaLabAPI::SeaLabFeatureRandomness
{
public:
    virtual ~IrpsSeLRandomness() {};

    virtual bool ComputeRandomValue(const SeaLabSimulationData &Data, double &dValue) = 0;

    virtual bool GenerateRandomMatrixFP(const SeaLabSimulationData &Data, mat &dRandomValueArray) = 0;

    virtual bool GenerateRandomCubeFPS(const SeaLabSimulationData &Data, cube &dRandomValueCube) = 0;

	virtual bool OnInitialSetting(const SeaLabSimulationData &Data) = 0;

};

} //namespace SeaLabAPI

#endif  // IRPSSELRANDOMNESS_H
