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

#include "RPSAspasiaZerva.h"
#include <Base/Console.h>

#include <QMessageBox>
#include <QThread>
#include <fstream>
#include <Mod/GeneralTools/App/UniformRandomPhaseMatrixGenerator.h>
#include <Mod/SeismicLabAPI/App/RPSSeismicLabFramework.h>

const double PI = 3.14159265358979323846;

using namespace SeismicLab;
using namespace SeismicLabAPI;

PROPERTY_SOURCE(SeismicLab::CRPSAspasiaZerva, SeismicLabAPI::SeismicLabFeatureSimulationMethod)

CRPSAspasiaZerva::CRPSAspasiaZerva()
{
    ADD_PROPERTY_TYPE(StandardDeviation, (300), "Parameters", App::Prop_None, "The standard deviation of the excitation.");
    ADD_PROPERTY_TYPE(DominantFrequency, (5.0), "Parameters", App::Prop_None, "The dominant frequency of the earthquake excitation.");
    ADD_PROPERTY_TYPE(DampingRatio, (0.3), "Parameters", App::Prop_None, "The bandwidth of the earthquake excitation.");
    ADD_PROPERTY_TYPE(EnvelopValueAtNinetyPercentDuration, (0.3), "Parameters", App::Prop_None, "The value of the envelop function at 90 percent of the duration.");
    ADD_PROPERTY_TYPE(NormalizedDurationTimeAtPeak, (0.4), "Parameters", App::Prop_None, "The normalized duration time when ground motion achieves peak.");
    this->LinkToWiki.setValue("https://wiki.labrps.com/Plugin_SeismicLab_AspasiaZerva#Aspasia_Zerva_1992");

}

//Initial setting
bool CRPSAspasiaZerva::OnInitialSetting(const SeismicLabAPI::SeismicLabSimulationData& Data)
{
	return true;
}

// The simulation function 
bool CRPSAspasiaZerva::Simulate(const SeismicLabAPI::SeismicLabSimulationData& Data, cube &dPhenomenon)
{

    if (Data.stationarity.getValue() ) {
        Base::Console().Error("The simulation method is only for non-stationary ground motion.\n");
        return false;
    }

    if (!Data.uniformModulation.getValue() ) {
        Base::Console().Error("The simulation method is only for non-stationary uniformly modulated ground motion.\n");
        return false;
    }

    auto PbuInfo = SeismicLabAPI::CRPSSeismicLabFramework::getSeismicLabFeatureDescription(Data.modulationFunction.getValue());
    if (!PbuInfo) {
        Base::Console().Error(
            "Invalid active modulation function.\n");
        return false;
    }
    if (!PbuInfo->IsUniformModulationFeature.getValue()) {
        Base::Console().Error("The method accepts only uniform modulation function.\n");
        return false;
    }

    int n = Data.numberOfSpatialPosition.getValue();
    int N = Data.numberOfFrequency.getValue();
    double dt = Data.timeIncrement.getQuantityValue().getValueAs(Base::Quantity::Second);
    double timeMin = Data.minTime.getQuantityValue().getValueAs(Base::Quantity::Second);
    double deltaomega = Data.frequencyIncrement.getQuantityValue().getValueAs(Base::Quantity::Hertz);
    int T = Data.numberOfTimeIncrements.getValue();
    double sampleN = Data.numberOfSample.getValue();

    // local array for the location coordinates
    mat dLocCoord(n, 4);
    mat frequencies(N, n);
    vec freq(N);
    vec PSD(N);
    vec Env(T);
    vec EnvVar(T);

    //compute the simulation point coordinates
    bool returnResult = SeismicLabAPI::CRPSSeismicLabFramework::ComputeLocationCoordinateMatrixP3(Data, dLocCoord);

    if (!returnResult) {
        Base::Console().Warning("The computation of the location coordinates matrix has failed.\n");
        return false;
    }

    // generate n sequences of random phase angles phi(l), l = 1, 2, ..., N
    mat thet(N, n);

    if (!returnResult) {
        Base::Console().Warning("The generation of the random phase matrix has failed.\n");
        return false;
    }

    // compute le frequency matrix. Note that this method required the double indexing frequency
    returnResult = SeismicLabAPI::CRPSSeismicLabFramework::ComputeFrequenciesMatrixFP(Data, frequencies);

    if (!returnResult) {
        Base::Console().Warning("The computation of the frequency distribution has failed.\n");
        return false;
    }

    for (int ss = 1; ss <= sampleN && false == Data.isInterruptionRequested.getValue(); ss++) 
    {
        double time = 0;

        returnResult = SeismicLabAPI::CRPSSeismicLabFramework::GenerateRandomMatrixFP(Data, thet);
        if (!returnResult) {
            Base::Console().Warning("The computation of the random phases has failed.\n");
            return false;
        }

        for (int j = 1; j <= n && false == Data.isInterruptionRequested.getValue(); j++)
        {
            Base::Vector3d point(dLocCoord(j - 1, 1), dLocCoord(j - 1, 2), dLocCoord(j - 1, 3));
            returnResult = SeismicLabAPI::CRPSSeismicLabFramework::ComputeAutoSpectrumVectorF(Data, point, 0.0, freq, PSD);
            if (!returnResult)
            {
                Base::Console().Warning("The computation of the auto spectrum vector has failed.\n");
                return false;
            }

            returnResult = SeismicLabAPI::CRPSSeismicLabFramework::ComputeModulationVectorT(Data, point, 0.0, EnvVar, Env);

            if (!returnResult) {
                Base::Console().Warning(
                    "The computation of the modulation function vector has failed.\n");
                return false;
            }

            for (int p = 1; p <= T && false == Data.isInterruptionRequested.getValue(); p++) {
                time = (p - 1) * dt + timeMin;
                dPhenomenon(p - 1, 0, ss - 1) = time;
                for (int k = 1; k <= N && false == Data.isInterruptionRequested.getValue(); k++) {
                    dPhenomenon(p - 1, j, ss - 1) = dPhenomenon(p - 1, j, ss - 1)
                        + Env(p - 1) * 2 * std::sqrt(PSD(k - 1) * deltaomega)
                            * std::cos(frequencies(k - 1, j - 1) * time + thet(k - 1, j - 1))
                            * ScaleCoefficient.getValue();
                }
            }             
         }
    }
return true;
}
