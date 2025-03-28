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

#include "RPSYangEtAl1997Ind.h"
#include <Mod/WindLabAPI/App/RPSWindLabFramework.h>
#include <unsupported/Eigen/FFT>
#include "Widgets/DlgYangEtAl1997Ind.h"
#include <Gui/Control.h>
#include <Base/Console.h>

#include <QMessageBox>
#include <QThread>
#include <fstream>

using namespace WindLab;
using namespace WindLabAPI;

PROPERTY_SOURCE(WindLab::CRPSYangEtAl1997Ind, WindLabAPI::WindLabFeatureSimulationMethod)

CRPSYangEtAl1997Ind::CRPSYangEtAl1997Ind()
{
    ADD_PROPERTY_TYPE(NumberOfLocation, (3), "Parameters", App::Prop_None, "The number of simulation points evenly distributed at a constant height.");
    ADD_PROPERTY_TYPE(NumberOfFrequencies, (1024), "Parameters", App::Prop_None, "The number of frequency increments.");
    ADD_PROPERTY_TYPE(LocationHeight, (60000.00), "Parameters", App::Prop_None, "The constant height at which the points are evenly distributed.");
    ADD_PROPERTY_TYPE(LocationSpacing, (5000.00), "Parameters", App::Prop_None, "The constant spacing between any two adjacent points.");
    ADD_PROPERTY_TYPE(MeanSpeed, (37196.3), "Parameters", App::Prop_None, "The constant mean wind speed at all points.");
    ADD_PROPERTY_TYPE(CoherenceDecayCoefficient, (7.0), "Parameters", App::Prop_None, "The decay coefficient in the Davenport coherence function.");
    ADD_PROPERTY_TYPE(UpperCutOffFrequency, (2.00), "Parameters", App::Prop_None, "The upper cut off frequency.");
    ADD_PROPERTY_TYPE(NumberOfTimeIncrements, (6144), "Parameters", App::Prop_None, "The number of time increments.");
    ADD_PROPERTY_TYPE(RoughnessLength, (10.0), "Parameters", App::Prop_None, "The terrain roughness length.");
    this->LinkToWiki.setValue("https://wiki.labrps.com/Plugin_WindLab");

    

}
//Initial setting
bool CRPSYangEtAl1997Ind::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    WindLabGui::DlgYangEtAl1997IndEdit* dlg = new WindLabGui::DlgYangEtAl1997IndEdit(NumberOfLocation, NumberOfFrequencies, LocationHeight, LocationSpacing, MeanSpeed, CoherenceDecayCoefficient, UpperCutOffFrequency, NumberOfTimeIncrements, RoughnessLength, Data.simulationMethod);
    Gui::Control().showDialog(dlg);
	return true;
}

    // The simulation function 
bool CRPSYangEtAl1997Ind::Simulate(const WindLabAPI::WindLabSimulationData& Data, cube &dPhenomenon)
{

    int n = NumberOfLocation.getValue();
    int N = NumberOfFrequencies.getValue();
    double timeMin = 0.00;
    int M = 2*N;
    int T = NumberOfTimeIncrements.getValue();
    double wu = UpperCutOffFrequency.getQuantityValue().getValueAs(Base::Quantity::Hertz);
    double dt = 2 * 3.14 / (2 * wu);
    double deltaomega = wu/N;
    double distance = LocationSpacing.getQuantityValue().getValueAs(Base::Quantity::Metre);
    double speed = MeanSpeed.getQuantityValue().getValueAs(Base::Quantity::MetrePerSecond);
    double height = LocationHeight.getQuantityValue().getValueAs(Base::Quantity::Metre);
    double zo = RoughnessLength.getQuantityValue().getValueAs(Base::Quantity::Metre);
    double Uo = 0.4 * speed / log(height / zo);
    double Cy = CoherenceDecayCoefficient.getValue();
    double sampleN = Data.numberOfSample.getValue();

    vec PSD1(N);
    vec Kz(N);
    cx_mat B = Eigen::MatrixXcd::Zero(n,M);
    cx_mat G = Eigen::MatrixXcd::Zero(n,M);
    mat thet(N, n);
    vec w(N);



    std::complex<double> i2(0, 1);
    Eigen::FFT<double> fft;

    for (int ss = 1; ss <= sampleN && false == Data.isInterruptionRequested.getValue(); ss++) {

    bool returnResult = CRPSWindLabFramework::GenerateRandomMatrixFP(Data, thet);
    if(!returnResult)
    {
       Base::Console().Warning("The computation of the random phase angle matrix has failed.\n");
       return false;
    }

      for (int j = 1; j <= n && false == Data.isInterruptionRequested.getValue(); j++) {
        for (int m = 1; m <= j && false == Data.isInterruptionRequested.getValue(); m++) {
            for (int l = 1; l <= N && false == Data.isInterruptionRequested.getValue(); l++) {
                w(l - 1) = (l - 1) * deltaomega + (double)m / n * deltaomega;
                PSD1(l - 1) = 200 * height * Uo * Uo / speed
                    / (pow(1 + 50 * w(l - 1) * height / speed, 5.0 / 3.0));
            }

            for (int l = 1; l <= N && false == Data.isInterruptionRequested.getValue(); l++) {
                Kz(l - 1) = exp(-2 * w(l - 1) * distance * Cy / (speed + speed));
            }

            for (int l = 1; l <= N && false == Data.isInterruptionRequested.getValue(); l++) {
                if (m == 1) {
                    B(m - 1, l - 1) = 2 * sqrt(deltaomega) * pow(PSD1(l - 1), 0.5)
                        * pow(Kz(l - 1), abs(m - j)) * exp(i2 * thet(l - 1, m - 1));
                }
                else {
                    B(m - 1, l - 1) = 2 * sqrt(deltaomega) * pow(PSD1(l - 1), 0.5)
                        * pow(Kz(l - 1), abs(m - j)) * pow((1 - pow(Kz(l - 1), 2)), 0.5)
                        * exp(i2 * thet(l - 1, m - 1));
                }
            }
        }

        for (int ii = 1; ii <= j && false == Data.isInterruptionRequested.getValue(); ii++) {
            G.row(ii - 1) = (double)(M)*fft.inv(B.row(ii - 1));
        }

        int q = 0;
        double time = 0;

        for (int p = 1; p <= T && false == Data.isInterruptionRequested.getValue(); p++) {

            q = fmod(p - 1, M);

            time = (p - 1) * dt + timeMin;

            dPhenomenon(p - 1, 0, ss - 1) = time;

            for (int k = 1; k <= j && false == Data.isInterruptionRequested.getValue(); k++) {

            dPhenomenon(p - 1, j, ss - 1) = dPhenomenon(p - 1, j, ss - 1)
                    + real(G(k - 1, q) * exp(i2 * (k * deltaomega * time / n))) * ScaleCoefficient.getValue();
            }
        }
    }


    }

return true;
}
