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

#include "RandomnessProviders.h"
#include "widgets/cgaussianrandomnessproviderdlg.h"
#include "widgets/RPSRandomDistributionDialog.h"

#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_cdf.h>
#include <Base/Quantity.h>

using namespace RandomnessProviders;

static double para0 = 0;
static double para1 = 1;
static double para2 = 1;

//Weibull
PROPERTY_SOURCE(RandomnessProviders::CWeibullRandomnessProvider, WindLabAPI::WindLabFeatureRandomness)

CWeibullRandomnessProvider::CWeibullRandomnessProvider()
{
   ADD_PROPERTY_TYPE(ScaleParameter, (1.0), "Parameters", App::Prop_None, "The scale parameter");
   ADD_PROPERTY_TYPE(ExponentFactor, (1.0), "Parameters", App::Prop_None, "The Weibull distribution exponential factor");
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#the-weibull-distribution");

}

bool CWeibullRandomnessProvider::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
        para1 = dlg->getParametersVector().at(1);
    }

    return true;
}

bool CWeibullRandomnessProvider::GenerateRandomCubeFPS(const WindLabAPI::WindLabSimulationData& Data, cube& dRandomValueCube)
{
    bool returnResult = true;

    for (int i = 0; i < Data.numberOfSample.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        mat array(Data.numberOfFrequency.getValue(), Data.numberOfSpatialPosition.getValue());

        returnResult = this->GenerateRandomMatrixFP(Data, array);

        for (int j = 0; j <Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            for (int k = 0; k <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; k++)
            {
                dRandomValueCube(k, j, i) = array(k, j);
            }
        }
    }
    return returnResult;
}

bool CWeibullRandomnessProvider::GenerateRandomMatrixFP(const WindLabAPI::WindLabSimulationData& Data, mat &dRandomValueArray)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);

     bool returnResult = true;

    for (int i = 0; i < Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        for (int j = 0; j <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            dRandomValueArray(j, i) = gsl_ran_weibull(r, ScaleParameter.getValue() /*a*/, ExponentFactor.getValue() /*b*/);
        }
    }

    return returnResult;
}

bool CWeibullRandomnessProvider::ComputeRandomValue(const WindLabAPI::WindLabSimulationData& Data, double &dValue)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);
    dValue = gsl_ran_weibull(r, ScaleParameter.getValue() /*a*/, ExponentFactor.getValue() /*b*/);

    return true;

}

//Gaussian
PROPERTY_SOURCE(RandomnessProviders::CGaussianRandomnessProvider, WindLabAPI::WindLabFeatureRandomness)

CGaussianRandomnessProvider::CGaussianRandomnessProvider()
{
   ADD_PROPERTY_TYPE(Mean, (0.0), "Parameters", App::Prop_None, "The mean value");
   ADD_PROPERTY_TYPE(StandardDeviation, (1.0), "Parameters", App::Prop_None, "The standard deviation");
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#the-gaussian-distribution");


}
bool CGaussianRandomnessProvider::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
        para1 = dlg->getParametersVector().at(1);
    }

    return true;
}

bool CGaussianRandomnessProvider::GenerateRandomCubeFPS(const WindLabAPI::WindLabSimulationData& Data, cube& dRandomValueCube)
{
    bool returnResult = true;

    for (int i = 0; i < Data.numberOfSample.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        mat array(Data.numberOfFrequency.getValue(), Data.numberOfSpatialPosition.getValue());

        returnResult = this->GenerateRandomMatrixFP(Data, array);

        for (int j = 0; j <Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            for (int k = 0; k <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; k++)
            {
                dRandomValueCube(k, j, i) = array(k, j);
            }
        }
    }
    return returnResult;
}

bool CGaussianRandomnessProvider::GenerateRandomMatrixFP(const WindLabAPI::WindLabSimulationData& Data, mat &dRandomValueArray)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);

     bool returnResult = true;

    for (int i = 0; i < Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        for (int j = 0; j <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            returnResult = ComputeRandomValue(Data, dRandomValueArray(j, i));
            gsl_ran_gaussian(r, StandardDeviation.getQuantityValue().getValueAs(Base::Quantity::MetrePerSecond)) + Mean.getValue();
        }
    }
    return returnResult;

}

bool CGaussianRandomnessProvider::ComputeRandomValue(const WindLabAPI::WindLabSimulationData& Data, double &dValue)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);
    dValue = gsl_ran_gaussian(r, StandardDeviation.getQuantityValue().getValueAs(Base::Quantity::MetrePerSecond)) + Mean.getValue();
        return true;

}

//Exponential
PROPERTY_SOURCE(RandomnessProviders::CExponentialRandomnessProvider, WindLabAPI::WindLabFeatureRandomness)

CExponentialRandomnessProvider::CExponentialRandomnessProvider()
{
   ADD_PROPERTY_TYPE(Mean, (1.0), "Parameters", App::Prop_None, "The mean or the expected value");
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#the-exponential-distribution");

}

bool CExponentialRandomnessProvider::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
    }

    return true;
}

bool CExponentialRandomnessProvider::GenerateRandomCubeFPS(const WindLabAPI::WindLabSimulationData& Data, cube& dRandomValueCube)
{
    bool returnResult = true;

    for (int i = 0; i < Data.numberOfSample.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        mat array(Data.numberOfFrequency.getValue(), Data.numberOfSpatialPosition.getValue());

        returnResult = this->GenerateRandomMatrixFP(Data, array);

        for (int j = 0; j <Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            for (int k = 0; k <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; k++)
            {
                dRandomValueCube(k, j, i) = array(k, j);
            }
        }
    }
    return returnResult;
}


bool CExponentialRandomnessProvider::GenerateRandomMatrixFP(const WindLabAPI::WindLabSimulationData& Data, mat &dRandomValueArray)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);

    bool returnResult = true;

    for (int i = 0; i < Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        for (int j = 0; j <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            returnResult = ComputeRandomValue(Data, dRandomValueArray(j, i));
            gsl_ran_exponential(r, static_cast<double>(1 / Mean.getValue() /*lambda*/));
        }
    }
    return returnResult;

}

bool CExponentialRandomnessProvider::ComputeRandomValue(const WindLabAPI::WindLabSimulationData& Data, double &dValue)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);
    dValue = gsl_ran_exponential(r, static_cast<double>(1 / Mean.getValue() /*lambda*/));
        return true;

}

//Laplace
PROPERTY_SOURCE(RandomnessProviders::CLaplaceRandomnessProvider, WindLabAPI::WindLabFeatureRandomness)

CLaplaceRandomnessProvider::CLaplaceRandomnessProvider()
{
   ADD_PROPERTY_TYPE(Width, (1.0), "Parameters", App::Prop_None, "The mean or the expected value");
   ADD_PROPERTY_TYPE(Mean, (1.0), "Parameters", App::Prop_None, "The mean or the expected value");
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#the-laplace-distribution");

}
bool CLaplaceRandomnessProvider::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
        para1 = dlg->getParametersVector().at(1);
    }

    return true;
}

bool CLaplaceRandomnessProvider::GenerateRandomCubeFPS(const WindLabAPI::WindLabSimulationData& Data, cube& dRandomValueCube)
{
    bool returnResult = true;

    for (int i = 0; i < Data.numberOfSample.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        mat array(Data.numberOfFrequency.getValue(), Data.numberOfSpatialPosition.getValue());

        returnResult = this->GenerateRandomMatrixFP(Data, array);

        for (int j = 0; j <Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            for (int k = 0; k <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; k++)
            {
                dRandomValueCube(k, j, i) = array(k, j);
            }
        }
    }
    return returnResult;
}

