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

#include "PreCompiled.h"
#include "RPSSeaLabFramework.h"
#include <iostream> 
#include <math.h>
#include <fstream>				
#include <Mod/SeaLabAPI/App/RPSSeaLabAPI.h>
#include <App/Application.h>
#include <App/Document.h>

using namespace std;
using namespace SeaLabAPI;

CRPSSeaLabFramework::CRPSSeaLabFramework()
{
}

CRPSSeaLabFramework::~CRPSSeaLabFramework()
{
}

bool CRPSSeaLabFramework::ComputeCrossCorrelationVectorT(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }
   
    SeaLabAPI::IrpsSeLCorrelation* SelectedCorreObject = static_cast<SeaLabAPI::IrpsSeLCorrelation*>(doc->getObject(Data.correlationFunction.getValue()));
    

	if (NULL == SelectedCorreObject)
	{
        return false;
	}

    bool returnValue = SelectedCorreObject->ComputeCrossCorrelationVectorT(Data, locationJ, locationK, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeCrossCorrelationMatrixPP(const SeaLabAPI::SeaLabSimulationData &Data, const double &dTime, mat &dCorrelationMatrix)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }
   
    SeaLabAPI::IrpsSeLCorrelation* SelectedCorreObject = static_cast<SeaLabAPI::IrpsSeLCorrelation*>(doc->getObject(Data.correlationFunction.getValue()));
    

	if (NULL == SelectedCorreObject)
	{
        return false;
	}

    bool returnValue = SelectedCorreObject->ComputeCrossCorrelationMatrixPP(Data, dTime, dCorrelationMatrix);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeFrequenciesVectorF(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &location, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLFrequencyDistribution* SelectedFrequencyDistributionObject = static_cast<SeaLabAPI::IrpsSeLFrequencyDistribution*>(doc->getObject(Data.frequencyDistribution.getValue()));

	if (NULL == SelectedFrequencyDistributionObject)
	{
        return false;
	}

    bool returnValue = SelectedFrequencyDistributionObject->ComputeFrequenciesVectorF(Data, location, dValVector);


    return returnValue;
}

bool CRPSSeaLabFramework::ComputeFrequenciesMatrixFP(const SeaLabAPI::SeaLabSimulationData& Data, mat& dMatrix)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLFrequencyDistribution* SelectedFrequencyDistributionObject = static_cast<SeaLabAPI::IrpsSeLFrequencyDistribution*>(doc->getObject(Data.frequencyDistribution.getValue()));

	if (NULL == SelectedFrequencyDistributionObject)
	{
        return false;
	}

    bool returnValue = SelectedFrequencyDistributionObject->ComputeFrequenciesMatrixFP(Data, dMatrix);


    return returnValue;
}


bool CRPSSeaLabFramework::ComputeLocationCoordinateMatrixP3(const SeaLabAPI::SeaLabSimulationData &Data, mat &dLocCoord)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLLocationDistribution* SelectedDistrObject = static_cast<SeaLabAPI::IrpsSeLLocationDistribution*>(doc->getObject(Data.spatialDistribution.getValue()));


	if (NULL == SelectedDistrObject)
	{
        return false;
	}

    bool returnValue = SelectedDistrObject->ComputeLocationCoordinateMatrixP3(Data, dLocCoord);


    return returnValue;
}

