#ifndef RPSWINDLABSIMULATIONWORKER_H
#define RPSWINDLABSIMULATIONWORKER_H

#include <QMutex>
#include <QObject>
#include <Mod/WindLabAPI/App/WindLabSimuData.h>
#include <Mod/WindLab/App/WindLabSimulation.h>

namespace WindLabGui
{

class RPSWindLabSimulationWorker : public QObject 
{
    Q_OBJECT
 
public:
    RPSWindLabSimulationWorker(WindLab::WindLabSimulation* sim);
    ~RPSWindLabSimulationWorker();
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

    bool workerComputeGustFactorValue();
    bool workerComputeGustFactorVectorP();
    bool workerComputeGustFactorVectorT();

    bool workerComputeKurtosisValue();
    bool workerComputeKurtosisVectorP();
    bool workerComputeKurtosisVectorT();

    bool workerMatrixToolCompute();

    bool workerComputeMeanWindSpeedValue();
    bool workerComputeMeanWindSpeedVectorP();
    bool workerComputeMeanWindSpeedVectorT();

    bool workerComputeModulationValue();
    bool workerComputeModulationVectorP();
    bool workerComputeModulationVectorT();

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

    bool workerComputeRoughnessValue();
    bool workerComputeRoughnessVectorP();
    bool workerComputeRoughnessVectorT();

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

    bool workerComputeTurbulenceIntensityValue();
    bool workerComputeTurbulenceIntensityVectorP();
    bool workerComputeTurbulenceIntensityVectorT();

    bool workerComputeTurbulenceScaleValue();
    bool workerComputeTurbulenceScaleVectorP();
    bool workerComputeTurbulenceScaleVectorT();

    bool workerUserDefinedRPSObjectCompute();

    bool workerComputeVarianceValue();
    bool workerComputeVarianceVectorP();
    bool workerComputeVarianceVectorT();

    bool workerComputeWavePassageEffectValue();
    bool workerComputeWavePassageEffectVectorF();
    bool workerComputeWavePassageEffectVectorT();
    bool workerComputeWavePassageEffectMatrixPP();

    bool workerComputeXCrossSpectrumValue();
    bool workerComputeXCrossSpectrumVectorF();
    bool workerComputeXCrossSpectrumVectorT();
    bool workerComputeXCrossSpectrumMatrixPP();
    bool workerComputeXAutoSpectrumValue();
    bool workerComputeXAutoSpectrumVectorF();
    bool workerComputeXAutoSpectrumVectorT();

    bool workerComputeYCrossSpectrumValue();
    bool workerComputeYCrossSpectrumVectorF();
    bool workerComputeYCrossSpectrumVectorT();
    bool workerComputeYCrossSpectrumMatrixPP();
    bool workerComputeYAutoSpectrumValue();
    bool workerComputeYAutoSpectrumVectorF();
    bool workerComputeYAutoSpectrumVectorT();

    bool workerComputeZCrossSpectrumValue();
    bool workerComputeZCrossSpectrumVectorF();
    bool workerComputeZCrossSpectrumVectorT();
    bool workerComputeZCrossSpectrumMatrixPP();
    bool workerComputeZAutoSpectrumValue();
    bool workerComputeZAutoSpectrumVectorF();
    bool workerComputeZAutoSpectrumVectorT();

    void stop();

    Q_SIGNALS:
    void finished();
    void signalDisplayResultInTable(QString str, int what);
    void signalDisplayResultInMatrix(QString str, int what);
    void signalDisplayResultInTableA(QString str, int what);
    void signalDisplayResultInMatrixA(QString str, int what);
    void signalDisplayResultInTableB(QString str, int what);
    void signalDisplayResultInMatrixB(QString str, int what);

private:
    WindLab::WindLabSimulation* m_sim;
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

public:
    //QStringList getInformation();
    //WindLabAPI::WindLabSimuData* getSimulationWorkerWindLabData();
    double getSimulationTime();
    std::string getComparisonName() const;
    void setComparisonName(std::string name);
    void setComputingFunction(QString function) { m_computingFunction = function; }
    QString getComputingFunction() { return m_computingFunction; }
};

}//namespace WindLabGui


#endif // RPSWINDLABSIMULATIONWORKER_H