bool CLaplaceRandomnessProvider::GenerateRandomMatrixFP(const WindLabAPI::WindLabSimulationData& Data, mat &dRandomValueArray)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);

     bool returnResult = true;

    for (int i = 0; i < Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        for (int j = 0; j <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            returnResult = ComputeRandomValue(Data, dRandomValueArray(j, i));
            gsl_ran_laplace(r, Width.getValue() /*a*/) + Mean.getValue() /*mu*/;
        }
    }
    return returnResult;

}

bool CLaplaceRandomnessProvider::ComputeRandomValue(const WindLabAPI::WindLabSimulationData& Data, double &dValue)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);
    dValue = gsl_ran_laplace(r, Width.getValue() /*a*/) + Mean.getValue() /*mu*/;
    return true;

}

//Exponential Power
PROPERTY_SOURCE(RandomnessProviders::CExponentialPowerRandomnessProvider, WindLabAPI::WindLabFeatureRandomness)

CExponentialPowerRandomnessProvider::CExponentialPowerRandomnessProvider()
{
   ADD_PROPERTY_TYPE(ScaleParameter, (1.0), "Parameters", App::Prop_None, "The scale parameter");
   ADD_PROPERTY_TYPE(ExponentFactor, (2.5), "Parameters", App::Prop_None, "The exponent factor");
   ADD_PROPERTY_TYPE(Mean, (0.0), "Parameters", App::Prop_None, "The mean value");
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#the-exponential-power-distribution");

}
bool CExponentialPowerRandomnessProvider::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
        para1 = dlg->getParametersVector().at(1);
        para2 = dlg->getParametersVector().at(2);

    }

    return true;
}

bool CExponentialPowerRandomnessProvider::GenerateRandomCubeFPS(const WindLabAPI::WindLabSimulationData& Data, cube& dRandomValueCube)
{
    bool returnResult = true;

    for (int i = 0; i < Data.numberOfSample.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        mat array(Data.numberOfFrequency.getValue(), Data.numberOfSpatialPosition.getValue());

        returnResult = this->GenerateRandomMatrixFP(Data, array);

        for (int j = 0; j <Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            for (int k = 0; k <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; k++)
            {
                dRandomValueCube(k, j, i) = array(k, j);
            }
        }
    }
    return returnResult;
}

bool CExponentialPowerRandomnessProvider::GenerateRandomMatrixFP(const WindLabAPI::WindLabSimulationData& Data, mat &dRandomValueArray)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);

    bool returnResult = true;

    for (int i = 0; i < Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        for (int j = 0; j <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            dRandomValueArray(j, i) = gsl_ran_exppow(r, ScaleParameter.getValue() /*a*/, ExponentFactor.getValue() /*b*/) + Mean.getValue() /*mu*/;

        }
    }
    return returnResult;

}

bool CExponentialPowerRandomnessProvider::ComputeRandomValue(const WindLabAPI::WindLabSimulationData& Data, double &dValue)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);
    dValue = gsl_ran_exppow(r, ScaleParameter.getValue() /*a*/, ExponentFactor.getValue() /*b*/) + Mean.getValue() /*mu*/;
    return true;

}

//Cauchy
PROPERTY_SOURCE(RandomnessProviders::CCauchyRandomnessProvider, WindLabAPI::WindLabFeatureRandomness)

CCauchyRandomnessProvider::CCauchyRandomnessProvider()
{
   ADD_PROPERTY_TYPE(ScaleParameter, (1.0), "Parameters", App::Prop_None, "The scale parameter");
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#the-cauchy-distribution");

}

bool CCauchyRandomnessProvider::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
    }

    return true;
}

bool CCauchyRandomnessProvider::GenerateRandomCubeFPS(const WindLabAPI::WindLabSimulationData& Data, cube& dRandomValueCube)
{
    bool returnResult = true;

    for (int i = 0; i < Data.numberOfSample.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        mat array(Data.numberOfFrequency.getValue(), Data.numberOfSpatialPosition.getValue());

        returnResult = this->GenerateRandomMatrixFP(Data, array);

        for (int j = 0; j <Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            for (int k = 0; k <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; k++)
            {
                dRandomValueCube(k, j, i) = array(k, j);
            }
        }
    }
    return returnResult;
}

bool CCauchyRandomnessProvider::GenerateRandomMatrixFP(const WindLabAPI::WindLabSimulationData& Data, mat &dRandomValueArray)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);

     bool returnResult = true;

    for (int i = 0; i < Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        for (int j = 0; j <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            dRandomValueArray(j, i) = gsl_ran_cauchy(r, ScaleParameter.getValue() /*a*/);

        }
    }
    return returnResult;

}

bool CCauchyRandomnessProvider::ComputeRandomValue(const WindLabAPI::WindLabSimulationData& Data, double &dValue)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);
    dValue = gsl_ran_cauchy(r, ScaleParameter.getValue() /*a*/);
        return true;

}

//Rayleigh
PROPERTY_SOURCE(RandomnessProviders::CRayleighRandomnessProvider, WindLabAPI::WindLabFeatureRandomness)

CRayleighRandomnessProvider::CRayleighRandomnessProvider()
{
   ADD_PROPERTY_TYPE(ScaleParameter, (1.0), "Parameters", App::Prop_None, "The scale parameter");
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#the-rayleigh-distribution");

}

bool CRayleighRandomnessProvider::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
    }

    return true;
}

bool CRayleighRandomnessProvider::GenerateRandomCubeFPS(const WindLabAPI::WindLabSimulationData& Data, cube& dRandomValueCube)
{
    bool returnResult = true;

    for (int i = 0; i < Data.numberOfSample.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        mat array(Data.numberOfFrequency.getValue(), Data.numberOfSpatialPosition.getValue());

        returnResult = this->GenerateRandomMatrixFP(Data, array);

        for (int j = 0; j <Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            for (int k = 0; k <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; k++)
            {
                dRandomValueCube(k, j, i) = array(k, j);
            }
        }
    }
    return returnResult;
}

bool CRayleighRandomnessProvider::GenerateRandomMatrixFP(const WindLabAPI::WindLabSimulationData& Data, mat &dRandomValueArray)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);

    bool returnResult = true;

    for (int i = 0; i < Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        for (int j = 0; j <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            dRandomValueArray(j, i) = gsl_ran_rayleigh(r, ScaleParameter.getValue() /*sigma*/);

        }
    }
    return returnResult;

}

bool CRayleighRandomnessProvider::ComputeRandomValue(const WindLabAPI::WindLabSimulationData& Data, double &dValue)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);
    dValue = gsl_ran_rayleigh(r, ScaleParameter.getValue() /*sigma*/);
            return true;

}

//Rayleigh Tail
PROPERTY_SOURCE(RandomnessProviders::CRayleighTailRandomnessProvider, WindLabAPI::WindLabFeatureRandomness)

CRayleighTailRandomnessProvider::CRayleighTailRandomnessProvider()
{
   ADD_PROPERTY_TYPE(ScaleParameter, (1.0), "Parameters", App::Prop_None, "The scale parameter");
   ADD_PROPERTY_TYPE(LowerLimit, (1.0), "Parameters", App::Prop_None, "The scale parameter");
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#the-rayleigh-tail-distribution");

}

