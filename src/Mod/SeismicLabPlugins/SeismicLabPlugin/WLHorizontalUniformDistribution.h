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

#ifndef SEISMICLAB_PLUGIN_HORIZONTALUNIFSD_H
#define SEISMICLAB_PLUGIN_HORIZONTALUNIFSD_H

#include <App/PropertyGeo.h>
#include <Mod/SeismicLabAPI/App/IrpsSLLocationDistribution.h>
#include <App/PropertyUnits.h>

namespace SeismicLabAPI { class SeismicLabSimuData; }

namespace SeismicLab {

class CWLHorizontalUniformDistribution : public SeismicLabAPI::IrpsSLLocationDistribution
{
    PROPERTY_HEADER_WITH_OVERRIDE(SeismicLab::CWLHorizontalUniformDistribution);

public:
	CWLHorizontalUniformDistribution();
	
	~CWLHorizontalUniformDistribution() {};

	bool ComputeLocationCoordinateMatrixP3(const SeismicLabAPI::SeismicLabSimulationData& Data, mat &dLocCoord);

	//Initial setting
    bool OnInitialSetting(const SeismicLabAPI::SeismicLabSimulationData& Data);

public:
	// the first point the distribution will start from
    App::PropertyPosition FirstPoint;

	// the even spacing between the points
    App::PropertyLength Spacing;
};

} // namespace App

#endif // SEISMICLAB_PLUGIN_HORIZONTALUNIFSD_H
