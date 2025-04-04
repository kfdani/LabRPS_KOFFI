
#ifndef SEALAB_PLUGIN_RPSISSC_PSD_H
#define SEALAB_PLUGIN_RPSISSC_PSD_H

#include <Mod/SeaLabAPI/App/IrpsSeLFrequencySpectrum.h>
#include <App/PropertyUnits.h>


namespace SeaLab {

class CRPSISSCSpectrum : public SeaLabAPI::IrpsSeLFrequencySpectrum
{
    PROPERTY_HEADER_WITH_OVERRIDE(SeaLab::CRPSISSCSpectrum);
public:
	CRPSISSCSpectrum();
	
	~CRPSISSCSpectrum() {};

    bool ComputeCrossFrequencySpectrumValue(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dFrequency, const double &dTime, std::complex<double> &dValue);

    bool ComputeCrossFrequencySpectrumVectorF(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dTime, vec &dVarVector, cx_vec &dValVector);

    bool ComputeCrossFrequencySpectrumVectorT(const SeaLabAPI::SeaLabSimulationData& Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dFrequency, vec &dVarVector, cx_vec &dValVector);

    bool ComputeCrossFrequencySpectrumMatrixPP(const SeaLabAPI::SeaLabSimulationData &Data, const double &dFrequency, const double &dTime, cx_mat &psdMatrix);

    bool ComputeAutoFrequencySpectrumValue(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &location, const double &dFrequency, const double &dTime, double &dValue);
    
    bool ComputeAutoFrequencySpectrumVectorF(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &location, const double &dTime, vec &dVarVector, vec &dValVector);

    bool ComputeAutoFrequencySpectrumVectorT(const SeaLabAPI::SeaLabSimulationData& Data, const Base::Vector3d &location, const double &dFrequency, vec &dVarVector, vec &dValVector);

    bool OnInitialSetting(const SeaLabAPI::SeaLabSimulationData& Data);

public:

   App::PropertyLength SignificantWaveHeight;
   App::PropertyFrequency PeakFrequency;
   App::PropertyFloat Constant1;
   App::PropertyFloat Constant2;
};

} // namespace App

#endif// SEALAB_PLUGIN_RPSISSC_PSD_H