bool CRayleighTailRandomnessProvider::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
        para1 = dlg->getParametersVector().at(1);
    }

    return true;
}

bool CRayleighTailRandomnessProvider::GenerateRandomCubeFPS(const WindLabAPI::WindLabSimulationData& Data, cube& dRandomValueCube)
{
    bool returnResult = true;

    for (int i = 0; i < Data.numberOfSample.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        mat array(Data.numberOfFrequency.getValue(), Data.numberOfSpatialPosition.getValue());

        returnResult = this->GenerateRandomMatrixFP(Data, array);

        for (int j = 0; j <Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            for (int k = 0; k <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; k++)
            {
                dRandomValueCube(k, j, i) = array(k, j);
            }
        }
    }
    return returnResult;
}

bool CRayleighTailRandomnessProvider::GenerateRandomMatrixFP(const WindLabAPI::WindLabSimulationData& Data, mat &dRandomValueArray)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);

    bool returnResult = true;

    for (int i = 0; i < Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        for (int j = 0; j <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            dRandomValueArray(j, i) = gsl_ran_rayleigh_tail(r, LowerLimit.getValue() /*a*/, ScaleParameter.getValue() /*sigma*/);

        }
    }
    return returnResult;
}

bool CRayleighTailRandomnessProvider::ComputeRandomValue(const WindLabAPI::WindLabSimulationData& Data, double &dValue)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);
    dValue = gsl_ran_rayleigh_tail(r, LowerLimit.getValue() /*a*/, ScaleParameter.getValue() /*sigma*/);
    return true;

}

//Landau
PROPERTY_SOURCE(RandomnessProviders::CLandauRandomnessProvider, WindLabAPI::WindLabFeatureRandomness)

CLandauRandomnessProvider::CLandauRandomnessProvider() {
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#the-landau-distribution");

}

bool CLandauRandomnessProvider::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {

    }

    return true;
}

bool CLandauRandomnessProvider::GenerateRandomCubeFPS(const WindLabAPI::WindLabSimulationData& Data, cube& dRandomValueCube)
{
    bool returnResult = true;

    for (int i = 0; i < Data.numberOfSample.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        mat array(Data.numberOfFrequency.getValue(), Data.numberOfSpatialPosition.getValue());

        returnResult = this->GenerateRandomMatrixFP(Data, array);

        for (int j = 0; j <Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            for (int k = 0; k <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; k++)
            {
                dRandomValueCube(k, j, i) = array(k, j);
            }
        }
    }
    return returnResult;
}

bool CLandauRandomnessProvider::GenerateRandomMatrixFP(const WindLabAPI::WindLabSimulationData& Data, mat &dRandomValueArray)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);

    bool returnResult = true;

    for (int i = 0; i < Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        for (int j = 0; j <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            dRandomValueArray(j, i) = gsl_ran_landau(r);

        }
    }
    return returnResult;

}

bool CLandauRandomnessProvider::ComputeRandomValue(const WindLabAPI::WindLabSimulationData& Data, double &dValue)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);
    dValue = gsl_ran_landau(r);
    return true;

}

//LevyAlphaStable
PROPERTY_SOURCE(RandomnessProviders::CLevyAlphaStableRandomnessProvider, WindLabAPI::WindLabFeatureRandomness)

CLevyAlphaStableRandomnessProvider::CLevyAlphaStableRandomnessProvider()
{
   ADD_PROPERTY_TYPE(ScaleParameter, (1.0), "Parameters", App::Prop_None, "The scale parameter");
   ADD_PROPERTY_TYPE(ExponentFactor, (1.0), "Parameters", App::Prop_None, "The exponent factor");
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#the-levy-alpha-stable-distributions");

}
bool CLevyAlphaStableRandomnessProvider::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
        para1 = dlg->getParametersVector().at(1);
    }

    return true;
}

bool CLevyAlphaStableRandomnessProvider::GenerateRandomCubeFPS(const WindLabAPI::WindLabSimulationData& Data, cube& dRandomValueCube)
{
    bool returnResult = true;

    for (int i = 0; i < Data.numberOfSample.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        mat array(Data.numberOfFrequency.getValue(), Data.numberOfSpatialPosition.getValue());

        returnResult = this->GenerateRandomMatrixFP(Data, array);

        for (int j = 0; j <Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            for (int k = 0; k <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; k++)
            {
                dRandomValueCube(k, j, i) = array(k, j);
            }
        }
    }
    return returnResult;
}

bool CLevyAlphaStableRandomnessProvider::GenerateRandomMatrixFP(const WindLabAPI::WindLabSimulationData& Data, mat &dRandomValueArray)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);

    bool returnResult = true;

    for (int i = 0; i < Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        for (int j = 0; j <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            dRandomValueArray(j, i) = gsl_ran_levy(r, ScaleParameter.getValue() /*c*/, ExponentFactor.getValue() /*alpha*/);

        }
    }
    return returnResult;

}

bool CLevyAlphaStableRandomnessProvider::ComputeRandomValue(const WindLabAPI::WindLabSimulationData& Data, double &dValue)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);
    dValue = gsl_ran_levy(r, ScaleParameter.getValue() /*c*/, ExponentFactor.getValue() /*alpha*/);
    return true;

}

//LevySkewAlphaStable
PROPERTY_SOURCE(RandomnessProviders::CLevySkewAlphaStableRandomnessProvider, WindLabAPI::WindLabFeatureRandomness)

CLevySkewAlphaStableRandomnessProvider::CLevySkewAlphaStableRandomnessProvider()
{
   ADD_PROPERTY_TYPE(ScaleParameter, (1.0), "Parameters", App::Prop_None, "The scale parameter");
   ADD_PROPERTY_TYPE(ExponentFactor, (1.0), "Parameters", App::Prop_None, "The exponent factor");
   ADD_PROPERTY_TYPE(SkewnessParameter, (1.0), "Parameters", App::Prop_None, "The skewness parameter");
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#the-levy-skew-alpha-stable-distribution");

}

bool CLevySkewAlphaStableRandomnessProvider::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
        para1 = dlg->getParametersVector().at(1);
        para2 = dlg->getParametersVector().at(2);
    }

    return true;
}

bool CLevySkewAlphaStableRandomnessProvider::GenerateRandomCubeFPS(const WindLabAPI::WindLabSimulationData& Data, cube& dRandomValueCube)
{
    bool returnResult = true;

    for (int i = 0; i < Data.numberOfSample.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        mat array(Data.numberOfFrequency.getValue(), Data.numberOfSpatialPosition.getValue());

        returnResult = this->GenerateRandomMatrixFP(Data, array);

        for (int j = 0; j <Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            for (int k = 0; k <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; k++)
            {
                dRandomValueCube(k, j, i) = array(k, j);
            }
        }
    }
    return returnResult;
}

bool CLevySkewAlphaStableRandomnessProvider::GenerateRandomMatrixFP(const WindLabAPI::WindLabSimulationData& Data, mat &dRandomValueArray)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);

    bool returnResult = true;

    for (int i = 0; i < Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        for (int j = 0; j <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            dRandomValueArray(j, i) = gsl_ran_levy_skew(r, ScaleParameter.getValue() /*c*/, ExponentFactor.getValue() /*alpha*/, SkewnessParameter.getValue() /*beta*/);

        }
    }
    return returnResult;

}

bool CLevySkewAlphaStableRandomnessProvider::ComputeRandomValue(const WindLabAPI::WindLabSimulationData& Data, double &dValue)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);
    dValue = gsl_ran_levy_skew(r, ScaleParameter.getValue() /*c*/, ExponentFactor.getValue() /*alpha*/, SkewnessParameter.getValue() /*beta*/);
    return true;

}

