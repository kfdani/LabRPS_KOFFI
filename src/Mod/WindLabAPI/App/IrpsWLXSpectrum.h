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

#ifndef IRPSWLXSPECTRUM_H
#define IRPSWLXSPECTRUM_H

#include "WindLabSimuData.h"
#include <Mod/WindLabAPI/App/WindLabFeatureXSpectrum.h>
#include <Base/Vector3D.h>

namespace WindLabAPI {

class IrpsWLXSpectrum : public WindLabAPI::WindLabFeatureXSpectrum
{
public:
    virtual ~IrpsWLXSpectrum() {};

    virtual bool ComputeXCrossSpectrumValue(const WindLabAPI::WindLabSimuData &Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dFrequency, const double &dTime, std::complex<double> &dValue) = 0;

    virtual bool ComputeXCrossSpectrumVectorF(const WindLabAPI::WindLabSimuData &Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dTime, vec &dVarVector, cx_vec &dValVector) = 0;

    virtual bool ComputeXCrossSpectrumVectorT(const WindLabAPI::WindLabSimuData& Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dFrequency, vec &dVarVector, cx_vec &dValVector) = 0;

    virtual bool ComputeXCrossSpectrumMatrixPP(const WindLabAPI::WindLabSimuData &Data, const double &dFrequency, const double &dTime, cx_mat &psdMatrix) = 0;

    virtual bool ComputeXAutoSpectrumValue(const WindLabAPI::WindLabSimuData &Data, const Base::Vector3d &location, const double &dFrequency, const double &dTime, double &dValue) = 0;
    
    virtual bool ComputeXAutoSpectrumVectorF(const WindLabAPI::WindLabSimuData &Data, const Base::Vector3d &location, const double &dTime, vec &dVarVector, vec &dValVector) = 0;

    virtual bool ComputeXAutoSpectrumVectorT(const WindLabAPI::WindLabSimuData& Data, const Base::Vector3d &location, const double &dFrequency, vec &dVarVector, vec &dValVector) = 0;

	virtual bool OnInitialSetting(const WindLabSimuData &Data) = 0;

};

} //namespace WindLabAPI

#endif  // IRPSWLSIMUMETHOD_H
