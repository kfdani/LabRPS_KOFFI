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

#ifndef SEALAB_PLUGIN_GRIDPOINTS_H
#define SEALAB_PLUGIN_GRIDPOINTS_H

#include <App/PropertyGeo.h>
#include <Mod/SeaLabAPI/App/IrpsSeLLocationDistribution.h>
#include <App/PropertyUnits.h>

namespace SeaLab {

class WLGridPoints : public SeaLabAPI::IrpsSeLLocationDistribution
{
    PROPERTY_HEADER_WITH_OVERRIDE(SeaLab::WLGridPoints);

public:
	WLGridPoints();
	
	~WLGridPoints() {};

	bool ComputeLocationCoordinateMatrixP3(const SeaLabAPI::SeaLabSimulationData& Data, mat &dLocCoord);

	//Initial setting
    bool OnInitialSetting(const SeaLabAPI::SeaLabSimulationData& Data);

	// the center point of the grid
    App::PropertyPosition GridStartingPoint;

};

} // namespace App

#endif // SEALAB_PLUGIN_GRIDPOINTS_H