//Flat
PROPERTY_SOURCE(RandomnessProviders::CFlatRandomnessProvider, WindLabAPI::WindLabFeatureRandomness)

CFlatRandomnessProvider::CFlatRandomnessProvider()
{
    ADD_PROPERTY_TYPE(LowerBound, (0.5), "Parameters", App::Prop_None, "The lower bound limit value");
    ADD_PROPERTY_TYPE(UpperBound, (2.5), "Parameters", App::Prop_None, "The upper bound limit value");
    this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#the-flat-uniform-distribution");

}
bool CFlatRandomnessProvider::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
        para1 = dlg->getParametersVector().at(1);
    }

    return true;
}

bool CFlatRandomnessProvider::GenerateRandomCubeFPS(const WindLabAPI::WindLabSimulationData& Data, cube& dRandomValueCube)
{
    bool returnResult = true;

    for (int i = 0; i < Data.numberOfSample.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        mat array(Data.numberOfFrequency.getValue(), Data.numberOfSpatialPosition.getValue());

        returnResult = this->GenerateRandomMatrixFP(Data, array);

        for (int j = 0; j <Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            for (int k = 0; k <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; k++)
            {
                dRandomValueCube(k, j, i) = array(k, j);
            }
        }
    }
    return returnResult;
}

bool CFlatRandomnessProvider::GenerateRandomMatrixFP(const WindLabAPI::WindLabSimulationData& Data, mat &dRandomValueArray)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);

     bool returnResult = true;

    for (int i = 0; i < Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        for (int j = 0; j <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            dRandomValueArray(j, i) = gsl_ran_flat(r, LowerBound.getValue() /*a*/, UpperBound.getValue() /*b*/);

        }
    }
    return returnResult;
}

bool CFlatRandomnessProvider::ComputeRandomValue(const WindLabAPI::WindLabSimulationData& Data, double &dValue)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);
    dValue = gsl_ran_flat(r, LowerBound.getValue() /*a*/, UpperBound.getValue() /*b*/);
            return true;

}

//Gamma
PROPERTY_SOURCE(RandomnessProviders::CGammaRandomnessProvider, WindLabAPI::WindLabFeatureRandomness)

CGammaRandomnessProvider::CGammaRandomnessProvider()
{
    ADD_PROPERTY_TYPE(ParameterA, (0.5), "Parameters", App::Prop_None, "The A parameter");
    ADD_PROPERTY_TYPE(ParameterB, (2.5), "Parameters", App::Prop_None, "The B parameter");
    this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#the-gamma-distribution");

}
bool CGammaRandomnessProvider::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
        para1 = dlg->getParametersVector().at(1);
    }

    return true;
}

bool CGammaRandomnessProvider::GenerateRandomCubeFPS(const WindLabAPI::WindLabSimulationData& Data, cube& dRandomValueCube)
{
    bool returnResult = true;

    for (int i = 0; i < Data.numberOfSample.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        mat array(Data.numberOfFrequency.getValue(), Data.numberOfSpatialPosition.getValue());

        returnResult = this->GenerateRandomMatrixFP(Data, array);

        for (int j = 0; j <Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            for (int k = 0; k <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; k++)
            {
                dRandomValueCube(k, j, i) = array(k, j);
            }
        }
    }
    return returnResult;
}

bool CGammaRandomnessProvider::GenerateRandomMatrixFP(const WindLabAPI::WindLabSimulationData& Data, mat &dRandomValueArray)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);

     bool returnResult = true;

    for (int i = 0; i < Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        for (int j = 0; j <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            dRandomValueArray(j, i) = gsl_ran_gamma(r, ParameterA.getValue() /*a*/, ParameterB.getValue() /*b*/);

        }
    }
    return returnResult;

}

bool CGammaRandomnessProvider::ComputeRandomValue(const WindLabAPI::WindLabSimulationData& Data, double &dValue)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);
    dValue = gsl_ran_gamma(r, ParameterA.getValue() /*a*/, ParameterB.getValue() /*b*/);
            return true;

}

//Lognormal
PROPERTY_SOURCE(RandomnessProviders::CLognormalRandomnessProvider, WindLabAPI::WindLabFeatureRandomness)

CLognormalRandomnessProvider::CLognormalRandomnessProvider()
{
    ADD_PROPERTY_TYPE(Mean, (0.0), "Parameters", App::Prop_None, "The mean value");
    ADD_PROPERTY_TYPE(StandardDeviation, (1.0), "Parameters", App::Prop_None, "The stantard devialtion");
    this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#the-lognormal-distribution");

}
bool CLognormalRandomnessProvider::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
        para1 = dlg->getParametersVector().at(1);
    }

    return true;
}

bool CLognormalRandomnessProvider::GenerateRandomCubeFPS(const WindLabAPI::WindLabSimulationData& Data, cube& dRandomValueCube)
{
    bool returnResult = true;

    for (int i = 0; i < Data.numberOfSample.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        mat array(Data.numberOfFrequency.getValue(), Data.numberOfSpatialPosition.getValue());

        returnResult = this->GenerateRandomMatrixFP(Data, array);

        for (int j = 0; j <Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            for (int k = 0; k <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; k++)
            {
                dRandomValueCube(k, j, i) = array(k, j);
            }
        }
    }
    return returnResult;
}

bool CLognormalRandomnessProvider::GenerateRandomMatrixFP(const WindLabAPI::WindLabSimulationData& Data, mat &dRandomValueArray)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);

     bool returnResult = true;

    for (int i = 0; i < Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        for (int j = 0; j <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            dRandomValueArray(j, i) = gsl_ran_lognormal(r, Mean.getValue() /*zeta*/, StandardDeviation.getQuantityValue().getValueAs(Base::Quantity::MetrePerSecond) /*sigma*/);

        }
    }
    return returnResult;

}

bool CLognormalRandomnessProvider::ComputeRandomValue(const WindLabAPI::WindLabSimulationData& Data, double &dValue)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);
    dValue = gsl_ran_lognormal(r, Mean.getValue() /*zeta*/, StandardDeviation.getQuantityValue().getValueAs(Base::Quantity::MetrePerSecond) /*sigma*/);
     return true;

}

//ChiSquared
PROPERTY_SOURCE(RandomnessProviders::CChiSquaredRandomnessProvider, WindLabAPI::WindLabFeatureRandomness)

CChiSquaredRandomnessProvider::CChiSquaredRandomnessProvider()
{
    ADD_PROPERTY_TYPE(DegreeOfFreedom, (1.0), "Parameters", App::Prop_None, "The degree of freedom");
    this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#the-chi-squared-distribution");

}

bool CChiSquaredRandomnessProvider::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
    }

    return true;
}

bool CChiSquaredRandomnessProvider::GenerateRandomCubeFPS(const WindLabAPI::WindLabSimulationData& Data, cube& dRandomValueCube)
{
    bool returnResult = true;

    for (int i = 0; i < Data.numberOfSample.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        mat array(Data.numberOfFrequency.getValue(), Data.numberOfSpatialPosition.getValue());

        returnResult = this->GenerateRandomMatrixFP(Data, array);

        for (int j = 0; j <Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            for (int k = 0; k <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; k++)
            {
                dRandomValueCube(k, j, i) = array(k, j);
            }
        }
    }
    return returnResult;
}

