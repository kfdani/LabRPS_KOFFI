#ifndef WINDLAB_PLUGIN_KRENKCOHERENCE_H
#define WINDLAB_PLUGIN_KRENKCOHERENCE_H

#include <Mod/WindLabAPI/App/IrpsWLCoherence.h>
#include <App/PropertyUnits.h>

namespace WindLabAPI { class WindLabSimuData; }

namespace WindLab {

class RPSKrenkCoherence : public WindLabAPI::IrpsWLCoherence
{
    PROPERTY_HEADER_WITH_OVERRIDE(WindLab::RPSKrenkCoherence);

public:
    RPSKrenkCoherence();

    ~RPSKrenkCoherence() {};

   bool ComputeCrossCoherenceVectorF(const WindLabAPI::WindLabSimuData &Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dTime, vec &dVarVector, cx_vec &dValVector) ;

   bool ComputeCrossCoherenceVectorT(const WindLabAPI::WindLabSimuData &Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dFrequency, vec &dVarVector, cx_vec &dValVector);

   bool ComputeCrossCoherenceMatrixPP(const WindLabAPI::WindLabSimuData &Data, const double &dFrequency, const double &dTime, cx_mat &dCoherenceMatrix);

   bool ComputeCrossCoherenceValue(const WindLabAPI::WindLabSimuData &Data, const Base::Vector3d &locationJ, const Base::Vector3d &locationK, const double &dFrequency, const double &dTime, std::complex<double> &dValue);

   bool OnInitialSetting(const WindLabAPI::WindLabSimuData& Data);

public:
   App::PropertyLength IntegralLengthScale;

};

} // namespace App

#endif // WINDLAB_PLUGIN_KRENKCOHERENCE_H