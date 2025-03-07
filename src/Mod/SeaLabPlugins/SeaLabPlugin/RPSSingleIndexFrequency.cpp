

#include "RPSSingleIndexFrequency.h"
#include <QMessageBox>
#include "Widgets/RPSFrequencyDistributionDialog.h"
#include <Base/Console.h>

using namespace SeaLab;

PROPERTY_SOURCE(SeaLab::CRPSSingleIndexFrequency, SeaLabAPI::SeaLabFeatureFrequencyDistribution)

CRPSSingleIndexFrequency::CRPSSingleIndexFrequency()
{
   this->LinkToWiki.setValue("https://wiki.labrps.com/Plugin_SeaLab#Single_Index_Frequency_Discretization");

}

bool CRPSSingleIndexFrequency::ComputeFrequenciesVectorF(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &location, vec &dValVector)
{
	for (int l = 0; l < Data.numberOfFrequency.getValue(); l++)
	{
        ComputeFrequencyValue(Data, location, l, dValVector(l));
	}

	return true;
}

bool CRPSSingleIndexFrequency::ComputeFrequenciesMatrixFP(const SeaLabAPI::SeaLabSimulationData &Data, mat &dMatrix)
{
    Base::Vector3d location(0, 0, 0);//unused

    for (int l = 0; l < Data.numberOfFrequency.getValue(); l++) {
        
        for (int m = 0; m < Data.numberOfSpatialPosition.getValue(); m++) {

            ComputeFrequencyValue(Data, location, l, dMatrix(l, m));
        }
    }
    return true;
}

bool CRPSSingleIndexFrequency::OnInitialSetting(const SeaLabAPI::SeaLabSimulationData& Data)
{
    // the input diolag
    std::unique_ptr<RPSFrequencyDistributionDialog> dlg(new RPSFrequencyDistributionDialog(1));

    if (dlg->exec() == QDialog::Accepted) //
    {
    }

    return true;
}

bool CRPSSingleIndexFrequency::ComputeFrequencyValue(const SeaLabAPI::SeaLabSimulationData &Data, const Base::Vector3d &location, const int &frequencyIndex, double &dValue)
{
    dValue = (Data.minFrequency.getQuantityValue().getValueAs(Base::Quantity::Hertz) + frequencyIndex * Data.frequencyIncrement.getQuantityValue().getValueAs(Base::Quantity::Hertz)) * ScaleCoefficient.getValue();
	return true;
}

//short CRPSSingleIndexFrequency::mustExecute(void) const
//{
//    return SeaLabFeature::mustExecute();
//}
//
//App::DocumentObjectExecReturn* CRPSSingleIndexFrequency::recompute(void)
//{
//    try {
//        return SeaLabAPI::SeaLabFeature::recompute();
//    }
//    catch (Base::Exception& e) {
//
//        App::DocumentObjectExecReturn* ret = new App::DocumentObjectExecReturn(e.what());
//        if (ret->Why.empty()) ret->Why = "Unknown exception";
//        return ret;
//    }
//}
//
//App::DocumentObjectExecReturn* CRPSSingleIndexFrequency::execute(void)
//{
//    return SeaLabFeature::execute();
//}
//
//void CRPSSingleIndexFrequency::onChanged(const App::Property* prop)
//{
//
//    SeaLabFeature::onChanged(prop);
//}