bool CChiSquaredRandomnessProvider::GenerateRandomMatrixFP(const WindLabAPI::WindLabSimulationData& Data, mat &dRandomValueArray)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);

    bool returnResult = true;

    for (int i = 0; i < Data.numberOfSpatialPosition.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; i++)
    {
        for (int j = 0; j <Data.numberOfFrequency.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
        {
            dRandomValueArray(j, i) = gsl_ran_chisq(r, DegreeOfFreedom.getValue() /*nu*/);

        }
    }
    return returnResult;

}

bool CChiSquaredRandomnessProvider::ComputeRandomValue(const WindLabAPI::WindLabSimulationData& Data, double &dValue)
{
    // create a generator chosen by the environment variable GSL_RNG_TYPE
    gsl_rng_env_setup();
    const gsl_rng_type *T = gsl_rng_default;
    gsl_rng *r = gsl_rng_alloc(T);
    dValue = gsl_ran_chisq(r, DegreeOfFreedom.getValue() /*nu*/);
    return true;

}

//ALL PROBABILITY DENSITY FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


PROPERTY_SOURCE(RandomnessProviders::CWeibullPDF, WindLabAPI::WindLabFeatureProbabilityDensityFunction)

CWeibullPDF::CWeibullPDF()
{
   ADD_PROPERTY_TYPE(ScaleParameter, (1.0), "Parameters", App::Prop_None, "The scale parameter");
   ADD_PROPERTY_TYPE(ExponentFactor, (1.0), "Parameters", App::Prop_None, "The Weibull distribution exponential factor");
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_ran_weibull_pdf");

}

bool CWeibullPDF::ComputePDFValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
   dValue = gsl_ran_weibull_pdf(x, ScaleParameter.getValue() /*a*/, ExponentFactor.getValue() /*b*/);
    return true;

}
bool CWeibullPDF::ComputePDFVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{
    bool returnResult = true;

   for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
        dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
        returnResult = ComputePDFValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;

}
bool CWeibullPDF::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para1 = dlg->getParametersVector().at(1);
    }

    return true;
}

//
PROPERTY_SOURCE(RandomnessProviders::CGaussianPDF, WindLabAPI::WindLabFeatureProbabilityDensityFunction)

CGaussianPDF::CGaussianPDF()
{
   ADD_PROPERTY_TYPE(Mean, (0.0), "Parameters", App::Prop_None, "The mean value");
   ADD_PROPERTY_TYPE(StandardDeviation, (1.0), "Parameters", App::Prop_None, "The standard deviation");
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_ran_gaussian_pdf");


}
bool CGaussianPDF::ComputePDFValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
   dValue = gsl_ran_gaussian_pdf(x, StandardDeviation.getQuantityValue().getValueAs(Base::Quantity::MetrePerSecond) /*a*/) + Mean.getValue() /*mu*/;
   return true;

}
bool CGaussianPDF::ComputePDFVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{
    bool returnResult = true;

   for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
        dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
        returnResult = ComputePDFValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;

}
bool CGaussianPDF::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para1 = dlg->getParametersVector().at(1);
    }

    return true;
}

//
PROPERTY_SOURCE(RandomnessProviders::CExponentialPDF, WindLabAPI::WindLabFeatureProbabilityDensityFunction)

CExponentialPDF::CExponentialPDF()
{
   ADD_PROPERTY_TYPE(Mean, (1.0), "Parameters", App::Prop_None, "The mean or the expected value");
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_ran_exponential_pdf");

}
bool CExponentialPDF::ComputePDFValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
   dValue = gsl_ran_exponential_pdf(x, Mean.getValue());
   return true;

}
bool CExponentialPDF::ComputePDFVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{
    bool returnResult = true;

   for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
        dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
        returnResult = ComputePDFValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;
}
bool CExponentialPDF::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
    }

    return true;}

//
PROPERTY_SOURCE(RandomnessProviders::CLaplacePDF, WindLabAPI::WindLabFeatureProbabilityDensityFunction)

CLaplacePDF::CLaplacePDF()
{
   ADD_PROPERTY_TYPE(Width, (1.0), "Parameters", App::Prop_None, "The with");
   ADD_PROPERTY_TYPE(Mean, (1.0), "Parameters", App::Prop_None, "The mean or the expected value");
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_ran_laplace_pdf");
}
bool CLaplacePDF::ComputePDFValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
   dValue = gsl_ran_laplace_pdf(x, Width.getValue() /*a*/) + Mean.getValue() /*mu*/;
   return true;

}
bool CLaplacePDF::ComputePDFVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{
    bool returnResult = true;

   for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
        dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
        returnResult = ComputePDFValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;

}
bool CLaplacePDF::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
        para1 = dlg->getParametersVector().at(1);
    }

    return true;
}


//
PROPERTY_SOURCE(RandomnessProviders::CExponentialPowerPDF, WindLabAPI::WindLabFeatureProbabilityDensityFunction)

CExponentialPowerPDF::CExponentialPowerPDF()
{
   ADD_PROPERTY_TYPE(ScaleParameter, (1.0), "Parameters", App::Prop_None, "The scale parameter");
   ADD_PROPERTY_TYPE(ExponentFactor, (2.5), "Parameters", App::Prop_None, "The exponent factor");
   ADD_PROPERTY_TYPE(Mean, (0.0), "Parameters", App::Prop_None, "The mean value");
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_ran_exppow_pdf");

}

bool CExponentialPowerPDF::ComputePDFValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
   dValue = gsl_ran_exppow_pdf(x, ScaleParameter.getValue() /*a*/, ExponentFactor.getValue() /*b*/) + Mean.getValue() /*mu*/;
   return true;
}
bool CExponentialPowerPDF::ComputePDFVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{
    bool returnResult = true;

   for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
        dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
        returnResult = ComputePDFValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;

}
bool CExponentialPowerPDF::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
        para1 = dlg->getParametersVector().at(1);
        para2 = dlg->getParametersVector().at(2);
    }

    return true;
}

//

PROPERTY_SOURCE(RandomnessProviders::CCauchyPDF, WindLabAPI::WindLabFeatureProbabilityDensityFunction)

CCauchyPDF::CCauchyPDF()
{
   ADD_PROPERTY_TYPE(ScaleParameter, (1.0), "Parameters", App::Prop_None, "The scale parameter");
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_ran_cauchy_pdf");

}

bool CCauchyPDF::ComputePDFValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
   dValue = gsl_ran_cauchy_pdf(x, ScaleParameter.getValue() /*a*/);
    return true;
}

bool CCauchyPDF::ComputePDFVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{

    bool returnResult = true;

   for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
        dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
        returnResult = ComputePDFValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;
}
bool CCauchyPDF::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
    }

    return true;
}


//
PROPERTY_SOURCE(RandomnessProviders::CRayleighPDF, WindLabAPI::WindLabFeatureProbabilityDensityFunction)

CRayleighPDF::CRayleighPDF()
{
   ADD_PROPERTY_TYPE(ScaleParameter, (1.0), "Parameters", App::Prop_None, "The scale parameter");
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_ran_rayleigh_pdf");
}
bool CRayleighPDF::ComputePDFValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
   dValue = gsl_ran_rayleigh_pdf(x, ScaleParameter.getValue() /*sigma*/);
   return true;

}

bool CRayleighPDF::ComputePDFVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{
    bool returnResult = true;

   for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
        dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
        returnResult = ComputePDFValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;

}
bool CRayleighPDF::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
    }

    return true;
}