bool CRPSSeaLabFramework::ComputeMeanAccelerationSpeedVectorP(const SeaLabAPI::SeaLabSimulationData &Data, const double &dTime, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLMeanAcceleration* SelectedMeanObject = static_cast<SeaLabAPI::IrpsSeLMeanAcceleration*>(doc->getObject(Data.meanFunction.getValue()));

	if (NULL == SelectedMeanObject)
	{
        return false;
	}

    bool returnValue = SelectedMeanObject->ComputeMeanAccelerationVectorP(Data, dTime, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeMeanAccelerationSpeedVectorT(const SeaLabAPI::SeaLabSimulationData &Data, Base::Vector3d location, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLMeanAcceleration* SelectedMeanObject = static_cast<SeaLabAPI::IrpsSeLMeanAcceleration*>(doc->getObject(Data.meanFunction.getValue()));

	if (NULL == SelectedMeanObject)
	{
        return false;
	}

    bool returnValue = SelectedMeanObject->ComputeMeanAccelerationVectorT(Data, location, dVarVector, dValVector);

    return returnValue;
}


bool CRPSSeaLabFramework::ComputeModulationValue(const SeaLabAPI::SeaLabSimulationData &Data, Base::Vector3d location, const double &dFrequency, const double &dTime, double &dValue)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLModulation* SelectedModulationObject = static_cast<SeaLabAPI::IrpsSeLModulation*>(doc->getObject(Data.modulationFunction.getValue()));

	if (NULL == SelectedModulationObject)
	{
        return false;
	}

    bool returnValue = SelectedModulationObject->ComputeModulationValue(Data, location, dFrequency, dTime, dValue);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeModulationVectorP(const SeaLabAPI::SeaLabSimulationData &Data, const double &dFrequency, const double &dTime, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLModulation* SelectedModulationObject = static_cast<SeaLabAPI::IrpsSeLModulation*>(doc->getObject(Data.modulationFunction.getValue()));

	if (NULL == SelectedModulationObject)
	{
        return false;
	}

    bool returnValue = SelectedModulationObject->ComputeModulationVectorP(Data, dFrequency, dTime, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeModulationVectorT(const SeaLabAPI::SeaLabSimulationData &Data, Base::Vector3d location, const double &dFrequency, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLModulation* SelectedModulationObject = static_cast<SeaLabAPI::IrpsSeLModulation*>(doc->getObject(Data.modulationFunction.getValue()));

	if (NULL == SelectedModulationObject)
	{
        return false;
	}

    bool returnValue = SelectedModulationObject->ComputeModulationVectorT(Data, location, dFrequency, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeModulationVectorF(const SeaLabAPI::SeaLabSimulationData &Data, Base::Vector3d location, const double &dTime, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLModulation* SelectedModulationObject = static_cast<SeaLabAPI::IrpsSeLModulation*>(doc->getObject(Data.modulationFunction.getValue()));

	if (NULL == SelectedModulationObject)
	{
        return false;
	}

    bool returnValue = SelectedModulationObject->ComputeModulationVectorF(Data, location, dTime, dVarVector, dValVector);

    return returnValue;
}



bool CRPSSeaLabFramework::ComputeDecomposedCrossSpectrumVectorF(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dTime, vec &dVarVector, cx_vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLPSDdecompositionMethod* SelectedPSDdecomMethodObject = static_cast<SeaLabAPI::IrpsSeLPSDdecompositionMethod*>(doc->getObject(Data.cpsdDecompositionMethod.getValue()));

    if (NULL == SelectedPSDdecomMethodObject)
    {
        return false;
    }

    bool returnValue = SelectedPSDdecomMethodObject->ComputeDecomposedCrossSpectrumVectorF(Data, locationJ, locationK, dTime, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeDecomposedCrossSpectrumVectorT(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dFrequency, vec &dVarVector, cx_vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLPSDdecompositionMethod* SelectedPSDdecomMethodObject = static_cast<SeaLabAPI::IrpsSeLPSDdecompositionMethod*>(doc->getObject(Data.cpsdDecompositionMethod.getValue()));

    if (NULL == SelectedPSDdecomMethodObject)
    {
        return false;
    }

    bool returnValue = SelectedPSDdecomMethodObject->ComputeDecomposedCrossSpectrumVectorT(Data, locationJ, locationK, dFrequency, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeDecomposedCrossSpectrumMatrixPP(const SeaLabAPI::SeaLabSimulationData &Data, const double &dFrequency, const double &dTime, cx_mat &dPSDMatrix)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLPSDdecompositionMethod* SelectedPSDdecomMethodObject = static_cast<SeaLabAPI::IrpsSeLPSDdecompositionMethod*>(doc->getObject(Data.cpsdDecompositionMethod.getValue()));

	if (NULL == SelectedPSDdecomMethodObject)
	{
        return false;
	}

    bool returnValue = SelectedPSDdecomMethodObject->ComputeDecomposedCrossSpectrumMatrixPP(Data, dFrequency, dTime, dPSDMatrix);

    return returnValue;
}

bool CRPSSeaLabFramework::GenerateRandomMatrixFP(const SeaLabAPI::SeaLabSimulationData &Data, mat &dRandomValueArray)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLRandomness* SelectedRandomnessObject = static_cast<SeaLabAPI::IrpsSeLRandomness*>(doc->getObject(Data.randomnessProvider.getValue()));

	if (NULL == SelectedRandomnessObject)
	{
        return false;
	}

    bool returnValue = SelectedRandomnessObject->GenerateRandomMatrixFP(Data, dRandomValueArray);

    return returnValue;
}

// frequency spectrum
bool CRPSSeaLabFramework::ComputeCrossFrequencySpectrumValue(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dFrequency, const double &dTime, std::complex<double> &dValue)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLFrequencySpectrum* SelectedSpectrumObject = static_cast<SeaLabAPI::IrpsSeLFrequencySpectrum*>(doc->getObject(Data.frequencySpectrum.getValue()));

	if (NULL == SelectedSpectrumObject)
	{
        return false;
	}

    bool returnValue = SelectedSpectrumObject->ComputeCrossFrequencySpectrumValue(Data, locationJ, locationK, dFrequency, dTime, dValue);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeCrossFrequencySpectrumVectorF(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dTime, vec &dVarVector, cx_vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLFrequencySpectrum* SelectedSpectrumObject = static_cast<SeaLabAPI::IrpsSeLFrequencySpectrum*>(doc->getObject(Data.frequencySpectrum.getValue()));

	if (NULL == SelectedSpectrumObject)
	{
        return false;
	}

    bool returnValue = SelectedSpectrumObject->ComputeCrossFrequencySpectrumVectorF(Data, locationJ, locationK, dTime, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeCrossFrequencySpectrumVectorT(const SeaLabAPI::SeaLabSimulationData& Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dFrequency, vec &dVarVector, cx_vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLFrequencySpectrum* SelectedSpectrumObject = static_cast<SeaLabAPI::IrpsSeLFrequencySpectrum*>(doc->getObject(Data.frequencySpectrum.getValue()));

	if (NULL == SelectedSpectrumObject)
	{
        return false;
	}

    bool returnValue = SelectedSpectrumObject->ComputeCrossFrequencySpectrumVectorT(Data, locationJ, locationK, dFrequency, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeCrossFrequencySpectrumMatrixPP(const SeaLabAPI::SeaLabSimulationData &Data, const double &dFrequency, const double &dTime, cx_mat &psdMatrix)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLFrequencySpectrum* SelectedSpectrumObject = static_cast<SeaLabAPI::IrpsSeLFrequencySpectrum*>(doc->getObject(Data.frequencySpectrum.getValue()));

	if (NULL == SelectedSpectrumObject)
	{
        return false;
	}

    bool returnValue = SelectedSpectrumObject->ComputeCrossFrequencySpectrumMatrixPP(Data, dFrequency, dTime, psdMatrix);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeAutoFrequencySpectrumValue(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &location, const double &dFrequency, const double &dTime, double &dValue)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLFrequencySpectrum* SelectedSpectrumObject = static_cast<SeaLabAPI::IrpsSeLFrequencySpectrum*>(doc->getObject(Data.frequencySpectrum.getValue()));

	if (NULL == SelectedSpectrumObject)
	{
        return false;
	}

    bool returnValue = SelectedSpectrumObject->ComputeAutoFrequencySpectrumValue(Data, location, dFrequency, dTime, dValue);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeAutoFrequencySpectrumVectorF(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &location, const double &dTime, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLFrequencySpectrum* SelectedSpectrumObject = static_cast<SeaLabAPI::IrpsSeLFrequencySpectrum*>(doc->getObject(Data.frequencySpectrum.getValue()));

	if (NULL == SelectedSpectrumObject)
	{
        return false;
	}

    bool returnValue = SelectedSpectrumObject->ComputeAutoFrequencySpectrumVectorF(Data, location, dTime, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeAutoFrequencySpectrumVectorT(const SeaLabAPI::SeaLabSimulationData& Data, const Base::Vector3d &location, const double &dFrequency, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLFrequencySpectrum* SelectedSpectrumObject = static_cast<SeaLabAPI::IrpsSeLFrequencySpectrum*>(doc->getObject(Data.frequencySpectrum.getValue()));

	if (NULL == SelectedSpectrumObject)
	{
        return false;
	}

    bool returnValue = SelectedSpectrumObject->ComputeAutoFrequencySpectrumVectorT(Data, location, dFrequency, dVarVector, dValVector);

    return returnValue;
}

// direction spectrum
bool CRPSSeaLabFramework::ComputeCrossDirectionalSpectrumValue(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dFrequency, const double &dTime, const double &dDirection, std::complex<double> &dValue)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLDirectionalSpectrum* SelectedSpectrumObject = static_cast<SeaLabAPI::IrpsSeLDirectionalSpectrum*>(doc->getObject(Data.directionalSpectrum.getValue()));

	if (NULL == SelectedSpectrumObject)
	{
        return false;
	}

    bool returnValue = SelectedSpectrumObject->ComputeCrossDirectionalSpectrumValue(Data, locationJ, locationK, dFrequency, dTime, dDirection, dValue);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeCrossDirectionalSpectrumVectorF(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dTime, const double &dDirection, vec &dVarVector, cx_vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLDirectionalSpectrum* SelectedSpectrumObject = static_cast<SeaLabAPI::IrpsSeLDirectionalSpectrum*>(doc->getObject(Data.directionalSpectrum.getValue()));

	if (NULL == SelectedSpectrumObject)
	{
        return false;
	}

    bool returnValue = SelectedSpectrumObject->ComputeCrossDirectionalSpectrumVectorF(Data, locationJ, locationK, dTime, dDirection, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeCrossDirectionalSpectrumVectorT(const SeaLabAPI::SeaLabSimulationData& Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dFrequency, const double &dDirection, vec &dVarVector, cx_vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLDirectionalSpectrum* SelectedSpectrumObject = static_cast<SeaLabAPI::IrpsSeLDirectionalSpectrum*>(doc->getObject(Data.directionalSpectrum.getValue()));

	if (NULL == SelectedSpectrumObject)
	{
        return false;
	}

    bool returnValue = SelectedSpectrumObject->ComputeCrossDirectionalSpectrumVectorT(Data, locationJ, locationK, dFrequency, dDirection, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeCrossDirectionalSpectrumMatrixPP(const SeaLabAPI::SeaLabSimulationData &Data, const double &dFrequency, const double &dTime, const double &dDirection, cx_mat &psdMatrix)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLDirectionalSpectrum* SelectedSpectrumObject = static_cast<SeaLabAPI::IrpsSeLDirectionalSpectrum*>(doc->getObject(Data.directionalSpectrum.getValue()));

	if (NULL == SelectedSpectrumObject)
	{
        return false;
	}

    bool returnValue = SelectedSpectrumObject->ComputeCrossDirectionalSpectrumMatrixPP(Data, dFrequency, dTime, dDirection,psdMatrix);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeAutoDirectionalSpectrumValue(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &location, const double &dFrequency, const double &dTime, const double &dDirection, double &dValue)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLDirectionalSpectrum* SelectedSpectrumObject = static_cast<SeaLabAPI::IrpsSeLDirectionalSpectrum*>(doc->getObject(Data.directionalSpectrum.getValue()));

	if (NULL == SelectedSpectrumObject)
	{
        return false;
	}

    bool returnValue = SelectedSpectrumObject->ComputeAutoDirectionalSpectrumValue(Data, location, dFrequency, dTime, dDirection, dValue);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeAutoDirectionalSpectrumVectorF(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &location, const double &dTime, const double &dDirection, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLDirectionalSpectrum* SelectedSpectrumObject = static_cast<SeaLabAPI::IrpsSeLDirectionalSpectrum*>(doc->getObject(Data.directionalSpectrum.getValue()));

	if (NULL == SelectedSpectrumObject)
	{
        return false;
	}

    bool returnValue = SelectedSpectrumObject->ComputeAutoDirectionalSpectrumVectorF(Data, location, dTime, dDirection, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeAutoDirectionalSpectrumVectorT(const SeaLabAPI::SeaLabSimulationData& Data, const Base::Vector3d &location, const double &dFrequency, const double &dDirection, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLDirectionalSpectrum* SelectedSpectrumObject = static_cast<SeaLabAPI::IrpsSeLDirectionalSpectrum*>(doc->getObject(Data.directionalSpectrum.getValue()));

	if (NULL == SelectedSpectrumObject)
	{
        return false;
	}

    bool returnValue = SelectedSpectrumObject->ComputeAutoDirectionalSpectrumVectorT(Data, location, dFrequency, dDirection, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeAutoDirectionalSpectrumVectorD(const SeaLabAPI::SeaLabSimulationData& Data, const Base::Vector3d &location, const double &dFrequency, const double &dTime, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLDirectionalSpectrum* SelectedSpectrumObject = static_cast<SeaLabAPI::IrpsSeLDirectionalSpectrum*>(doc->getObject(Data.directionalSpectrum.getValue()));

	if (NULL == SelectedSpectrumObject)
	{
        return false;
	}

    bool returnValue = SelectedSpectrumObject->ComputeAutoDirectionalSpectrumVectorD(Data, location, dFrequency, dTime, dVarVector, dValVector);

    return returnValue;
}

// directional spreading function
bool CRPSSeaLabFramework::ComputeDirectionalSpreadingFunctionValue(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &location, const double &dfrequency, const double &dDirection, double &dValue)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLDirectionalSpreadingFunction* SelectedSpectrumObject = static_cast<SeaLabAPI::IrpsSeLDirectionalSpreadingFunction*>(doc->getObject(Data.directionalSpectrum.getValue()));

	if (NULL == SelectedSpectrumObject)
	{
        return false;
	}

    bool returnValue = SelectedSpectrumObject->ComputeDirectionalSpreadingFunctionValue(Data,location, dfrequency, dDirection, dValue);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeDirectionalSpreadingFunctionVectorF(const SeaLabAPI::SeaLabSimulationData& Data, const Base::Vector3d &location, const double &dDirection, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLDirectionalSpreadingFunction* SelectedSpectrumObject = static_cast<SeaLabAPI::IrpsSeLDirectionalSpreadingFunction*>(doc->getObject(Data.directionalSpectrum.getValue()));

	if (NULL == SelectedSpectrumObject)
	{
        return false;
	}

    bool returnValue = SelectedSpectrumObject->ComputeDirectionalSpreadingFunctionVectorF(Data, location, dDirection, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeDirectionalSpreadingFunctionVectorP(const SeaLabAPI::SeaLabSimulationData& Data, const double &dTime, const double &dDirection, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLDirectionalSpreadingFunction* SelectedSpectrumObject = static_cast<SeaLabAPI::IrpsSeLDirectionalSpreadingFunction*>(doc->getObject(Data.directionalSpectrum.getValue()));

	if (NULL == SelectedSpectrumObject)
	{
        return false;
	}

    bool returnValue = SelectedSpectrumObject->ComputeDirectionalSpreadingFunctionVectorP(Data, dTime, dDirection, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeDirectionalSpreadingFunctionVectorD(const SeaLabAPI::SeaLabSimulationData& Data, const Base::Vector3d &location, const double &dTime, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLDirectionalSpreadingFunction* SelectedSpectrumObject = static_cast<SeaLabAPI::IrpsSeLDirectionalSpreadingFunction*>(doc->getObject(Data.directionalSpectrum.getValue()));

	if (NULL == SelectedSpectrumObject)
	{
        return false;
	}

    bool returnValue = SelectedSpectrumObject->ComputeDirectionalSpreadingFunctionVectorD(Data, location, dTime, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeCrossCoherenceValue(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dFrequency, const double &dTime, std::complex<double> &dValue)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLCoherence* SelectedCoherenceObject = static_cast<SeaLabAPI::IrpsSeLCoherence*>(doc->getObject(Data.coherenceFunction.getValue()));

	if (NULL == SelectedCoherenceObject)
	{
        return false;
	}

    bool returnValue = SelectedCoherenceObject->ComputeCrossCoherenceValue(Data, locationJ, locationK, dFrequency, dTime, dValue);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeCrossCoherenceVectorF(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dTime, vec &dVarVector, cx_vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLCoherence* SelectedCoherenceObject = static_cast<SeaLabAPI::IrpsSeLCoherence*>(doc->getObject(Data.coherenceFunction.getValue()));

    if (NULL == SelectedCoherenceObject)
    {
        return false;
    }

    bool returnValue = SelectedCoherenceObject->ComputeCrossCoherenceVectorF(Data, locationJ, locationK, dTime, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeCrossCoherenceVectorT(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dFrequency, vec &dVarVector, cx_vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLCoherence* SelectedCoherenceObject = static_cast<SeaLabAPI::IrpsSeLCoherence*>(doc->getObject(Data.coherenceFunction.getValue()));

    if (NULL == SelectedCoherenceObject)
    {
        return false;
    }

    bool returnValue = SelectedCoherenceObject->ComputeCrossCoherenceVectorT(Data,locationJ, locationK, dFrequency, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeCrossCoherenceMatrixPP(const SeaLabAPI::SeaLabSimulationData &Data, const double &dFrequency, const double &dTime, cx_mat &dCoherenceMatrix)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLCoherence* SelectedCoherenceObject = static_cast<SeaLabAPI::IrpsSeLCoherence*>(doc->getObject(Data.coherenceFunction.getValue()));

    if (NULL == SelectedCoherenceObject)
    {
        return false;
    }

    bool returnValue = SelectedCoherenceObject->ComputeCrossCoherenceMatrixPP(Data, dFrequency, dTime, dCoherenceMatrix);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeCrossCorrelationValue(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dTime, double &dValue)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLCorrelation* SelectedCorreObject = static_cast<SeaLabAPI::IrpsSeLCorrelation*>(doc->getObject(Data.correlationFunction.getValue()));

	if (NULL == SelectedCorreObject)
	{
        return false;
	}

    bool returnValue = SelectedCorreObject->ComputeCrossCorrelationValue(Data, locationJ, locationK, dTime, dValue);

    return returnValue;
}


bool CRPSSeaLabFramework::ComputeMeanAccelerationSpeedValue(const SeaLabAPI::SeaLabSimulationData &Data, Base::Vector3d location, const double &dTime, double &dValue)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLMeanAcceleration* SelectedMeanObject = static_cast<SeaLabAPI::IrpsSeLMeanAcceleration*>(doc->getObject(Data.meanFunction.getValue()));

	if (NULL == SelectedMeanObject)
	{
        return false;
	}

    bool returnValue = SelectedMeanObject->ComputeMeanAccelerationValue(Data, location, dTime, dValue);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeRandomValue(const SeaLabAPI::SeaLabSimulationData &Data, double &dValue)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLRandomness* SelectedRandomnessObject = static_cast<SeaLabAPI::IrpsSeLRandomness*>(doc->getObject(Data.randomnessProvider.getValue()));

	if (NULL == SelectedRandomnessObject)
	{
        return false;
	}

	if (Data.comparisonMode.getValue())
	{

	}

    bool returnValue = SelectedRandomnessObject->ComputeRandomValue(Data, dValue);

    return returnValue;
}

 bool CRPSSeaLabFramework::ComputeFrequencyValue(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &location, const int &frequencyIndex, double &dValue)
 {
     auto doc = App::GetApplication().getActiveDocument();

     if (!doc)
     {
         return false;
     }

     SeaLabAPI::IrpsSeLFrequencyDistribution* SelectedFrequencyDistributionObject = static_cast<SeaLabAPI::IrpsSeLFrequencyDistribution*>(doc->getObject(Data.frequencyDistribution.getValue()));

     if (NULL == SelectedFrequencyDistributionObject)
     {
         return false;
     }

     bool returnValue = SelectedFrequencyDistributionObject->ComputeFrequencyValue(Data, location, frequencyIndex, dValue);

     return returnValue;
 }

SeaLabAPI::SeaLabFeatureDescription* CRPSSeaLabFramework::getSeaLabFeatureDescription(const std::string& name)
{
     auto doc = App::GetApplication().getActiveDocument();

     if (!doc)
     {
         return nullptr;
     }

     SeaLabAPI::SeaLabFeature* feature = static_cast<SeaLabAPI::SeaLabFeature*>(doc->getObject(name.c_str()));

     if (!feature)
     {
         return nullptr;
     }

     SeaLabAPI::SeaLabFeatureDescription* information = new SeaLabAPI::SeaLabFeatureDescription();

     information->PluginName.setValue(feature->PluginName.getValue());
     information->PublicationTitle.setValue(feature->PublicationTopic.getValue());
     information->PublicationLink.setValue(feature->LinkToPublication.getValue());
     information->PublicationAuthor.setValue(feature->PublicationAuthor.getValue());
     information->PublicationDate.setValue(feature->PublicationDate.getValue());
     information->ReleaseDate.setValue(feature->ReleaseDate.getValue());
     information->Version.setValue(feature->Version.getValue());
     information->Author.setValue(feature->Author.getValue());
     information->APIVersion.setValue(feature->APIVersion.getValue());
     information->PluginVersion.setValue(feature->PluginVersion.getValue());
     information->Stationarity.setValue(feature->IsStationary.getValue());
     information->type.setValue(feature->FeatureType.getValue());
     information->group.setValue(feature->FeatureGroup.getValue());
     information->LabRPSVersion.setValue(feature->LabrpsVersion.getValue());
     information->Description.setValue(feature->Description.getValue());
     information->IsUniformModulationFeature.setValue(feature->IsUniformModulationFeature.getValue());
     information->OutputUnitString.setValue(feature->OutputUnitString.getValue());
     information->ScaleCoefficient.setValue(feature->ScaleCoefficient.getValue());
     information->ApplicationFields.setValue(feature->ApplicationFields.getValue());
     
	return information;
}

bool CRPSSeaLabFramework::TableToolCompute(const SeaLabAPI::SeaLabSimulationData &Data, const mat &inputTable, mat &outputTable)
{
     auto doc = App::GetApplication().getActiveDocument();

     if (!doc)
     {
         return false;
     }

     SeaLabAPI::IrpsSeLTableTool* feature = static_cast<SeaLabAPI::IrpsSeLTableTool*>(doc->getObject(Data.tableTool.getValue()));

     if (!feature)
     {
         return false;
     }

     bool returnValue = feature->TableToolCompute(Data, inputTable, outputTable);

     return returnValue;
 }

bool CRPSSeaLabFramework::MatrixToolCompute(const SeaLabAPI::SeaLabSimulationData &Data, const mat &inputMatrix, mat &outputMatrix)
{
      auto doc = App::GetApplication().getActiveDocument();

     if (!doc)
     {
         return false;
     }

     SeaLabAPI::IrpsSeLMatrixTool* feature = static_cast<SeaLabAPI::IrpsSeLMatrixTool*>(doc->getObject(Data.matrixTool.getValue()));

     if (!feature)
     {
         return false;
     }

     bool returnValue = feature->MatrixToolCompute(Data, inputMatrix, outputMatrix);

     return returnValue;
}

bool CRPSSeaLabFramework::UserDefinedRPSObjectCompute(const SeaLabAPI::SeaLabSimulationData &Data, mat &dresult)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLUserDefinedRPSObject* SelectedUserDefinedRPSObject = static_cast<SeaLabAPI::IrpsSeLUserDefinedRPSObject*>(doc->getObject(Data.userDefinedRPSObject.getValue()));

	if (NULL == SelectedUserDefinedRPSObject)
	{
        return false;
	}

    bool returnValue = SelectedUserDefinedRPSObject->UserDefinedRPSObjectCompute(Data, dresult);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeCPDValue(const SeaLabAPI::SeaLabSimulationData &Data, const double &x, double &dValue)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLCumulativeProbabilityDistribution* SelectedObject = static_cast<SeaLabAPI::IrpsSeLCumulativeProbabilityDistribution*>(doc->getObject(Data.cumulativeProbabilityDistribution.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputeCPDValue(Data, x, dValue);

    return returnValue;
}
bool CRPSSeaLabFramework::ComputeCPDVectorX(const SeaLabAPI::SeaLabSimulationData &Data, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLCumulativeProbabilityDistribution* SelectedObject = static_cast<SeaLabAPI::IrpsSeLCumulativeProbabilityDistribution*>(doc->getObject(Data.cumulativeProbabilityDistribution.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputeCPDVectorX(Data, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeKurtosisValue(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &location, const double &dTime, double &dValue)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLKurtosis* SelectedObject = static_cast<SeaLabAPI::IrpsSeLKurtosis*>(doc->getObject(Data.kurtosis.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputeKurtosisValue(Data, location, dTime, dValue);

    return returnValue;
}
bool CRPSSeaLabFramework::ComputeKurtosisVectorP(const SeaLabAPI::SeaLabSimulationData &Data, const double &dTime, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLKurtosis* SelectedObject = static_cast<SeaLabAPI::IrpsSeLKurtosis*>(doc->getObject(Data.kurtosis.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputeKurtosisVectorP(Data, dTime, dVarVector, dValVector);

    return returnValue;
}
bool CRPSSeaLabFramework::ComputeKurtosisVectorT(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &location, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLKurtosis* SelectedObject = static_cast<SeaLabAPI::IrpsSeLKurtosis*>(doc->getObject(Data.kurtosis.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputeKurtosisVectorT(Data, location, dVarVector,dValVector);

    return returnValue;
}
bool CRPSSeaLabFramework::ComputePeakFactorValue(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &location, const double &dTime, double &dValue)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLPeakFactor* SelectedObject = static_cast<SeaLabAPI::IrpsSeLPeakFactor*>(doc->getObject(Data.peakFactor.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputePeakFactorValue(Data, location, dTime, dValue);

    return returnValue;
}
bool CRPSSeaLabFramework::ComputePeakFactorVectorP(const SeaLabAPI::SeaLabSimulationData &Data, const double &dTime, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLPeakFactor* SelectedObject = static_cast<SeaLabAPI::IrpsSeLPeakFactor*>(doc->getObject(Data.peakFactor.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputePeakFactorVectorP(Data, dTime, dVarVector, dValVector);

    return returnValue;
}
bool CRPSSeaLabFramework::ComputePeakFactorVectorT(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &location, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLPeakFactor* SelectedObject = static_cast<SeaLabAPI::IrpsSeLPeakFactor*>(doc->getObject(Data.peakFactor.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputePeakFactorVectorT(Data, location, dVarVector, dValVector);

    return returnValue;
}
bool CRPSSeaLabFramework::ComputePDFValue(const SeaLabAPI::SeaLabSimulationData &Data, const double &x, double &dValue)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLProbabilityDensityFunction* SelectedObject = static_cast<SeaLabAPI::IrpsSeLProbabilityDensityFunction*>(doc->getObject(Data.probabilityDensityFunction.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputePDFValue(Data, x, dValue);

    return returnValue;
}
bool CRPSSeaLabFramework::ComputePDFVectorX(const SeaLabAPI::SeaLabSimulationData &Data, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLProbabilityDensityFunction* SelectedObject = static_cast<SeaLabAPI::IrpsSeLProbabilityDensityFunction*>(doc->getObject(Data.probabilityDensityFunction.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputePDFVectorX(Data, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeShearVelocityOfFlowValue(const SeaLabAPI::SeaLabSimulationData &Data, Base::Vector3d location, const double &dTime, double &dValue)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLShearVelocityOfFlow* SelectedObject = static_cast<SeaLabAPI::IrpsSeLShearVelocityOfFlow*>(doc->getObject(Data.shearVelocityOfFlow.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputeShearVelocityOfFlowValue(Data,location, dTime, dValue);

    return returnValue;
}
bool CRPSSeaLabFramework::ComputeShearVelocityOfFlowVectorP(const SeaLabAPI::SeaLabSimulationData &Data, const double &dTime, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLShearVelocityOfFlow* SelectedObject = static_cast<SeaLabAPI::IrpsSeLShearVelocityOfFlow*>(doc->getObject(Data.shearVelocityOfFlow.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputeShearVelocityOfFlowVectorP(Data, dTime, dVarVector, dValVector);

    return returnValue;
}


bool CRPSSeaLabFramework::ComputeShearVelocityOfFlowVectorT(const SeaLabAPI::SeaLabSimulationData &Data, Base::Vector3d location, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLShearVelocityOfFlow* SelectedObject = static_cast<SeaLabAPI::IrpsSeLShearVelocityOfFlow*>(doc->getObject(Data.shearVelocityOfFlow.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputeShearVelocityOfFlowVectorT(Data, location, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeSkewnessValue(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &location, const double &dTime, double &dValue)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLSkewness* SelectedObject = static_cast<SeaLabAPI::IrpsSeLSkewness*>(doc->getObject(Data.skewness.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputeSkewnessValue(Data, location, dTime, dValue);

    return returnValue;
}


bool CRPSSeaLabFramework::ComputeSkewnessVectorP(const SeaLabAPI::SeaLabSimulationData &Data, const double &dTime, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLSkewness* SelectedObject = static_cast<SeaLabAPI::IrpsSeLSkewness*>(doc->getObject(Data.skewness.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputeSkewnessVectorP(Data, dTime, dVarVector, dValVector);

    return returnValue;
}


bool CRPSSeaLabFramework::ComputeSkewnessVectorT(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &location, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLSkewness* SelectedObject = static_cast<SeaLabAPI::IrpsSeLSkewness*>(doc->getObject(Data.skewness.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputeSkewnessVectorT(Data, location, dVarVector, dValVector);

    return returnValue;
}


bool CRPSSeaLabFramework::ComputeStandardDeviationValue(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &location, const double &dTime, double &dValue)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLStandardDeviation* SelectedObject = static_cast<SeaLabAPI::IrpsSeLStandardDeviation*>(doc->getObject(Data.standardDeviation.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputeStandardDeviationValue(Data, location, dTime, dValue);

    return returnValue;
}


bool CRPSSeaLabFramework::ComputeStandardDeviationVectorP(const SeaLabAPI::SeaLabSimulationData &Data, const double &dTime, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLStandardDeviation* SelectedObject = static_cast<SeaLabAPI::IrpsSeLStandardDeviation*>(doc->getObject(Data.standardDeviation.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputeStandardDeviationVectorP(Data, dTime, dVarVector, dValVector);

    return returnValue;
}


bool CRPSSeaLabFramework::ComputeStandardDeviationVectorT(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &location, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLStandardDeviation* SelectedObject = static_cast<SeaLabAPI::IrpsSeLStandardDeviation*>(doc->getObject(Data.standardDeviation.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputeStandardDeviationVectorT(Data, location, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeVarianceValue(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &location, const double &dTime, double &dValue)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLVariance* SelectedObject = static_cast<SeaLabAPI::IrpsSeLVariance*>(doc->getObject(Data.variance.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputeVarianceValue(Data, location, dTime, dValue);

    return returnValue;
}
bool CRPSSeaLabFramework::ComputeVarianceVectorP(const SeaLabAPI::SeaLabSimulationData &Data, const double &dTime, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLVariance* SelectedObject = static_cast<SeaLabAPI::IrpsSeLVariance*>(doc->getObject(Data.variance.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputeVarianceVectorP(Data, dTime, dVarVector, dValVector);

    return returnValue;
}
bool CRPSSeaLabFramework::ComputeVarianceVectorT(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &location, vec &dVarVector, vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLVariance* SelectedObject = static_cast<SeaLabAPI::IrpsSeLVariance*>(doc->getObject(Data.variance.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputeVarianceVectorT(Data, location, dVarVector, dValVector);

    return returnValue;
}
bool CRPSSeaLabFramework::ComputeWavePassageEffectValue(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dFrequency, const double &dTime, std::complex<double> &dValue)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLWavePassageEffect* SelectedObject = static_cast<SeaLabAPI::IrpsSeLWavePassageEffect*>(doc->getObject(Data.wavePassageEffect.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputeWavePassageEffectValue(Data, locationJ, locationK, dFrequency, dTime, dValue);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeWavePassageEffectVectorF(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dTime, vec &dVarVector, cx_vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLWavePassageEffect* SelectedObject = static_cast<SeaLabAPI::IrpsSeLWavePassageEffect*>(doc->getObject(Data.wavePassageEffect.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputeWavePassageEffectVectorF(Data, locationJ, locationK, dTime, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeWavePassageEffectVectorT(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dFrequency, vec &dVarVector, cx_vec &dValVector)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLWavePassageEffect* SelectedObject = static_cast<SeaLabAPI::IrpsSeLWavePassageEffect*>(doc->getObject(Data.wavePassageEffect.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputeWavePassageEffectVectorT(Data, locationJ, locationK, dFrequency, dVarVector, dValVector);

    return returnValue;
}

bool CRPSSeaLabFramework::ComputeWavePassageEffectMatrixPP(const SeaLabAPI::SeaLabSimulationData &Data, const double &dFrequency, const double &dTime, cx_mat &dCoherenceMatrix)
{
    auto doc = App::GetApplication().getActiveDocument();

    if (!doc)
    {
        return false;
    }

    SeaLabAPI::IrpsSeLWavePassageEffect* SelectedObject = static_cast<SeaLabAPI::IrpsSeLWavePassageEffect*>(doc->getObject(Data.wavePassageEffect.getValue()));

    if (NULL == SelectedObject)
    {
        return false;
    }

    bool returnValue = SelectedObject->ComputeWavePassageEffectMatrixPP(Data, dFrequency, dTime, dCoherenceMatrix);

    return returnValue;
}

int CRPSSeaLabFramework::getLocationIndex(const SeaLabAPI::SeaLabSimulationData &Data, Base::Vector3d locationTobeFound)
{
    mat locationMatrix(Data.numberOfSpatialPosition.getValue(), 4);

    bool returnResult = ComputeLocationCoordinateMatrixP3(Data, locationMatrix);

    if (!returnResult)
    {
        return -1;
    }

    Base::Vector3d location(0, 0, 0);

    for (int m = 0; m < locationMatrix.rows(); m++) {

        location = Base::Vector3d(locationMatrix(m, 1), locationMatrix(m, 2), locationMatrix(m, 3));

        if (location == locationTobeFound) {
            
            return m;
        }
    }

    return 0;
}

std::string CRPSSeaLabFramework::getCurrentDateTime()
{
    // Get the current time
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // Convert to tm structure
    std::tm local_time;
    localtime_s(&local_time, &now_time);// Use localtime_s for safety (Windows)

    // Format the date and time into a string using strftime
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", &local_time);

    return std::string(buffer);
}