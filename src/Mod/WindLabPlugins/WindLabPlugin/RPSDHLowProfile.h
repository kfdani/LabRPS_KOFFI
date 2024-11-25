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

#ifndef RPSDHLOWPROFILE_H
#define RPSDHLOWPROFILE_H

#include <Mod/WindLabAPI/App/RPSWindLabAPI.h>
#include <App/PropertyUnits.h>

namespace WindLabAPI { class WindLabSimulationData; }

namespace WindLab {


	class CRPSDHLowProfile : public  WindLabAPI::IrpsWLMean
	{
        PROPERTY_HEADER_WITH_OVERRIDE(WindLab::CRPSDHLowProfile);

	public:
		CRPSDHLowProfile();

		~CRPSDHLowProfile() {};

    bool ComputeMeanWindSpeedValue(const WindLabAPI::WindLabSimulationData &Data, Base::Vector3d location, const double &dTime, double &dValue);

    bool ComputeMeanWindSpeedVectorP(const WindLabAPI::WindLabSimulationData &Data, const double &dTime, vec &dVarVector, vec &dValVector);

    bool ComputeMeanWindSpeedVectorT(const WindLabAPI::WindLabSimulationData &Data, Base::Vector3d location, vec &dVarVector, vec &dValVector);

	bool OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data);

	public:
		 App::PropertyLength TerrainRoughness;
		 App::PropertySpeed ShearVelocity;
         App::PropertyLength ZeroPlanDisplacement;
		 App::PropertyAngle Latitude;
		 App::PropertyFrequency EarthAngularVelocity;
		 App::PropertyFloat Betta;

	};

}

#endif  // RPSDHLOWPROFILE_H