//
PROPERTY_SOURCE(RandomnessProviders::CRayleighTailPDF, WindLabAPI::WindLabFeatureProbabilityDensityFunction)

CRayleighTailPDF::CRayleighTailPDF()
{
   ADD_PROPERTY_TYPE(ScaleParameter, (1.0), "Parameters", App::Prop_None, "The scale parameter");
   ADD_PROPERTY_TYPE(LowerLimit, (1.0), "Parameters", App::Prop_None, "The scale parameter");
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_ran_rayleigh_tail_pdf");

}
bool CRayleighTailPDF::ComputePDFValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
   dValue = gsl_ran_rayleigh_tail_pdf(x, LowerLimit.getValue() /*a*/, ScaleParameter.getValue() /*sigma*/);
   return true;
}

bool CRayleighTailPDF::ComputePDFVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{
    bool returnResult = true;

   for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
        dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
        returnResult = ComputePDFValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;

}
bool CRayleighTailPDF::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
        para1 = dlg->getParametersVector().at(1);
    }

    return true;
}


//
PROPERTY_SOURCE(RandomnessProviders::CLandauPDF, WindLabAPI::WindLabFeatureProbabilityDensityFunction)

CLandauPDF::CLandauPDF()
{
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_ran_landau_pdf");
}

bool CLandauPDF::ComputePDFValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
    dValue = gsl_ran_landau_pdf(x);
    return true;

}
bool CLandauPDF::ComputePDFVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{
    bool returnResult = true;

   for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
        dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
        returnResult = ComputePDFValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;
}

bool CLandauPDF::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {

    }

    return true;
}


//
PROPERTY_SOURCE(RandomnessProviders::CFlatPDF, WindLabAPI::WindLabFeatureProbabilityDensityFunction)

CFlatPDF::CFlatPDF()
{
    ADD_PROPERTY_TYPE(LowerBound, (0.5), "Parameters", App::Prop_None, "The lower bound limit value");
    ADD_PROPERTY_TYPE(UpperBound, (2.5), "Parameters", App::Prop_None, "The upper bound limit value");
    this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_ran_flat_pdf");
}

bool CFlatPDF::ComputePDFValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
    dValue = gsl_ran_flat_pdf(x, LowerBound.getValue() /*a*/, UpperBound.getValue() /*b*/);
    return true;

}
bool CFlatPDF::ComputePDFVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{
    if (Data.minVariableX.getValue() < LowerBound.getValue()
        || (Data.minVariableX.getValue()
            + Data.numberOfIncrementOfVariableX.getValue() * Data.incrementOfVariableX.getValue())
            > UpperBound.getValue())
        return false;

    bool returnResult = true;

   for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
        dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
        returnResult = ComputePDFValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;

}
bool CFlatPDF::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
        para1 = dlg->getParametersVector().at(1);
    }

    return true;
}


//
PROPERTY_SOURCE(RandomnessProviders::CGammaPDF, WindLabAPI::WindLabFeatureProbabilityDensityFunction)

CGammaPDF::CGammaPDF()
{
    ADD_PROPERTY_TYPE(ParameterA, (0.5), "Parameters", App::Prop_None, "The A parameter");
    ADD_PROPERTY_TYPE(ParameterB, (2.5), "Parameters", App::Prop_None, "The B parameter");
    this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_ran_gamma_pdf");

}

bool CGammaPDF::ComputePDFValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
    dValue = gsl_ran_gamma_pdf(x, ParameterA.getValue() /*a*/, ParameterB.getValue() /*b*/);
            return true;

}
bool CGammaPDF::ComputePDFVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{
    bool returnResult = true;

   for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
        dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
        returnResult = ComputePDFValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;

}
bool CGammaPDF::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
        para1 = dlg->getParametersVector().at(1);
    }

    return true;
}


//
PROPERTY_SOURCE(RandomnessProviders::CLognormalPDF, WindLabAPI::WindLabFeatureProbabilityDensityFunction)

CLognormalPDF::CLognormalPDF()
{
    ADD_PROPERTY_TYPE(Mean, (0.0), "Parameters", App::Prop_None, "The mean value");
    ADD_PROPERTY_TYPE(StandardDeviation, (1.0), "Parameters", App::Prop_None, "The stantard devialtion");
    this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_ran_lognormal_pdf");
}

bool CLognormalPDF::ComputePDFValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
    dValue = gsl_ran_lognormal_pdf(x, Mean.getValue() /*zeta*/, StandardDeviation.getQuantityValue().getValueAs(Base::Quantity::MetrePerSecond) /*sigma*/);
    return true;

}
bool CLognormalPDF::ComputePDFVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{
    bool returnResult = true;

   for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
        dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
        returnResult = ComputePDFValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;

}
bool CLognormalPDF::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
        para1 = dlg->getParametersVector().at(1);
    }

    return true;
}


//
PROPERTY_SOURCE(RandomnessProviders::CChiSquaredPDF, WindLabAPI::WindLabFeatureProbabilityDensityFunction)

CChiSquaredPDF::CChiSquaredPDF()
{
    ADD_PROPERTY_TYPE(DegreeOfFreedom, (1.0), "Parameters", App::Prop_None, "The degree of freedom");
    this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_ran_chisq_pdf");

}

bool CChiSquaredPDF::ComputePDFValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
    dValue = gsl_ran_chisq_pdf(x, DegreeOfFreedom.getValue() /*nu*/);
    return true;

}
bool CChiSquaredPDF::ComputePDFVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{
    bool returnResult = true;

   for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
        dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
        returnResult = ComputePDFValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;

}
bool CChiSquaredPDF::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
    }

    return true;
}

//ALL CUMULATIVE PROBABILITY DENSITY
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//static int CPDtype = 0;
//

PROPERTY_SOURCE(RandomnessProviders::CWeibullCPD, WindLabAPI::WindLabFeatureCumulativeProbabilityDistribution)

CWeibullCPD::CWeibullCPD()
{
   ADD_PROPERTY_TYPE(ScaleParameter, (1.0), "Parameters", App::Prop_None, "The scale parameter");
   ADD_PROPERTY_TYPE(ExponentFactor, (1.0), "Parameters", App::Prop_None, "The Weibull distribution exponential factor");
   ADD_PROPERTY_TYPE(Types, ((long int)0), "CPD Type", App::Prop_None, "The CPD type");

   static const char* types[] = {"Lower tail", "Upper tail", nullptr};
   Types.setEnums(types);
    this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_cdf_weibull_P");

}


bool CWeibullCPD::ComputeCPDValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
    switch (Types.getValue()) {
    case 0:
        dValue = gsl_cdf_weibull_P(x,  ScaleParameter.getValue() /*a*/, ExponentFactor.getValue() /*b*/);
        break;
    case 1:
        dValue = gsl_cdf_weibull_Q(x, ScaleParameter.getValue() /*a*/, ExponentFactor.getValue() /*b*/);
        break;
    default:
        break;
    }

            return true;

}
bool CWeibullCPD::ComputeCPDVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{
     bool returnResult = true;
    for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
       dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
       returnResult = ComputeCPDValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;
}
bool CWeibullCPD::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para1 = dlg->getParametersVector().at(1);
        para2 = dlg->getParametersVector().at(2);

    }

    return true;

}

//
PROPERTY_SOURCE(RandomnessProviders::CGaussianCPD, WindLabAPI::WindLabFeatureCumulativeProbabilityDistribution)

