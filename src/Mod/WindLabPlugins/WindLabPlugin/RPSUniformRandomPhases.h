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


#ifndef WINDLAB_PLUGIN_UNIFORMRANDOMNESS_H
#define WINDLAB_PLUGIN_UNIFORMRANDOMNESS_H

#include <Mod/WindLabAPI/App/IrpsWLRandomness.h>

namespace WindLabAPI { class WindLabSimuData; }

class QStringList;

namespace WindLab {

class CRPSUniformRandomPhases : public WindLabAPI::IrpsWLRandomness
{
    PROPERTY_HEADER_WITH_OVERRIDE(WindLab::CRPSUniformRandomPhases);

public:

	CRPSUniformRandomPhases();

	~CRPSUniformRandomPhases() {};

	bool GenerateRandomMatrixFP(const WindLabAPI::WindLabSimuData& Data, mat &dRandomValueArray);

	bool OnInitialSetting(const WindLabAPI::WindLabSimuData& Data);

	bool ComputeRandomValue(const WindLabAPI::WindLabSimuData& Data, double &dValue);

public:
    App::PropertyFloat MinimumValue;
    App::PropertyFloat MaximumValue;
};

}

#endif  // WINDLAB_PLUGIN_UNIFORMRANDOMNESS_H