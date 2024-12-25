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

#ifndef RPSSEISMICLABSIMULATIONWORKER_H
#define RPSSEISMICLABSIMULATIONWORKER_H

#include <QMutex>
#include <QObject>
#include <Mod/SeismicLabAPI/App/RPS.h>

namespace SeismicLab
{
class SeismicLabSimulation;
}

namespace SeismicLabGui
{

class RPSSeismicLabSimulationWorker : public QObject 
{
    Q_OBJECT
 
public:
    RPSSeismicLabSimulationWorker(SeismicLab::SeismicLabSimulation* sim);
    ~RPSSeismicLabSimulationWorker();
    bool isStopped();
    void setComputationTime();

public Q_SLOTS:
    bool workerComputeLocationCoordinateMatrixP3();

    bool workerComputeCrossCoherenceValue();
    bool workerComputeCrossCoherenceVectorF();
    bool workerComputeCrossCoherenceVectorT();
    bool workerComputeCrossCoherenceMatrixPP();

    bool workerComputeCrossCorrelationValue();
    bool workerComputeCrossCorrelationVectorT();
    bool workerComputeCrossCorrelationMatrixPP();

    bool workerComputeCPDValue();
    bool workerComputeCPDVectorX();

    bool workerComputeFrequencyValue();
    bool workerComputeFrequenciesVectorF();
    bool workerComputeFrequenciesMatrixFP();

    bool workerComputeKurtosisValue();
    bool workerComputeKurtosisVectorP();
    bool workerComputeKurtosisVectorT();

    bool workerMatrixToolCompute();

    bool workerComputeMeanAccelerationValue();
    bool workerComputeMeanAccelerationVectorP();
    bool workerComputeMeanAccelerationVectorT();

    bool workerComputeModulationValue();
    bool workerComputeModulationVectorP();
    bool workerComputeModulationVectorT();
    bool workerComputeModulationVectorF();

    bool workerComputePeakFactorValue();
    bool workerComputePeakFactorVectorP();
    bool workerComputePeakFactorVectorT();

    bool workerComputePDFValue();
    bool workerComputePDFVectorX();

    bool workerComputeDecomposedPSDValue();
    bool workerComputeDecomposedCrossSpectrumVectorF();
    bool workerComputeDecomposedCrossSpectrumVectorT();
    bool workerComputeDecomposedCrossSpectrumMatrixPP();

    bool workerComputeRandomValue();
    bool workerGenerateRandomMatrixFP();
    bool workerGenerateRandomCubeFPS();

    bool workerComputeShearVelocityOfFlowValue();
    bool workerComputeShearVelocityOfFlowVectorT();
    bool workerComputeShearVelocityOfFlowVectorP();

    bool workerSimulate();
    bool workerSimulateInLargeScaleMode();

    bool workerComputeSkewnessValue();
    bool workerComputeSkewnessVectorP();
    bool workerComputeSkewnessVectorT();

    bool workerComputeStandardDeviationValue();
    bool workerComputeStandardDeviationVectorP();
    bool workerComputeStandardDeviationVectorT();

    bool workerTableToolCompute();

    bool workerUserDefinedRPSObjectCompute();

    bool workerComputeVarianceValue();
    bool workerComputeVarianceVectorP();
    bool workerComputeVarianceVectorT();

    bool workerComputeWavePassageEffectValue();
    bool workerComputeWavePassageEffectVectorF();
    bool workerComputeWavePassageEffectVectorT();
    bool workerComputeWavePassageEffectMatrixPP();

    bool workerComputeCrossSpectrumValue();
    bool workerComputeCrossSpectrumVectorF();
    bool workerComputeCrossSpectrumVectorT();
    bool workerComputeCrossSpectrumMatrixPP();
    bool workerComputeAutoSpectrumValue();
    bool workerComputeAutoSpectrumVectorF();
    bool workerComputeAutoSpectrumVectorT();

    void stop();
    void complete();
    
    Q_SIGNALS:
    void finished();
    void signalDisplayResultInTable(QString str, int what);
    void signalDisplayResultInMatrix(QString str, int what);
    void signalDisplayResultInTableA(QString str, int what);
    void signalDisplayResultInMatrixA(QString str, int what);
    void signalDisplayResultInTableB(QString str, int what);
    void signalDisplayResultInMatrixB(QString str, int what);

private:
    SeismicLab::SeismicLabSimulation* m_sim;
    std::string featureName;

    std::string m_comparisonName;
    QMutex mutex;
    bool stopped;
    int m_simulationTime;
    QString m_computingFunction;
    std::string activeFeatureName;
 public:
    vec m_ResultVectorVar;
    vec m_ResultVectorVal;

    mat m_ResultMatrix;
    mat m_ResultMatrix2;

    cx_vec m_ResultVectorVal_cx;
    cx_mat m_ResultMatrix_cx;

    cube m_ResultCube;

public:
    double getSimulationTime();
    std::string getComparisonName() const;
    void setComparisonName(std::string name);
    void setComputingFunction(QString function) { m_computingFunction = function; }
    QString getComputingFunction() { return m_computingFunction; }
};

}//namespace SeismicLabGui


#endif // RPSSEISMICLABSIMULATIONWORKER_H