CGaussianCPD::CGaussianCPD()
{
   ADD_PROPERTY_TYPE(Mean, (0.0), "Parameters", App::Prop_None, "The mean value");
   ADD_PROPERTY_TYPE(StandardDeviation, (1.0), "Parameters", App::Prop_None, "The standard deviation");
   ADD_PROPERTY_TYPE(Types, ((long int)0), "CPD Type", App::Prop_None, "The CPD type");

   static const char* types[] = {"Lower tail", "Upper tail", nullptr};
   Types.setEnums(types);
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_cdf_gamma_P");

}
bool CGaussianCPD::ComputeCPDValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
   switch (Types.getValue()) {
    case 0:
        dValue = gsl_cdf_gaussian_P(x, StandardDeviation.getQuantityValue().getValueAs(Base::Quantity::MetrePerSecond) /*a*/) + Mean.getValue() /*mu*/;
        break;
    case 1:
        dValue = gsl_cdf_gaussian_Q(x, StandardDeviation.getQuantityValue().getValueAs(Base::Quantity::MetrePerSecond) /*a*/) + Mean.getValue() /*mu*/;
        break;
    default:
        break;
    }

            return true;

}
bool CGaussianCPD::ComputeCPDVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{
     bool returnResult = true;
    for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
       dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
       returnResult = ComputeCPDValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;

}
bool CGaussianCPD::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
        para1 = dlg->getParametersVector().at(1);

    }

    return true;

}

//
PROPERTY_SOURCE(RandomnessProviders::CExponentialCPD, WindLabAPI::WindLabFeatureCumulativeProbabilityDistribution)

CExponentialCPD::CExponentialCPD()
{
   ADD_PROPERTY_TYPE(Mean, (1.0), "Parameters", App::Prop_None, "The mean or the expected value");
    ADD_PROPERTY_TYPE(Types, ((long int)0), "CPD Type", App::Prop_None, "The CPD type");

    static const char* types[] = {"Lower tail", "Upper tail", nullptr};
    Types.setEnums(types);
    this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_cdf_exponential_P");

}

bool CExponentialCPD::ComputeCPDValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
    switch (Types.getValue()) {
    case 0:
        dValue = gsl_cdf_exponential_P(x, Mean.getValue());
        break;
    case 1:
        dValue = gsl_cdf_exponential_Q(x, Mean.getValue());
        break;
    default:
        break;
    }

            return true;

}
bool CExponentialCPD::ComputeCPDVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{
     bool returnResult = true;
    for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
       dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
       returnResult = ComputeCPDValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;

}
bool CExponentialCPD::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
    }

    return true;
}

//
PROPERTY_SOURCE(RandomnessProviders::CLaplaceCPD, WindLabAPI::WindLabFeatureCumulativeProbabilityDistribution)

CLaplaceCPD::CLaplaceCPD()
{
   ADD_PROPERTY_TYPE(Width, (1.0), "Parameters", App::Prop_None, "The mean or the expected value");
   ADD_PROPERTY_TYPE(Mean, (1.0), "Parameters", App::Prop_None, "The mean or the expected value");
   ADD_PROPERTY_TYPE(Types, ((long int)0), "CPD Type", App::Prop_None, "The CPD type");

   static const char* types[] = {"Lower tail", "Upper tail", nullptr};
   Types.setEnums(types);
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_cdf_laplace_P");

}

bool CLaplaceCPD::ComputeCPDValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
   switch (Types.getValue()) {
    case 0:
        dValue = gsl_cdf_laplace_P(x, Width.getValue() /*a*/) + Mean.getValue() /*mu*/;
        break;
    case 1:
        dValue = gsl_cdf_laplace_Q(x, Width.getValue() /*a*/) + Mean.getValue() /*mu*/;
        break;
    default:
        break;
    }

            return true;

}
bool CLaplaceCPD::ComputeCPDVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{
     bool returnResult = true;
    for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
       dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
       returnResult = ComputeCPDValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;

}
bool CLaplaceCPD::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
        para1 = dlg->getParametersVector().at(1);
    }

    return true;
}

//
PROPERTY_SOURCE(RandomnessProviders::CExponentialPowerCPD, WindLabAPI::WindLabFeatureCumulativeProbabilityDistribution)

CExponentialPowerCPD::CExponentialPowerCPD()
{
   ADD_PROPERTY_TYPE(ScaleParameter, (1.0), "Parameters", App::Prop_None, "The scale parameter");
   ADD_PROPERTY_TYPE(ExponentFactor, (2.5), "Parameters", App::Prop_None, "The exponent factor");
   ADD_PROPERTY_TYPE(Mean, (0.0), "Parameters", App::Prop_None, "The mean value");
   ADD_PROPERTY_TYPE(Types, ((long int)0), "CPD Type", App::Prop_None, "The CPD type");

   static const char* types[] = {"Lower tail", "Upper tail", nullptr};
   Types.setEnums(types);
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_cdf_exppow_P");
}

bool CExponentialPowerCPD::ComputeCPDValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
   switch (Types.getValue()) {
    case 0:
        dValue = gsl_cdf_exppow_P(x, ScaleParameter.getValue() /*a*/, ExponentFactor.getValue() /*b*/) + Mean.getValue() /*mu*/;
        break;
    case 1:
        dValue = gsl_cdf_exppow_P(x, ScaleParameter.getValue() /*a*/, ExponentFactor.getValue() /*b*/) + Mean.getValue() /*mu*/;
        break;
    default:
        break;
    }

            return true;

}
bool CExponentialPowerCPD::ComputeCPDVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{
     bool returnResult = true;
    for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
       dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
       returnResult = ComputeCPDValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;
}

bool CExponentialPowerCPD::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
        para1 = dlg->getParametersVector().at(1);
        para2 = dlg->getParametersVector().at(2);

    }

    return true;
}

//
PROPERTY_SOURCE(RandomnessProviders::CCauchyCPD, WindLabAPI::WindLabFeatureCumulativeProbabilityDistribution)

CCauchyCPD::CCauchyCPD()
{
   ADD_PROPERTY_TYPE(ScaleParameter, (1.0), "Parameters", App::Prop_None, "The scale parameter");
   ADD_PROPERTY_TYPE(Types, ((long int)0), "CPD Type", App::Prop_None, "The CPD type");

   static const char* types[] = {"Lower tail", "Upper tail", nullptr};
   Types.setEnums(types);
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_cdf_cauchy_P");
}

bool CCauchyCPD::ComputeCPDValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
   switch (Types.getValue()) {
    case 0:
        dValue = gsl_cdf_cauchy_P(x, ScaleParameter.getValue() /*a*/);
        break;
    case 1:
        dValue = gsl_cdf_cauchy_Q(x, ScaleParameter.getValue() /*a*/);
        break;
    default:
        break;
    }

    return true;

}
bool CCauchyCPD::ComputeCPDVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{
     bool returnResult = true;
    for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
       dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
       returnResult = ComputeCPDValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;

}
bool CCauchyCPD::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
    }

    return true;
}


//
PROPERTY_SOURCE(RandomnessProviders::CRayleighCPD, WindLabAPI::WindLabFeatureCumulativeProbabilityDistribution)

