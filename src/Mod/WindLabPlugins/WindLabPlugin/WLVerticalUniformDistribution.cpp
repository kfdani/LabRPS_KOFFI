#include "PreCompiled.h"
#include "WLVerticalUniformDistribution.h"
#include "widgets/DlgVerticalLocationDistribution.h"
#include <Gui/Control.h>
#include <QMessageBox>


using namespace WindLab;

PROPERTY_SOURCE(WindLab::CWLVerticalUniformDistribution, WindLabAPI::WindLabFeatureLocationDistribution)

App::PropertyQuantityConstraint::Constraints CWLVerticalUniformDistribution::SpacingContraint = {0.0, DBL_MAX, 1.0};


CWLVerticalUniformDistribution::CWLVerticalUniformDistribution()
{
    static const char* locationgroup = "Distribution";

    ADD_PROPERTY_TYPE(LowestPoint, (Base::Vector3d(0,0,40)), locationgroup, App::Prop_None, "the lowest point the distribution will start from.");
    ADD_PROPERTY_TYPE(Spacing, (5.0), locationgroup, App::Prop_None, "The even spacing between the points");

    Spacing.setConstraints(&SpacingContraint);
}
bool CWLVerticalUniformDistribution::ComputeLocationCoordinateMatrixP3(const WindLabAPI::WindLabSimuData& Data, mat &dLocCoord)
{
    // Computing the location coordinates
    for (int loop = 0; loop < Data.numberOfSpatialPosition.getValue(); loop++) {
        //dLocCoord(loop, 0) = loop + 1;
        //dLocCoord(loop, 1) = LowestPoint.getValue().x;
        //dLocCoord(loop, 2) = LowestPoint.getValue().y;
        //dLocCoord(loop, 3) = LowestPoint.getValue().z + loop * Spacing.getValue();

       dLocCoord(loop, 0) = loop + 1;		
	   dLocCoord(loop, 1) = (Base::Quantity(LowestPoint.getValue().x, LowestPoint.getUnit())).getValueAs(Base::Quantity::Metre);
       dLocCoord(loop, 2) = (Base::Quantity(LowestPoint.getValue().y, LowestPoint.getUnit())).getValueAs(Base::Quantity::Metre);
       dLocCoord(loop, 3) = (Base::Quantity(LowestPoint.getValue().z, LowestPoint.getUnit())).getValueAs(Base::Quantity::Metre) + loop * Spacing.getQuantityValue().getValueAs(Base::Quantity::Metre);

    }

    return true;
}


bool CWLVerticalUniformDistribution::OnInitialSetting(const WindLabAPI::WindLabSimuData& Data)
{
    WindLabGui::VerticalLocationDistributionDialogEdit* dlg = new WindLabGui::VerticalLocationDistributionDialogEdit(LowestPoint, Spacing,Data.spatialDistribution);
    Gui::Control().showDialog(dlg);

	return true;
}
