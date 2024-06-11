

#include "RPSWavePassageEffect.h"
#include <Mod/WindLabAPI/App/RPSWindLabFramework.h>
#include <Mod/WindLabTools/App/wavePassageEffect/WavePassageEffect.h>
#include "Widgets/DlgWavePassageEffect.h"
#include <Base/Console.h>
#include <Gui/Control.h>


using namespace WindLab;
using namespace WindLabAPI;

PROPERTY_SOURCE(WindLab::CRPSWavePassageEffect, WindLabAPI::WindLabFeatureWavePassageEffect)

CRPSWavePassageEffect::CRPSWavePassageEffect()
{
   ADD_PROPERTY_TYPE(ApparentWaveVelocity, (10000.0), "Parameters", App::Prop_None, "The apparent wave velocity.");
   ADD_PROPERTY_TYPE(Coefficient, (5.0), "Parameters", App::Prop_None, "The appropriate coefficient that can obtained from experiments.");
}


bool CRPSWavePassageEffect::ComputeWavePassageEffectVectorF(const WindLabAPI::WindLabSimuData& Data, const Base::Vector3d& locationJ, const Base::Vector3d& locationK, const double& dTime, vec& dVarVector, cx_vec& dValVector)
{
    bool returnResult = true;
    
    returnResult = CRPSWindLabFramework::ComputeFrequenciesVectorF(Data, locationJ, dVarVector);
    if (!returnResult)
    {
        return false;
    }

    for (int loop = 0; loop < Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; loop++)
    {
        returnResult = ComputeWavePassageEffectValue(Data, locationJ, locationK, dVarVector(loop), dTime, dValVector(loop));
    }

     return returnResult;
}

bool CRPSWavePassageEffect::ComputeWavePassageEffectVectorT(const WindLabAPI::WindLabSimuData& Data, const Base::Vector3d& locationJ, const Base::Vector3d& locationK, const double& dFrequency, vec& dVarVector, cx_vec& dValVector)
{
     bool returnResult = true;

    for (int loop = 0; loop < Data.numberOfTimeIncrements.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; loop++)
    {
        dVarVector(loop) = Data.minTime.getValue() + loop * Data.timeIncrement.getValue();
        returnResult = ComputeWavePassageEffectValue(Data, locationJ, locationK, dFrequency, dVarVector(loop), dValVector(loop));
    }

     return returnResult;
}

bool CRPSWavePassageEffect::ComputeWavePassageEffectMatrixPP(const WindLabAPI::WindLabSimuData& Data, const double& dFrequency, const double& dTime, cx_mat& dCoherenceMatrix)
{
     // Local array for location coordinates
    mat dLocCoord(Data.numberOfSpatialPosition.getValue(), 4);
    
    bool returnResult = CRPSWindLabFramework::ComputeLocationCoordinateMatrixP3(Data, dLocCoord);
     if(!returnResult)
    {
       Base::Console().Warning("The computation of the location coordinates matrix has failed.") ;
       return false;
    }
    
    Base::Vector3d locationJ(0, 0, 0);
    Base::Vector3d locationK(0, 0, 0);

    for (int loop1 = 0; loop1 < Data.numberOfSpatialPosition.getValue(); loop1++)
    {
        for (int loop2 = 0; loop2 < Data.numberOfSpatialPosition.getValue(); loop2++)
        {
           locationJ = Base::Vector3d(dLocCoord(loop1, 1), dLocCoord(loop1, 2), dLocCoord(loop1, 3));
           locationK = Base::Vector3d(dLocCoord(loop2, 1), dLocCoord(loop2, 2), dLocCoord(loop2, 3));
           returnResult = ComputeWavePassageEffectValue(Data, locationJ, locationK, dFrequency, dTime, dCoherenceMatrix(loop1, loop2));
        }
    }

    return returnResult;
}

bool CRPSWavePassageEffect::ComputeWavePassageEffectValue(const WindLabAPI::WindLabSimuData& Data, const Base::Vector3d& locationJ, const Base::Vector3d& locationK, const double& dFrequency, const double& dTime, std::complex<double>& dValue)
{
    bool returnResult = true;

   double MEANj = 0.0;
   double MEANk = 0.0;
   double apparentWaveVelocity = 0.0;
   double wavePassageEff = 0.0;

    returnResult = CRPSWindLabFramework::ComputeMeanWindSpeedValue(Data, locationJ, dTime, MEANj);
   if (!returnResult) {
        Base::Console().Warning("The computation of the mean wind velocity failed.");
        return false;
   }
    returnResult = CRPSWindLabFramework::ComputeMeanWindSpeedValue(Data, locationK, dTime, MEANk);
   if (!returnResult) {
        Base::Console().Warning("The computation of the mean wind velocity failed.");
        return false;
   }

    WindLabTools::WavePassageEffect wavePassageEf;
    if (ApparentWaveVelocity.getValue() == 0.0)
    {
      apparentWaveVelocity = wavePassageEf.computeApparentWaveVelocity(MEANj, MEANk, Coefficient.getValue());
      ApparentWaveVelocity.setValue(apparentWaveVelocity);
    }
    dValue = wavePassageEf.computeWavePassageEffect(locationJ, locationK, dFrequency, ApparentWaveVelocity.getQuantityValue().getValueAs(Base::Quantity::MetrePerSecond));

    return true;
}


//Initial setting
bool CRPSWavePassageEffect::OnInitialSetting(const WindLabAPI::WindLabSimuData& Data)
{
	// the input diolag
    WindLabGui::DlgWavePassageEffectEdit* dlg = new WindLabGui::DlgWavePassageEffectEdit(ApparentWaveVelocity, Coefficient, Data.alongWindSpectrumModel);
	Gui::Control().showDialog(dlg);

    return true;
}