CRayleighCPD::CRayleighCPD()
{
   ADD_PROPERTY_TYPE(ScaleParameter, (1.0), "Parameters", App::Prop_None, "The scale parameter");
   ADD_PROPERTY_TYPE(Types, ((long int)0), "CPD Type", App::Prop_None, "The CPD type");

   static const char* types[] = {"Lower tail", "Upper tail", nullptr};
   Types.setEnums(types);
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_cdf_rayleigh_P");

}

bool CRayleighCPD::ComputeCPDValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
   switch (Types.getValue()) {
    case 0:
        dValue = gsl_cdf_rayleigh_P(x, ScaleParameter.getValue() /*sigma*/);
        break;
    case 1:
        dValue = gsl_cdf_rayleigh_Q(x, ScaleParameter.getValue() /*sigma*/);
        break;
    default:
        break;
    }

            return true;

}
bool CRayleighCPD::ComputeCPDVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{
     bool returnResult = true;
    for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
       dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
       returnResult = ComputeCPDValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;
}
bool CRayleighCPD::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
    }

    return true;
}

//
PROPERTY_SOURCE(RandomnessProviders::CFlatCPD, WindLabAPI::WindLabFeatureCumulativeProbabilityDistribution)

CFlatCPD::CFlatCPD()
{
    ADD_PROPERTY_TYPE(LowerBound, (0.5), "Parameters", App::Prop_None, "The lower bound limit value");
    ADD_PROPERTY_TYPE(UpperBound, (2.5), "Parameters", App::Prop_None, "The upper bound limit value");
    ADD_PROPERTY_TYPE(Types, ((long int)0), "CPD Type", App::Prop_None, "The CPD type");

    static const char* types[] = {"Lower tail", "Upper tail", nullptr};
    Types.setEnums(types);
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_cdf_flat_P");

}

bool CFlatCPD::ComputeCPDValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
    switch (Types.getValue()) {
    case 0:
        dValue = gsl_cdf_flat_P(x, LowerBound.getValue() /*a*/, UpperBound.getValue() /*b*/);
        break;
    case 1:
        dValue = gsl_cdf_flat_Q(x, LowerBound.getValue() /*a*/, UpperBound.getValue() /*b*/);
        break;
    default:
        break;
    }

    return true;

}
bool CFlatCPD::ComputeCPDVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{
    bool returnResult = true;
    for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
       dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
       returnResult = ComputeCPDValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;

}
bool CFlatCPD::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
        para1 = dlg->getParametersVector().at(1);

    }

    return true;
}


//
PROPERTY_SOURCE(RandomnessProviders::CGammaCPD, WindLabAPI::WindLabFeatureCumulativeProbabilityDistribution)

CGammaCPD::CGammaCPD()
{
    ADD_PROPERTY_TYPE(ParameterA, (0.5), "Parameters", App::Prop_None, "The A parameter");
    ADD_PROPERTY_TYPE(ParameterB, (2.5), "Parameters", App::Prop_None, "The B parameter");
    ADD_PROPERTY_TYPE(Types, ((long int)0), "CPD Type", App::Prop_None, "The CPD type");

    static const char* types[] = {"Lower tail", "Upper tail", nullptr};
    Types.setEnums(types);
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_cdf_gamma_P");

}

bool CGammaCPD::ComputeCPDValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
    switch (Types.getValue()) {
    case 0:
        dValue = gsl_cdf_gamma_P(x, ParameterA.getValue() /*a*/, ParameterB.getValue() /*b*/);
    case 1:
        dValue = gsl_cdf_gamma_Q(x, ParameterA.getValue() /*a*/, ParameterB.getValue() /*b*/);
        break;
    default:
        break;
    }

            return true;

}
bool CGammaCPD::ComputeCPDVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{
     bool returnResult = true;
    for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
       dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
       returnResult = ComputeCPDValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;
}
bool CGammaCPD::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
        para1 = dlg->getParametersVector().at(1);
    }

    return true;
}


//
PROPERTY_SOURCE(RandomnessProviders::CLognormalCPD, WindLabAPI::WindLabFeatureCumulativeProbabilityDistribution)

CLognormalCPD::CLognormalCPD()
{
   ADD_PROPERTY_TYPE(Mean, (0.0), "Parameters", App::Prop_None, "The mean value");
   ADD_PROPERTY_TYPE(StandardDeviation, (1.0), "Parameters", App::Prop_None, "The standard deviation");
   ADD_PROPERTY_TYPE(Types, ((long int)0), "CPD Type", App::Prop_None, "The CPD type");

    static const char* types[] = {"Lower tail", "Upper tail", nullptr};
    Types.setEnums(types);
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_cdf_lognormal_P");

}
bool CLognormalCPD::ComputeCPDValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
    switch (Types.getValue()) {
    case 0:
        dValue = gsl_cdf_lognormal_P(x, Mean.getValue() /*zeta*/, StandardDeviation.getQuantityValue().getValueAs(Base::Quantity::MetrePerSecond) /*sigma*/);
    case 1:
        dValue = gsl_cdf_lognormal_Q(x, Mean.getValue() /*zeta*/, StandardDeviation.getQuantityValue().getValueAs(Base::Quantity::MetrePerSecond) /*sigma*/);
        break;
    default:
        break;
    }

            return true;

}
bool CLognormalCPD::ComputeCPDVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{
     bool returnResult = true;
    for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
       dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
       returnResult = ComputeCPDValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;
}
bool CLognormalCPD::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
        para1 = dlg->getParametersVector().at(1);
    }

    return true;
}


//
PROPERTY_SOURCE(RandomnessProviders::CChiSquaredCPD, WindLabAPI::WindLabFeatureCumulativeProbabilityDistribution)

CChiSquaredCPD::CChiSquaredCPD()
{
    ADD_PROPERTY_TYPE(DegreeOfFreedom, (1.0), "Parameters", App::Prop_None, "The degree of freedom");
    ADD_PROPERTY_TYPE(Types, ((long int)0), "CPD Type", App::Prop_None, "The CPD type");

    static const char* types[] = {"Lower tail", "Upper tail", nullptr};
    Types.setEnums(types);
   this->LinkToWiki.setValue("https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_cdf_chisq_P");

}

bool CChiSquaredCPD::ComputeCPDValue(const WindLabAPI::WindLabSimulationData& Data, const double &x, double &dValue)
{
    switch (Types.getValue()) {
    case 0:
        dValue = gsl_cdf_chisq_P(x, DegreeOfFreedom.getValue() /*nu*/);
    case 1:
        dValue = gsl_cdf_chisq_Q(x, DegreeOfFreedom.getValue() /*nu*/);
        break;
    default:
        break;
    }

            return true;

}
bool CChiSquaredCPD::ComputeCPDVectorX(const WindLabAPI::WindLabSimulationData& Data, vec &dVarVector, vec &dValVector)
{
    bool returnResult = true;
    for (int j = 0 ; j < Data.numberOfIncrementOfVariableX.getValue() && false == Data.isInterruptionRequested.getValue() && true == returnResult; j++)
    {
       dVarVector(j) = Data.minVariableX.getValue() + j * Data.incrementOfVariableX.getValue();
       returnResult = ComputeCPDValue(Data, dVarVector(j), dValVector(j));    
    }

    return returnResult;

}
bool CChiSquaredCPD::OnInitialSetting(const WindLabAPI::WindLabSimulationData& Data)
{
    std::unique_ptr<RandomDistributionDialog> dlg(new RandomDistributionDialog());
    if (dlg->exec() == QDialog::Accepted)
    {
        para0 = dlg->getParametersVector().at(0);
    }

    return true;
}

